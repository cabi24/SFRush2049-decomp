/*
 * Per-player control input: steering (0x720), throttle/brake (0x728/0x72C)
 * and gear / button bytes (0x730-0x732) in each car's state record.
 * Hand-written from the assembly (cloud Lane A).
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
    /* 0x18 */ s32 throttleSrc;
    /* 0x1C */ s32 brakeSrc;
    /* 0x20 */ s32 steerSrc;
    /* 0x24 */ u32 gearUp;
    /* 0x28 */ u32 gearDown;
    /* 0x2C */ u32 reverse;
    /* 0x30 */ u32 button30;
    /* 0x34 */ u8 pad34[0x3C - 0x34];
    /* 0x3C */ u32 button3C;
    /* 0x40 */ u8 pad40[0x4C - 0x40];
} InputRecord;                          /* 0x4C */

typedef struct {
    /* 0x000 */ u8 pad0[0x640];
    /* 0x640 */ s8 autoGear;
    /* 0x641 */ u8 pad641[0x720 - 0x641];
    /* 0x720 */ f32 steer;
    /* 0x724 */ u8 pad724[4];
    /* 0x728 */ f32 brake;
    /* 0x72C */ f32 throttle;
    /* 0x730 */ s8 gear;
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
extern u8 D_80151AD8;
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
    f32 r;
    f32 d;
    f32 prev;

    if (D_8013FECB != 0) {
        st->steer = 0.0f;
        return;
    }
    x = D_80140620[in->pad][0];
    prev = st->steer;
    if (in->steerSrc == 0x19) {
        if (x < D_80124498) {
            x += D_8012449C;
        } else if (D_801244A0 < x) {
            x -= D_801244A0;
        } else {
            x = 0.0f;
        }
    } else {
        x = (x >= 0.0f ? x : -x) * (x >= 0.0f ? x : -x) * x;
    }
    q = x * 127.0f;
    if (q < 0.0f) {
        q -= 0.5f;
    } else {
        q += 0.5f;
    }
    r = (f32) (s32) q / 127.0f;
    d = r - prev;
    if (D_801244A4 < d || d < D_801244A8) {
        prev = r;
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
    if (in->throttleSrc == 0x13 || in->throttleSrc == 0x19) {
        st->throttle = D_80140620[in->pad][1];
        if (st->throttle < 0.0f) {
            st->throttle = 0.0f;
        }
    } else if (in->throttleSrc == 0x15) {
        st->throttle = (u32) D_80156CF0[in->pad].throttle / 255.0f;
    } else if (in->throttleSrc == 0x16) {
        st->throttle = (u32) D_80156CF0[in->pad].brake / 255.0f;
    } else if (in->throttleSrc & D_8013FED0[in->pad]) {
        st->throttle = 1.0f;
    } else {
        st->throttle = 0.0f;
    }
    if (in->reverse & D_8013FED0[in->pad]) {
        st->throttle = 1.0f;
    }
    if (D_8013F2FC != 0) {
        st->brake = 0.0f;
    } else if (in->brakeSrc == 0x13 || in->brakeSrc == 0x19) {
        st->brake = -D_80140620[in->pad][1];
        if (st->brake < D_801244AC) {
            st->brake = 0.0f;
        }
    } else if (in->brakeSrc == 0x15) {
        st->brake = (u32) D_80156CF0[in->pad].throttle / 255.0f;
    } else if (in->brakeSrc == 0x16) {
        st->brake = (u32) D_80156CF0[in->pad].brake / 255.0f;
    } else if (in->brakeSrc & D_8013FED0[in->pad]) {
        st->brake = 1.0f;
    } else {
        st->brake = 0.0f;
    }
    q = st->throttle * 15.0f;
    if (q < 0.0f) {
        q -= 0.5f;
    } else {
        q += 0.5f;
    }
    st->throttle = (f32) (s32) q / 15.0f;
    q = st->brake * 15.0f;
    if (q < 0.0f) {
        q -= 0.5f;
    } else {
        q += 0.5f;
    }
    st->brake = (f32) (s32) q / 15.0f;
}

void func_800E681C(void)
{
    InputRecord *in;
    CarState *st;
    Car *car;
    s32 i;
    s32 speed;

    in = input_rec0;
    for (i = 0; i < active_player_count; i++, in++) {
        st = &D_8014A250[in->player];
        if (D_801170FC != 0 || (state_word_a & 8)) {
            continue;
        }
        func_800E6460(st, in);
        func_800E627C(st, in);
        if (st->autoGear == 0) {
            car = &player_array[st->car];
            if (car->f359 <= 0 && D_8013FECB == 0) {
                if (in->reverse & D_8013FED0[in->pad]) {
                    st->gear = -1;
                } else if (in->gearDown & D_801403C0[in->pad]) {
                    if (--st->gear <= 0) {
                        st->gear = 1;
                    }
                } else if (in->gearUp & D_801403C0[in->pad]) {
                    st->gear++;
                    if (st->gear >= 5) {
                        st->gear = 4;
                    }
                    if (st->gear == 0) {
                        car = &player_array[st->car];
                        goto pick;
                    }
                } else if (st->gear < 0) {
                pick:
                    speed = car->speed >> 2;
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
            }
        }
        if (D_8014A110 != 2 && D_8013FECB == 0 && (in->button3C & D_8013FED0[in->pad])) {
            st->button3C = 1;
        } else {
            st->button3C = 0;
        }
        if (D_8014A110 != 6 && D_8013FECB == 0 && (in->button30 & D_801403C0[in->pad])) {
            st->button30 = 1;
        } else {
            st->button30 = 0;
        }
    }
}

/* stand-in caller: keeps func_800E627C out of line under -O3 */
void __standin_func_800E627C(void)
{
    func_800E627C(0, 0);
}

/* stand-in caller: keeps func_800E6460 out of line under -O3 */
void __standin_func_800E6460(void)
{
    func_800E6460(0, 0);
}
