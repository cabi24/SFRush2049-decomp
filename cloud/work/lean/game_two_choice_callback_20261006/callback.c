/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete two-choice menu text callback. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Blit {
    s32 texture;
    u32 field04, field08;
    unsigned short field0C;
    s16 field0E, field10;
    unsigned short field12;
    s16 field14, field16;
    u8 field18, field19;
    s8 hidden;
    u8 field1B;
    s16 field1C, field1E, field20, field22;
    s32 field24;
    s32 callback_state;
} Blit;
typedef struct { s16 x, y; } Point16;
typedef struct { u8 prefix[0x30]; unsigned short first; } TextBank;
typedef struct { u8 prefix[12]; TextBank *bank; u8 **strings; } MenuText;
typedef struct OSMesgQueue OSMesgQueue;
extern Point16 D_80117280;
extern MenuText D_8017A4E0;
extern s32 D_801170FC;
extern s8 D_80117354;
extern u8 **countdown_object;
extern OSMesgQueue D_801461D0;
void Input_ApplyPadConfig(Blit *);
void render_helper(float);
s32 osRecvMesg(OSMesgQueue *,void **,s32);
s32 osJamMesg(OSMesgQueue *,void *,s32);
s32 slot_state_setup(s32);
s16 object_bytes_sum_global(void);
s32 object_manager_update(u8 *,s16);
void dispatch_handler(s32);
void state_utility(s16,s16,void *);
static void gfx_lock(void) { osRecvMesg(&D_801461D0,0,1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0,0,0); }
static s32 font_set(s32 font) {
    s32 old;
    gfx_lock(); old=slot_state_setup(font); gfx_unlock();
    return old;
}
s32 func_8010B9C8(Blit *blit)
{
    s32 x,y;
    s32 width;
    u8 *text;
    if ((D_801170FC < 7) != blit->hidden) {
        blit->hidden = D_801170FC < 7;
        Input_ApplyPadConfig(blit);
    }
    if (blit->hidden == 0) {
        render_helper(0.0f);
        font_set(13);
        x = D_80117280.x;
        y = object_bytes_sum_global()*2+D_80117280.y;
        font_set(10);
        dispatch_handler(1);
        text = countdown_object[216];
        state_utility((s16)((s16)x-((u32)object_manager_update(text,-1)>>1)),
                      (s16)y,text);
        y += object_bytes_sum_global();
        width = object_manager_update(countdown_object[216],-1);
        x = D_80117280.x-width/4;
        text = D_8017A4E0.strings[D_8017A4E0.bank->first];
        dispatch_handler(D_80117354 ? 22 : 1);
        state_utility((s16)((s16)x-((u32)object_manager_update(text,-1)>>1)),
                      (s16)y,text);
        x = D_80117280.x+width/4;
        text = D_8017A4E0.strings[D_8017A4E0.bank->first+1];
        dispatch_handler(D_80117354 ? 1 : 22);
        state_utility((s16)((s16)x-((u32)object_manager_update(text,-1)>>1)),
                      (s16)y,text);
        render_helper(-1.0f);
    }
    return 1;
}
