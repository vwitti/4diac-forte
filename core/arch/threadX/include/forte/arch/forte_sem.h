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
#include "forte/datatype.h"

namespace forte::arch {

  /*! \ingroup ThreadX-HAL
   * rief Counting semaphore for synchronizing operations in FORTE.
   */
  class CThreadXSemaphore {
    public:
      explicit CThreadXSemaphore(unsigned int paInitialValue = 0);
      ~CThreadXSemaphore();

      void inc();
      void waitIndefinitely();

      bool timedWait(const TForteUInt64 paRelativeTimeout);
      bool tryNoWait();

    private:
      TX_SEMAPHORE mSemaphore{};
      bool mValid{false};
  };

  typedef CThreadXSemaphore CSemaphore;

} // namespace forte::arch
