// Copyright (c) 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_CORE_HERQULES_INLINE_H_
#define MOJO_CORE_HERQULES_INLINE_H_

#include <fcntl.h>
#include <linux/futex.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/syscall.h>

#include "base/logging.h"
#include "base/memory/unsafe_shared_memory_region.h"
#include "mojo/core/channel.h"
#include "mojo/core/herqules.h"

// #include <internal/pthread_impl.h>
#define _m_lock __u.__vi[1]

namespace mojo {
namespace core {

// Randomly-chosen identifier
constexpr uint32_t kHerQulesHandshakeID = 0x3E6907E8;
constexpr unsigned kHerQulesHandshakeNumHandles = 1;
constexpr size_t kHerQulesHandshakeSize =
    sizeof(Channel::Message::Header) + sizeof(kHerQulesHandshakeID) +
    sizeof(Channel::Message::HerQulesExtraHeader) +
    kHerQulesHandshakeNumHandles * sizeof(Channel::Message::HerQulesEntry);

constexpr HerQulesStatus kHerQulesFull = (1U << 31);

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
       ~(kChannelMessageAlignment - 1)) != header_size) {
    LOG(ERROR) << "Message metadata and size mismatch!";
    return false;
  }

  handles->resize(num_handles);
  for (size_t i = 0; i < num_handles; ++i) {
    int recv_fd = -1;

    if (fcntl(fd, F_POPFD, &recv_fd)) {
      PLOG(ERROR) << "Cannot pop from file descriptor " << fd;
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
      LOG(ERROR) << "Unexpected handle type!";
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
    LOG(ERROR) << "Handshake metadata mismatch!";
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
  header->status_ = 0;
  if (syscall(SYS_futex, &header->status_, FUTEX_WAKE, INT_MAX, nullptr,
              nullptr, 0) < 0)
    PLOG(ERROR) << "Cannot wake write futex!";
}

static inline void HerQulesInit(struct HerQulesShmHdr* header) {
  header->status_ = 0;

  // Set the mutex robust and keep it always locked
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
  pthread_mutex_init(&header->mutex_, &attr);
  pthread_mutexattr_destroy(&attr);
  pthread_mutex_lock(&header->mutex_);
}

static inline void HerQulesLock(struct HerQulesShmHdr* header) {
  while (base::subtle::Acquire_CompareAndSwap(&header->lock_, 0, 1)) {
  }
}

// Requires lock
static inline void HerQulesUnlock(struct HerQulesShmHdr* header) {
  DCHECK(base::subtle::Acquire_Load(&header->lock_) == 1);
  base::subtle::Release_Store(&header->lock_, 0);
}

static inline void HerQulesDestroy(struct HerQulesShmHdr* header) {
  if (syscall(SYS_futex, &header->status_, FUTEX_WAKE, INT_MAX, nullptr, nullptr,
              0) < 0)
    PLOG(ERROR) << "Cannot wake write futex!";

  // Unlock to avoid SIGSEGV in robust_list after thread is unmapped
  pthread_mutex_unlock(&header->mutex_);
  pthread_mutex_destroy(&header->mutex_);
  // Permanently set the died bit
  header->mutex_._m_lock |= FUTEX_OWNER_DIED;
}

static inline uint8_t* HerQulesGetMessage(struct HerQulesShmHdr* header,
                                          HerQulesStatus offset) {
  return reinterpret_cast<uint8_t*>(header) + sizeof(*header) + offset;
}

static inline HerQulesStatus HerQulesGetStatus(
    const struct HerQulesShmHdr* header) {
  return header->status_;
}

static inline HerQulesStatus HerQulesIsFull(const HerQulesStatus status) {
  return status & kHerQulesFull;
}

static inline HerQulesStatus HerQulesGetWrite(const HerQulesStatus status) {
  return status & ~kHerQulesFull;
}

static inline auto HerQulesGetClosed(const struct HerQulesShmHdr* header) {
  return &header->mutex_._m_lock;
}

static inline bool HerQulesIsClosed(int status) {
  return status & FUTEX_OWNER_DIED;
}

static inline HerQulesStatus HerQulesSetFull(struct HerQulesShmHdr* header) {
  auto status = (header->status_ |= (1U << 31));
  if (syscall(SYS_futex, &header->status_, FUTEX_WAKE, INT_MAX, nullptr,
              nullptr, 0) < 0)
    PLOG(ERROR) << "Cannot wake write futex!";
  return status;
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
      int push_fd = handle.TakeHandle().TakeFD().release();
      if (fcntl(fd, F_PUSHFD, push_fd)) {
        PLOG(ERROR) << "Cannot push to file descriptor " << fd;
        return false;
      }
    }
  }

  return true;
}

// Requires lock
static inline void HerQulesUpdateSend(struct HerQulesShmHdr* header,
                                      HerQulesStatus sz) {
  header->status_ += sz;
  if (syscall(SYS_futex, &header->status_, FUTEX_WAKE, INT_MAX, nullptr, nullptr,
              0) < 0)
    PLOG(ERROR) << "Cannot wake write futex!";
}

static inline void HerQulesWaitWrite(struct HerQulesShmHdr* header,
                                     const HerQulesStatus val,
                                     const struct timespec* timeout) {
  if (syscall(SYS_futex, &header->status_, FUTEX_WAIT | FUTEX_CLOCK_REALTIME,
              val, timeout, nullptr, 0) &&
      errno != EAGAIN)
    PLOG(ERROR) << "Cannot wait on write futex!";
}

static inline bool HerQulesResetFull(struct HerQulesShmHdr* header,
                                     HerQulesStatus& status,
                                     HerQulesStatus& read) {
  HerQulesLock(header);
  status = HerQulesGetWrite(HerQulesGetStatus(header));
  bool ret = status == read;
  if (ret) {
    HerQulesReset(header);
    status = 0;
    read = 0;
  }
  HerQulesUnlock(header);
  return ret;
}

}  // namespace core
}  // namespace mojo

#endif
