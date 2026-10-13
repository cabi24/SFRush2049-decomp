/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * audio_doppler_full (0x800B61FC, 952 B) = ShowCountdown(int active) of countdown.c. The N64 label is
 * wrong: GC rush_d.elf ShowCountdown (countdown.c, 0x8000E518) is the same function (strings
 * "CNTDWN3"/"CNTDWN%d", locals xyz[4][3], dist, i, winidx, str[20], function-local statics
 * poly/polyTidx/lastCountDown/polyColor/polyXYZ). Per window: while active, scale the 4-corner
 * countdown quad by 12 * frac(time remaining - 0.001) (min 5), transform it into camera space
 * (WorldVector = func_8008C544 on &gCamera[w].mat, then + gCamera[w].pos), create the poly on the
 * first frame (MBOX_FindTexture_Sub "CNTDWN3", ZOID_NewPoly), and when gCountDown drops below
 * lastCountDown swap the texture to "CNTDWN%d" (last window only), play sound 75/76/77 for 3/2/other
 * (PlayGameSound, gated by D_8010FFC0), update and show the poly. Inactive: remove the poly and
 * reset gCountDown to -1.
 * Shaping (all natural, from the GC source): the five statics are function-local (as on GC; as
 * externs the polyTidx address web is coloured and takes polyColor's s8); PlayGameSound and
 * ZOID_ShowPoly are inlined static helpers (frame 256 needs both; the sound gate is an early
 * `return 0;`); gNumModels D_80140BDC is volatile (address form, as in locked func_800B24EC); the
 * poly flags are written as the OR of four constants 0x8000|0x200|0x80|0x40 (a folded constant
 * `0x82C0` leaves the ugen temp ring 3 registers behind retail).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Poly {
    s16 active;
    u16 flags;
} Poly;

typedef struct Camera {
    u8 pad0[0x24];
    f32 pos[3];
    u8 mat[0x98 - 0x30];
} Camera;

extern s16 gNumWindows;          /* gNumWindows */
extern s16 gCountDown;          /* gCountDown */
extern Camera gCamera[];     /* gCamera */
extern volatile u8 gNumModels;           /* gNumModels */
extern s8 D_8010FFC0;

f32 viGetTimeToDeadline(void);
void func_8008C544(void *, void *, void *);
void *func_800B24EC(char *name, u16 *out, s8 lo, s8 hi, s32 err);
Poly *func_800A78BC(s32, f32 *, u16, u8 *, u16, s32);
void func_8008C074(Poly *, s32, f32 *, u16, u8 *, u16, s32);
void func_8008D0C0(Poly *);
s32 entity_flags_apply(s32, s32, s32, u8);
s32 sprintf(char *, const char *, ...);

static s32 PlayGameSound(s32 fxid, s32 subId, s32 replace, u8 priority)
{
    if (!D_8010FFC0) {
        return 0;
    }
    return entity_flags_apply(fxid, subId, replace, priority);
}

static void ZOID_ShowPoly(Poly *poly)
{
    poly->flags &= 0x7FFF;
}

void ShowCountdown(s32 active)
{
    f32 xyz[4][3];
    f32 dist;
    s32 i;
    s32 winidx;
    char str[20];
    static Poly *D_80154368[12];
    static u16 D_80154398;
    static u16 D_801543A4;
    static u8 D_801140F4[4] = { 0xFF, 0xFF, 0xFF, 0xC0 };
    static f32 D_801140F8[4][3] = {
        { 1.0f, 1.0f, 6.0f }, { -1.0f, 1.0f, 6.0f }, { -1.0f, -1.0f, 6.0f }, { 1.0f, -1.0f, 6.0f }
    };

    for (winidx = 0; winidx < gNumWindows; winidx++) {
        if (active) {
            dist = viGetTimeToDeadline() - 0.001f;
            dist = (dist - (s32)dist) * 12.0f;
            if (dist < 5.0f) {
                dist = 5.0f;
            }
            for (i = 0; i < 4; i++) {
                D_801140F8[i][2] = dist;
                func_8008C544(D_801140F8[i], xyz[i], gCamera[winidx].mat);
                xyz[i][0] += gCamera[winidx].pos[0];
                xyz[i][1] += gCamera[winidx].pos[1];
                xyz[i][2] += gCamera[winidx].pos[2];
            }
            if (D_80154368[winidx] == 0) {
                func_800B24EC("CNTDWN3", &D_80154398, 0, gNumModels - 1, 1);
                D_80154368[winidx] = func_800A78BC(4, xyz[0], D_80154398, D_801140F4, (1 << winidx) | 0x8000 | 0x200 | 0x80 | 0x40, 1);
                D_801543A4 = 4;
            } else if (D_801543A4 > gCountDown) {
                if (winidx == gNumWindows - 1) {
                    sprintf(str, "CNTDWN%d", gCountDown);
                    func_800B24EC(str, &D_80154398, 0, gNumModels - 1, 1);
                    D_801543A4 = gCountDown;
                    if (gCountDown == 3) {
                        PlayGameSound(75, 0, 1, 0);
                    } else if (gCountDown == 2) {
                        PlayGameSound(76, 0, 1, 0);
                    } else {
                        PlayGameSound(77, 0, 1, 0);
                    }
                }
                func_8008C074(D_80154368[winidx], 4, xyz[0], D_80154398, D_801140F4, 0, 1);
                ZOID_ShowPoly(D_80154368[winidx]);
            } else {
                func_8008C074(D_80154368[winidx], 4, xyz[0], D_80154398, D_801140F4, 0, 1);
            }
        } else if (D_80154368[winidx] != 0) {
            func_8008D0C0(D_80154368[winidx]);
            D_80154368[winidx] = 0;
            gCountDown = -1;
        }
    }
}
