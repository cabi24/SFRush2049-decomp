/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * buffer_swap (0x800924F4, 419 words): Visual callback (v, op) for a per-car part that grows/shrinks in six
 * timed steps (flags 0x100 grow, 0x200 shrink, 0x1000 active, 0x10 shown), with an engine-ish sound handle
 * on the model record (start: entity_flags_apply(61,...) + volume 0.75 + client_sync 0.5; stop:
 * scheduler_recv), basis rotation from the car pose, and the CheckVisible/CheckXlu tail shared with the
 * locked sibling entity_anim_texture. No exact arcade ancestor (AnimateTire is the closest shape).
 * STATE (w23d): unit EQUAL ("locked bodies that differ in this unit: 0") and MATCH as a member of the
 *   superseding group codex_visual_tire_module_20261002 (score.py group, own .rodata verified). It is not a
 *   standalone match: entity_transform_calc's IPA float-register convention and the register-parameter
 *   model_bounds_calc/matrix_scale_apply need the group context.
 * Shaping devices / hypotheses (all disclosed; helper identities unknown):
 *   - ehu_inline: byte-identical copy of the locked entity_hierarchy_update body as a cfe `__inline` function
 *     (retail inlines it twice; umerge does not inline the kept locked one). Renamed so that the locked symbol
 *     is not redefined.
 *   - tire_extend / tire_sound_on / tire_sound_off: `__inline` helpers. They give retail's inline-area layout
 *     (sound handle homes 136/112). Block 1's sound start is written in place; block 2 is
 *     `tire_extend(v, model); if (model->sound == -1) tire_sound_on(model);`.
 *     The `if` sits in the caller, and tire_extend does not call tire_sound_on, because of the group
 *     compile (w23d). umerge drops the Uloc records of a leaf inlinee. A nested inlinee (one that itself
 *     inlined something) keeps them in a small group compile but not in the whole-program unit. The two
 *     closing-brace Ulocs of a nested inlinee that ends at an if-join split that join, and PRE then
 *     hoists v->flags (283 words off in the group).
 *   - pX, pY[3]: unused locals = exact named-frame residual (retail position at sp+208, reverse at sp+192:
 *     one word between sign and position, three between position and reverse). Frame 240 with them,
 *     224 without (4 slots, 16 B; unit `frame 240/224`).
 *   - model = (Model2056 *)(u32)&D_8014A250[slot]: launder (w9 technique) so the position[0] store kills
 *     the model->type load as in retail (lbu reload + multu by a hoisted 12). It is assigned before
 *     `reverse` so that its expression web precedes the v->flags web in f_spilltemps order (home sp+92).
 *   - `(0, sign)`: comma-operator device for retail's `mul.s $f6,$f0,$f4` (sign first). Every plain spelling
 *     tried gives mpy(load, sign): both source orders, +sign, (f32)sign, sign*1.0f, register, a tbl pointer.
 *     An expression-for-sign gives the right code, but without the named `sign` the frame comes out one slot
 *     wrong.
 *   - Time updates are written `v->time = D_801543CC + 0.015f;` before the index update (as1 order).
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16;
typedef unsigned short u16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct Visual { u8 opaque0[4]; s16 index, object, slot, opaque10; u32 flags; f32 time; void (*func)(struct Visual *v, s16 op); } Visual;
typedef struct Model2056 { u8 opaque0[8]; u8 type; u8 opaque9[1007]; s16 state; u8 opaque1018[2]; s32 sound; u8 opaque1024[966]; s16 player; u8 opaque1992[64]; } Model2056;
typedef struct Pose { u8 opaque0[20]; f32 angle; } Pose;
typedef struct Player952 { u8 opaque0[232]; u32 flags; u8 opaque236[624]; s8 shift, quality, opaque862, disabled; u8 opaque864[32]; Pose *pose; u8 opaque900[52]; } Player952;
typedef struct Model68 { u32 flags; u8 opaque4[64]; } Model68;
extern Model2056 D_8014A250[];
extern Player952 player_array[];
extern Model68 D_8012E700[];
extern f32 D_8011B4B4[][3];
extern f32 D_8011418C[9];
extern f32 D_801543CC;
extern s8 D_8013FECB;
extern s8 D_80140418;
extern s16 active_player_count;
void model_data_load(s32 a, s32 mode, u32 v);
void model_transform_setup(s32 a, s32 mode, u32 v);
u32 entity_flags_apply(u32 index, u32 other, u32 value, u8 mode);
void entity_transform_calc(s32 h, f32 x, f32 y, f32 z, f32 w);
s32 osRecvMesg(void *, void *, s32);
s32 osSendMesg(void *, void *, s32);
extern s32 D_80142728;
__inline void ehu_inline(s32 h, f32 value)
{
    osRecvMesg(&D_80142728, 0, 1);
    entity_transform_calc(h, -2.0f, -2.0f, -2.0f, value);
    osSendMesg(&D_80142728, 0, 0);
}
void client_sync(s32 h, f32 opacity);
void scheduler_recv(s32 h);
void func_80092484(s16 arg0, s16 arg1);
void math_utility(void *arg0, void *arg1);
void func_80090F44(f32 angle, f32 uv[][3]);
void func_8008D6FC(s16 arg0, void *arg1, void *arg2);
void entity_spawn_init(s16 slot, u32 type, s32 fast, f32 *offset);
s32 model_bounds_calc(Visual *v, s32 on);
s32 matrix_scale_apply(Visual *v, s32 show, s32 part);

__inline void tire_sound_on(Model2056 *model)
{
    model->sound = entity_flags_apply(61, model->player, 2, 2);
    ehu_inline(model->sound, 0.75f);
    client_sync(model->sound, 0.5f);
}
__inline void tire_extend(Visual *v, Model2056 *model)
{
    model_transform_setup(v->object, 0, 15);
    v->flags |= 0x1110;
    v->time = D_801543CC + 0.015f; v->index = 0;
}
__inline void tire_sound_off(Model2056 *model)
{
    if (model->sound != -1) {
        scheduler_recv(model->sound);
        model->sound = -1;
    }
}

void buffer_swap(Visual *v, s16 op)
{
    s32 hulk, lucent;
    s16 slot;
    f32 sign;
    f32 pX;
    f32 position[3];
    f32 pY[3];
    s32 reverse;
    f32 basis[3][3];
    Model2056 *model;
    Player952 *car;

    slot = v->slot;
    if (op == 0) {
        if (v->object >= 0) model_data_load(v->object, 0, 15);
        v->func = 0;
        v->object = -1;
        return;
    }
    model = (Model2056 *)(u32)&D_8014A250[slot];
    reverse = (v->flags & 1) ? 1 : 0;
    sign = reverse ? 1.0f : -1.0f;
    position[0] = D_8011B4B4[model->type][0] * (0, sign);
    position[1] = D_8011B4B4[model->type][1];
    position[2] = 0.0f;
    if (model->state == 0 || D_8013FECB) {
        if (v->flags & 0x10) {
            v->flags &= ~0x100;
            v->flags |= 0x1200;
            if (model->sound == -1) {
                model->sound = entity_flags_apply(61, model->player, 2, 2);
                ehu_inline(model->sound, 0.75f);
                client_sync(model->sound, 0.5f);
            }
        } else if (!(v->flags & 0x1000)) {
            func_80092484(slot, reverse);
            model_data_load(v->object, 0, 15);
            return;
        }
    }
    if (model->state != 0 && !(v->flags & 0x10)) {
        tire_extend(v, model);
        if (model->sound == -1) tire_sound_on(model);
    }
    if (v->flags & 0x1000) {
        if (v->flags & 0x100) {
grow:
            if (v->time < D_801543CC) {
                position[0] *= (f32)v->index / 6.0f;
                func_8008D6FC(v->object, position, 0);
                v->time = D_801543CC + 0.015f; v->index++;
                if (v->index == 6) {
                    v->flags &= ~0x1100;
                    tire_sound_off(model);
                }
            }
        } else if (v->flags & 0x200) {
            if (model->state != 0 && !D_8013FECB) {
                v->flags &= ~0x200;
                v->flags |= 0x100;
                goto grow;
            }
            if (v->time < D_801543CC) {
                position[0] *= (f32)v->index / 6.0f;
                func_8008D6FC(v->object, position, 0);
                v->time = D_801543CC + 0.015f; v->index--;
                if (v->index == -1) {
                    v->flags &= ~0x1210;
                    tire_sound_off(model);
                }
            }
        }
    }
    if (v->flags & 0x10) {
        math_utility(D_8011418C, basis);
        func_80090F44(player_array[slot].pose->angle * 0.507f, basis);
        func_8008D6FC(v->object, 0, basis);
        if (active_player_count == 1) entity_spawn_init(slot, reverse ? 7 : 6, 0, position);
    }
    car = &player_array[slot];
    hulk = (car->flags & 0x10) != 0;
    lucent = (D_80140418 != 0) || ((car->flags & 8) != 0);
    if (model_bounds_calc(v, !hulk && !car->disabled)) {
        matrix_scale_apply(v, car->quality >= 2, car->shift);
        if (lucent) D_8012E700[v->object].flags ^= 0x80000000;
        else D_8012E700[v->object].flags &= 0x7FFFFFFF;
    }
}
