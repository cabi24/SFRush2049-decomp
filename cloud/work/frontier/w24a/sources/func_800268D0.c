/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_800268D0 - read one frame from the stream's 32-bit input ring (stream->ring[1024] at 0x1A0,
 * bit position stream->pos at 0x11C8) and decode it into OUT (160 samples). A frame header bit selects a
 * 4-bit silence run (frames of zeros, bzero 320 bytes) or a 7-bit run of coded frames; each coded frame is
 * the 76 GSM 06.10 parameters at their standard widths (LARc 6,6,5,5,4,4,3,3; per sub-frame Nc 7, bc 2,
 * Mc 2, xmaxc 6, xmc 13 x 3), LSB first, read with the GETBITS macro (fast path when more than N bits
 * are left, else straddle into the next ring word). Then func_80026558 (Gsm_Decoder).
 * Levers (w24a): (1) the header bit is read into j (the sub-frame loop variable reused; a separate
 * `flag` local is coloured a0 instead of t3: true=45); (2) six named scalar locals (j, left, word, pos,
 * S, ring - all used): retail's 0xF8 frame holds a 4-byte home for each (left at 0x58, pos at 0x50
 * when spilled around the calls); without S/ring the frame is 8 bytes short (true=45 strict=379).
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
typedef struct GsmStream_26F60 {
    GsmState_26F60 gsm;
    unsigned char unknown190[16];
    unsigned int ring[1024];
    unsigned char unknown11A0[40];
    int pos;
} GsmStream_26F60;
extern void func_80026558(GsmState_26F60 *, short *, short *, short *, short *, short *, short *, short *);
extern void bzero(void *, int);

#define GETBITS(dst, n) if (left > n) { dst = word & ((1 << n) - 1); word >>= n; left -= n; } else { dst = ((1 << left) - 1) & word; pos += 32; word = ring[(pos >> 5) & 0x3FF]; if (left != n) dst |= (word & ((1 << (n - left)) - 1)) << left; word >>= n - left; left += 32 - n; }

int func_800268D0(GsmStream_26F60 *stream, short *out)
{
    short LARc[8], Nc[4], Mc[4], bc[4], xmaxc[4], xmc[13 * 4];
    int j;
    int left;
    unsigned int word;
    int pos;
    GsmState_26F60 *S;
    unsigned int *ring;

    S = &stream->gsm;
    ring = stream->ring;
    pos = stream->pos;
    word = ring[(pos >> 5) & 0x3FF] >> (pos & 31);
    left = 32 - (pos & 31);
    if (S->repeat == 0 && S->silence == 0) {
        GETBITS(j, 1);
        if (j) {
            GETBITS(S->silence, 4);
            S->silence++;
        } else {
            GETBITS(S->repeat, 7);
            S->repeat++;
        }
    }
    if (S->silence != 0) {
        bzero(out, 320);
        S->silence--;
    } else {
        GETBITS(LARc[0], 6);
        GETBITS(LARc[1], 6);
        GETBITS(LARc[2], 5);
        GETBITS(LARc[3], 5);
        GETBITS(LARc[4], 4);
        GETBITS(LARc[5], 4);
        GETBITS(LARc[6], 3);
        GETBITS(LARc[7], 3);
        for (j = 0; j < 4; j++) {
            GETBITS(Nc[j], 7);
            GETBITS(bc[j], 2);
            GETBITS(Mc[j], 2);
            GETBITS(xmaxc[j], 6);
            GETBITS(xmc[j * 13 + 0], 3);
            GETBITS(xmc[j * 13 + 1], 3);
            GETBITS(xmc[j * 13 + 2], 3);
            GETBITS(xmc[j * 13 + 3], 3);
            GETBITS(xmc[j * 13 + 4], 3);
            GETBITS(xmc[j * 13 + 5], 3);
            GETBITS(xmc[j * 13 + 6], 3);
            GETBITS(xmc[j * 13 + 7], 3);
            GETBITS(xmc[j * 13 + 8], 3);
            GETBITS(xmc[j * 13 + 9], 3);
            GETBITS(xmc[j * 13 + 10], 3);
            GETBITS(xmc[j * 13 + 11], 3);
            GETBITS(xmc[j * 13 + 12], 3);
        }
        S->repeat--;
        func_80026558(S, LARc, Nc, bc, Mc, xmaxc, xmc, out);
    }
    stream->pos = (pos & ~31) - left + 32;
    return 0;
}
