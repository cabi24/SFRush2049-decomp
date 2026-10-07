typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct { u8 pad[0xEF]; s8 state; u8 pad2[0x3B8 - 0xF0]; } Racer;   /* 0x3B8 bytes */
typedef struct { s32 x, y; } Pos;

extern u32 D_801174B4;
extern s32 D_801170FC;
extern s8 D_8015723C;
extern s16 D_80151AD0;
extern s32 D_80118E20[2];
extern u8 D_801461D0[];
extern f32 D_80142740[];
extern f32 D_80142770[];
extern f32 D_8002EB94;
extern Racer D_80152818[];
extern Pos D_80115EA8[][4];

extern void render_helper(f32);
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osJamMesg(void *, void *, s32);
extern void dispatch_handler(s32);
extern void state_utility(s16, s16, u8 *);
extern void func_800ED66C(f32);
extern s32 camera_shake_update(u16);
extern s8 object_byte9_set(s8);
extern s32 slot_state_setup(s32);

static void gfx_lock(void) { osRecvMesg(D_801461D0, 0, 1); }
static void gfx_unlock(void) { osJamMesg(D_801461D0, 0, 0); }
static s32 font_set(s32 font)
{
    s32 old;
    gfx_lock(); old = slot_state_setup(font); gfx_unlock();
    return old;
}

s32 func_801084D4(s32 arg0)
{
    u8 buf[10];
    u8 cbuf[2];
    s32 j;
    s32 i;
    s32 ms;
    s32 w1, w2;
    s32 x, y, px;
    s8 old;
    f32 t;

    if ((D_801174B4 & 8) || D_8015723C == 0) {
        return 1;
    }
    render_helper(0);
    D_80118E20[1] = 3;
    D_80118E20[0] = 1;
    if (D_80151AD0 == 1) {
        font_set(1);
    } else {
        font_set(2);
    }
    w1 = camera_shake_update(56) + 1;
    old = object_byte9_set(0);
    w2 = camera_shake_update(58) + 1;
    object_byte9_set(old);
    for (i = 0; i < D_80151AD0; i++) {
        if (D_801170FC == 0) {
            D_80142740[i] -= D_8002EB94;
        }
        t = D_80142740[i];
        if (t <= 0.0f || D_80152818[i].state == 1) {
            D_80142740[i] = 0.0f;
            continue;
        }
        x = (6 * w1 + 2 * w2) / 2;
        ms = (s32)(D_80142770[i] * 1000.0f);
        ms = ms < 0 ? 0 : ms;
        buf[0] = ms / 600000 + 48;
        buf[1] = ms / 60000 % 10 + 48;
        buf[2] = 58;
        buf[3] = ms / 10000 % 6 + 48;
        buf[4] = ms / 1000 % 10 + 48;
        buf[5] = 46;
        buf[6] = ms / 100 % 10 + 48;
        buf[7] = ms / 10 % 10 + 48;
        buf[8] = ms % 10 + 48;
        cbuf[1] = 0;
        y = D_80115EA8[D_80151AD0 - 1][i].y;
        if (t < 3.0f) {
            func_800ED66C((f32)(s32)(t * 255.0f / 3.0f));
        }
        px = D_80115EA8[D_80151AD0 - 1][i].x - x;
        for (j = 0; j < 9; j++) {
            cbuf[0] = buf[j];
            if (buf[j] != 58 && buf[j] != 46) {
                cbuf[0] = 56;
                dispatch_handler(0);
                state_utility(px, (s16)y, cbuf);
                cbuf[0] = buf[j];
            }
            dispatch_handler(22);
            state_utility(px, (s16)y, cbuf);
            if (j == 1 || j == 2 || j == 4 || j == 5) {
                px += (w1 + w2) / 2;
            } else {
                px += w1;
            }
        }
        func_800ED66C(-1.0f);
    }
    D_80118E20[1] = 3;
    D_80118E20[0] = 0;
    render_helper(-1.0f);
    return 1;
}
