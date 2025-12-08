/**
 * Rush 2049 - Camera Functions
 * Camera system, chase/cockpit views
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


// === func_800A04C4 @ 0x800A04C4 ===

void func_800A04C4(void) {
    \n"
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
        "beq     $zero, $zero, 0x800A06D8\n"
        "nop\n"
        "lw      $v0, 0($v1)\n"
        "lui     $t6, 0xDB02\n"
        "addiu   $t7, $zero, 24\n"
        "addiu   $t9, $v0, 8\n"
        "sw      $t9, 0($v1)\n"
        "sw      $t7, 4($v0)\n"
        "sw      $t6, 0($v0)\n"
        "lw      $v0, 0($v1)



// === func_800A0BAC @ 0x800A0BAC ===

void func_800A0BAC(void) {
    int state = 1;

    unsigned char t6 = *(unsigned char*)(__sp + 21);
    unsigned char t7 = t6 & 0x0001;

    if (t7 != 0) {
        int a0 = 1;
        }

    unsigned char a0 = *(unsigned char*)(__sp + 20);
    if (a0 == 255) {
        }
    int a3 = 0;
    a3 = 0;
    unsigned short v1 = *(unsigned short*)(__s5 + 22);
    unsigned int a0_ = *(unsigned int*)(v1 + 20);
    unsigned char a1 = *(unsigned char*)(__s5 + 21);
    unsigned char a2 = *(unsigned char*)(__s5 + 20);

    if (a0 != 0) {
        unsigned short a0_ = *(unsigned short*)(__fp + 28);
        v1 = *(unsigned short*)(__s5 + 22);
        unsigned int t7 = 0x8014;
        a3 = 0;

        unsigned char t9 = *(unsigned char*)(__fp + 8);
        unsigned int t6 = t9 >> 10;
        unsigned int t7 = t6 << 3;
        t7 += 0x8014ED0B;
        v1 <<= 2;
        v1 -= v1;
        v1 <<= 3;

        unsigned int v0 = *(unsigned int*)t7 + v1;
        a2 = *(unsigned char*)(__s5 + 20);

        if (a0 != 0) {
            unsigned short a0_ = *(unsigned short*)(__fp + 28);
            v1 = *(unsigned short*)(__s5 + 22);
            unsigned int t7 = 0x8014;
            a3 = 0;

            unsigned char t9 = *(unsigned char*)(__fp + 8);
            unsigned int t6 = t9 >> 10;
            unsigned int t7 = t6 << 3;
            t7 += 0x800ED0B;
            v1 <<= 2;
            v1 -= v1;
            v1 <<= 3;

            unsigned int v0 = *(unsigned int*)t7 + v1;
            a2 = *(unsigned char*)(__s5 + 20);
        }
    }
}



// === func_800A0F74 @ 0x800A0F74 ===


void func_800A0F74(u32 t6) {
    if (t6 != 0) {
        u32 s0 = car_array;
        ((u32 (*)())func_80007270)(s0, 1);
        ((u32 (*)())func_80095FD8)(s0 + 0x1A400, 0);
        ((u32 (*)())func_800075E0)(s0, 0);
    }
}

void func_800A0FDC(u32 a0, u32 a1) {
    u32 t0 = a0;
    u32 t2 = a1;

    ((u32 (*)())func_80007270)(car_array, 1);
    ((u32 (*)())func_80095F8C)((u32)gstate + 8, t0);

    if (gstate != 0 && gstate != t0) {
        u32 t6 = gstate & -32;
        if (((s1 = *(u32 *)t6) != 0) && s1 < gstate) {
            u32 v1 = *(u32 *)(t6 + 4);
            if (v1 < gstate) {
                u32 t7 = t0 + t6;
                u32 t9 = a3 + 12;
                if (t9 < t7) {
                    u32 t8 = *(u32 *)(a3 + 4);
                    if (t8 != 0) {
                        s1 = t0 - 32;
                        t9 = 1;
                        if (t8 != 0 && s1 < gstate) {
                            *(u32 *)s1 = t8;
                        }
                    }
                }
            }
        }
    }
}



// === func_800A0FDC @ 0x800A0FDC ===


void func_800A0FDC(u32 a0, u32 a1) {

    s32 sp = -72;

    CarData* car = &car_array[a1];

    void* gstate = (void*)0x801146EC;

    void* game_struct = (void*)0x801461D0;

    u32 t2 = a1 + 31;

    s32 at = -32 & t2;

    if (car) {

        while (at != 0) {

            CarData* next_car = &car_array[a0 + *car];

            car[1] = (u32)(next_car + 32);

            if ((u32)(next_car + 32) <= a0 + *car) {

                memset(next_car, 0, 32);

                car[1] = (u32)(next_car + 32);

            } else {

                car[1] = 0;

            }

            car += 4;

            at -= 4;

        }

    }

}


