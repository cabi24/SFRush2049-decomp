/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete time_result_display body from the accepted credits-scroll context. */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct FiveParts FiveParts;
extern OSMesgQueue D_801461D0;
extern char **countdown_object;
extern s32 D_8015698C;
extern s8 D_80116DA8;
extern FiveParts *D_80116DA4;
extern char D_80121004[];
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osJamMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32);
extern void fcvt_wrapper(char *, char *, ...);
extern s32 object_manager_update(char *, s32);
extern s16 object_bytes_sum_global(void);
extern void crowd_cheer_play(FiveParts *,s32,s32,s32,s32);
extern FiveParts *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
static void gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0, 0, 0); }
static s32 font_set(s32 font)
{
    s32 old;
    gfx_lock();
    old = slot_state_setup(font);
    gfx_unlock();
    return old;
}
void time_result_display(void)
{
    char buf[76];
    s32 w;
    font_set(13);
    if (D_80116DA8) {
        fcvt_wrapper(buf,D_80121004,D_8015698C+1,countdown_object[233]);
        w=object_manager_update(buf,-1);
    } else {
        w=object_manager_update(countdown_object[50],-1);
    }
    if (D_80116DA4) {
        crowd_cheer_play(D_80116DA4,152-w/2,6,w/2+168,object_bytes_sum_global()+14);
    } else {
        D_80116DA4=ambient_sound_set(152-w/2,6,w/2+168,object_bytes_sum_global()+14,176,0,0,0);
    }
}
