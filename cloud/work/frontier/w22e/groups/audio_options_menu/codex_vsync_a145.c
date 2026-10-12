/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Group codex_vsync_a145, extended by w11h (wave 11): the command-0x0A writer
 * speed_set (internal; receives blend/amount in f20/f22 and the two byte
 * flags in s1/s2 through IPA, clobbers s0/s3) with all four of its in-unit
 * callers: vsync_wait (already locked), speed_mode0_wrapper,
 * speed_mode1_wrapper and continue_prompt.
 * Source of speed_set/vsync_wait is PR #138's (dot_speed_flags_c9210_20261006).
 * Shaping quirk: the three one-call wrappers must be written on several lines.
 * Written on one line each, as1 sinks the IPA saves of s3/s0 below the
 * argument moves (6/22, 6/22, 10/23 words off); the multi-line layout gives
 * retail's prologue order. continue_prompt's literal is written as 0.05f
 * (own .rodata 0x80124280 = 0x3D4CCCCD, verified by the scorer).
 */
typedef signed char s8;
typedef signed int s32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
#define NULL ((void *)0)
#define M2C_FIELD(p,t,o) (*(t)((char *)(p)+(o)))
extern s32 D_80142728,D_801427A8;
extern void *func_80091B00(void);
extern s32 osRecvMesg(OSMesgQueue *,void *,s32);
extern s32 osJamMesg(OSMesgQueue *,void *,s32);
extern f32 D_801247EC;
extern s8 D_80146115,D_8010FFC0;
void speed_set(f32 blend,f32 amount,s32 first,s32 second) {
    f32 var_f0;
    void *temp_v0;

    osRecvMesg((OSMesgQueue *) &D_80142728, NULL, 1);
    temp_v0 = func_80091B00();
    M2C_FIELD(temp_v0, s8 *, 2) = 0xA;
    if (blend < 0.0f) {
        M2C_FIELD(temp_v0, f32 *, 4) = 0.0f;
    } else {
        if (blend > 1.0f) {
            var_f0 = 1.0f;
        } else {
            var_f0 = blend;
        }
        M2C_FIELD(temp_v0, f32 *, 4) = var_f0;
    }
    if (amount < 0.0f) {
        M2C_FIELD(temp_v0, f32 *, 8) = 0.0f;
    } else {
        M2C_FIELD(temp_v0, f32 *, 8) = (f32) amount;
    }
    M2C_FIELD(temp_v0, s8 *, 0xC) = (s8) first;
    M2C_FIELD(temp_v0, s8 *, 0xD) = (s8) second;
    osJamMesg((OSMesgQueue *) &D_80142728, NULL, 0);
    osJamMesg((OSMesgQueue *) &D_801427A8, temp_v0, 0);
}

void speed_mode0_wrapper(f32 blend, f32 amount)
{
    speed_set(blend, amount, 1, 0);
}

void speed_mode1_wrapper(f32 blend, f32 amount)
{
    speed_set(blend, amount, 0, 1);
}

void continue_prompt(void)
{
    speed_set(0.0f, 0.05f, 1, 0);
}

void vsync_wait(s32 flag) {
    f32 blend;
    D_8010FFC0=flag;
    if(flag) blend=0.0f;
    else blend=(f32)D_80146115/10.0f;
    speed_set(blend,D_801247EC,0,1);
}
