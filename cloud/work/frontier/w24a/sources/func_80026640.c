/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80026640 - libgsm gsm_decode.c gsm_decode(s, c, target) without the GSM_MAGIC check and
 * with the four sub-frames as a loop. The header bits as one expression per LARc, the sub-frame bytes
 * as c[j * 7 + n] (uopt strength-reduces them to the retail `move v0,a1` pointer; *c++ gives a different
 * schedule and frame, true=8150). Locals in libgsm's declaration order (stack slots match).
 * Segment 0x26F60 (unconverted asm; rom/lib_26f60 after `layout convert 0x26f60`), IDO 7.1, -g0 -O2 -mips2.
 * The segment is a fixed-point GSM 06.10 full-rate speech decoder derived from Jutta Degener and Carsten
 * Bormann's libgsm (decode.c / short_term.c / gsm_decode.c), cut down for the streamed voice player in
 * lib_26ab0 (func_80025F74 decodes one 160-sample frame per call). GsmState_26F60 = the 400-byte
 * decoder state at the head of lib_26ab0's StreamState (LARpp[2][8] at 0x168, j at 0x188 as libgsm's
 * gsm_state; repeat/silence counters at 0x18A/0x18C are this variant's run-length frame header).
 * Callees func_800277D0 (Gsm_RPE_Decoding without S), func_800278C4 (Gsm_Long_Term_Synthesis_Filtering
 * writing into S->dp0) and func_80027B44 (the rest of Gsm_Short_Term_Synthesis_Filter) lie past the
 * boot-tail code end (0x800277D0); they are extern here. w24a, wave 24. No shaping devices, no unused
 * locals, no pad arrays, no implicit int. */
typedef struct GsmState_26F60 {
    short dp0[180];
    short LARpp[2][8];
    short j;
    short repeat;
    short silence;
    unsigned char unknown18E[2];
} GsmState_26F60;
extern void func_80026558(GsmState_26F60 *, short *, short *, short *, short *, short *, short *, short *);
int func_80026640(GsmState_26F60 *s, unsigned char *c, short *target)
{
    short LARc[8], Nc[4], Mc[4], bc[4], xmaxc[4], xmc[13 * 4];
    int j;

    LARc[0] = ((c[0] & 0xF) << 2) | ((c[1] >> 6) & 0x3);
    LARc[1] = c[1] & 0x3F;
    LARc[2] = (c[2] >> 3) & 0x1F;
    LARc[3] = ((c[2] & 0x7) << 2) | ((c[3] >> 6) & 0x3);
    LARc[4] = (c[3] >> 2) & 0xF;
    LARc[5] = ((c[3] & 0x3) << 2) | ((c[4] >> 6) & 0x3);
    LARc[6] = (c[4] >> 3) & 0x7;
    LARc[7] = c[4] & 0x7;
    for (j = 0; j < 4; j++) {
        Nc[j] = (c[j * 7 + 5] >> 1) & 0x7F;
        bc[j] = ((c[j * 7 + 5] & 0x1) << 1) | ((c[j * 7 + 6] >> 7) & 0x1);
        Mc[j] = (c[j * 7 + 6] >> 5) & 0x3;
        xmaxc[j] = ((c[j * 7 + 6] & 0x1F) << 1) | ((c[j * 7 + 7] >> 7) & 0x1);
        xmc[j * 13 + 0] = (c[j * 7 + 7] >> 4) & 0x7;
        xmc[j * 13 + 1] = (c[j * 7 + 7] >> 1) & 0x7;
        xmc[j * 13 + 2] = ((c[j * 7 + 7] & 0x1) << 2) | ((c[j * 7 + 8] >> 6) & 0x3);
        xmc[j * 13 + 3] = (c[j * 7 + 8] >> 3) & 0x7;
        xmc[j * 13 + 4] = c[j * 7 + 8] & 0x7;
        xmc[j * 13 + 5] = (c[j * 7 + 9] >> 5) & 0x7;
        xmc[j * 13 + 6] = (c[j * 7 + 9] >> 2) & 0x7;
        xmc[j * 13 + 7] = ((c[j * 7 + 9] & 0x3) << 1) | ((c[j * 7 + 10] >> 7) & 0x1);
        xmc[j * 13 + 8] = (c[j * 7 + 10] >> 4) & 0x7;
        xmc[j * 13 + 9] = (c[j * 7 + 10] >> 1) & 0x7;
        xmc[j * 13 + 10] = ((c[j * 7 + 10] & 0x1) << 2) | ((c[j * 7 + 11] >> 6) & 0x3);
        xmc[j * 13 + 11] = (c[j * 7 + 11] >> 3) & 0x7;
        xmc[j * 13 + 12] = c[j * 7 + 11] & 0x7;
    }
    func_80026558(s, LARc, Nc, bc, Mc, xmaxc, xmc, target);
    return 0;
}
