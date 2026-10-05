/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research only: N64 model-clock reset. No strict match or production claim.
 * These three adjacent fields are independently used by func_800E6AF8;
 * battle_mode_setup consumes last_time. The complete function is N64-specific.
 * Arcade reckon.c supplies related time/step semantics, not a full donor.
 */
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct ModelClockRecord {
    u8 before_clock[1808];
    s32 ticks;
    f32 last_time;
    f32 step;
    u8 after_clock[236];
} ModelClockRecord;

extern s32 D_80143FF4;
extern s32 D_8014A110;
extern f32 D_8002AFB8;
extern f32 D_801543CC;
extern ModelClockRecord D_8014A250[6];

void func_800E762C(u32 ticks)
{
    s32 negative_ticks;
    f32 time;
    f32 ratio;
    f32 step;
    s32 mode;
    ModelClockRecord *model;

    negative_ticks = (s32)(0u - ticks);
    D_80143FF4 = negative_ticks;
    step = D_8002AFB8;
    time = D_80143FF4 * step;
    D_801543CC = time;
    mode = D_8014A110;
    for (model = D_8014A250; model != D_8014A250 + 6; model++) {
        if (mode != 2 || model == D_8014A250 || model->step == 0.0f) {
            model->ticks = negative_ticks;
            model->last_time = time;
            model->step = step;
        } else {
            ratio = time / model->step;
            if (ratio < 0.0f)
                model->ticks = (s32)(ratio - 0.5f);
            else
                model->ticks = (s32)(ratio + 0.5f);
            model->last_time = time;
        }
    }
}
