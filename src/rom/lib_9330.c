/* GENERATED ROM-aligned TU — segment 0x9330 (rom/lib_9330)
 * layout map 030152813d73343687c493ddf9a7a7f8fe1c8cd4cdb4d934e7fc9618169c2e19; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* Canonical SDK math macros and real readonly constants; no new storage. */
#undef ABS
#define ABS(d) (((d)>0)?(d):-(d))
#define ROUND(d) (int)(((d)>=0.0)?((d)+0.5):((d)-0.5))
extern const f64 gSinCoeffs[5],gTwoOverPi,gPiOver2Hi,gPiOver2Lo;
extern const f64 gCosCoeffs[5],gCosAngleScale,gCosPiOver2Hi,gCosPiOver2Lo;
extern const f32 gNaN,gCosOne,gNaNf;


#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_9330/sinf.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_9330/cosf.s")
