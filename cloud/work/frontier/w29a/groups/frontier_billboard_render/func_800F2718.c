/* Existing accepted F2718 body only; synthetic F1210 and callers omitted. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

typedef struct {
    u8 pad00[4];
    s32 unk04;
    u8 pad08[0x4C - 8];
} InputRecord;

extern s16 active_player_count;
extern s32 gameplay_mode;
extern InputRecord input_rec0[];
extern s8 D_80142694[];
extern s8 D_80143CE8;
extern s8 *D_80149418[];
extern s32 D_80140B20;
extern s32 D_801148CC;

void sound_handles_array_clear(s8 *arg0);
s32 viDeadlinePassed(s32 arg0);
void resource_type_select(s32 arg0);
void sound_stop(s32 arg0);

extern s8 *D_80146208;
extern s8 *D_80149798[3];
void func_800F1210(s32);


void func_800F2718(void)
{
    s32 i;
    s32 done;

    done = 1;
    for (i = 0; i < active_player_count; i++) {
        if (D_80142694[i] == 1) {
            if ((input_rec0[i].unk04 & 3) || viDeadlinePassed(input_rec0[i].unk04)) {
                resource_type_select(input_rec0[i].unk04);
                D_80142694[i] = 0;
            }
        } else if (D_80149418[i] != 0) {
            sound_handles_array_clear(D_80149418[i]);
            D_80149418[i] = 0;
        }
    }
    for (i = 0; i < active_player_count; i++) {
        if (D_80142694[i] == 1) {
            done = 0;
        }
    }
    if (done == 1) {
        if (D_80143CE8) {
            func_800F1210(2);
        } else if (gameplay_mode == 0 || gameplay_mode == 3 || gameplay_mode == 2) {
            func_800F1210(3);
        } else {
            func_800F1210(0);
        }
    }
}
