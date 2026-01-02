/*
 * Function: osContGetQuery
 * Address:  0x800097AC
 * Category: libultra/cont
 * Status:   WIP
 *
 * Get controller query results - wrapper for __osContGetStatus
 *
 * Compiler flags: -g0 -O1 -mips2 -G 0 -non_shared
 */

#include "types.h"

typedef struct {
    u16 type;
    u8 status;
    u8 errno;
} OSContStatus;

extern void __osContGetStatus(u8 *bitPattern, OSContStatus *status);

void osContGetQuery(OSContStatus *status) {
    u8 bitPattern;
    __osContGetStatus(&bitPattern, status);
}
