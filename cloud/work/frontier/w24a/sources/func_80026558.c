/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80026558 - libgsm decode.c Gsm_Decoder, modified: Gsm_RPE_Decoding takes no state,
 * Long_Term_Synthesis writes into S->dp0 (no drp / wt[160] copy loop), the short-term filter reads
 * S->dp0 as wt, no Postprocessing. Loop header increment order as libgsm.
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
extern void func_800277D0(short, short, short *, short *);
extern void func_800278C4(GsmState_26F60 *, short, short, short *);
extern void func_80026360(GsmState_26F60 *, short *, short *, short *);
void func_80026558(GsmState_26F60 *S, short *LARcr, short *Ncr, short *bcr, short *Mcr, short *xmaxcr,
                   short *xMcr, short *s)
{
    int j;
    short erp[40];

    for (j = 0; j <= 3; j++, xmaxcr++, bcr++, Ncr++, Mcr++, xMcr += 13) {
        func_800277D0(*xmaxcr, *Mcr, xMcr, erp);
        func_800278C4(S, *Ncr, *bcr, erp);
    }
    func_80026360(S, LARcr, S->dp0, s);
}
