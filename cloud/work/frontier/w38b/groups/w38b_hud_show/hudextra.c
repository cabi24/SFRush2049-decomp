/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B, 0x803925D0..0x80392894 (708 bytes): ShowHUDExtra. On first call
 * builds the battle or stunt HUD multi-blit (stunt: font by window count and
 * one coloured pane per player sized from "8888888"), clears the battle-point
 * ring and resets every player's weapon/power-up icon, battle coin and stunt
 * coin records.
 * Donor: MAT3 GameCube decomp, GameSource/src/hudextra.c ShowHUDExtra
 * (GC 0x8002271c, size 0x218) with the static ClearBattlePoints
 * (GC 0x80024204) inlined; typed port, MAT3 names/order, N64 offsets.
 * N64 differences: the pane half-sizes are written as w/2, h/2 in the
 * InitPane arguments (MAT3 has `w /= 2; h /= 2;`): retail halves inside the
 * player-count guard, which only the loop-invariant argument form gives.
 * ClearBattlePoints is defined after ShowHUDExtra, as in the original file
 * order: the inlined statements keep its (later) source lines, and as1 breaks
 * scheduling ties by line (defined first, two stores schedule early).
 * The object starts with ClearBattlePoints' 8-byte deleted-procedure stub
 * (`jr ra; nop`, no symbol), which is retail's func_803925C8.
 * Earlier research: Astra (CLAIMS 2026-10-12, 13/177; lean packet
 * cloud/work/lean/runtime_b_hud_init_20261006, 18/177).
 * Historical callee names: sound_control = NewMultiBlit, object_create =
 * HAL_SetFont, object_manager_update = HAL_GetTextWidth, object_bytes_sum_global
 * = TB_GetFontLineHeight, ambient_sound_set = InitPane, unlock_notification =
 * ChangePaneColor. No unused locals.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct tUVMatrix { f32 uvs[4][3]; } tUVMatrix;
typedef struct tHudIcon { s16 type, idx; tUVMatrix mat; } tHudIcon;          /* anon_type_idx_34 */
typedef struct tBattleCoin { s16 idx; s16 pad; tUVMatrix mat; } tBattleCoin; /* anon_idx_mat_34 */
typedef struct tStuntCoin {
    s16 type, idx;
    tUVMatrix mat;
    f32 alpha;
    s16 screenPos[2], startPos[2], destPos[2];
    f32 scrollTime;
} tStuntCoin;
typedef struct tBattlePoint {      /* anon_active_cnt_18 */
    s8 active, cnt, plto, plfrom;
    f32 startx, starty, endx, endy;
    s8 p;
    u8 pad[3];
} tBattlePoint;
typedef struct tBlitSet tBlitSet;
typedef struct tBlitDef tBlitDef;

extern void *D_80395ED4;
extern s16 D_80151AD0, active_player_count;
extern s32 gameplay_mode;
extern tHudIcon D_80395C00[4], D_80395CD0[4];
extern tBattleCoin D_80395DA0[4];
extern tStuntCoin D_803950C0[4][10];
extern s8 D_80395ED0;
extern tBattlePoint D_80395E70[4];
extern s32 D_80393F50[4][4][2];
extern u8 D_80394250[4][2][4];
extern tBlitDef D_80393DE8[], D_80393B84[];
extern char D_80394A80[];
#define blit D_80395ED4
#define gNumWindows D_80151AD0
#define gNumPlayers active_player_count
#define gRace gameplay_mode
#define weaponIcon D_80395C00
#define powerupIcon D_80395CD0
#define battleCoin D_80395DA0
#define stuntCoin D_803950C0
#define battlePointCnt D_80395ED0
#define battlePoint D_80395E70
#define stuntPos D_80393F50
#define playerPaneColors D_80394250
#define BattleMultiBlit D_80393DE8
#define StuntMultiBlit D_80393B84
#define kBattle 6
#define kStunt 4

extern void *sound_control(s16, s16, tBlitDef *, s16);                 /* NewMultiBlit */
extern s32 object_create(s32);                                          /* HAL_SetFont */
extern u32 object_manager_update(void *, s16);                          /* HAL_GetTextWidth */
extern s16 object_bytes_sum_global(void);                               /* TB_GetFontLineHeight */
extern tBlitSet *ambient_sound_set(s32, s32, s32, s32, s32, void *, u32, s32); /* InitPane */
extern void unlock_notification(tBlitSet *, u8 *, u8 *);               /* ChangePaneColor */
#define NewMultiBlit sound_control
#define HAL_SetFont object_create
#define HAL_GetTextWidth object_manager_update
#define TB_GetFontLineHeight object_bytes_sum_global
#define InitPane ambient_sound_set
#define ChangePaneColor unlock_notification

void func_803925C8(void);

void func_803925D0(void) /* ShowHUDExtra */
{
    int i;
    tBlitSet *pane;
    int w, h;
    int playerIdx;
    int s;
    if (!blit) {
        if (gRace == kBattle)
            blit = NewMultiBlit(0, 0, BattleMultiBlit, 10);
        else if (gRace == kStunt) {
            if (gNumWindows == 1) HAL_SetFont(10);
            else if (gNumWindows == 2) HAL_SetFont(11);
            else HAL_SetFont(12);
            w = HAL_GetTextWidth(D_80394A80, -1) + 8;
            h = TB_GetFontLineHeight() + 4;
            for (playerIdx = 0; playerIdx < gNumPlayers; playerIdx++) {
                pane = InitPane(stuntPos[gNumWindows-1][playerIdx][0]-w/2,
                    stuntPos[gNumWindows-1][playerIdx][1]-h/2,
                    stuntPos[gNumWindows-1][playerIdx][0]+w/2,
                    stuntPos[gNumWindows-1][playerIdx][1]+h/2,
                    128, 0, playerIdx << 4, 1);
                ChangePaneColor(pane, playerPaneColors[playerIdx][0], playerPaneColors[playerIdx][1]);
            }
            blit = NewMultiBlit(0, 0, StuntMultiBlit, 17);
        }
        func_803925C8();
        for (i = 0; i < 4; i++) {
            weaponIcon[i].type = 8;
            weaponIcon[i].idx = -1;
            powerupIcon[i].type = 0;
            powerupIcon[i].idx = -1;
            battleCoin[i].idx = -1;
            for (s = 0; s < 10; s++) {
                stuntCoin[i][s].type = 11;
                stuntCoin[i][s].idx = -1;
            }
        }
    }
}

void func_803925C8(void)
{
    int i;
    battlePointCnt = 0;
    for (i = 0; i < 4; i++) battlePoint[i].active = 0;
}
