/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Exact genuine w8a predicate; all D_zz/zz_caller stand-ins omitted. */
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

