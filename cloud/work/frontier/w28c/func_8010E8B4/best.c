/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010E8B4 (340 B) DRAFT + sound_loop_set (= GC _TitlePane@optionscr.c) in one file. Score with `blob_unit
 * ... --internal sound_loop_set`: sound_loop_set EQUAL (83 words; physics_sym stays EQUAL), func_8010E8B4 21/85,
 * ops 0, frame 96/80. draft_check (v2 body) EQUIVALENT 571 calls. Residual: 16 B more frame above font_set's
 * `old` (retail old at 68 as ours) and the ring position of the controller-present load (retail v0).
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

void sound_loop_set(void)
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

s32 func_8010E8B4(void *item)
{
    s16 y;

    if (!D_8011AD38) {
        if (!(D_801174B4 & 0x7C03FFFE)) {
            font_set(13);
            D_8011AD6C = D_80156CF0[D_8015698C][0] == 0;
            sound_loop_set();
            y = object_bytes_sum_global() * 2 + 10;
            ambient_sound_set(D_8011AD70[0] - 8, y - 4, D_8011AD70[0] + D_8011AD70[1] + 8, y + D_8011AD70[2] + 4,
                              176, 0, 0, 0);
        }
        ambient_sound_set(20, 136, 300, 204, 176, 0, 0, 0);
        D_8011AD38 = 1;
    }
    return 1;
}
