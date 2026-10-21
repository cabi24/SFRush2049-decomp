/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_8010E8B4 = animPanes@videoscr.c (MAT3 GameCube videoscr.c animPanes). */
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
extern u32 D_801174B4;                             /* gGameState */
extern s32 D_8015698C;                             /* playerInPause */
typedef struct { s8 attached; s8 pad[15]; } ControllerData;
extern ControllerData D_80156CF0[];                /* gControllerData */
extern s8 D_8011AD38;                              /* panesCreated */
extern s8 D_8011AD6C;                              /* gControllerRemoved */
typedef struct { s32 x, w, h; } MenuRect;
extern MenuRect D_8011AD70;                        /* menuRect */
s16 object_bytes_sum_global(void);                 /* TB_GetFontLineHeight */
void *ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32); /* InitPane */
void TitlePane(void);                              /* _TitlePane@videoscr.c */

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

s32 func_8010E8B4(s32 arg0)
{
    s32 h;
    s16 y;
    char buf[8];
    if (!D_8011AD38) {
        if (!(D_801174B4 & 0x7c03fffe)) {
            HAL_SetFont(13);
            D_8011AD6C = !D_80156CF0[D_8015698C].attached;
            TitlePane();
            h = object_bytes_sum_global() * 2 + 10;
            y = h;
            ambient_sound_set(D_8011AD70.x - 8, y - 4, D_8011AD70.x + D_8011AD70.w + 8,
                              y + D_8011AD70.h + 4, 176, 0, 0, 0);
        }
        ambient_sound_set(20, 136, 300, 204, 176, 0, 0, 0);
        D_8011AD38 = 1;
    }
    return 1;
}
