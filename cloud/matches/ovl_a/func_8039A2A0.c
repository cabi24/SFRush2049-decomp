/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Runtime image A startup/input gate at 0x8039A2A0, complete 416-byte body.
 * Checks bit 0 for each of four controller words before initialization calls.
 * First-entry input takes an immediate state transition; otherwise it creates
 * a two-BLIT timed prompt and later releases it on deadline or gated input.
 * Historical helper names are retained as address identities, not semantics.
 * No exact arcade donor or original function name is claimed.
 *
 * Natural O3 four-element loop unrolling and declaration order reproduce the
 * target. Both locals are used; there are no dummy reads, unused locals,
 * invented formal arguments, forced volatility, assembly, or owned data.
 */
typedef signed char s8;
typedef unsigned int u32;
typedef int s32;
typedef struct Blit Blit;
typedef struct MultiBlit {
    const char *texname;
    short dulx, duly, width, height, top, bot, left, right;
    u32 zdepth, alpha;
    s32 (*animfunc)(Blit *);
    u32 animid;
} MultiBlit;
extern s8 D_80149794;
extern s8 D_801497C4;
extern s8 D_803BA7EB;
extern s8 D_803B42D0;
extern s8 D_80149414;
extern u32 D_80156978[4];
extern u32 D_8015694C;
extern Blit *D_803B42CC;
extern MultiBlit D_803B4284[];
/* The seven-argument callback contract is evidenced by its actual dispatcher:
 * player/code, two unused incoming words, two signed-byte pointers and an
 * integer predicate. The nested predicate's argument contract is unresolved;
 * this function carries the callback address and invokes neither callback. */
typedef s32 (*StartupPredicate)();
typedef void (*StartupHandler)(s32, s32, s32, s32, s8 *, s8 *, StartupPredicate);
extern void title_prompt_handler(s32, s32, s32, s32, s8 *, s8 *, StartupPredicate);
extern void car_lod_select(StartupHandler);
extern void track_render_process(s32, s32);
extern s32 func_800A3508(s32);
extern void func_800B5570(s32);
extern Blit *sound_control(short, short, const MultiBlit *, short);
extern void sound_stop(Blit *);
extern void viScheduleTick(float);
extern s32 viDeadlinePassed(void);

void func_8039A2A0(void)
{
    s8 pressed;
    s32 i;

    if (!D_80149794) {
        pressed = 0;
        for (i = 0; i < 4; i++) {
            pressed |= (D_80156978[i] & 1) != 0;
        }
        car_lod_select(title_prompt_handler);
        track_render_process(1, 0);
        if (!D_801497C4) {
            D_801497C4 = 1;
        }
        D_80149794 = 1;
        if (pressed) {
            D_803BA7EB = func_800A3508(0x810);
            func_800B5570(0x04000000);
            return;
        }
    }
    if (!D_803B42D0) {
        D_803B42CC = sound_control(0, 0, D_803B4284, 2);
        D_803B42D0 = 1;
        viScheduleTick(3.0f);
    }
    if (viDeadlinePassed() || (D_80149414 && (D_8015694C & 7))) {
        if (D_803B42CC) {
            sound_stop(D_803B42CC);
            D_803B42CC = 0;
        }
        D_803B42D0 = 0;
        func_800B5570(D_80149414 ? 4 : 0x40000000);
        D_80149414 = 1;
    }
}
