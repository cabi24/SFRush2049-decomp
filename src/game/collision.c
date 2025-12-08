/**
 * Rush 2049 - Collision Functions
 * Collision detection and response
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


