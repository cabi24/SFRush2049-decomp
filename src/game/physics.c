/**
 * Rush 2049 - Physics Functions
 * Car physics, forces, tire simulation, suspension
 *
 * Auto-extracted from Ollama decompilation output
 * Source: ollama_analysis/refined_c_code_clean.txt
 */

#include "types.h"
#include "game/structs.h"

/* External declarations */
extern u8 gstate;
extern u8 gstate_prev;
extern CarData car_array[];
extern GameStruct* game_struct;
extern u32 frame_counter;
extern PadEntry pad_array[];


// === func_800B8000 @ 0x800B8000 ===

void func_800B8000(u32 arg) {
    s32 sp = -24;
    s32 at = -1;
    if (arg == at) {
        return;
    }
    u32 ra = (u32)&sp + 20;

    CarData* car = &car_array[arg];
    if ((arg << 10) < 0 || (arg << 9) >= 4) {
        return;
    }

    f32* gstate_ptr = (f32*)(gstate + 28924);
    if (*gstate_ptr == 0.0f) {
        f32 v4 = *(f32*)((u32)&game_struct + 15840);
        f32 v6 = *(f32*)0x8003EB94;
        *gstate_ptr = v4;
        *(f32*)(gstate + 0) = v6;
    }

    u32* gstate_prev_ptr = (u32*)(gstate_prev + 28924);
    if (*gstate_prev_ptr != 1 && *gstate_prev_ptr != 2 && *gstate_prev_ptr != 3 && *gstate_prev_ptr != 4) {
        u32 v0 = *(u32*)((u32)&game_struct + -24304);
        func_800B7A40(v0, v0);
    }
}



// === func_800B811C @ 0x800B811C ===

void func_800B811C(CarData* car) {
    u32 sp = 0xFFFFFE70;
    s32 pad_index = gstate & GSTATE_CARS ? 0 : 1;

    PadEntry* pad_entry = &pad_array[pad_index];
    s8 lap_count = game_struct->lap_count;

    if (car->place == 1) {
        car->mph = 0;
        f32 v_x = car->dr_vel[0];
        f32 v_y = car->dr_vel[1];
        f32 v_z = car->dr_vel[2];

        car->RWR[0] += v_x;
        car->RWR[1] += v_y;
        car->RWR[2] += v_z;

        func_8008D6B0(car);
    }
}



// === func_800B821C ===

void update_car_state(u32 car_index) {
    CarData *car = &car_array[car_index];
    u32 state = (car->place << 16) | (car->laps << 8) | car->checkpoint;

    if ((state & GSTATE_INIT) != 0) {
        // Handle initialization logic
    }

    if ((state & GSTATE_SETUP) != 0) {
        // Handle setup logic
    }

    u32 flags = state;
    if ((flags & GSTATE_CAMERA) != 0) {
        flags |= 1;
    }
    if ((flags & GSTATE_READY) != 0) {
        flags |= 3;
    } else if ((flags & GSTATE_PLAY) != 0) {
        flags |= 2;
    }

    s32 car_offset = (flags << 2) - flags;
    CarData *car_data = (CarData*)((u32)&car_array + car_offset);

    // Update game state
    gstate_prev = gstate;
    gstate |= flags;

    // Call external function with updated state
    ((void (*)(GameStruct*, u8))0x8008D6B0)(game_struct, (u8)flags);
}



// === func_800B82C8 ===

void func_800B82C8() {
    s32 i;
    u16 game_state;
    u8 car_index;

    \n"
        "sw $s2, 28($sp)\n"
        "sw $s1, 24($sp)\n"
        "sw $s0, 20($sp)\n"
        "sw $ra, 36($sp)\n"
        "move $s0, $zero\n"
        "addiu $s1, $zero, 4\n"
        "addiu $s2, $zero, 1\n"
        "addiu $s3, $zero, 2\n"

        "bne $s0, $zero, .Lcontinue\n"
        "sltiu $a0, $s2, 2\n"
        "srli $t6, $a0, 16\n"
        "beqz $zero, .Lend\n"
        "or $a0, $t6, $zero\n"

    ".Lcontinue:\n"
        "bne $s0, $s2, .Lskip\n"
        "sltiu $v0, $s3, 2\n"
        "srli $t7, $v0, 16\n"
        "beqz $zero, .Lend\n"
        "or $v0, $t7, $zero\n"

    ".Lskip:\n"
        "bne $s0, $s3, .Ldone\n"
        "addiu $v1, $zero, 8\n"
        "beqz $zero, .Lend\n"
        "or $v1, $s1, $zero\n"

    ".Ldone:\n"
        "or $v0, $v1, $zero\n"
        "sltiu $a0, $v0, 2\n"
        "srli $t7, $a0, 16\n"
        "beqz $zero, .Lend\n"
        "or $a0, $t7, $zero\n"

    ".Lend:\n"
        "jal 0x800B821C\n"
        "nop\n"
        "addiu $s0, $s0, 1\n"
        "bne $s0, $s1, .Lcontinue\n"
        "nop\n"
        "jal 0x800B811C\n"
        "nop\n"

        "lw $ra, 36($sp)\n"
        "lw $s0, 20($sp)\n"
        "lw $s1, 24($sp)\n"
        "lw $s2, 28($sp)\n"
        "lw $s3, 32($sp)\n"
        "jr $ra\n"
    );
}



// === func_800B8374 @ 0x800B8374 ===

u32 func_800B8374(void) {
    u32 local_124 = 0x2F6C; // 1025 in decimal
    CarData *car_array_ptr = (CarData *)0x80152818;
    u8 *gstate_prev_ptr = (u8 *)0x801146ED;
    GameStruct *game_struct_ptr = (GameStruct *)0x801461D0;

    s32 local_6c = 0; // s7
    s32 local_58 = 0; // s1
    s32 local_4c = 0; // s3
    s32 local_40 = 0; // s4
    s32 local_3c = 0; // s5
    s32 local_38 = 0; // s6
    u32 local_10 = 744; // fp

    *gstate_prev_ptr = (u8)local_124;
    ((u32 *)car_array_ptr)[10] = local_124;
    local_58 = gstate_prev_ptr + 60;
    local_4c = car_array_ptr + 17996;
    local_40 = game_struct_ptr + 17960;
    local_3c = car_array_ptr + 16996;
    local_38 = car_array_ptr + 6864;

    *local_58 = local_124;

    u8 state_prev = *gstate_prev_ptr;
    if (state_prev == 0xFF) {
        state_prev = 0;
    }
    *local_6c = state_prev << 24;

    ((u32 *)car_array_ptr)[10] = *local_6c;

    return 0;
}



// === func_800B8650 @ 0x800B8650 ===

void func_800B8650() {
    CarData* car_array = (CarData*)0x80152818;
    GameStruct* game_struct = (GameStruct*)0x801461D0;

    // Initialize variables
    u8* gstate = (u8*)0x801146EC;
    u8* gstate_prev = (u8*)0x801146ED;
    PadEntry* pad_array = (PadEntry*)0x80140BF0;

    // Call functions with appropriate arguments
    func_80007270(car_array, 0);
    func_800B4200(1);
    func_800075E0(car_array, 0);

    // Additional initialization and function calls
    u32 frame_counter = *(u32*)0x80142AFC;
    PadEntry* current_pad_entry = &pad_array[frame_counter % 4];

    // Load data from memory
    u8 data_valid = car_array[0].data_valid;
    if (data_valid != 0) {
        CarData* current_car = &car_array[data_valid - 1];
        f32 speed = current_car->mph;
        u16 rpm = current_car->rpm;

        // Call another function with the loaded data
        func_800B3FA4(speed, rpm);
    }
}



// === func_800B8820 @ 0x800B8820 ===

void func_800B8820() {
    s32 sp = -136;
    void (*ra)(s32);
    s32 t6 = 1;
    u8* gstate_ptr = (u8*)0x801146EC;
    CarData* car_array_ptr = (CarData*)0x80152818;

    // Call a function to perform some initial setup
    func_800B8650();
    *gstate_ptr |= 0xFFFE;

    u32 gstate = *(u32*)(gstate_ptr + 1);
    if (gstate & 0x7FFE) {
        ra(0);
        return;
    }

    s32* game_struct_ptr = (s32*)0x801461D0;
    s32 frame_counter = *(s32*)0x80142AFC;
    PadEntry* pad_array_ptr = (PadEntry*)0x80140BF0;

    s32 car_index = 0;
    while (car_index < 8) {
        CarData* car = &car_array_ptr[car_index];
        if (car->place == -1) {
            break;
        }
        car_index++;
    }

    if (car_index >= 8) {
        return;
    }

    s32 checkpoint = car->checkpoint;
    u32 flags = *(u32*)(game_struct_ptr + 4);
    if ((flags & 0x7FFE) != 0) {
        ra(0);
        return;
    }

    // Call another function with parameters
    func_800B8374(0, car_index, checkpoint);
}



// === func_800B9130 @ 0x800B9130 ===

void func_800B9130(CarData *car) {
    u32 temp;
    s32 pad_data;

    // Set up stack frame
    ");
    ");

    // Call func_800B90F8 with car_array and pad_data
    temp = (u32)car;
    pad_data = *(s32 *)pad_array[0].data;
    func_800B90F8(car_array, pad_data);

    // Call func_80096130 multiple times with game_struct members
    for (int i = 0; i < 5; i++) {
        s32 offset = 14872 + i * 8;
        func_80096130((s32)game_struct + offset);
    }

    // Call func_80002790 with specific arguments
    func_80002790(0, car->mph);

    // Clean up stack frame
    ");
    }

void func_800B9194() {
    CarData *car;
    s32 temp;

    // Set up stack frame
    car = (CarData *)*(s32 *)game_struct + 15760;
    *(u32 *)(temp = *(s32 *)pad_array[0].data) = 0;
    s32 *stack = (s32 *)(temp + 16);
    stack[10] = 100;
    stack[9] = 88;

    // Save registers
    ");
    ");
    ");
    ");
    ");

    // Call func_80002790 with specific arguments
    func_80002790(car->mph);

    // Restore registers and clean up stack frame
    ");
    ");
    ");
    ");
    ");
    }



// === func_800B9194 @ 0x800B9194 ===

void func_800B9194() {
    u32 frame_counter = *(u32*)0x80142AFC;
    CarData* car_array = (CarData*)0x80152818;
    PadEntry* pad_array = (PadEntry*)0x80140BF0;

    u32 state_flags = *(u32*)0x801146EC;
    u32 prev_state_flag = *(u32*)0x801146ED;
    GameStruct* game_struct = (GameStruct*)0x801461D0;

    u32 timer_value = 100;
    u32 lap_count = 88;
    u32 temp_var;

    \n"
        "sw %1, 32($sp)\n"
        "sw %2, 36($sp)\n"
        "sw %3, 40($sp)\n"
        "sw %4, 44($sp)\n"
        "jal 0x800B04D0\n"
        "sb $zero, 0(%0)\n"
        : "+r"(frame_counter), "+r"(car_array), "+r"(pad_array), "+r"(state_flags), "+r"(prev_state_flag)
        :
        : "memory"
    );

    *(u32*)0x80142AFC = frame_counter;
    *(CarData**)0x80152818 = car_array;
    *(PadEntry**)0x80140BF0 = pad_array;

    if ((state_flags & GSTATE_INIT) && (prev_state_flag & GSTATE_SETUP)) {
        state_flags |= GSTATE_CAMERA;
    } else {
        state_flags &= ~GSTATE_CAMERA;
    }

    if (!(state_flags & GSTATE_PLAY)) {
        return;
    }

    for (int i = 0; i < 4; i++) {
        PadEntry* pad_entry = &pad_array[i];
        CarData* car_data = &car_array[i];

        temp_var = *(u8*)0x801146EC;

        if ((temp_var & GSTATE_INIT) && (prev_state_flag & GSTATE_SETUP)) {
            temp_var |= GSTATE_CAMERA;
        } else {
            temp_var &= ~GSTATE_CAMERA;
        }

        if (!(temp_var & GSTATE_PLAY)) {
            continue;
        }

        car_data->mph = 0.0f; // Initialize speed
    }
}



// === func_800B93A8 @ 0x800B93A8 ===

#include <stdint.h>

typedef struct CarData {
    f32 dr_pos[3];
    f32 dr_vel[3];
    f32 dr_uvs[3][3];
    f32 RWV[3];
    f32 RWR[3];
    f32 mph;
    s16 rpm;
    u8 data_valid;
    s8 place;
    s8 laps;
    s8 checkpoint;
    s8 difficulty;
} CarData;

typedef struct GameStruct {
    // Other members
} GameStruct;

u8 gstate = 0x801146EC;
u8 gstate_prev = 0x801146ED;
CarData car_array[8];
GameStruct game_struct;
u32 frame_counter = 0x80142AFC;
PadEntry pad_array[4];

void func_800B93A8() {
    u32 sp = 0;
    CarData* car = &car_array[sp];
    u32 lap_count = *(u16*)((u32)game_struct + 7400);
    f32 checkpoint_time = *(f32*)((u32)&car->dr_pos + 15860);

    if (checkpoint_time < 1.0f) {
        sp++;
        car = &car_array[sp];
    }

    if (sp != 0) {
        u16 lap_current = *(u16*)&car->lap_count;
        u16 lap_next = lap_current + 1;

        if (lap_current == lap_next) {
            lap_current = lap_next - 1;
        } else {
            sp++;
            car = &car_array[sp];
        }

        u32 lap_time = *(u32*)((u32)&car->dr_pos + 48);
        lap_time -= 1;

        if (lap_time < 0) {
            lap_time += 65536;
        }
    }

    if (lap_current > lap_next) {
        u32 diff = lap_current - lap_next;
        lap_current -= diff;
    }

    sp++;
    car = &car_array[sp];
}



// === func_800B9774 @ 0x800B9774 ===

void func_800B9774(u16 arg0, CarData* car) {
    u32 state = *(u16*)(gstate + 10148);
    s8 place = car->place;

    if (state > 0) {
        *(s32*)sp -= 8;
        arg0 >>= 8;
        arg0 &= 0x7FF;
        state = *(u16*)(game_struct + 2032);
        u8 lap = 6;
        u8 checkpoint = 1;

        if (arg0 < state) {
            if (*(s8*)(gstate_prev + 8) != 0) {
                arg1 = checkpoint;
            } else {
                arg1 = 0;
            }

            u32 addr = *(u32*)(game_struct + 2536);
            addr -= arg0 * 4;
            addr *= 2;

            u8 lap_num = (u8)(addr >> 12);
            addr &= 0xFFF;
            u8 checkpoint_num = (addr >> 4) & 0xF;

            if (lap_num < lap && checkpoint_num == checkpoint) {
                // Logic here
            }
        }
    }
}



// === func_800B9A0C @ 0x800B9A0C ===

static int func_800B9A0C(u32 arg1, u32 arg2, CarData* car) {
    s32 index;
    u16 car_index = (u16)arg1;
    u32 frame_count = *frame_counter;
    
    " : : "r" (car), "r" (&s5));
    " : : "r" (car), "r" (&s4));
    " : : "r" (car), "r" (&s3));
    " : : "r" (car), "r" (&s2));
    " : : "r" (car), "r" (&s1));
    " : : "r" (car), "r" (&s0));
    
    s0 = arg1;
    s1 = arg2;
    s2 = car;
    s3 = arg3;
    
    s5 += 0x7F0;
    s4 += 0x1CE8;
    " : : "r" (car), "r" (&ra));
    
    func_800B98D8(car);
    
    if (v0 == 0) {
        index = s2 * 2;
        if (index >= 0x70) {
            return 0;
        }
        
        u16 car_index_value = pad_array[index].u16_4C;
        if (car_index_value < 2) {
            u32 offset = 0x7580 + car_index_value * 0x50;
            s16 check_point = *(s16*)(offset + 46);
            
            if (check_point < s0) {
                return 0;
            }
        }
    }
    
    return 1;
}



// === func_800B9D68 @ 0x800B9D68 ===

#include <stdint.h>

#define GSTATE_INIT 0x40000
#define GSTATE_SETUP 0x80000
#define GSTATE_MENU 0x100000
#define GSTATE_CAMERA 0x200000
#define GSTATE_READY 0x400000
#define GSTATE_PLAY 0x800000
#define GSTATE_CARS 0x1000000
#define GSTATE_FINISH 0x2000000

typedef struct CarData {
    float dr_pos[3];
    float dr_vel[3];
    float dr_uvs[3][3];
    float RWV[3];
    float RWR[3];
    float mph;
    int16_t rpm;
    uint8_t data_valid;
    int8_t place;
    int8_t laps;
    int8_t checkpoint;
    int8_t difficulty;
} CarData;

uint8_t gstate = 0x801146EC;
uint8_t gstate_prev = 0x801146ED;
CarData car_array[8];
struct GameStruct {
    // Assume additional members here
};
uint32_t frame_counter = 0x80142AFC;
struct PadEntry {
    // Assume additional members here
};
struct PadEntry pad_array[4];

void func_800B9D68() {
    uint32_t* sp = (uint32_t*)-96;
    uint32_t s2 = 0x801420F0;
    uint32_t v0 = *(uint8_t*)(s2 + 8);
    uint32_t s3 = 0;
    uint32_t s1 = 0;
    *sp = (uint32_t)s2;
    sp++;
    *sp = v0;
    sp++;
    *sp = (uint32_t)&s3;
    sp++;
    *sp = (uint32_t)&ra;
    sp++;
    *sp = (uint32_t)&fp;
    sp++;
    *sp = (uint32_t)&s7;
    sp++;
    *sp = (uint32_t)&s6;
    sp++;
    *sp = (uint32_t)&s5;
    sp++;
    *sp = (uint32_t)&s4;
    sp++;
    *sp = (uint32_t)&s1;
    sp++;
    *sp = (uint32_t)&s0;
    sp++;

    if (v0 <= 0) {
        s3 = 0;
        s1 = 0;
        fp = 1;
        s7 = 2;
        s6 = sp + 20;
        s5 = sp + 24;
        s4 = -1;
        uint32_t t6 = *(uint32_t*)(s2 + 12);
        uint8_t t1 = 0;
        uint16_t t2 = 1;
        s0 = t6 + s1;
        uint8_t t7 = *(uint8_t*)s0;
        if (t7 != s7) {
            return;
        }
        *(int8_t*)(s0 + 1) = s4;
        uint32_t t8 = *(uint32_t*)(s2 + 12);
        uint32_t t9 = t8 + s1;
        *(int16_t*)(t9 + 2) = 0;
        uint32_t t6_2 = *(uint32_t*)(s2 + 12);
        uint32_t t7_2 = t6_2 + s1;
        *(int8_t*)(t7_2 + 4) = s4;
        uint32_t t8_2 = *(uint32_t*)(s2 + 12);
        uint32_t t9_2 = t8_2 + s1;
    }
}



// === func_800BA00C @ 0x800BA00C ===

void func_800BA00C(s32 arg0, s32 arg1) {
    u32 sp = -120;
    CarData* car = &car_array[arg0];
    CarData* otherCar = &car_array[arg1];
    u8* state = (u8*)gstate_prev;
    f64 f22;
    s16 t9;

    );
    
    if (arg0 < arg1) {
        car = &car_array[arg1];
        otherCar = &car_array[arg0];
    }

    if (arg1 >= 4) return;

    u16 checkpoint = *(u16*)(0x8015 + (arg1 * 2));
    if (checkpoint != otherCar->checkpoint) return;

    if (state[3] == 0x100000 || state[3] == 0x200000) {
        u16 lap = *(u16*)(0x8014 + arg1);
        if (lap != otherCar->laps) return;
    }

    }



// === func_800BA2E8 ===

void func_800BA2E8(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    s16 state = *(u16 *)arg3;
    f32 val4 = (*(f32 *)arg0);
    f32 val6 = (*(f32 *)(arg0 + 4));
    u8 *sp = (u8 *)__builtin_alloca(56);

    __builtin_memcpy(sp + 56, &arg0, sizeof(arg0));
    __builtin_memcpy(sp + 60, &arg1, sizeof(arg1));

    f32 val18 = (*(f32 *)0x80127FDC);
    s32 idx = -1;
    u32 i = 0;

    if (state < 0) {
        *(f32 *)(sp + 32) = val4;
    } else {
        *(f32 *)(sp + 36) = val6;
    }

    if (idx >= 0) {
        u16 *val30 = (u16 *)(*(s32 *)arg1);
        s32 val38 = (*(s32 *)arg2);

        0x80127FDC)
            : "r" (val4), "r" (val6), "r" (val18), "r" (*(f32 *)0x80127FEC)
        );

        i = idx * 8;
        s32 val50 = *(s32 *)(arg1 + i);
        u16 val54 = *(u16 *)(arg1 + i + 20);

        0x80127FDC)
            : "r" (val50)
        );

        u16 *val58 = (u16 *)(*(s32 *)arg1 + 4);
        s32 val60 = *(s32 *)(arg1 + i + 4);

        0x80127FDC)
            : "r" (val60)
        );

        u16 *val64 = (u16 *)(*(s32 *)arg1 + 8);
        s32 val66 = *(s32 *)(arg1 + i + 8);

        0x80127FDC)
            : "r" (val66)
        );
    }

    return;
}



// === func_800BA46C @ 0x800BA46C ===

#include <stdint.h>

typedef struct CarData {
    f32 dr_pos[3];
    f32 dr_vel[3];
    f32 dr_uvs[3][3];
    f32 RWV[3];
    f32 RWR[3];
    f32 mph;
    s16 rpm;
    u8 data_valid;
    s8 place;
    s8 laps;
    s8 checkpoint;
    s8 difficulty;
} CarData;

u8 gstate = 0x801146EC;
u8 gstate_prev = 0x801146ED;
CarData* car_array = (CarData*)0x80152818;
GameStruct* game_struct = (GameStruct*)0x801461D0;
u32 frame_counter = *(u32*)0x80142AFC;
PadEntry* pad_array = (PadEntry*)0x80140BF0;

s32 func_800BA46C(u32 car_index, u32 player_index) {
    CarData* car = &car_array[car_index];
    CarData* player_car = &car_array[player_index];

    if (gstate != GSTATE_PLAY || gstate_prev != GSTATE_PLAY) {
        return -1;
    }

    u8 player_place = player_car->place;

    if (player_place == 0xFF || player_place > car->place) {
        return -1;
    }

    s32 offset = ((car_index * 4 + player_index * 2) << 2);
    f32* value = (f32*)((u8*)game_struct + offset);

    if (*value != 2.0f) {
        if (player_place == car->place) {
            return -1;
        }
    }

    u32 ai_difficulty = player_car->difficulty;

    f32 car_speed = car->mph;
    f32 player_speed = player_car->mph;

    if (ai_difficulty == 0 || car_speed < player_speed) {
        return 1;
    } else {
        return -1;
    }
}



// === func_800BA644 @ 0x800BA644 ===

void func_800BA644(u32 arg0, u32 arg1, u32 arg2) {
    f32* car_pos = (f32*)((u32)arg0 + 24);
    f32* car_vel = (f32*)((u32)arg0 + 32);
    CarData* car_data = (CarData*)game_struct;
    u8* game_state = (u8*)gstate;

    \n"
        "lwc1 $f6, 8(%[car_pos])\n"
        "lui $at, %hi(0x8012)\n"
        "sdc1 $f20, 8($sp)\n"
        "sw %[arg0], 56($sp)\n"
        "lwc1 $f0, 15872($at)\n"
        "li $v1, -1\n"
        "move $a1, $zero\n"
        "swc1 $f4, 32($sp)\n"
        "bltz $a2, 16(%cfa)\n"
        "swc1 $f6, 36($sp)\n"
        "lui $a0, %hi(0x8014)\n"
        "i-type op=0x11\n"
        "lw $a0, 2036($a0)\n"
        "lwc1 $f2, 12(%[car_pos])\n"
        "lwc1 $f12, 20(%[car_pos])\n"
        "lwc1 $f18, 48($sp)\n"
        "li $t1, 6\n"
        "lh $t0, 20($sp)\n"
        "bne $a1, $a2, 12(%cfa)\n"
        "move $a1, $zero\n"
        "r-type funct=0x19\n"
        "r-type funct=0x12\n"
        "addu $a3, $a0, %t2\n"
        "lh $t3, 0($a3)\n"
        "i-type op=0x11\n"
        "nop\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"
        "swc1 $f4, 40($sp)\n"
        "lh $t4, 4($a3)\n"
        "li $a3, 1\n"
        "i-type op=0x11\n"
        "nop\n"
        "i-type op=0x11\n"
        "lwc1 $f6, 36($sp)\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"

        :
        : [car_pos]"r"(car_pos), [arg0]"r"(arg0), [arg1]"r"(arg1), [arg2]"r"(arg2)
        : "$sp", "$at", "$v1", "$a1", "$t1", "$t0", "$t3", "$t4", "$f4", "$f6", "$f20", "$f0", "$f12", "$f18"
    );
}



// === func_800BA7C4 @ 0x800BA7C4 ===

void func_800BA7C4(u16 state) {
    s32 i;
    CarData *car;
    u16 prev_state = gstate_prev;

    if (state <= 0) {
        for (i = 0; i < 8; i++) {
            car = &car_array[i];
            f32 *dr_pos = car->dr_pos;
            f32 *dr_vel = car->dr_vel;
            f32 *dr_uvs = car->dr_uvs[0];

            f32 value1 = dr_pos[0];
            f32 value2 = dr_pos[1];
            f32 value3 = dr_pos[2];

            car->mph = 0.0f;
            car->rpm = 0;
        }
    } else {
        gstate_prev = state;

        for (i = 0; i < 8; i++) {
            car = &car_array[i];
            f32 *dr_pos = car->dr_pos;
            f32 *dr_vel = car->dr_vel;
            f32 *dr_uvs = car->dr_uvs[0];

            f32 value1 = dr_pos[0];
            f32 value2 = dr_pos[1];
            f32 value3 = dr_pos[2];

            car->mph = 0.0f;
            car->rpm = 0;
        }
    }

    gstate_prev = prev_state;
}



// === func_800BAAA0 @ 0x800BAAA0 ===

void func_800BAAA0() {
    s32 sp = -80;
    u8* gstate_ptr = (u8*)0x801146EC;
    u8 state = *gstate_ptr;

    \n"
        "sw $s6, 72(%1)\n"
        "sw $s5, 68(%1)\n"
        "sw $s4, 64(%1)\n"
        "sw $s3, 60(%1)\n"
        "sw $s2, 56(%1)\n"
        "sw $s1, 52(%1)\n"
        "sw $s0, 48(%1)\n"
        "sdc1 $f24, 40(%1)\n"
        "sdc1 $f22, 32(%1)\n"
        : 
        : "r"(state), "r"(&sp)
    );

    if (state != 0) {
        \n"
            : 
            : "r"(&sp)
        );
        func_800B93A8();
        func_800BA7C4();
        func_800B9D68(0xFFFFFFFF);
        func_800B9740();
        func_800B93A8();
    }

    if (state != 0) {
        \n"
            : "=r"(sp)
            : "r"(&sp)
        );
        u8* car_array_ptr = (u8*)0x80152818;
        func_80007C68(car_array_ptr + 812, *(car_array_ptr + 8));
    }
}



// === func_800BAD58 @ 0x800BAD58 ===

void func_800BAD58() {
    u32 *sp = (u32 *)malloc(24);
    sp[0] = gstate;
    sp[1] = 0x800BAAA0;

    CarData *car_array_ptr = car_array;
    for (int i = 0; i < 8; i++) {
        f32 *v0 = (f32 *)(((u32)car_array_ptr + i * sizeof(CarData)) & ~3);
        v0[0] = 0.0f;
        v0[16168 / 4] = 0.0f;
    }

    u32 addr = ((u32)car_array >> 16) & 0xFFFF;
    f32 *v0 = (f32 *)(addr << 16);
    v0[0] = 0.0f;

    free(sp);
}



// === func_800BADE0 @ 0x800BADE0 ===

void func_800BADE0(void) {
    s32 i;
    CarData *car;
    u16 value;

    car_array = (CarData *)0x80152818;
    game_struct = (GameStruct *)0x801461D0;

    func_80007C68(car_array, 812);

    for (i = 0; i < 8; i++) {
        car = &car_array[i];
        value = *(u16 *)(game_struct + 0x1CE8 + (i << 1));

        if (value != -1) {
            car->data_valid = 1;
            car->dr_pos[0] = *(f32 *)(game_struct + 0x1D20 + (i << 6));
            car->dr_pos[1] = *(f32 *)(game_struct + 0x1D24 + (i << 6));
            car->dr_pos[2] = *(f32 *)(game_struct + 0x1D28 + (i << 6));
            value = *(u16 *)(game_struct + 0x1D2C + (i << 1));

            if (value != -1) {
                car->dr_vel[0] = *(f32 *)(game_struct + 0x1D48 + (i << 6));
                car->dr_vel[1] = *(f32 *)(game_struct + 0x1D4C + (i << 6));
                car->dr_vel[2] = *(f32 *)(game_struct + 0x1D50 + (i << 6));
                value = *(u16 *)(game_struct + 0x1D54 + (i << 1));

                if (value != -1) {
                    car->dr_uvs[0][0] = *(f32 *)(game_struct + 0x1D78 + (i << 6));
                    car->dr_uvs[0][1] = *(f32 *)(game_struct + 0x1D7C + (i << 6));
                    car->dr_uvs[0][2] = *(f32 *)(game_struct + 0x1D80 + (i << 6));
                    car->dr_uvs[1][0] = *(f32 *)(game_struct + 0x1D84 + (i << 6));
                    car->dr_uvs[1][1] = *(f32 *)(game_struct + 0x1D88 + (i << 6));
                    car->dr_uvs[1][2] = *(f32 *)(game_struct + 0x1D8C + (i << 6));
                    car->dr_uvs[2][0] = *(f32 *)(game_struct + 0x1D90 + (i << 6));
                    car->dr_uvs[2][1] = *(f32 *)(game_struct + 0x1D94 + (i << 6));
                    car->dr_uvs[2][2] = *(f32 *)(game_struct + 0x1D98 + (i << 6));
                }
            }
        }
    }
}



// === func_800BAF64 @ 0x800BAF64 ===

void func_800BAF64() {
    u8* game_state = (u8*)0x801146EC;
    *game_state = 0;

    func_800BADE0();

    *game_state = 1;
}

void func_800BAF98(void* param) {
    CarData* car_array = (CarData*)0x80152818;
    u32 frame_counter = *(u32*)0x80142AFC;

    if (*(s8*)param != 0) {
        func_80007270(car_array, param, 1);
    }

    if (*(u32*)0x80152818 == 1) {
        return;
    }

    CarData* car = &car_array[frame_counter];
    s8 checkpoint = car->checkpoint;

    if (checkpoint != 0) {
        u8 state_flags = *(u8*)game_state;
        state_flags |= 0x80000;
        *(u8*)game_state = state_flags;
    }
}



// === func_800BAF98 @ 0x800BAF98 ===

#include <stdint.h>

#define GSTATE_INIT 0x40000
#define GSTATE_SETUP 0x80000
#define GSTATE_MENU 0x100000
#define GSTATE_CAMERA 0x200000
#define GSTATE_READY 0x400000
#define GSTATE_PLAY 0x800000
#define GSTATE_CARS 0x1000000
#define GSTATE_FINISH 0x2000000

typedef struct CarData {
    float dr_pos[3];
    float dr_vel[3];
    float dr_uvs[3][3];
    float RWV[3];
    float RWR[3];
    float mph;
    int16_t rpm;
    uint8_t data_valid;
    int8_t place;
    int8_t laps;
    int8_t checkpoint;
    int8_t difficulty;
} CarData;

uint8_t* gstate = (uint8_t*)0x801146EC;
uint8_t* gstate_prev = (uint8_t*)0x801146ED;
CarData* car_array = (CarData*)0x80152818;
void* game_struct = (void*)0x801461D0;
uint32_t frame_counter = *(uint32_t*)0x80142AFC;
void* pad_array = (void*)0x80140BF0;

void func_800BAF98(void* arg0) {
    uint32_t temp = ((uint32_t)arg0 & 0xFFFF) << 16;
    CarData* car = car_array + (temp >> 2);

    if (*gstate != GSTATE_PLAY) {
        return;
    }

    uint32_t flag = *(uint32_t*)((uint32_t)game_struct + 0x27C8);
    uint32_t pad_index = temp >> 16;

    if (pad_index > 3) {
        return;
    }

    uint32_t* pad_data = (uint32_t*)(pad_array + pad_index * 4);

    if (*gstate_prev != GSTATE_PLAY) {
        *pad_data = 0;
        return;
    }

    uint8_t state = *pad_data & 0xFF;

    if (state == 0x01) {
        car->dr_pos[2] -= 1.5f;
    } else if (state == 0x02) {
        car->dr_pos[2] += 1.5f;
    }

    *pad_data = 0;
}



// === func_800BB044 @ 0x800BB044 ===

void func_800BB044(u32 arg0, u16 arg1, void* arg2) {
    s32 sp = -32;
    u8 pad_data[3];
    CarData* car = (CarData*)arg0;
    PadEntry* pad_entry = (PadEntry*)arg2;

    *pad_data = 0;
    *(pad_data + 16) = 0;
    *(pad_data + 17) = 255;

    if (arg1 != 0) {
        *(u16*)(pad_data + 18) = arg1;
    }

    func_800BAF98(arg0, arg1, arg2);

    s32 result = func_800BAF98(arg0, arg1, arg2);
    if (result < 512) {
        car->dr_uvs[0][1] = *(pad_entry + (arg1 * 4));
        car->dr_uvs[1][1] = *(pad_entry + ((arg1 * 4) + 1));

        u8 value1 = pad_data[1];
        u8 value2 = pad_data[2];
        if (value1 != 0 || value2 != 0) {
            u32 offset = ((value2 << 8) | value1);
            car->RWV[0] = *(car_array + (offset * 4));
        }
    }

    return;
}



// === func_800BB140 @ 0x800BB140 ===

void func_800BB140(u32 arg0) {
    s32 sp = (s32)&sp - 168;
    s32 ra = (s32)&ra;
    s32 fp = (s32)&fp;
    s32 s7 = (s32)&s7;
    s32 s6 = (s32)&s6;
    s32 s5 = (s32)&s5;
    s32 s4 = (s32)&s4;
    s32 s3 = (s32)&s3;
    s32 s2 = (s32)&s2;
    s32 s1 = (s32)&s1;
    s32 s0 = (s32)&s0;

    __m128 f22, f20;

    if (arg0 != 0) {
        f20 = *(__m128 *)(sp + 32);
        func_8039156C();
    }

    f22 = *(__m128 *)(sp + 40);

    gstate_prev = gstate;

    if (gstate == GSTATE_INIT) {
        CarData *car;
        PadEntry *pad;
        for (s3 = 0; s3 < 8; s3++) {
            car = &car_array[s3];
            pad = &pad_array[s3];

            car->mph = pad->speed;
            car->rpm = pad->rpm;
            car->data_valid = 1;
            car->place = -1;
            car->laps = 0;
            car->checkpoint = 0;
        }
    }

    *(u32 *)fp = ra;
}



// === func_800BB69C @ 0x800BB69C ===

u8 *gstate = (u8 *)0x801146EC;
CarData *car_array = (CarData *)0x80152818;

void func_800BB69C() {
    u8 gameState = *gstate;
    s32 sp = -48;

    register u32 ra asm("$ra");
    register u32 s5 asm("$s5");
    register u32 s4 asm("$s4");
    register u32 s3 asm("$s3");
    register u32 s2 asm("$s2");
    register u32 s1 asm("$s1");
    register u32 s0 asm("$s0");

    if (gameState <= 0) {
        s0 = sp;
        ra = func_800B90F8();
    }

    if (!(gameState & GSTATE_CARS)) {
        return;
    }

    u32 carIndex = gameState >> 16;

    if (carIndex == 7 || carIndex == 8 || carIndex == 9 || carIndex == 10 ||
        carIndex == 11 || carIndex == 12 || carIndex == 13 || carIndex == 17 ||
        carIndex == 18) {
        ra = func_800B90F8();
    }
}



// === func_800BB834 @ 0x800BB834 ===

void func_800BB834(u8* arg0) {
    CarData* car_data = (CarData*)(game_struct + 16328);
    PadEntry* pad_entry = &pad_array[130];
    u8* state_flags = (u8*)(gstate_prev + 18000);
    u8* state_flags_new = (u8*)(car_data->RWV);

    // Set initial values in CarData
    car_data->dr_pos[0] = 0.0f;
    car_data->dr_pos[1] = 0.0f;
    car_data->dr_pos[2] = 0.0f;
    car_data->dr_vel[0] = 0.0f;
    car_data->dr_vel[1] = 0.0f;
    car_data->dr_vel[2] = 0.0f;
    car_data->RWV[0] = 0.0f;
    car_data->RWV[1] = 0.0f;
    car_data->RWV[2] = 0.0f;
    car_data->RWR[0] = 0.0f;
    car_data->RWR[1] = 0.0f;
    car_data->RWR[2] = 0.0f;
    car_data->mph = 0.0f;
    car_data->rpm = 0;
    car_data->data_valid = 0;
    car_data->place = 0;
    car_data->laps = 0;
    car_data->checkpoint = 0;
    car_data->difficulty = 2;

    // Call another function with CarData as argument
    func_800B04D0(car_data);

    // Set state flags
    *state_flags_new = *state_flags;

    // Set a value in memory
    pad_entry->dr_pos[2] = 1.0f;
}



// === func_800BB9BC @ 0x800BB9BC ===

void func_800BB9BC(u32 arg0, CarData* car_array) {
    s8 state = (gstate & 0x1000000) ? 1 : 0;
    if (state == 0) {
        u32 arg2 = *(u32*)(car_array + 4);
        func_80097798(arg0 + 101, 0, arg2);
        (*(u8*)(0x801461D0 + 14864)) = 1;
    } else {
        u32 state_value = *(u32*)(0x801461D0 + 14864);
        func_80096288(state_value, 0, 0);
        if (state_value) {
            u8* flags_ptr = (u8*)(0x80152818 + state_value * sizeof(CarData));
            *flags_ptr |= 0x1;
        }
    }
}


