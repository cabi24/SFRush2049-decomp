/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct FiveParts FiveParts;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct { s32 x, width, height; } PanelMetrics;
extern OSMesgQueue D_801461D0;
extern s8 D_8011AD38, D_8011AD6C;
extern s8 D_80156CF0[][16];
extern s32 D_8015698C;
extern u32 state_word_a;
extern PanelMetrics D_8011AD70;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32);
s16 object_bytes_sum_global(void);
void sound_loop_set(void);
FiveParts *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
static void gfx_lock(void) { osRecvMesg(&D_801461D0,0,1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0,0,0); }
static s32 font_set(s32 font) {
    s32 old;
    gfx_lock(); old=slot_state_setup(font); gfx_unlock();
    return old;
}
s32 func_8010E8B4(void *blit)
{
    s16 y;
    if (D_8011AD38 == 0) {
        if (!(state_word_a & 0x7C03FFFE)) {
            font_set(13);
            D_8011AD6C = D_80156CF0[D_8015698C][0] == 0;
            sound_loop_set();
            y = object_bytes_sum_global()*2+10;
            ambient_sound_set(D_8011AD70.x-8,y-4,
                D_8011AD70.x+D_8011AD70.width+8,
                y+D_8011AD70.height+4,176,0,0,0);
        }
        ambient_sound_set(20,136,300,204,176,0,0,0);
        D_8011AD38 = 1;
    }
    return 1;
}
