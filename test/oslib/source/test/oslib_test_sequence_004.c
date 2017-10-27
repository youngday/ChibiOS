/*
    ChibiOS - Copyright (C) 2006..2017 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "hal.h"
#include "oslib_test_root.h"

/**
 * @file    oslib_test_sequence_004.c
 * @brief   Test Sequence 004 code.
 *
 * @page oslib_test_sequence_004 [4] Objects Factory
 *
 * File: @ref oslib_test_sequence_004.c
 *
 * <h2>Description</h2>
 * This sequence tests the ChibiOS library functionalities related to
 * the object factory.
 *
 * <h2>Conditions</h2>
 * This sequence is only executed if the following preprocessor condition
 * evaluates to true:
 * - (CH_CFG_USE_FACTORY == TRUE) && (CH_CFG_USE_MEMPOOLS == TRUE) && (CH_CFG_USE_HEAP == TRUE)
 * .
 *
 * <h2>Test Cases</h2>
 * - @subpage oslib_test_004_001
 * - @subpage oslib_test_004_002
 * - @subpage oslib_test_004_003
 * .
 */

#if ((CH_CFG_USE_FACTORY == TRUE) && (CH_CFG_USE_MEMPOOLS == TRUE) && (CH_CFG_USE_HEAP == TRUE)) || defined(__DOXYGEN__)

/****************************************************************************
 * Shared code.
 ****************************************************************************/


/****************************************************************************
 * Test cases.
 ****************************************************************************/

#if (CH_CFG_FACTORY_OBJECTS_REGISTRY == TRUE) || defined(__DOXYGEN__)
/**
 * @page oslib_test_004_001 [4.1] Objects Registry
 *
 * <h2>Description</h2>
 * This test case verifies the static objects registry.
 *
 * <h2>Conditions</h2>
 * This test is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_FACTORY_OBJECTS_REGISTRY == TRUE
 * .
 *
 * <h2>Test Steps</h2>
 * - [4.1.1] Retrieving a registered object by name, must not exist.
 * - [4.1.2] Registering an object, it must not exists, must succeed.
 * - [4.1.3] Registering an object with the same name, must fail.
 * - [4.1.4] Retrieving the registered object by name, must exist, then
 *   increasing the reference counter, finally releasing both
 *   references.
 * - [4.1.5] Releasing the first reference to the object, must not
 *   trigger an assertion.
 * - [4.1.6] Retrieving the registered object by name again, must not
 *   exist.
 * .
 */

static void oslib_test_004_001_teardown(void) {
  registered_object_t *rop;

  rop = chFactoryFindObject("myobj");
  if (rop != NULL) {
    while (rop->element.refs > 0U) {
      chFactoryReleaseObject(rop);
    }
  }
}

static void oslib_test_004_001_execute(void) {
  registered_object_t *rop;

  /* [4.1.1] Retrieving a registered object by name, must not exist.*/
  test_set_step(1);
  {
    rop = chFactoryFindObject("myobj");
    test_assert(rop == NULL, "found");
  }

  /* [4.1.2] Registering an object, it must not exists, must succeed.*/
  test_set_step(2);
  {
    static uint32_t myobj = 0x55aa;

    rop = chFactoryRegisterObject("myobj", (void *)&myobj);
    test_assert(rop != NULL, "cannot register");
  }

  /* [4.1.3] Registering an object with the same name, must fail.*/
  test_set_step(3);
  {
    registered_object_t *rop1;
    static uint32_t myobj = 0x55aa;

    rop1 = chFactoryRegisterObject("myobj", (void *)&myobj);
    test_assert(rop1 == NULL, "can register");
  }

  /* [4.1.4] Retrieving the registered object by name, must exist, then
     increasing the reference counter, finally releasing both
     references.*/
  test_set_step(4);
  {
    registered_object_t *rop1, *rop2;

    rop1 = chFactoryFindObject("myobj");
    test_assert(rop1 != NULL, "not found");
    test_assert(*(uint32_t *)(rop1->objp) == 0x55aa, "object mismatch");
    test_assert(rop == rop1, "object reference mismatch");
    test_assert(rop1->element.refs == 2, "object reference mismatch");

    rop2 = (registered_object_t *)chFactoryDuplicateReference((dyn_element_t *)rop1);
    test_assert(rop1 == rop2, "object reference mismatch");
    test_assert(*(uint32_t *)(rop2->objp) == 0x55aa, "object mismatch");
    test_assert(rop2->element.refs == 3, "object reference mismatch");

    chFactoryReleaseObject(rop2);
    test_assert(rop1->element.refs == 2, "references mismatch");

    chFactoryReleaseObject(rop1);
    test_assert(rop->element.refs == 1, "references mismatch");
  }

  /* [4.1.5] Releasing the first reference to the object, must not
     trigger an assertion.*/
  test_set_step(5);
  {
    chFactoryReleaseObject(rop);
  }

  /* [4.1.6] Retrieving the registered object by name again, must not
     exist.*/
  test_set_step(6);
  {
    rop = chFactoryFindObject("myobj");
    test_assert(rop == NULL, "found");
  }
}

static const testcase_t oslib_test_004_001 = {
  "Objects Registry",
  NULL,
  oslib_test_004_001_teardown,
  oslib_test_004_001_execute
};
#endif /* CH_CFG_FACTORY_OBJECTS_REGISTRY == TRUE */

#if (CH_CFG_FACTORY_GENERIC_BUFFERS == TRUE) || defined(__DOXYGEN__)
/**
 * @page oslib_test_004_002 [4.2] Dynamic Buffers Factory
 *
 * <h2>Description</h2>
 * This test case verifies the dynamic buffers factory.
 *
 * <h2>Conditions</h2>
 * This test is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_FACTORY_GENERIC_BUFFERS == TRUE
 * .
 *
 * <h2>Test Steps</h2>
 * - [4.2.1] Retrieving a dynamic buffer by name, must not exist.
 * - [4.2.2] Creating a dynamic buffer it must not exists, must
 *   succeed.
 * - [4.2.3] Creating a dynamic buffer with the same name, must fail.
 * - [4.2.4] Retrieving the dynamic buffer by name, must exist, then
 *   increasing the reference counter, finally releasing both
 *   references.
 * - [4.2.5] Releasing the first reference to the dynamic buffer, must
 *   not trigger an assertion.
 * - [4.2.6] Retrieving the dynamic buffer by name again, must not
 *   exist.
 * .
 */

static void oslib_test_004_002_teardown(void) {
  dyn_buffer_t *dbp;

  dbp = chFactoryFindBuffer("mybuf");
  if (dbp != NULL) {
    while (dbp->element.refs > 0U) {
      chFactoryReleaseBuffer(dbp);
    }
  }
}

static void oslib_test_004_002_execute(void) {
  dyn_buffer_t *dbp;

  /* [4.2.1] Retrieving a dynamic buffer by name, must not exist.*/
  test_set_step(1);
  {
    dbp = chFactoryFindBuffer("mybuf");
    test_assert(dbp == NULL, "found");
  }

  /* [4.2.2] Creating a dynamic buffer it must not exists, must
     succeed.*/
  test_set_step(2);
  {
    dbp = chFactoryCreateBuffer("mybuf", 128U);
    test_assert(dbp != NULL, "cannot create");
  }

  /* [4.2.3] Creating a dynamic buffer with the same name, must fail.*/
  test_set_step(3);
  {
    dyn_buffer_t *dbp1;

    dbp1 = chFactoryCreateBuffer("mybuf", 128U);
    test_assert(dbp1 == NULL, "can create");
  }

  /* [4.2.4] Retrieving the dynamic buffer by name, must exist, then
     increasing the reference counter, finally releasing both
     references.*/
  test_set_step(4);
  {
    dyn_buffer_t *dbp1, *dbp2;

    dbp1 = chFactoryFindBuffer("mybuf");
    test_assert(dbp1 != NULL, "not found");
    test_assert(dbp == dbp1, "object reference mismatch");
    test_assert(dbp1->element.refs == 2, "object reference mismatch");

    dbp2 = (dyn_buffer_t *)chFactoryDuplicateReference((dyn_element_t *)dbp1);
    test_assert(dbp1 == dbp2, "object reference mismatch");
    test_assert(dbp2->element.refs == 3, "object reference mismatch");

    chFactoryReleaseBuffer(dbp2);
    test_assert(dbp1->element.refs == 2, "references mismatch");

    chFactoryReleaseBuffer(dbp1);
    test_assert(dbp->element.refs == 1, "references mismatch");
  }

  /* [4.2.5] Releasing the first reference to the dynamic buffer, must
     not trigger an assertion.*/
  test_set_step(5);
  {
    chFactoryReleaseBuffer(dbp);
  }

  /* [4.2.6] Retrieving the dynamic buffer by name again, must not
     exist.*/
  test_set_step(6);
  {
    dbp = chFactoryFindBuffer("mybuf");
    test_assert(dbp == NULL, "found");
  }
}

static const testcase_t oslib_test_004_002 = {
  "Dynamic Buffers Factory",
  NULL,
  oslib_test_004_002_teardown,
  oslib_test_004_002_execute
};
#endif /* CH_CFG_FACTORY_GENERIC_BUFFERS == TRUE */

#if (CH_CFG_FACTORY_SEMAPHORES == TRUE) || defined(__DOXYGEN__)
/**
 * @page oslib_test_004_003 [4.3] Dynamic Semaphores Factory
 *
 * <h2>Description</h2>
 * This test case verifies the dynamic semaphores factory.
 *
 * <h2>Conditions</h2>
 * This test is only executed if the following preprocessor condition
 * evaluates to true:
 * - CH_CFG_FACTORY_SEMAPHORES == TRUE
 * .
 *
 * <h2>Test Steps</h2>
 * - [4.3.1] Retrieving a dynamic semaphore by name, must not exist.
 * - [4.3.2] Creating a dynamic semaphore it must not exists, must
 *   succeed.
 * - [4.3.3] Creating a dynamic semaphore with the same name, must
 *   fail.
 * - [4.3.4] Retrieving the dynamic semaphore by name, must exist, then
 *   increasing the reference counter, finally releasing both
 *   references.
 * - [4.3.5] Releasing the first reference to the dynamic semaphore
 *   must not trigger an assertion.
 * - [4.3.6] Retrieving the dynamic semaphore by name again, must not
 *   exist.
 * .
 */

static void oslib_test_004_003_teardown(void) {
  dyn_semaphore_t *dsp;

  dsp = chFactoryFindSemaphore("mysem");
  if (dsp != NULL) {
    while (dsp->element.refs > 0U) {
      chFactoryReleaseSemaphore(dsp);
    }
  }
}

static void oslib_test_004_003_execute(void) {
  dyn_semaphore_t *dsp;

  /* [4.3.1] Retrieving a dynamic semaphore by name, must not exist.*/
  test_set_step(1);
  {
    dsp = chFactoryFindSemaphore("mysem");
    test_assert(dsp == NULL, "found");
  }

  /* [4.3.2] Creating a dynamic semaphore it must not exists, must
     succeed.*/
  test_set_step(2);
  {
    dsp = chFactoryCreateSemaphore("mysem", 0);
    test_assert(dsp != NULL, "cannot create");
  }

  /* [4.3.3] Creating a dynamic semaphore with the same name, must
     fail.*/
  test_set_step(3);
  {
    dyn_semaphore_t *dsp1;

    dsp1 = chFactoryCreateSemaphore("mysem", 0);
    test_assert(dsp1 == NULL, "can create");
  }

  /* [4.3.4] Retrieving the dynamic semaphore by name, must exist, then
     increasing the reference counter, finally releasing both
     references.*/
  test_set_step(4);
  {
    dyn_semaphore_t *dsp1, *dsp2;

    dsp1 = chFactoryFindSemaphore("mysem");
    test_assert(dsp1 != NULL, "not found");
    test_assert(dsp == dsp1, "object reference mismatch");
    test_assert(dsp1->element.refs == 2, "object reference mismatch");

    dsp2 = (dyn_semaphore_t *)chFactoryDuplicateReference((dyn_element_t *)dsp1);
    test_assert(dsp1 == dsp2, "object reference mismatch");
    test_assert(dsp2->element.refs == 3, "object reference mismatch");

    chFactoryReleaseSemaphore(dsp2);
    test_assert(dsp1->element.refs == 2, "references mismatch");

    chFactoryReleaseSemaphore(dsp1);
    test_assert(dsp->element.refs == 1, "references mismatch");
  }

  /* [4.3.5] Releasing the first reference to the dynamic semaphore
     must not trigger an assertion.*/
  test_set_step(5);
  {
    chFactoryReleaseSemaphore(dsp);
  }

  /* [4.3.6] Retrieving the dynamic semaphore by name again, must not
     exist.*/
  test_set_step(6);
  {
    dsp = chFactoryFindSemaphore("mysem");
    test_assert(dsp == NULL, "found");
  }
}

static const testcase_t oslib_test_004_003 = {
  "Dynamic Semaphores Factory",
  NULL,
  oslib_test_004_003_teardown,
  oslib_test_004_003_execute
};
#endif /* CH_CFG_FACTORY_SEMAPHORES == TRUE */

/****************************************************************************
 * Exported data.
 ****************************************************************************/

/**
 * @brief   Array of test cases.
 */
const testcase_t * const oslib_test_sequence_004_array[] = {
#if (CH_CFG_FACTORY_OBJECTS_REGISTRY == TRUE) || defined(__DOXYGEN__)
  &oslib_test_004_001,
#endif
#if (CH_CFG_FACTORY_GENERIC_BUFFERS == TRUE) || defined(__DOXYGEN__)
  &oslib_test_004_002,
#endif
#if (CH_CFG_FACTORY_SEMAPHORES == TRUE) || defined(__DOXYGEN__)
  &oslib_test_004_003,
#endif
  NULL
};

/**
 * @brief   Objects Factory.
 */
const testsequence_t oslib_test_sequence_004 = {
  "Objects Factory",
  oslib_test_sequence_004_array
};

#endif /* (CH_CFG_USE_FACTORY == TRUE) && (CH_CFG_USE_MEMPOOLS == TRUE) && (CH_CFG_USE_HEAP == TRUE) */
