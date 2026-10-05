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

typedef struct DamageModel {
    u8 before_force[16];
    f32 force[3];
    u8 before_spin[52];
    f32 spin;
    u8 before_contact_force[16];
    f32 contact_force[4][3];
    u8 before_body_force[52];
    f32 body_force[4][3];
    u8 before_force_average[516];
    f32 force_average;
    u8 before_speed[240];
    f32 speed;
    u8 before_compression[488];
    f32 compression[4];
    f32 air_distance[4];
    u8 before_crash_threshold[64];
    f32 crash_threshold;
    s8 crash_flag;
    s8 top_scrape;
    s8 thump_flag;
    u8 alignment;
    s16 thump_side;
    u8 before_player[384];
    s16 player;
    u8 before_kind[4];
    s8 kind;
    u8 remaining[59];
} DamageModel;

extern CarView player_array[];
extern s32 gameplay_mode;
extern s8 D_80142B00;
extern f32 D_80124394, D_80124398, D_8012439C, D_801243A0;
extern f32 D_801243A4, D_801243A8, D_801243AC, D_801243B0;
extern f32 func_8008B3C8(f32 *);
extern void func_803914B4(s8, s8, s32);

void func_800E0B20(DamageModel *model)
{
    s16 level;
    s16 mask;
    s16 i;
    f32 magnitude;
    f32 sum;
    f32 threshold;
    f32 sample;
    s32 count;
    f32 pairs;

    level = 0;
    mask = 0;
    if (player_array[model->player].state != 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        if (model->compression[i] > D_8012439C && level < 3) {
            level = 3;
            mask |= 1 << i;
        } else if (model->compression[i] > D_80124398 && level < 2) {
            level = 2;
            mask |= 1 << i;
        } else if (model->compression[i] > D_80124394 && level < 1) {
            level = 1;
            mask |= 1 << i;
        }
    }
    model->thump_side = mask;
    model->thump_flag = level;
    magnitude = func_8008B3C8(model->force);
    if (magnitude > model->crash_threshold) {
        threshold = model->crash_threshold * D_801243A0;
        count = 0;
        sum = 0.0f;
        sample = func_8008B3C8(model->contact_force[0]);
        if (sample > threshold) {
            sum += sample;
            count++;
        }
        sample = func_8008B3C8(model->contact_force[1]);
        if (sample > threshold) {
            sum += sample;
            count++;
        }
        sample = func_8008B3C8(model->contact_force[2]);
        if (sample > threshold) {
            sum += sample;
            count++;
        }
        sample = func_8008B3C8(model->contact_force[3]);
        if (sample > threshold) {
            sum += sample;
            count++;
        }
        if (count >= 4) {
            magnitude -= sum * D_801243A4;
        } else if (count >= 3) {
            magnitude -= sum * D_801243A8;
        }
    }
    if ((gameplay_mode != 6 && magnitude > model->crash_threshold) ||
        (gameplay_mode != 6 && (model->speed > 400.0f || model->spin > 10.0f || model->spin < -10.0f))) {
        if (!model->crash_flag && (model->kind != 2 || !D_80142B00)) {
            model->crash_flag = 1;
        }
    }
    model->top_scrape = 0;
    pairs = 0.0f;
    if (model->body_force[2][0] < -700.0f || model->body_force[3][0] < -700.0f || model->body_force[0][0] < -700.0f || model->body_force[1][0] < -700.0f) {
        pairs = 1.0f;
        if (model->air_distance[1] > 3.0f && model->air_distance[1] < 5.0f && model->air_distance[2] > 3.0f && model->air_distance[2] < 5.0f) {
            pairs += 1.0f;
        }
        if (model->air_distance[0] > 3.0f && model->air_distance[0] < 5.0f && model->air_distance[3] > 3.0f && model->air_distance[3] < 5.0f) {
            pairs += 1.0f;
        }
    }
    if (pairs > 0.0f && model->force_average < D_801243AC && model->speed < 5.0f) {
        if (!model->crash_flag) {
            if (gameplay_mode == 6 || gameplay_mode == 4) {
                func_803914B4(model->player, model->player, -1);
            }
            model->crash_flag = 1;
        }
        return;
    }
    if (pairs >= 2.0f && model->speed < 40.0f) {
        if (!model->crash_flag) {
            if (gameplay_mode == 6 || gameplay_mode == 4) {
                func_803914B4(model->player, model->player, -1);
            }
            model->crash_flag = 1;
        }
        return;
    }
    if (pairs >= 2.0f && model->force_average < D_801243B0) {
        if (model->speed < 100.0f && !model->crash_flag) {
            if (gameplay_mode == 6 || gameplay_mode == 4) {
                func_803914B4(model->player, model->player, -1);
            }
            model->crash_flag = 1;
        }
        model->top_scrape = 1;
    }
}
