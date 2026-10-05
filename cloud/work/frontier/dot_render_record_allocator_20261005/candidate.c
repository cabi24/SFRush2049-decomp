/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research NONMATCH: renderer-record allocator, 240 bytes at 0x800A79F4.
 * Fields word0/word4 carry opaque native 32-bit image/info references.
 * Payload arguments are unsigned bit carriers, matching the accepted setter's
 * interface and making crop subtraction/narrowing defined modulo 2^16.
 * Record is an observed 32-byte view; no production type is changed. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct {
    u32 word0, word4;
    u16 half8, halfA, halfC, padE, half10, half12;
    u8 alpha, flip;
    s8 state;
    u8 flags;
    u16 top, bot, left, right;
} Record;
typedef char RecordMustBe32Bytes[(sizeof(Record) == 32) ? 1 : -1];
extern Record D_80140BF0[];
extern s32 D_801613AC, D_8013C234;

s32 func_800A79F4(u32 h8, u32 w4, u32 w0, u32 hA, u32 hC, u32 h10, u32 h12)
{
    s32 i;
    Record *r;
    for (i = 0; i < D_801613AC; i++) {
        if (D_80140BF0[i].state == 2)
            break;
    }
    if (i >= 200)
        return -1;
    r = &D_80140BF0[i];
    if (i >= D_801613AC)
        D_801613AC++;
    if (D_8013C234 < D_801613AC)
        D_8013C234 = D_801613AC;
    r->word4 = w4;
    r->word0 = w0;
    r->half8 = h8;
    r->halfA = hA;
    r->padE = 0;
    r->alpha = 255;
    r->top = 0;
    r->bot = 0;
    r->left = h12 - 1;
    r->right = h10 - 1;
    r->state = 0;
    r->flip = 0;
    r->half10 = h10;
    r->half12 = h12;
    r->halfC = hC;
    return i;
}
