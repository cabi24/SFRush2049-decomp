/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef struct Vehicle {
    u8 unknown000[1024];
    float catchup;
    u8 unknown404[964];
    s16 eligible;
    u8 unknown7CA[2];
    s8 mode;
    u8 unknown7CD[59];
} Vehicle;
typedef struct Player {
    u8 unknown000[256];
    float progress;
    u8 unknown104[597];
    s8 state;
    u8 unknown35A[94];
} Player;
typedef struct SlotMode { u8 unknown0[7]; u8 mode; } SlotMode;
extern s16 active_player_count;
extern s8 D_80152030, D_80150F14;
extern SlotMode D_80153E88[];
extern Vehicle D_8014A250[];
extern Player player_array[];
extern float D_80124310, D_80124314, D_80124318, D_8012431C;
void func_800DE860(void) {
    s16 i, selected;
    s8 flag;
    float leader_progress;
    float difference;
    float factor;
    if ((active_player_count == 1 && D_80152030 < 5) || (flag = D_80150F14) == 0) {
        for (i = 0; i < 6; i++) {
            if (D_80153E88[i].mode == 0 || D_80153E88[i].mode == 6)
                D_8014A250[i].catchup = 1.0f;
        }
        return;
    }
    selected = -1;
    for (i = 0; i < 6; i++) {
        if (D_8014A250[i].eligible != 0 && player_array[i].state < 2 &&
            (D_8014A250[i].mode == 2 || D_80152030 == 5)) {
            if (selected == -1) selected = i;
            else if (player_array[selected].progress < player_array[i].progress) selected = i;
        }
    }
    leader_progress = player_array[selected].progress;
    for (i = 0; i < 6; i++) {
        if (D_8014A250[i].eligible != 0 && player_array[i].state < 2 &&
            (D_8014A250[i].mode == 2 || D_80152030 == 5)) {
            difference = leader_progress - player_array[i].progress;
            if (difference > D_80124314) factor = D_80124310 + 1.0f;
            else factor = difference * D_80124310 / D_80124314 + 1.0f;
            if (flag == 1) factor = (1.0f - factor) * 0.5f + 1.0f;
            D_8014A250[i].catchup = D_8014A250[i].catchup * D_80124318 + D_8012431C * factor;
        }
    }
}
