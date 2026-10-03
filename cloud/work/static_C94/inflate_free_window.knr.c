/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
typedef struct C94Huft C94Huft;
struct C94Huft { u8 extra, bits; union { u16 value; C94Huft *table; } data; };
extern u16 gInflateMaskBits[];
extern u8 *gInflateInPtr, *gInflateInEnd, *gInflateOutPtr;
extern u32 gInflateBitBuf, gInflateBitCount;
extern s32 inflate_read_bits(void);
extern void *memcpy(void *, const void *, u32);
#define C94_NEED(n) do { while (count < (n)) { u32 word; if (gInflateInPtr < gInflateInEnd) { gInflateInPtr += 2; word = (gInflateInPtr[-1] << 8) | gInflateInPtr[-2]; } else word = inflate_read_bits(); buffer |= word << count; count += 16; } } while (0)
#define C94_DROP(n) do { buffer >>= (n); count -= (n); } while (0)
s32 inflate_free_window(literals, distances, literal_bits, distance_bits)
C94Huft *literals, *distances;
s32 literal_bits, distance_bits;
{
    register u32 extra, length;
    register C94Huft *entry;
    u32 literal_mask, distance_mask;
    register u32 buffer, count;
    s32 displacement;
    literal_mask = gInflateMaskBits[literal_bits];
    distance_mask = gInflateMaskBits[distance_bits];
    buffer = gInflateBitBuf;
    count = gInflateBitCount;
    for (;;) {
        C94_NEED(literal_bits);
        entry = literals + (buffer & literal_mask);
        extra = entry->extra;
        if (extra > 16) {
            do {
                if (extra == 99) return 1;
                C94_DROP(entry->bits);
                extra -= 16;
                C94_NEED(extra);
                entry = entry->data.table + (buffer & gInflateMaskBits[extra]);
                extra = entry->extra;
            } while (extra > 16);
        }
        C94_DROP(entry->bits);
        if (extra == 16) {
            *gInflateOutPtr++ = entry->data.value;
        } else if (extra == 15) {
            break;
        } else {
            C94_NEED(extra);
            length = (buffer & gInflateMaskBits[extra]) + entry->data.value;
            C94_DROP(extra);
            C94_NEED(distance_bits);
            entry = distances + (buffer & distance_mask);
            extra = entry->extra;
            if (extra > 16) {
                do {
                    if (extra == 99) return 1;
                    C94_DROP(entry->bits);
                    extra -= 16;
                    C94_NEED(extra);
                    entry = entry->data.table + (buffer & gInflateMaskBits[extra]);
                    extra = entry->extra;
                } while (extra > 16);
            }
            C94_DROP(entry->bits);
            C94_NEED(extra);
            displacement = -entry->data.value - (buffer & gInflateMaskBits[extra]);
            C94_DROP(extra);
            if (displacement < -8) {
                memcpy(gInflateOutPtr, gInflateOutPtr + displacement, length);
                gInflateOutPtr += length;
            } else {
                while (length--) {
                    *gInflateOutPtr = gInflateOutPtr[displacement];
                    gInflateOutPtr++;
                }
            }
        }
    }
    gInflateBitBuf = buffer;
    gInflateBitCount = count;
    return 0;
}
