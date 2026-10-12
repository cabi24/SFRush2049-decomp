/* flags: -g1 -O1 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* viDisableAccum (static, boot 0x800013DC, 24 B): disable accumulation of frame time.
 * Segment 0x1f50 (src/rom/lib_1050.c), IDO 5.3. Rush frame-timing code (VI tick counter -> scheduler
 * delta/elapsed seconds); no library ancestor. w24b: the whole 0x1f50 segment is `-g1 -O1 -Wab,-r4300_mul`
 * (the flagset of the adjacent lib_1050_sdk scheduler), not the -g0 -O2 the layout derives from the two O2
 * locks; at -g1 -O1 viDeadlinePassed is 48 B and absorbs the 20-B "viStub" slot (its two trailing `jr ra; nop`
 * pairs), see RESULTS.md. gViTickCounter / gViLastTick are `volatile` (interrupt-updated tick counter and
 * its snapshot): the retail load order in viUpdateTime's first statement needs both volatile (as1 keeps
 * volatile loads in order); TU integration needs rom_tu.h to declare them volatile (patch in RESULTS.md).
 * No shaping devices, no unused locals. */
extern int gScTimeEnabled;

void viDisableAccum(void) {
    gScTimeEnabled = 0;
}
