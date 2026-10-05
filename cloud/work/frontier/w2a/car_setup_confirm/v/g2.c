/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    /* 0x000 */ char pad0[0x7C6];
    /* 0x7C6 */ s16 slot;
    /* 0x7C8 */ char pad7C8[4];
    /* 0x7CC */ s8 drone_type;
    /* 0x7CD */ char pad7CD[0x1B];
    /* 0x7E8 */ s8 unk7E8;
    /* 0x7E9 */ char pad7E9[0x1F];
} Model; /* 0x808 */

typedef struct {
    /* 0x000 */ char pad0[0xEE];
    /* 0x0EE */ s8 place;
    /* 0x0EF */ s8 place_locked;
    /* 0x0F0 */ f32 score;
    /* 0x0F4 */ char padF4[0xC];
    /* 0x100 */ f32 distance;
    /* 0x104 */ char pad104[0x255];
    /* 0x359 */ s8 unk359;
    /* 0x35A */ char pad35A[0x5E];
} GameCar; /* 0x3B8 */

extern s8 D_80152744;
extern s16 D_80152734;
extern Model D_8014A250[];
extern GameCar D_80152818[];
extern Model ALIAS[];
extern s32 D_8014A110;
extern s16 D_8014A108;
extern s8 D_80152718;
extern f32 D_801543AC;
extern s16 D_801543CA;

f32 viGetTimeToDeadline(void);
void viAddTicks(f32);
void func_800D1AB0(void);

void car_setup_confirm(s32 slot, f32 score) {
    s16 i, j, index, temp, num_locked, place[7], num_humans_locked;
    f32 scores[6];
    f32 ftemp;
    s32 temp2;

    D_80152818[slot].place_locked = 1;
    D_80152818[slot].score = score;
    num_humans_locked = 0;
    for (i = 0, num_locked = -1; i < D_80152744; i++) {
        index = D_8014A250[i].slot;
        if (D_80152818[index].place_locked == 1) {
            place[++num_locked] = index;
            scores[num_locked] = D_80152818[index].score;
            if (D_8014A250[index].drone_type == 2) {
                if (D_8014A110 != 2 || ALIAS == &D_8014A250[index]) {
                    num_humans_locked++;
                }
            }
            if (D_80152818[index].unk359 > 0 && D_8014A250[index].unk7E8 < D_80152734) {
                temp2 = 9.0f * D_801543AC;
                scores[num_locked] = 5999.999f + temp2 - D_80152818[index].distance;
            }
        }
    }
    if (D_8014A250[slot].drone_type == 2) {
        if ((num_humans_locked == D_8014A108 || (D_8014A110 == 2 && num_humans_locked == 1)) && D_80152718 == 0) {
            D_80152718 = 1;
            viAddTicks(1.0f - viGetTimeToDeadline());
        }
    }
    for (i = 0; i < num_locked; i++) {
        for (j = 0; j < num_locked - i; j++) {
            if (scores[j] > scores[j + 1]) {
                ftemp = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = ftemp;
                temp = place[j];
                place[j] = place[j + 1];
                place[j + 1] = temp;
            }
        }
    }
    for (i = 0; i < num_locked + 1; i++) {
        if (D_80152818[place[i]].unk359 > 0 && D_8014A250[place[i]].unk7E8 < D_80152734) {
            D_80152818[place[i]].place = D_801543CA - num_locked + i - 1;
        } else {
            D_80152818[place[i]].place = i;
        }
    }
    func_800D1AB0();
}
