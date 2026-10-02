/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef int s32;
typedef signed short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef struct Record {s8 owner; u8 pad1[4]; u8 ordinal; u8 flags; u8 index;} Record;
extern s8 D_8014978C;
extern struct Count {s16 count;} D_801543CA;
extern Record D_80153E88[];
extern s16 D_801163F4;
extern struct Input {s8 index;} input_rec0;
extern s8 D_80150C04;
extern s8 D_80150F14;
void func_800EC190(s8 owner) {
    s16 i, n;
    D_8014978C = owner;
    D_801543CA.count = 6;
    for (i = 0, n = 0; i < 6; i++) {
        if (i >= 6) {D_80153E88[i].flags = 0; D_80153E88[i].index = i + 6;}
        else {
            D_80153E88[i].ordinal = n++;
            D_80153E88[i].index = 0;
            D_80153E88[i].flags = 176;
            D_80153E88[i].owner = D_8014978C;
        }
    }
    i = D_801163F4 + 1;
    if (i >= 6) i = 0;
    input_rec0.index = i;
    D_80150C04 = i;
    D_80150F14 = 2;
    D_801163F4 = i;
}
