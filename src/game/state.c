/**
 * Rush 2049 - Game_State Functions
 * Game state machine, attract, playgame, countdown
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


// === func_800CA300 @ 0x800CA300 ===

#include <stdint.h>

void func_800CA300(uint32_t arg) {
    uint32_t* gstate = (uint32_t*)gstate;
    CarData* car_array = (CarData*)car_array;
    GameStruct* game_struct = (GameStruct*)game_struct;

    if ((arg & GSTATE_INIT) != 0) {
        func_800A4AC4(57);
    }

    if ((arg & 0x100) != 0) {
        uint32_t t8 = arg & 0x80;
        if (t8 != 0) {
            func_800A4AC4(60);
            func_800A5BB8();
        } else {
            func_800A4AC4(56);
        }
    }

    if ((arg & 0x80) != 0) {
        uint32_t t9 = arg << 2;
        func_800A4AC4(t9);
        if (t9 >= 0) {
            func_800C9334(54);
            func_800C9334(58);
            func_800C9334(59);
        }
    }

    uint32_t t6 = *gstate;
    if (t6 != 0) {
        uint32_t t7 = *(uint32_t*)gstate + 4;
        if (t6 == t7) {
            func_800CAD7C();
        } else {
            func_800A4AC4(55);
            func_800C90E0();
        }
    }

    uint32_t t10 = *gstate;
    if (t10 != 0) {
        uint32_t t11 = *(uint32_t*)gstate + 4;
        if (t10 == t11) {
            func_800CAD7C();
        } else {
            func_800A4AC4(55);
            func_800C90E0();
        }
    }

    uint32_t t12 = *gstate;
    if (t12 != 0) {
        uint32_t t13 = *(uint32_t*)gstate + 4;
        if (t12 == t13) {
            func_800CAD7C();
        } else {
            func_800A4AC4(55);
            func_800C90E0();
        }
    }
}



// === func_800CA308 @ 0x800CA308 ===

void func_800CA308(u16 state_flags) {
    if (state_flags & GSTATE_INIT) {
        func_800A4AC4(57);
    }

    if (state_flags & 0x0100) {
        func_800A4AC4(56);
        func_800A5BB8();
    }

    if (state_flags & 0x0080) {
        u32 index = state_flags << 2;
        func_800A4AC4(60);

        if (index >= 0x1000000) {
            func_800A4AC4(55);
            func_800C9334(54);
            func_800C9334(58);
            func_800C9334(59);
        }
    }
}

void func_800CA3B4(void) {
    u8* gstate = (u8*)gstate;
    CarData* car_array = (CarData*)car_array;
    u32 game_struct = 0x801461D0;

    __asm volatile (
        "addiu $sp, $sp, -232\n"
        "sw $s0, 56($sp)\n"
        "lui $s0, 0x8011\n"
        "addiu $s0, $s0, 29876\n"
        "lui $t7, 0x8011\n"
        "lw $t7, 29880($t7)\n"
        "lw $t6, 0($s0)\n"
        "sw $ra, 76($sp)\n"
        "sw $s4, 72($sp)\n"
        "sw $s3, 68($sp)\n"
        "sw $s2, 64($sp)\n"
        "sw $s1, 60($sp)\n"
        "sdc1 $f22, 48($sp)\n"
        "beq $t6, $t7, 0x800CAD7C\n"
        "sdc1 $f20, 40($sp)\n"
        "jal 0x800CA300\n"
        "nop\n"
        "lui $t8, 0x8011\n"
        "lw $v1, 29880($t8)\n"
        "lui $at, 0x8014\n"
        "andi $t9, $v1, 0x0001\n"
        "beq $t9, $zero, 0x800CA4F4\n"
        "sw $v1, 0($s0)\n"
        "sb $zero, 25080($at)\n"
        "sb $zero, 25093($at)\n"
        "lui $at, 0x8015\n"
        "sb $zero, -27628($at)\n"
        "sb $zero, -26764($at)\n"
        "sb $zero, -26732($at)\n"
        "sb $zero, -26684($at)\n"
        "jal 0x800A5BB8\n"
        "nop\n"
    );
}



// === func_800CA3B4 @ 0x800CA3B4 ===

void func_800CA3B4(void) {
    char* sp = (char*)__builtin_alloca(232);

    \n\t"
        "lui $s0, 0x8011\n\t"
        "addiu $s0, $s0, 29876\n\t"
        "lui $t7, 0x8011\n\t"
        "lw $t7, 29880($t7)\n\t"
        "lw $t6, 0($s0)\n\t"
        "sw $ra, 76($sp)\n\t"
        "sw $s4, 72($sp)\n\t"
        "sw $s3, 68($sp)\n\t"
        "sw $s2, 64($sp)\n\t"
        "sw $s1, 60($sp)\n\t"
        "sdc1 $f22, 48($sp)\n\t"
        "beq $t6, $t7, 0x800CAD7C\n\t"
        "sdc1 $f20, 40($sp)\n\t"
        "jal 0x800CA300\n\t"
        "nop\n\t"
        "lui $t8, 0x8011\n\t"
        "lw $v1, 29880($t8)\n\t"
        "lui $at, 0x8014\n\t"
        "andi $t9, $v1, 0x0001\n\t"
        "beq $t9, $zero, 0x800CA4F4\n\t"
        "sw $v1, 0($s0)\n\t"
        "sb $zero, 25080($at)\n\t"
        "li $at, 0x8014\n\t"
        "sb $zero, 25093($at)\n\t"
        "li $at, 0x8015\n\t"
        "sb $zero, -27628($at)\n\t"
        "li $at, 0x8015\n\t"
        "sb $zero, -26764($at)\n\t"
        "li $at, 0x8015\n\t"
        "sb $zero, -26732($at)\n\t"
        "li $at, 0x8015\n\t"
        "jal 0x800A5BB8\n\t"
        "sb $zero, -26684($at)\n\t"
        "lui $t6, 0x8015\n\t"
        "lb $t6, 27028($t6)\n\t"
        "addiu $a0, $zero, 32\n\t"
        "addiu $a1, $zero, 16\n\t"
        "bne $t6, $zero, 0x800CA48C\n\t"
        "addiu $a2, $zero, 16\n\t"
        "lui $at, 0x3F80\n\t"
        "i-type op=0x11\n\t"
        "lui $at, 0x4348\n\t"
        "i-type op=0x11\n\t"
        "addiu $t7, $zero, 64\n\t"
        "sw $t7, 16($sp)\n\t"
        "addiu $a3, $zero, 5\n\t"
        "sw $zero, 28($sp)\n\t"
        "swc1 $f4, 20($sp)\n\t"
        "jal 0x800C8B8C\n\t"
        "swc1 $f6, 24($sp)\n\t"
        "jal 0x800C885C\n\t"
        "jal 0x800C8FA4\n\t"
        "addiu $a0, $zero, 1\n\t"
        "move $v0, $zero\n\t"
        "lui $at, 0x8015\n\t"
    );
}



// === func_800CADA4 @ 0x800CADA4 ===

void func_800CADA4() {
    \n"
        "lui     $s1, 0x8014\n"
        "lw      $s1, -3600($s1)\n"
        "sw      $ra, 92($sp)\n"
        "sw      $fp, 88($sp)\n"
        "sw      $s7, 84($sp)\n"
        "sw      $s6, 80($sp)\n"
        "sw      $s5, 76($sp)\n"
        "sw      $s4, 72($sp)\n"
        "sw      $s3, 68($sp)\n"
        "sw      $s2, 64($sp)\n"
        "sw      $s0, 56($sp)\n"
        "sdc1    $f26, 48($sp)\n"
        "sdc1    $f24, 40($sp)\n"
        "sdc1    $f22, 32($sp)\n"
        "beq     $s1, $zero, 0x800CB6D8\n"
        "sdc1    $f20, 24($sp)\n"
        "lui     $at, 0x4700\n"
        "i-type  op=0x11\n"
        "lui     $at, 0x8012\n"
        "lwc1    $f24, 16320($at)\n"
        "lui     $at, 0x8012\n"
        "lui     $fp, 0x8011\n"
        "lui     $s6, 0x8013\n"
        "addiu   $s6, $s6, -6400\n"
        "addiu   $fp, $fp, 29532\n"
        "lwc1    $f22, 16324($at)\n"
        "addiu   $s7, $zero, 68\n"
        "addiu   $s0, $zero, 3\n"
        "lw      $t6, 0($s1)\n"
        "lui     $t2, 0x8015\n"
        "addiu   $t2, $t2, 10264\n"
        "sw      $t6, 256($sp)\n"
        "lw      $v0, 80($s1)\n"
        "lh      $v1, 84($s1)\n"
        "addiu   $at, $zero, 5\n"
        "srl     $s5, $v0, 17\n"
        "andi    $t7, $s5, 0x000F\n"
        "sll     $t8, $t7, 16\n"
        "andi    $t0, $v0, 0x0010\n"
        "beq     $t0, $zero, 0x800CB1A8\n"
        "sra     $s5, $t8, 16\n"
        "sll     $t1, $v1, 4\n"
        "subu    $t1, $t1, $v1\n"
        "sll     $t1, $t1, 3\n"
        "subu    $t1, $t1, $v1\n"
        "sll     $t1, $t1, 3\n"
        "addu    $s2, $t1, $t2\n"
        "lh      $s4, 248($s2)\n"
        "lb      $t9, 861($s2)\n"
        "addiu   $t1, $zero, 1\n"
        "sra     $t3, $s4, 2\n"
        "sll     $t4, $t3, 16\n"
        "sra     $s4, $t4, 16\n"
    );
}



// === func_800DB1E0 @ 0x800DB1E0 ===

void func_800DB1E0() {
    \n"
        "lui     $at, 0x7C03\n"
        "ori     $at, $at, 0xFFFE\n"
        "and     $t7, $t6, $at\n"
        "sw      $ra, 108($sp)\n"
        "sw      $fp, 104($sp)\n"
        "sw      $s7, 100($sp)\n"
        "sw      $s6, 96($sp)\n"
        "sw      $s5, 92($sp)\n"
        "sw      $s4, 88($sp)\n"
        "sw      $s3, 84($sp)\n"
        "sw      $s2, 80($sp)\n"
        "sw      $s1, 76($sp)\n"
        "sw      $s0, 72($sp)\n"
        "sdc1    $f30, 64($sp)\n"
        "sdc1    $f28, 56($sp)\n"
        "sdc1    $f26, 48($sp)\n"
        "sdc1    $f24, 40($sp)\n"
        "sdc1    $f22, 32($sp)\n"
        "beq     $t7, $zero, 0x800DB250\n"
        "sdc1    $f20, 24($sp)\n"
        "lui     $a3, 0x8015\n"
        "lui     $a0, 0x8015\n"
        "lw      $a3, 26956($a3)\n"
        "beq     $zero, $zero, 0x800DB274\n"
        "lw      $a0, -26748($a0)\n"
        "lui     $v0, 0x8015\n"
        "lw      $v0, 27020($v0)\n"
        "lui     $a3, 0x8015\n"
        "lui     $a0, 0x8014\n"
        "sll     $t8, $v0, 2\n"
        "addu    $a3, $a3, $t8\n"
        "addu    $a0, $a0, $t8\n"
        "lw      $a3, 27032($a3)\n"
        "lw      $a0, 14848($a0)\n"
        "lui     $t9, 0x8011\n"
        "lb      $t9, 28052($t9)\n"
        "i-type  op=0x15\n"
        "andi    $t6, $a3, 0x0005\n"
        "sw      $a0, 116($sp)\n"
        "jal     0x800DABDC\n"
        "sw      $a3, 120($sp)\n"
        "lw      $a0, 116($sp)\n"
        "lw      $a3, 120($sp)\n"
        "andi    $t6, $a3, 0x0005\n"
        "beq     $t6, $zero, 0x800DB2C4\n"
        "andi    $t8, $a0, 0x0400\n"
        "or      $a0, $a3, $zero\n"
        "jal     0x800B5FC4\n"
        "sw      $a3, 120($sp)\n"
        "addiu   $t7, $zero, 2\n"
        "lui     $at, 0x8011\n"
        "lw      $v0, gstate_prev\n"
        "andi    $t6, $a3, 0x0005\n"
        "sw      $a0, 116($sp)\n"
        "jal     0x800DABDC\n"
        "sw      $a3, 120($sp)\n"
        "lw      $a0, 116($sp)\n"
        "lw      $a3, 120($sp)\n"
        "andi    $t6, $a3, 0x0005\n"
        "beq     $t6, $zero, 0x800DB2C4\n"
        "andi    $t8, $a0, 0x0400\n"
        "or      $a0, $a3, $zero\n"
        "jal     0x800B5FC4\n"
        "sw      $a3, 120($sp)\n"
        "addiu   $t7, $zero, 2\n"
        "lui     $at, 0x8011\n"
        "lw      $v0, gstate_prev\n"
        "andi    $t6, $a3, 0x0005\n"
        "sw      $a0, 116($sp)\n"
        "jal     0x800DABDC\n"
        "sw      $a3, 120($sp)\n"
        "lw      $a0, 116($sp)\n"
        "lw      $a3, 120($sp)\n"
        "andi    $t6, $a3, 0x0005\n"
        "beq     $t6, $zero, 0x800DB2C4\n"
        "andi    $t8, $a0, 0x0400\n"
        "or      $a0, $a3, $zero\n"
        "jal     0x800B5FC4\n"
        "sw      $a3, 120($sp)\n"
        "addiu   $t7, $zero, 2\n"
        "lui     $at, 0x8011\n"
        "lw      $v0, gstate_prev\n"
        "andi    $t6, $a3, 0x0005\n"
        "sw      $a0, 116($sp)\n"
        "jal     0x800DABDC\n"
        "sw      $a3, 120($sp)\n"
        "lw      $a0, 116($sp)\n"
        "lw      $a3, 120($sp)\n"
        "andi    $t6, $a3, 0x0005\n"
        "beq     $t6, $zero, 0x800DB2C4\n"
        "andi    $t8, $a0, 0x0400\n"
        "or      $a0, $a3, $zero\n"
        "jal     0x800B5FC4\n"
        "sw      $a3, 120($sp)\n"
        "addiu   $t7, $zero, 2\n"
        "lui     $at, 0x8011\n"
        "lw      $v0, gstate_prev\n"
        "andi    $t6, $a3, 0x0005\n"
        "sw      $a0, 116($sp)\n"
        "jal     0x800DABDC\n"
        "sw      $a3, 120($sp)\n"
        "lw      $a0, 116($sp)\n"
        "lw      $a3, 120($sp)\n"
        "andi    $t6, $a3, 0x0005\n"
        "beq     $t6, $zero, 0x800DB2C4\n"
        "andi    $t8, $a0, 0x0400\n"
        "or      $a0, $a3, $zero\n"
        "jal     0x800B5FC4\n"
        "sw      $a3, 120($sp)\n"
        "addiu   $t7, $zero, 2\n"
        "lui     $at, 0x8011\n"
        "lw      $v0, gstate_prev\n"
        "andi    $t6, $a3, 0x0005\n"
        "sw      $a0, 116($sp)\n"
        "jal     0x800DABDC\n"
        "sw      $a3, 120($sp)\n"
        "lw      $a0, 116($sp)\n"
        "lw      $a3, 120($sp)\n"
        "andi    $t6, $a3, 0x0005\n"
        "beq     $t6, $zero, 0x800DB2C4\n"
        "andi    $t8, $a0, 0x0400\n"
        "or      $a0, $a3, $zero\n"
        "jal     0x800B5FC4\n"
        "sw      $a3, 120($sp)\n"
        "addiu   $t7, $zero, 2\n"
        "lui     $at, 0x8011\n"
        "lw      $v0, gstate_prev\n"
        "andi    $t6, $a3, 0x0005\n"
        "sw      $a0, 116($sp)\n"
        "jal     0x800DABDC\n"
        "sw      $a3, 120($sp)\n"
        "lw      $a0, 116($sp)\n"
        "lw      $a3, 120($sp)\n"
        "andi    $t6, $a3, 0x0005\n"
        "beq     $t6, $zero, 0x800DB2C4\n"
        "andi    $t8, $a0, 0x0400\n"
        "or      $a0, $a3, $zero\n"
        "jal     0x800B5FC4\n"
        "sw      $a3, 120($sp)\n"
        "addiu   $t7, $zero, 2\n"
        "lui     $at, 0x8011\n"
        "lw      $v0, gstate_prev\n"
        "andi    $t6, $a3, 0x0005\n"
        "sw      $a0, 116($sp)\n"
        "jal     0x800DABDC\n"
        "sw      $a3, 120($sp)\n"
        "lw      $a0, 116($sp)\n"
        "lw      $a3, 120($sp)\n"
        "andi    $t6, $a3, 0x0005\n"
        "beq     $t6, $zero, 0x800DB2C4\n"
        "andi    $t8, $a0, 0x0400\n"
        "or      $a0, $a3, $zero\n"
        "jal     0x800B5FC4\n"
        "sw      $a3, 120($sp)\n"
        "addiu   $t7, $zero, 2\n"
        "lui     $at, 0x8011\n"
        "lw      $v0, gstate_prev\n"
        "andi    $t6, $a3, 0x0005\n"
        "sw      $a0, 116($sp)\n"
        "jal     0x800DABDC\n"
        "sw      $a3, 120($sp)\n"
        "lw      $a0, 116($sp)\n"
        "lw      $a3, 120($sp)\n"
        "andi    $t6, $a3, 0x0005\n"
        "beq     $t6, $zero, 0x800DB2C4\n"
        "andi    $t8, $a0, 0x0400\n"
        "or      $a0, $a3, $zero\n"
        "jal     0x800B5FC4\n"
        "sw      $a3, 120($sp)\n"
        "addiu   $t7, $zero, 2\n"
        "lui     $at, 0x8011\n"
        "lw      $v0, gstate_prev\n"
        "andi    $t6, $a3, 0x0005\n"
        "sw      $a0, 116($sp)\n"
        "jal     0x800DABDC\n"
        "sw      $a3, 120($sp)\n"
        "lw      $a0, 116($sp)\n"
        "lw      $a3, 120($sp)\n"
        "andi    $t6, $a3, 0x0005\n"
        "beq     $t6, $zero, 0x800DB2C4\n"
        "andi    $t8, $a0, 0x0400\n"
        "or      $a0, $a3, $zero\n"
        "jal     0x800B5FC4\n"
        "sw      $a3, 120($sp)\n"
        "addiu   $t7, $zero, 2\n"
        "lui     $at, 0x8011\n"
        "lw      $v0, gstate_prev\n"
        "andi    $t6, $a3, 0x0005\n"
        "sw      $a0, 116($sp)\n"


// === func_800DB758 @ 0x800DB758 ===

void func_800DB758(CarData *car) {
    \n"
        "sw      $ra, 52($sp)\n"
        "sw      $s2, 44($sp)\n"
        "sw      $s1, 40($sp)\n"
        "sdc1    $f22, 24($sp)\n"
        "nop\n"
        "j       func_800C9210\n"
        "nop\n"
        "move    $s2, $zero\n"
        "lw      $ra, 52($sp)\n"
        "ldc1    $f20, 16($sp)\n"
        "ldc1    $f22, 24($sp)\n"
        "lw      $s0, 36($sp)\n"
        "lw      $s1, 40($sp)\n"
        "lw      $s2, 44($sp)\n"
        "lw      $s3, 48($sp)\n"
        "jr      $ra\n"
        "addiu   $sp, $sp, 56\n"
    );
}

void func_800DB7B4() {
    \n"
        "lui     $a0, 0x8014\n"
        "addiu   $a0, $a0, 10024\n"
        "move    $a1, $zero\n"
        "jal     func_80007270\n"
        "nop\n"
        "addiu   $a2, $zero, 1\n"
        "jal     func_80091B00\n"
        "sw      $v0, 28($sp)\n"
        "addiu   $t6, $zero, 7\n"
        "sb      $t6, 2($v0)\n"
        "lui     $a0, 0x8014\n"
        "addiu   $a0, $a0, 10024\n"
        "move    $a1, $zero\n"
        "jal     func_800075E0\n"
        "move    $a2, $zero\n"
        "lui     $a0, 0x8014\n"
        "addiu   $a0, $a0, 10152\n"
        "lw      $a1, 28($sp)\n"
        "jal     func_800075E0\n"
        "move    $a2, $zero\n"
        "lw      $ra, 20($sp)\n"
        "addiu   $sp, $sp, 32\n"
        "jr      $ra\n"
    );
}

void func_800DB81C() {
    \n"
        "sw      $ra, 60($sp)\n"
        "sw      $s5, 56($sp)\n"
        "sw      $s4, 52($sp)\n"
        "sw      $s3, 48($sp)\n"
        "sw      $s2, 44($sp)\n"
        "sw      $s1, 40($sp)\n"
        "sw      $s0, 36($sp)\n"
        "sdc1    $f



// === func_800DB7B4 @ 0x800DB7B4 ===

void func_800DB7B4() {
    \n"
        "lui     $a0, 0x8014\n"
        "addiu   $a0, $a0, 10024\n"
        "move    $a1, $zero\n"
        "jal     0x80007270\n"
        "addiu   $a2, $zero, 1\n"
        "jal     0x80091B00\n"
        "nop\n"
        "sw      $v0, 28($sp)\n"
        "addiu   $t6, $zero, 7\n"
        "lui     $a0, 0x8014\n"
        "sb      $t6, 2($v0)\n"
        "addiu   $a0, $a0, 10024\n"
        "move    $a1, $zero\n"
        "jal     0x800075E0\n"
        "move    $a2, $zero\n"
        "lui     $a0, 0x8014\n"
        "addiu   $a0, $a0, 10152\n"
        "lw      $a1, 28($sp)\n"
        "jal     0x800075E0\n"
        "move    $a2, $zero\n"
        "lw      $ra, 20($sp)\n"
        "addiu   $sp, $sp, 32\n"
        "jr      $ra\n"
        "nop\n"
    );
}

void func_800DB81C() {
    \n"
        "sw      $ra, 60($sp)\n"
        "sw      $s5, 56($sp)\n"
        "sw      $s4, 52($sp)\n"
        "sw      $s3, 48($sp)\n"
        "sw      $s2, 44($sp)\n"
        "sw      $s1, 40($sp)\n"
        "sw      $s0, 36($sp)\n"
        "sdc1    $f22, 24($sp)\n"
        "beq     $v1, $zero, 0x800DB864\n"
        "sdc1    $f20, 16($sp)\n"
        "move    $a0, $zero\n"
        "jal     0x800C813C\n"
        "move    $a1, $zero\n"
        "lui     $v1, 0x8011\n"
        "lw      $v1, 28924($v1)\n"
        "sltiu   $at, $v1, 9\n"
        "beq     $at, $zero, 0x800DBF68\n"
        "addiu   $t0, $zero, 2\n"
        "sll     $t6, $v1, 2\n"
        "lui     $at, 0x8012\n"
        "addu    $at, $at, $t6\n"
        "lw      $t6, 17028($at)\n"
        "jr      $t6\n"
        "nop\n"
        "lui     $a1, 0x8015\n"
        "lh      $a1, 6864($a1)\n"
        "lui     $a0, 0x8015\n"
    );
}



// === func_800DB81C @ 0x800DB81C ===


void func_800DB81C() {
    u32* sp = (u32*)malloc(272);

    u32 gstate_value = gstate;
    u32 car_array_value = car_array;
    u32 game_struct_value = game_struct;

    u32 ra = *(sp + 15);
    u32 s5 = *(sp + 14);
    u32 s4 = *(sp + 13);
    u32 s3 = *(sp + 12);
    u32 s2 = *(sp + 11);
    u32 s1 = *(sp + 10);
    u32 s0 = *(sp + 9);
    f32 f22 = *(f32*)(sp + 8);

    if (gstate_value) {
        f32 f20 = *(f32*)(sp + 7);
        func_800C813C(0, 0);

        if (*(u32*)gstate_value < 9) {
            u32 t6 = (*(u32*)gstate_value << 2);
            void* at = (void*)((t6 >> 16) + 0x80124284);
            return ((void (*)(void))at)();
        }
    }

    s16 a1 = *(s16*)(car_array_value + 6864);
    u32 a0 = car_array_value - 24296;
    if (a1 > 0) {
        u32 v1 = 0;
        u32 a2 = car_array_value + 27888;
        u32 v0 = *(u32*)a0 & 0x0001;
        u32 t9 = v1 << 4;

        if (v0) {
            v0 = 1;
        } else {
            v0 = 0;
        }

        while (v0 == 0) {
            u8 v1 = *(u8*)(a2 + t9);
            u32 t7 = v1 < 1;

            if (t7 != 0) {
                v0 = t7;
            } else {
                v0 = 1;
            }

            v1++;
            if (v1 >= a1) {
                break;
            }

            a0 += 76;
        }

        *(u32*)(car_array_value + 29520) = v1;
    }

    free(sp);
}



// === func_800FB234 @ 0x800FB234 ===

void func_800FB234(u8 arg0) {
    if (arg0 != 0) {
        gstate[56] = arg0;
        
        if (some_condition()) {
            int temp = 0;
            
            ((int (*)(void))0x800C9210)();
            
            temp++;
        }
    }
}

void func_800FB2C8(u32 arg0, u32 arg1) {
    if (arg1 != 0) {
        gstate[18164] = 0;
        
        if (gstate[18164] == 0) {
            ((int (*)(void))0x800C9BE0)();
            
            gstate[18164] = 1;
        }
        
        int game_struct_value = gstate[0];
        
        if (game_struct_value & 0x0008) {
            u32 flag = ((game_struct_value >> 3) & 1) | ((game_struct_value >> 2) & 1);
            
            func_800FB234(flag);
        }
    }
}



// === func_800FB2C8 @ 0x800FB2C8 ===

void func_800FB2C8(u32 arg0, u32 arg1) {
    char *sp = (char *)__builtin_alloca(256);

    __asm volatile (
        "addiu $sp, $sp, -256\n"
        "sw $ra, 76($sp)\n"
        "sw $fp, 72($sp)\n"
        "sw $s7, 68($sp)\n"
        "sw $s6, 64($sp)\n"
        "sw $s5, 60($sp)\n"
        "sw $s4, 56($sp)\n"
        "sw $s3, 52($sp)\n"
        "sw $s2, 48($sp)\n"
        "sw $s1, 44($sp)\n"
        "sw $s0, 40($sp)\n"
        "beq $a1, $zero, 0x800FB30C\n"
        "sw $a0, 256($sp)\n"
        "lui $at, 0x8011\n"
        "sb $zero, 18164($at)\n"
        "beq $zero, $zero, 0x800FBBCC\n"
        "movz $v0, $zero\n"
        "lui $t6, 0x8011\n"
        "lb $t6, 18164($t6)\n"
        "addiu $t7, $zero, 1\n"
        "beq $t6, $zero, 0x800FB764\n"
        "jal 0x800C9BE0\n"
        "sb $t7, 18164($at)\n"
        "lui $v0, 0x8011\n"
        "lw $v0, 29876($v0)\n"
        "lui $at, 0x0200\n"
        "andi $a0, $v0, 0x0008\n"
        "sltiu $t8, $a0, 1\n"
        "beq $t8, $zero, 0x800FB354\n"
        "or $a0, $t8, $zero\n"
        "and $a0, $v0, $at\n"
        "sltiu $t9, $a0, 1\n"
        "or $a0, $t9, $zero\n"
        "jal 0x800FB234\n"
        "nop\n"
        "lui $v0, 0x8011\n"
        "lw $v0, 29876($v0)\n"
        "lui $t1, 0x8015\n"
        "lui $at, 0x42B4\n"
        "sll $t6, $v0, 6\n"
        "bgez $t6, 0x800FB654\n"
        "andi $t8, $v0, 0x0008\n"
        "lui $t2, 0x8015\n"
        "addiu $t2, $t2, 10060\n"
        "sh $zero, 0($t2)\n"
        "lh $v1, 0($t2)\n"
        "addiu $t1, $t1, 10088\n"
        "lui $t7, 0x8015\n"
        "addiu $t7, $t7, 16338\n"
        "sh $v1, 0($t1)\n"
        "lui $t8, 0x8015\n"
        "sh $v1, 0($t7)\n"
        "lh $t8, -24312($t8)\n"
        "lui $s0, 0x8015\n"
        "addiu $s0, $s0, -24296\n"
        "blez $t8, 0x800FB4A8\n"
        "move $s1, $zero\n"
        "lbu $v1, 0($s0)\n"
        "lui $t6, 0x8015\n"
        "addiu $t6, $t6, 1



// === func_800FBBFC @ 0x800FBBFC ===

void func_800FBBFC() {
    s32 sp = -24;
    u32 *sp_ptr = (u32 *)sp;

    // Save return address
    *(sp_ptr + 1) = (u32)&func_800FBC3C;

    // Call some function with no arguments
    func_8000BE50();

    // Move zero to argument register
    u32 arg0 = 0;
    // Load constant value
    u32 at = 7;

    // Call another function with argument register
    func_800C9AE0(arg0);

    // Restore return address
    *(sp_ptr + 1) = (u32)&func_800FBC3C;

    // Return to caller
    return;
}

void func_800FBC3C() {
    s32 sp = -344;
    u32 *sp_ptr = (u32 *)sp;

    // Check if global state is not 0x10002 (DRONE_HUMAN)
    u32 t6 = gstate_prev & GSTATE_CARS;
    if (t6 != DRONE_HUMAN) {
        *(sp_ptr + 5) = (u32)&func_800FBC3C;

        // Load constant value
        s32 at = 1;
        if (at <= gstate_prev & GSTATE_CARS) {
            // Set global variable
            gstate_prev &= ~GSTATE_CARS;
        }

        *(sp_ptr + 5) = (u32)&func_800FBC3C;

        // Load constant value
        s32 s0 = 1;
        if (s0 <= gstate_prev & GSTATE_CARS) {
            // Set global variable
            gstate_prev &= ~GSTATE_CARS;
        }

        *(sp_ptr + 5) = (u32)&func_800FBC3C;

        // Load constant value
        s32 a1 = 776;
        func_80004990((CarData *)gstate_prev, &a1);

        *(sp_ptr + 5) = (u32)&func_800FBC3C;

        // Load constant value
        s32 s2 = -23328;
        u32 v1 = *(s2 + 4);
        func_8018E6A4(&v1, gstate_prev);

        return;
    }

    return;
}



// === func_800FBC30 @ 0x800FBC30 ===

void func_800FBC30() {
    u8 *gstate = (u8 *)gstate_prev;

    CarData *car_array = (CarData *)car_array;
    GameStruct *game_struct = (GameStruct *)game_struct;

    s32 sp = 0x800FBC38;
    s32 at = 2;
    u8 s0 = gstate[0];
    s32 v1 = 0;
    s32 t7 = 0;
    s32 a1 = 0;
    s32 s2 = 0;
    s32 s4 = sp + 88;
    s32 s5 = 0;

    if (s0 != at) {
        a1 = &car_array[game_struct->num_cars][0].data_valid;
        v1 = 1;
    } else {
        s0 = ((u16 *)gstate)[1];
        if (s0 <= 0) {
            return;
        }
        while (--s0 >= 0) {
            t7 = car_array[game_struct->num_cars][v1].place;
            v1++;
            if (t7 == s0) {
                break;
            }
        }

        if (v1 != s0 + 1) {
            a1 = &car_array[game_struct->num_cars][v1].data_valid;
            v1 = 320;
            if (game_struct->field_18A4[0] != 0) {
                a1 = &car_array[game_struct->num_cars][v1].data_valid;
                v1 = 18168;
            }
        } else {
            a1 = &car_array[game_struct->num_cars][v1].data_valid;
            v1 = 320;
        }

        func_80004990(sp + 80, 776, v1);
        func_80004990(s4, 780, v1);

        s5 = (u16 *)game_struct->field_18A4[3];
        a1 = &car_array[game_struct->num_cars][v1].data_valid;
        t7 = car_array[game_struct->num_cars][v1].place;

        func_80004990(s4, 780, v1);
        func_800A4770(s5 + 512);

        s5 = (u16 *)gstate[0];
        a1 = &car_array[game_struct->num_cars][v1].data_valid;
        t7 = car_array[game_struct->num_cars][v1].place;

        func_80007270(s5 + 25040, 0);
        func_800B4200(11, v0, 0);

        func_800075E0(s5 + 25040, 0);
        func_800B74A0(22, (u16 *)gstate[0]);
    }
}



// === func_800FBC38 @ 0x800FBC38 ===

void func_800FBC38(int arg) {
    int *sp = (int *)malloc(344);
    int at = 2;

    if (arg != at) {
        sp[11] = ((int (*)(void))sp[56])();
        return;
    }

    at = 0x80152818;
    arg = *(short *)(at - 24312);

    if (arg <= 0)
        return;

    int v0, t7;
    do {
        v0 = 0x8015 + 27888;
        t7 = *(char *)v0++;
        arg--;
    } while (t7 != '\0');

    if (arg > 0)
        v0 += 16;

    int s1 = sp + 80, a1 = 0x8012, at = 0x8016;
    *(int *)(at + 5040) = v0;
    int s0 = 320;
    if (arg > 0)
        *(int *)(at + 18168) = s0;

    *(int *)s1 = v0;
    arg += 776;
    ((void (*)(int, int))0x80004990)(s1, arg);

    do {
        arg++;
        a1 += 4;
        *(int *)(at + 5040) = v0;
        int s4 = sp + 88;
        CarData *car_data = (*(CarData **)(arg + car_array));
        int t8 = car_data->mph;
        int a2 = car_data->rpm;
        *(int *)(s4 + 20) = s1;
        *(int *)(s4 + 16) = t8;
        *(int *)(s4 + 28) = arg;
        *(int *)(s4 + 24) = car_data->place;
        int t6 = car_data->laps;
        *(int *)(s4 + 32) = t6;
        a1 += 772;
        int t7 = car_data->checkpoint;
        ((void (*)(int, int))0x800A4770)(s4, t7);
        t7 = *(int *)(at + 29876);
        if (t7 & 1) {
            if (t7 & 2)
                break;
            s0 <<= 1;
        }
    } while (arg < 11);

    free(sp);
}



// === func_800FBF2C @ 0x800FBF2C ===

void func_800FBF2C() {
    \n"
        "jal     0x80001578\n"
        "nop\n"
        "lui     $at, 0x8012\n"
        "lwc1    $f4, 18424($at)\n"
        "lui     $v0, 0x8015\n"
        "addiu   $v0, $v0, 8242\n"
        "i-type  op=0x11\n"
        "lui     $at, 0x8011\n"
        "lui     $t9, 0x0010\n"
        "i-type  op=0x11\n"
        "i-type  op=0x11\n"
        "nop\n"
        "sh      $t7, 0($v0)\n"
        "lh      $t8, 0($v0)\n"
        "i-type  op=0x17\n"
        "lw      $ra, 20($sp)\n"
        "sw      $t9, 29880($at)\n"
        "lw      $ra, 20($sp)\n"
        "addiu   $sp, $sp, 24\n"
        "jr      $ra\n"
    );
}

void func_800FBF90() {
    \n"
        "sll     $t6, $v0, 13\n"
        "bgez    $t6, 0x800FBFD8\n"
        "addiu   $t7, $zero, 1\n"
        "lui     $at, 0x8011\n"
        "jal     0x800C9BE0\n"
        "sw      $t7, 29884($at)\n"
        "jal     0x800FAF6C\n"
        "nop\n"
        "lui     $t8, 0x8015\n"
        "lb      $t8, 3836($t8)\n"
        "lui     $at, 0x8015\n"
        "lui     $t9, 0x0008\n"
        "sb      $t8, 3860($at)\n"
        "i-type  op=0x17\n"
        "bne     $zero, $zero, 0x800FBFE4\n"
        "sw      $t9, 29880($at)\n"
        "sll     $t6, $v0, 12\n"
        "bgez    $t6, 0x800FC030\n"
        "sll     $t9, $v0, 10\n"
        "addiu   $a0, $zero, 1\n"
        "jal     0x800FB2C8\n"
        "move    $a1, $zero\n"
        "beq     $v0, $zero, 0x800FC01C\n"
        "move    $a0, $zero\n"
        "addiu   $t7, $zero, 1\n"
        "lui     $at, 0x8011\n"
        "sb      $t7, 18160($at)\n"
        "lui     $at, 0x8011\n"
        "lui     $t8, 0x0020\n"
        "jal     0x800B6138\n"
        "sw      $t8, 29880($at)\n"
        "bne     $zero, $zero, 0x800FC034\n"



// === func_800FBF88 @ 0x800FBF88 ===

void func_800FBF88(u32 arg) {
    struct GameStruct *game_struct = (struct GameStruct *)gstate;
    struct CarData *car_array = (struct CarData *)car_array;

    u32 temp_v0 = gstate + 29876;
    u32 temp_t6 = arg << 13;

    if (temp_t6 >= 0) {
        u32 temp_t7 = 1;
        func_800C9BE0(arg);
        *(u8 *)(temp_v0 + 29884) = temp_t7;
        func_800FAF6C();
    }

    u8 temp_t8 = car_array->place;
    if (temp_t8 != 0) {
        *(u8 *)(car_array + 3860) = temp_t8;
    }

    if (*(u16 *)0x80110004 == 0) {
        *(u8 *)(car_array + 29880) = 0x08;
        func_800B6138();
        if (*(u8 *)0x80110004 != 0) {
            func_800F8D9C();
            func_800EC0DC();
            func_800BD2C8();
        }
    }

    u32 temp_s2 = car_array - 23984;
    u16 temp_a0 = *(u16 *)(temp_s2 + 1990);
    if (temp_a0 != 0) {
        u8 temp_t7 = *(u8 *)0x80110004;
        if (temp_t7 != 0) {
            func_0x11();
            if (*(u32 *)0x8003B5E0 & 0x2000) {
                func_0x11();
                func_0x11();
                func_0x11();
                func_0xE762C();
            }
        }
    }
}



// === func_800FBF90 @ 0x800FBF90 ===

void func_800FBF90(u16 param) {
    s32 sp = -24;
    u16 *ra = (u16 *)sp + 20;

    if (param >> 13 >= 0) {
        u16 t7 = 1;
        u32 at = gstate;
        func_800C9BE0(at);
        *(u16 *)(at + 29884) = t7;
        func_800FAF6C();

        s8 *t8 = (s8 *)car_array + 3836;
        *(s8 *)(gstate_prev + 3860) = *t8;

        if (at != 0) {
            *(u16 *)(at + 29880) = 8;
            func_0x800B6138(at);
        }

        u16 a0 = 1;
        func_800FB2C8(a0, 0);

        if (at != 0) {
            *(u16 *)(gstate + 18160) = 1;
        }

        func_800C813C(1);
        if ((*(u32 *)0x80110000 & 0x80110000) != 0) {
            u16 t6 = *(u16 *)gstate;
            if (t6 != 0) {
                func_800F8D9C();
                func_800EC0DC();
                func_800BD2C8();
            }
        }

        f32 *f6 = (f32 *)0x80030000;
        f6[-5196] = func_800E762C();
        *(u16 *)gstate = 0;
        func_800FBF2C();
        func_800FBE30();
    }
}



// === func_800FBFE4 @ 0x800FBFE4 ===

void func_800FBFE4(void) {
    s8 a0 = 1;
    s8 a1 = 0;

    u32 v0 = func_800FB2C8(a0);

    if (v0 == 0) {
        a0 = 0;
        a1 = 1;

        gstate[7345] = 1;

        u16* car_array_ptr = (u16*)car_array;
        func_800B6138(car_array_ptr[0], 29880);

        if (a0 == 0) {
            a0 = 1;

            u16 gstate_value = gstate[7248];
            if (gstate_value != 0) {
                func_800C813C(0, 1);
            }
        }

        u16* car_array_ptr2 = (u16*)car_array;
        func_800F8D9C(car_array_ptr2[0], 29148);

        func_800EC0DC(0);
        func_800BD2C8(0);

        u32* game_struct_ptr = (u32*)game_struct;
        s16 car_id = *(s16*)(game_struct_ptr[0] + 47858);
        if (car_id >= 0) {
            func_800BEAA0(car_id, 20258);
        }

        u16 gstate_value2 = gstate[7345];
        if (gstate_value2 != 0) {
            func_800FBE30(0);
        }
    } else {
        a0 = 0;

        u16 gstate_value3 = gstate[7248];
        if (gstate_value3 != 0) {
            func_800C813C(0, 1);
        }
    }

    a0 = 0;

    u16 gstate_value4 = gstate[7248];
    if (gstate_value4 != 0) {
        func_800FBF2C(0);
    }
}



// === func_800FC038 @ 0x800FC038 ===

void func_800FC038() {
    u8* gstate = (u8*)gstate;
    CarData* car_array = (CarData*)car_array;
    GameStruct* game_struct = (GameStruct*)game_struct;

    if (*gstate != 0) {
        func_800F8D9C();
        func_800EC0DC();
        func_800BD2C8();

        u8* car_data = (u8*)0x8015A6E0;
        s16 value = *car_data;

        if (*gstate != 0) {
            __asm volatile("nop" ::: "memory");
        }

        float f6;
        *(float*)&f6 = *((float*)(0x4060D2AC + (int)value));
        func_800E762C(&f6);
        *car_data = 0;

        func_800FBF2C();
        func_800FBE30();

        CarData* current_car = &car_array[game_struct->current_car_index];

        for (int i = 1; i < 4; i++) {
            if (i == game_struct->num_cars) break;

            u8* data_ptr = (u8*)0x8015A6E0;
            int index = *data_ptr;

            if (*data_ptr != 0) {
                __asm volatile("nop" ::: "memory");
            }

            if (index == i + 3 && game_struct->some_flag) {
                u8* ptr = (u8*)0x8014980C;
                int value = *ptr;

                __asm volatile("lw %0, %1(0)" : "=r"(value) : "i"(0x8014A0D4));

                u8* data_ptr2 = (u8*)0x8015A6E0;
                int index2 = *data_ptr2;

                if (*data_ptr2 != 0) {
                    __asm volatile("nop" ::: "memory");
                }

                if (value == 78 && game_struct->some_flag) {
                    u8* ptr2 = (u8*)0x8014980C;
                    int value2 = *ptr2;

                    __asm volatile("lw %0, %1(0)" : "=r"(value2) : "i"(0x8014A0D4));

                    if (value2 == 78 && game_struct->some_flag) {
                        u8* ptr3 = (u8*)0x8014980C;



// === func_800FC0EC @ 0x800FC0EC ===

void func_800FC0EC(u32 arg0) {

    CarData *car_array = (CarData *)0x80152818;
    GameStruct *game_struct = (GameStruct *)0x801461D0;

    u32 index = arg0 << 11 | (arg0 >> 7);
    CarData *car = &car_array[index];

    u32 value = car->data_valid;
    u32 temp = 1;

    if (value == temp) {
        if (temp != 0) {
            }
    }

    temp = 5;
    if (value == temp) {
        }
}



// === func_800FC228 @ 0x800FC228 ===

void func_800FC228() {
    float f8;
    s32 v0, t9, t6, a0, s0, s1, at, t7, s5, s6, a3, a1;

    gstate = 0x80152818;
    car_array = game_struct->cars;

    f8 = *(float*)(gstate + 0x72B8);
    game_struct->some_field = f8;

    if (game_struct->another_field != 0) {
        v0 = game_struct->yet_another_field;
        t9 = v0 << 9;
        t6 = v0 << 7;
        s0 = car_array + 16412;
        *s0 = f8;

        func_800C813C(v0, 1);
        func_800FA9B4();
        func_800FBE30();

        t6 = *(char*)(game_struct->some_field + 9881);
        if (t6 != 0) {
            func_800F7F3C();
            t7 = 0x100;
            gstate = 0x80152818;
            func_800D510C(t7);
            func_800D58CC();

            if (gstate == 0) {
                s5 = car_array - 309;
                a3 = *(char*)s5;

                if (a3 != 0) {
                    s6 = gstate - 24304;
                    a1 = *(short*)s6;
                    s2 = 1;
                    s0 = 0;

                    while (a1 > 0) {
                        s1 = car_array + 10080;
                        t7 = *(char*)s1;
                        a0 = s0 << 16;
                        t9 = a0 >> 16;

                        if (t9 != 0) {
                            func_800CF604(t9);
                        }

                        a0 = t9;
                        if (a0 != 0) {
                            a1--;
                        } else {
                            break;
                        }
                    }
                }
            }
        }
    }
}



// === func_800FC25C @ 0x800FC25C ===

void func_800FC25C(u32 arg0) {
    u32 temp = arg0 << 7;
    float f10 = *(float*)(game_struct + 0x41A4);
    float f6 = *(float*)(car_array + 0x43CC);

    func_800C813C(0, 1);
    *(float*)(car_array + 0x43CC) = f8;
    func_800FA9B4();
    func_800FBE30();

    if (*(u8*)(gstate + 9881) != 0) {
        func_800F7F3C(0x0100, *(u32*)0x8011);
        func_800D510C();
        *(u32*)(gstate + 29880) = 1;
        func_800D58CC();
    }

    if (*(u8*)(game_struct - 309) != 0) {
        u16* ptr3 = (u16*)(car_array - 24304);
        u16 value = *ptr3;
        if (value == 0) return;

        u16* ptr4 = (u16*)game_struct + 10080;
        u16 t8 = *ptr4;
        u32 temp2 = arg0 << 16;
        u16 index = (temp2 >> 16) & 0xFFFF;

        func_800CF604(index, 0);
        if (t8 == 0) return;

        if (index < value) t8 = *ptr4;

        func_800E7DD0(t8);
        if (t8 == 0 || index == 0) return;

        u32 temp3 = *(u32*)(car_array - 24304);
        *ptr3 = 4;
        func_800F7F3C(0x0100, *(u32*)0x8011);
    }
}



// === func_800FC89C @ 0x800FC89C ===

void func_800FC89C(volatile u32* state) {

    if (*state >= 0) {
        int index = *state << 6;
        volatile u16* data_ptr = (volatile u16*)car_array + 10052;
        u8 count = *data_ptr;

        if (count > 0) {
            volatile u16* next_ptr = (volatile u16*)car_array + 10264;
            volatile u16* prev_ptr = (volatile u16*)car_array - 23984;
            int step = -9, max_step = 952, increment = 2056;

            while (count != 0) {
                int new_index = *next_ptr + *state;
                volatile u16* entry = (volatile u16*)func_800D5828(new_index);
                u8 upper_byte = *entry >> 8, lower_byte = *entry & 0xFF;
                u8 modified_byte = lower_byte & step;
                func_800B0180(upper_byte, modified_byte);
                *entry = (modified_byte << 8) | (*entry & ~step);

                count = *data_ptr;
                int next_step = (new_index >> 16) + 1;
                new_index = next_step << 16;

                if (count == next_step)
                    break;

                next_step >>= 16;
            }

            func_800D5798();
            func_800D5374();

            beqz $zero, 1f");
            volatile u32* gstate_ptr = (volatile u32*)gstate;
            if (*gstate_ptr != 0) {
                int new_index = *gstate_ptr << 8;
                func_800F64D4(new_index);
                beqz $zero, 1f");
            }

            or $a0, $zero, $s0; or $a1, $zero, $s1; jal 0x800AB18C; lw $ra, 20($sp); addiu $t7, $s2, 152");
        }

        and $t9, $v0, $at; li $t0, 0x14; lw $ra, 20($sp)");
        bnez $t7, 1f");
    }
}



// === func_800FC960 @ 0x800FC960 ===

void func_800FC960(u32 arg0) {
    if (arg0 >= 1000) {
        u32 index = arg0 << 8;
        func_800F64D4(index);
        gstate->unk_74B4 = car_array[index];
        if (gstate->unk_74B4 != 0) {
            s32 value = gstate->unk_74B4 & 0x60;
            u16 count = *((u16*)(car_array + 0x1A9C)) - value;
            while (count--) {
                func_800AB18C(0, car_array[value]);
                value += 256;
                if (value >= 6576) {
                    break;
                }
            }
        }
    } else {
        u32 index = arg0 << 8;
        gstate->unk_74B4 = car_array[index];
        if (gstate->unk_74B4 != 0) {
            s32 value = gstate->unk_74B4 & 0x60;
            u16 count = *((u16*)(car_array + 0x1A9C)) - value;
            while (count--) {
                func_800AB18C(0, car_array[value]);
                value += 256;
                if (value >= 6576) {
                    break;
                }
            }
        }
    }
}



// === func_800FCA00 @ 0x800FCA00 ===

u32 func_800FCA00() {
    s32 sp = -40;

    u32 at = 0xC080 + (0x6000 >> 2);

    u32 gstate = 0x801146EC;
    CarData* car_array = 0x80152818;
    GameStruct* game_struct = 0x801461D0;

    u32 t5 = gstate + (0x1100 >> 2);
    u32 t4 = 0x8003461C;
    u32 t3 = car_array + (0x1A90 >> 2);
    u32 t2 = gstate + (0x104C >> 2);

    gstate += 0x44BC;
    car_array += 0x1B48;
    game_struct -= 0x20E8;

    t5 += 0x4498;
    t3 += 0x170C;
    t2 += 0x1D64;

    u32 a0 = gstate + 0x44B8;
    gstate += 0x43CC;
    game_struct = (GameStruct*)1;
    car_array = (CarData*)4;

    u32 v0 = 0;
    u32 v1 = *(u32*)a0;
    s16 a2 = *(s16*)&v1;

    if (a2 > 0) {
        u32 t0 = v0 << 3;
        t0 -= v0;
        t0 <<= 3;
        s16 t6 = *(s16*)(t3);
        f32 f6 = *(f32*)(t4);

        a1++;
        u32 t7 = t6 << 3;
        t7 -= t6;
        t7 <<= 5;

        u32 t8 = t2 + t7;
        u16 t9 = *(u16*)(t8 + 16);
        f32 f4 = *(f32*)(t9 - 208);

        t8 += (t7 << 5) + t0;
        a3 += 20;

        while (a3 < v1 + 20) {
            u32 t7 = t6 << 3;
            t7 -= t6;
            t7 <<= 5;

            u32 t9 = t2 + t7;
            u16 t8 = *(u16*)(t9 + 18);
            f32 f10 = *(f32*)(t8 - 204);

            t8 += (t7 << 5) + t0;
            a3 += 20;
        }
    }

    return v0;
}



// === func_800FCDF8 @ 0x800FCDF8 ===

void func_800FCDF8(u8 param) {
    if (param == 0) return;

    u32 *sp = &param;
    u32 *ra = sp + 1;
    u8 *gstate_ptr = (u8*)gstate;
    u8 t7 = 1;
    u8 at = 0;
    u32 t8 = 0;

    if (*gstate_ptr == 0) {
        *gstate_ptr = 1;

        u32 car_index = *(u32*)car_array;
        u8 *at_ptr = (u8*)gstate + 16732;
        *at_ptr = 1;

        if ((car_index << 6) < 0) {
            *(u16*)(car_array + 6864) = t8;
            *(u32*)gstate - 24304 = 0;
        }

        func_800EC190(0);

        u16 *a0_ptr = (u16*)gstate + 16728;
        u8 t7 = *(u8*)sp;

        // Function calls and logic based on the game structure
        func_800FB2C8(0);
        func_800F8D9C(0);
        func_800EC0DC();
        func_800FBE60();

        t9 = 1;
        if (t7 == 0) {
            *(u8*)sp = 1;
            at = 0;

            // Additional game logic and function calls
            func_800FA9B4();

            if (t7 != 0) {
                u32 *s3 = &param;
                u8 *at_ptr = (u8*)car_array + 10052;
                *at_ptr = 0;

                u8 t7 = *(u8*)sp;
                if (t7 > 0) {
                    u32 s0 = 0;
                    u32 *s1 = &param;
                    u32 s2 = 2056;

                    // Loop to process game data
                    do {
                        func_800D5828(s1);
                        u16 *a0_ptr = (u16*)s9 + 1990;
                        u8 t8 = *(u8*)sp;
                        s0 += 1;
                        u32 s0_shifted = s0 << 16;
                        u32 s0_sra = s0_shifted >> 16;
                        if (s0_sra < t8) {
                            continue;
                        }
                    } while (1);

                    func_800D5798();
                    func_800D5374();
                }
            }
        }
    }
}



// === func_800FD024 @ 0x800FD024 ===

void func_800FD024(void) {
    u8* gstate = (u8*)gstate;
    CarData* car_array = (CarData*)car_array;
    GameStruct* game_struct = (GameStruct*)game_struct;

    if (*(u8*)(gstate + 16716) == 0) {
        u32 s0 = 0;
        func_800B37E8(0, 0, 1);
        *(u32*)(game_struct->some_field + 16720) = 1;

        // Code to handle some conditions and update game state
        u32 at = 0;
        if (*(u16*)car_array->some_field == 5) {
            func_800B6138();
            sb(1, gstate + 16716);
        }

        if (game_struct->some_field2 != 0 && game_struct->some_field3 != 0) {
            lw(at, gstate + 16720);
            func_800B358C(at, 0);
            sb(0, gstate + 16716);
        }
    }

    // Additional code to handle other cases
}



// === func_800FD238 @ 0x800FD238 ===

void func_800FD238(u32 param) {
    u8* gstate = (u8*)gstate;
    CarData* car_array = (CarData*)car_array;
    GameStruct* game_struct = (GameStruct*)game_struct;

    if (param & 0x0002) {
        func_8039A2A0();
    }

    if (!(param >> 16)) {
        return;
    }

    if (param & 0x0004) {
        func_8038A400();
    }

    if (!(param >> 2)) {
        return;
    }

    if (param & 0x0008) {
        func_8038F454();
    }

    if (!(param >> 13)) {
        return;
    }

    if (param & 0x0010) {
        func_800FD024();
    }

    if (!(param >> 12)) {
        return;
    }

    if (param & 0x2000) {
        ((u32*)game_struct->field_0x80146a04)[0] = 0;
        func_80097694(54, -1);
        func_800965BC(*gstate_prev);
        func_80390418((u32)(param | *gstate));
    }

    if (!(param >> 1)) {
        return;
    }

    if (param & 0x0020) {
        func_80394058();
    }

    if (!(param >> 7)) {
        return;
    }

    if (param & 0x0040) {
        func_8039842C();
    }

    if (!(param >> 14)) {
        return;
    }

    if (param & 0x0080) {
        func_8039C140();
    }

    if (!(param >> 16)) {
        return;
    }

    if (param & 0x0100) {
        func_8038E5DC();
    }

    if (!(param >> 24)) {
        return;
    }

    if (param & 0x0200) {
        func_803A1EAC();
    }
}



// === func_800FD240 @ 0x800FD240 ===

void func_800FD240(s32 flags) {
    s32 t6 = flags & 0x0002;
    if (t6 != 0) {
        u16 t7 = flags << 1;
        func_8039A2A0(t7);
    }

    if (flags >= 0) {
        s32 t8 = flags & 0x0004;
        if (t8 != 0) {
            func_8038A400(flags);
        }

        s32 t9 = flags & 0x0008;
        if (t9 != 0) {
            func_8038F454(flags);
        }

        t6 = flags & 0x0010;
        if (t6 != 0) {
            func_800FD024(flags);
        }
    }

    if (flags >= 0) {
        s32 t7 = flags & 0x2000;
        if (t7 != 0) {
            *((char *)game_struct + 2564) = 0;
            func_80097694(54, -1);
            func_800965BC(flags);
        }

        if (flags >= 0) {
            s32 t8 = flags & 0x0020;
            if (t8 != 0) {
                func_80394058(flags);
            }

            s32 t9 = flags & 0x0040;
            if (t9 != 0) {
                func_8039842C(flags);
            }

            t6 = flags & 0x4000;
            if (t6 != 0) {
                func_80399D10(flags);
            }

            if (flags >= 0) {
                s32 t7 = flags & 0x0080;
                if (t7 != 0) {
                    func_8039C140(flags);
                }

                if (flags >= 0) {
                    s32 t8 = flags & 0x0100;
                    if (t8 != 0) {
                        func_8038E5DC(flags);
                    }

                    s32 t9 = flags & 0x0200;
                    if (t9 != 0) {
                        func_803A1EAC(flags);
                    }

                    t6 = flags & 0x0400;
                    if (t6 != 0) {
                        func_800DB1E0(flags);
                    }
                }
            }
        }
    }
}



// === func_800FD464 @ 0x800FD464 ===

void func_800FD464() {
    u8 currentState = *(u8*)(gstate + 18156);
    s32 ra = *(s32*)((char*)sp + 76);
    s32 fp = *(s32*)((char*)sp + 72);
    s32 s7 = *(s32*)((char*)sp + 68);
    s32 s6 = *(s32*)((char*)sp + 64);
    s32 s5 = *(s32*)((char*)sp + 60);
    s32 s4 = *(s32*)((char*)sp + 56);
    s32 s3 = *(s32*)((char*)sp + 52);
    s32 s2 = *(s32*)((char*)sp + 48);
    s32 s1 = *(s32*)((char*)sp + 44);
    s32 s0 = *(s32*)((char*)sp + 40);
    f64 f22 = *(f64*)((char*)sp + 32);
    f64 f20 = *(f64*)((char*)sp + 24);

    if (currentState != 0) {
        *(s32*)((char*)sp + 84) = 0;
        s32 padEntryOffset = 0x801461D0;
        *(u8*)(padEntryOffset + 11004) = 0;
        u16 checkpoint = (*(s32*)(gstate + 18156)) & 0x000F;
        if (checkpoint != 0) {
            CarData* carData = *(CarData**)(car_array);
            f32 lapDistance = carData->dr_pos[0] - carData->RWR[0];
            if (lapDistance < 0.0f) {
                f10 = *(f64*)-20552;
                if ((currentState & 0x000F) != 0) {
                    u32 aiDifficulty = (*(s16*)(gstate + 18152)) & 0xFFFF;
                    func_800C9AE0(aiDifficulty);
                    s32 carDataOffset = 0x80152818;
                    *(CarData**)(car_array) = (CarData*)((char*)carData + 64);
                }
            }
        }
    }
}



// === func_800FD7E8 @ 0x800FD7E8 ===

void func_800FD7E8(void* arg0, void* arg1) {
    u32 car_index = *(u32*)arg0;
    u32 offset = car_index * 4 + car_index * 2 - car_index;
    u8* car_data = (u8*)(car_array - offset);
    
    if (*(u8*)car_data == 5) {
        u32 offset2 = car_index * 4 + car_index * 2 - car_index;
        u8* game_data = (u8*)(game_struct - offset2);
        
        if (*(u8*)game_data & 0x01) {
            *(u8*)car_data = 1;
        }
    } else {
        u32 offset3 = car_index * 4 + car_index * 2 - car_index;
        u8* car_data2 = (u8*)(car_array - offset3);
        
        if (*(u32*)car_data2 == 1) {
            u32 car_id = arg1 & 0xFF;
            func_800C9158(car_id, 1);
        }
    }
}

void func_800FD8DC(void* arg0, void* arg1) {
    u8 value = *(u8*)(arg0 + 1600);
    
    if (value != 0) {
        if (*(u8*)arg0 == 0) {
            f32 float_value;
            memcpy(&float_value, (void*)(0x801257B4), sizeof(float));
            func_800C9158(1, 1);
        }
    } else {
        f32 float_values[4];
        memcpy(float_values, (void*)(0x801257B4 + 16), 16 * sizeof(f32));
        func_800C9158(*(u32*)arg1, 1);
    }
}



// === func_800FD8DC @ 0x800FD8DC ===

void func_800FD8DC(void *arg0, float *arg1, void *arg2) {
    u8 t6 = *(u8 *)arg0;
    void *a3 = arg0;

    f32 f16, f14, f12, f18, f20, f22, f24, f26;

    if (t6 != 0) {
        f16 = *(f32 *)(gstate + 0x75DC);
        f18 = *(f32 *)(gstate + 0x75E0);
    } else {
        f16 = arg1[0];
        f14 = arg1[1];
        f12 = arg1[2];
    }

    float v0[3] = {f16, f18, f16};
    f18 = *(f32 *)(a3 + 52);
    f16 = *(f32 *)(a3 + 56);

    if (v1) {
        f18 = v1;
    }

    arg0 = a0 + 64;
    u32 v1 = arg0;

    while ((*(u32 *)v1 & 0xFFFFFFFE) != 0) {
        *(f32 *)(v0 + 12) = f18;
        f18 = *(f32 *)(v0 + 16);
        f16 = *(f32 *)(v0 + 20);
        v0[0] = f16;
        v0[1] = f18;
        v0[2] = f16;
        v0 += 4;
    }

    a0 = (void *)52;
    arg1 = (void *)40;
    arg2 = a3 + 1952;

    func_800A61B0(arg0, arg1, arg2);

    f10 = *(f32 *)(a3 + 72);
    f8 = *(f32 *)arg1;
    f6 = *(f32 *)(arg1 + 4);
    *(f32 *)arg1 = f18;

    f4 = *(f32 *)(a3 + 80);
    f18 = *(f32 *)(arg1 + 8);
    *(f32 *)(arg1 + 4) = f10;
    *(f32 *)(arg1 + 12) = f4;

    *(u32 *)ra = 20;
    *(void **)(sp + 72) = a3 + 72;
    a3 = (void *)*(s32 *)(sp + 72);
    f8 = *(f32 *)(a3 + 40);
    f10 = *(f32 *)(arg1 + 292);

    *(f32 *)(arg1 + 292) = f18;
    f6 = *(f32 *)(a3 + 44);
    f18 = *(f32 *)(arg1 + 300);
    *(f32 *)(arg1 + 296) = f10;

    f4 = *(f32 *)(a3 + 48);
    *(f32 *)(arg1 + 300) = f6;
    *(u32 *)ra = 20;
    *(void **)(sp + 72) = a3 + 72;
    a3 = (void *)*(s32 *)(sp + 72);
    *(f32 *)a3 = 52;

    sp += 72;
    jr(ra);
}



// === func_800FD9F8 @ 0x800FD9F8 ===

int func_800FD9F8(float param1, float param2, void* param3) {
    f32 f6, f4;
    int t1, t0;

    \n"
        "sw $a0, 120($sp)\n"
        "sw $a3, 132($sp)\n"

        "lwc1 %f6, 40(%2)\n"
        "lwc1 %f4, 1940(%1)\n"
        "or %t1, %a2, $zero\n"
        "or %t0, %a1, $zero\n"

        "j 0x800A61B0\n"
        "nop\n"
        "nop\n"

        "lwc1 %f8, 96($sp)\n"
        "lw %t1, 128($sp)\n"
        "lwc1 %f10, 68($sp)\n"
        "lw %t0, 124($sp)\n"

        "lw %v0, 0(%t1)\n"
        "move $v1, $zero\n"

        "lwc1 %f4, 60($sp)\n"
        "lwc1 %f16, 8($v0)\n"
        "j 0x800A61B0\n"
        "nop\n"
        "nop\n"

        "addiu $v1, $zero, 1\n"
        "jr $ra\n"

        : "=f"(f6), "=f"(f4), "=r"(t1), "=r"(t0)
        : "f"(param1), "f"(param2), "r"(param3)
    );

    return 0;
}



// === func_800FDF94 @ 0x800FDF94 ===

void func_800FDF94(void) {
    u32 t7 = (u32)a0 << 2;
    t7 += a0;
    t7 <<= 2;
    t7 -= a0;
    t7 <<= 2;
    t8 += t7;

    s32 v1 = *(s32*)(t8 - 24224);
    u8 ra_save = a1;
    u32 a0_save = a0;

    a1 = 0;
    a2 = 76;
    t0 = *(u32*)(v1 + 44);
    a3 = *(u32*)t0;
    a0 = a3 + 1780;
    func_80002790();

    u8 car_index = *(u8*)&car_array[gstate->current_car].index;

    t2 = (car_index << 4) - car_index;
    t3 = 0x80152818[car_index].flags;
    t2 += t3 * 4;
    t3 = *(s32*)(t3 + 28);
    s32 flags = *(s32*)t3;

    v1 = (v1 >> 1) & 0xFFFF;
    a1 = (car_index << 4) - car_index;
    t2 += (a1 * 4) - a1;
    t2 = ((u32)v1 << 16) | flags;
    a3 = *(s32*)game_struct;

    if (*(u8*)(gstate + 24871)) {
        t8 += (a3 << 2);
    }

    *(u8*)(t8 + 8) = v1;
    s32 timer = *(s32*)(*(s32*)0x8003000F - 5232);
    *(u8*)(t8 + 4) = 1;
    *((float*)(t8 + 16)) = f4;
    *(u8*)(t8 + 7) = timer;

    a1 = ra_save;
    a0 = a0_save;
}


