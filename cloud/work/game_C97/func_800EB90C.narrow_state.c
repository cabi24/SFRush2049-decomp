/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
typedef struct C97Player {
    u8 unknown0[8]; f32 position[3]; u8 unknown20[60];
    f32 basis[9]; u8 unknown116[128]; s32 model;
    u8 unknown248[612]; s8 selection, mode, next_mode; u8 unknown863[89];
} C97Player;
typedef struct C97Vehicle {
    u8 unknown0[1484]; f32 contact_value[4]; u8 unknown1500[16];
    f32 contact_height[4]; u8 unknown1532[460]; s16 active;
    u8 unknown1994[2]; s8 state; u8 unknown1997[18]; s8 hidden;
    u8 unknown2016[40];
} C97Vehicle;
typedef struct C97Object { u8 unknown0[36]; f32 position[3]; } C97Object;
typedef struct C97Model { u8 unknown0[8]; C97Object *object; u8 unknown12[56]; } C97Model;
typedef struct C97File { u8 unknown0[44]; s32 state; } C97File;
typedef struct C97Input {
    u8 controller; u8 unknown1[3]; u32 buttons; u8 unknown8[44];
    u32 pressed; u8 unknown56[16]; C97File **handle;
} C97Input;
typedef struct C97Camera {
    u8 unknown0[132]; f32 position[3]; u8 unknown144[4];
    u8 player; u8 unknown149[3];
} C97Camera;
extern s32 gameplay_mode, state_word_a;
extern u32 D_8015697C, D_8015699C, D_8011735C;
extern s16 D_801543CA, D_80151AD0;
extern f32 D_801543CC, D_801106A4[], D_801106B4[];
extern u8 D_80150C04;
extern s8 D_80110680[], D_801525F8[];
extern f32 D_80152680[], D_801526A8[][3];
extern C97Player D_80152818[];
extern C97Vehicle D_8014A250[];
extern C97Input D_8014A118[];
extern C97Camera D_80150B70[];
extern C97Model D_8012E700[];
extern f32 viGetTimeToDeadline(void);
extern void func_800E9234(C97Player *);
extern void world_object_destroy(C97Player *);
extern void func_800EB028(C97Player *);
extern void func_800CC8C8(C97File **,u8);
extern void func_800CFCA8(void);
extern void math_utility(f32 *,C97Object *);
void func_800EB90C(void)
{
    s32 selected, index;
    s16 car;
    C97Player *player;
    C97Vehicle *vehicle;
    C97Input *input;
    C97Camera *camera;
    C97Object *object;
    s8 mode, selection;
    f32 *last_time, *delay;
    s32 random_value, random_next;
    u32 choice;
    s8 *contacts;
    f32 *average;
    s32 contact_count, model;
    if (gameplay_mode == 2 && (D_8015697C & 2) && (D_8015699C & 4)) {
        selected = D_80150C04;
        player = &D_80152818[selected];
        player->selection = -1;
        for (;;) {
            selected++;
            player++;
            if (selected >= D_801543CA) {
                player = D_80152818;
                selected = 0;
            }
            if (D_8014A250[selected].active) break;
            if (player->selection >= 0) break;
        }
        player->selection = 0;
        D_80150C04 = selected;
    }
    input = D_8014A118;
    camera = D_80150B70;
    for (index = 0; index < D_80151AD0; index++, camera++) {
        selected = camera->player;
        player = &D_80152818[selected];
        vehicle = &D_8014A250[selected];
        mode = player->mode;
        selection = player->selection;
        if (state_word_a & 8) {
            last_time = &D_801106A4[selection];
            delay = &D_801106B4[selection];
            if ((*delay < D_801543CC - *last_time || D_801543CC < *last_time) && mode != 4) {
                if (viGetTimeToDeadline() > 3.0f) {
                    *last_time = D_801543CC;
                    random_value = D_8011735C * 1103515245 + 12345;
                    D_8011735C = random_value;
                    random_next = random_value * 1103515245 + 12345;
                    D_8011735C = random_next;
                    *delay = (f32)((f64)((((s32)random_value >> 16) & 32767) * 1.0f / 32768.0f) + 2.5);
                    choice = (u32)((((s32)random_next >> 16) & 32767) * 11.0f / 32768.0f);
                    switch (choice) {
                    case 0: mode = 0; break;
                    case 1:
                        mode = 1;
                        D_801526A8[selection][0] = 0.0f;
                        D_801526A8[selection][1] = 0.0f;
                        D_801526A8[selection][2] = 0.0f;
                        break;
                    case 2: if (mode != 3) mode = 2; break;
                    case 3: if (mode != 2) mode = 3; break;
                    case 4: func_800E9234(player); mode = 10; break;
                    default:
                        mode = 6;
                        world_object_destroy(player);
                        *delay = 3.0f;
                        break;
                    }
                }
            }
        } else if (D_80110680[input->controller] && mode != 4) mode = 5;
        else if (mode != 4 && (input->pressed & input->buttons)) {
            mode++;
            if (mode >= 4) mode = 0;
            if ((*input->handle)->state) func_800CC8C8(input->handle, (u8)mode);
            if (mode == 1) {
                D_801526A8[player->selection][0] = 0.0f;
                D_801526A8[player->selection][1] = 0.0f;
                D_801526A8[player->selection][2] = 0.0f;
            } else if (mode == 2) {
                func_800E9234(player);
                D_80150B70[player->selection].position[0] = player->position[0];
                D_80150B70[player->selection].position[1] = player->position[1] + 1.0f;
                D_80150B70[player->selection].position[2] = player->position[2];
            } else if (mode == 3) func_800E9234(player);
        }
        if (mode != player->mode) {
            player->next_mode = mode;
            player->mode = mode;
        }
        func_800EB028(player);
        contacts = &D_801525F8[selection];
        average = &D_80152680[selection];
        *contacts = 0;
        *average = 0.0f;
        input++;
        if (vehicle->contact_height[0] < 4.0f) { (*contacts)++; *average += vehicle->contact_value[0]; }
        if (vehicle->contact_height[1] < 4.0f) { (*contacts)++; *average += vehicle->contact_value[1]; }
        if (vehicle->contact_height[2] < 4.0f) { (*contacts)++; *average += vehicle->contact_value[2]; }
        if (vehicle->contact_height[3] < 4.0f) { (*contacts)++; *average += vehicle->contact_value[3]; }
        contact_count = *contacts;
        if (contact_count >= 3) *average /= (f32)contact_count;
        else *average = 0.0f;
    }
    func_800CFCA8();
    for (car = 0; car < 6; car++) {
        vehicle = &D_8014A250[car];
        if (vehicle->active) {
            player = &D_80152818[car];
            model = player->model;
            if (model >= 0 && vehicle->state != 2 && !vehicle->hidden) {
                object = D_8012E700[(s16)model].object;
                object->position[0] = player->position[0];
                object->position[1] = player->position[1];
                object->position[2] = player->position[2];
                object->position[1] += 0.25f;
                math_utility(player->basis,object);
            }
        }
    }
}
