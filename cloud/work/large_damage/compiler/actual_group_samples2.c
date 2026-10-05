/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;

typedef struct CarView {
    u8 prefix[857];
    s8 state;
    u8 remaining[94];
} CarView;

extern CarView player_array[];
extern s32 gameplay_mode;
extern s8 D_80142B00;
extern f32 D_80124394, D_80124398, D_8012439C, D_801243A0;
extern f32 D_801243A4, D_801243A8, D_801243AC, D_801243B0;
extern f32 func_8008B3C8(f32 *);
extern void func_803914B4(s8, s8, s8);
void effect_cleanup(s8 a,s8 b,s8 c)
{
    if (gameplay_mode == 6 || gameplay_mode == 4) {
        func_803914B4(a,b,c);
    }
}

#define MF(offset) (*(f32 *)(model + (offset)))
#define MB(offset) (*(s8 *)(model + (offset)))
#define MS(offset) (*(s16 *)(model + (offset)))
#define MV(offset) ((f32 *)(model + (offset)))

void func_800E0B20(void *arg0)
{
    u8 *model;
    s32 level;
    s16 mask;
    s16 i;
    f32 magnitude;
    f32 sum;
    f32 threshold;
    f32 sample0;
    f32 sample1;
    s32 count;
    f32 pairs;

    model = arg0;
    level = 0;
    mask = 0;
    if (player_array[MS(1990)].state != 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (MF(1500 + i * 4) > 1.2f && level < 3) {
            mask |= 1 << i;
            level = 3;
        } else if (MF(1500 + i * 4) > 0.9f && level < 2) {
            mask |= 1 << i;
            level = 2;
        } else if (MF(1500 + i * 4) > 0.6f && level < 1) {
            mask |= 1 << i;
            level = 1;
        }
    }
    MS(1604) = mask;
    MB(1602) = level;
    magnitude = func_8008B3C8(MV(16));
    if (magnitude > MF(1596)) {
        threshold = MF(1596) * 0.15f;
        count = 0;
        sum = 0.0f;
        sample0 = func_8008B3C8(MV(100));
        if (sample0 > threshold) {
            sum += sample0;
            count++;
        }
        sample1 = func_8008B3C8(MV(112));
        if (sample1 > threshold) {
            sum += sample1;
            count++;
        }
        sample1 = func_8008B3C8(MV(124));
        if (sample1 > threshold) {
            sum += sample1;
            count++;
        }
        sample1 = func_8008B3C8(MV(136));
        if (sample1 > threshold) {
            sum += sample1;
            count++;
        }
        if (count >= 4) {
            magnitude -= sum * 0.4f;
        } else if (count >= 3) {
            magnitude -= sum * 0.2f;
        }
    }
    if ((gameplay_mode != 6 && magnitude > MF(1596)) ||
        (gameplay_mode != 6 && (MF(1008) > 400.0f || MF(80) > 10.0f || MF(80) < -10.0f))) {
        if (!MB(1600) && (MB(1996) != 2 || !D_80142B00)) {
            MB(1600) = 1;
        }
    }
    MB(1601) = 0;
    pairs = 0.0f;
    if (MF(224) < -700.0f || MF(236) < -700.0f || MF(200) < -700.0f || MF(212) < -700.0f) {
        pairs = 1.0f;
        if (MF(1520) > 3.0f && MF(1520) < 5.0f && MF(1524) > 3.0f && MF(1524) < 5.0f) {
            pairs += 1.0f;
        }
        if (MF(1516) > 3.0f && MF(1516) < 5.0f && MF(1528) > 3.0f && MF(1528) < 5.0f) {
            pairs += 1.0f;
        }
    }
    if (pairs > 0.0f && MF(764) < -0.1f && MF(1008) < 5.0f) {
        if (!MB(1600)) {
            effect_cleanup(MS(1990), MS(1990), -1);
            MB(1600) = 1;
        }
        return;
    }
    if (pairs >= 2.0f && MF(1008) < 40.0f) {
        if (!MB(1600)) {
            effect_cleanup(MS(1990), MS(1990), -1);
            MB(1600) = 1;
        }
        return;
    }
    if (pairs >= 2.0f && MF(764) < 0.707f) {
        if (MF(1008) < 100.0f && !MB(1600)) {
            effect_cleanup(MS(1990), MS(1990), -1);
            MB(1600) = 1;
        }
        MB(1601) = 1;
    }
}
