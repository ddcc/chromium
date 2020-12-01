// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mojo/core/channel.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>

#include <algorithm>
#include <atomic>
#include <limits>
#include <memory>

#include "base/bind.h"
#include "base/containers/queue.h"
#include "base/location.h"
#include "base/logging.h"
#include "base/macros.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_pump_for_io.h"
#include "base/metrics/histogram_macros.h"
#include "base/synchronization/lock.h"
#include "base/task/current_thread.h"
#include "base/task_runner.h"
#include "base/time/time.h"
#include "build/build_config.h"
#include "mojo/core/channel.h"
#include "mojo/core/core.h"
#include "mojo/core/herqules_inline.h"
#include "mojo/public/cpp/platform/platform_channel.h"

namespace mojo {
namespace core {

namespace {

class ChannelHerQules : public Channel,
                        public base::CurrentThread::DestructionObserver,
                        public base::MessagePumpForSHM::ShmWatcher {
 public:
  ChannelHerQules(Delegate* delegate,
                  ConnectionParams connection_params,
                  HandlePolicy handle_policy,
                  scoped_refptr<base::SingleThreadTaskRunner> shm_task_runner)
      : Channel(delegate, handle_policy, DispatchBufferPolicy::kUnmanaged),
        self_(this),
        shm_task_runner_(shm_task_runner) {
    PlatformHandle handle;
    if (connection_params.server_endpoint().is_valid()) {
      handle = connection_params.TakeServerEndpoint().TakePlatformHandle();
    } else {
      handle = connection_params.TakeEndpoint().TakePlatformHandle();
    }

    if (handle.is_shm_tx_fd()) {
      send_ = handle.TakeFD();
    } else if (handle.is_shm_rx_fd()) {
      receive_ = handle.TakeFD();
    } else {
      NOTREACHED();
    }
  }

  void Start() override {
    if (shm_task_runner_->RunsTasksInCurrentSequence()) {
      StartOnSHMThread();
    } else {
      shm_task_runner_->PostTask(
          FROM_HERE, base::BindOnce(&ChannelHerQules::StartOnSHMThread, this));
    }
  }

  void ShutDownImpl() override {
    // Always shut down asynchronously when called through the public interface.
    shm_task_runner_->PostTask(
        FROM_HERE, base::BindOnce(&ChannelHerQules::ShutDownOnSHMThread, this));
  }

  void Write(MessagePtr message) override {
    UMA_HISTOGRAM_COUNTS_100000("Mojo.Channel.WriteMessageSize",
                                message->data_num_bytes());
    UMA_HISTOGRAM_COUNTS_100("Mojo.Channel.WriteMessageHandles",
                             message->NumHandlesForTransit());

    bool queued = false;
    {
      base::AutoLock lock(write_lock_);
      queued = !outgoing_messages_.empty();
      outgoing_messages_.emplace_back(std::move(message));
    }

    UMA_HISTOGRAM_BOOLEAN("Mojo.Channel.WriteQueued", queued);
  }

  void LeakHandle() override {
    DCHECK(shm_task_runner_->RunsTasksInCurrentSequence());
    leak_handle_ = true;
  }

  bool GetReadPlatformHandles(const void* payload,
                              size_t payload_size,
                              size_t num_handles,
                              const void* extra_header,
                              size_t extra_header_size,
                              std::vector<PlatformHandle>* handles,
                              bool* deferred) override {
    if (num_handles > std::numeric_limits<uint16_t>::max())
      return false;

    if (!HerQulesParseMessageHandles(
            receive_region_.GetPlatformHandle().fd,
            reinterpret_cast<const Message::HerQulesExtraHeader*>(extra_header),
            extra_header_size, num_handles, handles)) {
      LOG(ERROR) << "Cannot parse message handles!";
      return false;
    }

    return true;
  }

 private:
  ~ChannelHerQules() override {
    DCHECK(!write_watcher_);
    DCHECK(!read_watcher_);
  }

  void StartOnSHMThread() {
    DCHECK(!write_watcher_);
    DCHECK(!read_watcher_);

    // Each channel is unidirectional, so a new channel pair is created and sent
    // via the handshake. See the task port implementation for Mac.
    // Note: Even read-only channels must be opened writable to support locking
    // and circular buffer reset
    if (send_.is_valid()) {
      PlatformChannel channel;
      shm_task_runner_->PostTask(
          FROM_HERE, base::BindOnce(&ChannelHerQules::InitSend, this));
      DCHECK(!receive_.is_valid());
      receive_ = channel.TakeLocalEndpoint().TakePlatformHandle().TakeFD();
      shm_task_runner_->PostTask(
          FROM_HERE, base::BindOnce(&ChannelHerQules::InitReceive, this));
      Write(HerQulesCreateHandshake(
          channel.TakeRemoteEndpoint().TakePlatformHandle()));
      handshake_done_ = true;
    } else if (receive_.is_valid()) {
      // Initialize asynchronously to avoid recursive lock on |lock_|
      // e.g. while processing BIND_BROKER_HOST message
      shm_task_runner_->PostTask(
          FROM_HERE, base::BindOnce(&ChannelHerQules::InitReceive, this));
    } else {
      NOTREACHED();
    }

    base::CurrentThread::Get()->AddDestructionObserver(this);
  }

  void ShutDownOnSHMThread() {
    base::CurrentThread::Get()->RemoveDestructionObserver(this);

    write_watcher_.reset();
    read_watcher_.reset();
    if (leak_handle_) {
      // FIXME: Leak from the regions?
      ignore_result(send_.release());
      ignore_result(receive_.release());
    }

    // May destroy the |this| if it was the last reference.
    self_ = nullptr;
  }

  void InitReceive() {
    read_watcher_.reset(
        new base::MessagePumpForSHM::ShmWatchController(FROM_HERE));
    receive_region_ = HerQulesCreateRx(std::move(receive_));
    if (!base::CurrentSHMThread::Get()->WatchMemoryRegion(
            receive_region_, base::MessagePumpForSHM::WATCH_READ,
            read_watcher_.get(), this))
      OnError(Error::kConnectionFailed);
  }

  void InitSend() {
    write_watcher_.reset(
        new base::MessagePumpForSHM::ShmWatchController(FROM_HERE));
    send_region_ = HerQulesCreateTx(std::move(send_));
    if (!base::CurrentSHMThread::Get()->WatchMemoryRegion(
            send_region_, base::MessagePumpForSHM::WATCH_WRITE,
            write_watcher_.get(), this))
      OnError(Error::kConnectionFailed);
  }

  // base::CurrentThread::DestructionObserver:
  void WillDestroyCurrentMessageLoop() override {
    DCHECK(shm_task_runner_->RunsTasksInCurrentSequence());
    if (self_)
      ShutDownOnSHMThread();
  }

  // base::MessagePumpForSHM::ShmWatcher:
  ssize_t OnShmDoRead(int fd, const uint8_t* memory, size_t max_len) override {
    size_t total = max_len, msg_size;
    bool error = false;

    if (!memory) {
      OnError(Error::kDisconnected);
      return -1;
    }

    while (max_len) {
      if (handshake_done_) {
        DispatchResult result = TryDispatchMessage(
            base::make_span(reinterpret_cast<const char*>(memory), max_len),
            &msg_size);
        if (result != DispatchResult::kOK) {
          LOG(ERROR) << "Received invalid message!";
          // Messages should never be fragmented or otherwise erroneous
          error = true;
          break;
        }
      } else {
        std::vector<PlatformHandle> handles;
        MessagePtr msg = HerQulesParseHandshake(fd, memory, max_len, &handles);
        if (!msg) {
          LOG(ERROR) << "Received invalid handshake message!";
          error = true;
          break;
        }

        msg_size = msg->data_num_bytes();

        // Initialize asynchronously to avoid recursive lock on |lock_|
        DCHECK(!send_.is_valid());
        send_ = handles[0].TakeFD();
        shm_task_runner_->PostTask(
            FROM_HERE, base::BindOnce(&ChannelHerQules::InitSend, this));
        handshake_done_ = true;
      }

      memory += msg_size;
      max_len -= msg_size;
    }

    if (error) {
      OnError(Error::kReceivedMalformedData);
      return -1;
    }

    return total;
  }

  // base::MessagePumpForSHM::ShmWatcher:
  ssize_t OnShmDoWrite(int fd,
                       uint8_t* memory,
                       size_t max_len,
                       bool& full,
                       bool empty) override {
    bool error = false;
    size_t written = 0;

    {
      base::AutoLock lock(write_lock_);

      while (outgoing_messages_.size()) {
        auto sz = outgoing_messages_.front()->data_num_bytes();
        if (written + sz > max_len) {
          if (!written && empty) {
            LOG(ERROR) << "Cannot fit message of size " << sz << "!";
            error = true;
            break;
          }

          full = true;
          break;
        }

        bool ret =
            HerQulesSend(fd, memory, std::move(outgoing_messages_.front()));
        outgoing_messages_.pop_front();
        if (!ret) {
          LOG(ERROR) << "Cannot send message!";
          error = true;
          break;
        }

        written += sz;
        memory += sz;
      }
    }

    if (error) {
      OnError(Error::kDisconnected);
      return -1;
    }

    return written;
  }

  // Keeps the Channel alive at least until explicit shutdown on the SHM thread.
  scoped_refptr<Channel> self_;

  scoped_refptr<base::SingleThreadTaskRunner> shm_task_runner_;

  // The shared memory file handles over which to communicate. Only valid after
  // constructor and before StartOnShmThread(), or during handshake.
  base::ScopedFD send_, receive_;
  // Memory regions for shared memory file handles. Only valid after
  // StartOnShmThread().
  base::UnsafeSharedMemoryRegion send_region_, receive_region_;

  // Whether or not the channel-internal handshake, which establishes bi-
  // directional communication, is complete.
  bool handshake_done_ = false;

  // These watchers must only be accessed on the SHM thread.
  std::unique_ptr<base::MessagePumpForSHM::ShmWatchController> write_watcher_,
      read_watcher_;

  // Protects |outgoing_messages_|.
  base::Lock write_lock_;
  base::circular_deque<MessagePtr> outgoing_messages_;

  bool leak_handle_ = false;

  DISALLOW_COPY_AND_ASSIGN(ChannelHerQules);
};  // namespace

}  // namespace

// static
scoped_refptr<Channel> Channel::Create(
    Delegate* delegate,
    ConnectionParams connection_params,
    HandlePolicy handle_policy,
    scoped_refptr<base::SingleThreadTaskRunner> shm_task_runner) {
  return new ChannelHerQules(delegate, std::move(connection_params),
                             handle_policy, shm_task_runner);
}

}  // namespace core
}  // namespace mojo
