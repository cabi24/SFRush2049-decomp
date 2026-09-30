typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct { u8 b0, b1, b2, b3, b4, b5, b6, b7; } Slot;
typedef struct { u8 p[76]; } Ply;
extern s8 D_8014978C;
extern s16 D_801543CA;
extern Slot D_80153E88[];
extern s16 D_801163F4;
extern Ply D_8014A118[];
extern u8 D_80150C04;
extern u8 D_80150F14;
void func_800EC190(s8 a) {

    s16 j;
    s16 i;
    D_8014978C = a;
    D_801543CA = 6;
    j = 0;
    for (i = 0; i < 6; i++) {
        if (i < 6) {
            D_80153E88[i].b5 = j;
            j++;
            D_80153E88[i].b7 = 0;
            D_80153E88[i].b6 = 176;
            D_80153E88[i].b0 = D_8014978C;
        } else {
            D_80153E88[i].b6 = 0;
            D_80153E88[i].b7 = i + 6;
        }
    }
}
