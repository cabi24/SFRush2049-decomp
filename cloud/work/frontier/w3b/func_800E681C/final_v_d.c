/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (whole-program group; keep = func_800E681C) */
/*
 * Per-player control input into each car's model record (0x808 bytes at 0x8014A250):
 *   func_800E627C  steering (+0x720)                      -- already locked, source unchanged
 *                  except that InputRecord now has `u32 map[13]` (in->steerSrc -> in->map[2])
 *   func_800E6460  throttle (+0x72C) / brake (+0x728), quantised to 1/15
 *   func_800E681C  the per-player loop, gear (+0x730) and two button bytes (+0x731/+0x732)
 * No arcade ancestor (arcade reads pots in mdrive.c).  Replaces the stand-in group
 * src/blob/groups/func_800E681C: with the real caller kept, neither callee needs a stand-in
 * caller to stay out of line.
 *
 * State (tools/cloud/score.py group): func_800E627C MATCH, func_800E6460 MATCH,
 * func_800E681C 16/179 words (three rematerialised invariants after the first call are
 * coloured a2/a3/t0 = &D_8013FED0 / 2 / &D_8014A110; retail has 2 / &D_8014A110 / &D_8013FED0).
 * func_800E6460 is internal (four-wide t6-t9 ring; 94 of 239 words differ compiled alone), and its only
 * caller is not strict yet, so its MATCH is recorded as provisional by the wave rules.
 *
 * What made func_800E6460 match (was 32 words):
 *  - the input record holds an action->source array `u32 map[13]` at +0x18, and every button
 *    test is written `D_8013FED0[in->pad] & in->map[k]`.  With a scalar field on either side cfe
 *    puts the array load first whatever the source order; two subscripted operands keep the
 *    retail order (map load first, `and tX,map,mask`).
 *  - throttle/brake are `volatile F32` (three reloads per rounding, no CSE): unchanged from the
 *    earlier attempt.
 * What moved func_800E681C from 173 to 16 words:
 *  - same `mask & in->map[k]` spelling; no `car` local (player_array[st->car] is written at both
 *    uses and uopt's partial redundancy gives the retail recompute on the gear==0 path);
 *  - `gear` is `volatile s8` like the other mainin bytes (retail never reuses a gear read);
 *    `st->gear += 1;` for the up-shift, `--st->gear <= 0` for the down-shift;
 *  - the last test is `if (...) { st->button30 = 1; continue; } else { st->button30 = 0; }`
 *    (retail reloads active_player_count separately on the two arms).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    /* 0x00 */ u8 player;
    /* 0x01 */ u8 pad;
    /* 0x02 */ u8 pad2[0x18 - 2];
    /* 0x18 */ u32 map[13];
} InputRecord;                          /* 0x4C */

typedef struct {
    /* 0x000 */ u8 pad0[0x640];
    /* 0x640 */ s8 autoGear;
    /* 0x641 */ u8 pad641[0x720 - 0x641];
    /* 0x720 */ f32 steer;
    /* 0x724 */ u8 pad724[4];
    /* 0x728 */ volatile f32 brake;
    /* 0x72C */ volatile f32 throttle;
    /* 0x730 */ volatile s8 gear;
    /* 0x731 */ s8 button3C;
    /* 0x732 */ s8 button30;
    /* 0x733 */ u8 pad733[0x7C6 - 0x733];
    /* 0x7C6 */ s16 car;
    /* 0x7C8 */ u8 pad7C8[0x808 - 0x7C8];
} CarState;                             /* 0x808 */

typedef struct {
    /* 0x000 */ u8 pad0[0xF8];
    /* 0x0F8 */ s16 speed;
    /* 0x0FA */ u8 padFA[0x359 - 0xFA];
    /* 0x359 */ s8 f359;
    /* 0x35A */ u8 pad35A[0x3B8 - 0x35A];
} Car;                                  /* 0x3B8 */

typedef struct {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ u8 throttle;
    /* 0xD */ u8 brake;
    /* 0xE */ u8 padE[2];
} PadState;                             /* 0x10 */

extern InputRecord input_rec0[];
extern CarState D_8014A250[];
extern Car player_array[8];
extern s16 active_player_count;
extern s32 state_word_a;
extern s32 D_801170FC;
extern s8 D_8013FECB;                   /* controls locked */
extern u32 D_8013FED0[];
extern u32 D_801403C0[];
extern s32 D_8014A110;
extern f32 D_80140620[][2];             /* analog stick per pad */
extern f32 D_80124498, D_8012449C, D_801244A0, D_801244A4, D_801244A8, D_801244AC;
extern s8 D_80151AD8;
extern s8 D_80140A04;
extern s8 D_8013F1D9;
extern s8 D_8013F2FC;
extern PadState D_80156CF0[];

void func_800E627C(CarState *st, InputRecord *in);
void func_800E6460(CarState *st, InputRecord *in);
void func_800E681C(void);

/* steering */
void func_800E627C(CarState *st, InputRecord *in)
{
    f32 x;
    f32 q;
    f32 d;
    f32 prev;

    if (D_8013FECB != 0) {
        st->steer = 0.0f;
        return;
    }
    x = D_80140620[in->pad][0];
    prev = st->steer;
    if (in->map[2] == 0x19) {
        if (x < D_80124498) {
            f32 c = D_8012449C;
            x += c;
        } else if (D_801244A0 < x) {
            x -= D_801244A0;
        } else {
            x = 0.0;
        }
    } else {
        x = (x >= 0.0f ? x : -x) * (x >= 0.0f ? x : -x) * x;
    }
    q = x * 127.0f;
    x = (f32) (s32) (q < 0.0f ? q - 0.5f : q + 0.5f) / 127;
    d = x - prev;
    if (D_801244A4 < d) {
        prev = x;
    } else if (d < D_801244A8) {
        prev = x;
    }
    if ((D_80151AD8 != 0 || D_80140A04 != 0) &&
        (D_80151AD8 == 0 || D_80140A04 == 0 || D_8013F1D9 != 0)) {
        prev = -prev;
    }
    st->steer = prev;
}

/* throttle and brake, quantized to 1/15 */
void func_800E6460(CarState *st, InputRecord *in)
{
    f32 q;

    if (D_8013FECB != 0) {
        st->throttle = 0.0f;
        st->brake = 0.0f;
        return;
    }
    if (in->map[0] == 0x13 || in->map[0] == 0x19) {
        st->throttle = D_80140620[in->pad][1];
        if (st->throttle < 0.0f) {
            st->throttle = 0.0f;
        }
    } else if (in->map[0] == 0x15) {
        st->throttle = (u32) D_80156CF0[in->pad].throttle / 255.0f;
    } else if (in->map[0] == 0x16) {
        st->throttle = (u32) D_80156CF0[in->pad].brake / 255.0f;
    } else if (D_8013FED0[in->pad] & in->map[0]) {
        st->throttle = 1.0f;
    } else {
        st->throttle = 0.0f;
    }
    if (D_8013FED0[in->pad] & in->map[5]) {
        st->throttle = 1.0f;
    }
    if (D_8013F2FC != 0) {
        st->brake = 0.0f;
    } else if (in->map[1] == 0x13 || in->map[1] == 0x19) {
        st->brake = -D_80140620[in->pad][1];
        if (st->brake < D_801244AC) {
            st->brake = 0.0f;
        }
    } else if (in->map[1] == 0x15) {
        st->brake = (u32) D_80156CF0[in->pad].throttle / 255.0f;
    } else if (in->map[1] == 0x16) {
        st->brake = (u32) D_80156CF0[in->pad].brake / 255.0f;
    } else if (D_8013FED0[in->pad] & in->map[1]) {
        st->brake = 1.0f;
    } else {
        st->brake = 0.0f;
    }
    if (st->throttle * 15.0f < 0.0f) {
        q = st->throttle * 15.0f - 0.5f;
    } else {
        q = st->throttle * 15.0f + 0.5f;
    }
    st->throttle = (f32) (s32) q / 15;
    if (st->brake * 15.0f < 0.0f) {
        q = st->brake * 15.0f - 0.5f;
    } else {
        q = st->brake * 15.0f + 0.5f;
    }
    st->brake = (f32) (s32) q / 15;
}

void func_800E6AE8(CarState *st, InputRecord *in);
void func_800E6AF0(CarState *st, InputRecord *in);
void func_800E681C(void)
{
    InputRecord *in;
    CarState *st;
    s32 i;

    in = input_rec0;
    for (i = 0; i < active_player_count; i++, in++) {
        st = &D_8014A250[in->player];
        if (D_801170FC != 0 || (state_word_a & 8)) {
            continue;
        }
        func_800E6460(st, in);
        func_800E627C(st, in);
        func_800E6AE8(st, in);
        if (D_8014A110 != 2 && D_8013FECB == 0 && (D_8013FED0[in->pad] & in->map[9])) {
            st->button3C = 1;
        } else {
            st->button3C = 0;
        }
        if (D_8014A110 != 6 && D_8013FECB == 0 && (D_801403C0[in->pad] & in->map[6])) {
            st->button30 = 1;
            continue;
        } else {
            st->button30 = 0;
        }
    }
}

void func_800E6AE8(CarState *st, InputRecord *in)
{
    s32 speed;

    if (st->autoGear != 0) {
        return;
    }
    if (player_array[st->car].f359 > 0 || D_8013FECB != 0) {
        return;
    }
    if (D_8013FED0[in->pad] & in->map[5]) {
        st->gear = -1;
        return;
    }
    if (D_801403C0[in->pad] & in->map[4]) {
        if (--st->gear <= 0) {
            st->gear = 1;
        }
        return;
    }
    if (D_801403C0[in->pad] & in->map[3]) {
        st->gear += 1;
        if (st->gear >= 5) {
            st->gear = 4;
        }
        if (st->gear != 0) {
            return;
        }
    } else if (st->gear >= 0) {
        return;
    }
    speed = player_array[st->car].speed >> 2;
    if (speed >= 111) {
        st->gear = 4;
    } else if (speed >= 81) {
        st->gear = 3;
    } else if (speed >= 51) {
        st->gear = 2;
    } else {
        st->gear = 1;
    }
}
