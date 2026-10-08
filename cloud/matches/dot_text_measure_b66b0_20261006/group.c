/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* The real menu caller supplies both native call sites; it is unclaimed context. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct OSMesgQueue_s OSMesgQueue;
typedef void *OSMesg;
#define NULL ((void *)0)
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
s32 osJamMesg(OSMesgQueue *, OSMesg, s32);
void audio_doppler_calc(u8 *,u16);
s32 func_80092DCC();
s32 func_800A473C();
void *memcpy(void *, const void *, unsigned int);
void func_800B6748(u8 *,f32 *,s16 *,s16 *,s8 *,s8 *,s8 *);
s32 func_800B66B0(u8 *,s16);
extern s8 D_8011473C;
extern f32 D_80114744;
extern f32 D_80114748;
extern s8 D_8011474C;
extern s32 D_801461D0;
extern s32 D_80149B48;
extern s8 D_80149B60;
extern s8 D_80149B70;
extern s16 D_80149D92;
extern s16 D_80149D9E;
extern s8 D_80149DA0;
extern s32 D_80149DD0;
extern u8 D_80149DD8[800];
extern s32 D_80153F60;
extern f32 D_80153F80;
extern s16 D_80153FD0;
extern s16 D_80154180;
extern s8 D_80154184;
extern s8 D_80154194;
extern s8 D_8015419C;

void menu_input_process(u8 *arg0, s16 arg1) {
    s32 sp170;
    u8 sp70[256];
    s32 needed;
    f32 temp_f0;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t6_3;
    s32 temp_t6_4;
    s32 temp_t7;
    s32 temp_t7_2;
    s32 temp_t7_3;
    s32 temp_t7_4;
    s32 temp_t7_5;
    s32 temp_t7_6;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 temp_t9_3;
    s32 temp_v0;
    s32 temp_v0_5;
    s32 var_v1;
    s8 temp_v0_2;
    s8 temp_v0_3;
    u32 temp_v0_4;

    if (D_80114744 >= 0.0f) {
        temp_v0=func_800B66B0(arg0,arg1);
        audio_doppler_calc(arg0,(u16)temp_v0);
        return;
    }
    if (D_8011473C != 0) {
        if (arg1 < 0) {
            func_800A473C(sp70, arg0);
        } else {
            func_80092DCC(sp70, arg0, arg1);
        }
    }
    osRecvMesg((OSMesgQueue *) &D_801461D0, NULL, 1);
    if (D_8011474C == 0) {
        D_8011474C = 1;
        D_80149DD0 = 0;
        func_800B6748((u8 *)&D_80153F60, &D_80153F80, &D_80153FD0, &D_80154180,&D_80154184,&D_80154194,&D_8015419C);
    }
    needed=0;
    if (D_80154184 != D_80149DA0) { needed+=2; }
    if (D_80154194 != D_80149B60) { needed+=2; }
    if (D_8015419C != D_80149B70) { needed+=2; }
    if ((D_80153FD0 != D_80149D92) || (D_80154180 != D_80149D9E)) { needed+=5; }
    if (D_80153F80 != D_80114748) { needed+=5; }
    if (D_80153F60 != D_80149B48) { needed+=5; }
    temp_v0 = func_800B66B0(arg0, arg1);
    var_v1 = D_80149DD0;
    if ((var_v1 + (needed + temp_v0 + 2)) >= 0x320) {
        osJamMesg((OSMesgQueue *) &D_801461D0, NULL, 0); return;
    }
    if (D_80154184 != D_80149DA0) {
        D_80154184 = D_80149DA0;
        temp_t7 = var_v1 + 1;
        *((u8 *) (D_80149DD8 + var_v1)) = 0;
        D_80149DD0 = temp_t7;
        *((u8 *) (D_80149DD8 + temp_t7)) = D_80149DA0;
        var_v1 = temp_t7 + 1;
        D_80149DD0 = var_v1;
    }
    temp_v0_2 = D_80149B60;
    if (D_80154194 != temp_v0_2) {
        D_80154194 = temp_v0_2;
        temp_t9 = var_v1 + 1;
        *((u8 *) (D_80149DD8 + var_v1)) = 1;
        D_80149DD0 = temp_t9;
        *((u8 *) (D_80149DD8 + temp_t9)) = temp_v0_2;
        var_v1 = temp_t9 + 1;
        D_80149DD0 = var_v1;
    }
    temp_v0_3 = D_80149B70;
    if (D_8015419C != temp_v0_3) {
        D_8015419C = temp_v0_3;
        temp_t7_2 = var_v1 + 1;
        *((u8 *) (D_80149DD8 + var_v1)) = 2;
        D_80149DD0 = temp_t7_2;
        *((u8 *) (D_80149DD8 + temp_t7_2)) = temp_v0_3;
        var_v1 = temp_t7_2 + 1;
        D_80149DD0 = var_v1;
    }
    if ((D_80153FD0 != D_80149D92) || (D_80154180 != D_80149D9E)) {
        *((u8 *) (D_80149DD8 + var_v1)) = 3;
        temp_t6 = var_v1 + 1;
        D_80149DD0 = temp_t6;
        temp_t6_2 = temp_t6 + 1;
        *((u8 *) (D_80149DD8 + temp_t6)) = (s8) ((s16) D_80149D92 >> 8);
        temp_t9_2 = temp_t6_2 + 1;
        *((u8 *) (D_80149DD8 + temp_t6_2)) = (s8) D_80149D92;
        D_80149DD0 = temp_t6_2;
        D_80149DD0 = temp_t9_2;
        temp_t9_3 = temp_t9_2 + 1;
        *((u8 *) (D_80149DD8 + temp_t9_2)) = (s8) ((s16) D_80149D9E >> 8);
        D_80149DD0 = temp_t9_3;
        var_v1 = temp_t9_3 + 1;
        *((u8 *) (D_80149DD8 + temp_t9_3)) = (s8) D_80149D9E;
        D_80149DD0 = var_v1;
        D_80153FD0 = D_80149D92;
        D_80154180 = D_80149D9E;
    }
    temp_f0 = D_80114748;
    temp_t7_3 = var_v1 + 1;
    if (D_80153F80 != temp_f0) {
        D_80153F80 = temp_f0;
        *((u8 *) (D_80149DD8 + var_v1)) = 4;
        temp_v0_4 = M2C_FIELD(&D_80114748,u32 *,0);
        D_80149DD0 = temp_t7_3;
        *((u8 *) (D_80149DD8 + temp_t7_3)) = (s8) (temp_v0_4 >> 0x18);
        temp_t7_4 = temp_t7_3 + 1;
        D_80149DD0 = temp_t7_4;
        *((u8 *) (D_80149DD8 + temp_t7_4)) = (s8) (temp_v0_4 >> 0x10);
        temp_t7_5 = temp_t7_4 + 1;
        D_80149DD0 = temp_t7_5;
        temp_t7_6 = temp_t7_5 + 1;
        *((u8 *) (D_80149DD8 + temp_t7_5)) = (s8) (temp_v0_4 >> 8);
        D_80149DD0 = temp_t7_6;
        var_v1 = temp_t7_6 + 1;
        *((u8 *) (D_80149DD8 + temp_t7_6)) = (s8) temp_v0_4;
        D_80149DD0 = var_v1;
    }
    temp_v0_5 = D_80149B48;
    if (D_80153F60 != temp_v0_5) {
        D_80153F60 = temp_v0_5;
        *((u8 *) (D_80149DD8 + var_v1)) = 5;
        temp_t6_3 = var_v1 + 1;
        D_80149DD0 = temp_t6_3;
        sp170 = temp_v0;
        memcpy((u8 *) (D_80149DD8 + temp_t6_3), &D_80149B48, 4);
        var_v1 = D_80149DD0 + 4;
        D_80149DD0 = var_v1;
    }
    *((u8 *) (D_80149DD8 + var_v1)) = 6;
    temp_t8 = var_v1 + 1;
    *((u8 *) (D_80149DD8 + temp_t8)) = (s8) temp_v0;
    D_80149DD0 = temp_t8;
    temp_t6_4 = temp_t8 + 1;
    D_80149DD0 = temp_t6_4;
    sp170 = temp_v0;
    memcpy((u8 *) (D_80149DD8 + temp_t6_4), arg0, temp_v0);
    D_80149DD0 += temp_v0;
    osJamMesg((OSMesgQueue *) &D_801461D0, NULL, 0); return;
}
s32 func_800B66B0(u8 *str, s16 max) {
    s32 count;
    u32 len;
    u8 c;

    count = 0;
    if (*str == 0xFF) {
        len = 1;
        do {
            len += 2;
            count++; if (str[len - 1] == 0 && str[len - 2] == 0) {
            break;
            }
        } while (max < 0 || count <= max);
    } else {
        len = 0;
        do {
            c = *str;
            len++;
            count++;
            str++;
            if (c == 0) {
                break;
            }
        } while (max < 0 || count <= max);
    }
    return len;
}

