/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Native three-choice controller/menu text helper at 800DC99C. */
typedef signed char s8;
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
s16 object_bytes_sum_global(void);
void state_utility(s16,s16,void *);
/* Text words retain the existing real caller's declared eight-formal interface. */
void attract_demo_handler(s8 first,s32 menu,s32 item,s8 second,
                          s32 heading,s32 line0,s32 line1,s32 line2)
{
    MenuEntry44 *entry;
    s32 inner_height;
    s32 height,x,y;
    D_80118E20[0] = 1;
    D_80118E20[1] = 1;
    font_set(11);
    dispatch_handler(1);
    entry = &D_80153FD8[menu][item];
    inner_height = entry->height-40;
    camera_auto_follow((s16)(entry->x+entry->width/2),
                       (s16)(entry->y+inner_height/2+20),
                       entry->width,(s16)inner_height,-1,0,(u8 *)heading);
    height = object_bytes_sum_global();
    x = entry->x+entry->width/2;
    y = entry->y+entry->height-height*3;
    font_set(first ? 10 : 11);
    dispatch_handler(first ? 22 : 1);
    state_utility((s16)x,(s16)(y-(first ? 1 : 0)),(void *)line0);
    font_set(second ? 10 : 11);
    dispatch_handler(second ? 22 : 1);
    state_utility((s16)x,(s16)(y-(second ? 1 : 0)+height),(void *)line1);
    font_set(first || second ? 11 : 10);
    dispatch_handler(first || second ? 1 : 22);
    state_utility((s16)x,(s16)(y-(first || second ? 0 : 1)+height*2),(void *)line2);
    D_80118E20[1] = 3;
    D_80118E20[0] = 0;
}
