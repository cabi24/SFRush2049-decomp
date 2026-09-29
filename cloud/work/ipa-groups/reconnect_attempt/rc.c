typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct { u8 pad0; u8 owner; u8 pad2[0x46]; u8 **text; } Car;                /* 0x4C bytes */
typedef struct { u8 pad[6]; s8 ready; u8 pad7[0x304 - 7]; } Player;                 /* 0x304 bytes */
typedef struct { u8 pad[0x12]; s16 x; s32 y; } Cell;                                /* 0x18 bytes */
typedef struct { Cell e[4]; } Row;                                                  /* 0x60 bytes */
typedef struct { u8 pad[0x2a8]; u8 *f2a8; u8 *f2ac; u8 pad2[0x388 - 0x2b0]; u8 *f388; } Song;
typedef struct { s32 a; Song *song; } SongRef;

extern s32 D_801170FC;
extern s16 D_8014A108;
extern u8 D_801461D0[];
extern Car D_8014A118[];
extern Player D_80144030[];
extern Row D_80117100[];
extern s32 D_801146AC[];
extern SongRef D_8017A4E0;

extern void Input_ApplyPadConfig(s8 *);
extern void render_helper(f32);
extern void osRecvMesg(void *, void *, s32);
extern void osJamMesg(void *, void *, s32);
extern void dispatch_handler(s32);
extern s32 object_bytes_sum_global(void);
extern s32 object_manager_update(u8 *, s32);
extern void state_utility(s16, s16, u8 *);
extern s8 slot_state_setup(s32);

s32 func_8010BC84(s8 *arg0)
{
    u16 i;
    s32 x, y;
    s32 all;
    u8 *str;
    s32 s;
    s32 flag;
    s32 px, cx;
    s32 n;

    all = 1;
    flag = D_801170FC != 6;
    if (flag != arg0[26]) {
        arg0[26] = flag;
        Input_ApplyPadConfig(arg0);
    }
    if (arg0[26] != 0) {
        return 1;
    }
    render_helper(0.0f);
    n = D_8014A108;
    for (i = 0; i < n; i++) {
        if (D_80144030[D_8014A118[i].owner].ready == 0) {
            all = 0;
        }
    }
    i = 0;
    if (all == 0) {
        osRecvMesg(D_801461D0, 0, 1);
        s = slot_state_setup(10);
        osJamMesg(D_801461D0, 0, 0);
        x = D_80117100[0].e[1].x;
        y = (s16)(D_80117100[0].e[1].y - object_bytes_sum_global() / 2);
        dispatch_handler(22);
        str = D_8017A4E0.song->f388;
        px = x - ((u32)object_manager_update(str, -1) >> 1);
        state_utility(px, y, str);
    }
    osRecvMesg(D_801461D0, 0, 1);
    s = slot_state_setup(11);
    osJamMesg(D_801461D0, 0, 0);
    dispatch_handler(1);
    for (i = 0; i < D_8014A108; i++) {
        x = D_80117100[D_8014A108 - 1].e[i].x;
        y = (s16)(D_80117100[D_8014A108 - 1].e[i].y - object_bytes_sum_global());
        dispatch_handler(D_801146AC[i]);
        str = *D_8014A118[i].text + 20;
        px = x - ((u32)object_manager_update(str, -1) >> 1);
        cx = (s16)x;
        state_utility(px, y, str);
        y = (s16)(y + object_bytes_sum_global());
        if (D_80144030[D_8014A118[i].owner].ready != 0) {
            str = D_8017A4E0.song->f2ac;
        } else {
            str = D_8017A4E0.song->f2a8;
        }
        dispatch_handler(1);
        px = cx - ((u32)object_manager_update(str, -1) >> 1);
        state_utility(px, y, str);
    }
    render_helper(-1.0f);
    return 1;
}
