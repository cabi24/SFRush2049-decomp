/* GENERATED ROM-aligned TU — segment 0x5610 (rom/lib_5610)
 * layout map 5f28eb4dc56fe3d76b0707d5fd55277a08a82e5e5499a3e3974d597d0c80bee6; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_io_wait.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/lzss_decode.s")
/* PROMOTED 2026-10-01 — inflate_flush_window
 * Source:   cloud/work/static_C/inflate_flush_window.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/inflate_flush_window.c:inflate_flush_window (score0);
 *           w26a: `gDisplayListSize = 0` (was `*(s32 *)&...`, a cast around rom_tu.h's
 *           former volatile), whole TU IDENTICAL under ido5.3s
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void inflate_flush_window(s32 arg0, s32 arg1) {
    gDisplayListHead = arg0;
    gDisplayListEnd = arg1;
    gDisplayListSize = 0;
}

/* PROMOTED 2026-09-24 — huft_alloc
 * Source:   work/auto/huft_alloc/matched.c (in-repo, locked); re-shaped 2026-10-12
 *           for the TU's ido5.3s recipe (w26a: cloud/work/frontier/w26a/sources/huft_alloc.c)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared, compiler ido5.3s (cc -S, no .loc, as1 -O1)
 * Evidence: lock:work/auto/huft_alloc/matched.c:huft_alloc (score0, standard -O2);
 *           w26a whole-TU IDENTICAL under ido5.3s (as1 -O1 keeps the final copy
 *           of a returned named local, so the sum is returned directly)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 huft_alloc(s32 arg0)
{
  int new_var;
  gDisplayListSize += arg0;
  new_var = gDisplayListSize;
  if (1)
  {
  }
  return (new_var - arg0) + gDisplayListHead;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/huft_build.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_free_window.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_stored.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_fixed.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_dynamic.s")
/* PROMOTED 2026-10-12 — inflate_block
 * Source:   cloud/work/frontier/w26a/sources/inflate_block.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w26a/sources/inflate_block.c:inflate_block (score0); rom_tu.h context: cloud/work/frontier/w26a/ctx53s.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "types.h"
extern u32 gInflateBitBuf, gInflateBitCount;
extern u8 *gInflateInPtr, *gInflateInEnd;
extern s32 inflate_read_bits(void), inflate_dynamic(void), inflate_stored(void), inflate_fixed(void);
#define NEEDBITS(n) do { while (k < (n)) { b |= (u32)(gInflateInPtr < gInflateInEnd ? (gInflateInPtr += 2, gInflateInPtr[-2] | (gInflateInPtr[-1] << 8)) : inflate_read_bits()) << k; k += 16; } } while (0)
#define DUMPBITS(n) do { b >>= (n); k -= (n); } while (0)
s32 inflate_block(s32 *last) {
    u32 t;
    register u32 b = gInflateBitBuf, k = gInflateBitCount;

    NEEDBITS(1);
    *last = b & 1;
    DUMPBITS(1);
    NEEDBITS(2);
    t = b & 3;
    DUMPBITS(2);
    gInflateBitBuf = b;
    gInflateBitCount = k;
    if (t == 2) return inflate_dynamic();
    if (t == 0) return inflate_stored();
    if (t == 1) return inflate_fixed();
    return 1;
}

/* PROMOTED 2026-10-12 — inflate_loop
 * Source:   cloud/work/frontier/w26a/sources/inflate_loop.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w26a/sources/inflate_loop.c:inflate_loop (score0); rom_tu.h context: cloud/work/frontier/w26a/ctx53s.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern s32 inflate_block(s32 *last);
s32 inflate_loop(void) {
    s32 e;

    gInflateBitCount = 0;
    gInflateBitBuf = 0;
    do {
        if (inflate_block(&e) != 0) {
            return 1;
        }
    } while (!e);
    return 0;
}

/* PROMOTED 2026-10-12 — inflate_read_bits
 * Source:   cloud/work/frontier/w26a/sources/inflate_read_bits.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w26a/sources/inflate_read_bits.c:inflate_read_bits (score0); rom_tu.h context: cloud/work/frontier/w26a/ctx53s.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "PR/os_message.h"
#include "PR/os_pi.h"
extern u32 gInflateSrc;
extern s32 gInflateToggle;
extern u8 gInflateBufferA[], gInflateBufferB[];
extern OSIoMesg gInflateDmaState;
extern OSMesgQueue gInflateMsgQueue;
extern void osInvalDCache(void *, s32);
extern s32 osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32, OSMesgQueue *);
s32 inflate_read_bits(void) {
    OSMesg msg;
    u8 *next;

    while (osRecvMesg(&gInflateMsgQueue, &msg, OS_MESG_NOBLOCK) == -1) {
    }
    if (gInflateToggle) {
        gInflateInPtr = gInflateBufferA;
        next = gInflateBufferB;
        gInflateToggle = 0;
    } else {
        gInflateInPtr = gInflateBufferB;
        gInflateToggle = 1;
        next = gInflateBufferA;
    }
    gInflateInEnd = gInflateInPtr + 4096;
    gInflateSrc += 4096;
    osInvalDCache(gInflateInPtr, 4096);
    osPiStartDma(&gInflateDmaState, 0, 0, gInflateSrc, next, 4096, &gInflateMsgQueue);
    gInflateInPtr += 2;
    return gInflateInPtr[-2] | (gInflateInPtr[-1] << 8);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_entry.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_entry_alt.s")
