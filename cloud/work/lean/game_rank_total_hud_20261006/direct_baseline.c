/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Native position/total HUD callback, historically named race_finish. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct { u8 prefix[0xEE]; s8 position,mode; u8 suffix[0x3B8-0xF0]; } Player952;
typedef struct { s32 x,y; } Point;
typedef struct { u8 r,g,b,a; } Color4;
typedef struct OSMesgQueue OSMesgQueue;
extern Player952 player_array[];
extern Point D_80115CA8[][4];
extern Color4 D_80116168[];
extern s32 state_word_a,gameplay_mode,D_80118E20,D_80118E24;
extern s16 D_801543CA,D_80151AD0;
extern s8 D_8016137C;
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *,void **,s32);
s32 osJamMesg(OSMesgQueue *,void *,s32);
s32 slot_state_setup(s32);
s32 camera_shake_update(unsigned short);
s32 func_800CF604(s16);
void render_helper(float);
void dispatch_handler(s32);
void state_utility(s16,s16,void *);
void func_800B7360(u8,u8,u8,u8);
static void gfx_lock(void) { osRecvMesg(&D_801461D0,0,1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0,0,0); }
s32 race_finish(u32 callback_argument)
{
    s32 player;
    s32 glyph_width;
    s32 x,y;
    u8 glyphs[3];
    u8 text[2];
    u8 *cursor;
    if ((state_word_a & 8) || D_801543CA < 2 || gameplay_mode == 1 || !D_8016137C)
        return 1;
    render_helper(0.0f);
    D_80118E20 = 1;
    D_80118E24 = 3;
    if (D_80151AD0 == 1) { gfx_lock(); slot_state_setup(8); gfx_unlock(); }
    else { gfx_lock(); slot_state_setup(5); gfx_unlock(); }
    glyph_width = camera_shake_update(56);
    glyphs[1] = '/';
    glyphs[2] = D_801543CA+'0';
    for (player=0; player<D_80151AD0; player++) {
        if (func_800CF604((s16)player) && player_array[player].mode != 1) {
            y = D_80115CA8[D_80151AD0-1][player].y;
            x = D_80115CA8[D_80151AD0-1][player].x-glyph_width;
            glyphs[0] = player_array[player].position+'1';
            for (cursor=glyphs; cursor<glyphs+3; cursor++) {
                text[0] = *cursor;
                text[1] = 0;
                if (cursor<glyphs+2) {
                    if (D_80151AD0 == 1) { gfx_lock(); slot_state_setup(8); gfx_unlock(); }
                    else { gfx_lock(); slot_state_setup(5); gfx_unlock(); }
                } else {
                    if (D_80151AD0 == 1) { gfx_lock(); slot_state_setup(5); gfx_unlock(); }
                    else { gfx_lock(); slot_state_setup(6); gfx_unlock(); }
                }
                dispatch_handler(0);
                state_utility((s16)(x+1),(s16)(y+1),text);
                if (cursor == glyphs) {
                    func_800B7360(D_80116168[player].r,D_80116168[player].g,
                                 D_80116168[player].b,255);
                } else dispatch_handler(1);
                state_utility((s16)x,(s16)y,text);
                x += glyph_width;
            }
        }
    }
    D_80118E24 = 3;
    D_80118E20 = 0;
    render_helper(-1.0f);
    return 1;
}
