/************************************************************************************
 * Copyright (c) 2016, 2024 fortiss GmbH, Jose Cabral
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 * Milan Vathoopan, Guru Chandrasekhara - initial API and implementation and/or initial documentation
 ************************************************************************************/

#include "tx_api.h"

#include "../c_interface/forte_c.h"


CHAR thread_name[] = "FORTE_THREAD";
#define STACK_SIZE		2000

static TX_THREAD tx_forte_thread;
static char tx_forte_thread_stack[STACK_SIZE];

namespace {
  const unsigned forteTaskPriority = 5;
  const unsigned int desiredFortePort = 61499;
  const configSTACK_DEPTH_TYPE stackDepth = STACK_SIZE;
} // namespace

void vForteTask(uint32_t arg) {
  TForteInstance forteInstance;

  if (auto result = forte::arch::CForteArchitecture::initialize(0, NULL); result != 0) {
	  tx_thread_delete(&tx_forte_thread);
  }

  if (auto result = forteStartInstance(desiredFortePort, &forteInstance); result != FORTE_OK) {
	  tx_thread_delete(&tx_forte_thread);
  }

  forteWaitForInstanceToStop(forteInstance);

  tx_thread_delete(&tx_forte_thread);
}

/* Assumes that schedule is already started tx_kernel_enter(); and we are somewhere in a task context */
int main_forte() {

  if (auto result = forteGlobalInitialize(0, nullptr); result != FORTE_OK) {
    return result;
  }

  UINT ret;

  ret = tx_thread_create(&tx_forte_thread,
		    thread_name,
			vForteTask,
			0,
			tx_forte_thread_stack,
			STACK_SIZE,
			5,         //priority,
			5,         //preempt_threshold,
			20,        //time_slice,
			TX_AUTO_START);

  // Will not get here unless there is insufficient RAM.
  return ret;
}
