/**
 * Rush 2049 - Render Functions
 * 3D rendering, display lists, visuals
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


// === func_80087068 ===



void func_80087068(u32 v0, u32 a0) {

    s32 sp = -24;

    u32 ra = *(u32*)(a0 + 16);

    u32 t6 = v0 & a0;

    if (t6 == 0) return;



    u32 t0 = a0;

    u32 t7 = ~a0;

    u32 t8 = v0 & t7;

    u32 t9 = a0 & 1;

    if (t9 != 0) {

        *(u32*)v1 = t8;



        u32 a1 = 0x80152818 - 27592;

        u32 v1 = *(u32*)a1;

        u32 t6 = v1 + 8;

        u32 a2 = 0xE2001E01;

        *(u32*)a1 = t6;

        *(u32*)(a1 + 4) = 0;

        *(u32*)a1 = a2;



        u32 t8 = a0 & 0x10;

        if (t8 != 0) {

            a1 = 0x80152818 - 27592;

            v1 = *(u32*)a1;

            t6 = v1 + 8;

            u32 a2 = 0xE2001D00;

            *(u32*)a1 = t6;

            *(u32*)(a1 + 4) = 0;

            *(u32*)a1 = a2;



            func_80086A50(a0);

        }



        u32 t7 = a0 & 0x20;

        if (t7 != 0) {

            func_80086A50(a0);

        }

    }



    *(u32*)(a0 + 16) = ra;

}



void func_80087118(u32 a0, u32 v0, u3


// === func_80087118 ===



int func_80087118(int a0, int a1, int a2, int a3) {

    int t0 = a0;

    int at = a0 < v0 ? 1 : 0;

    int t2 = a1;

    if (at != 0) {

        int t1 = a2;

        void* v1 = (void*)0x8013E608;

        int t6 = ((int)v1[0] & 4) ? 0 : 1;

        if (t6 == 0) {

            t8 = a1 + v0;

            a1 = t8 - a0;

        }

    }

    void* v0 = (void*)0x8013E608;

    int at = a2 < *v0 ? 1 : 0;

    if (at != 0) {

        int t9 = car_array[0];

        int a0 = *(int*)(a3 + 124);

        int t6 = (t9 & 8) ? 0 : 1;

        if (t6 == 0) {

            t7 = a0 + *v0;

            a0 = t7 - a2;

        }

    }

    void* v0 = (void*)0x8013E608;

    int at = v0 < a3 ? 1 : 0;

    if (at != 0) {

        int t8 = car_array[0];

        int a1 = *(int*)(a3 + 124);

        int t6 = (t8 & 8) ? 0 : 1;

        if (t6 == 0) {

            t9 = a0 + *v0;

            a0 = t9 - a1;

        }

    }

    void* v0 = (void*)0x8013E608;

    int at = v0


// === func_800878EC ===



void func_800878EC(u32 arg0, u32 arg1) {

    u32* car_ptr = &car_array[arg0];

    if (arg0 != arg1) {

        u32 car_index = arg0;

        u32 temp_value = arg1;

        u32 bit_check = arg0 & 0x4000;

        if (bit_check != 0) {

            *car_ptr = temp_value;

            *(u32*)0x801527C8 = 0x801527C8 + 8;

            u32* game_struct_ptr = &game_struct[0x801527E0];

            if (*car_ptr != 0) {

                if (arg0 < 4) {

                    *game_struct_ptr = 0xFFFFFFFF;

                } else {

                    *game_struct_ptr = 0xFFFFFFFC;

                }

                *(u32*)0x801527F8 = 0x801527F8 + 8;

            }

        }

        if (car_index & 0x0001) {

            *(u32*)0x801527C8 = 0x801527C8 + 8;

            *game_struct_ptr = 0xE3000A01;

            *(u32*)0x801527F8 = 0x801527F8 + 8;

        }

        if (car_index & 0x0010) {

            *(u32*)0x801527C8 = 0x801527C8 + 8;

            *ga


// === func_80087A08 @ 0x80087A08 ===

void func_80087A08(u32 arg0, u16 arg1, u16 arg2) {
    s32 sp = -632;

    u32 s0 = 0;
    u32 s1 = 0;
    u32 ra = func_80087A08;
    u32 a0 = arg0;
    u32 a1 = arg1;
    u16 a2 = arg2;

    if (s0 != 0) {
        u32 v0 = a2;
        u16 t7 = 0x0003;
        u16 t8 = 0x0002;
        u32 t9 = 0;

        // Function logic here

    }

    // Additional function logic here

    return;
}



// === func_8008A398 ===



void func_8008A398(u16 arg0) {

    u16 temp_t6 = arg0 & 0xFFFF;

    if (temp_t6 != gstate->current_state) {

        car_array[0]->current_state = temp_t6;

        *(u32*)(0x8013F75C) = 0;



        func_8008A148((void*)car_array, 4, 1);

        func_800878E0(0);



        if (arg0 >= 32) {

            *(u32*)(0x8013F6AC) = arg0;

        } else {

            gstate->current_state = arg0;

            *(u32*)(0x8013F69C) = arg0;



            if (arg1 >= 4) {

                *(u32*)(0x8013F650) = arg1;

            } else {

                *(u32*)(0x8013F64C) = arg1;



                if (arg2 > gstate->max_speed) {

                    *(u32*)(0x8013F640) = arg2;

                }



                if (arg3 > gstate->max_acceleration) {

                    *(u32*)(0x8013F674) = arg3;

                }

            }

        }

    }

}



void func_8008A474(u16 arg0, u16 arg1, u16 arg2, u16 arg3) {

    if (arg0 < 0x8013F65C) {

       


// === func_8008A474 ===



void func_8008A474(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {

    s32 t2 = (s32)arg0;

    s32 t3 = (s32)arg1;

    s32 t4 = (s32)arg2;

    s32 t5 = (s32)arg3;



    if (t2 < 0) {

        t2 = 0;

    }

    if (t3 < gstate->car_array) {

        t3 = gstate->car_array;

    }

    if (t4 > ggame_struct->max_cars) {

        t4 = ggame_struct->max_cars;

    }



    u8 *car_ptr = (u8 *)(gstate->car_array + t3);

    s32 car_index = (t4 - 1) % ggame_struct->max_cars;



    u32 car_data = car_ptr[0] << 24 | car_ptr[1] << 16 | car_ptr[2] << 8 | car_ptr[3];

    u32 car_data2 = (car_ptr[4] << 24) | (car_ptr[5] << 16) | (car_ptr[6] << 8) | car_ptr[7];



    if (t2 < t4 && t3 < ggame_struct->max_cars && t5 < ggame_struct->max_cars) {

        // Call other functions

    }



    car_ptr[36] = t5;

    car_ptr[32] = t4;

    car_ptr[28] = t3;

    car_ptr[24] = t2;



    // Function calls

}









// === func_8008A650 ===



void func_8008A650(u16 arg) {

    u16 temp = arg & 0xFFFF;

    if (temp != gstate->frame_count) {

        car_array[arg].index = (car_array[arg].index + 1) % 2;

        car_array[arg].timer = 0x0EE00 | (temp << 16);

        car_array[arg].unk_0C = temp;

        car_array[game_struct->player_car_index].current_speed = 0x10;

    }

}



void func_8008A6D0() {

    ((u8*)0x801527E4)[gstate->frame_count] = (u8)func_8008A6F8();

}



int func_8008A710() {

    if (gstate->frame_count != 0) {

        ((u8*)0x801527E4)[gstate->frame_count] = 1;

        func_8008A6F8();

        func_8008A6F8();

        return func_8008A6F8();

    }

    return 0;

}









// === func_8008A6D0 ===



void func_8008A6D0(void) {

    int result;

    func_800075E0((void*)0x80152818, 0);

}



void func_8008A710(int param) {

    if (param != 0) {

        func_80006A00((void*)0x80152818, 1, 0);

    } else {

        func_800075E0((void*)0x80152818, 0);

    }

}



void func_8008A77C(int param) {

    int result;

    void* sp = (void*)sp;

    sp -= 128;

    *(int*)((char*)sp + 128) = param;

    sp -= 60;

    *(int*)((char*)sp + 60) = (int)&func_8008A77C;

    sp -= 56;

    *(void**)((char*)sp + 56) = __builtin_frame_address(0);

    sp -= 52;

    *(void**)((char*)sp + 52) = s7;

    sp -= 48;

    *(void**)((char*)sp + 48) = s6;

    sp -= 44;

    *(void**)((char*)sp + 44) = s5;

    sp -= 40;

    *(void**)((char*)sp + 40) = s4;

    sp -= 36;

    *(void**)((char*)sp + 36) = s3;

    sp -= 32;

    *(void**)((char*)sp + 32) = s2;

    sp -= 28;

    *(void**)((char*)sp + 28) = s1;

    sp -= 24;

    *(void**)((char*)sp + 24) = s0;



    fu


// === func_8008A710 ===



void func_8008A710(int param1, int param2) {

    int sp = -32;

    int t7 = 1;



    if (param1 != 0) {

        ra = *(int *)(sp + 20);

        gstate = *(struct GState *)0x801146EC;

        car_array = *(struct Car **)0x80152818;

        game_struct = *(struct GameStruct *)0x801461D0;



        car_array[param1] = t7;

        param1 += 10212;

        gstate->field_80 = param1;

        param1 -= 26672;

        func_80006A00(param1, &t7);

        func_800075E0(param1, 0);

        param1 += 32;



        ra = *(int *)(sp + 20);

        sp += 32;

    } else {

        sp -= 128;

        a0 = *(int *)sp;

        ra = *(int *)(sp + 60);

        fp = *(int *)(sp + 56);

        s7 = *(int *)(sp + 52);

        s6 = *(int *)(sp + 48);

        s5 = *(int *)(sp + 44);

        s4 = *(int *)(sp + 40);

        s3 = *(int *)(sp + 36);

        s2 = *(int *)(sp + 32);

        s1 = *(int *)(sp + 28);

        s0 = *(int *)(sp + 24);



        p


// === func_8008A750 @ 0x8008A750 ===

void func_8008A750() {
    CarData *car_array = car_array;
    int index = 1;

    car_array[index].RWV[2] = func_80007270(car_array[index].RWV[2], 1);
}

void func_8008A77C() {
    u8 *gstate = gstate;
    CarData *car_array = car_array;

    \n"
                      "sw $ra, 60($sp)\n");

    u32 constant1 = (u32)car_array + 17308;
    u32 constant2 = (u32)car_array - 5912;

    car_array[constant2 / sizeof(CarData)].RWV[2] = func_80006A00(car_array[constant2 / sizeof(CarData)].RWV[2], 1);

    \n"
                      "sw $s7, 52($sp)\n"
                      "sw $s6, 48($sp)\n"
                      "sw $s5, 44($sp)\n"
                      "sw $s4, 40($sp)\n"
                      "sw $s3, 36($sp)\n"
                      "sw $s2, 32($sp)\n"
                      "sw $s1, 28($sp)\n"
                      "sw $s0, 24($sp)\n");

    u32 constant4 = (u32)car_array - 5404;
    u32 constant5 = (u32)car_array + 21592;

    car_array[constant5 / sizeof(CarData)].RWV[2] = func_80007270(car_array[constant5 / sizeof(CarData)].RWV[2], 1);

    \n"
                      "lw $s7, 52($sp)\n"
                      "lw $s6, 48($sp)\n"
                      "lw $s5, 44($sp)\n"
                      "lw $s4, 40($sp)\n"
                      "lw $s3, 36($sp)\n"
                      "lw $s2, 32($sp)\n"
                      "lw $s1, 28($sp)\n"
                      "lw $s0, 24($sp)\n"
                      "lw $a0, 128($sp)\n"
                      "lw $ra, 60($sp)\n"
                      "addiu $sp, $sp, 128\n"
                      "jr $ra");
}



// === func_8008A77C ===



void func_8008A77C(void *arg) {

    \n"

        "sw $ra, 60($sp)\n"

        "lui $a0, 0x8015\n"

        "addiu $a0, $a0, 17308\n"

        "lui $a1, 0x8015\n"

        "addiu $a1, $a1, 17272\n"

        "jal 0x80006A00\n"

        "li $a2, 1\n"

        "lui $a0, 0x8003\n"

        "addiu $a2, $a2, 17272\n"

        "lui $a0, 0x8015\n"

        "addiu $a0, $a0, -5912\n"

        "jal 0x800005D4\n"

        "li $a1, 120\n"

        "lui $fp, 0x8012\n"

        "lui $s7, 0x8012\n"

        "lui $s3, 0x8003\n"

        "addiu $s3, $s3, 21592\n"

        "addiu $s7, $s7, -5404\n"

        "subu $fp, $fp, 4884\n"

        "li $s6, 4\n"

        "li $s5, 1\n"

        "jal 0x80007270\n"

        "or $a2, $s5, $zero\n"

        "lui $t6, 0x8011\n"

        "lb $t6, 6476($t6)\n"

        "li $a0, 0x8015\n"

        "addiu $a0, $a0, -26672\n"

        "bne $t6, $zero, 0x8008A850\n"




// === func_8008AA20 ===



#include <stdint.h>



void func_8008AA20(void) {

    \n"

        "jal     0x800205E4\n"

        "nop\n"

        "lw      $ra, 20($sp)\n"

        "addiu   $sp, $sp, 24\n"

        "jr      $ra\n"

        "nop\n"

    );

}



void func_8008AA40(void* arg) {

    \n"

        "lui     $fp, 0x8015\n"

        "addiu   $fp, $fp, 10120\n"

        "sw      $ra, 68($sp)\n"

        "sw      %0, 104($sp)\n"

        "lui     $a1, 0x8015\n"

        "sw      $s7, 60($sp)\n"

        "sw      $s6, 56($sp)\n"

        "sw      $s5, 52($sp)\n"

        "sw      $s4, 48($sp)\n"

        "sw      $s3, 44($sp)\n"

        "sw      $s2, 40($sp)\n"

        "sw      $s1, 36($sp)\n"

        "sw      $s0, 32($sp)\n"

        "sdc1    $f20, 24($sp)\n"

        "sw      $zero, 92($sp)\n"

        "ad


// === func_8008AA40 ===



#include <stdint.h>



typedef struct {

    // Define the structure of game_struct here

} GameStruct;



typedef struct {

    // Define the structure of car_array here

} CarArray;



extern GameStruct* gstate;

extern CarArray* car_array;

extern void some_function(uint32_t, uint32_t);

extern float some_other_function(uint32_t);



void func_8008AA40(void) {

    \n"

        "lui     $fp, 0x8015\n"

        "addiu   $fp, $fp, 10120\n"

        "sw      $ra, 68($sp)\n"

        "sw      $a0, 104($sp)\n"

        "lui     $a1, 0x8015\n"

        "sw      $s7, 60($sp)\n"

        "sw      $s6, 56($sp)\n"

        "sw      $s5, 52($sp)\n"

        "sw      $s4, 48($sp)\n"

        "sw      $s3, 44($sp)\n"

        "sw      $s2, 40($sp)\n"

        "sw      $s1, 36($sp)\n"

        "sw      $s0, 32($sp)\n"

        "sdc1    $f20, 24($sp)\n"

        "sw      $zero, 92($


// === func_8008ABF4 ===



int func_8008ABF4(int arg0, int arg1, int arg2) {

    int sp = -40;

    int ra = *(&sp + 36);

    int at = (arg0 < arg2);

    if (!at) {

        return 0;

    } else {

        int v0 = 0;

        int t7 = arg0 + 1;

        int t9 = t7 & 0xFFFF;

        int t8 = *(int*)(arg1 + 8);

        int t6 = (t9 << 2) - t9;

        t6 <<= 2;

        *(short*)(arg1 + 4) = t7;

        v0 = t8 + t6;

        int t7a = *(int*)(v0 - 4);

        int a3 = *(int*)(v0 - 8);

        int t8a = 0x80153E68;

        *(int*)(sp + 16) = t7a;

        int t9a = *(int*)(v0 - 12);

        t8a += 0x3E68;

        int a0a = 0x80161438;

        *(int*)(sp + 24) = t8a;

        int a1a = 1;

        int a2a = 0;

        func_80008630(a0a, a1a, a2a);

        *(int*)(sp + 20) = t9a;

        if (t9a != 0) {

            v0 = 1;

        }

    }

    ra = *(&sp + 36);

    sp += 40;

    return v0;

}



void func_8008AC74(int arg0, int arg1) {

    int sp = -32;

    i


// === func_8008AC74 ===



int func_8008AC74(int arg0) {

    int sp = arg0 - 32;

    \n"

        "lui $s1, 0x8015\n"

        "addiu $s1, $s1, 15976\n"

        "sw $ra, 28($sp)\n"

        "sw $a0, 32($sp)\n"

        "lui $a1, 0x8015\n"

        "addiu $a1, $a1, 16112\n"

        "or $a0, $s1, $zero\n"

        "jal 0x80006A00\n"

        "li $a2, 1\n"

        "lui $s0, 0x8015\n"

        "addiu $s0, $s0, 16144\n"

        "sb $zero, 0($s0)\n"

        "or $a0, $s1, $zero\n"

        "move $a1, $zero\n"

        "jal 0x80007270\n"

        "li $a2, 1\n"

        "jal 0x8008ABE4\n"

        "nop\n"

        "i-type op=0x15\n"

        "or $a0, $s1, $zero\n"

        "sb $zero, 0($s0)\n"

        "lw $a0, 12($s0)\n"

        "move $a1, $zero\n"

        "jal 0x800075E0\n"

        "li $a2, 1\n"

        "beq $zero, $zero, 0x8008ACB4\n"

        "nop\n"

        "lw $ra, 28($sp)\n"

        "lw $s0, 20(


// === func_8008AD48 ===



int func_8008AD48(int a0) {

    int v0 = *(car_array + (a0 >> 2));

    int at = 0xFF00;

    int a1 = a0 & 0x00FF;

    int v1 = (v0 & at) >> 8;

    at = 0x0500;

    if (v1 != at) {

        a1 &= 0x00FF;

        v1 &= at;

        v0 &= ~at;

        v0 |= ((a1 << 8) | v1);

        *(car_array + (a0 >> 2)) = v0;

        return 2;

    }

    at = 0x0700;

    if (v1 != at) {

        a1 &= 0x00FF;

        v1 &= at;

        int t6 = v0 & 0xFFFF;

        t6 >>= 8;

        v0 &= ~at;

        v0 |= ((a1 << 8) | t6);

        *(car_array + (a0 >> 2)) = v0;

        return 2;

    }

    at = 0x0600;

    if (v1 == at) {

        a1 &= 0x00FF;

        v1 &= at;

        int t7 = v0 & 0xFFFF;

        t7 >>= 8;

        int t4 = a1 << 8;

        int t2 = v0 & 0xFF00;

        t2 >>= 8;

        int t6 = v0 & 0xFFFF;

        t6 >>= 8;

        int t3 = t1 | t2;

        t8 = t6 | t7;

        t9 = a1 << 8;

        int t5 = t3 | t4;

        v0 |


// === func_8008AE8C ===



void func_8008AE8C(volatile u32* gstate, int car_index, int flag) {

    volatile CarStruct* car_array = (volatile CarStruct*)0x80152818;

    GameStruct* game_struct = (GameStruct*)0x801461D0;



    s1 = 0x8013;

    s4 = -1;

    s3 = car_index;

    s2 = 68;

    s5 = 0;

    ra = func_8008AE8C;



    if (flag != 2) {

        u32 t6 = game_struct->car_data[car_index].some_value << 16;

        u32 t7 = t6 >> 16;

        u32 t9 = s1 - 6400 + (t7 * 8);

        volatile CarData* car_data = (volatile CarData*)t9;



        u32 v0 = car_data->some_value;

        u32 t2 = v0 | (s3 << 8);

        u32 s0 = s1 - 6400 + (v0 * 8);

        u16 value = ((volatile CarData*)s0)->some_other_value;



        if (value != -1) {

            ((volatile CarData*)s0)->some_value = t2;

        }

    }



    if (flag == 3) {

        u32 t3 = game_struct->car_data[car_index].some_value << 16;

        u32 t4 = t3 >> 16;

        u32 t8 = s1 - 6400 + (t4 * 8);




// === func_8008B0D8 ===



void func_8008B0D8(int a0, int a1, int a2) {

    char *sp = (char *)__builtin_frame_address(0);

    int s3 = a2;

    int s1 = 0x8013 << 16;

    int s4 = -1;

    unsigned short *a2_ptr = (unsigned short *)0x8000;

    s1 -= 6400;



    sp -= 48;

    *(int *)(sp + 32) = s3;

    *(int *)(sp + 24) = a1;

    *(int *)(sp + 36) = s4;

    *(int *)(sp + 28) = a0;

    *(int *)(sp + 44) = (int)&func_8008B0D8 + 48;

    *(int *)(sp + 40) = a1;

    *(int *)(sp + 20) = s3;



    int v1 = 1;

    if (a1 != v1) {

        int t6 = a0 << 16;

        int t7 = t6 >> 16;

        t6 &= 0xFFFF;



        int t1 = s3 << 8;

        int t2 = ~t1 | 0x8000;



        int t9 = s1 + t6;

        unsigned char *v0 = *(unsigned char **)(t9);

        v1 = t7 & 0xFFFF;

        t3 = v0[v1] & t2;



        t9 = s1 + t0;

        unsigned short *v0_ptr = (unsigned short *)*(int *)(t9 + 2);



        if (*v0_ptr != s4) {

            int a0_temp = *v0_ptr;

           


// === func_8008B26C ===



int func_8008B26C(signed char *arg0) {

    if (arg0 != NULL) {

        int result = 0;

        int numCars = *(unsigned short *)(gstate + 0x735C);

        for (int i = 0; i < numCars; ++i) {

            struct CarStruct *car = &car_array[i];

            func_8008B0D8(car, arg0[6], 15);

        }

        return result;

    } else {

        int result = 0;

        int numCars = *(unsigned short *)(gstate + 0x735C);

        for (int i = 0; i < numCars; ++i) {

            struct CarStruct *car = &car_array[i];

            func_8008AE8C(car, arg0[6], 15);

        }

        return result;

    }

}









// === func_8008B474 ===



void func_8008B474(int a0, int a1) {

    int v0;

    float f4, f6, f8, f10, f12, f18;



    gstate = 0x801146EC; // Assuming this is a global variable



    func_8008B424();



    f4 = *(float *)a0;

    f6 = *(float *)(a0 + 4);

    f8 = *(float *)(a0 + 8);



    *(float *)a1 = f4;

    *(float *)(a1 + 4) = f6;

    *(float *)(a1 + 8) = f8;



    func_8008B474(a3, a2);

}



void func_8008B4C4(int a0, int a1) {

    float f2, f4, f6, f8, f10, f12, f18;

    int v0;



    gstate = 0x801146EC; // Assuming this is a global variable



    func_8008B474(a3, a2);



    f4 = *(float *)a0;

    f6 = *(float *)(a0 + 4);

    f8 = *(float *)(a0 + 8);



    *(float *)a1 = f4;

    *(float *)(a1 + 4) = f6;

    *(float *)(a1 + 8) = f8;



    f10 = *(float *)a3;

    f12 = *(float *)a3;

    f18 = *(float *)a3;



    func_8008B3C8();



    *(float *)(a2 + 4) = f4;

    *(float *)a2 = f10;



    *(float *)(a2 + 4) = f6;

    *(float *)(a2 + 8) = f8;





// === func_8008B4C4 ===



void func_8008B4C4(void* arg0, void* arg1) {

    float f4 = ((float*)arg0)[0];

    int a3 = (int)arg0;

    int a2 = (int)arg1;



    ((float*)arg1)[2] = f4;

    float f6 = ((float*)arg0)[2];

    ((float*)arg1)[4] = f6;

    float f8 = ((float*)arg0)[4];

    ((float*)arg1)[5] = f8;



    int a0 = (int)arg1 + 24;

    ((float*)arg1)[7] = f8;



    ((int*)sp)[36/4] = (int)&func_8008B4C4;

    ((double*)sp)[5] = ((double*)arg2)[22];

    ((double*)sp)[4] = ((double*)arg2)[20];



    func_8008B474(arg1);



    arg1 = (void*)((int)arg0 + 24);

    a3 = ((int*)sp)[36/4];

    a2 = ((int*)sp)[40/4];



    float f10 = ((float*)a3)[2];

    ((float*)arg0)[4] = f10;

    ((float*)arg0)[0] = f10;



    f6 = ((float*)a3)[0];

    ((float*)arg0)[2] = f6;

    func_8008B3C8(arg0);



    float* v0 = (float*)(0x8012 << 16 | 0x594);

    a2 = ((int*)sp)[44/4];

    v0 += 0x8011 << 16;

    float f4_2 = v0[0];

    ((float*)a2)[0] = f4_2;

    float f6_2 = v0


// === func_8008B660 ===



void func_8008B660(void *arg) {

    u32 sp = (u32)arg - 24;

    func_8008B4C4(arg);

    *(float *)(*(arg) + 36) = __fmove_d1_f12();

    *(float *)(*(arg) + 40) = __fmove_d1_f14();

    *(float *)(*(arg) + 44) = __fmove_d1_f16();

}



void func_8008B69C(void *arg, u16 arg2) {

    u32 sp = (u32)arg - 40;

    int t8 = *(short *)(*(arg + 1) + 8);

    int t6 = arg2 << 16;

    int t7 = t6 >> 16;

    int t9 = t8 << 4;

    t9 -= t8;

    t9 <<= 3;

    u32 addr1 = (0x8015 << 16) + 10264;

    if (t7 != 0) {

        addr1 = *(u32 *)(*(arg + 1) + 8);

        u8 t8_2 = (*(u32 *)0x801146EC >> 3) & 1;

        if (!t8_2) {

            *(short *)(*(arg + 1) + 6) = 0;

            float *v1 = (float *)(addr1 + *(short *)(*(arg + 1) + 8));

            " : "=r"(v1[3]));

            " : "=r"(v1[2]));

            " : "=r"(v1[1]));

            func_8008B64


// === func_8008B69C @ 0x8008B69C ===

void func_8008B69C(void *arg0, void *arg1) {
    u16 t6 = ((u16)arg0 << 16) >> 16;
    u32 car_index = (t6 * 4) + *(u32 *)arg1;

    if (car_index >= CAR_COUNT) {
        return;
    }

    CarData *car_data = &car_array[car_index];
    f32 f16, f14, f12;

    func_8008B640(&car_data->unk_0xB4);

    if (*(u32 *)arg1 >= 100) {
        // Code related to handling large values of arg1
        // Additional logic here...
    }

    u32 v1 = ((u32)arg1 << 16) >> 16;

    if (v1 < 100) {
        // Code related to handling small values of arg1
        // Additional logic here...
    }
}



// === func_8008B964 @ 0x8008B964 ===

void func_8008B964(void *arg0, int arg1, int arg2) {
    \n"
        "sw      $s2, 72($sp)\n"
        "sw      $s1, 68($sp)\n"
        "sw      $s0, 64($sp)\n"
        "sdc1    $f24, 56($sp)\n"
        "sdc1    $f22, 48($sp)\n"
        "sdc1    $f20, 40($sp)\n"
        "sw      %0, 84($sp)\n"

        "lh      $v0, 8(%1)\n"
        "sll     $t4, %2, 16\n"
        "sra     $t6, $t4, 16\n"
        "or      $t4, $t6, $zero\n"

        "sll     $t9, $v0, 8\n"
        "lui     $t6, 0x8015\n"
        "addu    $t9, $t9, $v0\n"
        "sll     $t9, $t9, 3\n"

        "li      $t6, -23984\n"
        "addu    $a3, $t9, $t6\n"

        "lwc1    $f4, 1008($a3)\n"

        "sll     $t7, $v0, 4\n"
        "subu    $t7, $t7, $v0\n"
        "sll     $t7, $t7, 3\n"

        "lui     $t8, 0x8015\n"
        "subu    $t7, $t7, $v0\n"
        "li      $t8, 10264\n"
        "addu    $t1, $t7, $t8\n"

        "sll     $t8, %2, 16\n"
        "sra     $t9, $t8, 16\n"

        "slti    $v1, $t9, 13\n"
        "xori    $t2, $v1, 0x0001\n"
        "or      $t3, %1, $zero\n"

        "beq     $t2, $zero, 0x8008BA10\n"

        "lb      $t0, 1601($a3)\n"
        "sltu    $t2, $zero, $t0\n"
        "bne     $t2, $zero, 0x8008BA10\n"

        "lw      $t2, 232($t1)\n"
        "andi    $t6, $t2, 0x0800\n"
        "sltu    $t2, $zero, $t6\n"

        "beq     $t0, $zero, 0x8008BA28\n"

        "or      $s1, $t3, $zero\n"

        "lui     $at, 0x4000\n"
        "i-type  op=0x11\n"
        "beq     $zero, $zero, 0x8008BA30\n"

        "lwc1    $f18, 216($t1)\n"
        "lwc1    $f0, 1348($a3)\n"
        "lwc1    $f18, 216($t1)\n"
        "lwc1    $f16, 220($t1)\n"

        "i-type  op=0x11\n"
        "i-type  op=0x11\n"
        "li      $at, 0x4000\n"
        "i-type  op=0x11\n"

        "sll     $a0, $t4, 16\n"
        "sra     $t8, $a0, 16\n"

        : 
        : "r"(arg0), "r"(arg1), "r"(arg2)
    );
}



// === func_8008BA84 @ 0x8008BA84 ===

void func_8008BA84(void *arg) {
    u32 sp = (u32)arg - 80;
    \n"
        "sw      $s2, 72($sp)\n"
        "sw      $s1, 68($sp)\n"
        "sw      $s0, 64($sp)\n"
        "sdc1    $f24, 56($sp)\n"
        "sdc1    $f22, 48($sp)\n"
        "sdc1    $f20, 40($sp)\n"
        "sw      $a1, 84($sp)\n"
        :
        : "r" (sp)
    );

    CarData *car = car_array + ((u32)arg & 0xFFFF);
    s16 lap_index = ((s16)arg >> 16) & 0xFFFF;
    u32 lap_offset = lap_index << 8;
    u32 lap_base = 0x80152818 + lap_offset;
    f32 car_speed = *(f32 *)(lap_base + 1008);
    s32 checkpoint_index = ((s16)arg >> 4) & 0xF;
    u32 checkpoint_offset = checkpoint_index << 3;
    u32 checkpoint_base = lap_base + 10264 + checkpoint_offset;
    s32 ai_difficulty = *(s8 *)(checkpoint_base + 194);

    // Rest of the function logic
}



// === func_8008BB8C @ 0x8008BB8C ===

void func_8008BB8C(void* a0, int a1) {
    u32 sp = 0x80000000 - 80;
    u32 ra = (u32)a0 + 76;
    u32 s2 = 0;
    u32 s1 = 0;
    u32 s0 = 0;
    f64 f24, f22, f20;
    CarData* car = (CarData*)((u32)a0 + 84);
    u8 state = (u8)a1;
    u16 lap_count = state >> 16;
    s16 lap_check = lap_count & 0xFFFF;

    \n"
        "sw      %1, 72($sp)\n"
        "sw      %2, 68($sp)\n"
        "sw      %3, 64($sp)\n"
        "sdc1    $f24, 56($sp)\n"
        "sdc1    $f22, 48($sp)\n"
        "sdc1    $f20, 40($sp)\n"

        "lwc1    $f4, 1008(%4)\n"
        "lui     $t6, 0x8015\n"
        "addu    $t9, %4, $v0\n"
        "sll     $t9, $t9, 3\n"
        "subu    $t6, $t6, -23984\n"
        "addu    $t0, $t9, $t6\n"

        "lui     $t6, 0x8015\n"
        "subu    $t7, $v0, $v0\n"
        "sll     $t7, $t7, 3\n"
        "addiu   $t8, $t8, 10264\n"

        "jal     func_8008B69C\n"

        : "+r"(sp), "+r"(ra), "+r"(s2), "+r"(s1)
        : "r"(a0), "r"(state), "r"(lap_count), "m"(car->dr_pos[0]), "m"(car->dr_pos[1]), "m"(car->dr_pos[2])
    );
}



// === func_8008BC94 @ 0x8008BC94 ===

void func_8008BC94(void* arg0, u16 arg1) {
    \n"
        "sw      $s2, 72($sp)\n"
        "sw      $s1, 68($sp)\n"
        "sw      $s0, 64($sp)\n"
        "sdc1    $f24, 56($sp)\n"
        "sdc1    $f22, 48($sp)\n"
        "sdc1    $f20, 40($sp)\n"
        "sw      %0, 84($sp)\n"

        "lh      $v0, 8($a0)\n"
        "sll     $t3, $a1, 16\n"
        "sra     $t6, $t3, 16\n"
        "or      $t3, $t6, $zero\n"

        "sll     $t9, $v0, 8\n"
        "lui     $t6, 0x8015\n"
        "addu    $t9, $t9, $v0\n"
        "sll     $t9, $t9, 3\n"

        "addiu   $t6, $t6, -23984\n"
        "addu    $t0, $t9, $t6\n"

        "lwc1    $f4, 1008($t0)\n"

        "sll     $t7, $v0, 4\n"
        "subu    $t7, $t7, $v0\n"
        "sll     $t7, $t7, 3\n"

        "lui     $t8, 0x8015\n"
        "subu    $t7, $t7, $v0\n"
        "addu    $a2, $t7, $t8\n"

        "sll     $t8, $a3, 16\n"
        "sra     $t9, $t8, 16\n"
        "slti    $v1, $t9, 21\n"
        "xori    $t1, $v1, 0x0001\n"

        "beq     $t1, $zero, 0x8008BD30\n"

        "or      $t2, $a0, $zero\n"
        "lw      $t1, 232($a2)\n"
        "andi    $t6, $t1, 0x2000\n"
        "sltu    $t1, $zero, $t6\n"

        "lui     $at, 0x8012\n"
        "lwc1    $f10, 14488($at)\n"
        "lwc1    $f8, 212($a2)\n"
        "lui     $at, 0x4000\n"

        "i-type  op=0x11\n"
        "lwc1    $f4, 1348($t0)\n"

        "i-type  op=0x11\n"
        "lwc1    $f10, 216($a2)\n"

        "i-type  op=0x11\n"
        "sll     $a0, $t3, 16\n"
        "sra     $t8, $a0, 16\n"
        "lwc1    $f24, 220($a2)\n"

        "or      $a0, $t8, $zero\n"
        "or      $s1, $t2, $zero\n"
        "or      $a1, $t1, $zero\n"

        "addiu   $s2, $zero, 194\n"
        "jal     0x8008B69C\n"

        "i-type  op=0x11\n"
        "lw      $ra, 76($sp)\n"
        "ldc1    $f20, 40($sp)



// === func_8008BD9C @ 0x8008BD9C ===

int func_8008BD9C(void* arg0, int arg1) {
    \n"
        "sw      $s2, 72($sp)\n"
        "sw      $s1, 68($sp)\n"
        "sw      $s0, 64($sp)\n"
        "sdc1    $f24, 56($sp)\n"
        "sdc1    $f22, 48($sp)\n"
        "sdc1    $f20, 40($sp)\n"
        "sw      %1, 84($sp)\n"

        "lh      $v0, 8(%0)\n"
        "sll     $t3, %1, 16\n"
        "sra     $t6, $t3, 16\n"
        "or      $t3, $t6, $zero\n"

        "sll     $t9, $v0, 8\n"
        "lui     $t6, 0x8015\n"
        "addu    $t9, $t9, $v0\n"
        "sll     $t9, $t9, 3\n"

        "addiu   $t6, $t6, -23984\n"
        "addu    $t0, $t9, $t6\n"

        "lwc1    $f4, 1008($t0)\n"
        "sll     $t7, $v0, 4\n"
        "subu    $t7, $t7, $v0\n"
        "sll     $t7, $t7, 3\n"

        "lui     $t8, 0x8015\n"
        "subu    $t7, $t7, $v0\n"
        "addu    $a2, $t7, $t8\n"

        "sll     $t8, %2, 16\n"
        "sra     $t9, $t8, 16\n"
        "slti    $v1, $t9, 21\n"
        "xori    $t1, $v1, 0x0001\n"

        "beq     $t1, $zero, 0x8008BE38\n"

        "or      $t2, %0, $zero\n"
        "lw      $t1, 232($a2)\n"
        "andi    $t6, $t1, 0x0100\n"
        "sltu    $t1, $zero, $t6\n"

        "lui     $at, 0x8012\n"
        "lwc1    $f10, 14492($at)\n"
        "lwc1    $f8, 188($a2)\n"
        "lui     $at, 0x4000\n"

        "i-type  op=0x11\n"
        "lwc1    $f4, 1164($t0)\n"
        "i-type  op=0x11\n"
        "lwc1    $f10, 192($a2)\n"
        "i-type  op=0x11\n"

        "sll     $a0, $t3, 16\n"
        "sra     $t8, $a0, 16\n"
        "lwc1    $f24, 196($a2)\n"
        "or      $a0, $t8, $zero\n"

        "or      $s1, $t2, $zero\n"
        "or      $a1, $t1, $zero\n"
        "addiu   $s2, $zero, 194\n"

        "jal     0x8008B69C\n"

        "i-type  op=0x11\n"
        "lw      $ra, 76($sp)\n"
        "ldc1    $f20, 40($sp)\n"
    );
}



// === func_8008C688 @ 0x8008C688 ===

#include <stdint.h>

void func_8008C688() {
    \n"
        "jal func_8008C5E0\n"
        "nop\n"
        "beq $zero, $zero, 0x8008C714\n"
        "lw $ra, 20($sp)\n"
        "lwc1 $f6, 14568(0x8012)\n"
        "lui $at, 0x3F80\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"
        "jal func_8008C5E0\n"
        "lwc1 $f8, 14572(0x8012)\n"
        "beq $zero, $zero, 0x8008C710\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"
        "jal func_8008C5E0\n"
        "i-type op=0x11\n"
        "lwc1 $f6, 14576(0x8012)\n"
        "nop\n"
        "beq $zero, $zero, 0x8008C710\n"
        "lw $ra, 20($sp)\n"
        "addiu $sp, $sp, 24\n"
        "jr $ra\n"
        "nop\n"
    );
}

void func_8008C724() {
    \n"
        "jal func_8008C680\n"
        "nop\n"
        "beq $zero, $zero, 0x8008C75C\n"
        "lw $ra, 20($sp)\n"
        "jal func_8008C680\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"
        "lw $ra, 20($sp)\n"
        "addiu $sp, $sp, 24\n"
        "jr $ra\n"
        "nop\n"
    );
}

void func_8008C76C() {
    \n"
        "jal func_8008C5E0\n"
        "nop\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"
        "jal func_8008C5E0\n"
        "nop\n"
        "beq $zero, $zero, 0x8008C868\n"
        "lw $ra, 20($sp)\n"
        "jal func_8008C5E0\n"
        "nop\n"
        "addiu $sp, $sp, 24\n"
        "jr $ra\n"
        "nop\n"
    );
}



// === func_8008C724 @ 0x8008C724 ===

void func_8008C724() {
    \n"
        "j 0x8008C680\n"
        "nop\n"
        "beq $zero, $zero, 0x8008C75C\n"
        "lw $ra, 20($sp)\n"
        "j 0x8008C680\n"
        "nop\n"
        "lwc1 $f0, 0x38f4($at)\n"
        "beq $zero, $zero, 0x8008C75c\n"
        "lwc1 $f0, 0x38f8($at)\n"
        "j 0x8008C680\n"
        "nop\n"
        "lwc1 $f0, 0x38fc($at)\n"
        "beq $zero, $zero, 0x8008C75c\n"
        "lwc1 $f0, 0x3900($at)\n"
        "j 0x8008C680\n"
        "nop\n"
        "j 0x8008C720\n"
        "nop\n"
    );
}

void func_8008C76C() {
    \n"
        "j 0x8008C680\n"
        "nop\n"
        "beq $zero, $zero, 0x8008C868\n"
        "lw $ra, 20($sp)\n"
        "j 0x8008C720\n"
        "nop\n"
    );
}



// === func_8008C76C @ 0x8008C76C ===

void func_8008C76C(void) {
    \n"
        "li      $t0, 10\n" // Assuming this is the result of some calculations
        "j       0x8008C868\n"
    );
}

void func_8008C884(void) {
    \n"
        "sw      $s1, 40($sp)\n"
        "sw      $s0, 36($sp)\n"
        "sw      $a1, 188($sp)\n"
        "lh      $v1, 8($a0)\n"
        "lui     $t5, 0x8016\n"
        "lb      $t5, 5032($t5)\n"
        // Continue with the assembly code for func_8008C884
    );
}



// === func_8008C884 @ 0x8008C884 ===

void func_8008C884(u16 *arg0) {
    u16 race_number = arg0[0];
    u8 controller_flags = *(u8 *)0x801603A8;
    u32 offset = (race_number << 4) - race_number;
    offset <<= 3;
    u32 address = (offset + 0x1000) * 9;
    u32 arg1 = arg0[1];
    u32 game_config = *(u32 *)0x8012FFD4;
    u8 controller_state = *(u8 *)(arg1 - 0x5E70);
    u32 car_data_address = (u32)arg0;

    if (controller_flags != 0) {
        *(u32 *)(car_data_address + 0x281C) = game_config;
        offset = race_number << 8;
        address = ((race_number << 8) - race_number) * 9;
        u32 ai_control_flags = *(u32 *)0x8015FAC8;
        u8 controller_input = arg1[9];
        u32 previous_ai_config = *(u32 *)(car_data_address + 0x281C);

        if (controller_input & 1) {
            u8 dpad_right = (arg1[7] >> 6) & 1;
            u8 brake_state = (arg0[2] & 1) | (arg0[2] << 2);
            *(u32 *)(car_data_address + 0x281C) = previous_ai_config;
        }
    }
}



// === func_8008D120 @ 0x8008D120 ===

void func_8008D120(void* arg0, int arg1) {

    u32 sp = (u32)arg0;
    s16 car_index = ((s16)arg1) << 16 >> 16;
    s16 track_segment = (car_index * 4 + 8) & 0xFFFF;
    u16* track_array = (u16*)0x80152818;
    CarData* car_data = (CarData*)arg0;
    GameStruct* game_struct_ptr = (GameStruct*)0x801461D0;
    f32* track_data = (f32*)((u32)track_array + track_segment * 4);
    s16 race_state = arg1;

    if (race_state != 0) {
        u32 segment_address = ((track_array[track_segment] & 0xFFFF) << 2) | 0x8015;
        u16* track_data_ptr = (u16*)segment_address;
        f32 dr_pos_x = track_data_ptr[0];
        f32 dr_pos_y = track_data_ptr[1];
        f32 dr_pos_z = track_data_ptr[2];
        car_data->dr_pos[0] = dr_pos_x;
        car_data->dr_pos[1] = dr_pos_y;
        car_data->dr_pos[2] = dr_pos_z;
    } else {
        u32 segment_address = ((track_array[track_segment] & 0xFFFF) << 2) | 0x8015;
        u16* track_data_ptr = (u16*)segment_address;
        f32 dr_vel_x = track_data_ptr[4];
        f32 dr_vel_y = track_data_ptr[5];
        f32 dr_vel_z = track_data_ptr[6];
        car_data->dr_vel[0] = dr_vel_x;
        car_data->dr_vel[1] = dr_vel_y;
        car_data->dr_vel[2] = dr_vel_z;
    }
}



// === func_8008D6B0 @ 0x8008D6B0 ===

void func_8008D6B0(float *src, float *dst) {
    src[0] = dst[0];
    src[1] = dst[1];
    src[2] = dst[2];
    src[3] = dst[3];
    src[4] = dst[4];
    src[5] = dst[5];
}

void func_8008D714(float *arg0, float *arg1, int arg2) {
    if (arg1 != NULL) {
        arg0[9] = arg1[0];
        arg0[10] = arg1[1];
        arg0[11] = arg1[2];
        arg0[12] = arg1[3];
    }
    if (arg2 != 0) {
        func_8008D6B0(arg0, arg1);
    }
}

void func_8008D764(float *arg0, float *arg1, int arg2) {
    float temp[5];

    func_800088F0();
    temp[0] = arg1[0];
    func_80008730();

    temp[1] = arg1[1];
    func_800088F0();
    temp[2] = arg1[2];
    func_80008730();

    temp[3] = arg1[3];
    func_800088F0();
    temp[4] = arg1[4];
    func_80008730();

    *(float *)(*(float **)arg2 + 16) = temp[2];
    *(float *)(*(float **)arg2 + 20) = temp[3];
    *(float *)(*(float **)arg2 + 24) = temp[4];
}



// === func_8008D714 @ 0x8008D714 ===

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

void func_8008D714(void *a0, void *a1, void *a2) {
    \n"
        "sw      $a0, 24($sp)\n"
        "beq     %1, $zero, 0x8008D744\n"
        "lw      %0, -6392(%0)\n"
        "lwc1    $f4, 0(%1)\n"
        "swc1    $f4, 36(%0)\n"
        "lwc1    $f6, 4(%1)\n"
        "swc1    $f6, 40(%0)\n"
        "lwc1    $f8, 8(%1)\n"
        "swc1    $f8, 44(%0)\n"
        "beq     %2, $zero, 0x8008D754\n"
        "or      $a0, %2, $zero\n"
        "jal     0x8008D6B0\n"
        "or      $a1, %0, $zero\n"
        "lw      $ra, 20($sp)\n"
        "addiu   $sp, $sp, 24\n"
        "jr      $ra\n"
    );
}

void func_8008D764(void *a0, void *a1) {
    \n"
        "sw      $s1, 32($sp)\n"
        "sw      $s0, 28($sp)\n"
        "sdc1    $f20, 16($sp)\n"
        "or      $s0, %0, $zero\n"
        "or      $s1, %1, $zero\n"
        "jal     0x800088F0\n"
        "lwc1    $f12, 0(%1)\n"
        "li      $t0, 65536\n"
        "jal     0x80008730\n"
        "lwc1    $f12, 0($s1)\n"
        "swc1    $f0, 56($sp)\n"
        "jal     0x800088F0\n"
        "lwc1    $f12, 4(%s1)\n"
        "lwc1    $f12, 4(%s1)\n"
        "jal     0x80008730\n"
        "swc1    $f0, 52($sp)\n"
        "lwc1    $f12, 8(%s1)\n"
        "jal     0x800088F0\n"
        "swc1    $f0, 48($sp)\n"
        "lwc1    $f12, 8(%s1)\n"
        "jal     0x80008730\n"
        "swc1    $f0, 44($sp)\n"
        "lwc1    $f16, 48($sp)\n"
        "lwc1    $f18, 56($sp)\n"
        "lwc1    $f2, 44($sp)\n"
        "lwc1    $f14, 52($sp)\n"
        "li      $t0, 32768\n"
        "swc1    $f18, 28($s0)\n"
        "li      $t0, 16384\n"
        "swc1    $f6, 12($s0)\n"
        "jal     0x80008730\n"
        "swc1    $f10, 24($s0)\n"
    );
}



// === func_8008D764 @ 0x8008D764 ===

void func_8008D764(void* arg0, void* arg1) {
    CarData* car = (CarData*)arg0;
    f32 temp_f;

    \n"
        " jal     0x80008730\n"
        "swc1    $f0, 56($sp)\n"
        "jal     0x800088F0\n"
        "lwc1    %1, 4(%1)\n"
        " jal     0x80008730\n"
        "swc1    $f0, 52($sp)\n"
        "jal     0x800088F0\n"
        "lwc1    %1, 8(%1)\n"
        " jal     0x80008730\n"
        "swc1    $f0, 48($sp)\n"
        "jal     0x800088F0\n"
        "lwc1    %1, 8(%1)\n"
        " jal     0x80008730\n"
        "swc1    $f0, 44($sp)\n"
        "jal     0x80008730\n"
        "lw      %2, 56($sp)\n"
        "jal     0x80008730\n"
        "swc1    $f6, 0(%2)\n"
        "jal     0x80008730\n"
        "lw      %2, 52($sp)\n"
        "jal     0x80008730\n"
        "swc1    $f8, 24(%2)\n"
        " jal     0x80008730\n"
        "lw      %2, 56($sp)\n"
        "jal     0x80008730\n"
        "swc1    $f6, 4(%2)\n"
        " jal     0x80008730\n"
        "lw      %2, 60($sp)\n"
        "jal     0x80008730\n"
        "swc1    $f4, 20(%2)\n"

        : "=f"(temp_f), "=m"(*car)
        :
        : "$ra", "$s0", "$s1", "$t7"
    );
}



// === func_8008D8D8 @ 0x8008D8D8 ===

void func_8008D8D8(int arg0, int arg1) {
    if (arg1 >= 0) return;

    int ra = *(int*)20;
    func_8008B26C();
    int arg3 = t0;

    if (s0 == 0) goto label_8008D918;

    a1 = 1;
    t6 = 1;
    lh(arg0, 6);
    func_8008B0D8();
    move(a1, zero);

    if (s0 == 0) goto label_8008D92C;

    v0 = s0 | 0;
    t7 = 1;
    lh(arg0, 6);
    func_8008AE8C();
    lw(v0, zero);

    if (s0 == 0) goto label_8008D92C;

label_8008D918:
    lw(ra, 20);

label_8008D92C:
    addiu(sp, sp, 24);
    jr(ra);

void func_8008D93C(int arg0, int arg1) {
    int sp = -224;
    t6 = a1 << 16;
    t7 = t6 >> 16;
    ra = *(int*)28;
    s0 = *(int*)24;
    a1 = *(int*)228;

    if (t7 != 0) goto label_8008D984;

    t0 = arg0;
    lh(arg0, 6);
    sw(t0, 224);
    a1 = 1;
    func_8008AE8C();
    a2 = 15;
    lw(t0, 224);
    t8 = -1;
    sw(zero, 20);

    if (s0 == 0) goto label_8008E058;

    sh(t8, 6);

label_8008D984:
    lh(v1, 8);
    lh(a2, 4);
    lui(a0, 0x8014);
    sll(t9, v1, 8);
    lb(a0, 1048);
    lui(t6, 0x8015);
    addu(t9, t9, v1);
    sll(t9, t9, 3);
    addiu(t6, t6, -23984);
    addu(t1, t9, t6);
    sll(t3, a2, 2);
    addu(t7, t1, t3);
    sltu(t8, zero, a0);
    lwc1(f2, 1484);

    if (t8 != 0) goto label_8008D9EC;

    sll(t9, v1, 4);
    subu(t9, t9, v1);
    sll(t9, t9, 3);
    subu(t9, t9, v1);
    sll(t9, t9, 3);
    lui(a0, 0x8015);
    addu(a0, a0, t9);
    lw(a0, 10496);
    andi(t6, a0, 0x0008);
    sltu(a0, zero, t6);

label_8008D9EC:
    sll(t8, v1, 4);
    subu(t8, t8, v1);
    sll(t8, t8, 3);
    subu(t8, t8, v1);
    sll(t8, t8, 3);
    lui(t9, 0x8015);
    addiu(t9, t9, 10264);
    sll(t8, t8, 3);
    sw(a0, 124);

label_8008E058:
    jr(ra);
}



// === func_8008D93C @ 0x8008D93C ===

void func_8008D93C(u32 car_index, u16 arg1) {
    s32 stack = (s32)((u32)&gstate + 224);
    u32 temp6 = arg1 << 16;
    u32 temp7 = temp6 >> 16;
    u32 ra_save = stack + 28;
    u32 s0_save = stack + 24;
    u32 a1_save = arg1;

    if (temp7 != 0) {
        u32 temp0 = car_index | 0;
        u16 v1 = *(u16*)((u32)&car_array + (car_index & 0xFFFF) * 8);
        a1_save = 1;
        func_8008AE8C(0x8014, 15);

        if (v1 == 0xFFFF) {
            memset(stack + 20, 0, 4);
        } else {
            *(u16*)(stack + 6) = v1;
        }
    }

    u16 v1 = *(u16*)((u32)&game_struct + (car_index & 0xFFFF) * 8);
    u32 temp9 = v1 << 8;
    u32 a0_base = 0x8014;
    u32 temp6_2 = 0x8015 - 23984;
    u32 temp1 = (temp9 + temp6_2) << 3;
    u32 temp3 = arg1 << 2;
    u32 temp7_2 = temp1 + temp3;
    u32 temp8 = v1 >> 0;

    if (*(f32*)(temp7_2 + 1484) != 0.0f) {
        temp8 = 0;
    }

    u32 temp9_2 = v1 << 4;
    u32 temp6_3 = temp9_2 - (v1 << 1);
    u32 temp7_3 = temp6_3 << 3;

    u32 a0_value = *(u16*)(0x8015 + 10264);
    u32 temp6_4 = *(u32*)&a0_value & 0x0008;
    u32 a0_flag = (temp6_4 != 0);

    u32 temp9_3 = v1 << 4;
    u32 temp8_2 = temp9_3 - (v1 << 1);
    u32 temp7_4 = temp8_2 << 3;

    if (*(u8*)(temp7_4 + 8) == 2) {
        u32 v0 = *(u32*)((u32)&car_array + (car_index & 0xFFFF) * 8);
        u16 v6 = *(u16*)((u32)&game_struct + (v0 & 0xFFFF) * 8);
        u16 temp5 = *(u16*)((u32)&car_array + (v0 & 0xFFFF) * 8 + 248);
        u8 temp9_4 = *(u8*)(temp7_4 + 8);
        u8 at = 2;

        if (temp5 >> 2 != at) {
            u16 v2 = *(u16*)((u32)&car_array + (v0 & 0xFFFF) * 8 + 232);
            u16 temp6_5 = *(u16*)((u32)&game_struct + (v0 & 0xFFFF) * 8 + 1990);
            u16 temp7_5 = temp6_5 + 1;

            if (temp7_5 != 0) {
                *(u16*)(stack + 130) = temp7_5;
            } else {
                *(u16*)(stack + 130) = 0;
            }
        }

        u8 temp8_3 = *(u8*)(temp7_4 + 8);
        u32 a2_base = 0x8011;
        u32 temp9_5 = temp8_3 << 2;
        u32 a2_value = a2_base + (temp9_5 << 3);
        u32 temp6_6 = (a2_value < 1);

        if (temp6_6 != 0) {
            u32 v1 = *(u32*)((u32)&car_array + (car_index & 



// === func_8008E0C8 @ 0x8008E0C8 ===

void func_8008E0C8(float *arg0) {
    float f4 = arg0[3];
    float f6 = *(float *)(car_array + (int)*gstate);
    float f8 = arg0[7];

    arg0[0] = f10;
    arg0[1] = f4;
    *(float *)(arg0 + 2) = f6;
    arg0[3] = f8;
}

void func_8008E19C(int arg0, int arg1) {
    int a2 = arg0 >> 16;
    int t7 = arg1 << 16;
    int a1 = t7 >> 16;

    if (a1 >= 0) {
        void *ra = __builtin_return_address(0);
        __builtin_save_reg(ra, 20);

        int v0 = *(int *)(0x8016 - 19884);
        if (v0 >= 0) {
            __builtin_return_address(0);
            int t6 = arg2 & 0xFFFF;
            int t9 = v0 * 4;
            int t10 = *(int *)(t1 + t9);
            __builtin_call((void (*)(void))t10, 0);
            *(short *)(v0 + 26) = a2;
            int t9 = v0 << 4;
            int t10 = *(int *)(t1 + t9);
            __builtin_call((void (*)(void))t10, 0);
            *(short *)(t10 + 26) = t6;
        }
    } else {
        void *ra = __builtin_return_address(0);
        __builtin_save_reg(ra, 24);

        int v0 = *(int *)(0x8016 - 19884);
        if (v0 >= 0) {
            *(short *)(v0 + 26) = a2;
        }
    }
}



// === func_8008E19C @ 0x8008E19C ===

#include <stdint.h>

void func_8008E19C(uint32_t arg0, uint32_t arg1) {
    uint32_t sp = 0;
    int16_t a2 = (arg0 << 16) >> 16;
    int16_t t7 = (arg1 << 16) >> 16;
    int16_t t6 = a2 >> 16;
    a2 = t6 | (t6 << 16);
    void *ra;

    sp -= 24;
    *(int32_t *)(sp + 28) = arg1;
    ra = *(void **)(sp + 20);

    if (arg1 >= 0) {
        *(int32_t *)(sp + 24) = arg0;
        int16_t a0 = *(int16_t *)0x8016B254;
        if (a0 < 0) {
            goto func_8008E25C;
        }
    } else {
        *(uint16_t *)(*(int32_t *)0x8016B254) = t6;
        func_8008E144();
        *(uint16_t *)(sp + 26) = a2;
        *(int32_t *)sp = (int32_t)func_8008E280;
        sp += 40;
        goto func_8008E25C;
    }

    int32_t t0 = (arg1 << 4) + arg1;
    uint32_t t1 = 0x80136400 + (t0 << 2);
    int16_t a0 = *(int16_t *)(t1 + 22);
    if (a0 == -1) {
        goto func_8008E25C;
    }
    *(uint16_t *)(t1 + 22) = a2;
    func_8008E144();
    *(uint16_t *)(sp + 26) = a2;
    t2 = (int32_t)(t1 << 4);
    *(int32_t *)sp = (int32_t)func_8008E280;
    sp += 40;

func_8008E25C:
    ra = *(void **)(sp + 20);
    sp += 24;
}



// === func_8008E280 @ 0x8008E280 ===

void func_8008E280(u32 arg0, u32 arg1, u32 arg2, u32 arg3, s16 arg4) {
    CarData* car_array = (CarData*)0x80152818;
    GameStruct* game_struct = (GameStruct*)0x801461D0;

    u32 t5 = 0;
    u32 t4 = arg0;
    u32 t3 = arg1;
    u32 t2 = arg3;
    s32 a3;
    CarData* car;
    u16 at;

    if (arg4 > 0) {
        for (a3 = 0; a3 < arg4; a3++) {
            u32 index = ((game_struct->time << 16) | game_struct->current_lap_time);
            if (index != a3) {
                car_array += 8;
            }
        }

        car = &car_array[arg3];
        at = (game_struct->time << 16) | game_struct->current_lap_time;

        car->place = t7;
        car->RWV[0] = 0;
        car->mph = t3;
        car->rpm = t4;
        car->dr_uvs[0][2] = arg2;
        car->dr_uvs[1][2] = arg2;
        car->dr_uvs[2][2] = arg2;
        car->dr_vel[0] = 0;
        car->dr_pos[0] = 0;
        car->RWV[1] = 0;
        car->RWV[2] = 0;
        car->dr_uvs[0][1] = 0;
        car->dr_uvs[0][0] = 0;
        car->dr_uvs[1][0] = 0;
        car->dr_uvs[1][1] = 0;
        car->dr_uvs[2][0] = 0;
        car->dr_uvs[2][1] = 0;

        func_8008E19C(car->place, at);
    }
}



// === func_8008E398 @ 0x8008E398 ===

void func_8008E398(u32 param) {
    u16 index = param >> 16;
    CarData* car = &car_array[index];
    s32 value = func_8008E26C(index);

    car->place = (s8)index;
    car->checkpoints_completed = 0;
    car->address = (u32)car;
    car->next_address = (u32)car;

    if (!gstate_prev->is_initialized) {
        u8 game_state_value = gstate_prev->game_state_value;
        gstate_prev->game_state_flag = (game_state_value << 16) | (game_state_value >> 16);
        return;
    }

    void* ptr = (void*)((u32)&game_struct + 0x92D8 - (u32)&game_struct);
    u16 config_value = *(u16*)((u32)ptr + 0xF8);

    switch (param & 0xFF) {
        case 1:
            config_value <<= 2;
            break;
        case 2:
            config_value <<= 2;
            car->next_address = 2;
            break;
        case 3:
            config_value <<= 2;
            car->next_address = 3;
            break;
        default:
            if (param & 0xFF == 4) {
                *(u32*)((u32)ptr + 0xFC) = param;
            }
            break;
    }

    if (car->next_address != 1 && config_value & 0x100) {
        config_value |= 0x100;
    }

    *(u16*)((u32)ptr + 0xF8) = config_value;
}



// === func_8008E440 @ 0x8008E440 ===

void func_8008E440(u32 arg0, u32 arg1, u32 arg2) {
    s32 sp = (s32)&arg2 - 256;
    s32 t7 = (arg0 >> 2) << 16;
    s32 t6 = t7 >> 16;
    s32 t0 = arg2;
    s32 t8 = ((u32*)0x8014)[(t7 >> 16) * 4];
    u32 a3 = arg1 << 2;
    void* ra = (void*)&arg2 + 20;

    if (arg1 != 0) {
        *((s16*)&sp + 9) = t6;
        if (arg1 == 1) {
            t7 = a3;
        } else if (arg1 == 2) {
            t7 += a3 * 2;
        } else if (arg1 == 3) {
            t7 += a3 * 4;
        }
    }

    u32 v0 = *(u32*)((u32*)t8 + (t7 >> 16));
    u32 v1 = *((u8*)&v0 + 1);
    if ((v1 & 0x01) != 0) {
        v1 |= 0x01;
    }
    if ((v1 & 0x02) != 0) {
        v1 |= 0x02;
    }
    if ((v1 & 0x04) != 0) {
        v1 |= 0x04;
    }
    if ((v1 & 0x08) != 0) {
        v1 |= 0x08;
    }

    *(u32*)((u32*)t8 + (t7 >> 16)) = v1;

    t8 = ((u32*)0x8014)[(t7 >> 16)];
    a3 <<= 2;
    *((s16*)&sp + 1) = (t7 >> 16);
    func_8008E3C0(arg0, arg1, arg2, ra);

    u32 t4 = *(u32*)((u32*)t8 + (t7 >> 16));
    if (t4 != 0) {
        v1 = *((u32*)&t4 + 15);
        f32 f8;
        *(f32*)&f8 = *((u32*)&v1 + 3);
        *((f32*)((u32*)t4 + 64)) = f8;
        *((s16*)((u32*)t4 + 84)) = *((s16*)&sp + 1);
    }

    return;
}



// === func_8008EA10 @ 0x8008EA10 ===

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
CarData car_array[8];
GameStruct game_struct = 0x801461D0;
u32 frame_counter = 0x80142AFC;
PadEntry pad_array[4];

void func_8008EA10(s32 arg0, s32 arg1) {
    u32 car_index = (arg0 << 16) >> 16;
    CarData *car = &car_array[arg1];
    u32 car_offset = car_index * sizeof(CarData);
    u32 state_flags = ((u32*)game_struct)[4 + (car_offset >> 2)];

    // Additional logic here...
}



// === func_80090088 @ 0x80090088 ===

int func_80090088(u8 car_index, u8 arg1, s32 arg2) {
    if (car_index != 0) {
        CarData *car = &car_array[car_index];
        car->checkpoint = -1;

        int game_field_offset = *(game_struct + arg2);
        int car_pos_offset = game_field_offset + 19884;
        int car_rpm = *(car_pos_offset + (arg2 * 4));

        if (car_rpm != 0) {
            car->RWV[0] = 0;

            if (arg1 == 0) {
                CarData *current_car = &car_array[car_index];
                current_car->checkpoint = arg2;
            }

            func_8008FFD0(car);
        }
    }

    return 0;
}



// === func_80090228 @ 0x80090228 ===

int func_80090228(void *arg0) {
    int result = ((func_80090088(arg0 >> 16)) << 16);
    return result;
}

int func_80090310(void *arg0, CarData *car_array, GameStruct *game_struct, int car_index) {
    if (func_80090254(arg0) == 0) {
        return 0;
    }

    game_struct->car_array[car_index].data_valid = func_80090284(car_index);

    if (game_struct->car_array[car_index].data_valid != 0) {
        game_struct->car_array[car_index].data_valid++;
    } else {
        game_struct->car_array[car_index].data_valid = car_array[car_index];
    }

    car_array[car_index] = game_struct->car_array[car_index].data_valid;

    return 1;
}



// === func_80090254 @ 0x80090254 ===

void func_80090254(u32 car_index) {
    s32 result = func_80090088(car_index, 0);
    if (!result) return;

    CarData *car_info = &car_array[car_index];
    u32 game_struct_value = *(u32 *)(game_struct + 0x90);
    if (game_struct_value == 0) return;

    s16 speedometer_index = 0x8013E66C - game_struct_value;
    s16 speedometer_value = car_info[speedometer_index] + 1;
    car_info[speedometer_index] = speedometer_value;

    if (speedometer_value > car_info[0x8013E678 - game_struct_value]) {
        u32 temp_buffer_offset = 274 * 2 + 274 * 2;
        u16 speedometer_old_value = *(u16 *)(temp_buffer_offset + 2);
        car_info[speedometer_index] = speedometer_new_value;

        *(u8 *)temp_buffer_offset = (u8)speedometer_new_value;
        *(u8 *)(temp_buffer_offset + 1) = (u8)speedometer_old_value;
        *(u32 *)(game_struct + 0x90) = 0;
        *(u32 *)(game_struct + 0x94) = 0;
        *(u32 *)(game_struct + 0x96) = -1;
        *(u32 *)(game_struct + 0x98) = 0;
        *(u32 *)(game_struct + 0x9C) = 0;
        *(u32 *)(game_struct + 0xA0) = 0;
    }
}

void func_80090310(u32 arg0) {
    s32 result = func_80090284();
    if (!result) return;

    u32 temp_buffer_offset = 274 * 2 + 274 * 2;
    f32 value1 = *(f32 *)(car_array + 17356);
    f32 value2 = 244.0f;
    f32 value3 = value1 - value2;

    *(u32 *)(game_struct + 0x8C) = (u32)temp_buffer_offset;
    *(u32 *)(game_struct + 0xA0) = *(u32 *)&value3;
    u16 speedometer_value = *(u16 *)temp_buffer_offset;
    *(u8 *)(temp_buffer_offset + 1) = (u8)speedometer_value;
}



// === func_80090310 @ 0x80090310 ===

void func_80090310(u16 *arg0) {
    s32 temp_v0;
    f32 temp_f4, temp_f6, temp_f8, temp_f10;
    u16 temp_t6, temp_t7, temp_t8;
    u32 temp_s0, temp_s1, temp_s2, temp_s3, temp_s4, temp_s5, temp_s6, temp_s7;

    \n"
        "jal     0x80090284\n"
        "sw      $t6, 212($sp)\n"
        "beq     $v0, $zero, 0x8009071C\n"
        "sw      $v0, 268($sp)\n"
        "lui     $t7, 0x8009\n"
        "addiu   $t7, $t7, 4076\n"
        "sw      $t7, 20($v0)\n"
        "lui     $at, 0x8015\n"
        "lwc1    $f4, 17356($at)\n"
        "addiu   $t9, $zero, 244\n"
        "lui     $at, 0x8012\n"
        "swc1    $f4, 16($v0)\n"
        "lh      $t8, 274($sp)\n"
        "addiu   $t6, $zero, 205\n"
        "addiu   $t7, $zero, 20\n"
        "sh      $t8, 8($v0)\n"
        "lh      $t8, 274($sp)\n"
        "sb      $t9, 212($sp)\n"
        "lwc1    $f30, 14760($at)\n"
        "lui     $at, 0x4040\n"
        "sll     $t9, $t8, 2\n"
        "subu    $t9, $t9, $t8\n"
        "sb      $t6, 213($sp)\n"
        "sb      $t7, 214($sp)\n"
        "sll     $t9, $t9, 3\n"
        "lwc1    $f26, 14764($at)\n"
        "lui     $t6, 0x8015\n"
        "addu    $t9, $t9, $t8\n"
        "sll     $t7, $t8, 4\n"
        "sll     $t9, $t9, 4\n"
        "addiu   $t6, $t6, 18016\n"
        "lwc1    $f24, 14768($at)\n"
        "addu    $s0, $t9, $t6\n"
        "lui     $t9, 0x8015\n"
        "lui     $at, 0x4700\n"
        "subu    $t7, $t7, $t8\n"
        "sll     $t7, $t7, 3\n"
        "addiu   $t9, $t9, 10264\n"
        "lui     $s2, 0x8014\n"
        "lui     $s7, 0x0004\n"
        "lui     $s4, 0x41C6\n"
        "lui     $s1, 0x8011\n"
    );
}



// === func_8009079C @ 0x8009079C ===

#include <stdint.h>

#define CAR_ARRAY_SIZE 15
#define GSTATE_INIT 0x40000
#define GSTATE_SETUP 0x80000

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

typedef struct GameStruct {
    void* next;
    CarData* current_car;
} GameStruct;

extern CarData car_array[CAR_ARRAY_SIZE];
extern uint8_t gstate_prev;
extern CarData* game_struct;
extern uint32_t frame_counter;
extern uint16_t pad_array[];

void func_8009079C(void* arg0) {
    int16_t value = *(int16_t*)arg0;
    CarData** carArray = (CarData**)&car_array;

    if (value > 0) {
        value--;
        func_8008AE8C(value, 15);
    }

    arg0->prev = -1;
    arg0->next = -1;

    GameStruct* gameStructPtr = (GameStruct*)game_struct;
    CarData* currentCar = gameStructPtr->current_car;

    if (currentCar != NULL && *(int32_t*)&currentCar == (int32_t)arg0) {
        gameStructPtr->current_car = carArray[(*(int32_t*)&gameStructPtr->current_car + value) % CAR_ARRAY_SIZE];
    }

    CarData* temp = (CarData*)carArray;
    for (int i = 0; i < CAR_ARRAY_SIZE; i++) {
        if (*(int32_t*)&temp[i] == (int32_t)arg0) {
            temp[i] = carArray[(i + value) % CAR_ARRAY_SIZE];
            break;
        }
    }

    uint8_t* gStatePtr = (uint8_t*)gstate_prev;
    if (*gStatePtr != 0 && *(CarData**)(*(int32_t*)gStatePtr) == arg0) {
        CarData* nextCar = carArray[(*(int32_t*)gStatePtr + value) % CAR_ARRAY_SIZE];
        *(CarData**)(*(int32_t*)gStatePtr) = nextCar;
    }

    for (int i = 0; i < CAR_ARRAY_SIZE; i++) {
        if (*(CarData**)(carArray + i) == arg0) {
            carArray[i] = carArray[(i + value) % CAR_ARRAY_SIZE];
            break;
        }
    }

    uint8_t* gStatePtr2 = (uint8_t*)gstate_prev;
    if (*gStatePtr2 != 0 && *(CarData**)(*(int32_t*)gStatePtr2) == arg0) {
        CarData* nextCar2 = carArray[(*(int32_t*)gStatePtr2 + value) % CAR_ARRAY_SIZE];
        *(CarData**)(*(int32_t*)gStatePtr2) = nextCar2;
    }
}

void func_800908A0(void* arg0, int arg1) {
    CarData** carArray = (CarData**)&car_array;
    uint16_t index = (arg1 << 16) >> 16;
    *(int32_t*)&arg0->place = index;

    carArray[index] = carArray[(index + value) % CAR_ARRAY_SIZE];
}



// === func_800908A0 @ 0x800908A0 ===

void func_800908A0(u32 arg0) {

    u32 *gstate = (u32 *)gstate;
    s32 idx = (arg0 >> 16);
    u32 temp_idx = (idx << 16);
    s32 local_a1 = 0;

    \n"
        "sw $ra, 108(%sp)\n"
        "sw $fp, 104(%sp)\n"
        "sw $s7, 100(%sp)\n"
        "sw $s6, 96(%sp)\n"
        "sw $s5, 92(%sp)\n"
        "sw $s4, 88(%sp)\n"
        "sw $s3, 84(%sp)\n"
        "sw $s2, 80(%sp)\n"
        "sw $s1, 76(%sp)\n"
        "sw $s0, 72(%sp)\n"
        "sdc1 $f30, 64(%sp)\n"
        "sdc1 $f28, 56(%sp)\n"
        "sdc1 $f26, 48(%sp)\n"
        "sdc1 $f24, 40(%sp)\n"
        "sdc1 $f22, 32(%sp)\n"
        "sdc1 $f20, 24(%sp)\n"
        "jal 0x80090088\n"
        "move $a2, $zero\n"
        "lw %0, 116(%sp)\n"
        "sh $t9, 6(%0)\n"
        "beqz $zero, 0x80090B24\n"
        "lw $ra, 108(%sp)\n"
        "lh %1, 8(%0)\n"
        "lui $t7, 0x8015\n"
        "bltz %1, 0x80090B24\n"
        "sll $t6, %1, 8\n"
        "addu $t6, $t6, $a1\n"
        "sll $t6, $t6, 3\n"
        "addu $t7, $t7, $t6\n"
        "lb %2, -22384($t7)\n"
        "lui $t1, 0x8015\n"
        "addiu $a1, $zero, 952\n"
        "i-type op=0x14\n"
        "lh $a0, 6(%0)\n"
        "addu %3, $t1, $t8\n"
        "lwc1 $f4, 8($3)\n"
        "r-type funct=0x19\n"
        "r-type funct=0x12\n"
        "lwc1 $f6, 12($3)\n"
        "r-type funct=0x19\n"
        "r-type funct=0x12\n"
        "lw %4, 8($3)\n"
        "swc1 $f4, 36(%t4)\n"
        "lwc1 $f6, 12($4)\n"
        "r-type funct=0x19\n"
        "r-type funct=0x12\n"
        "lw %5, 8($4)\n"
        "swc1 $f6, 40(%t5)\n"
        :
        : "r"(local_a1), "r"(temp_idx), "r"(idx), "r"((u32 *)car_array), "r"((u32 *)game_struct)
        : "$ra", "$fp", "$s7", "$s6", "$s5", "$s4", "$s3", "$s2", "$s1", "$s0"
    );
}



// === func_80090B70 @ 0x80090B70 ===

void func_80090B70(u32 arg0) {
    u16 *car_index = (u16 *)arg0;
    s16 car_num = (*car_index >> 16) & 0xFFFF;
    s16 car_offset = (*car_index & 0xFFFF) << 8;
    car_offset += *car_index;
    car_offset <<= 3;

    if (*(char *)(gstate + car_offset - 22384)) {
        return;
    }

    ((void (*)(u32, u32, s16, s16, s16))0x80090088)(car_array, car_num, *car_index, car_offset, car_offset + 8);

    if (arg1) {
        ((void (*)(u32, u32, s16, s16, s16))0x8009079C)((u32)arg1, 1, *car_index, car_offset, car_offset + 8);
    } else {
        ((void (*)(u32, u32, s16, s16, s16))0x8009079C)(car_array, 952, *car_index, car_offset, car_offset + 8);

        *(f32 *)(gstate + car_offset + 40) = *(f32 *)(game_struct + 10264);
        *(f32 *)(gstate + car_offset + 44) = *(f32 *)(game_struct + 10264 + 4);
    }
}



// === func_80090EA4 @ 0x80090EA4 ===

void func_80090EA4(void *arg) {
    f32 speed;
    void (*jal_func1)(void);
    void (*jal_func2)(void);

    jal_func1 = (void (*)(void))0x80008730;
    jal_func2 = (void (*)(void))0x800088F0;

    \n"
        "jal %0\n"
        "swc1 $f12, 32($sp)\n"
        "lwc1 $f12, 32($sp)\n"
        "jal %1\n"
        "swc1 $f0, 24($sp)\n"
        "lwc1 $f16, 24($sp)\n"
        "li $v1, 0\n"
        "lw $v0, 36($sp)\n"
        "li $a0, 3\n"
        "lwc1 $f2, 24($v0)\n"
        "lwc1 $f12, 0($v0)\n"
        "addiu $v1, $v1, 1\n"
        "i-type op=0x11\n"
        "lw $v0, 36($sp)\n"
        "addiu $a0, $v0, 4\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"
        "swc1 $f14, -4($v0)\n"
        "i-type op=0x11\n"
        "bne $v1, $a0, 0x80090EF8\n"
        "swc1 $f6, 20($v0)\n"
        "lw $ra, 20($sp)\n"
        "addiu $sp, $sp, 32\n"
        "jr $ra\n"
        :
        : "r" (jal_func1), "r" (jal_func2)
    );
}



// === func_80090F4C @ 0x80090F4C ===

void func_80090F4C(void) {
    float f6;
    int ra = *func_80090F50();
    int a1 = *func_80090F54();

    f6 = *(float*)(gstate + 14812);

    func_80008730();
    float f12 = *(float*)sp[32];
    func_800088F0();
    float f0 = *(float*)sp[24];
    float f16 = *(float*)sp[24];

    int v1 = 0;
    int v0 = *(int*)func_80090F5C() + 3;

    for (int i = 0; i < 3; ++i) {
        float f2 = *(float*)(v0 + 12);
        float f12 = *(float*)(v0 + 24);

        v1++;
        v0 += 4;
    }

    *(float*)(v0 + 8) = f16;

    return ra;
}

void func_80090FEC(void) {
    int sp[64];
    memset(sp, 0, sizeof(sp));

    int t7 = *(int*)func_80090FF4();
    int s4 = (a1 << 16) >> 16;
    int t6 = a1 >> 16;
    int s5 = a0 | 0;
    int s4 = t6 | 0;

    int ra = sp[27];
    int fp = sp[28];
    int s7 = sp[29];
    int s6 = sp[30];
    int s5 = sp[31];
    int s4 = sp[32];
    int s3 = sp[33];
    int s2 = sp[34];
    int s1 = sp[35];
    int s0 = sp[36];

    double f30, f28, f26, f24, f22, f20;

    sdc1(f30, 64);
    sdc1(f28, 56);
    sdc1(f26, 48);
    sdc1(f24, 40);
    sdc1(f22, 32);
    sdc1(f20, 24);

    if (t7 != 0) {
        sp[104] = a1;

        short t8 = *(short*)(a0 + 8);
        int s7 = 0x8015;
        s7 += 0x18016;
        int t9 = (t8 << 2) - t8 - ((t8 << 3) - t8 - ((t8 << 4) - t8));
        int t0 = 0x8003;
        t0 -= 5228;

        int v0 = s7 + t9;
        float f4 = *(float*)(v0 + 392);
        float f6 = *(float*)t0;
    }
}



// === func_80090FEC @ 0x80090FEC ===

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
    CarData car_array[8];
} GameStruct;

#define GSTATE_INIT 0x40000
#define GSTATE_SETUP 0x80000
#define GSTATE_MENU 0x100000
#define GSTATE_CAMERA 0x200000
#define GSTATE_READY 0x400000
#define GSTATE_PLAY 0x800000
#define GSTATE_CARS 0x1000000
#define GSTATE_FINISH 0x2000000

void func_80090FEC(u32 a0, u32 a1) {
    char *sp = (char *)malloc(256);

    GameStruct *gstate = *(GameStruct **)(gstate + 28924);
    gstate->car_array[a1 >> 16] = sp;

    u32 s7 = a0 & 0xFFFF;
    u32 t6 = a1 >> 16;
    char *sp64 = (char *)malloc(128);

    *(char **)(sp + 108) = sp64;
    *(int *)(sp + 100) = s7;
    *(int *)(sp + 96) = t6;

    // Save FPU registers
    \n"
        "sdc1 $f28, 56($sp)\n"
        "sdc1 $f26, 48($sp)\n"
        "sdc1 $f24, 40($sp)\n"
        "sdc1 $f22, 32($sp)\n"
        "sdc1 $f20, 24($sp)\n"
    );

    if (*(u32 *)(gstate + 28924) != 0) {
        *(int *)(sp + 260) = a1;

        u32 t8 = a0 & 0xFFFF;
        char *car_array_base = (char *)car_array + 18016;
        int t9 = (t8 << 2) - t8;
        t9 <<= 3;
        t9 += t8;
        t9 <<= 4;

        char *car_data_base = (char *)0x8003EB94 - 5228 + car_array_base[t9];
        float f4 = *(float *)(car_data_base + 392);

        \n"
            "swc1 $f8, 392(%0)\n"
        );

        int v1 = *(int *)(sp + 104);
        char *car_data_base2 = (char *)v1 + 336;
        int t1 = (v1 << 2) - v1;
        t1 <<= 3;
        t1 += v1;
        t1 <<= 4;

        char *car_data_base3 = car_array_base[t1] + v1;
        float f10 = *(float *)(car_data_base3 + 392);

        \n"
        );

        int t3 = v1 << 8;
        t3 += v1;
        t3 <<= 3;

        char *car_data_base4 = car_array_base[t3] + v1;
        *(int *)(sp + 108) = (t3 >> 5);

        \n"
        );
}



// === func_80091874 @ 0x80091874 ===

void func_80091874(u16 arg0, u16 arg1) {
    s32 sp = -80;

    u16 t6 = arg1 << 16 >> 16;

    gstate* ra = *(gstate**)(sp + 28);
    car_array* cars = (car_array*)(sp + 24);
    game_struct* game = *(game_struct**)(sp + 20);
    u16 a1 = arg0;

    if (t6 != 0) {
        u16 v1 = *(u16*)(a1 + 8);
        u16 a0 = *(u16*)(a1 + 6);
        func_8008AE8C(a0, 1, -1, 15);
        func_8008AE8C(a2, 1, -1, 15);
        game->cars[0].flags = 0;
    } else {
        u32 t9 = *(u32*)(cars + 12);
        t6 = v1 << 4;
        t7 = t6 - v1;

        if (t9 & 1) {
            t7 <<= 3;
            return;
        }

        t7 -= v1;
        u32 t8 = 0x8015 << 16 | 0x4100;
        func_8008B0D8(t7, 15);

        u32 v0 = (v1 << 4) + v1;
        v0 <<= 2;
        u32 a0 = 0x8011 << 16 | 0x418C;
        func_8008D6B0(t7, -6392);
        func_8008D6B0(a1, 16780);

        u32 t7 = *(u32*)(cars + 12);
        if (t7 & 0x10) {
            func_8008D6B0(t7, 896);
            func_8008D6B0(t7, 896);
            f4 = *(f32*)(t7 + 16);
            return;
        }
    }
}



// === func_80091C04 @ 0x80091C04 ===

void func_80091C04(void *car) {
    int sp = -(int)malloc(32);
    *(int *)(sp + 32) = car;
    int ra = (int)&func_80091C04;

    CarData **car_array = (CarData **)car_array;
    GameStruct *game_struct = (GameStruct *)game_struct;

    int ret1 = func_80007270((int *)game_struct + 25, 0);
    int arg3 = 1;
    func_80091BA8(arg3, &ret1);

    if (ret1 != 0) {
        car_array[ret1] = (CarData *)car;
    }

    char t6 = 6;
    car_array[ret1 + 1] = (CarData *)&t6;

    unsigned char t7 = ((CarData *)(car_array[ret1]))->data_valid;
    unsigned char t8 = t7 + 1;
    ((CarData *)(car_array[ret1]))->data_valid = t8;

    func_800075E0((int *)game_struct, 0);

    if (*((void **)(car_array[ret1] + 36)) != 0) {
        func_800075E0((int *)game_struct, *((void **)(car_array[ret1] + 36)));
    }

    ra = *((int *)sp + 5);
    sp += 32;
    jr(ra);
}



// === func_80091CA4 @ 0x80091CA4 ===

void func_80091CA4(void *arg) {
    u32 temp = (u32)arg;
    
    func_80091BA8();

    u8 a3 = 0;

    if (gstate->some_condition) {
        f32 f4, f6, f8, f10;

        // Load floats from memory
        f4 = gstate->f4;
        f6 = gstate->f6;
        f8 = gstate->f8;
        f10 = gstate->f10;

        // Save floats to memory
        car_array[temp].dr_pos[0] = f4;
        if (a3 == 0) {
            car_array[temp].dr_pos[1] = f6;
        }
        car_array[temp].dr_pos[2] = f8;
        if (a3 == 0) {
            car_array[temp].dr_pos[3] = f10;
        }
    }

    u32 t6 = 4;
    // Store value to memory
    car_array[temp].place = t6;

    u32 t7 = gstate->checkpoint + 1;
    gstate->checkpoint = t7;

    f32 f20;
    car_array[temp].dr_pos[0] = gstate->f4;
    car_array[temp].dr_pos[1] = gstate->f6;

    // Save float to memory
    if (a3 == 0) {
        car_array[temp].dr_pos[2] = f20;
    }
}



// === func_80091E5C @ 0x80091E5C ===

void func_80091E5C() {
    \n"
        "sw      %1, 68($sp)\n"
        "lui     $a0, 0x8014\n"
        "sdc1    $f28, 56($sp)\n"
        "sdc1    $f26, 48($sp)\n"
        "sdc1    $f24, 40($sp)\n"
        "sdc1    $f22, 32($sp)\n"
        "sdc1    $f20, 24($sp)\n"
        "addiu   $a0, $a0, 10024\n"
        "move    $a1, $zero\n"
        "li      $a2, 1\n"
        "jal     0x80007270\n"
        "swc1    $f12, 76($sp)\n"
        "lwc1    $f12, 76($sp)\n"
        "lui     $at, 0x3F80\n"
        "nop\n"
        "li      $v0, 4\n"
        "li      $v1, 1\n"
        "beq     $zero, $zero, 0x80091EE4\n"
        "nop\n"
        "li      $v0, 6\n"
        "nop\n"
        "li      $v1, 4\n"
        "beq     $zero, $zero, 0x80091EE0\n"
        "li      $v0, 6\n"
        "li      $v1, 6\n"
        "lw      %2, 72($sp)\n"
        "li      $v0, 5\n"
        "jal     0x80091CA4\n"
        "li      $v0, 6\n"
        "li      $v1, 6\n"
        "lui     $a0, 0x8014\n"
        "addiu   $a0, $a0, 10024\n"
        "move    $a1, $zero\n"
        "jal     0x800075E0\n"
        "li      $v0, 6\n"
        "lw      %1, 68($sp)\n"
        "lwc1    $f20, 24($sp)\n"
        "lwc1    $f22, 32($sp)\n"
        "lwc1    $f24, 40($sp)\n"
        "lwc1    $f26, 48($sp)\n"
        "lwc1    $f28, 56($sp)\n"
        "jr      %1\n"
        "addiu   $sp, $sp, 72\n"
        : "=r"(gstate), "=r"(game_struct), "=r"(car_array)
        :
        : "$a0", "$a1", "$a2", "$at", "$v0", "$v1"
    );
}

void func_80091F34() {
    \n"
        "sw      %1, 68($sp)\n"
        "sw      %2, 76($sp)\n"
        "lui     $a0, 0x8014\n"
        "sdc1    $f28, 56($sp)\n"
        "sdc1    $f26, 48($sp)\n"
        "sdc1    $f24, 40($sp)\n"
        "sdc1    $f22, 32($sp)\n"
        "sdc1    $f20, 24($sp)\n"
        "addiu   $a0, $a0, 10024\n"
        "move    $a1, $zero\n"
        "jal     0x800075E0\n"
        "li      $v0, 6\n"
        "lw      %1, 68($sp)\n"
        "lwc1    $f20, 24($sp)\n"
        "lwc1    $f22, 32($sp)\n"
        "lwc1    $f24, 40($sp)\n"
        "lwc1    $f26, 48($sp)\n"
        "lwc1    $f28, 56($sp)\n"
        "jr      %1\n"
        "addiu   $sp, $sp, 72\n"
        : "=r"(gstate), "=r"(game_struct), "=r"(pad_array)
        :
        : "$a0", "$a1", "$at", "$v0", "$v1"
    );
}



// === func_80091F34 @ 0x80091F34 ===

void func_80091F34(void *arg0, int arg1) {
    \n"
        "sw      $ra, 68($sp)\n"
        "sw      %1, 76($sp)\n"
        "lui     $a0, 0x8014\n"
        "sdc1    $f28, 56($sp)\n"
        "sdc1    $f26, 48($sp)\n"
        "sdc1    $f24, 40($sp)\n"
        "sdc1    $f22, 32($sp)\n"
        "sdc1    $f20, 24($sp)\n"
        "addiu   $a0, $a0, 10024\n"
        "move    $a1, $zero\n"
        "jal     0x80007270\n"
        "addiu   $a2, $zero, 1\n"
        "lui     $at, 0xC000\n"
        "i-type  op=0x11\n"
        "lw      %0, 72($sp)\n"
        "lwc1    $f28, 76($sp)\n"
        "i-type  op=0x11\n"
        "jal     0x80091CA4\n"
        "i-type  op=0x11\n"
        "addiu   $a0, $a0, 10024\n"
        "move    $a1, $zero\n"
        "jal     0x800075E0\n"
        "move    $a2, $zero\n"
        "lw      $ra, 68($sp)\n"
        "ldc1    $f20, 24($sp)\n"
        "ldc1    $f22, 32($sp)\n"
        "ldc1    $f24, 40($sp)\n"
        "ldc1    $f26, 48($sp)\n"
        "ldc1    $f28, 56($sp)\n"
        "jr      $ra\n"
        "addiu   $sp, $sp, 72\n"
        "beq     %1, $zero, 0x80092114\n"
        "lbu     $v0, 0(%0)\n"
        "or      $v1, $a1, $zero\n"
        "or      $a3, $a2, $zero\n"
        "beq     $v0, $zero, 0x80091FF0\n"
        "beq     $a2, $zero, 0x80091FE8\n"
        "lw      $v1, 0(%a1)\n"
        "beq     $zero, $zero, 0x80091FF0\n"
        "lw      $a3, 0(%a2)\n"
        "beq     $zero, $zero, 0x80091FF0\n"
        "move    $a3, $zero\n"
        "i-type  op=0x14\n"
        "lbu     $t1, 1(%0)\n"
        "lbu     $t6, 1(%a0)\n"
        "i-type  op=0x14\n"
        "lw      $a3, 8(%0)\n"
        "lw      $t0, 4($a3)\n"
        "i-type  op=0x14\n"
        "sw      $a1, 8(%0)\n"
        "beq     $v0, $zero, 0x80092024\n"
        "lw      $t7, 0($t0)\n"
        "beq     $zero, $zero, 0x80092030\n"
    );
}



// === func_8009229C @ 0x8009229C ===

void func_8009229C(void *arg0, void *arg1) {
    u32 temp_a1 = *(u32 *)game_struct;
    u32 temp_t6 = *(u32 *)(temp_a1 + 24836);
    u32 temp_t9 = arg1[12];
    u32 temp_t7 = (temp_t9 & temp_t6) | arg0[12];
    arg1[12] = temp_t7;

    u32 temp_t8 = *(u32 *)arg0;
    temp_a1 = *(u32 *)(temp_a1 + 24836);
    u32 temp_v0 = *(u32 *)arg1;
    u32 temp_t9 = temp_t8 + 1;
    u32 temp_t7 = ~temp_a1 & temp_v0;

    if (temp_t7 < temp_v0) {
        arg0[0] = arg2;
        temp_t7 = 1;
    } else {
        arg0[0] = 0;
        temp_t8 = temp_v0 + 1;
    }

    *(u32 *)0xBF800040 = 0;
    __asm volatile("swc1 $f0, 36($0)");
    __asm volatile("swc1 $f0, 40($0)");
    __asm volatile("swc1 $f0, 44($0)");
    func_8009211C(arg0);
    __asm volatile("swc1 $f6, 48($0)");

    temp_a1 = *(u32 *)game_struct + 19536;
    *(u32 *)arg0 = temp_a1;
    arg2[8] = *(u32 *)(temp_a1 + 8);
    func_80091FBC(arg0, &gstate);
    *(u32 *)0xBF80001C = 8;
    __asm volatile("swc1 $f4, 32($0)");
}

void func_80092360(void *arg0) {
    u32 temp_a0 = *(u32 *)game_struct + 10024;
    u32 temp_s1 = arg0[52];
    __asm volatile("jalr $ra, 0x80007270");
    __asm volatile("move $a1, $zero");
    __asm volatile("jalr $ra, 0x80091B00");

    u32 temp_t6 = 2;
    arg0[2] = temp_t6;

    u32 temp_t7 = *(u32 *)(arg0 + 48);
    arg0[4] = temp_t7;

    func_80092278(arg0, &gstate);

    u32 temp_v0 = *(u32 *)arg0;
    __asm volatile("sw $v0, 36($sp)");
    void *temp_arg2 = *(void **)temp_v0;
    u32 temp_v1 = arg0[40];
    __asm volatile("jalr $ra, 0x80092278");
    temp_arg2[20] = temp_v0;

    __asm volatile("sw $zero, 20($v0)");
    u32 temp_t8 = *(u32 *)arg0;
    u32 temp_a1 = *(u32 *)(temp_a0 + 8);
    __asm volatile("jalr $ra, 0x44812000");
    temp_arg2[20] = temp_v0;

    temp_t8 = *(u32 *)arg0;
    temp_a1 = *(u32 *)(temp_a0 + 8);
    __asm volatile("jalr $ra, 0x44812000");
}



// === func_80092360 @ 0x80092360 ===

void func_80092360(u32 arg0, u32 arg1) {
    s32 *sp = (s32 *)__builtin_frame_address(0);
    s32 car_index = sp[11];
    CarData *car_ptr = &car_array[car_index];
    u8 game_state = gstate;
    GameStruct *game_struct_ptr = &game_struct;

    func_80007270(game_state, 1);

    sp[14] = 0;
    func_80091B00(car_ptr);
    sp[6] = 2;

    car_ptr->rpm = 2;
    s32 temp = car_ptr->checkpoint;
    car_ptr->place = temp;

    func_80092278();

    CarData *v0 = car_ptr;
    f32 v1 = game_struct_ptr->lives - 2612;

    v0->difficulty = v1;
    v0->data_valid = 0;
    s32 t8 = v0->place;
    *(t8 + 20) = (f32)v1;

    *(t8 + 20) = 0.0f;
    t8 = v0->lap;

    s32 t9 = *(t8 + 4);
    *(t8 + 56) = t9;
    temp = *(sp + 7);
    *(t8 + 27) = temp;

    *(t8 + 16) = 1;
    s32 address = game_struct_ptr->lives - 2612;
    *(t8 + 25) = (char)*(address + t8);

    f32 f4, f6, f8;
    __builtin_memcpy(&f4, v0 + 4, sizeof(f4));
    __builtin_memcpy(&f6, v0 + 5, sizeof(f6));
    __builtin_memcpy(&f8, v0 + 6, sizeof(f8));

    s32 t10 = *(v0 + 3);
    func_800075E0(t10);

    sp[12] = t10;
    func_800075E0((game_state << 16) | (arg1 << 16));
}



// === func_800924F4 @ 0x800924F4 ===

void func_800924F4(void* arg) {
    u32 sp = (u32)__builtin_frame_address(0);

    s32 car_index = *(s32*)(sp + 68);
    s32 game_state = *(s32*)(sp + 76);
    f64 dr_pos[3] = {*(f64*)(sp + 56), *(f64*)(sp + 48), *(f64*)(sp + 40)};
    f64 dr_vel[3] = {*(f64*)(sp + 32), *(f64*)(sp + 24), *(f64*)(sp + 16)};
    s16 car_difficulty = *(u16*)((u32)arg + 8);
    u16 arg_low = (arg << 16) >> 16;
    void* car_array_ptr = arg;

    if (car_index != 0) {
        *(u16*)(sp + 230) = car_difficulty;
    } else {
        u16 arg_high = *(u16*)((u32)arg + 6);
        func_8008AE8C(arg_low, 0);
        func_8008AE8C(arg_high, 15);
        *(s32*)(sp + 20) = 0;

    label_80092B58:
        if (*(u32*)(sp + 20) == 0) {
            *(u16*)(sp + 6) = arg_high;
        }
        s32 lap_data = *(s32*)((u32)car_array_ptr + 12);
        u16 lap_flag = 1;
        u8 camera_state = 0xBF80;

        if (lap_flag == 0) {
            lap_flag <<= 3;
        } else {
            lap_flag += lap_data << 8;
            camera_state = 0x3F80;
            lap_flag += lap_flag + lap_flag - 23984;
            arg_high = lap_flag + lap_flag;
            lap_flag -= 23984;

        label_800925D8:
            f32 dr_pos_x = *(f32*)(lap_flag);
            dr_pos_x *= 208.0;
            f64 x_velocity = dr_pos_x;
            f32 dr_pos_y = *(f32*)(lap_flag + 4);
            dr_pos_y *= 212.0;
            f64 y_velocity = dr_pos_y;

        label_800925F0:
            s32 car_flags = *(s32*)((u32)car_array_ptr + 1016);
            if (car_flags != 0) {
                u16 lap_counter = *(u16*)(lap_data + 8);
                u16 camera_id = 0x8012;
                camera_id -= 19276;
                camera_state += camera_id;
                lap_flag += camera_id;

                f32 dr_pos_z = *(f32*)lap_flag;
                x_velocity -= 216.0;



// === func_80092C58 @ 0x80092C58 ===

void func_80092C58(void *a0, void *a1, int a2, int a3) {
    s32 sp = -72;
    s32 *sp_ptr = (s32 *)(sp + 4);
    s32 *sp_ptr2 = (s32 *)(sp + 8);
    s32 *sp_ptr3 = (s32 *)(sp + 12);
    s32 *sp_ptr4 = (s32 *)(sp + 16);
    s32 *sp_ptr5 = (s32 *)(sp + 20);
    s32 *sp_ptr6 = (s32 *)(sp + 24);
    s32 *sp_ptr7 = (s32 *)(sp + 28);
    s32 *sp_ptr8 = (s32 *)(sp + 32);
    s32 *sp_ptr9 = (s32 *)(sp + 36);
    s32 *sp_ptr10 = (s32 *)(sp + 40);
    s32 *sp_ptr11 = (s32 *)(sp + 44);

    *sp_ptr10 = a0;
    *sp_ptr9 = a1;
    *sp_ptr7 = a2;
    *sp_ptr6 = a3;

    if (*sp_ptr7 != 0) {
        *sp_ptr8 = 0;
        if (*sp_ptr6 != 0) {
            return;
        }
    }

    s32 *gstate = (s32 *)gstate;
    s32 *car_array = (s32 *)car_array;
    s32 game_struct = 0x801461D0;

    void (*func_ptr)(void *, void *, int, int) = (void (*)(void *, void *, int, int))(*sp_ptr11);
    *sp_ptr11 = func_ptr(car_array[*sp_ptr7], car_array[*sp_ptr9], *sp_ptr7, *sp_ptr6);

    if (*sp_ptr11 >= 0) {
        s32 t9 = *sp_ptr7 & 1;
        if (t9 != 0) {
            *sp_ptr5 = *sp_ptr8;
        } else {
            *sp_ptr5 = *sp_ptr8 - 1;
        }
    } else {
        *sp_ptr5 = *sp_ptr8;
    }

    func_ptr(gstate, car_array[*sp_ptr9], *sp_ptr7, *sp_ptr5);
}



// === func_80092E2C @ 0x80092E2C ===

void func_80092E2C(u32 arg0, u32 arg1, u32 arg2) {
    s32 sp = -160;
    s32* stack = (s32*)(sp + 8);
    s32 s2 = arg2 << 24;
    s32 s1 = arg1 << 24;
    s32 t6 = s2 >> 24;
    s32 t7 = s1 >> 24;
    s32* sp4 = (s32*)(sp + 20);
    s32 s1_or_t7 = t7;
    s32 s2_or_t6 = t6;
    s32 ra = *(stack + 12);
    s32 s5 = *(stack + 16);
    s32 s3 = *(stack + 8);
    s32 s0 = *(stack + 4);
    u32 a1 = arg1;
    u32 a2 = arg2;
    u32 a3 = arg3;

    if (arg0) {
        s4 = 0;
        u8 t8 = *((u8*)arg0);
        if (!t8) {
            a1 = arg0;
            func_80092DCC(a1, 16);
        }
    }

    CarData* car_array = (CarData*)car_array;
    GameStruct* game_struct = (GameStruct*)game_struct;

    u32 t9 = 0x8012E68;
    ((void (*)(u32, u32))t9)(gstate, car_array);
    ((void (*)(u32, u32))t9 + 1)(game_struct, car_array);
    ((void (*)(u32, u32))t9 + 4)(gstate, car_array);
    ((void (*)(u32, u32))t9 + 8)(game_struct, car_array);

    if (s4) {
        *((u8*)(sp + 68)) = s4;
        *((u8*)(sp + 69)) = 0;
        *((u8*)(sp + 70)) = 0;
        if (t8) {
            *((u8*)(sp + 71)) = t8;
        }
    }

    while (--s2_or_t6 >= 0) {
        GameStruct* game_struct = (GameStruct*)game_struct;
        s5 = 0x8014BDC;
        u8 t4 = *((u8*)s5);
        if (t4 <= s1_or_t7) {
            s5 += 1;
            s1_or_t7 = *((u8*)s5);
            s1_or_t7--;
            s2_or_t6 = s1_or_t7 << 24 >> 24;
        } else {
            s0 = s2_or_t6;
            CarData* car_array = (CarData*)car_array;
            u32* gstate = (u32*)gstate;
            ((void (*)(u32, u32))gstate[5])(game_struct + 7, s0);
            if (s0) {
                a1 = sp + 68;
                func_80092C58(a1, car_array, gstate, 88);
            }
            break;
        }
    }

    if (s0 < 0 && s1_or_t7 > 0) {
        GameStruct* game_struct = (GameStruct*)game_struct;
        s5 = 0x8014BDC;



// === func_800930A4 @ 0x800930A4 ===

void func_800930A4(u16 a0) {
    CarData *car_array = (CarData *)0x80152818;
    GameStruct *game_struct = (GameStruct *)0x801461D0;
    u16 s5 = a0;
    u16 *gstate = (u16 *)0x801146EC;

    if (a0 != 0) {
        *(u16 *)(340 + (u32)&s5 - 368) = game_struct->unk_00;
        u32 s2 = 0x80147F30 + (game_struct->unk_00 << 6);
        u16 v0 = *(u16 *)s2;

        if (v0 != 2 || (340 + (u32)&s5 - 368) < 0) {
            if ((340 + (u32)&s5 - 368) >= -128) {
                *(u8 *)(343 + (u32)&s5 - 368) = 128;
            }
            if (v0 != 6) {
                u32 s7 = s5 << 4;
                s7 -= s5;
                s7 <<= 3;
                s7 -= s5;
                s7 <<= 3;
                s7 += 0x80151F58 + (s7 * 8);
                u16 t7 = *(u16 *)(s7 + 908) & 1;
                if (t7 != 0) {
                    u16 v1 = *(u16 *)s2;
                    u8 t9 = *(u8 *)(s7 + 929);
                    *(u8 *)(343 + (u32)&s5 - 368) = t9;
                }
            }
        }

        if (*(u16 *)&gstate[0x10] == 2) {
            u32 s7 = s5 << 4;
            s7 -= s5;
            s7 <<= 3;
            s7 -= s5;
            s7 <<= 3;
            s7 += 0x801534D8 + (s7 * 8);
            u16 t6 = *(u16 *)(s7 + 908) & 1;
            if (t6 != 0) {
                u16 v1 = *(u16 *)s2;
                u16 s4 = 68;
                u32 s1 = *(u32 *)(340 + (u32)&s5 - 368);
                u16 t0 = v1 & 0xFFFF;
                t0 >>= 16;
                t0 += 0x80130000;
                u16 *v3 = (u16 *)t0;
                u16 t8 = v3[2];
                u8 t9 = v3[3];
            }
        }

    }

    asm volatile ("lw $ra, 20($sp)\n"
                 "addiu $sp, $sp, 368\n"
                 :
                 : "r"(340 + (u32)&s5 - 368)
                 : "memory");
}



// === func_80093B20 @ 0x80093B20 ===

int func_80093B20(int a0, int a1) {

    u32 sp = 0x801976A0; // Adjusted stack pointer for clarity

    \n"
        "sw      $s7, 64($sp)\n"
        "sw      $s6, 60($sp)\n"
        "sw      $s5, 56($sp)\n"
        "sw      $s4, 52($sp)\n"
        "sw      $s3, 48($sp)\n"
        "sw      $s2, 44($sp)\n"
        "sw      $s1, 40($sp)\n"
        "sw      $s0, 36($sp)\n"
        "sdc1    $f22, 24($sp)\n"
        "sdc1    $f20, 16($sp)\n"
        "sw      %0, 400($sp)\n"
        "sw      %1, 404($sp)\n"
        "lh      $a3, 6(%0)\n"
        "sll     $t6, %1, 16\n"
        "sra     $t7, $t6, 16\n"
        "sll     $t9, $a3, 4\n"
        "addu    $t9, $t9, $a3\n"
        "sll     $t9, $t9, 2\n"
        "lui     $t6, 0x8013\n"
        "addu    $t6, $t6, $t9\n"
        "lhu     $t6, -6380($t6)\n"
        "lh      $a2, 8(%0)\n"
        "bne     $t7, $zero, 0x80093BB0\n"
        "sw      $t6, 380($sp)\n"
        "bltz    $a3, 0x80093B9C\n"
        "or      $a0, $a3, $zero\n"
        "addiu   $a1, $zero, 1\n"
        "jal     0x8008AE8C\n"
        "addiu   $a2, $zero, 15\n"
        "jal     0x8008AE8C\n"
        "lw      $t7, 400($sp)\n"
        "addiu   $t8, $zero, -1\n"
        "sw      $zero, 20($t7)\n"
        "beq     $zero, $zero, 0x800947BC\n"
        "sh      $t8, 6($t7)\n"
        "0x80093BB0:\nsll     $t9, %2, 4\n"
        "subu    $t9, $t9, %2\n"
        "sll     $t9, $t9, 3\n"
        "subu    $t9, $t9, %2\n"
        "lui     $t6, 0x8015\n"
        "addiu   $t6, $t6, 10264\n"
        "sll     $t9, $t9, 3\n"
        "addu    $t5, $t9, $t6\n"
        "lw      $t8, 232($t5)\n"
        "sll     $t6, %2, 6\n"
        "sh      %2, 392($sp)\n"
        "andi    $t7, $t8, 0x0010\n"
        "lui     $t8, 0x8014\n"
        "addiu   $t8, $t8, -27872\n"
        "sltu    $t9, $zero, $t7\n"
        "addu    $ra, $t6, $t8\n"
        "lw      $t7, 20($ra)\n"
        "lw      



// === func_80094890 @ 0x80094890 ===

void func_80094890(void* a0, u32 a1) {
    if (a1 != 0) {
        func_8009079C(a0, 1);
    } else {
        GameStruct* game_struct = (GameStruct*)((u32)gstate + 0x74);
        CarData* car_array = (CarData*)car_array;
        u32 a3 = *(u32*)((u32)a0 + 0xC);

        f64 temp_f4, temp_f6, temp_f8, temp_f16, temp_f18;

        \n"
            "lwc1 $f6, 0x8003EB94\n"
            "add.d $f8, $f4, $f6\n"
            "lwc1 $f16, 16(%0)\n"
            "add.d $f18, $f8, $f16\n"
            : "=r"(temp_f4), "=r"(temp_f6), "=r"(temp_f8), "=r"(temp_f16), "=r"(temp_f18)
            : "r"(a0)
        );

        u16 temp_t0 = *(u16*)((u32)a0 + 4);
        ++temp_t0;
        *(u16*)((u32)a0 + 4) = temp_t0;

        u16 temp_t1 = *(u16*)(car_array[*(u32*)a3].unk_5A);
        if (temp_t0 != temp_t1) {
            u16 temp_t2 = car_array[*(u32*)a3].unk_88;
            u16 temp_t3 = car_array[*(u32*)a3].unk_90;

            GameStruct* temp_t5 = (GameStruct*)((u32)game_struct + 0x7530);
            u32 temp_t4 = (temp_t3 << 2) - temp_t3;
            temp_t4 <<= 4;
            temp_t1 += temp_t4;

            u16 temp_t6 = *(u16*)(temp_t1 + 18);
            if ((temp_t6 & 0x1000) != 0) {
                if ((temp_t6 & 0x2000) == 0) {
                    car_array[*(u32*)a3].unk_5A = 0;
                } else {
                    func_80090088(car_array[*(u32*)a3].unk_4E, *(u32*)a3, a1);
                }
            }

            if (temp_t6 & 0x2000) {
                temp_t4 = car_array[*(u32*)a3].unk_88;
                u16 temp_t7 = car_array[*(u32*)a3].unk_90;

                GameStruct* temp_t5 = (GameStruct*)((u32)game_struct + 0x7530);
                u32 temp_t4 = (temp_t7 << 2) - temp_t7;
                temp_t4 <<= 4;
                temp_t1 += temp_t4;

                u16 temp_t6 = *(u16*)(temp_t1 + 18);
                if ((temp_t6 & 0x



// === func_80094A54 @ 0x80094A54 ===

void func_80094A54(u32 arg0) {
    u32 s0 = 0;
    u32 s1 = 0;
    u32 s2 = 4;
    u32 s3 = -1;
    u32 s4 = 60;
    u32 s5 = 12;
    u32 v0 = 0;
    u32 a0 = 0;
    u32 t0 = arg0;
    CarData *car_array = (CarData *)0x80152818;
    CarData *current_car = car_array + 5776 / sizeof(CarData);
    u32 v1 = 0;

    if (t0 != current_car->dr_pos[0]) {
        s0 -= s3;
        while (s0 < s2) {
            u32 t7 = s0 & 3;
            if (t7 != 0) {
                u32 t8 = a0 + s5 * t7;
                *(u32 *)(v1 + s4 * (v0++)) = *(u32 *)t8;
            }
            s0++;
        }
    }

    s0 = 0;
    while (s0 < s2) {
        u32 t7 = s0 & 3;
        if (t7 != 0) {
            u32 t8 = a0 + s5 * t7;
            *(u32 *)(v1 + s4 * (s1++)) = *(u32 *)t8;
        }
        s0++;
    }
}



// === func_80094C30 @ 0x80094C30 ===

void func_80094C30(u32 arg1) {
    if (arg1 != 0) {
        CarData *car = car_array + (arg1 >> 6);
        u32 idx = (arg1 & 0x3F);

        s16 value = *(s16 *)(game_struct + 0xA108);

        if (value > 0) {
            u32 offset = ((u32)value << 2) - value;
            car += offset;
            idx += (idx * 4);
        }

        car[idx].data_valid = 0;
    }
}



// === func_80094D20 @ 0x80094D20 ===

void func_80094D20(u32 param1, u32 param2, u32 param3) {
    if (param1 == 0) {
        return;
    }

    u32 index = param1 * 32;

    CarData* car = &gstate->car_array[index];

    // Case 1: param2 & 0x80
    if (param2 & 0x80) {
        car->field_21 |= 0x80;
    } else {
        car->field_21 &= ~0x80;
    }

    // Case 3: param2 & 0x4
    if (param2 & 0x4) {
        car->field_21 |= 0x04;
    } else {
        car->field_21 &= ~0x04;
    }

    // Case 5: param2 & 0xFB
    if (param2 & 0xFB) {
        car->field_21 |= 0xFB;
    } else {
        car->field_21 &= ~0xFB;
    }

    // Case 6: param3 is not zero
    if (param3 != 0) {
        game_struct->field_24 = (u16)(param3 & 0xFFFF);
    } else {
        car->field_24 = 0xFFFF;
    }

    // Case 7: param1 is negative
    if (param1 < 0) {
        game_struct->field_25 = (u16)(param3 & 0xFFFF);
    } else {
        car->field_25 = 0xFFFF;
    }

    // Case 8: param2 is not zero
    if (param2 != 0) {
        car->field_14 = (u32)param3 & 0xFFFF;
        car->field_16 = (u32)(param3 >> 16);
    }

    // Case 9: param2 is not zero
    if (param2 != 0) {
        car->field_1A = (u32)param3 & 0xFFFF;
        car->field_1C = (u32)(param3 >> 16);
    }

    // Case 10: param2 is not zero
    if (param2 != 0) {
        game_struct->field_24 = (u16)(param3 & 0xFFFF);
        car->field_28 = (u16)(param3 >> 16) - 1;
    }
}



// === func_80094D68 @ 0x80094D68 ===

void func_80094D68(u32 index, u32 value1, u32 value2, u32 value3) {
    if (value1 != 0) {
        CarData* car = &car_array[index];
        u8* data_ptr = (u8*)car + 21;

        *data_ptr |= 4;
        u16* rpm_ptr = (u16*)((u8*)car + 22);
        if (*rpm_ptr != 2) {
            u16 temp = value3 & 0xFFFF;
            if (value2 < 0) {
                *rpm_ptr = (temp == 0) ? 1 : temp;
                car->place = value2;
            } else {
                *rpm_ptr = (temp == 0xFFFF) ? 0 : temp;
                sh_u32(index, 26, value3);
            }
            if (value1 >= 0) {
                sh_u32(index, 24, 0);
            } else {
                sh_u32(index, 24, value1);
            }
        }
    }
}



// === func_80094DB0 @ 0x80094DB0 ===

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

#define GSTATE_INIT     0x40000
#define GSTATE_SETUP    0x80000
#define GSTATE_MENU     0x100000
#define GSTATE_CAMERA   0x200000
#define GSTATE_READY    0x400000
#define GSTATE_PLAY     0x800000
#define GSTATE_CARS     0x1000000
#define GSTATE_FINISH   0x2000000

void func_80094DB0(uint32_t car_index, uint32_t arg1, uint32_t arg2) {
    CarData *car = &car_array[car_index >> 5];

    s32 value = *(s32 *)(gstate + 0x1BF0);

    u16 temp1 = arg2 & 0xFFFF;

    if (car->difficulty == 2) {
        car->difficulty = (u8)(arg2 & 0xFFFF);
        if (!arg1) {
            car->RWV[0] = arg2;
            car->RWV[1] = arg3;
        }
    } else {
        car->RWV[0] = 0;
        car->RWV[1] = arg3;
    }

    if (temp1 == 65535) {
        car->RWV[0] = 0;
        car->RWV[1] = 0;
    }

    car->difficulty = (u16)(arg2 & 0xFFFF);
    car->checkpoint = (u8)(arg2 & 0xFFFF);

}

void func_80094EC8(uint32_t car_index, uint32_t *sp) {
    u16 value = *(u16 *)&gstate[52];

    CarData *car = &car_array[value >> 5];

    car->RWV[0] = sp[1];
    car->RWV[1] = sp[2];
    car->difficulty = (sp[0] >> 16) & 0xFF;
    car->checkpoint = (u8)(sp[0] & 0xFFFF);
}



// === func_80094E00 @ 0x80094E00 ===

void func_80094E00(u32 arg0, u32 arg1) {
    if (arg1 >= 0) {
        u32 offset = arg0 << 5;
        GameStruct *game_struct_ptr = &game_struct[offset];
        game_struct_ptr->data_valid = 0;
    }

    u32 offset2 = arg0 << 5;
    GameStruct *game_struct_ptr2 = &game_struct[offset2];
    game_struct_ptr2->mph = arg1;
    game_struct_ptr2->rpm = arg2;
    game_struct_ptr2->checkpoint = arg3;

    u16 value = game_struct_ptr2->place;
    if (value == 1) {
        game_struct_ptr2->place = -1;
    }
}



// === func_80094EC8 @ 0x80094EC8 ===

#include <stdint.h>

void func_80094EC8(uint16_t *args) {
    uint32_t sp = (uint32_t)args - 40;

    *(uint32_t *)(sp + 36) = args[0];
    *(uint32_t *)(sp + 32) = (uint32_t)args;

    uint16_t controller_index = args[26];
    uint32_t t7 = 0x8014 * 0x3C0F + 0x8015 * 0x25EF;
    uint32_t t6 = controller_index << 5;
    uint16_t ra = args[2];
    uint16_t s0 = *(uint32_t *)sp;
    uint16_t t5 = args[4];
    uint16_t t4 = args[5];
    uint16_t t3 = args[6];
    uint32_t t2 = (uint32_t)args + 8;
    uint32_t t1 = (uint32_t)args + 12;
    uint16_t t0 = args[7];

    *(uint16_t *)(sp + 18) = ra;
    *(uint16_t *)(sp + 16) = t5;
    *(uint16_t *)(sp + 12) = t4;
    *(uint16_t *)(sp + 10) = t3;
    *(uint32_t *)sp = t2;
    *(uint32_t *)(sp + 4) = t1;
    *(uint16_t *)(sp + 8) = t0;

    uint16_t t8 = args[15];
    uint16_t a3 = args[14];
    uint16_t a2 = args[13];
    uint16_t a1 = args[12];
    uint16_t controller_value = args[26];

    func_80094E00(controller_value);
    *(uint16_t *)(sp + 16) = t8;

    controller_value = args[26];
    a1 = *(uint8_t *)(sp + 25);
    a2 = *(uint8_t *)(sp + 24);

    func_80094DB0(controller_value, a1, a2);

    a3 = *(uint16_t *)(sp + 18);
    controller_value = args[26];
    func_80094D68(controller_value, a3);

    a1 = *(uint8_t *)(sp + 25);
    a1 = *(int32_t *)sp;
    controller_value = args[26];

    func_80094D20(controller_value, a1);

    if (a1 < 1) {
        *(uint32_t *)(sp + 24) = s0;
        *(uint16_t *)(sp + 20) = ra;
        sp += 24;

        return controller_index;
    }

    *(uint32_t *)(sp + 24) = args[0];
    *(uint16_t *)(sp + 20) = ra;
    sp += 24;

    return controller_index;
}

void func_80094F88(uint16_t *args, uint8_t value) {
    uint32_t sp = (uint32_t)args - 24;

    *(uint32_t *)(sp + 20) = args[0];

    uint8_t controller_index = args[26];
    args[26] += value;

    func_80094EC8(args);
    *(uint32_t *)(sp + 24) = (uint32_t)args;
    args[26] = controller_index;

    *(uint16_t *)(sp + 20) = ra;
    sp += 24;

    return controller_index;
}

uint32_t func_80094FF0(uint16_t *args) {
    // Original assembly and pseudo-C code not provided for this function.
    // Implementation needed based on the context of the program.
    return 0; // Placeholder
}



// === func_80094F88 @ 0x80094F88 ===

#include <stdint.h>

#define GSTATE_INIT     0x40000
#define GSTATE_SETUP    0x80000
#define GSTATE_MENU     0x100000
#define GSTATE_CAMERA   0x200000
#define GSTATE_READY    0x400000
#define GSTATE_PLAY     0x800000
#define GSTATE_CARS     0x1000000
#define GSTATE_FINISH   0x2000000

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

extern uint8_t gstate[];
extern CarData car_array[];

uint8_t func_80094F88(uint8_t index, uint8_t value) {
    if (index != 0xFF) {
        car_array[index].data_valid = value;
        uint8_t result = func_80094EC8(index);
        return gstate[index] | result;
    } else {
        unsigned int offset = ((unsigned int)index << 2) - index + (index << 6) + (index << 2);
        uint8_t *ptr = (uint8_t *)(0x80140000 + offset);
        return ptr[1];
    }
}

void func_80094FF0(uint8_t arg0, uint8_t arg1, uint8_t arg2) {
    if (arg0 != 0) {
        unsigned int offset = ((unsigned int)arg0 << 2) - arg0 + (arg0 << 6) + (arg0 << 2);
        uint8_t *ptr1 = (uint8_t *)(0x8015A3E0 - offset);
        uint8_t oldValue = car_array[arg1].data_valid;
        if (ptr1[6] != 0) {
            func_80094EC8(arg0);
            car_array[arg1].data_valid = 0;
        } else {
            func_80094AD04();
            arg2 = (arg2 != 0) ? 1 : 0;
        }
    }
}

void func_8009508C() {
    func_8008AD04();
}



// === func_80094FF0 @ 0x80094FF0 ===


void func_80094FF0(u32 a0) {
    u32 t1 = a0;
    if (gstate_prev <= 0) {
        gstate_prev = 1;
    }
    u32 t7 = (gstate_prev << 2) + gstate_prev - (gstate_prev << 2);
    u32 v1 = *(u8*)0x8015A93C + (t7 << 2);
    CarData* a1 = car_array + (t7 * 4);
    GameStruct* a2 = game_struct;
    u8 t6 = *(u8*)0x8014B3E0;
    gstate_prev = 0;
    if (*(u8*)(v1 + 76) != t6) {
        gstate_prev = 1;
    }
    if (gstate_prev == 1) {
        a2 = gstate;
        func_80094EC8(a2, t1);
    }
    *(u8*)(t1 + 26) = gstate_prev;
}

void func_8009508C() {
    func_8008AD04();
}

int func_800950AC(u32 a0, u32 a1, u32 a2) {
    if (a2 != 0) {
        u16 v0 = (u16)a2;
    } else {
        return 0;
    }
    if (v0 == 0) {
        return 0;
    }
    while (v0--) {
        u8 t6 = *(u8*)a0;
        u8 v1 = *(u8*)a1;
        if (*(u8*)a0 != *(u8*)a1) {
            break;
        }
        a0++;
        a1++;
    }
    return (int)(*(u8*)a0 - *(u8*)a1);
}



// === func_8009508C ===

int func_8009508C(u8 *str1, u8 *str2, int length) {
    if (length != 0) {
        int index = 0;

        while (index < length) {
            char char1 = *(u8 *)str1++;
            char char2 = *(u8 *)str2++;

            if (char1 != 0 && char1 == char2) {
                --length;
            } else {
                break;
            }

            ++index;
        }

        return (int)char1 - (int)char2;
    } else {
        return 0;
    }
}


// === func_80095120 @ 0x80095120 ===

int func_80095120(u8 a0, u8 a1) {
    if (a0 == 0) return -1;
    int ra = 0;
    if (a1 != 0) {
        ra = func_800950AC(15);
    }
    return ra;
}

int func_8009515C(u32 a0, u8 a1) {
    int ra = 0;
    if (a1 != 0) {
        ra = func_80094EC8(a0);
        *(unsigned char *)(a0 + 26) = 0;
    }
    return 1;
}

int func_800951E0(u32 a0, u8 a1) {
    CarData* car = (CarData*)a0;
    int v1 = *(int *)(car->dr_pos + 16);
    if (2 != v1) {
        // Simulated instructions here
    }
    unsigned char a0_25 = *(unsigned char *)(car + 25);
    if (a0_25 != 0 && 2 == v1) {
        // Simulated instructions here
    }
    return 1;
}



// === func_8009515C @ 0x8009515C ===

int func_8009515C(u32 arg0, u32 arg1) {
    s32 t7 = (arg1 << 6);
    register int ra asm("$ra") = func_80094EC8(arg0 + t7);

    u8 t8;
    lb(&t8, arg0 + 26);
    sb(0, arg0 + 26);

    return 1;
}

int func_800951E0(u32 arg0) {
    s32 a2 = arg0;
    u32 a3 = lw(arg0 + 8);
    s32 t0 = 2;
    s32 v1 = lw(a3 + 16);
    s32 t1 = 1;
    u32 t6 = game_struct;

    if (t0 == v1) {
        register int ra asm("$ra") = func_80094EC8(t6 + 0x25088);
    }

    s32 a0 = lb(a3 + 25);

    if (a0 != 0 && t0 == v1) {
        return 1;
    }
    return 0;
}



// === func_800951E0 @ 0x800951E0 ===

int func_800951E0(void *arg0) {
    int sp = arg0;
    int ra = *(int *)(sp + 20);
    int a3 = *(int *)(arg0 + 8);
    int t0 = 2;
    int a2 = arg0;
    float v1 = *(float *)(a3 + 16);
    int t1 = 1;
    int t6 = 0x8014;

    if (t0 != v1) {
        return 0;
    }

    int *v6 = (int *)*(int *)(t6 + 25088);
    float f4 = *(float *)(a3 + 36);
    float f6 = *(float *)(0x8012 + 14948);
    float f14 = *(float *)(0x8015 + 10056);
    float f0 = *(float *)(a3 + 32);
    int at = 0x4661;
    *(int *)(at + 70) = 0;
    *(int *)(at + 72) = 0;
    *(int *)(at + 76) = 0;
    *(int *)(at + 80) = 0;
    *(int *)(at + 92) = 0;
    *(int *)(at + 112) = 0;

    if (*(unsigned char *)(a3 + 25) != 0) {
        int *v7 = (int *)*(int *)(v6);
        float f8 = *(float *)(v6 + 36);
        *(int *)(0x8012 + 14952) = *(int *)(v6 + 36);

        int a1 = 0x8011 + 592;
        int v8 = *(int *)a1;
        int t7 = (v8 << 2) | 5;
        int at1 = v8 + t7;
        *(int *)(at1 + 3840) = 1;
        *(int *)(a1 + 4) = v8 + 1;
        *(unsigned char *)(v8 + 2) = 5;

        int t9 = *(int *)(v6 + 8);
        *(int *)(v8 + 4) = t9;

        unsigned char t71 = *(unsigned char *)(t6 + 26);
        if (t71 != 0) {
            *(unsigned char *)(t6 + 26) = t71 + 1;
        }

        int a31 = 0x8011 + 592;
        int v81 = *(int *)a31;
        float f81 = *(float *)(v6 + 36);
        *(int *)(0x8012 + 14952) = *(int *)(v6 + 36);

        int a11 = 0x8011 + 592;
        int v811 = *(int *)a11;
        int t711 = (v81 << 2) | 5;
        int at11 = v81 + t711;
        *(int *)(at11 + 3840) = 1;
        *(int *)(a11 + 4) = v81 + 1;
        *(unsigned char *)(v81 + 2) = 5;

        int t91 = *(int *)(v6 + 8);
        *(int *)(v81 + 4) = t91;

        return ra;
    }

    return ra;
}



// === func_80095240 @ 0x80095240 ===

int func_80095240(char *arg3, int arg2) {
    char state_char = *(char *)(arg3 + 25);

    if (state_char == '\0') {
        return 0;
    }

    if (*(int *)t0 != v1) {
        return 0;
    }

    float f4, f6, f14, f10;

    \n"
        "lwc1 $f6, 14948(%1)\n"
        "lwc1 $f14, 10056(%1)\n"
        "lwc1 $f0, 32(%0)\n"
        : "=m" (f4), "=m" (f6), "=m" (f14), "=m" (f0)
        : "r" (arg3), "r" (&gstate)
    );

    int v0 = (*(int *)t0 == v1) ? 1 : 0;
    if (!v0) {
        return 0;
    }

    \n"
        "lwc1 $f4, 36(%0)\n"
        : "=m" (f10)
        : "r" (&gstate), "r" (arg3)
    );

    int v2 = (*(int *)t0 == v1) ? 1 : 0;
    if (!v2) {
        return 0;
    }

    *(char *)(*(int *)gstate + 592) = v0;
    *(int *)(*(int *)gstate + 588) = v2;

    char *v3 = (char *)*(int *)gstate + 15600;
    *v3 = v0;

    int v4 = *(int *)arg2 + 8;
    *(int *)(v3 + 4) = v4;

    char v5 = *(char *)(v4 + 26);
    if (v5 != '\0') {
        *(char *)(v4 + 26) = v5 + 1;
    }

    char *v6 = (char *)*(int *)gstate + 15080;
    *v6 = v2;

    int v7 = *(int *)arg2 + 8;
    *(int *)(v6 + 4) = v7;

    return 1;
}



// === func_80095360 @ 0x80095360 ===

void func_80095360(u32 car_index, u32 arg1, u32 arg2) {
    CarData* car = &car_array[car_index];
    car->place = (u8)arg2;
    *arg1 = car_index + 1;
    car->RWV[0] = *arg1;

    float temp = car->mph;
    car->dr_vel[0] = temp;
    u8 byte = (temp >> 26) & 0xFF;
    ++byte;
    temp &= ~(0xFF << 26);
    temp |= (byte << 26);
    *(u8*)(car + 17) = byte;
}

int func_800953CC(u32 car_index, u32 arg1, u32 arg2) {
    CarData* car1 = &car_array[*((u32*)arg0)];
    CarData* car2 = &car_array[*(u32*)arg1];

    if (car1->place != car2->place) {
        return 0;
    }

    func_8009211C((u32)(gstate + 6576), car_index, arg2);
    func_80091FBC((u32)(gstate + 6428), car_index, *arg1);
    func_80091FBC((u32)(gstate + 6452), arg2, *arg1);

    return 1;
}



// === func_80095380 @ 0x80095380 ===

void func_80095380(void *arg) {
    u8 value = *(u8 *)arg;
    value++;
    *(u8 *)arg = value;
}

int func_800953CC(u16 arg, u16 *gstate, u16 *car_array) {
    if (arg == 4 || arg == 2) {
        CarData *game_struct = (CarData *)0x801461D0;
        if (*(u8 *)(game_struct + 1) != 0) {
            void *player_car = *(void **)(gstate + 1);
            if (*(u8 *)player_car == 0) {
                player_car += 24928;
                void *opponent_car = *(void **)player_car;
                if (opponent_car != 0) {
                    u32 opponent_type = *(u32 *)(opponent_car + 12);
                    func_8009211C(opponent_type, player_car, opponent_car);
                    func_80091FBC(player_car, opponent_car);
                }
            }
        }
    }
    return 0;
}

int func_800954A8(void *arg) {
    int result = func_800953CC(*(u16 *)arg, *(u16 **)(arg + 2), *(u16 **)(arg + 4));
    return result;
}



// === func_800953A0 @ 0x800953A0 ===

#include <stdint.h>

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

int func_800953A0(int arg1) {
    int t8 = *(int *)(arg1 + 8);
    int t7 = *(int *)(t8 + 28);
    int t9 = *(int *)(t7 + 28);
    return t7 - t9;
}

void func_800953CC(int arg1, int arg4) {
    uint8_t v1 = *(uint8_t *)(arg4 + 8);
    if (v1 == 4 || v1 == 2) {
        int v0 = *(int *)(arg4 + 12);
        uint8_t t6 = *(uint8_t *)(v0 + 1);
        if (!t6) {
            int s3 = 0x8014;
            int s0 = *(int *)(s3 + 24928);
            int s2 = 0x8014 + 24888;
            if (s0) {
                int t7 = *(int *)(s0 + 12);
                int s1 = *(int *)(s0 + 4);
                if (t7 != v0) {
                    func_8009211C(s3, s0);
                    func_80091FBC(s2, s0);
                    *(uint8_t *)(s0 + 8) = 1;
                    func_80091FBC(s2, arg4);
                }
            }
        }
    }
}

void func_800954A8(int arg0, int arg1, int arg2) {
    int sp = -64;
    int a0 = arg0;
    int a2 = 0x8014 + 24888;
    int ra = *(int *)(sp + 44);
    int s4 = *(int *)(sp + 40);
    int s3 = *(int *)(sp + 36);
    int s2 = *(int *)(sp + 32);
    int s1 = *(int *)(sp + 28);
    int s0 = *(int *)(sp + 24);
}



// === func_800953C0 @ 0x800953C0 ===

void func_800953C0(void *arg) {
    s8 state = *(s8 *)(car_array + ((u32)arg & 0x1F));
    if (state == 4 || state == 2) {
        CarData *car = car_array + ((u32)arg & 0x1F);
        u8 place = car->place;
        if (place != 0) {
            return;
        }
        game_struct = *(GameStruct *)0x801461D0;
        pad_entry = *(PadEntry *)0x80140BF0;
        frame_counter = *(u32 *)0x80142AFC;
        u32 car_index = (u32)arg & 0x1F;
        CarData *other_car = car_array + car_index;

        if (*(CarData *)car_array != *(CarData *)other_car) {
            func_8009211C(car_array, other_car);
            func_80091FBC(car_array, other_car, 0);
            func_80091FBC(other_car, car_array, 0);
        }

        if (place != 0) {
            u32 *pad_entry = (u32 *)0x80140BF0;
            pad_entry[0] = 0;
            pad_entry[1] = 0;
        }

        func_80091FBC((CarData *)car_array, pad_entry, 0);
        func_80091FBC(pad_entry, car_array, 0);
    }
}



// === func_800953CC @ 0x800953CC ===

void func_800953CC(u32 arg0) {
    u8 v1 = *(u8*)((u8*)&arg0 + 8);

    if (v1 != 4 && arg0 != v1) return;

    u32* v0 = *(u32**)((u8*)&arg0 + 12);

    if (*v0 & 1) {
        u32 s3 = game_struct;
        u32 s0 = *(u32*)(s3 + 24928);
        if (s0 == 0) return;

        u32 t7 = *(u32*)(s0 + 12);
        u32 s1 = *(u32*)(s0 + 4);

        if (*(u8*)arg0 != t7) {
            func_8009211C(s3, s0);
            func_80091FBC(game_struct, s4);
        }

        if (s1 == 0) return;

        *(u8*)arg0 = 0;
        *(u32*)(s0 + 8) = 0;
        func_80091FBC(game_struct, s4);
    }
}

void func_800954A8(u32 arg0) {
    int sp = -64;
    u32 a0 = arg0;
    u32 a2 = game_struct + 24888;
    u32 ra = *(((u32*)arg0) + 16);

    \n"
        "lui $t0, 0x8014\n"
        "addiu $a0, $t0, 24888\n"
        "sw %2, 44(%1)\n"
        "sw %3, 40(%1)\n"
        "sw %4, 36(%1)\n"
        "sw %5, 32(%1)\n"
        "sw %6, 28(%1)\n"
        "sw %7, 24(%1)\n"
        "lw $a1, 12(%0)\n"
        "sb $0, 9($a1)\n"
        "jal 0x8009211C\n"
        "sw $v0, 52(%1)\n"
        "lw %3, 52(%1)\n"
        "li $t6, 4\n"
        "sb $t6, 8($3)\n"
        "lw $t7, 64(%1)\n"
        "sw $0, 12($3)\n"
        "nop\n"
        "nop\n"
        "nop\n"
    );

    u32 s4 = ra;
    func_800953CC(arg0);
}



// === func_800954A8 @ 0x800954A8 ===

#include <stdint.h>

void func_800954A8(void *arg0) {
    uint32_t *sp = (uint32_t*)-64 + __builtin_frame_address(0);
    sp[1] = arg0;

    CarData **car_array = car_array;
    GameStruct *game_struct = game_struct;

    uint32_t *a1 = &game_struct->some_field;
    a1[0] = 0;
    func_8009211C();

    a1 = (uint32_t*)sp[5];
    sp[8] = 4;
    a1[1] = 0;

    float speed;
    func_800953CC(&speed);
    *(float*)(a1 + 4) = speed;

    void *ra = (void*)*(uint32_t*)sp[4];
    uint32_t s0 = *(uint32_t*)sp[0];
    uint32_t s1 = *(uint32_t*)sp[1];
    uint32_t s2 = *(uint32_t*)sp[2];
    uint32_t s3 = *(uint32_t*)sp[3];
    uint32_t s4 = *(uint32_t*)sp[5];

    return ra;
}

void func_80095528(void *arg1, void *arg2, void *arg3) {
    float speed;
    uint32_t a1 = (uint32_t)arg1;

    if (arg2 != 0) {
        uint32_t a3 = (uint32_t)arg3;
        uint32_t at = 0x42FE;
        func_800953CC(&speed);
        float f10 = *(float*)(at + 14956);
        uint32_t a0 = 1;
        uint16_t a9 = *(uint16_t*)(sp[7] + 30);
        uint32_t a2 = 1;
        uint32_t a3 = 0;
        func_800953CC(&speed);
        func_800953CC(&speed);
        func_800953CC(&speed);
        func_800953CC(&speed);
        func_800953CC(&speed);
        func_800953CC(&speed);
        a0 = 1;
        uint32_t a0 = 0x8000;
        if (a0 == 0) {
            a0 |= 0x8000;
        }
        a0 = -1;
        func_800953CC(&speed);
        return ra;
    }

    void *ra = (void*)*(uint32_t*)sp[4];
    uint32_t s0 = *(uint32_t*)sp[0];
    uint32_t s1 = *(uint32_t*)sp[1];
    uint32_t s2 = *(uint32_t*)sp[2];
    uint32_t s3 = *(uint32_t*)sp[3];
    uint32_t s4 = *(uint32_t*)sp[5];

    return ra;
}



// === func_800954E0 @ 0x800954E0 ===

void func_800954E0(void* param1, int param2, int param3) {
    register int sp asm("sp");
    void* s4 = (void*)((char*)sp + 52);
    int t6 = 4;
    *(char*)((char*)s4 + 8) = t6;

    int t7 = *(int*)((char*)sp + 64);
    *(int*)((char*)sp + 12) = 0;

    func_800953CC(s4);

    float f6;
    *(float*)((char*)s4 + 16) = f6;

    int ra = *(int*)((char*)sp + 44);
    void* s0 = (void*)((char*)sp + 24);
    void* s1 = (void*)((char*)sp + 28);
    void* s2 = (void*)((char*)sp + 32);
    void* s3 = (void*)((char*)sp + 36);
    *(int*)((char*)sp + 40) = *(int*)((char*)sp + 40);
}

void func_80095528(void* param1, int param2, void* param3, int param4) {
    register int sp asm("sp");
    int ra = *(int*)((char*)sp + 20);
    float f12;
    *(float*)((char*)sp + 24) = f12;
    int a1 = *(int*)((char*)sp + 28);

    if (param2 != 0) {
        int a3 = *(int*)((char*)sp + 36);
        *(int*)((char*)sp + 36) = a3;

        func_80020494(a1, param2);

        int t7 = param2 & 0xFF;
        if (t7 != 0) {
            if ((a1 & 0x8000) == 0) {
                a1 |= 0x8000;
            } else {
                a1 -= 1;
            }
            func_80020494(a1, t7);
        }
    }

    *(int*)((char*)sp + 36) = *(int*)((char*)sp + 36);
}



// === func_80095528 @ 0x80095528 ===

void func_80095528(u32 arg0, u32 arg1, u32 arg2) {
    if (arg2 != 0) {
        float f4 = *(float*)(gstate + 24);
        u16 t7;
        float f10;

        if (*(u16*)(game_struct + 14956) == 0) {
            f10 = *(float*)(car_array + (arg1 & 0x0078) * 4);
        } else {
            f10 = arg3;
        }

        t7 = arg2 & 0xFF;
        if ((*(u16*)(car_array + (t7 >> 3) * 4) & (1 << t7 % 8)) != 0) {
            f4 -= 1.0f;
        }

        if (arg0 == 0) {
            f4 += 1.0f;
        } else {
            f4 += arg0;
        }

        func_80020494(t7, f4);
    }
}



// === func_80095600 @ 0x80095600 ===

void func_80095600(u16 car_index) {
    if (car_index == 0) {
        return;
    }

    CarData *car = &car_array[car_index];
    u32 game_state = gstate;

    while (game_state & 0x78) {
        game_state >>= 3;
    }

    if (game_state != 0) {
        u32 result = func_80020494(car_index);
        return;
    }

    u16 car_status = car->mph & 0xFF;

    if (car_status == 3) {
        // Additional code here...
    }
}



// === func_8009570C @ 0x8009570C ===

void func_8009570C(u32 a0, u32 a1) {
    s32 t0;
    f32 t1;
    u32 v0;

    if (a0 == -1) return;

    t0 = 0x8014 + (a2 & 0xFF) * 56;
    t1 = gstate->someValue;

    func_800956BC(t0, a0);

    if (v0 != 0) {
        s32 t2;
        f32 t3;

        t2 = 0x8014 + 24888 + 12;
        t3 = gstate->someOtherValue;

        func_8009211C(t2, a1);

        s32 t4;
        f32 t5;

        t4 = 0x8014 + 24888 + 12;
        t5 = gstate->anotherValue;

        gstate->someValue = t6;
        gstate->someOtherValue = t7;
    }

    s32 t6;
    f32 t7;

    t6 = 0x8014 + 24888 + 12;
    t7 = gstate->anotherValue;

    func_800953CC(t6, a1);

    gstate->someValue = t8;
}



// === func_80095720 @ 0x80095720 ===


void func_80095720(u32 arg0, u32 arg1) {

    if (arg0 != 0) {
        s32 index = arg1;
        CarData* car = &car_array[arg0 & 0xFF];
        u32 offset = (index << 2) - index + (index << 3);
        u32 car_index = offset + 8;

        func_800956BC(index, &car->RWV);

        if (car->data_valid == 0) {
            float speed = *(float*)arg1;
            u32* temp_s4 = (u32*)(game_struct + 0x730);
            *temp_s4 = 0;
            func_8009211C(game_struct, temp_s4);

            s32* temp_a1 = (s32*)(temp_s4 + 12);
            u32 temp_t6 = *(u32*)0x80145C48;

            *temp_a1 = 3;
            *temp_s4 += temp_t6;

            *(float*)(temp_s4 + 4) = 0.0f;
            *(float*)(temp_s4 + 5) = 0.0f;
            *(float*)(temp_s4 + 6) = 0.0f;

            func_800953CC((float*)arg1);

            if (func_800953CC((float*)arg1)) {
                *(float*)(temp_s4 + 7) = speed;
            }

        } else {
            *(float*)((u32)car + offset + 28) = *(float*)arg1;
        }
    }
}

void func_80095800(u32 arg0, u32 arg1) {

    s32 temp_s2 = -24;
    addiu(sp, sp, temp_s2);
    sw(ra, 20 + sp);

    u32* temp_t7 = (u32*)(game_struct + 0x93E);
    u32 temp_t6 = *(u32*)arg1;
    u32 temp_t9 = *(u32*)(20 + s0);

    s32 flag = 1;

    and(t8, t6, *temp_t7);

    sw(t8, 12 + s0);

    if (flag != temp_t9) {
        sw(0, 16 + s0);
        u32 temp_v0 = *(u32*)(64 + s0);
        lbu(temp_t7, 26 + s0);
        lb(temp_t6, 9 + temp_v0);

        if (temp_t6 != 0) {
            func_8009211C(game_struct, &s1);
        }

    } else {
        sb(0, 9 + s1);
    }

    lw(ra, 20 + sp);
    addiu(sp, sp, 24);
    jr(ra);
}



// === func_80095800 @ 0x80095800 ===

void func_80095800(void *arg) {
    s32 sp = -24;
    void *ra = arg + 20;

    u16 t7 = game_struct->unk24836;
    u16 t6 = car_array->unk12;
    u16 t9 = car_array->unk20;
    s8 s2 = 1;

    t8 = t6 & t7;

    if (s2 != t9) {
        car_array->unk12 = 0;
    }

    u32 v0 = car_array->unk40;
    s8 t7_val = arg[26];
    s8 t6_val = v0[9];

    if (t6_val != 0) {
        void *a0 = (void *)0x8014 + 0x61E8;
        void *a1 = v0;
        func_8009211C(a0, a1);
    }

    arg[9] = 0;

    a0 = (void *)0x8014 + 0x61B0;
    u32 a2 = *(u32 *)(a0 + 8);
    func_80091FBC(a0, v0, a2);

    arg[8] = s2;
    car_array->unk40 = 0;

    t7_val = arg[26];
    a1 = arg;
    func_8009211C(arg + 4, a1);

    a0 = (void *)0x8011 + 0x28C;
    u32 s1 = gstate->unk60F4;
    t8 = *(u32 *)(a0 + 8);
    func_80091FBC(a0, s1, t8);

    arg[8] = s1;

    return;
}

void func_80095924(void *arg) {
    s32 sp = -64;
    void *ra = arg + 44;
    u32 s4 = 0xFFFFFFFF;
    u32 s3 = 0xFFFFFFFF;

    car_array->unk40 = 0xFFFFFFFF;

    u32 v1 = *(u32 *)(0x8014 + 0x60F4);
    if (v1 > 0) {
        void *t8 = (void *)0x8011 + 0x28C;
        u32 a2 = *(u32 *)(t8 + 8);
        s8 a3 = 1;

        do {
            u32 t6 = *(u32 *)a2;
            v1++;
            arg[24 * (v1 - 1)] = v1 - 1;
            arg[24 * (v1 - 1) + 4] = a3;
            void *a0 = (void *)0x8014 + 0x6C48;
            a1 = arg;
            u32 t7 = *(u32 *)a2 + v1 - 1;
            func_8009211C(a0, a1);
        } while (v1 < *(u32 *)t8);

        arg[0] = v1 - 1;
    }

    return;
}



// === func_80095840 @ 0x80095840 ===

void func_80095840(u32 arg0, u32 arg1) {
    u32 *s1 = &arg1;

    if (gstate_prev == 0) {
        s1[0] += 25064;
        func_8009211C(s1[0], s1);
        s1[1] = 0;
        *(u8 *)(car_array + 9) = 0;
    }

    u32 *a0 = (u32 *)game_struct;
    u32 a2 = a0[2];
    func_80091FBC((u32)s1, s0);
    *(u8 *)(s0 + 8) = gstate;

    a0 = (u32 *)game_struct;
    a2 = a0[2];
    func_80091FBC(s1, s0);
    *(u8 *)(s0 + 8) = s1;

    u8 *sp = s0;
    sp -= 24;
    sp[8] = 0;
    u8 t7 = sp[26];
    func_8009211C((u32)s0, sp);
    u32 a0_1 = *(u32 *)(s0 + 8);
    u32 s1_1 = (u32 *)game_struct;
    u32 a2_1 = s1_1[2];
    func_80091FBC(s1, sp);
    sp[8] = s1;

    func_80095924((u32)sp + 24);

    sp[8] = 0;
}

void func_80095924(u32 arg0) {
    u32 *sp = (u32 *)arg0 - 16;
    sp -= 16;
    sp[5] = ra;
    sp[4] = s4;
    sp[3] = s3;
    sp[2] = s2;
    sp[1] = s1;

    if (arg0 != -1) {
        sp[0] = s0;
        u8 v1 = arg0 & 0xFF;
        u32 *t7 = (u32 *)game_struct + (v1 << 2);
        t7 -= v1;
        t7 <<= 3;
        t7 += (u32 *)pad_array;
        u32 t9 = *(u32 *)(t7 + 8);
        if (arg0 != t9) {
            func_8009211C(0x801461F4, sp);
            return;
        }
    }

    u32 *a0_1 = (u32 *)game_struct + 0x13C8;
    func_80091FBC(a0_1[0], a0_1[1]);
}



// === func_80095924 @ 0x80095924 ===

void func_80095924(u32 car_index) {
    if (car_index == -1) return;

    u8 v1 = (u8)(car_index & 0xFF);
    void *t7 = (void *)game_struct + 0x4C48;
    t7 += v1 * 12 + ((v1 << 3) - v1);
    u32 t9 = *(u32 *)t7;

    if (car_index != t9) return;

    void *s4 = *(void **)(sp + 52);
    *(char *)(s4 + 8) = 0;
    u32 t6 = func_8009211C(0);
    void *s4_ptr = *(void **)(sp + 52);
    u32 v1_value = *(u32 *)(sp + 48);
    t6 += 2;
    *(char *)(s4_ptr + 8) = t6;
    void *t7_ptr = (void *)game_struct + 0x4C48;
    u32 t9_value = 1;
    u32 t8 = v1_value + *(u32 *)t7_ptr;
    *(u32 *)(s4_ptr + 12) = t8;
    *(char *)(t8 + 2) = t9_value;
    void *t7_ptr2 = *(void **)(sp + 52);
    func_800953CC(0);
    *(char *)(*(int *)t7_ptr2 + 8) = 0;
}

void func_800959DC(void *s0) {
    u32 t6 = 3;
    *(u32 *)(s0 + 16) = t6;
    func_8009211C(0);
    void *t1 = s0;
    u32 t6_value = (u32)s0;
    *(char *)t6_value = *(char *)t1;
    void *t1_ptr = (void *)game_struct + 0x4C50;
    *(u32 *)(s0 + 8) = *(u32 *)t1_ptr;
    func_80091FBC(0);
    *(u32 *)(s0 + 8) = t1_ptr;
}

void func_80095A24(u32 car_index) {
    if (car_index == -1) return;

    u8 v2 = (u8)(car_index & 0xFF);
    void *t7 = (void *)game_struct + 0x4C48;
    t7 += v2 * 12 + ((v2 << 3) - v2);
    u32 t9 = *(u32 *)t7;

    if (car_index != t9) return;

    void *s4 = *(void **)(sp + 52);
    *(char *)(s4 + 8) = 0;
    u32 t6 = func_8009211C(0);
    void *s4_ptr = *(void **)(sp + 52);
    u32 v1_value = *(u32 *)(sp + 48);
    t6 += 2;
    *(char *)(s4_ptr + 8) = t6;
    void *t7_ptr = (void *)game_struct + 0x4C48;
    u32 t9_value = 1;
    u32 t8 = v1_value + *(u32 *)t7_ptr;
    *(u32 *)(s4_ptr + 12) = t8;
    *(char *)(t8 + 2) = t9_value;
    void *t7_ptr2 = *(void **)(sp + 52);
    func_800953CC(0);
    *(char *)(*(int *)t7_ptr2 + 8) = 0;
}



// === func_80095960 @ 0x80095960 ===

void func_80095960(void *car_ptr) {
    u32 car_index = *(u32 *)car_ptr;

    void **car_entry = (void **)(gstate->car_array + car_index * 4);

    if (car_ptr != *car_entry) {
        u32 *sp = alloca(64);
        sp[15] = ra;
        sp[12] = car_index;

        func_8009211C();

        u32 old_car = *car_entry;
        sp[13] = old_car;
        sp[14] = 2;

        void **v1 = (void **)car_array;
        u32 v3 = *(u32 *)(game_struct + 0x4C48);
        u32 t9 = 1;
        u32 *t8 = (u32 *)(sp[14] + v1[v3]);
        sp[16] = t8;
        sp[17] = t9;

        func_800953CC();

        *t8 = 0;
    }
}

void func_80095A24(void *car_ptr) {
    void **sp = alloca(72);
    ra = *(u32 *)&sp[17];
    sp[16] = 3;
    func_8009211C();

    u32 car_index = (u32)car_ptr & 0xFF;

    u32 *v0 = (u32 *)(game_struct + 0x4D50);
    u32 pad_index = *(u32 *)car_ptr & 0xFF;
    u32 *t7 = v0 + (pad_index << 3);

    u32 t8 = *t7;
    u32 t9 = (pad_index << 3) | t8;

    sp[14] = t9;

    func_800956BC();

    if (v0 != 0) {
        f64 f6 = *(f64 *)sp[17];
        *(u32 *)&sp[17] = 0x8014;
    }
}



// === func_800959DC @ 0x800959DC ===

void func_800959DC(void *arg0) {
    u32 t6 = 3;
    ((CarData *)arg0)->checkpoint = t6;

    func_8009211C(arg0, arg0);
    func_80091FBC(0x80142C50);

    ((CarData *)arg0)->laps = 0x80142C50;
}

void func_80095A24(void *arg0) {
    u32 at = -1;
    f32 f6, f4;

    if (arg0 != (void *)at) {
        ((CarData *)arg0)->data_valid = (u8)arg0;

        f6 = ((CarData *)arg0)->mph;
        func_800956BC(arg0, &((CarData *)arg0)->mph);

        if (*((u32 *)arg0) == 0) {
            ((CarData *)arg0)->laps = *(u32 *)(0x80146138 + 12);
            *((char *)((CarData *)arg0->laps + 9)) = 0;

            func_8009211C(arg0, &((CarData *)arg0)->mph);

            u32 t6 = ((u32)arg0 >> 2) - ((u32)arg0 & 3) << 3;
            u32 v0 = *(u32 *)t6;
            u32 t7 = v0 + (t6 << 3);
            f4 = *(f32 *)(t7 + 12);

            *((char *)((CarData *)arg0->laps + 8)) = 3;

            func_53CC(arg0, &((CarData *)arg0)->mph);
        } else {
            f4 = *(f32 *)(*(u32 *)arg0 + 24);
        }

        *(f32 *)(*(u32 *)arg0 + 24) = f4;
    }
}



// === func_80095A24 @ 0x80095A24 ===

void func_80095A24(u32 a0, u32 a1) {
    s32 sp = -72;
    :
    );

    if (a0 == -1) {
        gstate_prev = a1;
        u32 index = a1 & 0xFF;
        s32 *ptr = (s32 *)(gstate + 0x4D8) + index * 17;
        CarData *car = (CarData *)*ptr;

        if (*ptr != 0) {
            f32 speed = car->mph;
            *(u32 *)(0x80146C54) = a2;
            func_8009211C(*(s32 *)*(u32 *)(0x80146C54), 0);
        } else {
            f32 speed = gstate_prev;
            u32 index = *(u32 *)(0x80146C54) & 0xFF;
            s32 *ptr = (s32 *)(gstate + 0x4D8) + index * 17;
            CarData *car = (CarData *)*ptr;

            if (*ptr != 0) {
                car->mph = speed;
                func_800953CC(car);
            }
        }

        \n"
            : "=r"(sp)
            :
        );

        return;
    }
}



// === func_80095A80 @ 0x80095A80 ===

void func_80095A80(float v0) {
    if (v0 != 0.0f) {
        float f6 = *(float *)(sp + 19);
        unsigned int a0 = 0x80140000;
        int *a1 = *(int **)(a0 + 24888) + 3;
        *a1 |= 9;
        int a2 = sp[5];
        func_8009211C();
        int a4 = *(int *)(sp + 15);
        float f0_copy = 0.0f;
        unsigned int t9 = 3;
        unsigned int at = 0xC0000000;
        unsigned int t6 = 0x80140000;
        a1[2] = t9;
        float f0_store = *(float *)(t6 + 4852);
        sp[12] = a2 + *(int **)(t6 + 24888)[3];
        a1[3] = 0.0f;
        a1[4] = 0.0f;
        a1[5] = 0.0f;
        float f6_store = *(float *)(sp + 19);
        if (f6_store != 0.0f) {
            sp[20] = f6_store;
            func_800953CC();
            if (a4 != -1) {
                a1 = *(int **)(sp + 76);
                a0 = 0x80140000;
                unsigned int a2_copy = a0 & 0xFF;
                int *v0_copy = *(int **)(a0 + 24888) + (a2_copy << 2) - a2_copy;
                v0_copy += (a2_copy << 3);
                int t8 = *(int *)(v0_copy + 1);
                unsigned int a2_shifted = a2_copy | 0x00000000;
                func_800956BC();
                unsigned int a0_shifted = a0 + v0_copy[0];
                if (a4 != 0.0f) {
                    f6 = *(float *)(sp + 19);
                    a0 = 0x80140000;
                    a1 = *(int **)(a0 + 24888) + 3;
                    *a1 |= 9;
                    a2 = sp[5];
                    func_8009211C();
                    a4 = *(int *)(sp + 15);
                    f6_store = *(float *)(t6 + 4852);
                    sp[12] = a2 + *(int **)(t6 + 24888)[3];
                    a1[2] = t9;
                }
            }
        }
    }
}



// === func_80095B10 @ 0x80095B10 ===

#include <stdint.h>

extern uint32_t gstate;
extern struct CarData car_array[];
extern struct GameStruct game_struct;

void func_80095B10(uint32_t param) {
    uint32_t sp = 0x7FFFFFFC - 72;
    int32_t a1 = -1;
    void (*ra)(void);
    struct CarData *s4;
    struct CarData *s3;
    struct CarData *s2;
    struct CarData *s1;
    struct CarData *s0;
    uint32_t v0;
    uint32_t t6;
    uint32_t t7;
    float f6;

    if (param != a1) {
        a1 = param;
        s4 = car_array + ((a1 & 0xFF) << 2);
        v0 = *(uint32_t *)(0x801461D0 + 0x7C4);
        t6 = (a1 & 0xFF) * 5;
        t6 -= a1 & 0xFF;
        t6 <<= 3;
        t7 = v0 + t6;
        uint32_t t8 = *(uint32_t *)(t7 + 8);

        if (t8 == 0) {
            f6 = *(float *)a1;

            (*(void (*)(struct CarData *, struct GameStruct *))0x800956BC)(s4, &game_struct);

            uint32_t t9 = v0 + t6;
            s0 = car_array + ((t9 >> 2) & 0xFF);
            uint32_t a2 = (t9 >> 2) & 0xFF;

            if (a2 == 3) {
                *(uint8_t *)(s0->address + 8) = 3;
                float f0 = f6;
                (*(void (*)(struct CarData *, float))0x8009211C)(s4, f0);

                s2 = s4;
                uint32_t t6 = v0 + t6;
                s1 = car_array + ((t6 >> 2) & 0xFF);
                a2 = (t6 >> 2) & 0xFF;

                *(uint8_t *)(s1->address + 8) = 3;
                (*(void (*)(struct CarData *, struct CarData *))0x800953CC)(s4, s1);

                if (*(float *)a1 != f6) {
                    *(float *)(s2->address + 20) = f6;
                }
            } else {
                *(float *)(s0->address + 20) = f6;
            }
        } else {
            f4 = *(float *)a1;
            *(float *)(t8 + 20) = f4;
        }
    }

    ra = (void (*)(void))*(uint32_t *)(sp + 44);
    sp += 72;
    return ra();
}

void func_80095BFC(uint32_t param) {
    uint32_t sp = 0x7FFFFF



// === func_80095BA0 @ 0x80095BA0 ===

void func_80095BA0(u8 car_index) {
    u32 *gstate = (u32*)gstate;
    CarData *car_array = (CarData*)car_array;
    GameStruct *game_struct = (GameStruct*)game_struct;

    f32 value_f0;

    *(u32*)0x80095BA4 = *(u32*)0x80095BB8;
    func_800953CC(&value_f0);

    if (*(u32*)0x80095BD0 != 0) {
        car_array[car_index].mph = value_f0;
        func_800956BC(car_index, *(u32*)0x8014);
        if (*(u32*)0x80095C14 == 0) {
            float *temp_f6 = (float*)0x80095C1C;
            func_211C(temp_f6);
        }
    }

    f32 value_f4;

    *(u32*)0x80095BC4 = *(u32*)0x80095C1C;
    func_800953CC(&value_f4);

    if (*(u32*)0x80095CCC != 0) {
        car_array[car_index].mph = value_f4;
        func_800956BC(car_index, *(u32*)0x8014);
        if (*(u32*)0x80095C14 == 0) {
            float *temp_f6 = (float*)0x80095C1C;
            func_211C(temp_f6);
        }
    }
}



// === func_80095BFC @ 0x80095BFC ===

void func_80095BFC(u32 arg0, u32 arg1) {
    s32 sp = -72;
    s32 t6 = -1;

    // Save registers
    asm volatile("sw $ra, 44(%0)" : : "r" (sp));
    asm volatile("sw $s4, 40(%0)" : : "r" (sp));
    asm volatile("sw $s3, 36(%0)" : : "r" (sp));
    asm volatile("sw $s2, 32(%0)" : : "r" (sp));
    asm volatile("sw $s1, 28(%0)" : : "r" (sp));
    asm volatile("sw $s0, 24(%0)" : : "r" (sp));

    // Conditional jump
    if (arg0 != t6) {
        asm volatile("sw %0, 76(%1)" : : "r" (arg1), "r" (sp));
        u32 v0 = 0x8014;
        u32 a2 = arg0 & 0xFF;
        u32 t6 = (v0 << 2) - a2 + ((a2 << 3));
        u32 *t7 = (u32 *)(v0 + t6);
        s32 t8 = *(s32 *)(t7 + 1);

        // Call function
        asm volatile("jal %0" : : "r" (0x800956BC));

        u32 a0 = v0 + t6;
        if (*(u32 *)a0 != 0) {
            f64 f6 = *(f64 *)(sp + 76);
            u32 a0 = 0x8014 + 0x24888;
            s32 *a1 = (s32 *)(*(u32 *)a0 + 12);
            a1[9] = 0;

            // Call function
            asm volatile("jal %0" : : "r" (0x8009211C));

            s32 *s4 = *(s32 **)(sp + 60);
            u32 a2 = *(u32 *)(sp + 52);
            u32 t9 = 3;
            s32 at = 0xC000;

            // Set values
            s4[8] = t9;
            f32 *t7 = (f32 *)(0x8014 + 0x19528);
            s4[12] = *(u32 *)(t7 + 1);

            // Save registers
            asm volatile("swc1 $f0, 20(%0)" : : "r" (s4));
            asm volatile("sw %0, 16(%0)" : : "r" (s4), "r" (sp + 52));
            u32 f4 = *(u32 *)(sp + 76);

            // Call function
            asm volatile("jal %0" : : "r" (0x800953CC));

            // Save registers
            asm volatile("swc1 $f4, 16(%0)" : : "r" (s4));
        } else {
            asm volatile("swc1 $f6, 16(%0)" : : "r" (a0));
        }
    }

    // Restore registers
    asm volatile("lw $ra, 44($sp)");
    asm volatile("lw $s4, 40($sp)");
    asm volatile("lw $s3, 36($sp)");
    asm volatile("lw $s2, 32($sp)");
    asm volatile("lw $s1, 28($sp)");
    asm volatile("lw $s0, 24($sp)");
    asm volatile("addiu $sp, $sp, 72");
}



// === func_80095CC0 @ 0x80095CC0 ===

void func_80095CC0(void) {
    // Placeholder function body for func_80095CC0
}

int func_80095D04(s32 a0, s32 a1, s32 a2) {
    if (a0 == -1) {
        return 0;
    }

    u8* gstate = (u8*)gstate;
    CarData* car_array = (CarData*)car_array;
    GameStruct* game_struct = (GameStruct*)game_struct;

    s32* ptr1 = (s32*)((u8*)car_array + 24820);
    if (*ptr1 == 0) {
        return -1;
    }

    char* ptr2 = (char*)((u8*)game_struct + 19528);
    char t6 = *ptr2;

    while (t6 != 0xFF) {
        *ptr2 = t6;
        ptr2++;
        t6++;
    }

    *ptr2 = 0;
    return 0;
}



// === func_80095D04 @ 0x80095D04 ===

int func_80095D04(int a0) {
    int sp = -80;
    int at = -1;

    void* ra = *(void**)(sp + 44);
    void* s4 = *(void**)(sp + 40);
    void* s3 = *(void**)(sp + 36);
    void* s2 = *(void**)(sp + 32);
    void* s1 = *(void**)(sp + 28);
    void* s0 = *(void**)(sp + 24);

    int a1 = *(int*)(sp + 84);
    int a2 = *(int*)(sp + 88);

    if (a0 != at) {
        int t0 = a0;
        int zero = 0;

        if (!(zero == zero)) {
            int v0 = -1;
            CarData* game_struct_ptr = (CarData*)0x801461D0;
            int v1 = *(int*)(game_struct_ptr + 2);
            unsigned char t6 = *(unsigned char*)v1;
            int t7 = -1;

            while (t7 != t6) {
                v0++;
                game_struct_ptr += 32;

                if (!(zero == zero)) {
                    break;
                }

                unsigned char* v1_ptr = (unsigned char*)(game_struct_ptr + v1);
                *(v1_ptr + t7 * 2) = 1;
            }
        }
    }

    return 0;
}



// === func_80095DE0 @ 0x80095DE0 ===

void func_80095DE0(u32 arg0, s32 arg1, u32 arg2) {
    u32 t7 = arg0 | arg2;
    u32 t8 = arg1 + arg0;

    gstate[t8] = t7;

    CarData* a2 = &car_array[game_struct];

    if (a2->place < 1) {
        a2->place += 1;
    }

    game_struct++;

    s32 at = game_struct >= 1;
    if (!at) {
        arg0 += 1;
    }

    car_array[game_struct] = at ? arg0 : arg0 + 1;

    CarData* t9 = &car_array[gstate];
    game_struct = (s32)t9->place;
    arg0 = 1;

    t8 = arg0 + gstate;
    if (!arg0) {
        u32 a1 = 24;
        func_8009211C(a1, arg0);
    }

    CarData* s4 = &car_array[gstate];
    f32 f4 = *(f32*)(sp + 84);

    s4->dr_uvs[0][0] = f4;
    f6 = *(f32*)(sp + 88);
    s4->dr_uvs[1][1] = f6;

    func_800953CC(s4);

    CarData* v0 = &car_array[gstate];
    u32 ra = *(u32*)(v0 + 44);
    u32 s0 = *(u32*)(v0 + 24);
    u32 s1 = *(u32*)(v0 + 28);
    u32 s2 = *(u32*)(v0 + 32);
    u32 s3 = *(u32*)(v0 + 36);

    sp += 80;
}

void func_80095EF4(u32 arg0, u32 arg1) {
    u32 a2 = arg1 >> 2;

    if (a2 != 0) {
        u32 v1 = arg0;
        while (1) {
            car_array[v1] = arg0;
            u32 a0 = 0x7FFFBAD;
            if (v1 == a2) break;
            v1 += 1;
        }
    }

    arg0 += 24;
    u32 v1 = *(u32*)(arg0 + 8);

    if (v1 != 0) {
        while (1) {
            u32 v0 = *(u32*)(v1 + 8);
            s32 t6 = *(s32*)v0;
            s32 t7 = t6 << 2;
            s32 t8 = v1 + t7;

            if (arg1 < t8) break;
            v1 = t8;
        }
    }

    return;
}



// === func_80095F00 @ 0x80095F00 ===

#include <stdint.h>

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

uint8_t* gstate = (uint8_t*)0x801146EC;
uint8_t* gstate_prev = (uint8_t*)0x801146ED;
CarData* car_array = (CarData*)0x80152818;
void* game_struct = (void*)0x801461D0;
uint32_t* frame_counter = (uint32_t*)0x80142AFC;
void* pad_array = (void*)0x80140BF0;

int func_80095F00(int a0, int a1) {
    int v1 = *(int*)(a0 + 4);

    if (v1 != 0) {
        if (a1 < v1) {
            int t6 = *(int*)*(a0 + 8);
            int t7 = v1 * 4;
            int t8 = a1 + t7;

            if (t8 < v1) {
                int a1_ = *(int*)a1;
                int v0 = *(int*)*(a0 + 8);

                if (*(int*)(v0 + 4) != 0) {
                    int at;

                    if (a1 < *(int*)(v0 + 4)) {
                        int v1_ = *(int*)*(v0 + 4);
                        at = a1 < v1_;
                    } else {
                        at = 0;
                    }

                    if (at == 0) {
                        v0 = *(int*)*(v0 + 4);

                        if (*(int*)(v0 + 4) != 0) {
                            int t7_ = *(int*)*(v0 + 4);
                            int t8_ = a1 < t7_;

                            if (t8_) {
                                v0 = *(int*)*(v0 + 4);

                                if (*(char*)(v0 + 21) == '$') {
                                    int t9 = *(int*)*(v0 + 4);

                                    if (a1 < t9) {
                                        at = a1 < t9;
                                    } else {
                                        at = 0;
                                    }

                                    if (at != 0) {
                                        v0 = *(int*)*(v0 + 4);

                                        if (*(char*)(v0 + 21) == '$') {
                                            int t7__ = *(int*)*(v0 + 4);
                                            int at_ = a1 < t7__;
                                            return at_;
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

    return 0;
}



// === func_80095FD8 @ 0x80095FD8 ===


void func_80095FD8(void) {

    int sp = -24;

    car_array[sp + 20] = 0;

    func_80095F8C(0);

    int a1 = 0;
    int s1 = func_80095EF4();
    int v0 = func_80095EC0(a1);

    func_80095EC0(v0 + 32);

    int a0 = *(int *)v0;
    int a1 = *(int *)(v0 + 16);
    int s0 = *(int *)(v0 + 4);

    memset((char *)s0, 0, 12);

    char t6 = *(char *)(a0 + 20);
    int t7 = *(int *)s0;

    if (t7 != 0) {
        *(int *)(s0 + 8) = s0;
        *(int *)(s1 + 12) = s0;
        int t8 = *(int *)(s0 + 12);
        int t9 = *(int *)a0;
        func_80095EC0(t6 + t9, 32);
        *(int *)(s0 + 12) = t7;
    } else {
        *(int *)(s1 + 12) = s0;
        int t8 = *(int *)(s0 + 12);
        int t9 = *(int *)a0;
        func_80095EC0(t6 + t9, 32);
        *(int *)(s0 + 12) = t7;
    }

    char *ra = (char *)car_array[sp + 20];
    int t9 = *(int *)(v0 + 4);

    if (t9 != 0) {
        *(int *)(v0 + 4) = s0;
        *(int *)(s1 + 12) = s0;
        int t6 = *(int *)s0;
        int t7 = *(int *)(v0 + 12);
        func_80095EC0(t6 + t7, 32);
        *(int *)(s0 + 12) = t9;
    } else {
        *(int *)(s1 + 12) = s0;
        int t6 = *(int *)s0;
        int t7 = *(int *)(v0 + 12);
        func_80095EC0(t6 + t7, 32);
        *(int *)(s0 + 12) = t9;
    }

    car_array[sp + 20] = ra;

}

void func_800960D4(void) {

    int sp = -32;

    car_array[sp + 32] = 0;

    *(int *)(car_array[sp + 28] + 24) = car_array[sp + 32];
    func_80095FD8();

    int a1 = car_array[sp + 32];
    int s1 = 0;
    int s0 = 0;

    func_80007270(0x80151068, 0);
    func_80095FD8();
    func_80007270(0x80151068, 0);

}



// === func_800960D4 @ 0x800960D4 ===

void func_800960D4(void* arg0) {
    CarData* car = (CarData*)arg0;
    u32 frame = frame_counter;

    \n"
        "jal     0x80095FD8\n"
        "li      $a2, 0\n"
    );

    u32 car_index = (frame * frame) % 8;
    CarData* current_car = &car_array[car_index];
    
    if (current_car->mph != 0) {
        s16 difficulty = pad_array[0].difficulty; // Assuming player is at index 0
        \n"
            "li      $a2, 1\n"
            "lw      $a1, 56($sp)\n"
            "jal     0x80095FD8\n"
            "li      $a2, 0\n"
        );

        }
}



// === func_8009614C @ 0x8009614C ===

void func_8009614C(void* arg0, void* arg1, void* arg2) {
    if (arg2 != NULL) {
        s16 value = *(short*)0x8003EB70;
        if (*(short*)0x80152770 == 0) {
            func_80007270(arg2, 1);
        }
        func_80095FD8(arg2);
        func_800075E0(0x80152770, 0, arg2);

        u8* gstate = (u8*)gstate;
        s32 index = *(short*)0x8011E37C + 8;
        func_80002790(gstate + index, 8);
    }
}

void func_80096240(void* arg0) {
    if (*(int*)arg0 != -1) {
        int at = *(int*)arg0;
        func_80018E2C(at);
        func_800154A4(0);
        func_80096130(arg0);
        *(int*)((char*)arg0 - 5472) = -1;
    }
}



// === func_80096240 @ 0x80096240 ===


void func_80096240(u32 arg1) {
    if (arg1 == -1) {
        return;
    }

    \n"
        "addiu $t6, $zero, -1\n"
        "lui $at, 0x8012\n"
        "sw $t6, -5472($at)"
    );
}

u32 func_80096298(u32 arg0) {
    "
    );
}

void func_800962D4(u32 arg0, u32 arg1) {
    \n"
        "jal 0x80007270\n"
        "addiu $a2, $zero, 1\n"
        "lw $a1, 24($sp)\n"
        "lui $a3, 0x8015\n"
        "addiu $a3, $a3, 10096\n"
        "jal 0x80095F8C\n"
        "or $a0, $a1, $zero\n"
        "or $a0, $v0, $zero\n"
        "jal 0x80095EF4\n"
        "move $a2, $zero\n"
        "lw $t6, 16($v0)\n"
        "lw $t7, 28($sp)\n"
        "or $a0, $a3, $zero\n"
        "beq $t6, $zero, 0x80096344\n"
        "move $a1, $zero\n"
        "move $a1, $zero\n"
        "jal 0x800075E0\n"
        "move $a2, $zero\n"
        "beqz $zero, 0x80096380\n"
        "lw $ra, 20($sp)\n"
        "blez $v1, 0x80096374\n"
        "addiu $t8, $v1, -1\n"
        "beqz $zero, 0x80096374\n"
        "sb $t8, 22($v0)\n"
        "lbu $v1, 22($v0)\n"
        "slti $at, $v1, 255\n"
        "beqz $at, 0x80096374\n"
        "addiu $t9, $v1, 1"
    );
}



// === func_80096298 @ 0x80096298 ===

#include <stdint.h>

typedef struct {
    uint8_t unknown;
} GameStruct;

typedef struct {
    uint8_t unknown;
} CarArrayEntry;

extern GameStruct gstate;
extern CarArrayEntry car_array[];

uint32_t get_car_data(uint32_t param1) {
    uint32_t index = param1 * 4;
    CarArrayEntry* entry = &car_array[index];
    GameStruct* game_struct_ptr = (GameStruct*)(((uintptr_t)&game_struct) + 0x6D44);
    return game_struct_ptr->unknown;
}

void set_car_data(uint32_t param1, uint32_t value) {
    uint32_t index = param1 * 4;
    CarArrayEntry* entry = &car_array[index];
    GameStruct* game_struct_ptr = (GameStruct*)(((uintptr_t)&game_struct) + 0x6D38);
    entry->unknown = value;
    func_800962D4(entry);
}

int func_800962D4(CarArrayEntry* param1) {
    uint8_t value = param1->unknown;
    if (value != 0) {
        value--;
        if (value < 255) {
            value++;
        }
        param1->unknown = value;
    }
    func_800963E0(param1);
    return 0;
}

void func_800963E0(CarArrayEntry* param1) {
    // Assuming this function does something with the entry
}



// === func_800962D4 @ 0x800962D4 ===


#include <stdint.h>

extern uint8_t gstate;
extern struct CarData car_array[];
extern struct GameStruct game_struct;

void func_800962D4(void* param) {
    uint32_t a0 = 0x80152770;
    uint32_t ra = (uint32_t)param + 20;
    uint32_t a1 = 0;
    uint32_t a2 = 1;
    struct CarData* car = (struct CarData*)param;

    func_80007270(a0, a1);
    func_80095F8C(car, 0x80152770, a2);

    uint32_t v0 = car->data_valid;
    uint32_t t6 = car_array[v0].checkpoint;

    if (t6 != 0) {
        uint8_t* ptr = (uint8_t*)&car_array[t6];
        *ptr = 0xFF - *ptr;
        if (*ptr >= 0xFF) {
            *ptr = 0x00;
        }
    }

    func_800075E0(0);
}

void func_8009638C(void* param) {
    uint32_t ra = (uint32_t)param + 20;
    uint32_t a1 = 0;
    struct CarData* car = (struct CarData*)param;

    func_80096288(car, a1);

    uint32_t t7 = car_array[car->checkpoint].checkpoint;
    uint32_t* ptr = (uint32_t*)&car_array[t7];

    *ptr += 1;

    func_800075E0(0);
}

void func_800963E8(void* param) {
    uint32_t ra = (uint32_t)param + 68;
    uint32_t fp = (uint32_t)param + 64;
    uint32_t s7 = (uint32_t)param + 60;
    uint32_t s6 = (uint32_t)param + 56;
    uint32_t s5 = (uint32_t)param + 52;
    uint32_t s4 = (uint32_t)param + 48;
    struct CarData* car = (struct CarData*)param;

    , "=m"(fp), "=m"(s7), "=m"(s6), "=m"(s5), "=m"(s4)
    );

    func_800962D4(car);

    uint32_t t8 = car->data_valid + 1;
    uint8_t* ptr = (uint8_t*)&car_array[t8];
    *ptr += 1;

    , "=m"(fp), "=m"(s7)
    );
}



// === func_8009638C @ 0x8009638C ===

void func_8009638C(void *arg0) {
    int sp = -32;
    void *ra = (void *)(*(int *)&stack[sp + 20]);
    CarData *car = (CarData *)arg0;
    u16 a1 = 0;

    func_80096288(car, a1);

    u32 t6 = (u32)car << 2 | (u32)car;
    int t7 = ((t6 << 2) + 0x801527960) >> 2;
    t6 <<= 2;
    CarData **car_ptr = (CarData **)t7;
    car_ptr[0] = car;

    func_800962D4();

    u32 v0 = *(u32 *)&stack[sp + 24];
    stack[v0 + 2] |= 1;
}

void func_800963E8(void *arg0, void *arg1, void *arg2) {
    int sp = -72;
    void *ra = (void *)(*(int *)&stack[sp + 68]);
    void *fp = (void *)(*(int *)&stack[sp + 64]);
    void *s7 = (void *)(*(int *)&stack[sp + 60]);
    void *s6 = (void *)(*(int *)&stack[sp + 56]);
    void *s5 = (void *)(*(int *)&stack[sp + 52]);
    void *s4 = (void *)(*(int *)&stack[sp + 48]);
    void *s3 = (void *)(*(int *)&stack[sp + 44]);
    void *s2 = (void *)(*(int *)&stack[sp + 40]);
    void *s1 = (void *)(*(int *)&stack[sp + 36]);
    void *s0 = (void *)(*(int *)&stack[sp + 32]);

    u32 v0 = *(u32 *)arg0;
    u16 at = 0xDF00 | (v0 & 0xFFFF);
    CarData *car = (CarData *)arg0;
    void *a1 = arg1;
    void *a2 = arg2;

    if (v0 != at) {
        a2 = arg2;
        u32 s7 = 0xFFFFF0FF;
        u16 fp = 0x8000;
        u16 s6 = 0x0F00;
        u16 s5 = 0xFF00;
        u32 s4 = *(u32 *)&stack[sp + 88];
        u32 v1 = (v0 & s5) | ((u32)a1 << 9);
        at = 0xDE00 | ((v1 >> 4) & 0x0F);
        u32 a0 = *(u32 *)(car + 4);
        u32 a1_value = v0;

        while ((a0 & s6) != 0) {
            v1 &= (s1 << 3);
            if ((v1 & 8) == 0) {
                v1 &= s7;
                u32 t7 = v1 & s5;
                if (t7 != 0) {
                    a0 += fp;
                    if (v1 == 0) {
                        a2 = arg2;
                        u32 s4 = *(u32 *)&stack[sp + 88];
                        void *s3 = arg1;
                        func_800963E8(car, s3, s4);
                        break;
                    }
                } else if ((v1 & 4) != 0) {
                    a2 = arg2;
                    u32 s4 = *(u32 *)&stack[sp + 88];
                    void *s3 = arg1;
                    func_800963E8(car, s3, s4);
                }
            }
        }
    }
}



// === func_800963E8 @ 0x800963E8 ===

void func_800963E8(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    s32 sp = (s32)&arg3 - 72;

    \n"
        "sw      $fp, 64($sp)\n"
        "sw      $s7, 60($sp)\n"
        "sw      $s6, 56($sp)\n"
        "sw      $s5, 52($sp)\n"
        "sw      $s4, 48($sp)\n"
        "sw      $s3, 44($sp)\n"
        "sw      $s2, 40($sp)\n"
        "sw      $s1, 36($sp)\n"
        "sw      $s0, 32($sp)\n"
    );

    CarData* car = (CarData*)arg0;
    u8* pad = (u8*)arg3;

    \n"
        "lui     $at, 0xDF00\n"
        "or      $s0, $a0, $zero\n"
        "or      $s1, $a3, $zero\n"
        "or      $s4, $a1, $zero\n"
        "beq     $v0, $at, 0x8009658C\n"
        "or      $s5, $a2, $zero\n"
        "lui     $s7, 0xF0FF\n"
        "ori     $s7, $s7, 0xFFFF\n"
        "lui     $fp, 0x8000\n"
        "lui     $s6, 0x0F00\n"
        "lui     $s3, 0xFF00\n"
        "lw      $s2, 88($sp)\n"
        "and     $v1, $v0, $s3\n"
        "lui     $at, 0xDE00\n"
        "i-type  op=0x15\n"
        "lui     $at, 0xE100\n"
        "lw      $a0, 4($s0)\n"
        "or      $a1, $v0, $zero\n"
        "addiu   $s0, $s0, 4\n"
        "and     $t6, $a0, $s6\n"
        "beq     $t6, $zero, 0x80096478\n"
        "andi    $v1, $s1, 0x0008\n"
        "bne     $zero, $zero, 0x80096488\n"
        "and     $a0, $a0, $s7\n"
        "and     $t7, $a0, $s3\n"
        "beq     $t7, $zero, 0x80096488\n"
        "addu    $a0, $a0, $fp\n"
        "bne     $v1, $zero, 0x8009649C\n"
        "lui     $at, 0x00FF\n"
        "i-type  op=0x15\n"
        "and     $t8, $a1, $at\n"
        "sw      $s5, 0($s0)\n"
        "addu    $a0, $a0, $fp\n"
        "lw      $v0, 0($s0)\n"
        "bne     $t8, $zero, 0x800964CC\n"
        "addiu   $s0, $s0, 4\n"
        "andi    $t9, $s1, 0x0004\n"
        "bne     $t9, $zero, 0x8009657C\n"
        "or      $a1, $s4, $zero\n"
        "or      $a2, $s5, $zero\n"
    );
}



// === func_800965BC @ 0x800965BC ===

void func_800965BC(u32 arg0) {
    s32 sp = -88;
    void* ra = (void*)&func_800965BC + 4;
    u16 s6 = 0;
    u16 s5 = 0;
    u16 s4 = 0;
    u16 s3 = 0;
    u16 s2 = 0;
    u16 s1 = 0;
    u8* s0 = (u8*)arg0;

    func_80096288(arg0, 1);

    u32 t6 = (arg0 << 2) + arg0;
    u32 t7 = ((t6 << 2) + gcar_list) & 0xFFFFFFFF;
    u32 t8 = t7 | (t6 << 2);
    u8 t9 = *(u8*)(t8 + 3);
    u8 v1 = *(u8*)0x8014A004;
    u8 t7b = *(u8*)(t8 + 4);

    if (t9 != t7b) {
        u32 v2 = *(u32*)(v1 + 2564);

        if (v2) {
            for (s3 = 64; arg0 < v2; arg0 += 16, s1 += 16, ++s5) {
                CarData* car = &car_array[s1 >> 8];

                if (car) {
                    u32 t10 = *(u32*)(car + 32);

                    if (t10 != 0) {
                        func_800963E8(t10, 2);
                    }
                }
            }
        }

        *(u8*)(t8 + 4) = s5;
    }
}



// === func_80096734 @ 0x80096734 ===

void func_80096734(void *arg0, int arg1, int arg2) {
    u32 sp = 0x1FFFFFFC - 232;
    u32 ra = *(u32 *)(sp + 44);
    s32 s4 = *(s32 *)(sp + 40);
    s32 s3 = *(s32 *)(sp + 36);
    s32 s2 = *(s32 *)(sp + 32);
    s32 s1 = *(s32 *)(sp + 28);
    s32 s0 = *(s32 *)(sp + 24);

    u32 t7 = sp + 164;
    u32 t6 = 0;
    u32 t1 = 0xFFFF;

    int a3 = arg0[244];
    u32 t3 = arg1;
    u32 t2 = arg2;

    s4 = 1;
    u32 a0 = arg0[164];

    for (int i = 0; i < 256; i++) {
        if ((a0 & 0xFF) == 0x80) {
            s2 = 128;
            break;
        }
        if ((a0 & 0xFF) == 0x84) {
            s2 = 128;
            break;
        }
        if (i < 9) continue;
        if (i < 64) continue;

        u32 t7 = arg0[8];
        u32 t8 = t7 & 0xFF00;
        t9 = t8 >> 24;
        s6 = t9 & 0x00FF;
        a0 = s6 & 0x00C0;

        if (a0 == 0x80) {
            break;
        }
        if (a0 == 0x84) {
            break;
        }

        if ((s1 & 0xFF) == 0xD2) continue;
        if ((s1 & 0xFF) == 0xD6) continue;

        if ((ra & 0xFF) == 0xDA) continue;
        if ((ra & 0xFF) == 0xDC) continue;
        if ((ra & 0xFF) == 0xDD) continue;
        if ((ra & 0xFF) == 0xDE) continue;

        t7 = arg0[4];
        u32 t8 = (t7 & 0xF00) >> 12;
        t9 = s1 & 0x0F00;

        if (s2 != t8 && s1 != t9) {
            continue;
        }

        s0 = t6 | t8;
    }
}



// === func_80096A00 @ 0x80096A00 ===

void func_80096A00(u32 a0, u32 a1, u32 a2) {
    if (a0 < a1) {
        u32 s2 = *(u32*)(sp + 12);
        u32 s1 = *(u32*)(sp + 8);
        u32 s0 = *(u32*)(sp + 4);
        v0 = a0;
        s0 = 0xFFFF;
        s2 = 4;
        s1 = 225;
        t5 = 0xF00;
        t4 = 253;
        t3 = 128;
        t2 = 64;
        t1 = 0xFF00;
        t6 = *(u32*)v0 & t1;
        t7 = t6 >> 24;
        t8 = t7 & 0xFF;
        a2 = t8 & 0xC0;

        if (a2 != t2) {
            goto func_80096ADC;
        }

        if (a2 != t3) {
            goto func_80096ADC;
        }

        if (!(t8 < 9)) {
            if (!(t8 < 64)) {
                goto func_80096ADC;
            }
        }

        if (!(a0 < 192)) {
            if (!(a0 < 214)) {
                return;
            }
        }

        v0 += 8;
        t9 = *(u32*)(v0 + 8) & t1;
        t6 = t9 >> 24;
        t8 &= t6;
        t0 = *(u32*)v0 + a3;
        t6 = t0 & s0;
        t7 = t0 & t5;
        a2 = t7 | t6;
        *(u32*)(v0 + 4) = a2;

func_80096ADC:
        v0 += 8;
    }

    while (1) {
        if (!(v0 < a1)) {
            break;
        }

        t6 = *(u32*)v0 & t1;
        t7 = t6 >> 24;
        t8 = t7 & 0xFF;
        t9 = *(u32*)(v0 + 8) & t1;
        t6 = t9 >> 24;
        t8 &= t6;
        t0 = *(u32*)v0 + a3;
        t6 = t0 & s0;
        t7 = t0 & t5;
        a2 = t7 | t6;
        *(u32*)(v0 + 4) = a2;

        v0 += 8;
    }

    sp += 16;
}



// === func_80096B5C @ 0x80096B5C ===

void func_80096B5C(void* a0, void* a1, u32* a2) {
    if (a0 == NULL) {
        return;
    }

    u32 v1 = 0;

    func_80096B00(a0);

    if (a2 != NULL) {
        *a2 = 0;
    }

    if (v1 == 0 && a2 != NULL) {
        *(u32*)a2 = 0;
        return;
    }

    u32* v0 = *(u32**)(gstate + 4);
    u32 t6 = *(u32*)(gstate + 8);

    if (a2 != NULL) {
        *a2 = t6;
    }

    if (a1 == NULL) {
        return;
    }

    u32 a3 = 0;
    u32 t7 = *(u32*)a1 + a2;

    if (t7 <= 0) {
        return;
    }

    u32 t3 = *(u32*)(gstate + 8) + a2;
    *a0 = t3;

    u32 t4 = *(u32*)(*a0);
    a3 += 1;
    u32 v0_addr = *(u32*)a0 + 12;
    t7 = *(u32*)v0_addr + a2;
    *a0 = t7;

    if (a3 <= 0) {
        return;
    }

    u32 t5 = *(u32*)(gstate + 8);
    a2 += 1;
    u32 v0_addr2 = *(u32*)a0 + 12;
    t7 = *(u32*)v0_addr2 + a2;
    *a0 = t7;
}



// === func_80096CC4 @ 0x80096CC4 ===

void func_80096CC4(void* a1, int t0, int t7) {
    char* sp = (char*)__builtin_frame_address(0);
    *(int*)(sp + 28) = __builtin_return_address(0);

    if (t8 != 0) {
        *(char*)(a3 + 1) = 1;
    } else {
        *(char*)(a3 + 1) = 0;
    }

    if (t0 == 0) {
        CarData* car_array = (CarData*)0x80152818;
        char t6 = *(char*)(a3 + 5);
        switch (t6 & 7) {
            case 1:
                car_array[*(int*)gstate].RWV[2] |= 16;
                break;
            case 2:
                car_array[*(int*)gstate].RWV[2] |= 8;
                break;
        }

        if (*(short*)(a3 - 24312) == 1) {
            sp[68] = t7;
            int* s7 = (int*)100;
            func_80096C28((void*)s7, *(int*)(a3 + 12), a3);
            int t0 = *(int*)(sp + 116);

            if (t0 != 0) {
                char* s5 = *(char**)(sp + 80);
                int t6 = *(int*)s7;
                int t7 = *(int*)(a3 + 16);
                sp[68] = t7;
                func_80096BBC((void*)s7, *(int*)(a3 + 12), a3);
            }

            *(char**)(sp + 52) = a3;
            *(int*)(sp + 64) = gstate[4929];
            __builtin_return_address(0);
        }
    }

    u8* gstate = (u8*)0x801146EC;
    CarData* car_array = (CarData*)0x80152818;

    if (*(char*)(a3 + 5) & 7 == 0) {
        break;
    }

    car_array[*(int*)gstate].RWV[2] |= (t7 == 1 ? 16 : 8);
}



// === func_80097184 @ 0x80097184 ===

void func_80097184(void *arg) {
    u32 gameState = *(u32 *)gstate;
    u32 index = gameState << 2;
    CarData **car = car_array + index;

    if (*car != NULL) {
        u8 place = (*car)->place;
        if (place != 0) {
            void *data = *car;
            s32 lap = *(s32 *)(0x8012 + index);
            void **nextData = (*(void **)(data + 12)) ? *(void **)(data + 12) : *(void **)(data + 12);

            lap += -6784;
            func_80008460(nextData, *(void **)(0x8012 + index));
            func_800084E0(nextData, *(void **)(0x8012 + index));
            func_80008630(*(void **)(0x8003 + 21520), nextData, *(void **)(0x8012 + index), *(void **)(0x8003 + 21520));
            func_80007270(lap, 0, 0, lap);
        }
    }

    ((u8 *)*car)[5] = 1;
    func_80096CA8();
}



// === func_8009731C @ 0x8009731C ===

void func_8009731C(void *arg) {
    u32 sp = (u32)arg;
    s32 local_30 = *(s32 *)(sp + 40);
    s32 local_28 = *(s32 *)(sp + 20);

    func_80007270((void *)gstate, 0, local_30);
    func_80095F8C(local_30, 1);

    local_30 = (local_30 & local_28) | local_30;
    gstate->unk_34 = (gstate->unk_34 & local_30) | local_30;

    s32 *car_ptr = *(s32 **)(sp + 16);
    func_800075E0((void *)gstate, 0, car_ptr);

    s32 *local_14 = (s32 *)(sp + 28);
    local_30 = *(u8 *)&car_ptr[1];
    local_30 |= 0x01;

    if (local_30) {
        local_30 = local_30 & 0xFFFE;
        s32 *temp = car_ptr + 4;

        while (*temp != 0) {
            s32 temp_var = *temp;
            s32 temp_var1 = *(s32 *)(temp + 8);

            if (temp_var1 < local_30) {
                gstate->unk_34 += temp_var1;
                local_30 = temp_var1;
            } else if (temp_var1 > local_30) {
                gstate->unk_34 += temp_var - temp_var1;
                local_30 = temp_var;
            }

            temp = car_ptr + 12;
        }

        s32 *local_8 = (s32 *)(sp + 12);
        if (*(u32 *)temp != 0) {
            *(s32 **)(temp + 4) = local_8;
        } else {
            *(s32 *)(temp + 8) = local_8;
        }

        s32 temp_var2 = *temp;
        s32 temp_var3 = *(s32 *)(temp + 12);
        *(s32 *)(local_8 + 4) = temp_var3;
        *local_8 = temp_var2;

        temp_var2++;
        *(u8 *)&car_ptr[1] = temp_var2;
    }
}



// === func_80097470 @ 0x80097470 ===

void func_80097470(void *arg0, int arg1) {
    void (*func)(int);
    int result;
    int car_index = 0;

    func = (void (*)(int))*(int *)(0x80152770 + 0x1000);
    func(1);

    if (arg1 != 0) {
        result = *(int *)0x80153FC8;
        func_80097384(result);
    }

    func = (void (*)(int))*(int *)(0x80152770 + 0x1000);
    func(0);
}

void func_800974EC(void *arg0, int arg1) {
    void (*func)(int);
    int result;
    int car_index = 0;

    func = (void (*)(int))*(int *)(0x80152770 + 0x1000);
    func(1);

    if (arg0 != NULL) {
        if (*(int *)0x80153FC8 == 0) {
            result = *(int *)*(int *)0x80153FC8;
            func_80097384(result);
        }
        car_array[car_index].place = 0;

        func = (void (*)(int))*(int *)(0x80152770 + 0x1000);
        func(0);

        if (*(int *)arg0 != 0) {
            car_array[car_index].place = (car_array[car_index].place << 2) | 1;
            func_80097384(*(int *)arg0);
        }
    }

    car_array[car_index].place = 0;
}



// === func_800974EC @ 0x800974EC ===

void func_800974EC(u32 arg0, u32 arg1) {
    CarData* car = (CarData*)car_array;
    CarData* ai_car = (CarData*)(car + 1);
    s32 lap_count = gstate_prev & GSTATE_LAPS;

    if (!(gstate & GSTATE_PLAY)) {
        return;
    }

    u8 lap_number = (lap_count >> 16) & 0xFF;
    u8 current_lap = ai_car->laps;

    if (current_lap != lap_number) {
        lap_number = current_lap;
    }

    // Additional logic here...
}



// === func_80097798 @ 0x80097798 ===


void func_80097798(s32 a0, s32 a1, u8 a2, s32 a3) {
    char *sp = (char *)malloc(112);
    
    if (!sp) return;

    sp[60] = *(char *)&ra;
    sp[56] = *(char *)&fp;
    sp[52] = *(char *)&s7;
    sp[48] = *(char *)&s6;
    sp[44] = *(char *)&s5;
    sp[40] = *(char *)&s4;
    sp[36] = *(char *)&s3;
    sp[32] = *(char *)&s2;
    sp[28] = *(char *)&s1;
    sp[24] = *(char *)&s0;
    sp[120] = a2;

    int t0 = a0;

    if (a1 != 0) {
        int t1 = a3;
        int a1_shifted = a3 << 24;
        int t6 = a1_shifted >> 24;
        func_80097694(t6);
        
        if (v0 < 0) goto end;

        a3 = v0;
        int t7 = v0 << 2;
        t7 += v0 << 2;
        t7 += v0 << 2;

        char *t8 = (char *)0x8015 + t7 + 27963;
        
        if (*t8 == 0) goto call_800965BC;
    }

call_800965BC:
    int a3_save = a3;
    sp[108] = *(char *)&a3;
    a3 = *(char *)&sp[108];

    char *v1 = (char *)0x8015 + 27960;
    int a2_save = 0;

    func_800944EC(v1, 64);

    *(char *)&sp[64] = v1;
    int a2_save = *(char *)&v1;

    if (*(char *)&v1 == 0) goto end;

    *v1 = 0;
    *v1 = 1;
    *v1 = 2;
    *v1 = 4;
    *v1 = t0;

    if (a1 != 0) {
        *v1 = a1;
        int t6 = t0 << 2;
        char *a1_addr = (char *)0x8003 + t6 - 6784;
        int at = -32;
        char *a0_save = (char *)&sp[128];

        a1 += 31;
        a1 &= at;
        a1 |= *(char *)&t9;

        func_800944EC(v1, 64);
        *(char *)&sp[80] = v1;
        *(char *)&sp[80] = v2;

    if (*(char *)&v1 != 0) {
        *v1 = v0;
        int t8 = t0 << 2;
        char *a1_addr = (char *)0x8003 + t8 - 6784;
        int at = -32;
        char *a0_save = (char *)&sp[128];

        a1 += 31;
        a1 &= at;
        a1 |= *(char *)&t9;

        func_800944EC(v1, 64);
        *(char *)&sp[80] = v1;
        *(char *)&sp[80] = v2;

end:
    free(sp);
}



// === func_800979A8 @ 0x800979A8 ===

int func_800979A8(void *arg0, int arg1) {
    int sp = -56;
    void **sp32;
    int s0 = (int)arg0;
    int ra;

    if (t6 != 0) {
        sp32[60] = (void *)arg1;
        u32 t7 = *(u32 *)0x8012FFA8;
        s32 at = -1;
        u32 t8 = *(u32 *)0x80150AD4;

        if (t7 != at) {
            arg1 = 0;
            u32 t9 = *(u32 *)0x80150AD4;

            if (s0 == t9) {
                goto end;
            }
        }

        s0 += 10;
        func_80097798((void *)(s0 + 16), 0, 0);

        void *v1 = (void *)0x80150AD4;
        *(u32 *)0x801506D8 = v0;

        func_8009638C(v0);
        u32 a3 = *(u32 *)0x80150AD4;
        func_80096288(a3, 0, 0);

        int t9 = (a3 << 2) + (a3 << 4) + (a3 << 6);
        u32 v0 = *(u32 *)(0x80150AD4 + t9);
        u32 t7 = *(u32 *)0x8012FFA8;
        u32 t8 = *(u32 *)0x80150AD4;
        u32 at = -3984;
        int s0 = (s0 << 2) + at;

        void *v1 = (void *)(0x80150AD4 + s0);
        *(u32 *)(0x801506D8 + 6876) = v0;

        u16 a1 = *(u16 *)*(v1 + 2);
        u32 a2 = *(u32 *)(*(v1 + 8));
        u32 a0 = *(u32 *)(*(v1 + 4));
        u32 a3 = *(u32 *)(*(v1 + 16));

        int t9 = (a3 << 2) + (a3 << 4);
        void *v1 = (void *)(0x80150AD4 + t9);

        func_8001536C(0, v1, a1);

        u32 t8 = *(u32 *)0x80150AD4;
        int s0 = (s0 << 2) + at;

        void *v1 = (void *)(0x80150AD4 + s0);

        u16 a0 = *(u16 *)*(v1 + 2);
        u32 a2 = *(u32 *)(*(v1 + 8));
        u32 a3 = *(u32 *)(*(v1 + 16));

        int t9 = (a3 << 2) + (a3 << 4);
        void *v1 = (void *)(0x80150AD4 + t9);

        *(u32 *)0x801506D8 = v0;

        u16 a0 = *(u16 *)*(v1 + 2);

    end:
        ra = *(int *)(sp + 36);
        s0 = *(void **)(sp + 32);
        sp += 56;
        return ra;
    }
}



// === func_80097AFC @ 0x80097AFC ===

void func_80097AFC(CarData *car) {
    u16 a0 = car->rpm;
    u8 temp = 255;
    int result = someFunction(a0, temp, temp);

    if (result == -1) {
        car->RWV[3] = result;
        return;
    }

    u32 at = 0x42FE;

    // Bit manipulation operations
    result = car->RWV[3];
    result &= 0x0078;

    if (result != 0) {
        return;
    }

    at |= 0x8000;
    if (at < 0) {
        goto label_80097BA0;
    }

    someFunction(at, 1);

label_80097BA0:
    at &= 0xFF;
    someOtherFunction(at);
    // More bit manipulation operations
    result &= 0x0078;

    if (result != 0) {
        return;
    }

    at |= -1;
    if (at < 0) {
        goto label_80097BA0;
    }

    someFunction(at, 1);
}



// === func_80098554 @ 0x80098554 ===

void func_80098574(void* param1, void* param2, void* param3, int param4) {
    u32 s3 = (u32)param1;
    u32 s2 = (u32)param2;
    u32 s1 = (u32)param3;
    u32 s0 = (u32)param4;

    if (s1 != 0) {
        func_80098574((void*)s3, (void*)s1, (void*)s2, s0);
        ((void (*)(int))s1)((int)s1);
        int t6 = *(int*)s0;
        t6++;
        *(int*)s0 = t6;
    }
}

void func_800985F4(void* param1) {
    u32 a3 = *(u32*)((u32)param1 + 8);
    func_80098574(param1, (void*)a3, 0, a3);
}

void func_80098620(void* param1, void* param2, void* param3) {
    u32 s2 = (u32)param1;
    u32 s1 = (u32)param2;
    u32 s0 = (u32)param3;

    u32 t9 = *(u32*)s2;
    *(u32*)s0 = s1;
    *(u32*)s1 = 0;
    ADC0(0, s0);
    u32 t7 = *(u32*)s2 + 4;
    if (t7 != 0) {
        *(u32*)s2 = t7;
    }
    ((void (*)(void))t9)((int)s1);
}



// === func_80098574 @ 0x80098574 ===

void func_80098574(u32 a0, u32 a1, u32 a2, u32 a3) {
    s32 s0 = (s32)a0;
    s32 s1 = (s32)a1;
    s32 s2 = (s32)a2;
    s32 s3 = (s32)a3;

    if (s1 != 0) {
        func_80098574(s3, s1, s2, s0);
        ((void (*)(u32))s2)(s1);

        u32 t6 = *(u32 *)s0;
        *(u32 *)s0 = t6 + 1;

        if (*(u32 *)(s1 + 4) != 0) {
            func_80098574(s3, s1, s2, s0);
        }
    }

    return;
}

void func_800985F4(u32 a0, u32 a1, u32 a2, u32 a3) {
    s32 s0 = (s32)a0;
    s32 s1 = (s32)a1;
    s32 s2 = (s32)a2;

    u32 t9 = *(u32 *)(s2 + 4);
    *(u32 *)s0 = s1;
    *(u32 *)s1 = 0;
    u32 t6 = *(u32 *)s0;
    *((u32 *)t6 + 1) = 0;

    if (*(u32 *)s2 != 0) {
        ((void (*)(u32))s2)(t9);
    }

    while (1) {
        s0 = *(u32 *)s0;
        if (*(u32 *)s0 == 0) {
            return;
        }
        s0 += 4;
    }
}

void func_80098620(u32 a0, u32 a1, u32 a2, u32 a3) {
    s32 s0 = (s32)a0;
    s32 s1 = (s32)a1;
    s32 s2 = (s32)a2;

    if (s2 != 0) {
        func_80098574(s3, s1, s2, s0);
        *(u32 *)s1 = 0;
        u32 t6 = *(u32 *)s0;
        *((u32 *)t6 + 1) = 0;

        if (*(u32 *)(s2 + 4) != 0) {
            func_80098574(s3, s1, s2, s0);
        }
    }

    while (1) {
        s0 = *(u32 *)s0;
        if (*(u32 *)s0 == 0) {
            return;
        }
        s0 += 4;
    }
}



// === func_800985F4 @ 0x800985F4 ===

void func_800985F4(void *arg0) {
    CarData *car = arg0;

    func_80098574(arg0, car->mph);
}

void func_80098620(void *arg0, void *arg1, void *arg2) {
    int s0 = (int)arg0;
    int s1 = (int)arg1;
    int s2 = (int)arg2;
    int t9;

    *(int *)s2 = 0;
    *(int *)(*(int *)s0 + 4) = 0;
    t9 = *(int *)(*(int *)s2);
    if (*(int *)s1 != 0) {
        *(int *)(*(int *)s2 + 4) = *(int *)s1;
    }
    while (1) {
        *(int *)s0 = *(int *)s0;
        if (*(int *)s0 == s2) {
            break;
        }
        s0 += 4;
    }
}

void func_800986B0(void *arg0, void *arg1) {
    int a1 = (int)arg0;
    int a2 = (int)arg1;

    func_80098620(arg0, arg1, arg0 + 8);
}



// === func_80098620 @ 0x80098620 ===

void func_80098620(u32 arg1, u32 arg2, u32 arg3) {
    CarData *car = (CarData *)arg1;
    PadEntry *pad = (PadEntry *)arg2;
    CarData *car_next = (CarData *)arg3;

    u32 state_current = *(u32 *)(car + 0);
    u32 pad_value = *(u32 *)(pad + 4);

    *(u32 *)(car + 0) = state_current;
    *(u32 *)(state_current + 0) = 0;
    *(u32 *)(car_next + 4) = 0;

    if (pad_value != 0) {
        pad_value += 1;
        *(u32 *)(pad + 4) = pad_value;
    }

    func_800986B0(car, &pad_value, car_next);
}

void func_800986B0(u32 arg1, u32 arg2) {
    CarData *car = (CarData *)arg1;
    PadEntry *pad = (PadEntry *)arg2;

    u32 state_current = *(u32 *)car;
    u32 pad_value = *(u32 *)pad;

    func_80098620(car, &pad_value, pad);
    state_current += 8;
    func_80098620(car, &pad_value, pad);
}



// === func_800986B0 @ 0x800986B0 ===

void func_800986B0(CarData *car) {
    void (*func_80098620)(void);
    
    \n"
        "jal     func_80098620\n"
        "addiu   $a1, %0, 8\n"
        "lw      $ra, 20($sp)\n"
        "addiu   $sp, $sp, 24\n"
        "jr      $ra\n"
        : 
        : "r"(car)
        : "$at", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8"
    );
}



// === func_800987E8 @ 0x800987E8 ===

int func_800987E8(int a0) {
    if (a0 == -1) return 0;

    int* ra = __builtin_frame_address(0);
    *ra = 20;

    u32 v1 = 0x8014;
    u32 a1 = a0 & 0xFF;
    u32 v1_offset = 19528;
    u32 t6 = (a1 << 2) - a1;
    t6 <<= 3;
    u32 v0 = v1 + t6;
    u32 t7 = 8;

    s8 t8 = *(s8*)(v0 + t7);
    if (t8 != 0) {
        a0 = *(u32*)(v0 + 16);
        u32 t9 = v0 + 1;
        if (t9 > 0) {
            v0 = 1;
        }
    } else {
        v0 = 0;
    }

    return v0;
}

int func_80098874(int s0) {
    int* ra = __builtin_frame_address(0);
    *ra = 20;

    u32 t7 = 0x8014;
    u32 t6 = *(u32*)(t7 + 24836);
    u32 t5 = *(u32*)s0;
    s8 t9 = *(s8*)(s0 + 13);

    u32 a0 = 0x8015;
    u32 t8 = t5 & t7;
    if (t9 == 0) {
        *(u32*)s0 = t8;
        a0 = 0x8015 - 26528;
        *(u32*)s0 = 0;
        *(s8*)(s0 + 13) = 0;
    }

    a0 = 0x8015 - 26616;
    u32 t2 = *(u32*)a0;
    *(u32*)s0 = 0;
    *(s8*)(s0 + 12) = 1;

    return 0;
}

int func_800988E0(int s3) {
    if (s3 == 0) return 0;

    u32 at = 0x4661;
    u32 s6 = 0x8015 + 10056;
    u32 s7 = 2;
    u32 s4 = -1;
    s8 t6 = *(s8*)(s3 + 14);
    u32 s5 = *(u32*)s3;

    if (t6 != 0) {
        }

    return 0;
}



// === func_80098874 @ 0x80098874 ===

void func_80098874(volatile u32 *s0) {
    u16 t6 = s0[1];

    if (t6 == 0) return;

    s0[1] = t6 & gstate->data_valid;

    CarData *car = car_array[s0[2]];
    func_8009211C(car, s0);

    s0[13] = 1;
    func_80091FBC(s0);
}

void func_800988E0(volatile u32 *s3) {
    if (s3 == 0) return;

    s3[16] = 2;
    s3[15] = -1;

    u8 t6 = s3[14];
    if (t6 != 0) goto end;

    u32 a0 = s3[9];
    if (a0 != -1) {
        f32 f0, f16, f2;
        memcpy(&f0, &s3[4], sizeof(f0));
        memcpy(&f16, &pad_array[s3[2]], sizeof(f16));
        memcpy(&f2, &s3[5], sizeof(f2));

        u32 v0 = 0;
        if (func_80091C04(s0) != 0) {
            func_80098874(s0);
            goto end;
        }
    }

    if (s3[9] == s0) {
        u8 t7 = s0[26];
        if (t7 == 0) {
            func_800987E8(s3, s0->car);
            goto end;
        }
    }

end:
    func_80091BA8(s0);
}



// === func_800988E0 @ 0x800988E0 ===

int func_800988E0(void) {
    u32 sp = -24;
    void *ra = *(void **)((char *)&sp + 20);

    if (s3 != 0) {
        u16 at = 0x4661;
        u32 s6 = 0x8015 << 16;
        u32 s7 = 2;
        s32 s4 = -1;
        char t6 = *(char *)(s3 + 14);
        CarData *car = (CarData *)s3;

        if (t6 != 0) {
            u32 a0 = *(u32 *)(s3 + 56);

            if (s4 != a0) {
                f32 f0 = car->mph;
                f32 f16 = *(f32 *)s6;
                f32 f2 = car->dr_vel[0];

                // i-type instructions
                // ...

                u32 v0 = 0;

                if (f0 != f16) {
                    v0 = 1;
                }

                if (v0 == 0) {
                    func_80091C04();
                    func_80098874(car);
                }

                s32 s0 = (s32)s3;
                if (*(u32 *)0 != 0) {
                    func_80091BA8();

                    if (v0 == 0) {
                        u32 v0 = *(u32 *)(s3 + 56);
                        s7 = 2;
                        char t7 = (v0 & 0xFF) + 1;

                        if (t7 < 4) {
                            *(char *)(s3 + 24) = t7;
                        }
                    }

                    s0 = (s32)s3;
                    s7 = 2;
                    t7 = (s0 & 0xFF) + 1;

                    if (t7 < 4) {
                        func_80098874(car);
                    }

                    v0 = *(u32 *)(s3 + 56);
                    s7 = 2;
                    t7 = (v0 & 0xFF) + 1;

                    if (t7 < 4) {
                        func_800987E8(func_800987E8);
                        u32 a0 = *(u32 *)(s3 + 60);

                        if (a0 != 0) {
                            func_5800();
                        }
                    }

                    v0 = *(char *)



// === func_80098AE4 @ 0x80098AE4 ===

void func_80098AE4() {
    \n"
        "lui     $a0, 0x8014\n"
        "sw      $fp, 112($sp)\n"
        "sw      $s7, 108($sp)\n"
        "sw      $s6, 104($sp)\n"
        "sw      $s5, 100($sp)\n"
        "sw      $s4, 96($sp)\n"
        "sw      $s3, 92($sp)\n"
        "sw      $s2, 88($sp)\n"
        "sw      $s1, 84($sp)\n"
        "sw      $s0, 80($sp)\n"
        "sdc1    $f30, 72($sp)\n"
        "sdc1    $f28, 64($sp)\n"
        "sdc1    $f26, 56($sp)\n"
        "sdc1    $f24, 48($sp)\n"
        "sdc1    $f22, 40($sp)\n"
        "sdc1    $f20, 32($sp)\n"
        "addiu   $a0, $a0, 10024\n"
        "move    $a1, $zero\n"
        "jal     0x80007270\n"
        "addiu   $a2, $zero, 1\n"
        "lui     $s4, 0x8014\n"
        "lw      $s4, 25072($s4)\n"
        "lui     $at, 0x3F80\n"
        "addiu   $s6, $sp, 304\n"
        "beq     $s4, $zero, 0x80098F5C\n"
        "lui     $s5, 0x8014\n"
        "i-type  op=0x11\n"
        "i-type  op=0x11\n"
        "addiu   $fp, $zero, 1\n"
        "addiu   $s5, $s5, 24968\n"
        "lw      $t9, 0($s4)\n"
        "sw      $t9, 368($sp)\n"
        "jal     0x80091BA8\n"
        "lw      $a0, 40($s4)\n"
        "lw      $v1, 16($v0)\n"
        "addiu   $at, $zero, 2\n"
        "or      $s0, $v0, $zero\n"
        "i-type  op=0x15\n"
        "lb      $t8, 24($s0)\n"
        "lbu     $t6, 26($v0)\n"
        "i-type  op=0x15\n"
        "lb      $t7, 25($v0)\n"
        "i-type  op=0x15\n"
        "jal     0x800987E8\n"
        "lw      $a0, 60($v0)\n"
        "i-type  op=0x15\n"
        "lb      $t8, 24($s0)\n"
        "jal     0x80095800\n"
        "nop\n"
        "beq     $zero, $zero, 0x80098F54\n"
        "lw      $s4, 368($sp)\n"
        "lb      $t8, 24($s0)\n"
        "move    $v1, $zero\n"
        "i-type  op=0x15\n"
        "lw      $s4, 368($sp)\n"



// === func_80098FC0 @ 0x80098FC0 ===

void func_80098FC0(u32 arg0) {
    s32 i;
    u32* ptr;

    if (arg0 != 0) {
        func_80098AE4();
        func_800988D8();

        gstate = game_struct;
        car_array[0] = 0;
        car_array[1] = 0;
        game_struct->unk_234 = 0;
        game_struct->unk_238 = 0;

        ptr = (u32*)((u32)game_struct + 0x3C);
        i = *ptr;
        if (i != 0) {
            func_800987E8(i);
        }

        u32 value = *(u32*)((u32)game_struct + 60);
        if (value == 0) {
            u8 byte = *(u8*)((u32)game_struct + 25);
            if (byte != 0) {
                func_800959DC();
            }
        }

        func_80095800();

        func_80098710(i);

        game_struct->unk_1D4 = i;
        u32* ptr2 = (u32*)((u32)game_struct + 0x24);
        u32 value2 = *ptr2;
        if (value2 != 0) {
            func_80098620(value, value2);
        }

        u32* ptr3 = (u32*)((u32)game_struct + 0x3C);
        i = *ptr3;

        if (i != 0) {
            game_struct->unk_1D4 = i;
            u32* ptr4 = (u32*)((u32)game_struct + 0x24);
            value2 = *ptr4;
            if (value2 != 0) {
                func_80098620(value, value2);
            }
        }

        game_struct->unk_1D0 = i;
    }
}



// === func_800992AC @ 0x800992AC ===

void func_800992AC(u16 arg0) {
    s32 sp = -208;
    CarData* car_array = (CarData*)car_array;

    \n\t"
        "sw $ra, 92($sp)\n\t"

        "lui $a0, 0x8014\n\t"
        "lui $a1, 0x8014\n\t"
        "sw $fp, 88($sp)\n\t"
        "sw $s7, 84($sp)\n\t"
        "sw $s6, 80($sp)\n\t"
        "sw $s5, 76($sp)\n\t"
        "sw $s4, 72($sp)\n\t"
        "sw $s3, 68($sp)\n\t"
        "sw $s2, 64($sp)\n\t"
        "sw $s1, 60($sp)\n\t"
        "sw $s0, 56($sp)\n\t"
        "sdc1 $f26, 48($sp)\n\t"
        "sdc1 $f24, 40($sp)\n\t"
        "sdc1 $f22, 32($sp)\n\t"
        "sdc1 $f20, 24($sp)\n\t"

        "addiu $a1, $a1, 11120\n\t"
        "addiu $a0, $a0, 10152\n\t"
        "jal 0x80006A00\n\t"

        "addiu $a2, $zero, 128\n\t"
        "lui $a0, 0x8003\n\t"
        "lui $a2, 0x8014\n\t"
        "addiu $a2, $a2, 10152\n\t"
        "addiu $a0, $a0, -5912\n\t"
        "jal 0x800005D4\n\t"

        "addiu $a1, $sp, 200\n\t"
        "lui $at, 0xC000\n\t"
        "lui $fp, 0x8014\n\t"
        "i-type op=0x11\n\t"
        "i-type op=0x11\n\t"
        "addiu $fp, $fp, 16416\n\t"

        "addiu $s7, $zero, 2\n\t"
        "addiu $s6, $zero, 1\n\t"

        "jal 0x80007270\n\t"
        "or $a2, $s6, $zero\n\t"
        "lw $t6, 196($sp)\n\t"
        "lui $a0, 0x8014\n\t"
        "lw $s5, 196($sp)\n\t"

        "lh $v1, 0($t6)\n\t"
        "addiu $a0, $a0, 10024\n\t"
        "move $a1, $zero\n\t"
        "bne $s6, $v1, 0x80099388\n\t"

        "jal 0x80098FB8\n\t"

        "addiu $fp, $fp, 16416\n\t"
        "addiu $s6, $zero, 1\n\t"
        "beq $zero, $zero, 0x80099338\n\t"

        "addiu $s7, $zero, 2\n\t"
        "jal 0x80007270\n\t"
        "or $a2, $s6, $zero\n\t"
        "lbu $t7, 2($s5)\n\t"

        "sltiu $at, $t7, 12\n\t"
        "beq $at, $zero, 0x8



// === func_80099BFC @ 0x80099BFC ===

void func_80099BFC(u32 arg1, u32 arg2) {
    register int sp asm("$sp");
    register int ra asm("$ra");
    register int t6 asm("$t6");
    register int t7 asm("$t7");
    register int v0 asm("$v0");
    register int t8 asm("$t8");
    register int t9 asm("$t9");

    sp -= 608;
    ra = func_80099C04(sp, arg1, arg2);

    if (arg1 != 0) {
        CarData* car = &car_array[arg1];
        v0 = (u32)car;
        t6 = (u32)(car + 1);
        u32 game_struct_offset = t7 * 4;

        if ((game_struct + game_struct_offset) < 0) {
            t9 = 0xDE00;
            *(u32*)(sp + 596) = (u32)car;
            *(u32*)v0 = t9;
        }

        if (car_array[2] != 0) {
            car_array[2] = v0;
        }
    }

    if (arg1 == 0) {
        return;
    }

    CarData* car1 = &car_array[arg1 + 2];
    CarData* car2 = &car_array[arg1 + 3];

    if (car2 != 0) {
        func_80087804((u32)car1, (u32)car2);
    }

    CarData* car3 = &car_array[arg1 + 5];
    CarData* car4 = &car_array[arg1 + 6];

    if (car4 != 0) {
        func_80087804((u32)car3, (u32)car4);
    }

    CarData* car5 = &car_array[20];
    *(u32*)(sp + 16) = arg1;

    u32 a1 = 0;
    u32 t7 = v0 << 15;

    if (t7 >= 0) {
        u32 t8 = v0 << 13;
        if (t8 == 0) {
            a1 = 2;
        } else {
            a1 <<= 14;
            if (a1 >= 0) {
                a1 = 2;
            } else {
                a1 <<= 12;
                if (a1 == 0) {
                    a1 = 2;
                } else {
                    a1 <<= 14;
                }
            }
        }
    }

    *(u32*)(sp + 21) = a1;
}



// === func_8009C5BC ===
void func_8009C5BC() {
    \n"
        "jal     0x8009C3F8\n"
        "move    $a0, $zero\n"
        "lw      $ra, 20($sp)\n"
        "addiu   $sp, $sp, 24\n"
        "jr      $ra\n"
        "nop\n"
    );
}

void func_8009C5E0(float param1, float param2, float param3) {
    \n"
        "sw      $a2, 96($sp)\n"
        "lwc1    $f8, 96($sp)\n"
        "lwc1    $f6, 108($sp)\n"
        "sdc1    $f22, 16($sp)\n"
        "jal     0x8009C3F8\n"
        "move    $a0, $zero\n"
        "lw      $ra, 20($sp)\n"
        "addiu   $sp, $sp, 24\n"
        "jr      $ra\n"
        "nop\n"
    );
}



// === func_8009C5E0 @ 0x8009C5E0 ===

void func_8009C5E0(void) {
    float f4, f6, f8, f10, f12, f14, f16, f18;
    unsigned int *sp = (unsigned int *)(((char *)__builtin_frame_address(0)) - 88);

    f4 = *(float *)(sp + 2);
    sp[3] = *(unsigned int *)(sp + 6);
    f8 = *(float *)(sp + 4);
    f6 = *(float *)(sp + 5);
    *(double *)(sp + 4) = (double)f10;
    *(double *)(sp + 5) = (double)f12;

    sp[7] = *(unsigned int *)(sp + 8);

    f4 = *(float *)(sp + 6);
    f10 = *(float *)(sp + 9);
    *(double *)(sp + 3) = (double)f8;

    float *at0 = (float *)0x80121000;
    sp[5] = 128;
    *(double *)(sp + 6) = *(double *)(at0 + 24);

    f6 = *(float *)(sp + 10);
    f10 = *(float *)(sp + 11);
    *(double *)(sp + 7) = (double)f14;

    sp[8] = 0x3F800000;
    *(unsigned int *)0x80004000 = 0x40490FDB;

    f10 = *(float *)(sp + 12);
    f6 = *(float *)(sp + 13);

    float *at1 = (float *)0x80121000;
    sp[5] = 128;
    *(double *)(sp + 9) = *(double *)(at1 + 24);
    *(double *)(sp + 6) = *(double *)(at1 + 28);

    *(unsigned int *)0x80004000 = 0x40490FDB;

    float *at2 = (float *)0x80121000;
    sp[5] = 128;
    *(double *)(sp + 7) = *(double *)(at2 + 32);

    f16 = *(float *)(sp + 14);
    f18 = *(float *)(sp + 15);
    *(double *)(sp + 8) = (double)f18;

    sp[9] = 0x42FE0000;
}



// === func_8009C8F0 @ 0x8009C8F0 ===

void func_8009C8F0(void *arg) {
    u32 sp = (u32)arg;
    u32 ra = *(u32 *)(sp + 44);
    u32 t1 = 0;
    u32 t7 = *(u32 *)(sp + 232);
    u32 t6 = (t7 & 0xFFFF0000) >> 16;
    u32 v0 = (t7 & 0x0000FFFF);
    u32 s7 = *(u32 *)game_struct;
    u32 v1 = *(u32 *)(s7 + 22);

    t8 = v0 >> 10;
    t9 = t8 << 3;
    t7 &= 0x03FF;
    t8 <<= 2;
    t8 -= t7;

    t6 += t9;
    t6 = *(u32 *)(t6 + 25076);
    t8 <<= 2;
    t8 -= t7;
    t8 <<= 3;

    u32 a0 = t6 + t8;
    s1 = *(short *)(a0 + 22);

    if (v1 & 4) {
        s1--;
        if (s1 == 0) {
            t8 = 256;
        } else {
            if (!(v1 & 3)) {
                t8 = 256;
                goto label_0x8009C980;
            }
        }
    } else {
label_0x8009C980:
        t7 <<= 4;
        u32 t6 = a0 + t7;
        *(float *)(0x8012 + 20200) = *(float *)(t6 + 12);
        t7 &= 0x0004;

        if (v1 & 8) {
            t8 = 256;
        } else {
            t9 <<= 4;
            t7 = a0 + t9;
            *(float *)(0x8012 + 20200) = *(float *)(t7 + 28);
            if (v1 & 3) {
                t8 = v1 & v0;
                u32 t7 = v1 & 8;

                if (!t8) {
                    goto label_0x8009CB10;
                }
            } else {
label_0x8009CB10:
                if (!(t7 >> 3)) {
                    goto label_0x8009CA88;
                }

                u32 t6 = *(u32 *)car_array;
                u32 t8 = s7 + 24;
                u32 t9 = a1 + 24;

                t7 = (t8 & 3) * 4;
                float f8 = *(float *)(t6 + t7);
                float f10 = *(float *)(t9 + t7);
                *(float *)0x801514C0 = f8;

                t7 = a1 + 24;
                t8 = (t7 & 3) * 4;
                f8 = *(float *)(t6 + t8);
                float f6 = *(float *)(t9 + t8);
                *(float *)0x801514C0 = f10;
            }
        }
    }

    v1 >>= 2;
    t7 <<= 4;
    u32 t6 = a0 + t7;
    float f6 = *(float *)(t6 + 12);
}



// === func_8009DC50 @ 0x8009DC50 ===

int func_8009DC50(u32 a0, u32 a1) {
    int sp = -48;

    int s1 = (0x8013 << 16) | (256 - 6400);
    int s2 = 68;
    int s3 = a1;
    int s4 = a0 & 256;
    int ra = sp + 44;

    u32 t7 = *(u32 *)a0;
    u32 t8 = t7 & s4;
    short v0 = *(short *)(a0 + 22);

    if (v0) {
        return 1;
    }

    v0 = *(short *)(a0 + 24);
    if (v0) {
        return 1;
    }

    sp += 8;
    s1 += t9 << 5;
    a0 = s1 + t0;
    func_8009DC50(a0, s3);

    v0 = *(short *)(a0 + 24);
    if (v0) {
        return 1;
    }

    v0 = 0;

    sp -= 8;
    ra += 4;
    return v0;
}

int func_8009DD18(u32 a0, u32 a1) {
    int sp = -24;

    u32 t6 = *(u32 *)a0;
    u32 t7 = (256 << 16) | (t6 & 256);
    u32 t9 = t6 & t8;
    short v0 = *(short *)(a0 + 22);

    if (v0) {
        return 1;
    }

    v0 = *(short *)(a0 + 24);
    if (v0 < 0) {
        int t1 = (0x8013 << 16) | (-6400);
        v0 <<= 4;
        v0 += v0;
        v0 <<= 2;
        a0 = t1 + v0;
        func_8009DC50(a0, a1);
    }

    v0 = 0;

    sp += 4;
    ra += 4;
    return v0;
}



// === func_8009DD18 @ 0x8009DD18 ===

void func_8009DD18(void *arg0) {
    u16 state = *(u16 *)arg0;
    const u16 MASK = 256;

    if (state & MASK) {
        s16 value = *(s16 *)(arg0 + 22);
        if (value != 0) {
            value <<= 4;
            value += value;
            value <<= 2;
            func_8009DC50(value - 6400);
        }
    }
}

void func_8009DD88(void *arg0, void *arg1) {
    u16 state = *(u16 *)arg0;
    const u32 BASE_A = 0x8013 << 16;
    const u32 BASE_B = 0x8012 << 16;
    const u32 OFFSET_A = 20420;
    const u32 OFFSET_B = 20448;
    const s32 SUB_A = 6400;
    const u32 SHIFT_A = 4;
    const u32 SHIFT_B = 2;
    const u16 MASK = 1;

    if (state & MASK) {
        s16 value = *(s16 *)(arg0 + 24);
        if (value != 0) {
            value <<= 4;
            value += value;
            value <<= 2;
            func_8009DC50(value - SUB_A);
        }
    }
}



// === func_8009DD88 @ 0x8009DD88 ===

void func_8009DD88(u32 arg0, u32 arg1, CarData *car) {
    \n"
        "sw $fp, 80($sp)\n"
        "sw $s7, 76($sp)\n"
        "sw $s6, 72($sp)\n"
        "sw $s5, 68($sp)\n"
        "sw $s4, 64($sp)\n"
        "sw $s3, 60($sp)\n"
        "sw $s2, 56($sp)\n"
        "sw $s1, 52($sp)\n"
        "sw $s0, 48($sp)\n"
        "sdc1 $f20, 40($sp)\n"
        "sw %0, 280($sp)\n"
        "sw %1, 284($sp)\n"
        "lw $t7, 0(%0)\n"
        "sw $zero, 260($sp)\n"
        "sw $zero, 268($sp)\n"
        "lui $s0, 0x8013\n"
        "lui $s5, 0x8012\n"
        "lui $fp, 0x8012\n"
        "sw $t7, 276($sp)\n"
        "or $s2, %2, $zero\n"
        "or $s7, %3, $zero\n"
        "move $s1, $zero\n"
        "lw $v1, 0(%3)\n"
        "addiu $fp, $fp, 20420\n"
        "addiu $s5, $s5, 20448\n"
        "addiu $s0, $s0, -6400\n"
        "li $s3, 68\n"
        "lui $s4, 0x0004\n"
        "lui $s6, 0x0080\n"
        "sll $t9, $v1, 0\n"
        "or $t9, $zero, $t9\n"
        "lh $v0, 24($s7)\n"
        "beqz $v0, 0x8009E784\n"
        "lh $v0, 24($s7)\n"
        "lh $v0, 24($s7)\n"
        "lw $a0, 0($fp)\n"
        "lw $t6, 300($sp)\n"
        "bgez $v0, 0x8009DE34\n"
        :
        : "r" (arg0), "r" (arg1), "r" (car), "r" (&game_struct)
        : "$t7", "$t9", "$a0", "$t6"
    );
}



// === func_8009EA70 @ 0x8009EA70 ===

void process_car_data(CarData *car) {
    f32 f6;

    \n"
        "lw $a1, 36($sp)\n"
        "lui $at, 0x8012\n"
        "lw $t0, 15112($at)\n"
        "jal 0x80008730\n"
        "swc1 $f12, 32($sp)\n"
        "lwc1 $f12, 32($sp)\n"
        "jal 0x800088F0\n"
        "swc1 $f0, 24($sp)\n"
        "lwc1 $f16, 24($sp)\n"
        "li $v1, 0\n"
        "lw $v0, 36($sp)\n"
        "li $a0, 3\n"
        "lwc1 $f2, 0($v0)\n"
        "lwc1 $f12, 12($v0)\n"
        "addiu $v1, $v1, 1\n"
        "i-type op=0x11\n"
        "addiu $v0, $v0, 4\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"
        "swc1 $f14, -4($v0)\n"
        "i-type op=0x11\n"
        "bne $v1, $a0, 0x8009EAC4\n"
        "swc1 $f6, 8($v0)\n"
        "lw $ra, 20($sp)\n"
        "addiu $sp, $sp, 32\n"
        "jr $ra\n"
    );
}

void process_car_data_reverse(CarData *car) {
    f32 f6;

    \n"
        "lw $a1, 36($sp)\n"
        "lui $at, 0x8012\n"
        "lw $t0, 15120($at)\n"
        "jal 0x80008730\n"
        "swc1 $f12, 32($sp)\n"
        "lwc1 $f12, 32($sp)\n"
        "jal 0x800088F0\n"
        "swc1 $f0, 24($sp)\n"
        "lwc1 $f16, 24($sp)\n"
        "li $v1, 0\n"
        "lw $v0, 36($sp)\n"
        "li $a0, 3\n"
        "lwc1 $f2, 8($v0)\n"
        "lwc1 $f12, 0($v0)\n"
        "addiu $v1, $v1, 1\n"
        "i-type op=0x11\n"
        "addiu $v0, $v0, 12\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"
        "i-type op=0x11\n"
        "swc1 $f14, -12($v0)\n"
        "i-type op=0x11\n"
        "bne $v1, $a0, 0x8009EB6C\n"
        "swc1 $f6, -4($v0)\n"
    );
}



// === func_8009EB18 @ 0x8009EB18 ===

void func_8009EB18(int arg) {
    float f6;

    int v1 = *(int*)gstate_prev;
    func_80008730();
    float f12 = *(float*)32;
    func_800088F0();
    float f0 = *(float*)24;
    float f16 = *(float*)24;
    int v0 = *(int*)36;
    float f2 = *(float*)8;
    float f14 = *(float*)-12;

    for (int i = 0; i < 3; ++i) {
        *(float*)(v0 + 4 * i) = f2;
        *(float*)(v0 + 4 * i + 12) = f12;
        v0 += 16;
    }
}

void func_8009EBC0(int arg) {
    int v1 = car_array - 25728;
    int a0 = *(int*)v1;
    int t6 = -1;
    int ra = *(int*)28;
    int s0 = *(int*)24;
    int a2 = *(int*)144;
    int zero = 0;

    if (a0 != zero) {
        t6 = 0;
        v0 = a0 | 0x3F40;
        float f4 = *(float*)0;
        float f6 = *(float*)52;
        *(float*)60 = f8;
        float f16 = *(float*)56;
        float f10 = *(float*)4;
        *(float*)60 = f10;
        *(float*)64 = f18;
        *(float*)68 = f8;
        *(float*)80 = f16;
        *(float*)84 = f10;
        *(float*)88 = f4;
    }
}



// === func_8009EBC0 @ 0x8009EBC0 ===

void func_8009EBC0(void *arg0, f32 arg1) {

    \n"
        "lw $a0, 0($v1)\n"
        "addiu $t6, $zero, -1\n"
        "sw $ra, 28($sp)\n"
        "sw $s0, 24($sp)\n"
        "sw $a2, 144($sp)\n"
        "sw $zero, 132($sp)\n"
        "sw $zero, 128($sp)\n"
        "beq $a0, $zero, 0x8009F044\n"
        "sw $t6, 120($sp)\n"
        "lui $at, 0x3F40\n"
        "or $v0, $a0, $zero\n"
        "lw $f4, 0($a1)\n"
        "lw $f6, 52($v0)\n"
        "swc1 $f8, 60($sp)\n"
        "lw $f16, 56($v0)\n"
        "lwc1 $f10, 4($a1)\n"
        "lwc1 $f10, 60($sp)\n"
        "swc1 $f18, 64($sp)\n"
        "lw $f6, 60($v0)\n"
        "lw $f4, 8($a1)\n"
        "swc1 $f8, 68($sp)\n"
        "lw $f16, 80($v0)\n"
        "lwc1 $f10, 92($v0)\n"
        "lw $t5, 68($v0)\n"
        "swc1 $f18, 64($sp)\n"
        "lw $f2, 84($v0)\n"
        "lw $f6, 68($sp)\n"
        "swc1 $f8, 68($sp)\n"
        "lw $f4, 88($v0)\n"
        "lw $t5, 68($v0)\n"
        "lwc1 $f16, 100($v0)\n"
        "lw $t7, 120($sp)\n"
        "lw $f16, 96($v0)\n"
        "lw $t8, 132($sp)\n"
        "lwc1 $f16, 96($v0)\n"
        "swc1 $f16, 96($v0)\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "nop\n"
        "lw $t9, 0($v1)\n"
        "lwc1 $f18, 124($sp)\n"
        "swc1 $f18, 124($sp)\n"
        "lw $t5, 68($v0)\n"
        "lw $t9, 0($v1)\n"
    );

}



// === func_8009F064 @ 0x8009F064 ===

void func_8009F064(u32 param_1, u32 param_2) {
    s32 s3 = param_1 << 4;
    u32 a1 = param_2 + 36;
    s32 s1 = t6 + s3;

    u32 t7 = *(u32 *)(0x8018D9F8 + (param_2 & 0xFFFC));
    u32 t8 = (t7 << 3) + param_2;
    u32 t8 = (t8 << 3) + t7;
    s32 s0 = 0x80157F68 + (t8 << 3);
    u32 a1 = *(u32 *)s0;
    u32 a0 = s1;

    func_80007C68(s1, 16);

    f32 f4 = *(f32 *)(s0 + 36);
    s32 t6 = *(u32 *)0x80157F98;
    u32 t7 = (t6 << 1);
    *(u16 *)s1 = t7;

    f32 f8 = *(f32 *)(s0 + 40);
    t7 = (t6 << 1) + t6;
    *(u16 *)(s1 + 2) = t7;

    f32 f4 = *(f32 *)(s0 + 44);
    t6 = (t7 << 2);
    *(u16 *)(s1 + 8) = t6;

    u32 t9 = (t8 << 2) + t8;
    *(u16 *)(s1 + 10) = t9;

    s3 = *(u32 *)(4 + s0);
    s7 = *(u32 *)0x8015FEB8;
    u32 t7 = (t6 << 2) + t6;
    u32 a2 = *(u16 *)s3;

    if (a2 != 0) {
        *(u32 *)(4 + s7) = a2;
        *(u32 *)(s7) = s8;
    } else {
        *(u32 *)(4 + s7) = t9;
        *(u32 *)s7 = 0;
    }

    u32 v1 = *(u32 *)(s0 + 4);
    u32 v0 = *(u32 *)0x8015FEB8;
    u16 a2 = *(u16 *)v1;
    u16 a3 = *(u16 *)(v1 + 2);
    u16 t2 = *(u16 *)(v1 + 4);
    u16 t3 = *(u16 *)(v1 + 6);

    u32 t6 = v0 + 8;
    *(u32 *)v7 = t6;

    *(u32 *)s7 = t9;

    u32 a1 = *(u32 *)v7;
    u32 s7 = *(u32 *)0x8015FEB8;
    u32 t7 = (t6 << 4);
    *(u32 *)(4 + s7) = t9;

    u32 a1 = *(u32 *)v7;
    u32 a2 = *(u32 *)(s7 + 4);
    u32 t8 = v0 + 8;
    *(u32 *)(4 + s7) = t8;

    *(u32 *)s7 = 0;

    u32 t1 = *(u32 *)v7;
    u32 t6 = t1 + 8;

    if (a2 != 0) {
        *(u32 *)(4 + s7) = t6;
        *(u32 *)s7 = v0 + 8;
    } else {
        *(u32 *)(4 + s7) = t8;
        *(u32 *)s7 = 0;
    }
}



// === func_8009FDC4 @ 0x8009FDC4 ===

void func_8009FDC4(void) {
    u32 state = *(u32*)gstate;
    if (state == GSTATE_INIT || state == GSTATE_SETUP) {
        int index = -1;
        CarData* car = (CarData*)game_struct->car_array;
        int offset = 0;

        // Initialize dr_pos
        car->dr_pos[0] = 0.0f;
        car->dr_pos[1] = 0.0f;
        car->dr_pos[2] = 0.0f;

        // Initialize RWV
        car->RWV[0] = 0.0f;
        car->RWV[1] = 0.0f;
        car->RWV[2] = 0.0f;

        // Update state flags
        if (state == GSTATE_INIT) {
            index = 1;
            *(u32*)((char*)car + offset) |= 0xFFFFD9FE;
            *(u32*)((char*)car + offset + 4) = 0x00000000;
            offset += 8;
        }

        // Initialize dr_vel
        car->dr_vel[0] = 0.0f;
        car->dr_vel[1] = 0.0f;
        car->dr_vel[2] = 0.0f;

        if (state == GSTATE_SETUP) {
            *(u32*)((char*)car + offset) |= 0x0A01D9FF;
            *(u32*)((char*)car + offset + 4) = 0x00000001;
        }

        // Update previous state
        if (index != -1) {
            int prev_state = *(u8*)gstate_prev;
            prev_state |= index << 7;
            *(u8*)gstate_prev = prev_state;

            car->dr_uvs[0][0] = 0.0f;
            car->dr_uvs[0][1] = 0.0f;
            car->dr_uvs[0][2] = 0.0f;
            car->dr_uvs[1][0] = 0.0f;
            car->dr_uvs[1][1] = 0.0f;
            car->dr_uvs[1][2] = 0.0f;
            car->dr_uvs[2][0] = 0.0f;
            car->dr_uvs[2][1] = 0.0f;
            car->dr_uvs[2][2] = 0.0f;

            car->data_valid = 0;
            car->place = -1;
            car->laps = 0;
            car->checkpoint = 0;
        }
    }
}


