/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete ordinary-boundary reconstruction; historical function names retained. */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct PlayerState {
    u8 unknown000[0xED];
    s8 transition;
    u8 unknown0EE[0x35C - 0xEE];
    s8 effect_index;
    s8 effect_mode;
    s8 saved_effect_mode;
    u8 unknown35F[0x3B8 - 0x35F];
} PlayerState;
typedef struct ModelState {
    u8 unknown000[0x22C];
    float position[3];
    u8 unknown238[0x7C6 - 0x238];
    s16 slot;
    u8 unknown7C8[4];
    s8 mode;
    u8 unknown7CD[0x808 - 0x7CD];
} ModelState;
typedef struct ScoreRecord {
    u8 unknown00[0x40];
    u16 transitions;
    u8 unknown42[0x4C - 0x42];
} ScoreRecord;
typedef struct EffectRecord {
    u8 unknown00[0x24];
    float position[3];
    u8 unknown30[0x84 - 0x30];
    float saved_position[3];
    u8 unknown90[8];
} EffectRecord;
typedef struct ObjectHeader { u8 unknown0[5]; s8 flags; } ObjectHeader;
typedef struct ObjectLink { ObjectHeader *header; } ObjectLink;
typedef struct ObjectBody { u8 unknown00[0x28]; ObjectLink *link; } ObjectBody;
typedef struct ObjectRef { ObjectBody *body; } ObjectRef;
typedef struct SlotStatus { u8 unknown0[7]; u8 state; } SlotStatus;

extern int D_8014A110;
extern s16 D_8014A108;
extern s8 D_80152744, D_8010FFC0, D_80114650;
extern u32 D_801174B4;
extern PlayerState D_80152818[];
extern ModelState D_8014A250[];
extern ScoreRecord D_8014A118[];
extern EffectRecord D_80150B70[];
extern ObjectRef *D_80152698[];
extern u32 D_801392D8[];
extern SlotStatus D_80153E88[];
extern u8 D_801141B0[];

extern void race_init_helper(void);
extern void camera_scene_manager(void);
extern void func_8038FCE0(void);
extern void func_80390F60(void);
extern void func_800D169C(void);
extern void render_large_objects(void);
extern void func_800F8EC8(void);
extern void hud_render(void);
extern void camera_position_update(void);
extern void save_write_data(void *, int, float, int);
extern void func_800D5524(ModelState *);
extern int entity_flags_apply(int, int, int, u8);
/* The second/fourth carriers are supplied natively but unused by this callee. */
extern int camera_target_track(float *, const void *, float, float, float,
                               float, int, int, int, u8);
extern void func_800C3578(int);
extern void func_800F8E90(s16);
extern void cpak_read(s16);
extern void race_setup_2(s16);
extern void race_setup_1(void);
extern void menu_controller_remap(void);
extern void skid_mark_render(void);

void render_viewport_init(void)
{
    int i;
    s16 slot;
    PlayerState *player;
    ModelState *model;
    EffectRecord *effect;
    ObjectRef *object;

    race_init_helper();
    if (D_8014A110 == 4) {
        camera_scene_manager();
    } else if (D_8014A110 == 6) {
        func_8038FCE0();
        func_80390F60();
    } else {
        if (D_8014A110 == 5) {
            func_800D169C();
        } else {
            render_large_objects();
        }
        func_800F8EC8();
    }
    hud_render();
    camera_position_update();
    player = D_80152818;
    model = D_8014A250;
    for (i = 0; i < D_80152744; i++, player++, model++) {
        slot = model->slot;
        if (player->transition < 0) {
            if (!(D_801174B4 & 8) && slot < D_8014A108) {
                D_8014A118[slot].transitions++;
            }
            if (player->effect_index >= 0) {
                effect = &D_80150B70[player->effect_index];
                player->saved_effect_mode = player->effect_mode;
                player->effect_mode = 4;
                effect->saved_position[0] = effect->position[0];
                effect->saved_position[1] = effect->position[1];
                effect->saved_position[2] = effect->position[2];
            }
            save_write_data(&slot, 1, 1.0f, 0);
            func_800D5524(model);
            if (model->mode == 2) {
                if (D_8014A110 != 2 ||
                    !(object = D_80152698[model->slot]) ||
                    object->body->link->header->flags < 0) {
                    if (D_8010FFC0) {
                        entity_flags_apply(45, model->slot, 1, 1);
                    }
                }
            } else if (D_8010FFC0) {
                camera_target_track(model->position, D_801141B0, 400.0f,
                                    0.0f, 1.0f, 0.0f, 45, model->slot, 0, 128);
            }
            if (D_8014A110 == 4) {
                func_800C3578(slot);
            }
            player->transition = 0;
        } else if (player->transition > 0) {
            D_801392D8[slot] &= ~32u;
            if (player->effect_index >= 0) {
                player->effect_mode = player->saved_effect_mode;
                func_800F8E90(player->effect_index);
            }
            player->transition = 0;
        }
        cpak_read(slot);
        if ((D_80153E88[slot].state == 0 || D_80153E88[slot].state == 6) &&
            (D_8014A110 != 2 || slot == 0)) {
            race_setup_2(slot);
        }
    }
    race_setup_1();
    menu_controller_remap();
    if (!D_80114650) {
        skid_mark_render();
    }
}
