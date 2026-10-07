/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B stunt/score HUD callback, reconstructed native record views. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct ScoreState {
    s16 active, lock;
    f32 started;
    s32 unknown08, score, shown_score, total_score, working;
    u8 unknown1C[24];
    s16 counts[10];
    u8 unknown48[29];
    s8 message_kind;
    u8 unknown66;
    s8 message_index;
    u8 unknown68[4];
    s32 multiplier, pending, state;
} ScoreState;
typedef struct Effect {
    s16 state, handle;
    u8 unknown04[48];
    f32 alpha;
    s16 x, y;
    u8 unknown3C[12];
} Effect;
typedef struct HudTexts { u8 before716[716]; char *kind1, *kind2, *multiplier; } HudTexts;
typedef struct Selection { u8 before76[76]; u16 index; } Selection;
typedef struct Language {
    s32 unknown0;
    HudTexts *texts;
    s32 unknown8;
    Selection *selection;
    char **messages;
} Language;
extern u32 state_word_a;
extern s16 D_80151AD0, D_8014A108;
extern f32 D_801543CC;
extern ScoreState D_80152038[4];
extern Effect D_803950C0[4][10];
extern s32 D_80395ED8[4];
extern s32 D_80393FD0[4][4][2], D_80393F50[4][4][2], D_80115F28[4][4][2];
extern char D_80394A88[], D_80394A90[], D_80394A94[], D_80394A9C[], D_80394AA4[];
extern Language countdown_state;
extern void render_helper(f32);
extern s32 object_create(s32);
extern void func_800B669C(u32, u32);
extern void func_800ED66C(f32);
extern void fcvt_wrapper(char *, char *, ...);
extern void dispatch_handler(s32);
extern s16 sound_pitch_diff_halved(void *, s16);
extern void state_utility(s16, s16, void *);
extern s16 object_bytes_sum_global(void);

s32 func_80392894(void *callback_context)
{
    s32 i, j;
    s32 x, y;
    ScoreState *score;
    Effect *effect;
    char text[32];
    if (state_word_a & 0x7C03FFFE) {
        return 1;
    }
    render_helper(0.0f);
    func_800B669C(0, 1);
    if (D_80151AD0 == 1) {
        object_create(10);
    } else {
        object_create(12);
    }
    for (i = 0; i < D_8014A108; i++) {
        score = &D_80152038[i];
        dispatch_handler(1);
        if (score->active && ((D_801543CC - score->started < 2.69f) || score->working)) {
            for (j = 0; j < 10; j++) {
                effect = &D_803950C0[i][j];
                if (effect->state != 11 && score->counts[effect->state] && effect->alpha) {
                    func_800ED66C((u8)effect->alpha);
                    x = (D_80151AD0 == 1 ? 12 : 6) + effect->x;
                    y = effect->y;
                    fcvt_wrapper(text, D_80394A88, score->counts[effect->state]);
                    state_utility(x, y, text);
                }
            }
            x = (D_80151AD0 == 1 ? 12 : 6) + D_80393FD0[D_80151AD0 - 1][i][0];
            y = D_80393FD0[D_80151AD0 - 1][i][1];
            fcvt_wrapper(text, D_80394A90, score->shown_score);
            state_utility(sound_pitch_diff_halved(text, x), y, text);
        } else if (!score->working) {
            score->active = 0;
            score->lock = 0;
            score->multiplier = 1;
            score->message_kind = 0;
            score->message_index = 0;
            score->shown_score = 0;
            for (j = 0; j < 10; j++) {
                score->counts[j] = 0;
            }
        }
        func_800ED66C(-1.0f);
        if (score->pending > 0) {
            if (D_80395ED8[i] == -1) {
                score->pending--;
                D_80395ED8[i] = -2;
            } else if (D_80395ED8[i] == -2) {
                D_80395ED8[i] = 10;
            }
            if (D_80395ED8[i] >= 0) {
                x = D_80115F28[D_80151AD0 - 1][i][0];
                y = D_80115F28[D_80151AD0 - 1][i][1];
                fcvt_wrapper(text, countdown_state.texts->multiplier);
                fcvt_wrapper(text, D_80394A94, text, score->multiplier);
                state_utility(sound_pitch_diff_halved(text, x), y, text);
                D_80395ED8[i]--;
            }
        } else {
            D_80395ED8[i] = -2;
        }
        if (!score->state) {
            score->shown_score = score->score;
        } else if (!score->lock) {
            x = D_80115F28[D_80151AD0 - 1][i][0];
            y = D_80115F28[D_80151AD0 - 1][i][1];
            if (score->multiplier > 1) {
                fcvt_wrapper(text, countdown_state.texts->multiplier);
                fcvt_wrapper(text, D_80394A9C, text, score->multiplier);
                state_utility(sound_pitch_diff_halved(text, x), y, text);
                y += object_bytes_sum_global();
            }
            if (score->message_index) {
                state_utility(sound_pitch_diff_halved(
                    countdown_state.messages[countdown_state.selection->index + score->message_index - 3], x),
                    y, countdown_state.messages[countdown_state.selection->index + score->message_index - 3]);
                y += object_bytes_sum_global();
            }
            if (score->message_kind == 1) {
                state_utility(sound_pitch_diff_halved(countdown_state.texts->kind1, x), y, countdown_state.texts->kind1);
            } else if (score->message_kind == 2) {
                state_utility(sound_pitch_diff_halved(countdown_state.texts->kind2, x), y, countdown_state.texts->kind2);
            }
        }
        dispatch_handler(1);
        x = D_80393F50[D_80151AD0 - 1][i][0];
        y = D_80393F50[D_80151AD0 - 1][i][1];
        fcvt_wrapper(text, D_80394AA4, score->total_score);
        state_utility(sound_pitch_diff_halved(text, x), y, text);
    }
    func_800B669C(0, 3);
    render_helper(-1.0f);
    return 1;
}
