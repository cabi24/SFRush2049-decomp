typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef float f32;

typedef struct {
    char pad0[0xEE];
    s8 place;
    s8 finished;
    f32 time;
    char padF4[0x3B8 - 0xF4];
} Car;

typedef struct {
    char pad0[0x7C6];
    s16 slot;
    char pad7C8[4];
    s8 active;
    char pad7CD[0x808 - 0x7CD];
} Model;

typedef struct {
    u8 slot;
    char pad1[0x4B];
} Pad;

extern Car D_80152818[];
extern Model D_8014A250[];
extern Pad D_8014A118[];
extern u8 D_801439D8[6][8];
extern f32 D_801543CC;
extern s8 D_80142DB4[];
extern s8 D_80152744;
extern volatile s16 D_80153FD2;
extern s16 D_80142D70;

s32 func_800D2128(f32 value, u8 *digits, u8 format);

s32 func_80105B74(s32 arg0) {
    s16 order[8];
    s32 k;
    s16 i;
    s16 j;
    s16 idx;
    s16 num;

    for (k = 0; k < 4; k++) {
        func_800D2128((D_80152818[k].finished == 1) ? D_80152818[k].time : D_801543CC, D_801439D8[k], 'f');
    }
    for (i = 0; i < 6; i++) {
        D_80142DB4[i] = -1;
    }
    num = D_80152744;
    for (i = 0; i < num; i++) {
        idx = 0;
        for (j = 0; j < num; j++) {
            idx = D_8014A250[j].slot;
            if (i == D_80152818[idx].place) {
                break;
            }
        }
        order[i] = idx;
    }
    i = 0;
    for (j = 0; j < D_80153FD2; j++) {
        D_80142DB4[num - j - 1] = D_8014A118[j].slot;
    }
    for (; i < num; i++) {
        idx = order[i];
        if (D_8014A250[idx].active == 1) {
            D_80142DB4[num - j - 1] = idx; j++;
        }
        if (j == num) {
            break;
        }
    }
    D_80142D70 = 1;
    return 1;
}
