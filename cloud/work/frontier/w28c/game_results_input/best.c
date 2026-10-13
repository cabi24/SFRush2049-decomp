/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * game_results_input (0x800FF724, 1,732 B) = animControls(struct _tBlit *blt) of controlscr.c (GC rush_d.elf
 * animControls 0x8006BC2C: string "CONTROLS_SM", same branch structure). The N64 label is wrong: it animates
 * one controls-screen button blit (data word +0x2C packs id/conflict/uninited/player/value nibbles).
 * DRAFT: unit 157/433 words, ops 11, frame 120/120. Residual: register colour only (force.sh with
 * first=t0, blt=t1, id=t2, action=t3, p=t4, conflict=t5 leaves 27 rows; then uninited takes a3 where
 * retail uses ra: in retail a3 is unavailable to every web, cause unknown) + the srl order at entry.
 * Shaping: validOption() and Hidden() are inlined static helpers (GC has both as functions); the named
 * local `valid` inside validOption gives the retail frame (120; 112 without it).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Blit {
    u32 word0, word4, word8;
    u16 half12;
    s16 x, y;
    u16 half18;
    s16 w, h;
    u8 alpha, byte25;
    s8 hidden;
    u8 byte27;
    s16 left, right, top, bottom;
    u32 word36;
    s32 state;
    u32 data;
} Blit;

typedef struct ButtonName {
    u8 pad0[48];
    f32 pos[3];
    u8 alpha;
    u8 pad3D[3];
} ButtonName;

typedef struct Player {
    u8 b0;
    u8 control;
    u8 pad2[74];
} Player;

typedef struct TextPos {
    s32 pad0, y, pad8, padC, x, pad14;
} TextPos;

extern s16 D_8014A100[];          /* gControlFirst */
extern s16 D_8014A10A;            /* gControlMenuSize */
extern s16 gNumWindows;            /* gNumWindows */
extern s8 D_80149DA8[][10];       /* gConflict */
extern s8 D_8015418C[];           /* controlScrActive */
extern Player D_8014A118[];       /* gPlayers */
extern s32 D_80154618[];          /* buttonsInited */
extern ButtonName D_80111998[][17]; /* buttonNames */
extern u8 gCamera[][152];      /* gCamera */
extern f32 D_80112A9C;            /* gButtonScale */
extern f32 D_80112AB0;            /* buttonZ */
extern TextPos D_80113EE0[][4];
extern s32 D_801140E8;
extern s32 D_8011407C[];          /* button textures */
extern s8 D_8013C068[][10];       /* gNewControls */
extern void Input_ApplyPadConfig(Blit *);
extern void func_800EF5B0(Blit *, char *, s32);
extern void brake_light_update(s32, f32 *, void *, f32 *, s16 *);
extern void stat_race_update(Blit *, s32, s32, s32);

static s32 validOption(s32 item)
{
    s32 valid;

    valid = 1;
    if (item == 8) {
        valid = 0;
    }
    return valid;
}

static s32 Hidden(Blit *blt, s32 hide)
{
    if (blt->hidden != hide) {
        blt->hidden = hide;
        Input_ApplyPadConfig(blt);
    }
    return blt->hidden;
}

s32 animControls(Blit *blt)
{
    s32 id;
    s32 conflict;
    s32 uninited;
    s32 p;
    s32 val;
    s32 action;
    s16 i;
    s16 j;
    s16 screenPos[2];
    f32 gTextPosScale;

    conflict = (blt->data >> 4) & 15;
    p = (blt->data >> 12) & 15;
    uninited = (blt->data >> 8) & 15;
    id = blt->data & 15;
    val = ((blt->data >> 16) & 255) - 1;
    action = D_8014A100[p] + id - 1;
    gTextPosScale = D_80112A9C;
    if (gNumWindows > 1) {
        gTextPosScale *= 0.5f;
    }
    for (i = 0, j = 0; i < id; j++) {
        if (!validOption(D_8014A100[p] + j)) {
            action++;
        } else {
            i++;
        }
    }
    if (Hidden(blt, D_8014A118[p].control == 5 || (conflict && !D_80149DA8[p][action]) || action >= 10 ||
                        !D_8015418C[p] || D_8014A10A < id)) {
        return 1;
    }
    if (uninited) {
        if (gNumWindows > 1) {
            func_800EF5B0(blt, "CONTROLS_SM", 0);
        }
        if (D_80154618[p]) {
            brake_light_update(p, D_80111998[p][action].pos, gCamera[p], 0, screenPos);
            blt->x = screenPos[0] + 55.0f * gTextPosScale * 300.0f / D_80112AB0;
            blt->y = screenPos[1] - blt->h / 2;
            blt->alpha = D_80111998[p][action].alpha;
        } else {
            blt->x = D_80113EE0[gNumWindows - 1][p].x + 20;
            blt->y = D_80113EE0[gNumWindows - 1][p].y + id * 16 + 8;
        }
        if (conflict) {
            blt->x += (gNumWindows == 1) ? 20 : 10;
        }
        Input_ApplyPadConfig(blt);
        blt->data &= 0x00FFF0FF;
        if (conflict) {
            blt->data = (blt->data & 0xFFFF) | 0x1C0000;
            stat_race_update(blt, D_801140E8, blt->h, blt->h);
        }
    }
    if (D_80154618[p]) {
        brake_light_update(p, D_80111998[p][action].pos, gCamera[p], 0, screenPos);
        blt->x = screenPos[0] + 75.0f * gTextPosScale * 300.0f / D_80112AB0;
        blt->y = screenPos[1] - blt->h / 2;
        blt->half18 = 0x7F00;
        if (conflict) {
            blt->x += (gNumWindows == 1) ? 20 : 10;
        }
        if (D_80111998[p][action].alpha >= 255) {
            blt->alpha = 254;
        } else {
            blt->alpha = D_80111998[p][action].alpha;
        }
        if (Hidden(blt, blt->alpha == 0)) {
            return 1;
        }
        Input_ApplyPadConfig(blt);
    } else {
        blt->alpha = 254;
        Input_ApplyPadConfig(blt);
    }
    if (conflict) {
        return 1;
    }
    if (val != D_8013C068[D_8014A118[p].control][action]) {
        val = D_8013C068[D_8014A118[p].control][action];
        blt->data = (blt->data & 0xFFFF) | (((val + 1) << 16) & 0xFF0000);
        if (val == 25 && (action == 0 || action == 1)) {
            val++;
        }
        stat_race_update(blt, D_8011407C[val], blt->h, blt->h);
    }
    return 1;
}
