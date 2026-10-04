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
s32 osJamMesg(OSMesgQueue *, void *, s32);
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
static void gfx_lock(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

static void gfx_unlock(void)
{
    osJamMesg(&D_801461D0, (void *)0, 0);
}

static s32 font_set(s32 font)
{
    s32 old;

    gfx_lock();
    old = slot_state_setup(font);
    gfx_unlock();
    return old;
}
s32 game_timer_display(s32 arg0)
{
    s16 h;

    if (!D_80110634) {
        if (!(state_word_a & 0x7C03FFFE)) {
            font_set(13);
            D_8011064C = !D_80156CF0[D_8015698C].flag;
            credits_scroll();
            h = object_bytes_sum_global() * 2 + 10;
            font_set(10);
            ambient_sound_set(D_80110650[0] - 8, h - 4, D_80110650[1] + D_80110650[0] + 8,
                              h + D_80110650[2] + 4, 176, 0, 0, 0);
        }
        D_80110634 = 1;
    }
    return 1;
}

s32 func_80100B8C(s32 arg0)
{

    render_helper(0.0f);
    func_800B669C(1, 1);
    if (D_801613E8 == 1) {
        font_set(13);
        dispatch_handler(22);
        camera_auto_follow(160, 10, 320, 110, -1, 0, D_8012029C);
        camera_auto_follow(160, 190, 320, 110, -1, 0, countdown_object[33]);
    } else {
        font_set(11);
        dispatch_handler(22);
        camera_auto_follow(160, 185, 320, 110, -1, 0, countdown_object[32]);
    }
    dispatch_handler(1);
    D_80118E20.second = 3;
    D_80118E20.first = 0;
    render_helper(-1.0f);
    return 1;
}
