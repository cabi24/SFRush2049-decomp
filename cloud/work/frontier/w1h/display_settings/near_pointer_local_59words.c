/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    /* 0x00 */ s8 busy;
    /* 0x01 */ s8 enabled;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ s8 player;
    /* 0x04 */ u8 primary[4];
    /* 0x08 */ u8 secondary[4];
    /* 0x0C */ u8 previous[4];
    /* 0x10 */ s8 present;
    /* 0x14 */ s32 mode;
    /* 0x18 */ s16 arg[4];
    /* 0x20 */ u8 pad20[8];
    /* 0x28 */ u32 deadline;
} Slot; /* 0x2C */

typedef struct {
    u8 pad0[6];
    s8 present;
    u8 pad7[0x2FD];
} Controller; /* 0x304 */

typedef struct {
    u8 pad0[0x27C];
    volatile u32 unk27C;
} Sched;

extern s8 D_80116DB4;
extern Slot D_80153FD8[4][2];
extern Controller D_80144030[];
extern f32 D_8002AFB4;
extern Sched D_8002E8E8;

s8 func_800DDF28(s32 arg0);
s8 func_800DDEA4(s32 arg0);
void player_state_set(s32 arg0, s32 arg1);
void player_mode_set(s32 arg0, s32 arg1);

void display_settings(s32 player, s32 mode, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 i;
    s32 j;
    Slot *slot;
    Slot *row;
    s8 present;
    static s8 D_80116DB8 = 0;

    if (mode == 10) {
        D_80116DB4 = 1;
    }
    if (!D_80116DB8) {
        for (j = 0; j < 4; j++) {
            for (i = 0; i < 2; i++) {
                D_80153FD8[j][i].mode = 0;
                D_80153FD8[j][i].busy = 1;
            }
        }
        D_80116DB8 = 1;
    }
    row = D_80153FD8[player];
    slot = row;
    for (j = 0; j < 2; j++) {
        if (slot->mode == 0) {
            break;
        }
        slot++;
    }
    slot->busy = 0;
    slot->deadline = (s32)(3.0f * D_8002AFB4) + D_8002E8E8.unk27C;
    slot->enabled = 1;
    slot->unk2 = 0;
    slot->mode = mode;
    for (i = 0; i < 4; i++) {
        row[j].primary[i] = func_800DDF28(i);
        row[j].secondary[i] = func_800DDEA4(i);
        row[j].previous[i] = 0;
    }
    player_state_set(-1, 0);
    player_mode_set(-1, 0);
    if (mode == 1) {
        player_state_set(-1, 1);
        player_mode_set(-1, 1);
    } else {
        slot->player = player;
        if (mode == 2) {
            player_state_set(-1, 1);
            player_mode_set(-1, 1);
        } else {
            player_state_set(slot->player, 1);
            player_mode_set(slot->player, 1);
        }
    }
    present = D_80144030[player].present;
    slot->arg[0] = arg2;
    slot->present = present;
    slot->arg[1] = arg3;
    slot->arg[2] = arg4;
    slot->arg[3] = arg5;
}
