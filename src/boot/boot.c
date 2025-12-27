/**
 * @file boot.c
 * @brief System boot/entrypoint stub
 *
 * From asm/us/1000.s
 * The actual entrypoint is handwritten assembly that clears BSS
 * and jumps to main(). This file exists for build system completeness.
 *
 * func_80000400 (entrypoint):
 * - Clears 0x58170 bytes starting at D_8002E8E0
 * - Sets stack pointer to D_80086A50
 * - Jumps to func_800020F0 (main)
 */

#include "types.h"

/* The actual entrypoint is in assembly - this is just a reference stub */
/* See asm/us/1000.s for the actual implementation */
