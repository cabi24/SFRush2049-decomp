/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* PARTIAL-SOURCE closure: complete semantic helpers; not compiled or matched. */
#include "layouts.h"

/* Actual private AA8C callee. Source formal is one s16, not a hidden ABI parameter.
 * Native entry uses s4 and unsaved s0-s3; keep with the future complete genuine root.
 */
static void func_8038A95C(s16 player)
{
    s32 i;
    for (i = 0; i < 5; i++) {
        if (D_80399550[player].handles[i] != -1) {
            sound_call_minimal((s16) D_80399550[player].handles[i]);
            D_80399550[player].handles[i] = -1;
        }
    }
    if (D_80399550[player].extra_handle != -1) {
        sound_call_minimal((s16) D_80399550[player].extra_handle);
        D_80399550[player].extra_handle = -1;
        D_80399550[player].state139 = -1;
    }
}

/* Actual private AA8C callee. Native entry uses a0 and unsaved s0-s3.
 * The comparison uses the complete s32 slot; only the removal argument narrows.
 */
static void func_8038AA14(s16 player)
{
    s32 i;
    for (i = 0; i < 5; i++) {
        if (D_80399120[player].handles[i] != -1) {
            sound_call_minimal((s16) D_80399120[player].handles[i]);
            D_80399120[player].handles[i] = -1;
        }
    }
}
