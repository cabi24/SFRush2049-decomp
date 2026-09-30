/*
 * A 5-bit password codec, not championship logic (names are historical).
 *   D_8012E618  bit buffer (bytes)
 *   D_801170E8  buffer size in bytes
 *   D_801170EC  payload length in bits
 *   D_801170F0  checksum length in bits (appended after the payload)
 *   D_801170F4  write position in bits
 *   D_80116FE8  character -> 5-bit value table (0xFF = invalid)
 *   D_80116FE4  pointer to the 32-character alphabet
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 D_8012E618[];
extern u16 D_801170E8;
extern u16 D_801170EC;
extern u16 D_801170F0;
extern u16 D_801170F4;
extern u8 D_80116FE8[];
extern char *D_80116FE4;

s32 championship_standings(u8 *str);
s32 func_800DC120(void);
s32 func_800DC1AC(u32 value, u32 nbits);
void tournament_trophy_award(char *out);

/* decode `str` into the bit buffer and verify its checksum */
s32 championship_standings(u8 *str)
{
    u16 i;
    u8 c;
    u8 v;
    s32 sum;
    s32 got;
    u16 pos;
    u16 j;

    for (i = 0; i < D_801170E8; i++) {
        D_8012E618[i] = 0;
    }
    D_801170F4 = 0;
    D_801170EC += D_801170F0;
    while ((c = *str) != 0) {
        v = D_80116FE8[c];
        str++;
        if (v == 0xFF) {
            D_801170EC -= D_801170F0;
            return 0;
        }
        if (!func_800DC1AC(v, 5)) {
            D_801170EC -= D_801170F0;
            return 0;
        }
    }
    D_801170EC -= D_801170F0;
    sum = func_800DC120() & ((1 << D_801170F0) - 1);
    got = 0;
    pos = D_801170EC;
    for (j = 0; j < D_801170F0; j++) {
        got |= ((D_8012E618[pos >> 3] >> (pos & 7)) & 1) << j;
        pos++;
    }
    if (sum != got) {
        return 0;
    }
    D_801170F4 = 0;
    return 1;
}

/* checksum of the payload bits */
s32 func_800DC120(void)
{
    u32 bit;
    u32 i;
    s32 sum;

    if (D_801170F0 == 0) {
        return 0;
    }
    i = 0;
    sum = 0;
    while (i < D_801170EC) {
        bit = D_8012E618[i >> 3] & (1 << (i & 7));
        i++;
        while (bit >= (1 << D_801170F0)) {
            bit >>= D_801170F0;
        }
        sum += bit;
    }
    return sum;
}

/* append the low `nbits` bits of `value` (also called by func_800F42C8) */
s32 func_800DC1AC(u32 value, u32 nbits)
{
    s32 i;
    if (nbits > 32) return 0;
    if (D_801170EC < D_801170F4 + nbits) return 0;
    for (i = 0; i < nbits; i++) {
        D_8012E618[D_801170F4 >> 3] |= (value & 1) << (D_801170F4 & 7);
        D_801170F4++;
        value >>= 1;
    }
    return 1;
}

/* append the checksum and encode the whole buffer as characters */
void tournament_trophy_award(char *out)
{
    u32 sum;
    u16 i;
    u16 pos;
    u16 total;
    u16 n;
    u16 k;
    u8 acc;
    u8 *p;
    s32 b;

    sum = func_800DC120();
    pos = D_801170EC;
    for (i = 0; i < D_801170F0; i++) {
        p = &D_8012E618[pos >> 3];
        b = pos & 7;
        *p &= ~(1 << b);
        *p |= (sum & 1) << b;
        sum >>= 1;
        pos++;
    }
    total = D_801170EC + D_801170F0;
    n = 0;
    i = 0;
    k = 0;
    acc = 0;
    for (i = 0; i < total; i++) {
        acc |= ((D_8012E618[i >> 3] >> (i & 7)) & 1) << k;
        if (k == 4) {
            out[n++] = D_80116FE4[acc];
            acc = 0;
            k = 0;
        } else {
            k++;
        }
    }
    if (k != 0) {
        out[n++] = D_80116FE4[acc];
    }
    out[n] = 0;
}

/* stand-in caller: keeps func_800DC120 out of line under -O3 */
void __standin_func_800DC120(void)
{
    func_800DC120();
}
