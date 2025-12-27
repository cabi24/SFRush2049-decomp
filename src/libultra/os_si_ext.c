/**
 * @file os_si_ext.c
 * @brief Extended SI (Serial Interface) functions
 *
 * Decompiled from asm/us/9330.s
 */

#include "types.h"

/**
 * Read data from SI
 * (func_80008730 - osSiReadData)
 *
 * Reads data from the Serial Interface (controller/PIF).
 *
 * @param data Buffer to receive data
 * @return 0 on success, error code on failure
 */
s32 osSiReadData(void *data) {
    /* SI read - stub */
    (void)data;
    return 0;
}

/**
 * Write data to SI
 * (func_800088F0 - osSiWriteData)
 *
 * Writes data to the Serial Interface (controller/PIF).
 *
 * @param data Data to write
 * @return 0 on success, error code on failure
 */
s32 osSiWriteData(void *data) {
    /* SI write - stub */
    (void)data;
    return 0;
}
