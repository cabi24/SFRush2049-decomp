/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * PROVISIONAL (w8a): func_800D8078(s8 car) -- predicate used twice by func_800D91A0 in its step-up/step-down
 * selection loops: with idx = D_8014A0F8[car] (s16) and row = D_8013C068[D_8014A118[car].row] (10-byte s8 rows,
 * 76-byte per-car records with the row number at +1), returns row[idx] != 23 && D_80114060[row[idx]] == 1 &&
 * (idx != 2 || row[2] == 25 || row[2] == 19) && (idx < 3 || (row[idx] != 19 && row[idx] != 25)).
 *
 * Internal: retail's caller keeps t0-t5 live across the call and the body uses only the t6-t9 ring, which
 * only happens with the function internal (kept: 25/55 words). Proven with stand-in callers zz_caller /
 * zz_caller2 (not real code). Shaping: `row[idx]` written out at every use (a `c` local gives a copy or a
 * pointer-compare rewrite); `row` declared/assigned before `idx`.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

typedef struct { u8 pad0; u8 row; u8 pad2[74]; } Rec76;
extern Rec76 D_8014A118[];
extern s16 D_8014A0F8[];
extern s8 D_8013C068[][10];
extern s8 D_80114060[];

s32 func_800D8078(s8 car)
{
    s8 *row = D_8013C068[D_8014A118[car].row];
    s16 idx = D_8014A0F8[car];

    return row[idx] != 23 && D_80114060[row[idx]] == 1 && (idx != 2 || row[2] == 25 || row[2] == 19) &&
           (idx < 3 || (row[idx] != 19 && row[idx] != 25));
}

extern s8 D_zz[];
s32 zz_caller(s32 a)
{
    s32 n = 0;
    while (!func_800D8078(a)) { n++; D_zz[n]++; }
    return n;
}
s32 zz_caller2(s32 a)
{
    s32 n = 0;
    while (!func_800D8078(a + 1)) { n += 3; D_zz[n]--; }
    return n;
}
