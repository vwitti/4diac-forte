/************************************************************************************
 * Copyright (c) 2026
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 ************************************************************************************/

#include "forte/arch/forte_sync.h"
#include "forte/util/devlog.h"

namespace forte::arch {

  CThreadXSyncObject::CThreadXSyncObject() {
    UINT st = tx_mutex_create(&mMutex, const_cast<CHAR *>("forte_mutex"), TX_INHERIT);
    mValid = (TX_SUCCESS == st);
    if(!mValid) {
      DEVLOG_ERROR("CThreadXSyncObject: tx_mutex_create failed (%u)\n", st);
    }
  }

  CThreadXSyncObject::~CThreadXSyncObject() {
    if(mValid) {
      tx_mutex_delete(&mMutex);
    }
  }

  void CThreadXSyncObject::lock() {
    if(mValid) {
      tx_mutex_get(&mMutex, TX_WAIT_FOREVER);
    }
  }

  void CThreadXSyncObject::unlock() {
    if(mValid) {
      tx_mutex_put(&mMutex);
    }
  }

} // namespace forte::arch
