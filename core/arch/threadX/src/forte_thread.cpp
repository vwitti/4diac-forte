/************************************************************************************
 * Copyright (c) 2026
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 ************************************************************************************/
#include "forte/arch/forte_thread.h"
#include "forte/util/criticalregion.h"

namespace forte::arch {

  // Default priority for FORTE threads. ThreadX typically: 0 is highest priority.
  const UINT CThreadXThread::scmForteThreadPriority = 10;

  CThreadXThread::CThreadXThread(long paStackSize) : CThreadBase(paStackSize) {
    // ThreadX requires the stack memory to remain valid for the lifetime of the thread.
    mStack = new char[paStackSize];
  }

  CThreadXThread::~CThreadXThread() = default;

  void CThreadXThread::threadFunction(ULONG paData) {
    CThreadBase::runThread(reinterpret_cast<CThreadXThread *>(paData));

    // After runThread returns, delete the underlying ThreadX thread.
    // Note: tx_thread_delete must be called when the thread is not executing, but this
    // function is executing in that thread. This is a common pattern in ThreadX ports
    // where the thread terminates itself before delete.
    TX_THREAD *self = tx_thread_identify();
    if(nullptr != self) {
      tx_thread_terminate(self);
      tx_thread_delete(self);
    }
  }

  CThreadBase<TX_THREAD *>::TThreadHandleType CThreadXThread::createThread(long paStackSize) {
    // Create control block on heap to allow safe deletion.
    mThreadCtrlBlock = new TX_THREAD();

    // ThreadX stack: mStack (allocated in ctor) is used here.
    UINT st = tx_thread_create(mThreadCtrlBlock,
                              const_cast<CHAR *>("FORTE"),
                              &CThreadXThread::threadFunction,
                              reinterpret_cast<ULONG>(this),
                              mStack,
                              static_cast<ULONG>(paStackSize),
                              scmForteThreadPriority,
                              scmForteThreadPriority,
                              TX_NO_TIME_SLICE,
                              TX_AUTO_START);

    if(TX_SUCCESS != st) {
      DEVLOG_ERROR("Error: Could not create ThreadX thread (tx_thread_create=%u)\n", st);
      delete mThreadCtrlBlock;
      mThreadCtrlBlock = nullptr;
      return nullptr;
    }

    return mThreadCtrlBlock;
  }

  void CThreadXThread::sleepThread(unsigned int paMilliSeconds) {
    // Convert ms to ticks (round up, avoid 0)
    const ULONG tps = TX_TIMER_TICKS_PER_SECOND;
    ULONG ticks = static_cast<ULONG>((static_cast<unsigned long long>(paMilliSeconds) * tps + 999ULL) / 1000ULL);
    if(ticks == 0) {
      ticks = 1;
    }
    tx_thread_sleep(ticks);
  }

} // namespace forte::arch
