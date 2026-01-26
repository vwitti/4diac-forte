/*******************************************************************************
 * Copyright (c) 2026
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *******************************************************************************/

#include <tx_api.h>

#include "forte/arch/forte_architecture_time.h"
#include "forte/util/forte_constants.h"

uint_fast64_t getNanoSecondsMonotonicArch() {
  // tx_time_get returns ticks since ThreadX start.
  const uint_fast64_t ticks = static_cast<uint_fast64_t>(tx_time_get());
  const uint_fast64_t tps = static_cast<uint_fast64_t>(TX_TIMER_TICKS_PER_SECOND);
  return ticks * (forte::util::constants::cNanosecondsPerSecond / tps);
}

uint_fast64_t getNanoSecondsRealtimeArch() {
  // Fallback to standard time() based seconds (see forte_standard_time.cpp)
  return static_cast<uint_fast64_t>(forte::arch::forte_time()) * 1000000000ULL;
}

time_t forte::arch::forte_timegm(struct tm *pa_tm) {
  time_t tm = mktime(pa_tm);
  return mktime(gmtime(&tm));
}

struct tm *forte::arch::forte_localtime(const time_t *paTime, struct tm *const paResult) {
  return localtime_r(paTime, paResult);
}

struct tm *forte::arch::forte_gmtime(const time_t *const paTime, struct tm *const paResult) {
  return gmtime_r(paTime, paResult);
}
