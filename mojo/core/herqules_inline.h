// Copyright (c) 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_CORE_HERQULES_INLINE_H_
#define MOJO_CORE_HERQULES_INLINE_H_

#include <fcntl.h>
#include <sys/mman.h>

#include "base/memory/unsafe_shared_memory_region.h"
#include "mojo/core/channel.h"
#include "mojo/core/herqules.h"

namespace mojo {
namespace core {

const unsigned F_PUSHFD = 100;
const unsigned F_PUSHFD_CLOEXEC = 101;
const unsigned F_POPFD = 102;

// Randomly-chosen identifier
constexpr uint32_t kHerQulesHandshakeID = 0x3E6907E8;
constexpr unsigned kHerQulesHandshakeNumHandles = 1;
constexpr size_t kHerQulesHandshakeSize =
    sizeof(Channel::Message::Header) + sizeof(kHerQulesHandshakeID) +
    sizeof(Channel::Message::HerQulesExtraHeader) +
    kHerQulesHandshakeNumHandles * sizeof(Channel::Message::HerQulesEntry);

static inline base::UnsafeSharedMemoryRegion HerQulesCreateRx(
    base::ScopedFD fd) {
  return base::UnsafeSharedMemoryRegion::Deserialize(
      base::subtle::PlatformSharedMemoryRegion::Take(
          std::move(fd),
          base::subtle::PlatformSharedMemoryRegion::Mode::kUnsafe,
          kHerQulesBufferSize, base::UnguessableToken::Create()));
}

static inline base::UnsafeSharedMemoryRegion HerQulesCreateTx(
    base::ScopedFD fd) {
  return base::UnsafeSharedMemoryRegion::Deserialize(
      base::subtle::PlatformSharedMemoryRegion::Take(
          std::move(fd),
          base::subtle::PlatformSharedMemoryRegion::Mode::kUnsafe,
          kHerQulesBufferSize, base::UnguessableToken::Create()));
}

static inline base::WritableSharedMemoryMapping HerQulesMap(
    base::UnsafeSharedMemoryRegion& region) {
  auto map = region.Map();
  if (map.IsValid())
    madvise(map.memory(), map.size(), MADV_DONTFORK);
  return map;
}

static inline Channel::MessagePtr HerQulesCreateHandshake(
    PlatformHandle handle) {
  std::vector<PlatformHandle> handles(kHerQulesHandshakeNumHandles);
  handles[0] = std::move(handle);
  Channel::MessagePtr msg(new Channel::Message(sizeof(kHerQulesHandshakeID),
                                               kHerQulesHandshakeNumHandles));
  reinterpret_cast<std::remove_cv_t<decltype(kHerQulesHandshakeID)>*>(
      msg->mutable_payload())[0] = kHerQulesHandshakeID;
  msg->SetHandles(std::move(handles));
  DCHECK(msg->data_num_bytes() == kHerQulesHandshakeSize);
  return msg;
}

static inline bool HerQulesParseMessageHandles(
    int fd,
    const Channel::Message::HerQulesExtraHeader* header,
    const size_t header_size,
    const size_t num_handles,
    std::vector<PlatformHandle>* handles) {
  if (!header || num_handles != header->num_fds ||
      ((sizeof(*header) + num_handles * sizeof(*header->entries) +
        (kChannelMessageAlignment - 1)) &
       ~(kChannelMessageAlignment - 1)) != header_size)
    return false;

  handles->resize(num_handles);
  for (size_t i = 0; i < num_handles; ++i) {
    int recv_fd = -1;

    if (fcntl(fd, F_POPFD, &recv_fd)) {
      return false;
    }

    auto type =
        static_cast<mojo::PlatformHandle::Type>(header->entries[i].type);
    if (type == PlatformHandle::Type::kFd) {
      (*handles)[i] = PlatformHandle(base::ScopedFD(recv_fd));
    } else if (type == PlatformHandle::Type::kFdShmRx) {
      (*handles)[i] = PlatformHandle(base::ScopedFD(recv_fd), true);
    } else if (type == PlatformHandle::Type::kFdShmTx) {
      (*handles)[i] = PlatformHandle(base::ScopedFD(recv_fd), false);
    } else {
      return false;
    }
  }

  return true;
}

static inline Channel::MessagePtr HerQulesParseHandshake(
    int fd,
    const void* memory,
    size_t max_len,
    std::vector<PlatformHandle>* handles) {
  Channel::MessagePtr msg = Channel::Message::Deserialize(
      memory, std::min(kHerQulesHandshakeSize, max_len));
  if (!msg || msg->payload_size() != sizeof(kHerQulesHandshakeID) ||
      msg->num_handles() != kHerQulesHandshakeNumHandles) {
    return nullptr;
  }

  if (!HerQulesParseMessageHandles(
          fd,
          reinterpret_cast<const Channel::Message::HerQulesExtraHeader*>(
              msg->extra_header()),
          msg->extra_header_size(), kHerQulesHandshakeNumHandles, handles)) {
    return nullptr;
  }

  return msg;
}

// Requires lock
static inline void HerQulesReset(struct HerQulesShmHdr* header) {
  header->write_ = 0;
  header->full_ = false;
}

static inline void HerQulesInit(struct HerQulesShmHdr* header) {
  pthread_mutex_init(&header->mutex_, nullptr);
  // pthread_cond_init(&header->cond_, nullptr);
  HerQulesReset(header);
  header->close_ = false;
}

static inline void HerQulesDestroy(struct HerQulesShmHdr* header) {
  // pthread_cond_signal(&header->cond_);
  pthread_mutex_destroy(&header->mutex_);
  // pthread_cond_destroy(&header->cond_);
  DCHECK(!header->close_);
  header->close_ = true;
  header = nullptr;
}

static inline void HerQulesLock(struct HerQulesShmHdr* header) {
  pthread_mutex_lock(&header->mutex_);
}

// Requires lock
static inline void HerQulesUnlock(struct HerQulesShmHdr* header) {
  pthread_mutex_unlock(&header->mutex_);
}

static inline uint8_t* HerQulesGetMessage(struct HerQulesShmHdr* header,
                                          size_t offset) {
  return reinterpret_cast<uint8_t*>(header) + sizeof(*header) + offset;
}

static inline size_t HerQulesGetWrite(const struct HerQulesShmHdr* header) {
  return header->write_;
}

static inline bool HerQulesIsClosed(const struct HerQulesShmHdr* header) {
  return header->close_;
}

// Requires lock
static inline bool HerQulesMaySend(struct HerQulesShmHdr* header,
                                   size_t sz,
                                   size_t total) {
  const size_t write = HerQulesGetWrite(header);
  if (sizeof(*header) + write + sz >= total) {
    header->full_ = true;
    return false;
  }

  return true;
}

static inline void HerQulesWake(struct HerQulesShmHdr* header) {
  // pthread_cond_signal(&header->signal_);
}

// Requires lock
static inline bool HerQulesSend(int fd,
                                uint8_t* memory,
                                Channel::MessagePtr message) {
  const auto sz = message->data_num_bytes();
  memcpy(memory, message->data(), sz);

  if (message->has_handles()) {
    std::vector<PlatformHandleInTransit> handles = message->TakeHandles();

    for (auto& handle : handles) {
      DCHECK(handle.handle().is_valid());
      if (fcntl(fd, F_PUSHFD, handle.TakeHandle().TakeFD().release())) {
        return false;
      }
    }
  }

  return true;
}

// Requires lock
static inline void HerQulesUpdateSend(struct HerQulesShmHdr* header,
                                      size_t sz) {
  header->write_ += sz;
  HerQulesWake(header);
}

// Requires lock
static inline void HerQulesWait(struct HerQulesShmHdr* header,
                                struct timespec* timeout) {
  // HerQulesLock(header);
  // pthread_cond_wait(header->cond_, timeout);
  // HerQulesUnlock(header);
}

static inline void HerQulesWaitRead(struct HerQulesShmHdr* header,
                                    const size_t read) {
  while (HerQulesGetWrite(header) == read)
    HerQulesWait(header, nullptr);
}

static inline void HerQulesResetIfFull(struct HerQulesShmHdr* header,
                                       size_t& read) {
  if (header->full_) {
    HerQulesLock(header);

    if (HerQulesGetWrite(header) == read) {
      read = 0;
      HerQulesReset(header);
      HerQulesWake(header);
    }

    HerQulesUnlock(header);
  }
}

}  // namespace core
}  // namespace mojo

#endif
