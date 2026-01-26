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

namespace forte::arch {

  /*! \ingroup ThreadX-HAL
   * rief Sync object implementation for ThreadX.
   */
  class CThreadXSyncObject {
    public:
      CThreadXSyncObject();
      ~CThreadXSyncObject();

      void lock();
      void unlock();

    private:
      TX_MUTEX mMutex{};
      bool mValid{false};
  };

  typedef CThreadXSyncObject CSyncObject;

} // namespace forte::arch
