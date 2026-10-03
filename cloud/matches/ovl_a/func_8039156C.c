/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
typedef struct { u8 b0; u8 id; u8 pad[6]; } Slot8;
typedef struct { s32 a; s8 b; s8 c; u8 pad[6]; } Rec12;
extern Slot8 D_80153E88[];
extern Rec12 D_803BA230[];

void func_8039156C(void)
{
    s32 i;

    for (i = 0; i < 13; i++) {
        D_80153E88[i].id = i;
    }
    for (i = 0; i < 16; i++) {
        D_803BA230[i].a = -1;
        D_803BA230[i].c = -1;
        D_803BA230[i].b = 0;
    }
}
