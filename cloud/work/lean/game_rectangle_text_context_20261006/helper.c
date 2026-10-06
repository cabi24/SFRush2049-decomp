/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Native controller/menu rectangle-text helper at 800DC88C. */
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct {
    u8 prefix[24];
    s16 x,y,width,height;
    u8 suffix[12];
} MenuEntry44;
typedef struct OSMesgQueue OSMesgQueue;
extern MenuEntry44 D_80153FD8[][2];
extern s32 D_80118E20[2];
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *,void **,s32);
s32 osJamMesg(OSMesgQueue *,void *,s32);
s32 slot_state_setup(s32);
void dispatch_handler(s32);
void camera_auto_follow(s16,s16,s16,s16,s16,s16,u8 *);
static void gfx_lock(void) { osRecvMesg(&D_801461D0,0,1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0,0,0); }
static s32 font_set(s32 font) {
    s32 old;
    gfx_lock(); old=slot_state_setup(font); gfx_unlock();
    return old;
}
void attract_mode_handler(s32 menu,s32 item,void *text)
{
    MenuEntry44 *entry;
    font_set(11);
    dispatch_handler(1);
    entry = &D_80153FD8[menu][item];
    D_80118E20[0] = 1;
    D_80118E20[1] = 1;
    camera_auto_follow((s16)(entry->x+entry->width/2),
                       (s16)(entry->y+entry->height/2),
                       entry->width,entry->height,-1,0,text);
    D_80118E20[1] = 3;
    D_80118E20[0] = 0;
}
