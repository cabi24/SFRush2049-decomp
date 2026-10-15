/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* UNCOMPILED RESEARCH, EMPTY CLAIMS. Image B, C910..CA24, 276 native bytes.
 * GC weapons.c supplies names and the two-argument callback type. N64's locked
 * func_800B27E4 establishes 21 visuals at car+0x110, unlike GC's 22 at +0x110.
 * Source correction: weapon visuals[20] remains at car+0x2f0. The earlier
 * +0xf8/22/index21 inference was wrong despite forming the same byte address.
 * N64 targets supply consumed offsets and ABI. No compiler result is inferred.
 * This is the natural embedded-visual experiment requested by the handoff.
 * It has no new score and does not supersede the historical 13/69 draft.
 *
 * All unknown arrays below/in the header preserve native persistent storage;
 * none is a local frame-shaping device. No pad locals, goto, asm, artificial
 * reads, volatile qualification or fabricated caller are used.
 *
 * AF5E0's seven s32 formals describe observed outgoing native words only.
 * They remain an ABI-facing research declaration, INCOMPATIBLE with the
 * accepted void/narrow save_slot_valid C declaration. Native AF5E0 forwards
 * E26C's sign-extended s16 scene index in v0. Shared prototype reconciliation
 * and strict compiler scoring are mandatory before integration.
 *
 * The original N64 time qualification is unproved; this experiment uses plain
 * f32. GC's volatile declaration is evidence to investigate, not a tuning knob.
 * Runtime table bytes are neither supplied nor presumed equal to GC.
 */
#include "weapons_types.h"

typedef struct WpnModel {
    WpnU8 unknown00[8];
    WpnU8 body_type;                  /* native unsigned byte load */
    WpnU8 unknown09[0x7ff];
} WpnModel;                          /* 0x808 */

extern WpnCar D_80152818[];
extern WpnCarParts D_80139320[];
extern WpnModel D_8014A250[];
extern WpnF32 D_803943A4[8][13][3];
extern WpnF32 D_8002EB90;
extern unsigned short D_80142972;
extern void func_8038AA8C(WpnVisual *, WpnS16);
extern WpnS32 func_800AF5E0(WpnS32, WpnS32, WpnS32, WpnS32,
                          WpnS32, WpnS32, WpnS32);
extern void func_8008D6FC(WpnS16, void *, void *);

void func_8038C910(WpnS16 slot)
{
    WpnVisual *v;
    WpnCarParts *parts;

    if (D_80152818[slot].visuals[20].objnum < 0) {
        v = &D_80152818[slot].visuals[20];
        v->index = 0;
        v->func = func_8038AA8C;
        v->timeStamp = D_8002EB90;
        v->slot = slot;
        parts = &D_80139320[slot];
        parts->weap = func_800AF5E0(D_80142972, 15, parts->frame,
                                  0x2000, slot, -1, 1);
        v->objnum = parts->weap;
        func_8008D6FC(v->objnum, D_803943A4[1][D_8014A250[slot].body_type], 0);
    }
}
