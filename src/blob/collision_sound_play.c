/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * collision_sound_play: N64 InitBlit (arcade LIB/blit.c). Looks the blit's texture up by name
 * with MBOX_FindTexture_Err(blit->Name, &blit->TexIndex, MBOX_WARN), which umerge inlined
 * as MBOX_FindTexture_Sub(name, &index, 0, MBOX_NumTexTables - 1, MBOX_WARN): that is
 * func_800B24EC, whose fifth formal `err` (MBOX_NOERR/WARN/FATAL) only selects compiled-out
 * warning prints, so the callee never reads it but every caller still passes it (proved
 * from the arcade library zmb.a: MBOX_FindTexture_Err stores its err argument to the fifth
 * outgoing slot of MBOX_FindTexture_Sub; MBOX_FindTexture stores 0 there).
 * The N64 version adds two special names: 0 selects the built-in image D_80110664 and
 * (char *)-1 selects nothing. It then resets the blit's state, alpha (255) and the five
 * crop fields to -1 (arcade Top/Bot/Left/Right/color).
 * Shaping: separate field assignments in field order (a chained `a = b = -1` reloads).
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

void collision_sound_play(Blit *blit) {
    if (blit->name == 0) {
        blit->texIndex = 0;
        blit->info = 0;
        blit->width = 0;
        blit->height = 0;
        blit->image = D_80110664;
    } else if (blit->name == (char *)-1) {
        blit->texIndex = 0;
        blit->info = 0;
        blit->width = 0;
        blit->height = 0;
        blit->image = 0;
    } else {
        blit->info = func_800B24EC(blit->name, &blit->texIndex, 0, D_80140BDC - 1, MBOX_WARN);
        blit->width = blit->info->width;
        blit->height = blit->info->height;
        blit->image = 0;
    }
    blit->state = 0;
    blit->alpha = 255;
    blit->top = -1;
    blit->bot = -1;
    blit->left = -1;
    blit->right = -1;
    blit->color = -1;
}
