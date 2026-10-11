/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80019420 - lib_17dc0 (IDO 7.1). Route selector SELECTOR of the sequence context playing
 * IDENTIFIER to CHANNEL: with the audio system up, find the context (func_80017644, locked,
 * same TU; 0xFFFFFFFF = none), store CHANNEL in its per-selector channel table (+0x528) and
 * mark the channel with kind 2 (func_8001C19C).
 * Natural form: D_80043EB8[index].channels528[selector] = channel; (scores true=10
 * reloc_blind=0: relocation NAME only, the bytes link identically).
 * Relocation-label view (disclosed shaping device): the reloc-aware target names the address
 * D_80043EB8+0x528 by its own label D_800443E0, so the table is addressed as
 * D_800443E0[index][selector] with the SequenceContext stride; same practice as
 * docs/reloc_reconciliation.md and w17g's func_8001C19C (D_8004F314). */
#include "sequence_context.h"
extern unsigned char D_8002C630;
extern unsigned char D_800443E0[][sizeof(SequenceContext)];
extern void func_8001C19C(unsigned char, unsigned char);
void func_80019420(unsigned int identifier, unsigned char selector, unsigned char channel)
{
    unsigned int index;
    if (D_8002C630) {
        index = func_80017644(identifier);
        if (index != 0xFFFFFFFF) {
            D_800443E0[index][selector] = channel;
            func_8001C19C(channel, 2);
        }
    }
}
