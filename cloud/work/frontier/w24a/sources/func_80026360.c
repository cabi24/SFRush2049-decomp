/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80026360 - libgsm short_term.c Gsm_Short_Term_Synthesis_Filter head: Decoding_of_the_coded_Log_
 * Area_Ratios(LARcr, LARpp[j]) written as one expression per coefficient (no GSM_ADD/GSM_MULT_R
 * saturation: plain int arithmetic; the per-step temp1 variable of libgsm gives a different schedule,
 * true=5610), then the remaining filter in func_80027B44 and S->j ^= 1 (libgsm toggles j up front).
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
extern void func_80027B44(GsmState_26F60 *, short *, short *);
void func_80026360(GsmState_26F60 *S, short *LARcr, short *wt, short *s)
{
    short *LARpp_j = S->LARpp[S->j];

#define STEP(i, B, MIC, INVA) \
    LARpp_j[i] = (((((LARcr[i] + MIC) << 10) - (B << 1)) * INVA + 16384) >> 15) << 1

    STEP(0, 0, -32, 13107);
    STEP(1, 0, -32, 13107);
    STEP(2, 2048, -16, 13107);
    STEP(3, -2560, -16, 13107);
    STEP(4, 94, -8, 19223);
    STEP(5, -1792, -8, 17476);
    STEP(6, -341, -4, 31454);
    STEP(7, -1144, -4, 29708);
    func_80027B44(S, wt, s);
    S->j ^= 1;
}
