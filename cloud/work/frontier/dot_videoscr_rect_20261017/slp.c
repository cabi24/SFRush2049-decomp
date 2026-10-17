/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * sound_loop_set (0x800B574C, 332 B) = GC _TitlePane@optionscr.c (GC locals width, str1[64]; N64 adds height and
 * x = width/2, all used). Option/pause screen title pane: font 13; "CONTROLLER %d %s" (playerInPause + 1, StrTable
 * +0x3A4) when gControllerRemoved (D_8011AD6C), else the screen title (StrTable +0xD4); the pane gTitlePane
 * (D_8011AD68) is resized (ChangePaneSize = crowd_cheer_play) or created (InitPane = ambient_sound_set).
 * Body: w28c. Internal function (unsaved s0,s1,s2,s4: its callers physics_sym and func_8010E8B4 save them).
 * Shaping (disclosed):
 *  - the HUD family's inlined lock/font helpers (gfx_lock/gfx_unlock/font_select/font_set) as non-static,
 *    uniquely named functions not on keep (w17j: static ones leave symbol-less stubs after func_800B5688).
 *  - the four helpers are written on ONE source line. With a single inlined call site umerge keeps the helpers'
 *    .loc lines and as1 then breaks the jal-osSendMesg delay-slot tie the other way (`move s0,v0` instead of
 *    `move a2,zero`, 4 words). In the unit with func_8010E8B4 also C (two call sites of font_set) the inlined
 *    code carries no .loc and matches either way (w28c). One line = no line-number difference = retail order.
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

void gfx_lock_slp(void) { osRecvMesg(&D_801461D0, (void *)0, 1); } void gfx_unlock_slp(void) { osSendMesg(&D_801461D0, (void *)0, 0); } s32 font_select_slp(s32 font) { return slot_state_setup(font); } s32 font_set_slp(s32 font) { s32 old; gfx_lock_slp(); old = font_select_slp(font); gfx_unlock_slp(); return old; }

void TitlePane(void)
{
    s32 width;
    s32 height;
    s32 x;
    char str1[64];

    font_set_slp(13);
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
