typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define TRUE 1
typedef struct { u32 opaque[6]; } OSMesgQueue;   /* 24-byte libultra queue */
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
extern char **countdown_object;
extern s32 state_word_a;
extern s32 D_8015698C;
typedef struct { s8 flag; s8 pad[15]; } Slot16;
extern Slot16 D_80156CF0[];
extern s8 D_80110634;
extern s8 D_8011064C;
extern s32 D_80110648;
extern s32 D_80110650[3];
extern char D_8011F6B0[];
extern s8 D_80116DA8;
extern s32 D_80116DA4;
extern char D_80121004[];
typedef struct { u32 first, second; } Pair;
extern Pair D_80118E20;
void func_800B669C(u32 first, u32 second);
extern s8 D_801613E8;
extern char D_8012029C[];
void fcvt_wrapper(char *, char *, ...);
s32 object_manager_update(char *, s32);
s16 object_bytes_sum_global(void);
void crowd_cheer_play(s32, s32, s32, s32, s32);
s32 ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);
void render_helper(f32);
void dispatch_handler(s32);
void camera_auto_follow(s32, s32, s32, s32, s32, s32, char *);
void credits_scroll(void);
void time_result_display(void);
void gfx_lock_g3d(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

void gfx_unlock_g3d(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

s32 font_set_g3d(s32 font)
{
    s32 old;

    gfx_lock_g3d();
    old = slot_state_setup(font);
    gfx_unlock_g3d();
    return old;
}
/* game_timer_display = animPanes@audioscr.c (MAT3 GameCube audioscr.c animPanes, GC 0x800662D0; w36w):
 * font_set_g3d = HAL_SetFont (inlined), credits_scroll = _TitlePane@audioscr.c, object_bytes_sum_global =
 * TB_GetFontLineHeight, ambient_sound_set = InitPane, D_80110650 = menuRect {x, w, h}.
 * Disclosed: x and buf are UNUSED locals = exact frame residual. Retail frame 120 with y at sp+116; with y
 * alone the frame is 112 (y at sp+110, 5 words differ); x puts y in retail's slot (sp+116) and buf supplies
 * the remaining 8 bytes of local area (s16 x + y + s32 + s32 also works; 8 bytes or less does not). */
typedef struct { s32 x, w, h; } MenuRect;
s32 game_timer_display(s32 arg0)
{
    s16 x;
    s16 y;
    s32 u1;
    s32 u2;

    if (!D_80110634) {
        if (!(state_word_a & 0x7C03FFFE)) {
            font_set_g3d(13);
            D_8011064C = !D_80156CF0[D_8015698C].flag;
            credits_scroll();
            y = 10 + 2 * object_bytes_sum_global();
            font_set_g3d(10);
            ambient_sound_set(((MenuRect *)D_80110650)->x - 8, y - 4,
                              ((MenuRect *)D_80110650)->x + ((MenuRect *)D_80110650)->w + 8,
                              y + ((MenuRect *)D_80110650)->h + 4, 176, 0, 0, 0);
        }
        D_80110634 = 1;
    }
    return 1;
}

s32 func_80100B8C(s32 arg0)
{
    char buf[40];   /* unused: exact frame residual (retail frame 144, 104 without it) -- see w27c RESULTS */

    render_helper(0.0f);
    func_800B669C(1, 1);
    if (D_801613E8 == 1U) {   /* 1U shares the u32 1 of the inlined func_800B669C(1, 1) stores */
        font_set_g3d(13);
        dispatch_handler(22);
        camera_auto_follow(160, 10, 320, 110, -1, 0, D_8012029C);
        camera_auto_follow(160, 190, 320, 110, -1, 0, countdown_object[33]);
    } else {
        font_set_g3d(11);
        dispatch_handler(22);
        camera_auto_follow(160, 185, 320, 110, -1, 0, countdown_object[32]);
        dispatch_handler(1);
    }
    D_80118E20.second = 3;
    D_80118E20.first = 0;
    render_helper(-1.0f);
    return 1;
}
