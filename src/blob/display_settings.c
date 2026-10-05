/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (also identical at -O2) */
/*
 * NOT a strict match: every text word equals retail, but the scorer cannot
 * verify the four references to this function's own .data object
 * (`MATCH (4 section-relative relocations unverified ...)`).
 *
 * Historical label display_settings is misleading: this arms a rumble/"event"
 * slot for a player (N64-only).  D_80153FD8 is a 4-player x 2-slot table of
 * 0x2C-byte records.  On first use every slot is cleared (mode 0, busy 1).
 * The first free slot of `player` (mode == 0; if both are taken the index
 * runs to 2, as in retail) gets: busy 0, deadline = (s32)(3.0f * D_8002AFB4)
 * + scheduler frame count, enabled 1, mode, a snapshot of the four per-port
 * values from func_800DDF28/func_800DDEA4, and the four s16 arguments plus
 * the controller-present flag D_80144030[player].present.  player_state_set /
 * player_mode_set are then cleared for all ports and re-set for all ports
 * (mode 1 or 2) or for this player only.  mode == 10 also sets D_80116DB4.
 *
 * What mattered (all recovered from the code, see RESULTS.md):
 *   - the "initialised" flag at 0x80116DB8 is a function-local static (it is
 *     referenced by no other function; retail word is 0 in .data).  As an
 *     extern its address is kept in a register and `mode` leaves a1;
 *   - the scheduler frame counter is volatile (unfolded `lui/addiu; lw 636`);
 *   - busy/enabled are s8 like the two flags, so every stored 1 and the
 *     `mode == 1` test share one constant web (s3);
 *   - the slot is always written as D_80153FD8[player][j].  Through a pointer
 *     local (slot = ...; slot->x) the instructions are the same but ugen no
 *     longer emits `.noalias reg,$sp`, and as1 then keeps the argument-home
 *     loads behind the slot stores (22 aligned rows / 59 words);
 *   - the controller flag goes through the local `present` (a uopt web in v0;
 *     written inline it is a ugen temp and the tail differs by 21 words).
 */
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
    for (j = 0; j < 2; j++) {
        if (D_80153FD8[player][j].mode == 0) {
            break;
        }
    }
    D_80153FD8[player][j].busy = 0;
    D_80153FD8[player][j].deadline = (s32)(3.0f * D_8002AFB4) + D_8002E8E8.unk27C;
    D_80153FD8[player][j].enabled = 1;
    D_80153FD8[player][j].unk2 = 0;
    D_80153FD8[player][j].mode = mode;
    for (i = 0; i < 4; i++) {
        D_80153FD8[player][j].primary[i] = func_800DDF28(i);
        D_80153FD8[player][j].secondary[i] = func_800DDEA4(i);
        D_80153FD8[player][j].previous[i] = 0;
    }
    player_state_set(-1, 0);
    player_mode_set(-1, 0);
    if (mode == 1) {
        player_state_set(-1, 1);
        player_mode_set(-1, 1);
    } else {
        D_80153FD8[player][j].player = player;
        if (mode == 2) {
            player_state_set(-1, 1);
            player_mode_set(-1, 1);
        } else {
            player_state_set(D_80153FD8[player][j].player, 1);
            player_mode_set(D_80153FD8[player][j].player, 1);
        }
    }
    present = D_80144030[player].present;
    D_80153FD8[player][j].arg[0] = arg2;
    D_80153FD8[player][j].present = present;
    D_80153FD8[player][j].arg[1] = arg3;
    D_80153FD8[player][j].arg[2] = arg4;
    D_80153FD8[player][j].arg[3] = arg5;
}
