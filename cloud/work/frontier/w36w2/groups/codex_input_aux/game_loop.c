/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * game_loop = GameLoop@game.c (MAT3 GameSource/src/game.c GameLoop, GC 0x8001407C; N64 0x800FD464, 704 B).
 * N64 form: OncePerFrame/getGameStateUpdate become `gGameState == gNewGameState ? game_mode_handler() : (wait for
 * the retrace flag, D_80149438 = 0, gfx_frame_begin, D_801497F4 = msgq_ptr, process_inputs)`; the controller-warning
 * blit (_PAD_ScrollBlit-like, game_state_flags with multiblit D_801146C4) is created when more than 300 (scaled
 * retrace ticks) passed since D_80111958 and gGameState has none of 0x4000000F, else removed; updateGameState =
 * playgame_state_change, updateFrontEnd = input_aux_handler, updatePauseScreen = RaceStateMachine_Update (pause:
 * gPauseState D_801170FC -> UpdateActiveObjects/PhysicsObjectList_Update/Effects_UpdateEmitters/
 * Input_ProcessGameplayPad(0) and return 1), _UpdateGame = countdown, _DrawNoControllers = countdown_handler.
 * gGameState = state_word_a, gNewGameState = state_word_b, gFrames = frame_counter (volatile: retail lui/addiu +
 * 0(reg) at both accesses), sInited = function-local static (0x801146EC, own .data, game.c's TU), GVSyncCount =
 * the volatile scheduler clock D_8002E8E8.tick (house type from func_80109A60.c).
 * Saves s0-s8/f20/f22 and keeps renderFlag at 84(sp) only with countdown + init_state_continue internal in the
 * program (they use s0-s8 unsaved): their sources are context here (byte copies of w36p's frontier_update_game).
 * Levers (w36w2): `gNewGameState == gGameState` operand order at both tests (MAT3 spelling), the elapsed time a
 * named f32 (product in $f0) assigned inside the if condition (a separate statement line flips the as1 order of
 * `lui t6` / `lui at,0x4f80`), static sInited (an extern gives a CSE'd address).  No unused locals, no stand-ins.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { s32 opaque[6]; } OSMesgQueue;

extern volatile s32 frame_counter;      /* gFrames */
extern void *game_state_flags;          /* _PAD controller-warning blit */
extern u8 D_801146C4[];                 /* its multiblit */
extern OSMesgQueue *msgq_ptr;
extern s32 state_word_a;                /* gGameState */
extern s32 state_word_b;                /* gNewGameState */
typedef struct Clock { u8 pad0[636]; u32 tick; } Clock;
extern volatile Clock D_8002E8E8;      /* scheduler clock; .tick = GVSyncCount */
extern s32 D_80111958;
extern f32 D_8002AFB8;
extern volatile s16 D_8002EB70;
extern s32 D_80149438;
extern OSMesgQueue *D_801497F4;
extern s32 D_801170FC;                  /* gPauseState */

void *sound_control(s16, s16, void *, s32);
void sound_stop(void *);
void game_mode_handler(void);
void gfx_frame_begin(void);
void process_inputs(void);
void playgame_state_change(void);
void input_aux_handler(void);
void RaceStateMachine_Update(void);
void countdown(void);
void countdown_handler(void);
void UpdateActiveObjects(void);
s32 PhysicsObjectList_Update(void);
void Effects_UpdateEmitters(void);
void Input_ProcessGameplayPad(s32);

s32 game_loop(void)
{
    s32 renderFlag;
    f32 elapsed;
    static s8 sInited = 0;

    renderFlag = 0;
    if (!sInited) {
        frame_counter = 0;
        sInited = 1;
    }
    if (state_word_b == state_word_a) {
        game_mode_handler();
    } else {
        while (D_8002EB70) {
        }
        D_80149438 = 0;
        gfx_frame_begin();
        D_801497F4 = msgq_ptr;
        process_inputs();
    }
    if ((elapsed = (f32)(u32)(D_8002E8E8.tick - D_80111958) * D_8002AFB8) > 300.0f && !(state_word_a & 0x4000000F)) {
        if (!game_state_flags) {
            game_state_flags = sound_control(0, 0, D_801146C4, 1);
        }
    } else if (game_state_flags) {
        sound_stop(game_state_flags);
        game_state_flags = 0;
    }
    playgame_state_change();
    if (state_word_a & 0x7C03FFFE) {
        input_aux_handler();
    }
    if ((state_word_a & 0x200000) || (state_word_a & 0x400000)) {
        RaceStateMachine_Update();
        if (D_801170FC) {
            UpdateActiveObjects();
            PhysicsObjectList_Update();
            Effects_UpdateEmitters();
            Input_ProcessGameplayPad(0);
            return 1;
        }
    }
    if (state_word_a & 0x3FC0000) {
        countdown();
    }
    if (state_word_b == state_word_a) {
        countdown_handler();
        UpdateActiveObjects();
        PhysicsObjectList_Update();
        Effects_UpdateEmitters();
        Input_ProcessGameplayPad(0);
        renderFlag = 1;
    }
    frame_counter++;
    return renderFlag;
}
