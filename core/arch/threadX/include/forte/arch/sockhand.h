/************************************************************************************
 * Copyright (c) 2026
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Notes:
 *  - This port assumes the NetX Duo BSD compatibility layer (nx_bsd) is enabled.
 *    If your BSP does not provide nx_bsd, you will need a native NetX Duo socket
 *    backend (and likely a custom socket handler).
 ************************************************************************************/

#pragma once

//#include <errno.h>
#include <string.h>

#include "forte/util/devlog.h"

#define close soc_close
//#define __SES_ARM
#define __CCRX__

// NetX Duo (BSD compatibility layer)
#include <nx_api.h>
#include <nxd_bsd.h>

// Avoid symbol clashes with C++17 std::bind and potential connect macros
#ifdef bind
#undef bind
#endif
#ifdef connect
#undef connect
#endif

// FORTE expects this typedef to exist for the generic socket handler infrastructure.
#ifndef FORTE_SOCKET_TYPE
#define FORTE_SOCKET_TYPE int
#endif

// These includes need to be last
#include "forte/arch/gensockhand.h"
#include "forte/arch/fdselecthand.h"
#include "forte/arch/bsdsocketinterf.h"

namespace forte::arch {
  typedef CGenericIPComSocketHandler<CFDSelectHandler, CBSDSocketInterface> CIPComSocketHandler;
} // namespace forte::arch
