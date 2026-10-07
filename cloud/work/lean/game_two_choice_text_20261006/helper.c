/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Native two-choice controller/menu text helper at 800DCDF4. */
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
typedef struct { u8 prefix[0x32]; unsigned short first; } TextBank;
typedef struct { s32 field0; u8 **labels; s32 field8; TextBank *bank; u8 **strings; } TextRoot;
extern TextRoot D_8017A4E0;
s16 object_bytes_sum_global(void);
void state_utility(s16,s16,void *);
/* The last two text words are passed by all native callers but unused here. */
void attract_video_handler(s8 selected,s32 menu,s32 item,void *heading,
                           s32 unused_option0,s32 unused_option1)
{
    MenuEntry44 *entry;
    s32 inner_height;
    s32 height,x,y;
    dispatch_handler(1);
    font_set(11);
    entry = &D_80153FD8[menu][item];
    inner_height = entry->height-40;
    D_80118E20[0] = 1;
    D_80118E20[1] = 1;
    camera_auto_follow((s16)(entry->x+entry->width/2),
                       (s16)(entry->y+inner_height/2+20),
                       entry->width,(s16)inner_height,-1,0,heading);
    height = object_bytes_sum_global();
    x = entry->x+entry->width/2;
    y = entry->y+entry->height-height*3;
    font_set(selected ? 11 : 10);
    dispatch_handler(selected ? 1 : 22);
    state_utility((s16)x,(s16)(y-(selected ? 0 : 1)+height),
                  D_8017A4E0.labels[236]);
    font_set(selected ? 10 : 11);
    dispatch_handler(selected ? 22 : 1);
    state_utility((s16)x,(s16)(y-(selected ? 1 : 0)),
                  D_8017A4E0.strings[D_8017A4E0.bank->first]);
    D_80118E20[1] = 3;
    D_80118E20[0] = 0;
}
