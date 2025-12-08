/**
 * Rush 2049 - Ai Functions
 * AI drones, pathfinding, steering
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



// === func_800A11E4 @ 0x800A11E4 ===

#include <stdint.h>

void func_800A11E4(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
    uint8_t temp;
    uint32_t result;

    // First function call
    temp = *(uint8_t*)arg3;
    result = func_800A0FDC(arg0);
    subu(arg1, arg2, arg0);
    result = func_0x800962D4(result);
    move(arg1, 0);
    move(arg0, arg4);
    move(arg1, arg0);
    jal(0x800026C0);
    addiu(arg2, 0, 1);
    move(arg0, arg2);
    jal(0x80008590);
    subu(arg1, arg1, arg2);
    addiu(temp, temp, 1);
    *(uint8_t*)arg3 = temp;

    // Second function call
    func_800A1244(arg0, arg1, arg2, arg3);
}

void func_800A1244(uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3) {
    uint32_t t6, t7, s0, s1, s2, s3, s4;

    // Save registers
    sw(s4, 56);
    sw(s3, 52);
    sw(s2, 48);
    sw(s1, 44);
    sw(s0, 40);

    // Load known globals and constants
    lui(t6, 0x8012);
    lui(t7, 0x00BE);
    sw(ra, 60);
    addiu(t7, t7, 19568);
    addiu(t6, t6, 14432);
    lui(s0, 0x8038);
    lui(s1, 0x803A);
    lui(s2, 0x8039);
    lui(s3, 0x8012);
    lui(s4, 0x8003);

    // Recursive call
    jal(0x800A11E4);

    // Restore registers
    lw(t7, 16);
    lw(ra, 60);
    lw(s0, 40);
    lw(s1, 44);
    lw(s2, 48);
    lw(s3, 52);
    lw(s4, 56);

    return;
}



// === func_800A1244 @ 0x800A1244 ===

void func_800A1244() {
    u8 gstate = *(u8*)gstate;
    CarData* car_array = (CarData*)car_array;
    GameStruct* game_struct = (GameStruct*)game_struct;

    void (*func_800A12D4)();
    func_800A12D4 = (void (*)())*(u32*)0x8012A100;

    void (*func_800A133C)();
    func_800A133C = (void (*)())*(u32*)0x00BE9380;

    u32 sp = -64;
    func_800A12D4();
    func_800A133C();

    return;
}

void func_800A12D4() {
    s32 sp = -32;
    u32 ra;

    if (*(u32*)0x8012A100) {
        CarData* car_array = (CarData*)(*(u32*)0x801527B0);
        func_80007270((s32)car_array, 0);

        s32 address = *(u32*)0x8039A400;
        func_80095FD8(address, 0);

        func_800075E0((s32)car_array, 0);
    }

    return;
}

void func_800A133C() {
    u8 gstate = *(u8*)gstate;
    CarData* car_array = (CarData*)car_array;
    GameStruct* game_struct = (GameStruct*)game_struct;

    void (*func_800A12D4)();
    func_800A12D4 = (void (*)())*(u32*)0x8012A100;

    void (*func_800A133C)();
    func_800A133C = (void (*)())*(u32*)0x00BE9380;

    s32 sp = -64;
    func_800A12D4();
    func_800A133C();

    return;
}



// === func_800A12D4 @ 0x800A12D4 ===

#include <stdint.h>

void func_800A12D4(uint32_t arg0) {
    \n"
        "sw      s1, 24(%0)\n"
        "beq     t6, zero, 0x1E4\n"
        "sw      s0, 20(%0)\n"
        "lui     a0, 0x8015\n"
        "addiu   a0, a0, 10096\n"
        "move    a1, zero\n"
        "jal     0x80007270\n"
        "addiu   a2, zero, 1\n"
        "lui     a1, 0x8039\n"
        "addiu   a1, a1, -23552\n"
        "jal     0x80095FD8\n"
        "move    a2, zero\n"
        "lui     a0, 0x8015\n"
        "addiu   a0, a0, 10096\n"
        "move    a1, zero\n"
        "jal     0x800075E0\n"
        "move    a2, zero\n"
        "lui     at, 0x8012\n"
        "sb      zero, -4864(at)\n"
        "lw      ra, 28(%0)\n"
        "lw      s0, 20(%0)\n"
        "lw      s1, 24(%0)\n"
        "jr      ra\n"
        ".align 2\n"
        "0x1E4:\n"
        "lw      $sp, 32(%0)\n"
    );
}

void func_800A133C(uint32_t arg0) {
    \n"
        "sw      s3, 52(%0)\n"
        "sw      s2, 48(%0)\n"
        "sw      s1, 44(%0)\n"
        "sw      s0, 40(%0)\n"
        "lui     t6, 0x8012\n"
        "lui     t7, 0x00BE\n"
        "sw      ra, 60(%0)\n"
        "addiu   t7, t7, -24320\n"
        "addiu   t6, t6, 14416\n"
        "lui     s0, 0x8038\n"
        "lui     s1, 0x803C\n"
        "lui     s2, 0x803C\n"
        "lui     s3, 0x8012\n"
        "lui     s4, 0x8003\n"
        "lui     a2, 0x803B\n"
        "ori     a2, a2, 0xB380\n"
        "lw      s4, -20448(s4)\n"
        "addiu   s3, s3, -4864\n"
        "addiu   s2, s2, -26032\n"
        "addiu   s1, s1, -20960\n"
        "ori     s0, s0, 0xA400\n"
        "sw      t6, 4(%0)\n"
        "jal     0x800A11E4\n"
        "sw      t7, 16(%0)\n"
        "lw      ra, 60(%0)\n"
        "lw      s0, 40(%0)\n"
    );
}



// === func_800A133C @ 0x800A133C ===

#include <stdint.h>

void func_800A133C(uint32_t player_index) {
    uint32_t sp = -64;
    uint32_t s4, s3, s2, s1, s0, t6, t7;

    t6 = 0x80120000 + (player_index * 4);
    t7 = 0x00BE0000 - 24320;
    s0 = 0x80380000;
    s1 = 0x803C0000;
    s2 = 0x803C0000;
    s3 = 0x80120000;
    s4 = 0x80030000;
    uint32_t a2 = 0x803B0000 + 0xB380;

    car_array[player_index].unk_A400 = player_index * 6416;
    func_800A11E4(s4);

    sp = -88;
    uint32_t v0 = 0x80110000 + 6476;
    uint32_t at = 0x80120000;

    sb(0, -5404);
    t6 = lb(v0);
    ra = sp + 44;
    s4 = sp + 40;
    s3 = sp + 36;
    s2 = sp + 32;
    s1 = sp + 28;

    if (t6 != 0) {
        s0 = sp + 24;
        t7 = 1;
        uint32_t a0 = 0x80150000 - 26672;
        uint32_t a1 = 0x80150000 + 10212;
        sb(t7, v0);
        func_0x80006A00(a2, 1, a0);
        func_0x800075E0(a0, 0);
        func_0x80007270(0x80150000 - 26672, sp + 68, 1);
    } else {
        s0 = sp + 24;
        uint32_t a0 = 0x80150000 - 26672;
        sb(1, v0);
        func_0x80006A00(a2, 1, a0);
        func_0x800075E0(a0, 0);
    }

    s0 = 0x80140000;
    s4 = 0x80030000;
    s4 += 21592;
    s0 += 16432;
}



// === func_800A13E8 @ 0x800A13E8 ===

void func_800A13E8(void) {
    s32 *sp = (s32 *)__builtin_alloca(88);
    u32 *gstate_ptr = gstate + 6476;
    sp[1] = (u32)gstate_ptr;

    char t6 = *gstate_ptr;

    if (t6 != 0) {
        sp[5] = 1;
        func_80006A00((void *)(car_array - 26672), (void *)0, 1);
        func_800075E0((void *)(car_array - 26672), 0, 0);
    } else {
        u32 t7 = 1;
        sp[4] = t7;
        func_80006A00((void *)(car_array - 26672), (void *)0, 1);
        func_800075E0((void *)(car_array - 26672), 0, 0);
    }

    u32 a0 = car_array - 26672;
    void *a1 = sp + 17;

    func_80007270(a0, a1, 1);

    s32 game_struct_ptr = game_struct + 16432;
    s32 pad_struct_ptr = 0x8003545C + 21592;
    sp[7] = 0;
    u32 s3 = 4;

    char t8 = *game_struct_ptr;
    if (t8 != 0) {
        a0 = pad_struct_ptr;
    }

    while (sp[7] < s3) {
        game_struct_ptr[125] = 0;
        func_8000A194(pad_array);
        func_80009F20(0, 0);
        sp[7]++;
        if (sp[7] == s3) break;
        game_struct_ptr += 772;
    }

    func_800075E0(a0, 0, 0);

    sp[4] = __builtin_return_address(0);
    sp[5] = 1;

    if (t6 != 0) {
        *(u32 *)0x8012EAE4 = 0;
        *sp = *(s32 *)(a0 + 8);
    }

    return;
}



// === func_800A153C @ 0x800A153C ===

void func_800A153C(u8 *arg0, u8 *arg1, u8 arg2) {
    u8 *car_array = (u8 *)car_array;
    struct GameStruct *game_struct = (struct GameStruct *)game_struct;

    while (*arg1 != 0) {
        if (*arg1 < arg2) {
            u32 index = ((u32)arg0 >> 8) + 256;
            u32 *temp = (u32 *)(car_array - 2697);
            temp[index] = 1;

            while (*arg1 != 0) {
                if (*arg1 < arg2) {
                    arg0++;
                    index++;
                    if (++temp[index] == 256) break;
                }
                arg1--;
            }

            arg0 += 2;
            arg1 = (u8 *)car_array + *arg0;
        } else {
            arg0++;
        }
    }
}



// === func_800A1648 @ 0x800A1648 ===

int func_800A1648(int param1, int param2) {
    u8* car_data = (u8*)param1;
    int a3 = param2 - 1;

    while (a3 < 0) {
        int t0 = param1 + a3 * 32;
        int v1 = 15;
        u8 a2 = *(u8*)(t0);

        do {
            a2 = ((v1 & 0x0F) + a2) % 256;
            v1--;
        } while (v1 >= 0);

        t0 -= 4;
        int t7 = a2 & 0xFF;

        if (t7 < 0) {
            *(u8*)(a3 * 32 + 32) = 0xFF;
        }

        if ((t7 & 0x00FF) == 65) {
            *(u8*)(a3 * 32 + 32) = a2;
            return param1 + 1;
        }

        a3++;
        if (a3 >= param2) {
            break;
        }
    }

    if (*(u8*)param1 == 0xFF) {
        *(u8*)param1 = 255;
        return param1 + 1;
    }

    int a4 = 0;
    if (param2 <= 0) {
        goto end;
    }

    while ((a2 & 3) != 0) {
        param1 += a2 & 3;
        a4++;
        if (a4 > 255) {
            break;
        }
    }

    u16* t10 = (u16*)(game_struct + 0x8012EAEC - 5396);
    for (int i = 0; i < param2; i++) {
        *(t10 + i * 4) = (*t10 + i * 2) >> 8;
        if (*(t10 + i * 4) == a4) {
            break;
        }
    }

end:
    return param1 + 1;
}



// === func_800A1660 @ 0x800A1660 ===

void func_800A1660(u32 arg0, u32 arg1, u32 arg2) {
    CarData *car_array = (CarData *)car_array;
    GameStruct *game_struct = (GameStruct *)game_struct;

    for (int i = 0; i < 15; i++) {
        u32 car_index = (arg1 >> i) & 0xFF;

        if ((car_index & 0xFE) == 0x7E) {
            car_array[arg0 + i].gstate = game_struct->gstate;
        } else if ((car_index & 0xFE) == 0x66) {
            car_array[arg0 + i].gstate = car_array[arg1 + i].gstate;
        }
    }

    u32 t3 = 255;
    car_array[arg0].gstate = t3;

    if (arg2 > 0) {
        arg2 &= 3;
        if (arg2 == 2) {
            u8 *t4 = (u8 *)0x8012EAEC;
            u8 t5 = *(t4 + 1);
            u8 t6 = (t5 << 1) & 0xFF;
            u32 t7 = ((t6 | 0x8012EAEC) * 0x01010101) >> 24;
            car_array[arg0 + 3].gstate = t7;
        }
    }
}



// === func_800A17F4 @ 0x800A17F4 ===

void func_800A17F4(u8 *output_buffer, u8 *index_array, u8 current_index) {
    CarData car_array[8];
    GameStruct game_struct;

    while (*index_array != 0xFF) {
        u32 index = (*index_array << 1) + game_struct.unk_1C;
        u16 value = car_array[index].mph; // Assuming mph is stored as a u16 for simplicity

        if (value & 0xFF00) {
            output_buffer[-3] = (value >> 8);
        }

        if (current_index != *index_array) {
            break;
        }

        output_buffer[-2] = (value & 0x00FF);
        output_buffer++;
        index_array++;

    }

    if (*index_array == 0xFF) {
        output_buffer[0] = 0;
    } else {
        output_buffer[0] = *index_array;
    }
}



// === func_800A19E4 @ 0x800A19E4 ===

#include <stdint.h>

void update_counter(uint8_t *counter) {
    (*counter)--;
}

uint8_t compare_counters(uint8_t current, uint8_t previous) {
    if (current == previous) {
        return 0;
    } else {
        return current - previous;
    }
}

uint32_t process_game_state(uint32_t state_flag) {
    uint32_t sp = 0xFFFFF50C; // Adjust stack pointer for function call
    uint32_t ra = (sp + 20);
    *((volatile uint32_t *)ra) = ra;

    uint8_t current_counter = *((uint8_t *)state_flag);

    uint32_t offset = ((current_counter << 2) - current_counter);
    offset <<= 6;
    offset += 0x8014 + (offset << 2);
    uint8_t counter_value = *((uint8_t *)(offset + 16));

    if (counter_value != 0) {
        goto func_800A1BA4;

    func_800A1AB4:
        uint32_t flags_offset = 0x8011 + 6476;
        uint8_t flags_value = *((uint8_t *)flags_offset);
        if (flags_value == 0) {
            goto func_800A1BA4;

        func_800A1AF8:
            uint32_t cars_offset = 0x8015 - 26672;
            uint8_t temp = *((uint8_t *)cars_offset);
            *((uint8_t *)cars_offset) = (temp + 1);

            );
            *((uint32_t *)sp + 4) = 1;
            );
            *((uint32_t *)sp + 4) = 0;
            );
            *((uint32_t *)sp + 6) = offset;

            uint8_t *arg1 = (uint8_t *)(sp + 44);
            uint8_t *arg2 = (uint8_t *)(sp + 64);
            );

        func_800A1BA4:
            return 0;

        }

    }

    return current_counter;
}



// === func_800A1A60 @ 0x800A1A60 ===

void func_800A1A60(u32 arg0) {
    s32 sp = -112;
    u32 ra = arg0 + 20;

    CarData* car_data = (CarData*)(*(u32*)arg0);
    u32 state_ptr = game_struct + 0x40D0;
    u8 current_state = *(u8*)gstate;

    if (current_state != 0) {
        goto addr_800A1BA4;
    }

    s32 player_index = (*(u32*)(car_data + 16)) & 0xFF;
    s32 lap_count = (*(u32*)(car_data + 17)) & 0xFF;

    u32 player_offset = (player_index << 2) - player_index;
    player_offset <<= 6;

    u32 lap_offset = lap_count << 2;
    lap_offset += player_offset;
    lap_offset <<= 2;

    u32 player_data_ptr = state_ptr + 0x40D0;
    u32 player_data_valid = *(u8*)player_data_ptr;

    if (player_data_valid != 0) {
        goto addr_800A1BA4;
    }

    u32 lap_offset2 = (player_index << 2) - player_index;
    lap_offset2 <<= 6;

    u32 lap_data_ptr = state_ptr + 64;
    lap_offset2 += *(u32*)lap_data_ptr;
    lap_offset2 <<= 2;

    u32 lap_state_ptr = state_ptr + 100;
    lap_offset2 += lap_count;
    lap_offset2 <<= 2;
    lap_offset2 += player_index;
    lap_offset2 <<= 3;

    u32 final_offset = lap_offset2 + lap_data_ptr;
    u32 final_data_ptr = state_ptr + 8C;

    s32 v0_3 = 1;
    goto addr_800A1BA4;

addr_800A1BA4:
    // Function continues from here
}



// === func_800A1D38 @ 0x800A1D38 ===

void func_800A1D38(u32* arg0, u32* arg1, u32 arg2, u32 arg3) {
    u32 t7 = *arg0 + *arg1;
    *arg2 = t7;

    if (t7 == 0) {
        return;
    }

    *arg1 = arg2[0];
    ++arg0;
    ++arg1;

    while (*arg0 != 0) {
        *arg1 = arg3;
        ++arg0;
        ++arg1;
        ++arg3;
        ++arg1;
    }

    u8 t6 = *arg3 & 0xFF;
    u8 t9 = *(arg1 + 19);
    u32 t8 = (t9 << 2) - t9;
    t8 <<= 6;
    t8 += (t9 << 2) + t9;
    t7 += *arg1 + t8;
    ++arg0;
    ++arg1;

    u16 t6_2 = *(t7 + 136);
    u32 t8_2 = arg2[2];
    u32 t9_2 = arg2[3];

    if (t8_2 > t9_2) {
        u32 t0 = t8_2;
        u32 t1 = t9_2;
    } else {
        u32 t0 = t9_2;
        u32 t1 = t8_2;
    }

    u32 t6_3 = arg2[4];
    u32 t7_2 = *(t0 + 64);
    u32 t9_3 = *(t1 + 64);

    if (t7_2 > t9_3) {
        u32 t0 = t7_2;
        u32 t1 = t9_3;
    } else {
        u32 t0 = t9_3;
        u32 t1 = t7_2;
    }

    u32 t8_3 = t0 - t1;
    arg2[5] = t8_3;

    if (t8_3 == 0) {
        return;
    }

    u32 t2 = t8_3 >> 5;
    u32 t6_4 = t2 >> 5;
    u32 t7_3 = arg2[6];
    u32 t9_4 = arg2[7];

    if (t7_3 > t9_4) {
        u32 t0 = t7_3;
        u32 t1 = t9_4;
    } else {
        u32 t0 = t9_4;
        u32 t1 = t7_3;
    }

    u32 t6_5 = arg2[8];
    u32 t7_4 = *(t0 + 64);
    u32 t9_5 = *(t1 + 64);

    if (t7_4 > t9_5) {
        u32 t0 = t7_4;
        u32 t1 = t9_5;
    } else {
        u32 t0 = t9_5;
        u32 t1 = t7_4;
    }

    u32 t8_4 = t0 - t1;
    arg2[9] = t8_4;

    if (t8_4 == 0) {
        return;
    }
}



// === func_800A1F18 @ 0x800A1F18 ===

#include <stdint.h>

int func_800A1F18(uint32_t a2) {
    if (a2 == 0) {
        return 1;
    }

    uint32_t *v0 = (uint32_t *)a2;
    uint8_t s6 = v0[4];
    uint8_t a3 = v0[5];
    uint32_t v1 = v0[18];

    int t6 = (s6 << 2) - (s6 >> 2);
    t6 <<= 6;
    int t7 = (a3 << 2) + (s6 >> 2);
    t7 <<= 3;

    t6 <<= 2;
    int t8 = t6 + t7;
    int t0 = t8 + ((*(uint8_t *)v1 & 0xFF) == 0);

    if (*(uint8_t *)(t0 + 133)) {
        return 1;
    }

    uint32_t a0 = v1[0];
    uint32_t s1 = (t0 * 4) + 16432;

    if (*(uint32_t *)a0 == 0) {
        return 1;
    }

    uint32_t a1 = v1[16];
    int t9 = s1 + 12;
    uint8_t *gstate = (uint8_t *)0x801146EC;

    a0 = *(uint32_t *)a1;
    a1 = v1[2];

    t6 = (s6 << 2) - (s6 >> 2);
    t6 <<= 6;
    t7 = (a3 << 2) + (s6 >> 2);
    t7 <<= 3;

    t6 <<= 2;
    t8 = t6 + t7;

    uint32_t a3 = *(uint32_t *)t0;
    uint8_t *gstate_prev = (uint8_t *)0x801146ED;

    return 0;
}


