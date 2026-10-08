/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* AF06C effect service and its genuine private 90308 helper. Research source. */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct EffectNode EffectNode;
struct EffectNode {
    EffectNode *next;
    s16 state;
    s16 scene;
    s16 index;
    u16 unknown0A;
    u32 data;
    f32 timer;
    void (*callback)(EffectNode *, s16);
};
typedef struct Transform {
    f32 matrix[9];
    f32 position[3];
} Transform;
typedef struct Player {
    u8 unknown000[8];
    f32 position[3];
    u8 unknown014[0x3B8 - 0x14];
} Player;
typedef struct ExtraEffect {
    s32 scene;
    Transform transform;
    u8 alpha;
    u8 phase;
    u16 unknown036;
    f32 timer;
} ExtraEffect;
typedef struct Debris {
    s32 scene;
    Transform transform;
    f32 scale;
    f32 speed;
    f32 velocity[3];
    f32 angle;
    u8 alpha;
    u8 lifetime;
    u16 unknown04E;
    f32 timer;
} Debris;
typedef struct EffectGroup {
    Debris debris[4];
    ExtraEffect extra;
    f32 timer;
} EffectGroup;
typedef struct Scene {
    u8 unknown00[12];
    f32 scale;
    f32 rate;
    u8 unknown14[40];
    u32 color;
    u8 unknown40[4];
} Scene;

extern s8 D_80156994, D_8014978C, D_8010FFC0;
extern s16 D_8014A108, D_8013C094;
extern u32 D_8011735C, D_8011B554;
extern f32 D_801543CC;
extern f32 D_801239A8, D_801239AC, D_801239B0;
extern f32 D_801239B4, D_801239B8, D_801239BC;
extern f32 D_801239C0, D_801239C4, D_80123C00, D_80123C04;
extern f32 D_8011418C[9], D_801141B0[3];
extern u16 D_801428FC[4], D_80142904, D_80142906, D_80142908;
extern Player D_80152818[];
extern EffectGroup D_80154660[];
extern ExtraEffect D_80154FD8[];
extern Scene D_8012E700[];
extern Transform *D_8013C238[50];
extern EffectNode *D_801391F0;

extern EffectNode *func_80090284(void);
extern void math_utility(const f32 *, f32 *);
extern void entity_spawn_callback(s16, s32, s32);
extern s32 func_8008E26C(s32, void *, s16, s32);
extern void entity_spawn_init(s16, s32, s32, s32);
extern s32 camera_target_track(const f32 *, const void *, f32, f32,
                              f32, f32, u32, s32, u32, u8);
extern void entity_collision_detect(EffectNode *, s16);
extern void entity_physics_update(EffectNode *, s16);
extern void entity_update_callback(EffectNode *, s16);

/* Native 90308 is a private saved-register-clobbering child. Keeping its real
 * body here admits whole-unit compilation; this is not an ordinary extern. */
static void func_80090308(s16 index)
{
    EffectNode *node;
    EffectGroup *group;
    Debris *debris;
    Player *player;
    Scene *scene;
    s32 i;
    f32 random_value;
    f32 scale, range, center;

    node = func_80090284();
    if (node != 0) {
        node->callback = entity_update_callback;
        node->timer = D_801543CC;
        node->index = index;
        group = &D_80154660[index];
        player = &D_80152818[index];
        scale = D_801239A8;
        range = D_801239AC;
        center = D_801239B0;
        for (i = 0; i < 4; i++) {
            debris = &group->debris[i];
            if (debris->scene != -1) {
                entity_spawn_callback((s16)debris->scene, 0, 0);
            }
            math_utility(D_8011418C, debris->transform.matrix);
            debris->transform.position[0] = player->position[0];
            debris->transform.position[1] = player->position[1] - 3.0f;
            debris->transform.position[2] = player->position[2];
            debris->scene = func_8008E26C(D_801428FC[i],
                                         &debris->transform, -1, 0x42000);
            debris->scale = scale;
            D_8011735C = D_8011735C * 0x41C64E6Du + 12345u;
            random_value = (f32)((D_8011735C >> 16) & 0x7FFFu);
            debris->speed = random_value * D_801239B4 / 32768.0f + D_801239B8;
            D_8011735C = D_8011735C * 0x41C64E6Du + 12345u;
            random_value = (f32)((D_8011735C >> 16) & 0x7FFFu);
            debris->velocity[1] = center - random_value * range / 32768.0f;
            D_8011735C = D_8011735C * 0x41C64E6Du + 12345u;
            random_value = (f32)((D_8011735C >> 16) & 0x7FFFu);
            debris->angle = center - random_value * range / 32768.0f;
            D_8011735C = D_8011735C * 0x41C64E6Du + 12345u;
            random_value = (f32)((D_8011735C >> 16) & 0x7FFFu);
            debris->lifetime = (u8)((s32)(random_value * 10.0f / 32768.0f) + 25);
            debris->velocity[0] = 0.0f;
            debris->velocity[2] = 0.0f;
            debris->timer = 0.0f;
            debris->alpha = 128;
            debris->transform.matrix[0] = debris->scale;
            debris->transform.matrix[4] = debris->scale;
            debris->transform.matrix[8] = debris->scale;
            D_8012E700[(s16)debris->scene].color = 0xF4CD1480u;
        }
        if (D_8014A108 < 4 && (D_80156994 != 0 || D_8014978C >= 6)) {
            math_utility(D_8011418C, group->extra.transform.matrix);
            group->extra.transform.position[0] = player->position[0];
            group->extra.transform.position[1] = player->position[1];
            group->extra.transform.position[2] = player->position[2];
            if (group->extra.scene != -1) {
                entity_spawn_callback((s16)group->extra.scene, 0, 0);
            }
            group->extra.scene = func_8008E26C(D_80142904,
                                    &group->extra.transform, -1, 0x52000);
            group->extra.alpha = 95;
            group->extra.phase = 0;
            scene = &D_8012E700[group->extra.scene];
            scene->scale = range;
            scene->rate = D_801239BC;
            scene->color = 0xFFFFFF5Fu;
        }
        group->extra.timer = D_801239C0;
        group->timer = D_801239C4;
        node->next = D_801391F0;
        D_801391F0 = node;
    }
    if (D_8014A108 < 4 && (D_80156994 != 0 || D_8014978C >= 6)) {
        entity_spawn_init(index, 5, 0, 0);
    }
}

void save_write_data(void *input, s32 mode, f32 scale, s32 sound)
{
    EffectNode *node;
    ExtraEffect *extra;
    Transform *transform;
    Player *player;
    s16 index;
    u32 color;
    s32 sound_id;

    if (D_80156994 == 0 && D_8014978C >= 0 && D_8014978C < 6) {
        func_80090308(*(s16 *)input);
        return;
    }
    if (mode != 0 && D_8014A108 < 4) {
        color = D_8011B554;
        index = *(s16 *)input;
        node = func_80090284();
        if (node != 0) {
            node->callback = entity_collision_detect;
            node->index = index;
            node->timer = D_801543CC;
            extra = &D_80154FD8[index];
            math_utility(D_8011418C, extra->transform.matrix);
            player = &D_80152818[index];
            extra->transform.position[0] = player->position[0];
            extra->transform.position[1] = player->position[1];
            extra->transform.position[2] = player->position[2];
            if (extra->scene != -1) {
                entity_spawn_callback((s16)extra->scene, 0, 0);
            }
            extra->scene = func_8008E26C(D_80142906, &extra->transform,
                                        -1, 0x42000);
            extra->alpha = 240;
            extra->phase = 0;
            D_8012E700[extra->scene].color = color;
            extra->timer = D_80123C00;
            node->next = D_801391F0;
            D_801391F0 = node;
        }
    }
    node = func_80090284();
    if (node == 0) {
        return;
    }
    transform = D_8013C238[D_8013C094];
    math_utility(D_8011418C, transform->matrix);
    node->callback = entity_physics_update;
    node->timer = D_80123C04;
    if (mode != 0) {
        node->index = *(s16 *)input;
        transform->position[0] = D_80152818[node->index].position[0];
        transform->position[1] = D_80152818[node->index].position[1];
        transform->position[2] = D_80152818[node->index].position[2];
    } else {
        node->index = -1;
        transform->position[0] = ((f32 *)input)[0];
        transform->position[1] = ((f32 *)input)[1];
        transform->position[2] = ((f32 *)input)[2];
    }
    node->state = 0;
    node->scene = func_8008E26C(D_80142908, transform, -1, 0x50000);
    node->next = D_801391F0;
    D_801391F0 = node;
    D_8012E700[node->scene].scale = scale;
    D_8013C094++;
    if (D_8013C094 >= 50) {
        D_8013C094 = 0;
    }
    if (sound != 0) {
        sound_id = scale >= 1.0f ? 45 : (scale >= 0.5f ? 69 : 47);
        if (D_8010FFC0 != 0) {
            camera_target_track(transform->position, D_801141B0, 400.0f,
                                0.0f, 1.0f, 0.0f, sound_id, node->scene, 0, 128);
        }
    }
}
