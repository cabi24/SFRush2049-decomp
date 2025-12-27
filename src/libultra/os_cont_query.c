/**
 * @file os_cont_query.c
 * @brief Controller query and status functions
 *
 * Decompiled from asm/us/A330.s
 *
 * Note: These functions are implemented in os_cont.c.
 * This file provides function prototypes and documentation.
 * The actual implementations use the PIF (Peripheral Interface)
 * to communicate with controllers via the Serial Interface.
 */

#include "types.h"
#include "PR/os_message.h"
#include "PR/os_cont.h"

/**
 * External declarations for implementations in os_cont.c
 *
 * osContStartQuery (0x80009730):
 *   Starts an async query of controller status. Call osContGetQuery()
 *   after receiving the completion message to get results.
 *
 * osContGetQuery (0x800097AC):
 *   Retrieves results of osContStartQuery(). Fills OSContStatus array
 *   with controller types and status flags.
 *
 * osContStartReadData2 (0x800097D0):
 *   Alternate version of osContStartReadData. Starts async read of
 *   controller button and stick state.
 *
 * osContGetReadData (0x80009854):
 *   Retrieves results of osContStartReadData. Fills OSContPad array
 *   with button states and analog stick positions.
 *
 * See os_cont.c for full implementations.
 */
