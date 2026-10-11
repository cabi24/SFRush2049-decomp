/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001C19C - lib_1a660 (IDO 7.1). With the audio system up (D_8002C630), set the kind byte
 * of channel CHANNEL's state to VALUE: D_8004F300[channel].kind14 (0x28-byte ChannelState).
 * Base: w17g lockonly/func_8001C19C.c (D10 body, verbatim). Relocation-label view (disclosed):
 * the byte is addressed through D_8004F314 = &D_8004F300[0].kind14, the symbol the reloc-aware
 * target names for that address (D_8004F300+0x14 links to the same bytes but scores as a
 * relocation-name difference); `[][40]` is the ChannelState stride. Same practice as
 * docs/reloc_reconciliation.md ("rewrite ... external symbol names to OURS"). */
extern unsigned char D_8002C630;
extern unsigned char D_8004F314[][40];

void func_8001C19C(unsigned char channel, unsigned char value)
{
    if (D_8002C630) {
        D_8004F314[channel][0] = value;
    }
}
