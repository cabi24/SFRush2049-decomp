/**
 * Rush 2049 - Checkpoint Functions
 * Checkpoint detection, lap counting
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


// === func_800C00E0 @ 0x800C00E0 ===

#include <stdint.h>
#include <float.h>

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

#define GSTATE_INIT 0x40000
#define GSTATE_SETUP 0x80000
#define GSTATE_MENU 0x100000
#define GSTATE_CAMERA 0x200000
#define GSTATE_READY 0x400000
#define GSTATE_PLAY 0x800000
#define GSTATE_CARS 0x1000000
#define GSTATE_FINISH 0x2000000

extern uint8_t gstate;
extern uint8_t gstate_prev;
extern CarData car_array[8];
extern int32_t frame_counter;
extern struct PadEntry pad_array[4];

void func_800C00E0() {
    \n"
        "lw $v0, 108(%1)\n"
        "li $t2, 68\n"
        "addu $s0, %1, 20\n"
        "lh $a0, 12($v0)\n"
        "lw $s2, 0($v0)\n"
        "li $a2, 1\n"
        "jalr $t6\n"
        "lw $t1, 28($s2)\n"
        "or $s3, $v0, $zero\n"
        "lw $t7, 64($t0)\n"
        "li $a1, 36\n"
        "andi $t8, $t7, 0x0020\n"
        "bne $t8, $zero, 0x800C0164\n"
        "lhu $t9, 14($v0)\n"
        "ori $t6, $t9, 0x0008\n"
        "andi $t6, $t9, 0x0008\n"
        "lw $f2, 4($s3)\n"
        "lwc1 $f0, 56($t0)\n"
        "lw $f4, 4($v0)\n"
        "beq $zero, $zero, 0x800C0154\n"
        "lwc1 $f2, 4($s3)\n"
        "lwc1 $f0, 56($t0)\n"
        "lwc1 $f2, 4($s3)\n"
        "lwc1 $f0, 56($t0)\n"
        "lwc1 $f2, 4($s3)\n"
        "jalr $t6\n"
        "lw $t7, 16($s2)\n"
        "lh $t7, 20($s2)\n"
        "addiu $v1, $a0, 1\n"
        "slt $at, $v1, $t7\n"
    );
}



// === func_800C02A0 @ 0x800C02A0 ===

void func_800C02A0(s32 arg0, s32 arg1) {
    s32 sp = -32;
    CarData* car = (CarData*)arg1;
    u8 result = 0;
    u16 temp1, temp2, temp3;
    f32 temp4;

    gstate = *(u8*)(car + 9708);

    if (!arg3) {
        return;
    }

    car_array = (CarData*)0x80152818;
    game_struct = (GameStruct*)0x801461D0;

    temp1 = *(u16*)(car + 22);
    temp2 = *(u16*)(car + 6);
    temp3 = *(u16*)(car + 2);

    result++;
    temp4 = *(f32*)(car_array + (temp2 & 0x7C00) * 8);
    car->place = (s8)((temp2 >> 10) & 0x3F);
    car->laps = (s8)(temp5 >> 5);
    car->checkpoint = (s8)(temp9 << 5);

    *(f32*)(sp + 0) = temp4;
}



// === func_800C04CC @ 0x800C04CC ===

void func_800C04CC(u32 arg0) {
    s32 i;
    u16 lap_count;
    f32 lap_time;
    CarData *car;

    \n");

    car = &car_array[arg0];
    lap_count = (u16)game_struct->lap_count;
    lap_time = car->RWV[0];

    if (car->place != 0 || (gstate & GSTATE_SETUP)) {
        \n"
                          "addiu $sp, $sp, 128\n");
        return;
    }

    car->lap_time = lap_time;

    if ((car->data_valid & 0x0002) != 0 && (gstate & GSTATE_SETUP) == 0) {
        \n"
                          "addiu $sp, $sp, 128\n");
        return;
    }

    if ((car->data_valid & 0x0080) != 0 && (gstate & GSTATE_SETUP) == 0) {
        \n"
                          "addiu $sp, $sp, 128\n");
        return;
    }

    for (i = 0; i < lap_count; i++) {
        if (car->dr_uvs[0][i] == 0.0f) {
            \n"
                              "addiu $sp, $sp, 128\n");
            return;
        }
    }

    car->RWV[1] = lap_time;

    \n"
                      "addiu $sp, $sp, 128\n");
}



// === func_800C085C @ 0x800C085C ===

void func_800C085C(u16 arg0) {
    s32 sp = -24;
    CarData* car = &car_array[arg0];
    GameStruct* game_struct = (GameStruct*)gstate;
    
    u16 state_flags = gstate & 0x00FFFFFF;
    if ((state_flags & GSTATE_PLAY) == GSTATE_PLAY) {
        // Check if the car's data is valid
        if (car->data_valid != 0) {
            s32 lap_count = game_struct->lap_count;
            if (arg0 < lap_count) {
                // Update lap count
                car->laps = arg0;
                
                // Update checkpoint
                car->checkpoint = lap_count - arg0;
                
                // Save data to global struct
                game_struct->data1 = arg0;
                game_struct->data2 = lap_count;
            }
        }
    }
}



// === func_800C0AC0 @ 0x800C0AC0 ===

void func_800C0AC0(u16 arg0) {
    u32 sp = -248;
    u32 t6 = (u32)arg0 << 16;
    u16 t7 = t6 >> 16;
    void (*ra)(void);
    u32 fp;

    __asm volatile("sw $ra, %0" : "=m"(sp + 76));
    __asm volatile("sw $fp, %0" : "=m"(sp + 72));
    __asm volatile("sw $s7, %0" : "=m"(sp + 68));
    __asm volatile("sw $s6, %0" : "=m"(sp + 64));
    __asm volatile("sw $s5, %0" : "=m"(sp + 60));
    __asm volatile("sw $s4, %0" : "=m"(sp + 56));
    __asm volatile("sw $s3, %0" : "=m"(sp + 52));
    __asm volatile("sw $s2, %0" : "=m"(sp + 48));
    __asm volatile("sw $s1, %0" : "=m"(sp + 44));
    __asm volatile("sw $s0, %0" : "=m"(sp + 40));
    __asm volatile("sdc1 $f22, %0" : "=m"(sp + 32));
    __asm volatile("sdc1 $f20, %0" : "=m"(sp + 24));
    __asm volatile("sw %0, %1" : "=r"(ra) : "m"(sp + 248), "r"(arg0));
    __asm volatile("sw %0, %1" : "=r"(sp + 252) : "r"(t7));

    if (t7 != 0) {
        __asm volatile("sb $zero, %0" : "=m"(sp + 207));
        ra = (void (*)(void))0x8009079C;
        ra();
        __asm volatile("addiu $a1, $zero, 1");
    }

    if ((*(volatile u32 *)0x801146EC & 0x0008) != 0) {
        __asm volatile("li $v0, 0x8011");
        __asm volatile("lw %0, %1" : "=r"(fp) : "m"(*(volatile u32 *)0x801174B4));
        __asm volatile("lui $at, 0x007C");
        __asm volatile("lui $t6, 0x8011");
        __asm volatile("and %0, $v0, $at" : "=r"(fp) : "r"(fp));
        if (fp != 0) {
            __asm volatile("andi %0, $v0, 0x0008" : "=r"(sp + 207));
            if (sp + 207 != 0) {
                __asm volatile("lw %0, %1" : "=r"(fp) : "m"(*(volatile u32 *)0x801190FC));
                __asm volatile("lw %0, %1" : "=r"(sp + 248) : "r"(arg0));
                __asm volatile("lui $t6, 0x8011");
                __asm volatile("addiu $t7, %0, 12" : "=r"(fp) : "r"(fp));
                __asm volatile("sw %0, %1" : "=m"(sp + 244), "r"(fp));
                __asm volatile("lw %0, %1" : "=r"(fp) : "m"(*(volatile u32 *)0x8015A06C));
                __asm volatile("sw $fp, %0" : "=m"(sp + 240), "r"(fp));
            }
        }
    }

    __asm volatile("lw $ra, %0" : "=r"(ra) : "m"(sp + 76));
    __asm volatile("lw $fp, %0" : "=r"(fp) : "m"(sp + 72));
}


