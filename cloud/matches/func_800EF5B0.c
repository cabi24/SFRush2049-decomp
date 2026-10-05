/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800EF5B0: N64 RenameBlit (arcade LIB/blit.c, pasted). Stores the new name; with
 * `preserve` it looks the texture up again and keeps the blit's other state, otherwise it
 * re-initialises the blit with InitBlit (collision_sound_play). Either way the blit is then
 * pushed to the display with UpdateBlit (Input_ApplyPadConfig).
 * The arcade MBOX_FindTexture(name, &idx) is inlined here as MBOX_FindTexture_Sub
 * (func_800B24EC) with lo 0, hi = table count - 1 and err MBOX_WARN (retail stores 1 in the
 * fifth outgoing slot, as in InitBlit). The arcade `if (!ti) FatalMsg(...)` is compiled out;
 * the empty `if` is kept for fidelity (the bytes are the same without it).
 * Matches at -O3 only (-O2 keeps blit in s0 with a 40-byte frame).
 */
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
        if (!(ti = func_800B24EC(blit->name, &blit->texIndex, 0, D_80140BDC - 1, MBOX_WARN))) {
        }
        blit->info = ti;
    } else {
        collision_sound_play(blit);
    }
    Input_ApplyPadConfig(blit);
}
