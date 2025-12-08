/**
 * Rush 2049 Decompiled Functions
 * Generated: 2025-12-08 10:23:50
 * Source: Ollama deep batch with arcade context
 */

#include "types.h"
#include "game/structs.h"

/* External declarations */
extern u8 gstate;
extern u8 gstate_prev;
extern CarData car_array[];
extern GameStruct game_struct;
extern u32 frame_counter;


// ============================================================
// Function: avoid_obstacles
// Address: 0x800A1800
// Category: ai - obstacle avoidance

void avoid_obstacles(u8* v0, u32 a0, u32 a1, u32 a2) {
    for (u32 i = 0; i < a2; i++) {
        u32 index = i * 2;
        u16 value = (*(v0 + index)) << 8 | (*(v0 + index + 1));
        s8 shift = value >> 8;
        *(v0 - shift) = shift;

        if (a3 == a0) {
            *(v0 - 1) = value & 0xFF;
        }

        u32 next_index = i * 4 + 32;
        if (i < a1) {
            u8 new_value = (*(v0 + next_index)) << 8 | (*(v0 + next_index + 1));
            s8 shift = new_value >> 8;
            *(v0 - shift) = shift;

            if (a3 == a0) {
                *(v0 - 1) = new_value & 0xFF;
            }
        }

        u32 last_index = i * 4 + 60;
        if (i < a2) {
            u8 new_value = (*(v0 + last_index)) << 8 | (*(v0 + last_index + 1));
            s8 shift = new_value >> 8;
            *(v0 - shift) = shift;

            if (a3 == a0) {
                *(v0 - 1) = new_value & 0xFF;
            }
        }
    }
}


// ============================================================
// Function: check_cps
// Address: 0x800C0000
// Category: checkpoint - checkpoint detection

#include <stdio.h>
#include <stdlib.h>

#define u32 unsigned int
#define f32 float

typedef struct {
    // CarData structure definition goes here
} CarData;

typedef struct {
    // GameStruct structure definition goes here
} GameStruct;

u8 gstate;
CarData *car_array;
GameStruct game_struct;
u32 frame_counter;

void check_cps(u32 arg0, u32 arg1) {
    f32 f4, f6, f8, f10, f14;
    s32 v0 = (s32)arg0;
    s32 v1 = (s32)arg1;

    f4 = *(f32 *)v0;
    f0 = *(f32 *)(v0 + 4);
    if (*(u8 *)v0 == 5) {
        e4460000(v0, 0); // swc1 $f6, 0($v0)
    }
    c4400000(v0, 0); // lwc1 $f0, 0($v0)

    if (*(u8 *)v0 == 3) {
        e44a0000(v0, 0); // swc1 $f10, 0($v0)
    }
    c46e0004(v1 + 4, &f14);
    c4480004(v0, &f8);

    v1 += 8;
    f6 = *(f32 *)v0;
    e44a0004(v0, 4); // swc1 $f10, 4($v0)
    c4400004(v0 + 4, &f0);

    if (*(u8 *)v0 == 3) {
        e4460004(v0, 4); // swc1 $f6, 4($v0)
    }
    f10 = *(f32 *)v0;
    c46e0004(v1 + 4, &f14);
    f8 = *(f32 *)v0;

    if (*(u8 *)v0 != arg1) {
        e4480004(v0, 4); // swc1 $f8, 4($v0)
    }
    if (arg0 != arg1) {
        return;
    }

    v0 += 8;
    *(u32 *)sp = ra; // store return address
    ldc1(f20, sp + 16); // load double register $f20 from stack
    addiu(sp, sp, 56); // adjust stack pointer
    jr(ra); // jump to return address
}


// ============================================================
// Function: init_cps
// Address: 0x800C0400
// Category: checkpoint - checkpoint init

void init_cps(u32 a1, u32 a2) {
    if (gstate != 0) {
        return;
    }

    s8 t7 = *(s8*)(a1 + 0x14);
    s8 t4 = *(s8*)(a1 + 0x1C);
    s8 t5 = *(s8*)(a1 + 0x20);

    if (t7 == -1) {
        t7 = 0;
    }
    if (t4 == -1) {
        t4 = 0;
    }
    if (t5 == -1) {
        t5 = 0;
    }

    *(s8*)(a1 + 0x00) = t7 >> 5;
    *(s8*)(a1 + 0x02) = t4 >> 5;
    *(s8*)(a1 + 0x04) = t5 >> 5;

    u32 t8 = *(u32*)(sp + 16);
    u32 t9 = *(u32*)(sp + 24);

    u32 t7_1 = (t8 << 10);
    u16 t6 = (t3 & 0x001F);
    u16 t2 = ((t7_1 >> 10) & 0x7C00);
    u16 t5_1 = (t9 << 5);
    u16 t3_1 = (t5_1 & 0x03E0);

    u16 t4_1 = (t6 | t2);
    u16 t8_1 = (t4_1 | t3_1);
    *(u16*)(a1 + 0x06) = t8_1;

    u16 a3 = 0x8015;
    u16 a2_1 = (a2 >> 5);
    if (a2_1 < a3) {
        return;
    }

    v1 += 32;
}


// ============================================================
// Function: update_lap
// Address: 0x800C0800
// Category: checkpoint - lap counting

void update_lap(CarData *car, GameStruct *game) {
    u8 state = *gstate;
    if (state == 0) {
        return;
    }

    CarData *car_array = game->car_array;
    s16 lap_count = car->lap_count;

    float time_at_checkpoint = car->time_at_checkpoint;
    float time_since_last_lap = car->time_since_last_lap;

    car_array[car->index].lap_count = lap_count;
    car_array[car->index].time_at_checkpoint = time_at_checkpoint;
    car_array[car->index].time_since_last_lap = time_since_last_lap;
}


// ============================================================
// Function: camera_update
// Address: 0x800A04C4
// Category: camera - camera system

#include <stdint.h>

#define gstate (*(volatile uint8_t*)0x801146EC)
#define car_array (*(volatile uint32_t*)0x80152818)
#define game_struct (*(volatile uint32_t*)0x801461D0)
#define frame_counter (*(volatile uint32_t*)0x80142AFC)

void camera_update() {
    __asm__ volatile (
        "addiu   $sp, $sp, -256\n"
        "lui     $at, 0x8018\n"
        "sw      $zero, -23376($at)\n"
        "lui     $at, 0x8018\n"
        "addiu   $t6, $zero, -1\n"
        "sw      $t6, -22984($at)\n"
        "lui     $v1, 0x8015\n"
        "lui     $at, 0x8018\n"
        "addiu   $v1, $v1, -26680\n"
        "sw      $zero, -23288($at)\n"
        "lw      $v0, 0($v1)\n"
        "sw      $ra, 116($sp)\n"
        "sw      $fp, 112($sp)\n"
        "addiu   $t7, $v0, 8\n"
        "sw      $s7, 108($sp)\n"
        "sw      $s6, 104($sp)\n"
        "sw      $s5, 100($sp)\n"
        "sw      $s4, 96($sp)\n"
        "sw      $s3, 92($sp)\n"
        "sw      $s2, 88($sp)\n"
        "sw      $s1, 84($sp)\n"
        "sw      $s0, 80($sp)\n"
        "sdc1    $f24, 72($sp)\n"
        "sdc1    $f22, 64($sp)\n"
        "sdc1    $f20, 56($sp)\n"
        "sw      $a0, 256($sp)\n"
        "sw      $t7, 0($v1)\n"
        "lui     $t8, 0xD9F9\n"
        "ori     $t8, $t8, 0xFFFF\n"
        "lui     $a2, 0x8014\n"
        "sw      $t8, 0($v0)\n"
        "sw      $zero, 4($v0)\n"
        "lh      $a2, 1560($a2)\n"
        "lui     $at, 0x8012\n"
        "sb      $zero, 20356($at)\n"
        "bltz    $a2, 0x800A06D8\n"
        "move    $a0, $zero\n"
        "beq     $a2, $zero, 0x800A0580\n"
        "lui     $t0, 0x8014\n"
        "addiu   $at, $zero, 1\n"
        "beq     $a2, $at, 0x800A05E8\n"
        "lui     $t0, 0x8014\n"
        "addiu   $at, $zero, 2\n"
        "beq     $a2, $at, 0x800A0650\n"
        "lui     $t0, 0x8014\n"
        "b         0x800A06D8\n"
        "nop\n"
        "lw      $v0, 0($v1)\n"
        "lui     $t6, 0xDB02\n"
        "addiu   $t7, $zero, 24\n"
        "addiu   $t9, $v0, 8\n"
        "sw      $t9, 0($v1)\n"
        "sw      $t7, 4($v0)\n"
        "sw      $t6, 0($v0)\n"
        "lw      $v0, 0($v1)\n"
        "lui     $t9, 0xDC08\n"
        "ori     $t9, $t9, 0x060A\n"
        "addiu   $t8, $v0, 8\n"
        "sw      $t8, 0($v1)\n"
        "addiu   $t0, $t0, 1720\n"
        :
        :
        : "$sp", "$at", "$v0", "$v1", "$t6", "$t7", "$t8", "$t9", "$a0", "$a2", "$s7", "$s6", "$s5", "$s4", "$s3", "$s2", "$s1", "$s0", "$f24", "$f22", "$f20"
    );
}


// ============================================================
// Function: chase_camera
// Address: 0x800A0800
// Category: camera - chase camera mode

void chase_camera(u32 frame_counter) {
    u32 *car_array = (u32*)0x80152818;
    u32 *game_struct = (u32*)0x801461D0;
    u32 gstate = *(u32*)0x801146EC;

    u32* car_data = car_array + 1; // Assuming car_array[0] is not used
    u32* game_data = game_struct + 1; // Assuming game_struct[0] is not used

    for (int i = 0; i < 8; i++) {
        if (*(u32*)car_data == 0) continue;

        u32 car_flags = *(u32*)(car_data + 9);
        if ((car_flags & 0x80) == 0) continue;

        if (gstate != 241) {
            *(u32*)(game_data + 652) = 0x800878E0;
            *(u32*)0x80165BAC = 0x8000;
        }

        u32* camera_settings = (u32*)0x8014A9FC;
        u32 camera_mode = *(u32*)(camera_settings + 2);
        if (camera_mode == 2) {
            camera_mode = 0;
        }

        if (*(u8*)camera_settings != 0) {
            u32 car_num = i;
            (*(void(*)())car_data)(car_num, frame_counter);
        }
    }
}


// ============================================================
// Function: cockpit_camera
// Address: 0x800A0C00
// Category: camera - cockpit view

#include <stdint.h>

typedef struct CarData {
    // Define the structure of CarData here
} CarData;

typedef struct GameStruct {
    // Define the structure of GameStruct here
} GameStruct;

extern uint8_t gstate;
extern CarData car_array[8];
extern GameStruct game_struct;
extern uint32_t frame_counter;

void cockpit_camera(uint8_t* camera_data, uint16_t* uvs) {
    uint8_t view_mode = *camera_data & 0x03;
    if (view_mode == 0) {
        // Handle view mode 0
        handle_view_mode_0(camera_data, uvs);
    } else if (view_mode == 1) {
        // Handle view mode 1
        handle_view_mode_1(camera_data, uvs);
    } else if (view_mode == 2) {
        // Handle view mode 2
        handle_view_mode_2(camera_data, uvs);
    }
}

void handle_view_mode_0(uint8_t* camera_data, uint16_t* uvs) {
    // Implementation for view mode 0
}

void handle_view_mode_1(uint8_t* camera_data, uint16_t* uvs) {
    // Implementation for view mode 1
}

void handle_view_mode_2(uint8_t* camera_data, uint16_t* uvs) {
    // Implementation for view mode 2
}


Please note that the actual implementation of `handle_view_mode_0`, `handle_view_mode_1`, and `handle_view_mode_2` functions is not provided as it depends on the specifics of the game's camera handling logic.

// ============================================================
// Function: render_object
// Address: 0x80099BFC
// Category: render - 3D model render

void render_object(u32 arg0, u16 arg1, u16 arg2) {
    s32 sp = -608;
    register u32 ra asm("$ra");
    register u32 t6 asm("$t6");
    register u32 t7 asm("$t7");
    register u32 v0 asm("$v0");
    register u32 t8 asm("$t8");
    register u16 a0 asm("$a0");

    t6 = car_array;
    if (arg1 != 0) {
        sp += 596;
        t7 = *(u32*)(s4 + 28);
        v0 = t6;
        t6 += 8;
        t8 = t7 << 4;
        if (t8 < 0) {
            u32* ptr = (u32*)t6;
            *ptr++ = 0xDE00;
            sp = 596;
        }
    }

    if (*(u32*)s1 == 0) {
        return;
    }

    t7 = *(u32*)arg0;
    u16 a1 = *(u16*)(arg0 + 4);
    u16 a2 = *(u16*)(arg0 + 2);

    if (gstate != 0) {
        u32 res = (*(u16*)(s0 + 2)) - (*(u16*)(s0 + 4));
        u16 res2 = (*(u16*)(s0 + 2)) - (*(u16*)(s0 + 6));
        u16 res3 = (*(u16*)(s0 + 16)) & 0xFFFF;
        u16 res4 = (*(u16*)(s0 + 18)) & 0xFFFF;

        if (res > 2) {
            res <<= 15;
        } else if (res < -2) {
            res >>= 13;
            res2 <<= 13;
        }

        if (res3 > 2) {
            res3 <<= 14;
        } else if (res3 < -2) {
            res3 >>= 12;
            res4 <<= 12;
        }
    }

    *(u16*)(s0 + 2) = a1;
    *(u16*)(s0 + 4) = a2;
}


// ============================================================
// Function: render_large
// Address: 0x80087A08
// Category: render - major render func

void render_large(u32 arg1, u32 arg2, u32 arg3, u32 arg4, u32 arg5) {
    u32 sp = 0x801146EC;
    u32 s0 = 0x80152818;
    u32 s1 = 0x801461D0;
    u32 ra = arg1;

    u32 a0 = sp + 632;
    u32 a1 = sp + 636;
    u32 a2 = sp + 640;
    u32 a3 = sp + 644;
    u32 t7 = arg5;

    if (gstate != 0) {
        return;
    }

    s1 &= 0xFFFF;
    ra = *a0;
    a0 = *a1;
    a1 = *a2;
    a2 = *a3;

    *a4 = arg4;
    *a5 = arg5;
    *a6 = t7;

    if (s0 == 0) {
        *a7 = arg5;
        u16 v0 = *(u16 *)(sp + 658);
        u32 t8 = v0 << 2;
        u32 t9 = ra + t8;
        if (t9 != 0) {
            *a4 = t9;

            if (s1 == 2) {
                v0 = *(u16 *)(sp + 658);
                t8 = s1 << 1;
                t9 = a0 + t8;
                if (t9 != 0) {
                    *a7 = v0;
                    u32 t6 = a2;
                    *a4 = t6;

                    if (s1 == 1) {
                        v0 = *(u16 *)(sp + 658);
                        u32 t8 = s1 << 1;
                        u32 t9 = a0 + t8;
                        if (t9 != 0) {
                            *a4 = t9;
                            u32 t6 = a2;
                            *a7 = v0;

                            if (s1 >= 2) {
                                u32 t8 = s1 << 1;
                                u32 t9 = a0 + t8;
                                if (t9 != 0) {
                                    *a4 = t9;
                                    u32 t6 = a2;
                                    *a7 = v0;

                                    if (s6 >= 0) {
                                        s6 >>= 1;
                                        s6 = (s6 + 1) >> 1;
                                        u32 t8 = t9 + s6;
                                        *a4 = t8;
                                    }

                                    u16 t6 = *(u16 *)(sp + 666);
                                    u16 t7 = t6 - v0;
                                    if (t7 != 0) {
                                        t7 += 1;
                                        *(u16 *)(sp + 650) = t7;

                                        if (t7 != 0) {
                                            *a4 = t6;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return;
}


// ============================================================
// Function: render_car
// Address: 0x8009C000
// Category: render - car rendering

void render_car(void *car_data, u16 game_state, void *game_struct, u32 frame_counter) {
    s32 temp;
    u8 *v0 = (u8 *)car_data;
    u16 t9 = ((u16 *)game_struct)[1];
    u16 t7 = ((u16 *)game_struct)[0];
    u16 t8 = game_state >> 5;
    u16 t6 = t8 & 31;
    u16 t9 = t7 >> 5;
    u16 t8 = (t6 + t9) >> 3;
    u16 t9 = t8 & 0x1FF;
    u16 t6 = t9 << 9;
    u16 t7 = *(u16 *)game_struct;
    u16 t8 = (t7 >> 2) & 4095;
    u16 t9 = ((u16 *)game_struct)[3];
    u16 t7 = (t9 >> 5) & 31;
    u16 t8 = t8 << 12;
    u16 t9 = (t9 >> 2) & 4095;
    u16 t7 = t8 | t9;
    *(u16 *)(v0 + 4) = t7;
    v0[0] = t6;

    if (((*(u16 *)game_struct >> 3) & 1)) {
        temp = (*(u16 *)game_struct >> 2) & 4095;
        u16 t8 = ((temp >> 5) & 31);
        u16 t7 = (t8 << 9);
        u16 t9 = *(u16 *)(v0 + 2);
        u16 t8 = (t9 >> 2) & 4095;
        u16 t9 = ((*(u16 *)game_struct >> 3) & 1);
        u16 t7 = t8 | t9;
        *(u16 *)(v0 + 2) = t7;
    }
}


// ============================================================
// Function: render_wheel
// Address: 0x8009D000
// Category: render - wheel rendering

void render_wheel(u16 param) {
    u8 *car_array = (u8 *)0x80152818;
    GameStruct *game_struct = (GameStruct *)0x801461D0;
    CarData *current_car = &car_array[(param >> 10) & 0x3FF];
    u16 car_index = (param >> 10) & 0x3FF;

    if (gstate != 0) {
        return;
    }

    u8 body_type = current_car->body_type;
    u16 *car_params = &game_struct->car_params[car_index];
    u16 param_1 = car_params[2] & 0x3FF;
    u16 param_2 = (car_params[2] >> 10) & 0x3FF;

    if (param_1 != 0) {
        car_params[2] = param_1;
    }

    if (body_type == 0 || body_type == 5 || body_type == 6 || body_type == 7) {
        return;
    }

    u16 *params = &game_struct->params[param_2];
    params[0] |= 1 << body_type;
    params[1] |= (param >> 10) & 0x3FF;

    if ((car_params[1] & 0x400) != 0) {
        return;
    }

    u16 *shadow_params = &game_struct->shadow_params[param_2];
    shadow_params[0] |= (param >> 10) & 0x3FF;

    if ((car_params[1] & 0x800) != 0) {
        return;
    }

    u16 *skid_params = &game_struct->skid_params[param_2];
    skid_params[0] |= (param >> 10) & 0x3FF;
}


// ============================================================
// Function: render_shadow
// Address: 0x8009E000
// Category: render - shadow rendering

void render_shadow(s32 arg0, f32 arg1, s16 arg2) {
    u8 game_state = *(u8*)0x801146EC;
    CarData *car_array = (CarData*)0x80152818;
    GameStruct *game_struct = (GameStruct*)0x801461D0;
    s32 frame_counter = *(s32*)0x80142AFC;

    if ((arg0 < game_state) || (arg0 > 7)) {
        return;
    }

    arg0 += 16;
    u32 *car_entry = (u32*)(game_struct->gObjList + arg0);
    CarData *car_data = &car_array[arg0];

    f32 shadow_scale = car_data->shadowScale;

    car_entry[48] = (*(f32*)&car_entry[48]) * shadow_scale;
    car_entry[52] = (*(f32*)&car_entry[52]) * shadow_scale;

    if (frame_counter & 0x1) {
        *(u32*)(game_struct->gObjList + arg0 + 36) = arg1;
    } else {
        *(u32*)(game_struct->gObjList + arg0 + 40) = arg1;
    }

    if (arg2 == 1) {
        s32 obj_num = car_data->objNum;
        if (obj_num != 0) {
            // Additional code for object-specific shadow rendering
        }
    }
}


// ============================================================
// Function: sound_control
// Address: 0x800B37E8
// Category: audio - main audio control

void sound_control(u16 a0, u16 a1, u16 a2, u16 a3) {
    s8 *sp = (s8 *)__builtin_frame_address(0);
    sp -= 40;

    sp[5] = a3;
    a3 = a3 >> 16;
    a0 = a0 << 16;
    a1 = a1 << 16;
    a2 = a2 << 16;
    a3 |= (a2 >> 16);
    a2 >>= 16;
    a1 >>= 16;
    sp[4] = a0;
    sp[3] = a1;
    sp[2] = a2;

    if (a3 > 0) {
        a2 = gstate; // Assuming gstate is global and accessible
        sp[6] = (u8)a2;
        a1 = car_array; // Assuming car_array is global and accessible
        sp[7] = (u8)a1;

        if (a3 >= 0) {
            for (int i = 0; i < a3; i++) {
                s16 *src = (s16 *)(sp[7] + i * 16);
                u8 *dest = (u8 *)(sp[5] + i * 4);

                s16 t0 = src[3];
                s16 t1 = src[2];
                s16 t9 = dest[2];
                s16 t2 = src[1];
                u32 t3 = dest[3];
                s16 t4 = src[0];
                s16 t5 = dest[1];
                s16 t6 = src[7];
                s16 t7 = dest[0];
                s16 t8 = src[6];

                u32 a0 = (u32)t9;
                u32 a1 = (u32)t2;
                u32 a2 = (u32)t5 & 0xFFFF;
                u32 a3 = dest[3] & 0xFF;

                sound_function(a0, a1, a2, a3); // Assuming sound_function is the target function

                dest[0] = t7;
                dest[1] = t8;
                dest[2] = t9;
                dest[3] = t10;
            }
        }
    }

    sp += 40;
}


// ============================================================
// Function: audio_wrapper
// Address: 0x8008A750
// Category: audio - audio forwarding

void audio_wrapper() {
    u8* car_array = (u8*)0x80152818;
    GameStruct* game_struct = (GameStruct*)0x801461D0;
    u32 frame_counter = *(u32*)0x80142AFC;

    // Function call
    func_80007270(car_array, &frame_counter, 1);
}

void func_8008A77C() {
    __asm__ volatile (
        "subu $sp, $sp, 128\n"
        "sw $a0, 128($sp)\n"
        "sw $ra, 60($sp)\n"
        "lui $a0, 0x8015\n"
        "lui $a1, 0x8015\n"
        "sw $fp, 56($sp)\n"
        "sw $s7, 52($sp)\n"
        "sw $s6, 48($sp)\n"
        "sw $s5, 44($sp)\n"
        "sw $s4, 40($sp)\n"
        "sw $s3, 36($sp)\n"
        "sw $s2, 32($sp)\n"
        "sw $s1, 28($sp)\n"
        "sw $s0, 24($sp)\n"
        "addiu $a1, $a1, 17308\n"
        "addiu $a0, $a0, 17272\n"
        "jal 0x80006A00\n"
        "addiu $a2, $zero, 1\n"
        "lui $a0, 0x8003\n"
        "lui $a2, 0x8015\n"
        "addiu $a2, $a2, 17272\n"
        "addiu $a0, $a0, -5912\n"
        "jal 0x800005D4\n"
        "addiu $a1, $sp, 120\n"
        "lui $fp, 0x8012\n"
        "lui $s7, 0x8012\n"
        "lui $s3, 0x8003\n"
        "addiu $s3, $s3, 21592\n"
        "addiu $s7, $s7, -5404\n"
        "addiu $fp, $fp, -4884\n"
        "addiu $s6, $zero, 4\n"
        "addiu $s5, $zero, 1\n"
        "lui $a0, 0x8015\n"
        "addiu $a0, $a0, 17272\n"
        "addiu $a1, $sp, 116\n"
        "jal 0x80007270\n"
        "or $a2, $s5, $zero\n"
        "lui $t6, 0x8011\n"
        "lb $t6, 6476($t6)\n"
        "bne $t6, $zero, 0x8008A850\n"
        "lui $at, 0x8011\n"
        "lui $a1, 0x8015\n"
        "addiu $a1, $a1, 10212\n"
        "sb $s5, 6476($at)\n"
        "jal 0x80006A00\n"
    );
}


// ============================================================
// Function: play_sfx
// Address: 0x80095360
// Category: audio - sound effect play

void play_sfx(void *arg0, void *arg1, u32 arg2) {
    s32 index = ((u32)arg1 & 0xFF) << 2;
    CarData *car_data = car_array + (index >> 5);
    GameStruct *game_struct_ptr = game_struct;
    u8 state = gstate;

    car_data->sfx_id = arg1;
    car_data->frame_counter = frame_counter;
    car_data->sound_channel = state & 0x3F;

    CarData *car_data2 = car_array + (index >> 5);
    u8 sound_type = car_data2->sound_type;
    u32 sfx_id = arg1;
    u32 sfx_id2 = car_data2->sfx_id;

    if ((u32)sound_type == 4) {
        return;
    }

    u32 sfx_bank = (arg2 >> 20) & 0xFF;
    u32 sfx_number = arg2 & 0xFFFF;
    u32 sfx_address = game_struct_ptr->sound_bank[sfx_bank] + sfx_number * 4;

    if ((u8)sfx_type != 1 || (u32)sfx_id == (u32)sfx_id2) {
        return;
    }

    func_8009211C(sfx_address, car_data);
    func_80091FBC(car_data, sfx_address);
}


// ============================================================
// Function: bgm_playback
// Address: 0x80095960
// Category: audio - music playback

void bgm_playback() {
    u32* t7 = (u32*)gstate;
    u32* t8 = &car_array[0];
    u32* s4 = (u32*)(sp + 52);
    u32 v1 = *t7;

    if (*t8 != 0) {
        u32* a0 = (u32*)0x801461D0;
        u32* a1 = &game_struct->field_12;
        memset(a1, 0, 9);
        sp -= 64;

        func_8009211C();
        memcpy(s4 + 13, a1 + 12, 8);

        u32 t6 = 2;
        a1[2] = t6;
        u32* t7 = (u32*)0x80146A58;
        u32 t9 = 1;
        s4[3] = t8 + 12;
        a1[3] = t9;

        func_800953CC();
        memset(s4 + 4, 0, 8);

        sp += 64;
    }

    u32* ra = (u32*)(sp + 20);
    u32* s0 = (u32*)(sp + 24);
    u32* s1 = (u32*)(sp + 28);
    u32* s2 = (u32*)(sp + 32);
    u32* s3 = (u32*)(sp + 36);
    u32* s4 = (u32*)(sp + 40);

    ra = (u32*)(sp + 20);
    s0 = (u32*)(sp + 24);
    s1 = (u32*)(sp + 28);
    s2 = (u32*)(sp + 32);
    s3 = (u32*)(sp + 36);
    s4 = (u32*)(sp + 40);

    func_800959DC();
}

void func_800959DC() {
    sp -= 24;
    u32* ra = (u32*)(sp + 20);
    u32 t6 = 3;
    s0[4] = t6;
    u32* a0 = s0 + 1;
    func_8009211C();
    u32 at = -1;
    sp -= 72;

    u32* ra = (u32*)(sp + 44);
    u32* s4 = (u32*)(sp + 40);
    u32* s3 = (u32*)(sp + 36);
    u32* s2 = (u32*)(sp + 32);
    u32* s1 = (u32*)(sp + 28);

    ra = (u32*)(sp + 44);
    s4 = (u32*)(sp + 40);
    s3 = (u32*)(sp + 36);
    s2 = (u32*)(sp + 32);
    s1 = (u32*)(sp + 28);

    func_80095A24();
}

void func_80095A24() {
    sp -= 72;
    u32 at = -1;
    u32* ra = (u32*)(sp + 44);
    u32* s4 = (u32*)(sp + 40);
    u32* s3 = (u32*)(sp + 36);
    u32* s2 = (u32*)(sp + 32);
    u32* s1 = (u32*)(sp + 28);

    ra = (u32*)(sp + 44);
    s4 = (u32*)(sp + 40);
    s3 = (u32*)(sp + 36);
    s2 = (u32*)(sp + 32);
    s1 = (u32*)(sp + 28);

    func_80095A28();
}


// ============================================================
// Function: process_input
// Address: 0x800C9AE0
// Category: input - input processing

void process_input(u8 *gstate, CarData car_array[], GameStruct *game_struct, u32 frame_counter) {
    __asm__ volatile (
        "addiu   $sp, $sp, -80\n"
        "lui     $t7, 0x8003\n"
        "sw      $ra, 36($sp)\n"
        "sw      $s2, 32($sp)\n"
        "sw      $s1, 28($sp)\n"
        "sw      $s0, 24($sp)\n"
        "addiu   $t7, $t7, 21618\n"
        "addiu   $t6, $zero, 1\n"
        "sb      $t6, 0($t7)\n"
        "addiu   $t8, $zero, 2750\n"
        "lui     $a0, 0x8003\n"
        "sh      $t8, 48($sp)\n"
        "addiu   $a0, $a0, -4872\n"
        "addiu   $a1, $sp, 48\n"
        "jal     0x800075E0\n"
        "addiu   $a2, $zero, 1\n"
        "lui     $s0, 0x8003\n"
        "lui     $t9, 0x8003\n"
        "addiu   $s0, $s0, -4928\n"
        "addiu   $s1, $sp, 44\n"
        "addiu   $t9, $t9, 21617\n"
        "sb      $zero, 0($t9)\n"
        "or      $a1, $s1, $zero\n"
        "or      $a0, $s0, $zero\n"
        "jal     0x80007270\n"
        "move    $a2, $zero\n"
        "addiu   $s2, $zero, -1\n"
        "or      $a0, $s0, $zero\n"
        "or      $a1, $s1, $zero\n"
        "jal     0x80007270\n"
        "addiu   $a2, $zero, 1\n"
        "or      $a0, $s0, $zero\n"
        "or      $a1, $s1, $zero\n"
        "jal     0x80007270\n"
        "move    $a2, $zero\n"
        "beq     $v0, $s2, 0x800C9B88\n"
        "or      $a0, $s0, $zero\n"
        "or      $a1, $s1, $zero\n"
        "jal     0x80007270\n"
        "move    $a2, $zero\n"
        "0x800C9B88:\n"
        "lui     $t1, 0x8003\n"
        "addiu   $t1, $t1, 21617\n"
        "addiu   $t0, $zero, 1\n"
        "lui     $t2, 0x8003\n"
        "sb      $t0, 0($t1)\n"
        "addiu   $t2, $t2, 21616\n"
        "sb      $zero, 0($t2)\n"
        "lui     $t3, 0x8015\n"
        "lw      $t3, -26680($t3)\n"
        "lui     $at, 0x8015\n"
        "jal     0x800013F4\n"
        "sw      $t3, -26636($at)\n"
        "jal     0x800C997C\n"
        "jal     0x800C9528\n"
        "lw      $ra, 36($sp)\n"
        "lw      $s0, 24($sp)\n"
        :
        : "r"(gstate), "r"(car_array), "r"(game_struct), "r"(frame_counter)
        : "$t7", "$t8", "$t9", "$t1", "$t2", "$t3", "$a0", "$a1", "$a2", "$s0", "$s1", "$s2"
    );
}


// ============================================================
// Function: read_controllers
// Address: 0x800140B0
// Category: input - controller read

#include <stdint.h>

typedef struct {
    uint8_t controller_data;
} CarData;

typedef struct {
    // Define GameStruct members as needed
} GameStruct;

extern uint8_t gstate;
extern CarData car_array[8];
extern GameStruct game_struct;
extern uint32_t frame_counter;

void read_controllers(void) {
    // Function implementation goes here
}

