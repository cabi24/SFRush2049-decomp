/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800193C8.c to the production canonical
 * types (include/sequence_context.h); code unchanged. Wave 4. */
#include "sequence_context.h"

extern unsigned char D_8002C630;
/* On the N64 ABI, unsigned 32-bit byte-offset arithmetic removes the
 * translator's high tag bit before recovering a pointer to its validated row.
 * Do not use the tagged word directly as a C array subscript.
 */
unsigned char func_800193C8(unsigned int identifier)
{
    unsigned int index;
    if (D_8002C630) {
        index = func_80017644(identifier);
        if (index != 0xFFFFFFFF) return (*(SequenceContext *)((unsigned int)D_80043EB8 +
                index * sizeof(SequenceContext))).channelFC4;
    }
    return 0;
}
