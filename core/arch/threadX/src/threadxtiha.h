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
#include "forte/timerha.h"

namespace forte::arch {

  class CThreadXTimerHandler final : public CTimerHandler {
    public:
      explicit CThreadXTimerHandler(CDeviceExecution &paDeviceExecution);
      ~CThreadXTimerHandler() override;

      void enableHandler() override;
      void disableHandler() override;

    private:
      static void callbackFunction(ULONG input);

      TX_TIMER mTimer{};
      bool mCreated{false};
  };

} // namespace forte::arch
