/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete corner-anchored help-text callback. */
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
typedef struct { unsigned short first; } TextBank;
typedef struct { u8 prefix[12]; TextBank *bank; u8 **strings; } MenuText;
typedef struct OSMesgQueue OSMesgQueue;
extern MenuText D_8017A4E0;
extern u8 *D_80114A0C[][11];
extern s32 state_word_a, D_80138660;
extern s32 D_801543A0, D_801543A8, D_801543B0, D_80154394;
extern OSMesgQueue D_801461D0;
void Input_ApplyPadConfig(Blit *);
s32 osRecvMesg(OSMesgQueue *,void **,s32);
s32 osJamMesg(OSMesgQueue *,void *,s32);
s32 slot_state_setup(s32);
void dispatch_handler(s32);
void state_utility(s16,s16,void *);
static void gfx_lock(void) { osRecvMesg(&D_801461D0,0,1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0,0,0); }
static s32 font_set(s32 font) {
    s32 old;
    gfx_lock(); old=slot_state_setup(font); gfx_unlock();
    return old;
}
s32 func_8010A53C(Blit *blit)
{
    s32 hidden;
    s32 x,y;
    s32 row;
    hidden = !(state_word_a & 8) || D_80138660 == 0;
    if (hidden != blit->hidden) {
        blit->hidden = hidden;
        Input_ApplyPadConfig(blit);
    }
    if (blit->hidden == 0) {
        switch (D_801543A0) {
        default:
            x = 290-D_801543A8;
            y = 155-D_801543B0*10;
            break;
        case 0:
            x = 30;
            y = 30;
            break;
        case 1:
            x = 290-D_801543A8;
            y = 30;
            break;
        case 2:
            x = 30;
            y = 155-D_801543B0*10;
            break;
        }
        font_set(11);
        dispatch_handler(14);
        state_utility((s16)x,(s16)y,
            D_8017A4E0.strings[D_8017A4E0.bank->first+D_80154394]);
        dispatch_handler(1);
        y += 15;
        for (row=0; row<D_801543B0; row++) {
            state_utility((s16)x,(s16)y,D_80114A0C[D_80154394][row]);
            y += 10;
        }
    }
    return 1;
}
