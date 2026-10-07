/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete menu header callback at 8010B5D0. */
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
typedef struct { s16 x,y; } Point16;
typedef struct { u8 prefix[0x70]; unsigned short first; } TextBank;
typedef struct { s32 field0; u8 **labels; s32 field8; TextBank *bank; u8 **strings; } TextRoot;
typedef struct { u8 prefix[0x48]; u8 **name; } PlayerInput;
typedef struct OSMesgQueue OSMesgQueue;
extern TextRoot D_8017A4E0;
extern Point16 D_80117280;
extern PlayerInput input_rec0[];
extern s32 D_801146AC[];
extern s32 D_8015698C,D_801170FC;
extern s8 D_80117350;
extern u8 D_801210CC[],D_801210E0[];
extern OSMesgQueue D_801461D0;
void Input_ApplyPadConfig(Blit *);
void render_helper(float);
s32 osRecvMesg(OSMesgQueue *,void **,s32);
s32 osJamMesg(OSMesgQueue *,void *,s32);
s32 slot_state_setup(s32);
void dispatch_handler(s32);
void fcvt_wrapper(u8 *,u8 *,...);
s32 object_manager_update(u8 *,s16);
void state_utility(s16,s16,void *);
static void gfx_lock(void) { osRecvMesg(&D_801461D0,0,1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0,0,0); }
static s32 font_set(s32 font) {
    s32 old;
    gfx_lock(); old=slot_state_setup(font); gfx_unlock();
    return old;
}
s32 func_8010B5D0(Blit *blit)
{
    /* Fixed scratch-region hypothesis; original capacity and text bound unproven. */
    u8 buffer[40];
    u8 *text;
    s32 x,y;
    if (blit->hidden != 0) {
        blit->hidden = 0;
        Input_ApplyPadConfig(blit);
    }
    if (blit->hidden == 0) {
        render_helper(0.0f);
        font_set(13);
        text = buffer;
        x = D_80117280.x;
        y = D_80117280.y;
        if (D_80117350 != 0) {
            fcvt_wrapper(buffer,D_801210CC,D_8015698C+1,D_8017A4E0.labels[233]);
            dispatch_handler(1);
        } else if (D_801170FC == 7 || D_801170FC == 8) {
            text = D_8017A4E0.strings[D_8017A4E0.bank->first+D_801170FC];
            dispatch_handler(1);
        } else if (D_801170FC == 6) {
            buffer[0] = 0;
        } else {
            fcvt_wrapper(buffer,D_801210E0,*input_rec0[D_8015698C].name+20,
                         D_8017A4E0.strings[D_8017A4E0.bank->first]);
            dispatch_handler(D_801146AC[D_8015698C]);
        }
        state_utility((s16)((s16)x-((u32)object_manager_update(text,-1)>>1)),
                      (s16)y,text);
        render_helper(-1.0f);
    }
    return 1;
}
