/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct tUVMatrix { f32 uvs[4][3]; } tUVMatrix;
typedef struct tStuntPlayer {
    s16 show, crash;
    f32 time;
    s32 maxStuntPoints, points, pointsHud, totalPoints, numstunts, lastStuntType;
    s16 stunts[10], stuntsHud[10], totalStunts[10], unused;
    f32 wingTime;
    s8 wingBonus, wingBonusHud, doneBonus, doneBonusHud;
    s32 bonus, bonusHud, showBonus, done;
} tStuntPlayer;
typedef struct tStuntCoin {
    s16 type, idx;
    tUVMatrix mat;
    f32 alpha;
    s16 screenPos[2], startPos[2], destPos[2];
    f32 scrollTime;
} tStuntCoin;
typedef struct tText {
    u16 numSingle;
    u8 **single;
    u16 numTab, numTabA;
    u16 *tab;
    u8 **taba;
} tText;

extern u32 state_word_a;
extern s16 D_80151AD0, active_player_count;
extern f32 D_801543CC;
extern tStuntPlayer D_80152038[4];
extern tText countdown_state;
extern s32 D_80115F28[4][4][2];
extern s32 D_80393F50[4][4][2], D_80393FD0[4][4][2];
extern s32 D_80395ED8[4];
extern tStuntCoin D_803950C0[4][10];
extern char D_80394A88[], D_80394A90[], D_80394A94[], D_80394A9C[], D_80394AA4[];
#define gGameState state_word_a
#define gNumWindows D_80151AD0
#define gNumPlayers active_player_count
#define gModelTime D_801543CC
#define gStuntPlayer D_80152038
#define gText countdown_state
#define wrongWayPos D_80115F28
#define stuntPos D_80393F50
#define stuntCoinPos D_80393FD0
#define bonuscnt D_80395ED8
#define stuntCoin D_803950C0

extern void render_helper(f32);                       /* HAL_SetTextDepth */
extern s32 object_create(s32);                        /* HAL_SetFont */
extern void func_800B669C(u32, u32);                  /* TB_SetJust */
extern void func_800ED66C(f32);                       /* HAL_SetTextTranslucency */
extern void fcvt_wrapper(void *, void *, ...);        /* TB_Sprintf */
extern void dispatch_handler(s32);                    /* TB_SetTextColor */
extern s16 sound_pitch_diff_halved(void *, s16);      /* TB_CenterText */
extern void state_utility(s16, s16, void *);          /* TB_DrawText */
extern s16 object_bytes_sum_global(void);             /* TB_GetFontLineHeight */
#define HAL_SetTextDepth render_helper
#define HAL_SetFont object_create
#define TB_SetJust func_800B669C
#define HAL_SetTextTranslucency func_800ED66C
#define TB_Sprintf fcvt_wrapper
#define TB_SetTextColor dispatch_handler
#define TB_CenterText sound_pitch_diff_halved
#define TB_DrawText state_utility
#define TB_GetFontLineHeight object_bytes_sum_global

int func_80392894(void *blt)
{
    u8 str[32];
    int i, j;
    int x, y;
    tStuntPlayer *p;
    /*@{*/tStuntCoin *coin;
    int showBonus;
    int unused0, unused1;/*@|int showBonus;
    tStuntCoin *coin;
    int unused0, unused1;@|tStuntCoin *coin;
    int unused0, unused1;
    int showBonus;@}*/
    if (gGameState & 0x7c03fffe) return 1;
    HAL_SetTextDepth(0.0f);
    TB_SetJust(0, 1);
    if (gNumWindows == 1) HAL_SetFont(10); else HAL_SetFont(12);
    for (i = 0; i < gNumPlayers; i++) {
        p = &gStuntPlayer[i];
        TB_SetTextColor(1);
        if (p->show && (gModelTime - p->time < 2.69f || p->numstunts)) {
            for (j = 0; j < 10; j++) {
                /*@{*/coin = &stuntCoin[i][j];
                if (coin->type != 11 && p->stuntsHud[coin->type] && coin->alpha) {
                    HAL_SetTextTranslucency((u8)coin->alpha);
                    x = (gNumWindows == 1 ? 12 : 6) + coin->screenPos[0];
                    y = coin->screenPos[1];
                    TB_Sprintf(str, D_80394A88, p->stuntsHud[coin->type]);/*@|if (stuntCoin[i][j].type != 11 && p->stuntsHud[stuntCoin[i][j].type] && stuntCoin[i][j].alpha) {
                    HAL_SetTextTranslucency((u8)stuntCoin[i][j].alpha);
                    x = stuntCoin[i][j].screenPos[0] + (gNumWindows == 1 ? 12 : 6);
                    y = stuntCoin[i][j].screenPos[1];
                    TB_Sprintf(str, D_80394A88, p->stuntsHud[stuntCoin[i][j].type]);@}*/
                    TB_DrawText(x, y, str);
                }
            }
            x = stuntCoinPos[gNumWindows-1][i][0] + (gNumWindows == 1 ? 12 : 6);
            y = stuntCoinPos[gNumWindows-1][i][1];
            TB_Sprintf(str, D_80394A90, p->pointsHud);
            TB_DrawText(TB_CenterText(str, x), y, str);
        } else if (!p->numstunts) {
            p->show = 0;
            p->crash = 0;
            p->bonusHud = 1;
            p->wingBonusHud = 0;
            p->doneBonusHud = 0;
            p->pointsHud = 0;
            for (j = 0; j < 10; j++) p->stuntsHud[j] = 0;
        }
        HAL_SetTextTranslucency(-1.0f);
        showBonus = p->showBonus;
        if (showBonus > 0) {
            if (bonuscnt[i] == -1) { gStuntPlayer[i].showBonus = showBonus - 1; bonuscnt[i] = -2; }
            else if (bonuscnt[i] == -2) bonuscnt[i] = 10;
            if (bonuscnt[i] >= 0) {
                int x = wrongWayPos[gNumWindows-1][i][0];
                int y = wrongWayPos[gNumWindows-1][i][1];
                TB_Sprintf(str, gText.single[181]);
                TB_Sprintf(str, D_80394A94, str, p->bonusHud);
                TB_DrawText(TB_CenterText(str, x), y, str);
                bonuscnt[i]--;
            }
        } else bonuscnt[i] = -2;
        if (!p->done) p->pointsHud = p->points;
        else if (!p->crash) {
            x = wrongWayPos[gNumWindows-1][i][0];
            y = wrongWayPos[gNumWindows-1][i][1];
            if (p->bonusHud > 1) {
                TB_Sprintf(str, gText.single[181]);
                TB_Sprintf(str, D_80394A9C, str, p->bonusHud);
                TB_DrawText(TB_CenterText(str, x), y, str);
                y += TB_GetFontLineHeight();
            }
            if (p->doneBonusHud) {
                TB_DrawText(TB_CenterText(gText.taba[gText.tab[38] + (p->doneBonusHud - 3)], x), y, gText.taba[gText.tab[38] + (p->doneBonusHud - 3)]);
                y += TB_GetFontLineHeight();
            }
            if (p->wingBonusHud == 1) {
                TB_DrawText(TB_CenterText(gText.single[179], x), y, gText.single[179]);
            } else if (p->wingBonusHud == 2) {
                TB_DrawText(TB_CenterText(gText.single[180], x), y, gText.single[180]);
            }
        }
        TB_SetTextColor(1);
        x = stuntPos[gNumWindows-1][i][0];
        y = stuntPos[gNumWindows-1][i][1];
        TB_Sprintf(str, D_80394AA4, p->totalPoints);
        TB_DrawText(TB_CenterText(str, x), y, str);
    }
    TB_SetJust(0, 3);
    HAL_SetTextDepth(-1.0f);
    return 1;
}
