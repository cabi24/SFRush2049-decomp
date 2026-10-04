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
void credits_scroll(void)
{
    char buf[76];
    s32 w;

    font_set(13);
    if (D_8011064C) {
        fcvt_wrapper(buf, D_8011F6B0, D_8015698C + 1, countdown_object[233]);
        w = object_manager_update(buf, -1);
    } else {
        w = object_manager_update(countdown_object[52], -1);
    }
    if (D_80110648) {
        crowd_cheer_play(D_80110648, 152 - w / 2, 6, w / 2 + 168, object_bytes_sum_global() + 14);
    } else {
        D_80110648 = ambient_sound_set(152 - w / 2, 6, w / 2 + 168, object_bytes_sum_global() + 14, 176, 0, 0, 0);
    }
}


void time_result_display(void)
{
    char buf[76];
    s32 w;

    font_set(13);
    if (D_80116DA8) {
        fcvt_wrapper(buf, D_80121004, D_8015698C + 1, countdown_object[233]);
        w = object_manager_update(buf, -1);
    } else {
        w = object_manager_update(countdown_object[50], -1);
    }
    if (D_80116DA4) {
        crowd_cheer_play(D_80116DA4, 152 - w / 2, 6, w / 2 + 168, object_bytes_sum_global() + 14);
    } else {
        D_80116DA4 = ambient_sound_set(152 - w / 2, 6, w / 2 + 168, object_bytes_sum_global() + 14, 176, 0, 0, 0);
    }
}

