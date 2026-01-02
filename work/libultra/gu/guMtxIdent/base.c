/*
 * Function: guMtxIdent
 * Address:  0x8000941C
 * Category: libultra/gu
 * Status:   WIP
 *
 * Create N64 fixed-point identity matrix
 * Calls guMtxIdentF to create float identity, then guMtxF2L to convert
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

/* Forward declarations */
extern void guMtxIdentF(f32 mf[4][4]);
extern void guMtxF2L(f32 mf[4][4], u32 *m);

void guMtxIdent(u32 *m) {
    f32 mf[4][4];

    guMtxIdentF(mf);
    guMtxF2L(mf, m);
}
