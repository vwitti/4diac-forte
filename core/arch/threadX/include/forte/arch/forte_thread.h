/************************************************************************************
 * Copyright (c) 2026
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 ************************************************************************************/

#pragma once

#include <tx_api.h>

#include "forte/util/devlog.h"
#include "forte/arch/threadbase.h"
#include "forte/arch/forte_sync.h"

// Allow to set a special stack size for forte threads
#ifndef FORTE_THREADX_MIN_STACK_SIZE
#define FORTE_THREADX_MIN_STACK_SIZE 15000
#endif

namespace forte::arch {

  class CThreadXThread;

  /*! \ingroup ThreadX-HAL
   * rief FORTE thread wrapper for ThreadX.
   */
  class CThreadXThread : public CThreadBase<TX_THREAD *> {
    public:
      explicit CThreadXThread(long paStackSize = FORTE_THREADX_MIN_STACK_SIZE);
      ~CThreadXThread() override;

      static void sleepThread(unsigned int paMilliSeconds);

    protected:
      void setPriority(int paPriority) {
        // ThreadX: smaller value => higher priority (typically).
        // We forward the numeric value as-is; adapt here if you want mapping.
        if(nullptr != getThreadHandle()) {
          UINT oldPrio = 0, oldThresh = 0;
          tx_thread_priority_change(getThreadHandle(), static_cast<UINT>(paPriority), &oldPrio);
          // keep threshold unchanged; if needed, set explicitly via tx_thread_threshold_set.
          (void)oldThresh;
        }
      }

    private:
      static void threadFunction(ULONG paData);

      TThreadHandleType createThread(long paStackSize) override;

      TX_THREAD *mThreadCtrlBlock{nullptr};

      // Default FORTE priority for internal threads (tunable)
      static const UINT scmForteThreadPriority;
  };

  typedef CThreadXThread CThread;

} // namespace forte::arch
