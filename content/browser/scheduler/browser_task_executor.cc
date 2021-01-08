// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/scheduler/browser_task_executor.h"

#include <atomic>

#include "base/bind.h"
#include "base/deferred_sequenced_task_runner.h"
#include "base/message_loop/message_pump_type.h"
#include "base/no_destructor.h"
#include "base/run_loop.h"
#include "base/task/post_task.h"
#include "base/task/task_traits_extension.h"
#include "base/threading/thread_task_runner_handle.h"
#include "base/trace_event/trace_event.h"
#include "build/build_config.h"
#include "content/browser/browser_process_sub_thread.h"
#include "content/browser/browser_thread_impl.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/common/content_features.h"

#if defined(OS_ANDROID)
#include "base/android/task_scheduler/post_task_android.h"
#endif

using QueueType = content::BrowserTaskQueues::QueueType;

namespace content {
namespace {

// Returns the BrowserThread::ID stored in |traits| which must be coming from a
// call through BaseBrowserTaskExecutor and hence have the
// BrowserTaskTraitsExtension.
BrowserThread::ID ExtractBrowserThreadId(const base::TaskTraits& traits) {
  DCHECK_EQ(BrowserTaskTraitsExtension::kExtensionId, traits.extension_id());
  const BrowserTaskTraitsExtension extension =
      traits.GetExtension<BrowserTaskTraitsExtension>();

  const BrowserThread::ID thread_id = extension.browser_thread();
  DCHECK_GE(thread_id, 0);
  return thread_id;
}

// |g_browser_task_executor| is intentionally leaked on shutdown.
BrowserTaskExecutor* g_browser_task_executor = nullptr;

}  // namespace

BaseBrowserTaskExecutor::BaseBrowserTaskExecutor() = default;

BaseBrowserTaskExecutor::~BaseBrowserTaskExecutor() = default;

bool BaseBrowserTaskExecutor::PostDelayedTask(const base::Location& from_here,
                                              const base::TaskTraits& traits,
                                              base::OnceClosure task,
                                              base::TimeDelta delay) {
  if (traits.extension_id() != BrowserTaskTraitsExtension::kExtensionId ||
      traits.GetExtension<BrowserTaskTraitsExtension>().nestable()) {
    return GetTaskRunner(ExtractBrowserThreadId(traits), traits)
        ->PostDelayedTask(from_here, std::move(task), delay);
  } else {
    return GetTaskRunner(ExtractBrowserThreadId(traits), traits)
        ->PostNonNestableDelayedTask(from_here, std::move(task), delay);
  }
}

scoped_refptr<base::TaskRunner> BaseBrowserTaskExecutor::CreateTaskRunner(
    const base::TaskTraits& traits) {
  return GetTaskRunner(ExtractBrowserThreadId(traits), traits);
}

scoped_refptr<base::SequencedTaskRunner>
BaseBrowserTaskExecutor::CreateSequencedTaskRunner(
    const base::TaskTraits& traits) {
  return GetTaskRunner(ExtractBrowserThreadId(traits), traits);
}

scoped_refptr<base::SingleThreadTaskRunner>
BaseBrowserTaskExecutor::CreateSingleThreadTaskRunner(
    const base::TaskTraits& traits,
    base::SingleThreadTaskRunnerThreadMode thread_mode) {
  return GetTaskRunner(ExtractBrowserThreadId(traits), traits);
}

#if defined(OS_WIN)
scoped_refptr<base::SingleThreadTaskRunner>
BaseBrowserTaskExecutor::CreateCOMSTATaskRunner(
    const base::TaskTraits& traits,
    base::SingleThreadTaskRunnerThreadMode thread_mode) {
  return GetTaskRunner(ExtractBrowserThreadId(traits), traits);
}
#endif  // defined(OS_WIN)

scoped_refptr<base::SingleThreadTaskRunner>
BaseBrowserTaskExecutor::GetTaskRunner(BrowserThread::ID identifier,
                                       const base::TaskTraits& traits) const {
  const QueueType queue_type = GetQueueType(traits);

  switch (identifier) {
    case BrowserThread::UI: {
      return browser_ui_thread_handle_->GetBrowserTaskRunner(queue_type);
    }
    case BrowserThread::IO:
      return browser_io_thread_handle_->GetBrowserTaskRunner(queue_type);
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
    case BrowserThread::SHM:
      return browser_shm_thread_handle_->GetBrowserTaskRunner(queue_type);
#endif
    case BrowserThread::ID_COUNT:
      NOTREACHED();
  }
  return nullptr;
}

// static
QueueType BaseBrowserTaskExecutor::GetQueueType(
    const base::TaskTraits& traits) {
  if (traits.extension_id() == BrowserTaskTraitsExtension::kExtensionId) {
    const BrowserTaskTraitsExtension extension =
        traits.GetExtension<BrowserTaskTraitsExtension>();

    const BrowserTaskType task_type = extension.task_type();
    DCHECK_LT(task_type, BrowserTaskType::kBrowserTaskType_Last);

    switch (task_type) {
      case BrowserTaskType::kBootstrap:
        // Note we currently ignore the priority for bootstrap tasks.
        return QueueType::kBootstrap;

      case BrowserTaskType::kPreconnect:
        // Note we currently ignore the priority for navigation and
        // preconnection tasks.
        return QueueType::kPreconnection;

      case BrowserTaskType::kDefault:
        // Defer to traits.priority() below.
        break;

      case BrowserTaskType::kBrowserTaskType_Last:
        NOTREACHED();
    }
  }

  switch (traits.priority()) {
    case base::TaskPriority::BEST_EFFORT:
      return QueueType::kBestEffort;

    case base::TaskPriority::USER_VISIBLE:
      return QueueType::kUserVisible;

    case base::TaskPriority::USER_BLOCKING:
      return QueueType::kUserBlocking;
  }
}

BrowserTaskExecutor::BrowserTaskExecutor(
    std::unique_ptr<BrowserUIThreadScheduler> browser_ui_thread_scheduler,
    std::unique_ptr<BrowserIOThreadDelegate> browser_io_thread_delegate
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
    ,
    std::unique_ptr<BrowserSHMThreadDelegate> browser_shm_thread_delegate
#endif
    )
    : ui_thread_executor_(std::make_unique<UIThreadExecutor>(
          std::move(browser_ui_thread_scheduler))),
      io_thread_executor_(std::make_unique<IOThreadExecutor>(
          std::move(browser_io_thread_delegate)))
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
      ,
      shm_thread_executor_(std::make_unique<SHMThreadExecutor>(
          std::move(browser_shm_thread_delegate)))
#endif
{
  browser_ui_thread_handle_ = ui_thread_executor_->GetUIThreadHandle();
  browser_io_thread_handle_ = io_thread_executor_->GetIOThreadHandle();
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
  browser_shm_thread_handle_ = shm_thread_executor_->GetSHMThreadHandle();
#endif
  ui_thread_executor_->SetIOThreadHandle(browser_io_thread_handle_);
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
  ui_thread_executor_->SetSHMThreadHandle(browser_shm_thread_handle_);
#endif
  io_thread_executor_->SetUIThreadHandle(browser_ui_thread_handle_);
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
  io_thread_executor_->SetSHMThreadHandle(browser_shm_thread_handle_);
  shm_thread_executor_->SetUIThreadHandle(browser_ui_thread_handle_);
  shm_thread_executor_->SetIOThreadHandle(browser_io_thread_handle_);
#endif
}

BrowserTaskExecutor::~BrowserTaskExecutor() = default;

// static
void BrowserTaskExecutor::Create() {
  DCHECK(!base::ThreadTaskRunnerHandle::IsSet());
  CreateInternal(std::make_unique<BrowserUIThreadScheduler>(),
                 std::make_unique<BrowserIOThreadDelegate>()
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
                     ,
                 std::make_unique<BrowserSHMThreadDelegate>()
#endif
  );
  Get()->ui_thread_executor_->BindToCurrentThread();
}

// static
void BrowserTaskExecutor::CreateForTesting(
    std::unique_ptr<BrowserUIThreadScheduler> browser_ui_thread_scheduler,
    std::unique_ptr<BrowserIOThreadDelegate> browser_io_thread_delegate
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
    ,
    std::unique_ptr<BrowserSHMThreadDelegate> browser_shm_thread_delegate
#endif
) {
  CreateInternal(std::move(browser_ui_thread_scheduler),
                 std::move(browser_io_thread_delegate)
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
                     ,
                 std::move(browser_shm_thread_delegate)
#endif
  );
}

// static
void BrowserTaskExecutor::BindToUIThreadForTesting() {
  g_browser_task_executor->ui_thread_executor_->BindToCurrentThread();
}

// static
void BrowserTaskExecutor::CreateInternal(
    std::unique_ptr<BrowserUIThreadScheduler> browser_ui_thread_scheduler,
    std::unique_ptr<BrowserIOThreadDelegate> browser_io_thread_delegate
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
    ,
    std::unique_ptr<BrowserSHMThreadDelegate> browser_shm_thread_delegate
#endif
) {
  DCHECK(!g_browser_task_executor);
  g_browser_task_executor =
      new BrowserTaskExecutor(std::move(browser_ui_thread_scheduler),
                              std::move(browser_io_thread_delegate)
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
                                  ,
                              std::move(browser_shm_thread_delegate)
#endif
      );
  base::RegisterTaskExecutor(BrowserTaskTraitsExtension::kExtensionId,
                             g_browser_task_executor);

  g_browser_task_executor->browser_ui_thread_handle_
      ->EnableAllExceptBestEffortQueues();

#if defined(OS_ANDROID)
  base::PostTaskAndroid::SignalNativeSchedulerReady();
#endif
}

// static
BrowserTaskExecutor* BrowserTaskExecutor::Get() {
  DCHECK(g_browser_task_executor)
      << "No browser task executor created.\nHint: if this is in a unit test, "
         "you're likely missing a content::BrowserTaskEnvironment member in "
         "your fixture.";
  return g_browser_task_executor;
}

// static
void BrowserTaskExecutor::ResetForTesting() {
#if defined(OS_ANDROID)
  base::PostTaskAndroid::SignalNativeSchedulerShutdownForTesting();
#endif
  if (g_browser_task_executor) {
    RunAllPendingTasksOnThreadForTesting(BrowserThread::UI);
    RunAllPendingTasksOnThreadForTesting(BrowserThread::IO);
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
    RunAllPendingTasksOnThreadForTesting(BrowserThread::SHM);
#endif
    base::UnregisterTaskExecutorForTesting(
        BrowserTaskTraitsExtension::kExtensionId);
    delete g_browser_task_executor;
    g_browser_task_executor = nullptr;
  }
}

// static
void BrowserTaskExecutor::PostFeatureListSetup() {
  DCHECK(Get()->browser_ui_thread_handle_);
  DCHECK(Get()->browser_io_thread_handle_);
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
  DCHECK(Get()->browser_shm_thread_handle_);
#endif
  Get()->browser_ui_thread_handle_->PostFeatureListInitializationSetup();
  Get()->browser_io_thread_handle_->PostFeatureListInitializationSetup();
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
  Get()->browser_shm_thread_handle_->PostFeatureListInitializationSetup();
#endif
}

// static
void BrowserTaskExecutor::Shutdown() {
  if (!g_browser_task_executor)
    return;

  DCHECK(Get()->ui_thread_executor_);
  DCHECK(Get()->io_thread_executor_);
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
  DCHECK(Get()->shm_thread_executor_);
#endif
  // We don't delete |g_browser_task_executor| because other threads may
  // PostTask or call BrowserTaskExecutor::GetTaskRunner while we're tearing
  // things down. We don't want to add locks so we just leak instead of dealing
  // with that. For similar reasons we don't need to call
  // PostTaskAndroid::SignalNativeSchedulerShutdown on Android. In tests however
  // we need to clean up, so BrowserTaskExecutor::ResetForTesting should be
  // called.
  Get()->ui_thread_executor_.reset();
  Get()->io_thread_executor_.reset();
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
  Get()->shm_thread_executor_.reset();
#endif
}

// static
void BrowserTaskExecutor::RunAllPendingTasksOnThreadForTesting(
    BrowserThread::ID identifier) {
  DCHECK(Get());

  base::RunLoop run_loop(base::RunLoop::Type::kNestableTasksAllowed);

  switch (identifier) {
    case BrowserThread::UI:
      Get()->browser_ui_thread_handle_->ScheduleRunAllPendingTasksForTesting(
          run_loop.QuitClosure());
      break;
    case BrowserThread::IO: {
      Get()->browser_io_thread_handle_->ScheduleRunAllPendingTasksForTesting(
          run_loop.QuitClosure());
      break;
    }
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
    case BrowserThread::SHM: {
      Get()->browser_shm_thread_handle_->ScheduleRunAllPendingTasksForTesting(
          run_loop.QuitClosure());
      break;
    }
#endif
    case BrowserThread::ID_COUNT:
      NOTREACHED();
  }

  run_loop.Run();
}

// static
void BrowserTaskExecutor::EnableAllQueues() {
  Get()->browser_ui_thread_handle_->EnableAllQueues();
  Get()->browser_io_thread_handle_->EnableAllQueues();
#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
  Get()->browser_shm_thread_handle_->EnableAllQueues();
#endif
}

// static
scoped_refptr<base::SingleThreadTaskRunner>
BrowserTaskExecutor::GetUIThreadTaskRunner(const BrowserTaskTraits& traits) {
  return Get()->GetTaskRunner(BrowserThread::UI, traits);
}

// static
scoped_refptr<base::SingleThreadTaskRunner>
BrowserTaskExecutor::GetIOThreadTaskRunner(const BrowserTaskTraits& traits) {
  return Get()->GetTaskRunner(BrowserThread::IO, traits);
}

#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
// static
scoped_refptr<base::SingleThreadTaskRunner>
BrowserTaskExecutor::GetSHMThreadTaskRunner(const BrowserTaskTraits& traits) {
  return Get()->GetTaskRunner(BrowserThread::SHM, traits);
}
#endif

// static
void BrowserTaskExecutor::InitializeIOThread() {
  Get()->browser_io_thread_handle_->EnableAllExceptBestEffortQueues();
}

#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
// static
void BrowserTaskExecutor::InitializeSHMThread() {
  Get()->browser_shm_thread_handle_->EnableAllExceptBestEffortQueues();
}
#endif

std::unique_ptr<BrowserProcessSubThread> BrowserTaskExecutor::CreateIOThread() {
  DCHECK(Get()->io_thread_executor_);

  std::unique_ptr<BrowserIOThreadDelegate> browser_io_thread_delegate =
      Get()->io_thread_executor_->TakeDelegate();

  DCHECK(browser_io_thread_delegate);
  TRACE_EVENT0("startup", "BrowserTaskExecutor::CreateIOThread");

  auto io_thread = std::make_unique<BrowserProcessSubThread>(BrowserThread::IO);

  if (browser_io_thread_delegate->allow_blocking_for_testing()) {
    io_thread->AllowBlockingForTesting();
  }

  base::Thread::Options options;
  options.message_pump_type = base::MessagePumpType::IO;
  options.delegate = browser_io_thread_delegate.release();
  // Up the priority of the |io_thread_| as some of its IPCs relate to
  // display tasks.
  if (base::FeatureList::IsEnabled(features::kBrowserUseDisplayThreadPriority))
    options.priority = base::ThreadPriority::DISPLAY;
  if (!io_thread->StartWithOptions(options))
    LOG(FATAL) << "Failed to start BrowserThread:IO";
  return io_thread;
}

#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
std::unique_ptr<BrowserProcessSubThread>
BrowserTaskExecutor::CreateSHMThread() {
  DCHECK(Get()->shm_thread_executor_);

  std::unique_ptr<BrowserSHMThreadDelegate> browser_shm_thread_delegate =
      Get()->shm_thread_executor_->TakeDelegate();

  DCHECK(browser_shm_thread_delegate);
  TRACE_EVENT0("startup", "BrowserTaskExecutor::CreateSHMThread");

  auto shm_thread =
      std::make_unique<BrowserProcessSubThread>(BrowserThread::SHM);

  if (browser_shm_thread_delegate->allow_blocking_for_testing()) {
    shm_thread->AllowBlockingForTesting();
  }

  base::Thread::Options options;
  options.message_pump_type = base::MessagePumpType::SHM;
  options.delegate = browser_shm_thread_delegate.release();
  // Up the priority of the |io_thread_| as some of its IPCs relate to
  // display tasks.
  if (base::FeatureList::IsEnabled(features::kBrowserUseDisplayThreadPriority))
    options.priority = base::ThreadPriority::DISPLAY;
  if (!shm_thread->StartWithOptions(options))
    LOG(FATAL) << "Failed to start BrowserThread:SHM";
  return shm_thread;
}
#endif

BrowserTaskExecutor::UIThreadExecutor::UIThreadExecutor(
    std::unique_ptr<BrowserUIThreadScheduler> browser_ui_thread_scheduler)
    : browser_ui_thread_scheduler_(std::move(browser_ui_thread_scheduler)) {
  browser_ui_thread_handle_ = browser_ui_thread_scheduler_->GetHandle();
}

BrowserTaskExecutor::UIThreadExecutor::~UIThreadExecutor() {
  if (bound_to_thread_)
    base::SetTaskExecutorForCurrentThread(nullptr);
}

void BrowserTaskExecutor::UIThreadExecutor::BindToCurrentThread() {
  bound_to_thread_ = true;
  base::SetTaskExecutorForCurrentThread(this);
}

scoped_refptr<BrowserUIThreadScheduler::Handle>
BrowserTaskExecutor::UIThreadExecutor::GetUIThreadHandle() {
  return browser_ui_thread_handle_;
}

void BrowserTaskExecutor::UIThreadExecutor::SetIOThreadHandle(
    scoped_refptr<BrowserUIThreadScheduler::Handle> io_thread_handle) {
  browser_io_thread_handle_ = std::move(io_thread_handle);
}

#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
void BrowserTaskExecutor::UIThreadExecutor::SetSHMThreadHandle(
    scoped_refptr<BrowserSHMThreadDelegate::Handle> shm_thread_handle) {
  browser_shm_thread_handle_ = std::move(shm_thread_handle);
}
#endif

BrowserTaskExecutor::IOThreadExecutor::IOThreadExecutor(
    std::unique_ptr<BrowserIOThreadDelegate> browser_io_thread_delegate)
    : browser_io_thread_delegate_(std::move(browser_io_thread_delegate)) {
  // |browser_io_thread_delegate_| can be null in tests.
  if (!browser_io_thread_delegate_)
    return;
  browser_io_thread_delegate_->SetTaskExecutor(this);
  browser_io_thread_handle_ = browser_io_thread_delegate_->GetHandle();
}

BrowserTaskExecutor::IOThreadExecutor::~IOThreadExecutor() = default;

scoped_refptr<BrowserUIThreadScheduler::Handle>
BrowserTaskExecutor::IOThreadExecutor::GetIOThreadHandle() {
  return browser_io_thread_handle_;
}

void BrowserTaskExecutor::IOThreadExecutor::SetUIThreadHandle(
    scoped_refptr<BrowserUIThreadScheduler::Handle> ui_thread_handle) {
  browser_ui_thread_handle_ = std::move(ui_thread_handle);
}

#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
void BrowserTaskExecutor::IOThreadExecutor::SetSHMThreadHandle(
    scoped_refptr<BrowserSHMThreadDelegate::Handle> shm_thread_handle) {
  browser_shm_thread_handle_ = std::move(shm_thread_handle);
}
#endif

#if defined(OS_POSIX) && BUILDFLAG(USE_HERQULES)
BrowserTaskExecutor::SHMThreadExecutor::SHMThreadExecutor(
    std::unique_ptr<BrowserSHMThreadDelegate> browser_shm_thread_delegate)
    : browser_shm_thread_delegate_(std::move(browser_shm_thread_delegate)) {
  // |browser_shm_thread_delegate_| can be null in tests.
  if (!browser_shm_thread_delegate_)
    return;
  browser_shm_thread_delegate_->SetTaskExecutor(this);
  browser_shm_thread_handle_ = browser_shm_thread_delegate_->GetHandle();
}

BrowserTaskExecutor::SHMThreadExecutor::~SHMThreadExecutor() = default;

scoped_refptr<BrowserUIThreadScheduler::Handle>
BrowserTaskExecutor::SHMThreadExecutor::GetSHMThreadHandle() {
  return browser_shm_thread_handle_;
}

void BrowserTaskExecutor::SHMThreadExecutor::SetIOThreadHandle(
    scoped_refptr<BrowserUIThreadScheduler::Handle> io_thread_handle) {
  browser_io_thread_handle_ = std::move(io_thread_handle);
}

void BrowserTaskExecutor::SHMThreadExecutor::SetUIThreadHandle(
    scoped_refptr<BrowserUIThreadScheduler::Handle> ui_thread_handle) {
  browser_ui_thread_handle_ = std::move(ui_thread_handle);
}
#endif

}  // namespace content
