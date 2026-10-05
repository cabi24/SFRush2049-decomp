/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* also MATCH at -O2 */
/*
 * Elapsed time in seconds since the mark D_80111958: (scheduler frame count
 * at D_8002E8E8+0x27C minus the mark), as unsigned, times the seconds-per-
 * frame constant D_8002AFB8.  N64-only.
 *
 * What mattered: the scheduler's frame counter is `volatile` (it is advanced
 * by the scheduler thread).  A volatile field is not folded into the symbol
 * (`lui/addiu base; lw 636(base)` instead of `lw %lo(sym+636)`).  The same
 * unfolded form appears in display_settings, controller_poll, func_800CD104,
 * control_settings, world_collision_response, world_trigger_activate,
 * emitter_update, game_loop and func_80109A60.
 */
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    /* 0x000 */ u8 pad0[0x27C];
    /* 0x27C */ volatile u32 frameCount;
} Sched;

extern Sched D_8002E8E8;
extern u32 D_80111958;
extern f32 D_8002AFB8;

f32 func_800FD41C(void) {
    return (f32)(u32)(D_8002E8E8.frameCount - D_80111958) * D_8002AFB8;
}
