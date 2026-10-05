/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Per-player name entry. Native input index arrives in s0 from billboard_render.
 * A 9-column grid has 54 characters plus -1 finish, -2 erase, -3 space.
 * The name has at most 12 bytes plus terminator. A completed name is assigned
 * a cache slot, then propagated to the matching 3-by-5 result entries.
 * Reconstructed directly from the complete registered retail function.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct InputRecord {
    u8 unk0, player_id;
    u8 unk2[2];
    u32 pressed;
    u32 unk8;
    u32 repeated;
    u8 unk10[60];
} InputRecord;
extern InputRecord input_rec0[];
extern s16 D_80144010[];
extern s16 D_80144C40[];
extern u8 D_80143A50[][13];
extern s8 D_80143F18[];
extern s32 D_801148D4[];
extern s32 D_801424F0[];
extern s16 D_80142528;
extern s32 D_80151690[12][3][5];
extern s8 D_80151AC0[3][5];
extern u8 *D_80117420;
extern void audio_distance_atten(u32);
extern void audio_doppler(u32);
extern void resource_type_select(u32);
extern s32 viDeadlinePassed(void);
extern u8 func_800F084C(s32);
extern s16 func_800F1D04(u8 *);

void func_800F207C(s32 player)
{
    s32 row, place;
    InputRecord *input;
    u8 *name;

    input = &input_rec0[player];
    if (input->repeated & 0x400) {
        audio_distance_atten(input->repeated);
        if (D_80144010[player] == -1) D_80144010[player] = 52;
        else if (D_80144010[player] == -2) D_80144010[player] = 49;
        else if (D_80144010[player] == -3) D_80144010[player] = 46;
        else if (D_80144010[player] < 3) D_80144010[player] = -3;
        else if (D_80144010[player] < 6) D_80144010[player] = -2;
        else if (D_80144010[player] < 9) D_80144010[player] = -1;
        else D_80144010[player] -= 9;
    } else if (input->repeated & 0x800) {
        audio_distance_atten(input->repeated);
        if (D_80144010[player] == -1) D_80144010[player] = 7;
        else if (D_80144010[player] == -2) D_80144010[player] = 4;
        else if (D_80144010[player] == -3) D_80144010[player] = 1;
        else if (D_80144010[player] < 45) D_80144010[player] += 9;
        else if (D_80144010[player] < 48) D_80144010[player] = -3;
        else if (D_80144010[player] < 51) D_80144010[player] = -2;
        else D_80144010[player] = -1;
    } else if (input->repeated & 0x1000) {
        audio_doppler(input->repeated);
        if (D_80144010[player] == -1) D_80144010[player] = -2;
        else if (D_80144010[player] == -2) D_80144010[player] = -3;
        else if (D_80144010[player] == -3) D_80144010[player] = -1;
        else if (D_80144010[player] % 9 == 0) D_80144010[player] += 8;
        else D_80144010[player]--;
    } else if (input->repeated & 0x2000) {
        audio_doppler(input->repeated);
        if (D_80144010[player] == -1) D_80144010[player] = -3;
        else if (D_80144010[player] == -2) D_80144010[player] = -1;
        else if (D_80144010[player] == -3) D_80144010[player] = -2;
        else if (D_80144010[player] % 9 == 8) D_80144010[player] -= 8;
        else D_80144010[player]++;
    }
    if ((input->pressed & 3) || viDeadlinePassed()) {
        resource_type_select(input->pressed);
        if ((input->pressed & 1) || viDeadlinePassed()) {
            D_80144010[player] = -1;
        }
        if (D_80144010[player] == -2) {
            if (D_80144C40[player] > 0) {
                D_80144C40[player]--;
                D_80143A50[player][D_80144C40[player]] = 0;
            }
        } else if (D_80144010[player] == -1) {
            while (D_80143A50[player][D_80144C40[player] - 1] == ' ') {
                D_80144C40[player]--;
                D_80143A50[player][D_80144C40[player]] = 0;
            }
            name = D_80143A50[player];
            func_800F084C((s32)name);
            D_80143F18[player] = 0;
            D_801424F0[player] = D_801148D4[input->player_id] = func_800F1D04(name);
            for (row = 0; row < 3; row++) {
                for (place = 0; place < 5; place++) {
                    if (D_80151AC0[row][place] == player) {
                        D_80151690[D_80142528][row][place] = D_801424F0[player];
                    }
                }
            }
        } else if (D_80144C40[player] < 12 &&
                   (D_80144010[player] != -3 || D_80144C40[player] > 0)) {
            if (D_80144010[player] == -3) {
                D_80143A50[player][D_80144C40[player]] = ' ';
            } else {
                D_80143A50[player][D_80144C40[player]] = D_80117420[D_80144010[player]];
            }
            D_80144C40[player]++;
            D_80143A50[player][D_80144C40[player]] = 0;
            if (D_80144C40[player] == 12) D_80144010[player] = -1;
        }
    } else if (input->pressed & 4) {
        resource_type_select(input->pressed);
        if (D_80144C40[player] > 0) {
            D_80144C40[player]--;
            D_80143A50[player][D_80144C40[player]] = 0;
        }
    }
}
