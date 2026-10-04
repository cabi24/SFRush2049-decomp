typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u8 pad000[0x3F0];
    f32 unk3F0;
    u8 pad3F4[0x7C6 - 0x3F4];
    s16 unk7C6;
    u8 pad7C8[0x7CC - 0x7C8];
    s8 unk7CC;
    u8 pad7CD[0x7E8 - 0x7CD];
    s8 unk7E8;
    u8 pad7E9[0x808 - 0x7E9];
} Car;

extern s8 D_80142760;
extern s16 active_player_count;
extern s32 D_80143F10;
extern f32 D_801525F4;
extern f32 D_8002EB94;
extern Car D_8014A250[];
extern s16 D_80152734;
extern s16 D_8015273A;
extern s32 gameplay_mode;
extern s8 D_80152744;

s32 func_800E7DD0(void)
{
    s16 i;
    s32 n;
    s32 result;
    s16 idx;

    if (D_80142760 != 0 && active_player_count >= 2 && D_80143F10 != 0) {
        if (D_801525F4 > 0.0f) {
            D_801525F4 += D_8002EB94;
            if (D_801525F4 > 4.0f) {
                return 1;
            }
        } else {
            for (i = 0; i < active_player_count; i++) {
                if (D_80152734 == D_8014A250[i].unk7E8) {
                    D_801525F4 = D_8002EB94;
                }
            }
        }
        return 0;
    }
    result = 1;
    if (gameplay_mode == 2) {
        n = 1;
    } else {
        n = D_80152744;
    }
    for (i = 0; i < n; i++) {
        idx = D_8014A250[i].unk7C6;
        if (D_8014A250[idx].unk7CC == 2) {
            if (D_80142760 == 0 || D_80152734 == D_8014A250[idx].unk7E8) {
                if (D_8014A250[idx].unk3F0 > 5.0f) {
                    result = 0;
                    break;
                }
            }
        }
    }
    if (result != 0 && gameplay_mode == 4) {
        return D_8015273A;
    }
    return result;
}
