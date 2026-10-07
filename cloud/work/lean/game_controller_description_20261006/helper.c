/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Native controller/menu description helper at 800DD0C0. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct {
    u8 field00;
    s8 selected;
    u8 prefix[22];
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
extern s32 state_word_a;
u8 *func_800BE6A4(u8 *,u8 *);
u8 *func_800BE4F0(u8 *,u8 *);
void func_800DD0C0(s32 menu,s32 item)
{
    /* Fixed inferred capacity from native scratch/next-local boundaries;
     * not an original recovered declaration or proven text-length bound. */
    u8 buffer[256];
    s32 local_mode;
    s32 margin,inner_height;
    s32 height,x,y;
    MenuEntry44 *entry;
    local_mode = !(state_word_a & 0x7C03FFFE);
    font_set(11);
    dispatch_handler(1);
    func_800BE6A4(buffer,D_8017A4E0.labels[104]);
    func_800BE4F0(buffer,D_8017A4E0.labels[local_mode ? 103 : 102]);
    D_80118E20[0] = 1;
    D_80118E20[1] = 1;
    margin = local_mode ? 40 : 20;
    entry = &D_80153FD8[menu][item];
    inner_height = entry->height-margin;
    camera_auto_follow((s16)(entry->x+entry->width/2),
                       (s16)(entry->y+inner_height/2+20),
                       entry->width,(s16)inner_height,-1,0,buffer);
    if (local_mode) {
        height = object_bytes_sum_global();
        x = entry->x+entry->width/2;
        y = entry->y+entry->height-height*3;
        font_set(entry->selected ? 11 : 10);
        dispatch_handler(entry->selected ? 1 : 22);
        state_utility((s16)x,(s16)(y-(entry->selected ? 0 : 1)+height),
                      D_8017A4E0.labels[236]);
        font_set(entry->selected ? 10 : 11);
        dispatch_handler(entry->selected ? 22 : 1);
        state_utility((s16)x,(s16)(y-(entry->selected ? 1 : 0)),
                      D_8017A4E0.strings[D_8017A4E0.bank->first]);
    }
    D_80118E20[1] = 3;
    D_80118E20[0] = 0;
    font_set(11);
}
