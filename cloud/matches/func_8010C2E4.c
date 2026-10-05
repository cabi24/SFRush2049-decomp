/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010C2E4: per-player trigger callback for a camera/scene object (no direct callers; reached
 * through a function pointer with four arguments, the last two unused and homed).
 * For player *player (record D_8014A250[], 0x808 bytes), when the record's +0x640 byte is 0 and its
 * +0x6C4 halfword is negative, it reads the 24-byte polygon records D_801497F8[] under the four
 * wheel polygon indices (+0x5A0..+0x5A6).  If any wheel polygon has flag 0x20 set and its 5-bit
 * id (bits 11..15) equals the scene id, the scene gets bit (24 + player) set (plus 0x200 when no
 * player bit was set yet or flag 0x4000 is set); otherwise that player's bit is cleared.
 * Always returns 0.  No arcade ancestor identified.
 * Shaping: the empty `if (cam == NULL)` (a compiled-out debug message) is what gives the camera
 * parameter its register priority, so it stays in a0 and ctl/sc take a1/a2 (found with the traced
 * uopt: forcing ctl=a1, sc=a2 reproduced retail exactly); `idx` as a variable (kept in v0 for the
 * bit shift); `id = sc->id; id <<= 11;` as two statements.  The frontier's group/ring label is a
 * false positive.  Also matches at -O2.
 */
#define NULL ((void *)0)
#define DEBUG_PRINT(args)
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct Poly24 {
    u16 pad0;
    u16 flags;
    u8 pad4[20];
} Poly24;

typedef struct Car {
    u8 pad0[0x5A0];
    u16 wheelPoly[4];
    u8 pad5A8[0x640 - 0x5A8];
    s8 b640;
    u8 pad641[0x6C4 - 0x641];
    s16 s6C4;
    u8 pad6C6[0x808 - 0x6C6];
} Car;

typedef struct CamScene {
    u8 pad00[0x10];
    u32 flags;
    s16 count;
    s16 id;
} CamScene;
typedef struct CamCtl {
    CamScene *scene;
} CamCtl;
typedef struct Camera {
    u8 pad00[0x6C];
    CamCtl *ctl;
} Camera;

extern Car D_8014A250[];
extern Poly24 *D_801497F8;

s32 func_8010C2E4(Camera *cam, s16 *player, s32 arg2, s32 arg3) {
    s32 idx;
    Car *car;
    CamCtl *ctl;
    CamScene *sc;
    s32 id;
    u16 f0, f1, f2, f3;

    idx = *player;
    car = &D_8014A250[idx];
    if (car->b640 != 0 || car->s6C4 >= 0) {
        return 0;
    }
    if (cam == NULL) {
        DEBUG_PRINT(("func_8010C2E4: no camera\n"));
    }
    ctl = cam->ctl;
    sc = ctl->scene;
    id = sc->id;
    id <<= 11;
    f0 = D_801497F8[car->wheelPoly[0]].flags;
    f1 = D_801497F8[car->wheelPoly[1]].flags;
    f2 = D_801497F8[car->wheelPoly[2]].flags;
    f3 = D_801497F8[car->wheelPoly[3]].flags;
    if (((f0 & 0x20) && (f0 & 0xF800) == id) || ((f1 & 0x20) && (f1 & 0xF800) == id) ||
        ((f2 & 0x20) && (f2 & 0xF800) == id) || ((f3 & 0x20) && (f3 & 0xF800) == id)) {
        if ((sc->flags >> 24) == 0 || (sc->flags & 0x4000)) {
            sc->flags |= 0x200;
        }
        sc->flags |= 1 << (idx + 24);
    } else {
        sc->flags &= ~(1 << (idx + 24));
    }
    return 0;
}
