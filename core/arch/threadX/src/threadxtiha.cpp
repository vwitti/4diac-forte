/************************************************************************************
 * Copyright (c) 2026
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 ************************************************************************************/
#include "threadxtiha.h"

#include "forte/devexec.h"
#include "../../common/src/utils/timespec_utils.h"
#include "forte/timerhandlerfactory.h"

using namespace forte::literals;

namespace forte::arch {
  namespace {
    [[maybe_unused]] const TimerHandlerFactory::EntryImpl<CThreadXTimerHandler> entry("default"_STRID);
  }

  CThreadXTimerHandler::CThreadXTimerHandler(CDeviceExecution &paDeviceExecution) : CTimerHandler(paDeviceExecution) {
    // ThreadX expects ticks for initial and reschedule.
    // We want a periodic timer that triggers with FORTE's internal tick rate.
    const ULONG periodTicks = static_cast<ULONG>(TX_TIMER_TICKS_PER_SECOND / getTicksPerSecond());
    const ULONG safePeriod = (periodTicks == 0) ? 1 : periodTicks;

    UINT st = tx_timer_create(&mTimer,
                             const_cast<CHAR*>("FORTETimer"),
                             &CThreadXTimerHandler::callbackFunction,
                             reinterpret_cast<ULONG>(this),
                             safePeriod,
                             safePeriod,
                             TX_NO_ACTIVATE);
    mCreated = (TX_SUCCESS == st);
  }

  CThreadXTimerHandler::~CThreadXTimerHandler() {
    disableHandler();
    if(mCreated) {
      tx_timer_delete(&mTimer);
    }
  }

  void CThreadXTimerHandler::enableHandler() {
    if(mCreated) {
      tx_timer_activate(&mTimer);
    }
  }

  void CThreadXTimerHandler::disableHandler() {
    if(mCreated) {
      tx_timer_deactivate(&mTimer);
    }
  }

  void CThreadXTimerHandler::callbackFunction(ULONG input) {
    static_cast<CThreadXTimerHandler *>(reinterpret_cast<void*>(input))->nextTick();
  }

} // namespace forte::arch
