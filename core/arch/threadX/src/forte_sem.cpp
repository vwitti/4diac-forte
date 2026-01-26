/************************************************************************************
 * Copyright (c) 2026
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 ************************************************************************************/

#include "forte/arch/forte_sem.h"
#include "forte/util/devlog.h"

namespace forte::arch {

  static inline ULONG nsToTicks(TForteUInt64 ns) {
    // ns -> ms -> ticks (rounded up)
    const ULONG tps = TX_TIMER_TICKS_PER_SECOND;
    const unsigned long long ms = static_cast<unsigned long long>(ns / 1000000ULL);
    unsigned long long ticks = (ms * tps + 999ULL) / 1000ULL;
    return (ticks == 0ULL) ? 1UL : static_cast<ULONG>(ticks);
  }

  CThreadXSemaphore::CThreadXSemaphore(unsigned int paInitialValue) {
    UINT st = tx_semaphore_create(&mSemaphore, const_cast<CHAR *>("forte_sem"), paInitialValue);
    mValid = (TX_SUCCESS == st);
    if(!mValid) {
      DEVLOG_ERROR("CThreadXSemaphore: tx_semaphore_create failed (%u)\n", st);
    }
  }

  CThreadXSemaphore::~CThreadXSemaphore() {
    if(mValid) {
      tx_semaphore_delete(&mSemaphore);
    }
  }

  void CThreadXSemaphore::inc() {
    if(mValid) {
      tx_semaphore_put(&mSemaphore);
    }
  }

  void CThreadXSemaphore::waitIndefinitely() {
    if(mValid) {
      tx_semaphore_get(&mSemaphore, TX_WAIT_FOREVER);
    }
  }

  bool CThreadXSemaphore::timedWait(const TForteUInt64 paRelativeTimeout) {
    if(!mValid) {
      return false;
    }
    ULONG ticks = nsToTicks(paRelativeTimeout);
    return (TX_SUCCESS == tx_semaphore_get(&mSemaphore, ticks));
  }

  bool CThreadXSemaphore::tryNoWait() {
    if(!mValid) {
      return false;
    }
    return (TX_SUCCESS == tx_semaphore_get(&mSemaphore, 0));
  }

} // namespace forte::arch
