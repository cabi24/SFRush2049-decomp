/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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

