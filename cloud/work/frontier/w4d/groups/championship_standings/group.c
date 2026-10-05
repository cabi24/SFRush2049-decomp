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

/*
 * decode `str` into the bit buffer and verify its checksum (frontier wave 2, w2c).
 * Shaping (all compile-affecting): one u16 counter `i` for both loops, set to 0 before the call to
 * func_800DC120 and again in the for-init (retail keeps it in t1 across the call, which that callee
 * preserves, and still loads the constant 0 for the loop's working copy); the table value is an int
 * local (`s32 v`; with `u8 v` uopt swaps the operands of the 0xFF compare); the checksum loop advances
 * the bit position in the for-header as `pos += 1` (with `pos++` got/pos swap registers).
 */
s32 championship_standings(u8 *str)
{
    u16 i;
    u8 c;
    s32 v;
    s32 sum;
    s32 got;
    u16 pos;

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
    i = 0;
    sum = func_800DC120() & ((1 << D_801170F0) - 1);
    got = 0;
    pos = D_801170EC;
    for (i = 0; i < D_801170F0; i++, pos += 1) {
        got |= ((D_8012E618[pos >> 3] >> (pos & 7)) & 1) << i;
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

/* append the low `nbits` bits of `value` (also called by func_800F42C8); locked source src/blob/func_800DC1AC.c, unchanged */
s32 func_800DC1AC(u32 value, u32 nbits)
{
    s32 i;
    if (nbits > 32) return 0;
    if (D_801170EC < D_801170F4 + nbits) return 0;
    for (i = 0; i < nbits; D_801170F4++, value >>= 1, i++) {
        D_8012E618[D_801170F4 >> 3] |= (value & 1) << (D_801170F4 & 7);

    }
    return 1;
}

/*
 * append the checksum to the bit buffer and encode the whole buffer as password characters
 * (5 bits per character through the D_80116FE4 alphabet), NUL-terminated, into `out`
 * (frontier wave 4, w4d).  Shaping: `i` is the bit position in both loops; the first loop's
 * counter `j` is its own variable (sharing `n` lets uopt skip re-zeroing `n` on the empty-loop
 * path); `j++, i++, sum >>= 1` in the for-header in that order; `i = 0` stated before the second
 * loop's empty for-init.
 */
void tournament_trophy_award(char *out)
{
    u32 sum;
    u16 i;
    u16 j;
    u16 n;
    u16 total;
    u16 k;
    u8 acc;
    u8 *p;
    s32 b;

    sum = func_800DC120();
    i = D_801170EC;
    for (j = 0; j < D_801170F0; j++, i++, sum >>= 1) {
        p = &D_8012E618[i >> 3];
        b = i & 7;
        *p &= ~(1 << b);
        *p |= (sum & 1) << b;
    }
    total = D_801170EC + D_801170F0;
    n = 0;
    i = 0;
    k = 0;
    acc = 0;
    for (; i < total; i++) {
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

