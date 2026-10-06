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
    s16 lap = D_8014A0F8[car];
    s8 c = row[lap];

    return c != 23 && D_80114060[row[lap]] == 1 && (lap != 2 || row[2] == 25 || row[2] == 19) &&
           (lap < 3 || (c != 19 && c != 25));
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
