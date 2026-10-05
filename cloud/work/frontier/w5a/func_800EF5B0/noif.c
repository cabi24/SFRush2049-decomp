typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;
typedef signed int s32;

typedef struct TexDef {
    char name[16];
    u16 width, height;
    u8 pad[16];
} TexDef;

typedef struct Blit {
    char *name;
    void *image;
    TexDef *info;
    s16 texIndex;
    u8 unk0E[4];
    u16 state;
    u16 width, height;
    u8 alpha;
    u8 unk19[3];
    s16 top, bot, left, right, color;
} Blit;

#define MBOX_WARN 1

extern u8 D_80110664[];
extern volatile u8 D_80140BDC; /* MBOX texture-table count */
extern TexDef *func_800B24EC(char *name, s16 *index, s8 lo, s8 hi, s32 err);

extern void collision_sound_play(Blit *blit);
extern void Input_ApplyPadConfig(Blit *blit);

void func_800EF5B0(Blit *blit, char *name, s32 preserve) {
    TexDef *ti;

    blit->name = name;
    if (preserve) {
        ti = func_800B24EC(blit->name, &blit->texIndex, 0, D_80140BDC - 1, MBOX_WARN);
        blit->info = ti;
    } else {
        collision_sound_play(blit);
    }
    Input_ApplyPadConfig(blit);
}
