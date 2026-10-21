/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* game_timer_display = animPanes@audioscr.c (MAT3 GC 0x800662D0, size 0xDC). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 fontID);                  /* _HAL_SetFont */
extern s32 state_word_a;                           /* gGameState */
extern s32 D_8015698C;                             /* playerInPause */
typedef struct { s8 attached; s8 pad[15]; } ControllerData;
extern ControllerData D_80156CF0[];                /* gControllerData */
extern s8 D_80110634;                              /* panesCreated */
extern s8 D_8011064C;                              /* gControllerRemoved */
typedef struct { s32 x, w, h; } MenuRect;
extern MenuRect D_80110650;                        /* menuRect */
s16 object_bytes_sum_global(void);                 /* TB_GetFontLineHeight */
s32 ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32); /* InitPane */
void credits_scroll(void);                         /* _TitlePane */

static void _GetSemaphore(void) { osRecvMesg(&D_801461D0, (void *)0, 1); }
static void _ReleaseSemaphore(void) { osSendMesg(&D_801461D0, (void *)0, 0); }
static s32 HAL_SetFont(s32 fontID)
{
    s32 old;
    _GetSemaphore();
    old = slot_state_setup(fontID);
    _ReleaseSemaphore();
    return old;
}

s32 game_timer_display(s32 arg0)
{
    s16 x;
    s16 y;
    s32 a;
    if (!D_80110634) {
        if (!(state_word_a & 0x7c03fffe)) {
            HAL_SetFont(13);
            D_8011064C = !D_80156CF0[D_8015698C].attached;
            credits_scroll();
            y = 10 + 2 * object_bytes_sum_global();
            HAL_SetFont(10);
            ambient_sound_set(D_80110650.x - 8, y - 4, D_80110650.x + D_80110650.w + 8, y + D_80110650.h + 4, 176, 0, 0, 0);
        }
        D_80110634 = 1;
    }
    return 1;
}
