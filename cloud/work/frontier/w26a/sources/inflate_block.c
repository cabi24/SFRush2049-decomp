/* flags: -g0 -O2 -mips2 -G 0 -non_shared (compiler ido5.3s: IDO 5.3 cc -S, .loc stripped, as1 -O1) */
/* inflate_block - lib_5610 (ido5.3s). gzip inflate.c inflate_block(): read the last-block bit
 * into *last and the 2-bit block type, store the bit buffer back, and decode the block:
 * 2 = dynamic Huffman (inflate_dynamic), 0 = stored (inflate_stored), 1 = fixed
 * (inflate_fixed); any other type returns 1. Rush's NEEDBITS pulls 16-bit little-endian
 * words from the double-buffered input (gInflateInPtr..gInflateInEnd), refilling through
 * inflate_read_bits when the buffer is empty.
 * From static_C23/inflate_block.c (w25a 99/105 under the recipe): w26a writes the word as
 * `ptr[-2] | (ptr[-1] << 8)` (retail loads the high byte first; the other operand order
 * swaps the two lbu registers) and NEEDBITS as one conditional expression (a named block-scope
 * `word` compiles identically here but moves inflate_stored's `n--` temporary from v1 to v0;
 * the whole TU uses this one macro text). No shaping devices. */
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
