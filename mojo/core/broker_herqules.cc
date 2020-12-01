// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo/core/broker.h"

#include <fcntl.h>

#include <utility>
#include <vector>

#include "base/files/scoped_file.h"
#include "base/logging.h"
#include "build/build_config.h"
#include "mojo/core/broker_messages.h"
#include "mojo/core/channel.h"
#include "mojo/core/herqules_inline.h"
#include "mojo/core/platform_handle_utils.h"

namespace mojo {
namespace core {

namespace {
bool WaitForMessage(HerQulesShmHdr* header,
                    HerQulesStatus& status,
                    HerQulesStatus& read) {
  while (1) {
    while ((status = HerQulesGetStatus(header)) == read) {
      auto val = *HerQulesGetClosed(header);
      if (HerQulesIsClosed(val)) {
        LOG(ERROR) << "Channel destroyed while broker waiting!";
        return false;
      }

      HerQulesWaitWrite(header, read, nullptr);
    }

    // New message available if not full or full and reset failed
    if (!HerQulesIsFull(status) || !HerQulesResetFull(header, status, read))
      break;
  }

  return true;
}

Channel::MessagePtr WaitForBrokerMessage(
    int fd,
    base::WritableSharedMemoryMapping& ro_mapping,
    HerQulesStatus& read,
    BrokerMessageType expected_type,
    size_t expected_num_handles,
    size_t expected_data_size,
    std::vector<PlatformHandle>* handles) {
  DCHECK(ro_mapping.IsValid());

  HerQulesStatus status;
  HerQulesShmHdr* header = ro_mapping.GetMemoryAs<HerQulesShmHdr>();
  if (!WaitForMessage(header, status, read))
    return nullptr;

  Channel::MessagePtr message = Channel::Message::Deserialize(
      HerQulesGetMessage(header, read), status - read);
  if (!message) {
    LOG(ERROR) << "Invalid broker channel message!";
    return nullptr;
  }
  read += message->data_num_bytes();

  const BrokerMessageHeader* broker_header =
      reinterpret_cast<const BrokerMessageHeader*>(message->payload());
  if (message->payload_size() !=
          sizeof(BrokerMessageHeader) + expected_data_size ||
      broker_header->type != expected_type) {
    LOG(ERROR) << "Invalid broker channel message size!";
    return nullptr;
  }

  if (!HerQulesParseMessageHandles(
          fd,
          reinterpret_cast<const Channel::Message::HerQulesExtraHeader*>(
              message->extra_header()),
          message->extra_header_size(), expected_num_handles, handles)) {
    LOG(ERROR) << "Cannot parse message handles!";
    return nullptr;
  }

  return message;
}

bool WaitForHandshake(int fd,
                      base::WritableSharedMemoryMapping& ro_mapping,
                      HerQulesStatus& read,
                      base::UnsafeSharedMemoryRegion& wr_region,
                      base::WritableSharedMemoryMapping& wr_mapping) {
  HerQulesStatus status;
  HerQulesShmHdr* header = ro_mapping.GetMemoryAs<HerQulesShmHdr>();
  if (!WaitForMessage(header, status, read))
    return false;

  std::vector<PlatformHandle> handles(kHerQulesHandshakeNumHandles);
  Channel::MessagePtr msg = HerQulesParseHandshake(
      fd, HerQulesGetMessage(header, read), status - read, &handles);
  if (!msg) {
    LOG(ERROR) << "Received invalid handshake message!";
    return false;
  }

  read += msg->data_num_bytes();
  wr_region = HerQulesCreateTx(handles[0].TakeFD());
  wr_mapping = HerQulesMap(wr_region);
  if (!wr_mapping.IsValid()) {
    LOG(ERROR) << "Cannot map TX memory region!";
    return false;
  }

  return true;
}

bool SendMessage(int fd,
                 base::WritableSharedMemoryMapping& wr_mapping,
                 Channel::MessagePtr message) {
  bool ret = true;
  HerQulesStatus status;
  HerQulesShmHdr* header = wr_mapping.GetMemoryAs<HerQulesShmHdr>();
  const auto max_len = wr_mapping.size() - sizeof(*header),
             size = message->data_num_bytes();

  HerQulesLock(header);
  while (1) {
    status = HerQulesGetStatus(header);
    auto full = HerQulesIsFull(status);

    if (!full) {
      if (status + size <= max_len)
        break;

      if (!status) {
        LOG(ERROR) << "Message of " << size << " cannot fit in buffer of "
                   << max_len;
        ret = false;
        break;
      }

      status = HerQulesSetFull(header);
    }

    HerQulesUnlock(header);
    HerQulesWaitWrite(header, status, nullptr);
    HerQulesLock(header);
  };

  if (HerQulesSend(fd, HerQulesGetMessage(header, status),
                   std::move(message))) {
    HerQulesUpdateSend(header, size);
  } else {
    LOG(ERROR) << "Cannot send message!";
    ret = false;
  }
  HerQulesUnlock(header);

  return ret;
}

}  // namespace

Broker::Broker(PlatformHandle handle, bool wait_for_channel_handle)
    : sync_channel_(std::move(handle)) {
  CHECK(sync_channel_.is_valid());

  if (!sync_channel_.is_shm_rx_fd()) {
    LOG(ERROR) << "Constructor expects RX file descriptor!";
    return;
  }

  ro_region_ = HerQulesCreateRx(sync_channel_.TakeFD());
  ro_mapping_ = HerQulesMap(ro_region_);
  if (!ro_mapping_.IsValid()) {
    LOG(ERROR) << "Cannot map RX memory region!";
    return;
  }
  read_ = 0;

  const auto fd = ro_region_.GetPlatformHandle().fd;
  if (!WaitForHandshake(fd, ro_mapping_, read_, wr_region_, wr_mapping_) ||
      !wait_for_channel_handle)
    return;

  // Wait for the first message, which should contain a handle.
  std::vector<PlatformHandle> handles;
  if (WaitForBrokerMessage(fd, ro_mapping_, read_, BrokerMessageType::INIT, 1,
                           0, &handles)) {
    inviter_endpoint_ = PlatformChannelEndpoint(std::move(handles[0]));
  }
}

Broker::~Broker() {
  // Destroy data structures if we are the writer
  if (wr_mapping_.IsValid()) {
    HerQulesDestroy(wr_mapping_.GetMemoryAs<HerQulesShmHdr>());
  }
}

PlatformChannelEndpoint Broker::GetInviterEndpoint() {
  return std::move(inviter_endpoint_);
}

base::WritableSharedMemoryRegion Broker::GetWritableSharedMemoryRegion(
    size_t num_bytes) {
  DCHECK(ro_mapping_.IsValid() && wr_mapping_.IsValid());
  base::AutoLock lock(lock_);

  BufferRequestData* buffer_request;
  Channel::MessagePtr out_message = CreateBrokerMessage(
      BrokerMessageType::BUFFER_REQUEST, 0, 0, &buffer_request);
  buffer_request->size = num_bytes;
  SendMessage(wr_region_.GetPlatformHandle().fd, wr_mapping_,
              std::move(out_message));

  std::vector<PlatformHandle> handles;
  Channel::MessagePtr message =
      WaitForBrokerMessage(ro_region_.GetPlatformHandle().fd, ro_mapping_,
                           read_, BrokerMessageType::BUFFER_RESPONSE, 2,
                           sizeof(BufferResponseData), &handles);
  if (message) {
    const BufferResponseData* data;
    if (!GetBrokerMessageData(message.get(), &data))
      return base::WritableSharedMemoryRegion();

    return base::WritableSharedMemoryRegion::Deserialize(
        base::subtle::PlatformSharedMemoryRegion::Take(
            CreateSharedMemoryRegionHandleFromPlatformHandles(
                std::move(handles[0]), std::move(handles[1])),
            base::subtle::PlatformSharedMemoryRegion::Mode::kWritable,
            num_bytes,
            base::UnguessableToken::Deserialize(data->guid_high,
                                                data->guid_low)));
  }

  return base::WritableSharedMemoryRegion();
}

}  // namespace core
}  // namespace mojo
