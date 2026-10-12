/* GENERATED ROM-aligned TU — segment 0x1f50 (rom/lib_1050)
 * layout map c8f0857aebf4ea1ed90dde16bb4db378a8281234d10bd7a742394c75635cc457; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */

#include "rom_tu.h"


/* PROMOTED 2026-10-12 — viTickStart
 * Source:   cloud/work/frontier/w24b/sources/viTickStart.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24b/sources/viTickStart.c:viTickStart (score0); rom_tu.h context: cloud/work/frontier/w24b/ctx53_vi.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern volatile int gViTickCounter;
extern volatile int gViLastTick;
extern f32 gScaleSecondsPerTick;
extern f32 __osScDeltaTime;
extern f32 __osScElapsedTime;
extern u32 osSetIntMask(u32);
void viTickStart(void) {
    u32 mask;
    register f32 t;

    mask = osSetIntMask(1);
    gViLastTick = gViTickCounter;
    __osScDeltaTime = t = 2.0f * gScaleSecondsPerTick;
    __osScElapsedTime = t;
    osSetIntMask(mask);
}

/* PROMOTED 2026-10-12 — viEnableAccum
 * Source:   cloud/work/frontier/w24b/sources/viEnableAccum.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24b/sources/viEnableAccum.c:viEnableAccum (score0); rom_tu.h context: cloud/work/frontier/w24b/ctx53_vi.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int gScTimeEnabled;
void viEnableAccum(void) {
    gScTimeEnabled = 1;
}

/* PROMOTED 2026-10-12 — viDisableAccum
 * Source:   cloud/work/frontier/w24b/sources/viDisableAccum.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24b/sources/viDisableAccum.c:viDisableAccum (score0); rom_tu.h context: cloud/work/frontier/w24b/ctx53_vi.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void viDisableAccum(void) {
    gScTimeEnabled = 0;
}

/* PROMOTED 2026-10-12 — viUpdateTime
 * Source:   cloud/work/frontier/w24b/sources/viUpdateTime.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24b/sources/viUpdateTime.c:viUpdateTime (score0); rom_tu.h context: cloud/work/frontier/w24b/ctx53_vi.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern int gViFrameDelta;
extern int gViAccumTotal;
extern int gViAccumTime;
void viUpdateTime(void) {
    gViFrameDelta = gViTickCounter - gViLastTick;
    gViLastTick = gViTickCounter;
    if (gScTimeEnabled) {
        gViAccumTotal += gViFrameDelta;
        gViAccumTime += gViFrameDelta;
    } else {
        __osScElapsedTime = (gViTickCounter - gViAccumTotal) * gScaleSecondsPerTick;
    }
    if (gViFrameDelta > 5) {
        gViFrameDelta = 5;
    }
    __osScDeltaTime = gViFrameDelta * gScaleSecondsPerTick;
}

/* PROMOTED 2026-10-12 — viScheduleTick
 * Source:   cloud/work/frontier/w24b/sources/viScheduleTick.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24b/sources/viScheduleTick.c:viScheduleTick (score0); rom_tu.h context: cloud/work/frontier/w24b/ctx53_vi.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern f32 gScaleTicksPerSecond;
void viScheduleTick(f32 seconds) {
    gViAccumTotal = gViTickCounter;
    gViAccumTime = (int)(seconds * gScaleTicksPerSecond) + gViTickCounter;
}

/* PROMOTED 2026-10-12 — viAddTicks
 * Source:   cloud/work/frontier/w24b/sources/viAddTicks.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24b/sources/viAddTicks.c:viAddTicks (score0); rom_tu.h context: cloud/work/frontier/w24b/ctx53_vi.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void viAddTicks(f32 seconds) {
    gViAccumTime += (int)(seconds * gScaleTicksPerSecond);
}

/* PROMOTED 2026-10-12 — viGetTimeToDeadline
 * Source:   cloud/work/frontier/w24b/sources/viGetTimeToDeadline.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24b/sources/viGetTimeToDeadline.c:viGetTimeToDeadline (score0); rom_tu.h context: cloud/work/frontier/w24b/ctx53_vi.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
f32 viGetTimeToDeadline(void) {
    return (gViAccumTime - gViTickCounter) * gScaleSecondsPerTick;
}


/* PROMOTED 2026-10-01 — viDeadlinePassed
 * Source:   cloud/work/static_C/viDeadlinePassed.c (in-repo, locked)
 * Flags:    -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul (w24s: segment flagset; re-locked
 *           against the 48-B target that absorbs the former 20-B "viStub" slot; was -g0 -O2)
 * Evidence: lock:cloud/work/static_C/viDeadlinePassed.c:viDeadlinePassed (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 viDeadlinePassed(void) {
    return (gViAccumTime - gViTickCounter) < 1;
}

