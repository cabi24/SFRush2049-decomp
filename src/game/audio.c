/**
 * Rush 2049 - Audio Functions
 * Sound effects, music playback
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



// === func_800B3710 @ 0x800B3710 ===

void func_800B3710(u32 arg0, CarData *car, s32 arg2, u16 arg3) {
    if (arg0 < 200) {
        return;
    }

    u32 car_index = arg0 - 1;
    CarData *car_array_ptr = car_array + car_index;
    PadEntry *pad_entry = pad_array[car_index];
    
    s8 prev_place = pad_entry->place;

    pad_entry->place = car->place;
    pad_entry->laps = car->laps;
    pad_entry->checkpoint = car->checkpoint;
    pad_entry->difficulty = car->difficulty;

    *(u16 *) (pad_entry + 2) = arg3;

    u8 *temp = (u8 *) &game_struct->field_0x9450;
    temp[car_index * 4] = 1;
    temp = &game_struct->field_0x9454;
    *(CarData **) temp = car_array_ptr;
}



// === func_800B37E8 @ 0x800B37E8 ===

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

#define GSTATE_INIT 0x40000
#define GSTATE_SETUP 0x80000
#define GSTATE_MENU 0x100000
#define GSTATE_CAMERA 0x200000
#define GSTATE_READY 0x400000
#define GSTATE_PLAY 0x800000
#define GSTATE_CARS 0x1000000
#define GSTATE_FINISH 0x2000000

void func_800B37E8(u32 a0, u32 a1, u32 a2, u32 a3) {
    s32 sp = -64;

    CarData *car0 = &car_array[a0];
    CarData *car1 = &car_array[a1];

    u32 t8 = (a3 << 16);
    u32 t0 = (a0 << 16);
    u32 t1 = (a1 << 16);

    if (t8 > 0) {
        a3 = a2;
        if (a0 <= 0) {
            return;
        }

        u16 *sp_ptr = (u16 *)(sp + 36);
        *sp_ptr++ = t0 >> 16;
        *sp_ptr++ = t1 >> 16;

        sp_ptr--;

        s32 s7_val = -1;
        u16 *sp_ptr2 = (u16 *)sp_ptr;
        u16 t0_val = *sp_ptr2++;
        u16 t1_val = *sp_ptr2++;

        u16 t9 = car_array[t0_val].field_4;
        u16 t2 = car_array[t1_val].field_6;

        u32 t3 = car_array[t0_val].field_20;
        u32 a0_val = car_array[t0_val].field_0;

        u32 a1_val = t9 + t1_val;
        u32 a2_val = t2 + t1_val;

        func_800B3704(a0, a1, a2, a3);

        u32 t4 = (t3 & 0x8000);
        car0->field_24 = t4;
        car0->field_18 = (t3 >> 16);
        car0->field_32 = (t3 & 0xFFFF);
        car0->field_20 = (car0->field_16 >> 16);
        car0->field_28 = (car0->field_12 >> 16);
        car0->field_34 = ((t3 & 0xFFFF) << 16);
        car0->field_30 = ((t3 >> 16) << 16);

        u16 v1 = car_array[t0_val].field_8;
        if (v1 < 0) {
            car0->field_...




// === func_800B39BC @ 0x800B39BC ===

void func_800B39BC(u8 state, u8* car_index, s32 checkpoint, CarData* car_array) {
    s32 pad_entry_offset = 0;
    if (state < checkpoint) {
        *car_index ^= checkpoint ^ state;
        state = *car_index;
        *car_index = checkpoint;
        checkpoint ^= state;
    }
    u8* car_data_ptr = &car_array[checkpoint].dr_pos[1];
    s32 temp_v0 = 0;
    if (pad_entry_offset < 5) {
        *(s16*)(car_data_ptr + 14) = *car_index;
        car_data_ptr += 4;
        *(s16*)(car_data_ptr + 16) = checkpoint;
        car_data_ptr += 4;
        temp_v0 = (u8*)car_data_ptr - (u8*)&car_array[checkpoint].mph;
    }
    *(s32*)(car_data_ptr + 20) = temp_v0;

    CarData* current_car = &car_array[state];
    current_car->dr_uvs[1][2] = 1;
    current_car->dr_uvs[2][0] = 1;
    current_car->data_valid = 1;
}



// === func_800B3B4C ===

typedef struct CarData {
    f32 dr_pos[3];      // 0x00: Dead reckoned position
    f32 dr_vel[3];      // 0x0C: Dead reckoned velocity
    f32 dr_uvs[3][3];   // 0x18: Dead reckoned orientation
    f32 RWV[3];         // Real world velocity
    f32 RWR[3];         // Real world position
    f32 mph;            // Speedometer reading
    s16 rpm;            // Engine RPM
    u8 data_valid;      // Data updated flag
    s8 place;           // Race position
    s8 laps;            // Current lap
    s8 checkpoint;      // Current checkpoint
    s8 difficulty;      // AI difficulty (DRONE_HUMAN=2)
} CarData;

#define GSTATE_INIT     0x40000
#define GSTATE_SETUP    0x80000
#define GSTATE_MENU     0x100000
#define GSTATE_CAMERA   0x200000
#define GSTATE_READY    0x400000
#define GSTATE_PLAY     0x800000
#define GSTATE_CARS     0x1000000
#define GSTATE_FINISH   0x2000000

void func_800B3B4C(u8* arg0, u32 arg1, u32 arg2, u32 arg3) {
    \n"
        "sw $fp, 64($sp)\n"
        "sw $s1, 36($sp)\n"
        "lui $s2, 0x8011\n"
        "or $fp, $a0, zero\n"
        "sw $ra, 68($sp)\n"
        "sw $s7, 60($sp)\n"
        "sw $s6, 56($sp)\n"
        "sw $s5, 52($sp)\n"
        "sw $s4, 48($sp)\n"
        "sw $s3, 44($sp)\n"
        "sw $s0, 32($sp)\n"
        "or $t0, $a1, zero\n"
        "or $t1, $a2, zero\n"
        "or $t2, $a3, zero\n"
        "addiu $s2, $s2, 28132\n"
        "move $s1, zero\n"
        "lb $t6, 0($s2)\n"
        "beqz $t6, 0x800B3B94\n"

        "li $at, 16\n"
        "addiu $s1, $s1, 1\n"
        "bltz $s1, 0x800B3B94\n"

        "addiu $s2, $s2, 32\n"
        "li $at, 16\n"
        "beqz $s1, 0x800B3BC4\n"

        "lw $s3, 88($sp)\n"
        "bne $zero, $zero, 0x800B3CDC\n"

        "move $v0, $s2\n"
        "slt $at, $t1, $fp\n"
        "beqz $at, 0x800B3BDC\n"

        "move $s1, zero\n"
        "xor $fp, $fp, $t1\n"
        "xor $t1, $t1, $fp\n"
        "xor $fp, $fp, $t1\n"

        "slt $at, $t2, $t0\n"
        "beqz $at, 0x800B3BF4\n"

        "li $t7, 16\n"
        "xor $t0, $t0, $t2\n"

        "j 0x800B3CDC\n"

        ".align 2\n"
        "0x800B3B94:\n"
        "addiu $s2, $s2, 32\n"
        "li $at, 16\n"
        "beqz $s1, 0x800B3BC4\n"

        "lw $s3, 88($sp)\n"
        "bne $zero, $zero, 0x800B3CDC\n"

        "move $v0, $s2\n"
        "slt $at, $t1, $fp\n"
        "beqz $at, 0x800B3BDC\n"

        "move $s1, zero\n"
        "xor $fp, $fp, $t1\n"
        "xor $t1, $t1, $fp\n"
        "xor $fp, $fp, $t1\n"

        "slt $at, $t2, $t0\n"
        "beqz $at, 0x800B3BF4\n"

        "li $t7, 16\n"
        "xor $t0, $t0, $t2\n"

        "j 0x800B3CDC\n"

        ".align 2\n"
        "0x800B3BC4:\n"
        "move $v0, $s2\n"

        ".align 2\n"
        "0x800B3BDC:\n"
        "jr $ra\n"
    );
}



// === func_800B3D20 @ 0x800B3D20 ===

void func_800B3D20(u32 arg0) {
    s32 index = arg0;
    CarData* car = &car_array[index];
    u16 game_state = *(u16*)gstate;

    if (game_state != GSTATE_INIT && game_state != GSTATE_SETUP) {
        return;
    }

    CarData* prev_car = &car_array[3]; // Assuming index 3 is the previous car
    u8 current_lap = car->laps;
    u8 prev_lap = prev_car->laps;

    if (current_lap != prev_lap) {
        prev_car->laps = current_lap;
        prev_car->data_valid = 1;
    }

    u8 next_checkpoint = car->checkpoint + 1;
    if (next_checkpoint > 3) {
        next_checkpoint = 0;
    }

    CarData* next_car = &car_array[next_checkpoint];
    s16 current_difficulty = car->difficulty;
    s16 prev_difficulty = next_car->difficulty;

    if (current_difficulty != prev_difficulty) {
        next_car->difficulty = current_difficulty;
        next_car->data_valid = 1;
    }
}



// === func_800B3F00 @ 0x800B3F00 ===

#include <stdint.h>

// GLOBALS
extern uint8_t gstate;
extern uint8_t gstate_prev;
extern CarData car_array[8];
extern GameStruct game_struct;
extern uint32_t frame_counter;
extern PadEntry pad_array[4];

typedef struct CarData {
    float dr_pos[3];      // 0x00: Dead reckoned position
    float dr_vel[3];      // 0x0C: Dead reckoned velocity
    float dr_uvs[3][3];   // 0x18: Dead reckoned orientation
    float RWV[3];         // Real world velocity
    float RWR[3];         // Real world position
    float mph;            // Speedometer reading
    int16_t rpm;          // Engine RPM
    uint8_t data_valid;   // Data updated flag
    int8_t place;          // Race position
    int8_t laps;           // Current lap
    int8_t checkpoint;    // Current checkpoint
    int8_t difficulty;     // AI difficulty (DRONE_HUMAN=2)
} CarData;

#define GSTATE_INIT     0x40000
#define GSTATE_SETUP    0x80000
#define GSTATE_MENU     0x100000
#define GSTATE_CAMERA   0x200000
#define GSTATE_READY    0x400000
#define GSTATE_PLAY     0x800000
#define GSTATE_CARS     0x1000000
#define GSTATE_FINISH   0x2000000

uint8_t getGameStateFlag(uint8_t index) {
    uint32_t offset = 0x80152818;
    return ((uint8_t*)offset)[index];
}

int main() {
    uint32_t sp = 0xFFFFFDDC; // Initial stack pointer value
    uint32_t ra = sp + 20;     // Return address on the stack

    // Call func_800B3D18 with gstate as argument
    func_800B3D18(gstate);

    // Get the state flag at index 2
    uint8_t gameStateFlag2 = getGameStateFlag(2);
    return 0;
}

// Placeholder for func_800B3D18
void func_800B3D18(uint8_t arg) {
    // Function implementation here
}



// === func_800B3F28 ===

u8 getGameState() {
    u8 *state = (u8 *)gstate;
    return state[3];
}


// === func_800B3F50 @ 0x800B3F50 ===

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
    float dr_pos[3];      // Dead reckoned position
    float dr_vel[3];      // Dead reckoned velocity
    float dr_uvs[3][3];   // Dead reckoned orientation
    float RWV[3];         // Real world velocity
    float RWR[3];         // Real world position
    float mph;            // Speedometer reading
    int16_t rpm;          // Engine RPM
    uint8_t data_valid;   // Data updated flag
    int8_t place;         // Race position
    int8_t laps;           // Current lap
    int8_t checkpoint;   // Current checkpoint
    int8_t difficulty;   // AI difficulty (DRONE_HUMAN=2)
} CarData;

CarData* car_array = (CarData*)0x80152818;
uint32_t frame_counter = *(volatile uint32_t*)0x80142AFC;
uint8_t gstate = *(volatile uint8_t*)0x801146EC;

int func_800B3F50(void) {
    uint8_t state = *gstate;
    CarData* car = &car_array[0]; // Assuming the first car is relevant
    int16_t rpm = car->rpm;
    int8_t place = car->place;

    return (int)((uint16_t)(rpm + place) << 16) >> 16;
}

int func_800B3FA4(uint32_t param) {
    uint32_t idx = param >> 16;
    CarData* car = &car_array[idx];
    int16_t rpm = car->rpm;
    int8_t place = car->place;

    return (int)((uint16_t)(rpm + place) << 16) >> 16;
}



// === func_800B3FA4 @ 0x800B3FA4 ===

void func_800B3FA4(u32 arg0) {
    s32 sp = -64;
    u32 temp_t4 = (arg0 << 16) >> 16;
    s16 temp_s5 = temp_t4;
    void* ra = __builtin_return_address(0);
    CarData* car_array = &car_array[temp_t4];
    CarData* current_car = car_array + 1;
    u8 place = 1;

    func_800B3D18(arg0);

    if (*(u8*)arg0 != 255) {
        u32 temp_t5 = *(u8*)(arg0 + 1);
        u32 temp_t3 = 1;
        while (temp_t3 == 1) {
            place = 2;
            temp_t5 = arg0 + 1;
            temp_t3 = 0;
            if (sp < 0) {
                s32 temp_a3 = -1;
                if (temp_t4 <= 0) {
                    return;
                }
            } else {
                u8 temp_a2 = *(u8*)temp_t5;
                if (temp_a2 == 0) {
                    temp_t3 = 1;
                }
                u8 temp_t8 = *(u8*)(temp_t5 + 1);
                if (temp_t8 != 0) {
                    return;
                }
            }
        }
    }

    *car_array->place = place;
}


