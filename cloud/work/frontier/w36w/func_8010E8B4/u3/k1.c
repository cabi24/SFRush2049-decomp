/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010E8B4 (0x8010E8B4, 340 B) = the pause/option-screen init arm of GC initScreen@optionscr.c (else-arm:
 * gControllerRemoved = !gControllerData[playerInPause], HAL_SetFont(13), _TitlePane, TB_GetFontLineHeight,
 * InitPane). sound_loop_set (= _TitlePane) is defined in this file too (w28c body).
 * DRAFT (w29b): unit 3/85 words with `blob_unit ... --internal sound_loop_set` (was 21/85, frame 96/80).
 * Levers that worked: the controller byte read into a named s8 `c` (retail keeps it in v0: 21 -> 7 incl. frame),
 * `D_8011AD70[1] + D_8011AD70[0] + 8` operand order (a2 = v1 + t6 as retail), `h` (s32) holding the
 * unnarrowed line-height expression.
 * Frame: retail 96; `s32 u0` is an UNUSED local = frame residual (80 without it and h; 96 with h + u0) -- not
 * proven to be the real source; disclose if kept.
 * Residual (3 words): retail narrows the line height through ring temps (`sll t8,t0; sra t9,t8`, uses t9) and
 * keeps the unnarrowed value in t0; ours narrows in place (`sra t0,t8`, ugen `sra $25; move $8,$25` merged by
 * as1). Same family residual as the `x - (w>>1)` state_utility narrowing in func_8010B5D0/B7FC/BC84.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

extern OSMesgQueue D_801461D0;
extern s8 D_8011AD38;
extern u32 D_801174B4;
extern s8 D_8011AD6C;
extern s32 D_8015698C;
extern s8 D_80156CF0[][16];
extern s32 D_8011AD70[3];
extern void *D_8011AD68;

typedef struct TblHdr {
    u8 pad0[0xD4];
    char *title;
    u8 padD8[0x3A4 - 0xD8];
    char *ctrl;
} TblHdr;

extern TblHdr *D_8017A4E4;

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void fcvt_wrapper(char *, char *, ...);
u32 object_manager_update(void *, s32);
void crowd_cheer_play(void *, s32, s32, s32, s32);
s16 object_bytes_sum_global(void);
void *ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);

static void gfx_lock(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

static void gfx_unlock(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

static s32 font_select(s32 font)
{
    return slot_state_setup(font);
}

static s32 font_set(s32 font)
{
    s32 old;

    gfx_lock();
    old = font_select(font);
    gfx_unlock();
    return old;
}

void TitlePane(void)
{
    s32 width;
    s32 height;
    s32 x;
    char str1[64];

    font_set(13);
    if (D_8011AD6C) {
        fcvt_wrapper(str1, "CONTROLLER %d %s", D_8015698C + 1, D_8017A4E4->ctrl);
        width = object_manager_update(str1, -1);
    } else {
        width = object_manager_update(D_8017A4E4->title, -1);
    }
    if (D_8011AD68) {
        height = object_bytes_sum_global();
        x = width / 2;
        crowd_cheer_play(D_8011AD68, 160 - x - 8, 6, x + 168, height + 14);
    } else {
        height = object_bytes_sum_global();
        x = width / 2;
        D_8011AD68 = ambient_sound_set(160 - x - 8, 6, x + 168, height + 14, 176, 0, 0, 0);
    }
}

typedef struct { s32 x, w, h; } MenuRect;
s32 func_8010E8B4(void *item)
{
    s32 u0;
    s16 y;
    s32 h;
    s8 c;

    if (!D_8011AD38) {
        if (!(D_801174B4 & 0x7C03FFFE)) {
            font_set(13);
            c = D_80156CF0[D_8015698C][0];
            D_8011AD6C = c == 0;
            TitlePane();
            h = object_bytes_sum_global() * 2 + 10;
            ambient_sound_set(D_8011AD70[0] - 8, (s16)h - 4, D_8011AD70[1] + D_8011AD70[0] + 8, (s16)h + D_8011AD70[2] + 4,
                              176, 0, 0, 0);
        }
        ambient_sound_set(20, 136, 300, 204, 176, 0, 0, 0);
        D_8011AD38 = 1;
    }
    return 1;
}
