/*
 * Function: apply_display_mode
 * Address:  0x80001E58
 * Category: boot
 * Status:   WIP
 *
 * Apply current display mode settings by calling osSetThreadPri
 * with the viewport structure.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

extern void *gViewportStruct;
extern void osSetThreadPri(void *thread);

void apply_display_mode(void) {
    osSetThreadPri(gViewportStruct);
}
