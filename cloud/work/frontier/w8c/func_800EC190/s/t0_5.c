typedef signed char s8;
typedef unsigned char u8;
typedef short s16;

typedef struct Slot {
    s8 owner;       /* 0 */
    u8 pad1[4];
    s8 ordinal;     /* 5 */
    u8 flags;       /* 6 */
    s8 index;       /* 7 */
} Slot;

extern s8 D_8014978C;
extern volatile s16 D_801543CA;
extern Slot D_80153E88[6];
extern s16 D_801163F4;
extern s8 D_8014A118;
extern s8 D_80150C04;
extern s8 D_80150F14;

void func_800EC190(s8 owner)
{
    s16 v;
    s16 n;
    s16 i;

    D_8014978C = owner;
    D_801543CA = 6;
    n = 0;
    for (i = 0; i < 6; i++) {
        if (i >= 6) {
            D_80153E88[i].flags = 0;
            D_80153E88[i].index = i + 6;
        } else {
            D_80153E88[i].owner = D_8014978C;
            D_80153E88[i].ordinal = n++;
            D_80153E88[i].index = 0;
            D_80153E88[i].flags = 176;
        }
    }
    v = D_801163F4 + 1;
    if (v >= 6) {
        v = 0;
    }
    D_8014A118 = v;
    D_80150C04 = v;
    D_80150F14 = 2;
    D_801163F4 = v;
}
