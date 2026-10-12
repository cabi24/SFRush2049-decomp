/* GENERATED ROM-aligned TU — segment 0x26f60 (rom/lib_26f60)
 * layout map 13381420c9b7fd120254c70fd3ba2a5fa9f90e2a163e7efec21b19d2c23e9b4a; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-12 — func_80026360
 * Source:   cloud/work/frontier/w24a/sources/func_80026360.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24a/sources/func_80026360.c:func_80026360 (score0); rom_tu.h context: cloud/work/frontier/w24a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct GsmState_26F60 { short dp0[180]; short LARpp[2][8]; short j; short repeat; short silence; unsigned char unknown18E[2]; } GsmState_26F60;
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

/* PROMOTED 2026-10-12 — func_80026558
 * Source:   cloud/work/frontier/w24a/sources/func_80026558.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24a/sources/func_80026558.c:func_80026558 (score0); rom_tu.h context: cloud/work/frontier/w24a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-12 — func_80026640
 * Source:   cloud/work/frontier/w24a/sources/func_80026640.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24a/sources/func_80026640.c:func_80026640 (score0); rom_tu.h context: cloud/work/frontier/w24a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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

/* PROMOTED 2026-10-12 — func_800268D0
 * Source:   cloud/work/frontier/w24a/sources/func_800268D0.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w24a/sources/func_800268D0.c:func_800268D0 (score0); rom_tu.h context: cloud/work/frontier/w24a/ctx71_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
typedef struct GsmStream_26F60 { GsmState_26F60 gsm; unsigned char unknown190[16]; unsigned int ring[1024]; unsigned char unknown11A0[40]; int pos; } GsmStream_26F60;
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

