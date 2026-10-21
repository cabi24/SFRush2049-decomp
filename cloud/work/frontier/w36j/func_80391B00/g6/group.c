/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B: update each player's flag/kind-selected HUD scene objects.
 * Strict members: inherited func_80391650 (532 bytes), new func_80391864
 * (668 bytes). Genuine kept func_80391B00 remains NONMATCH context.
 * Native caller passes the unused argument in stack slot zero; both internal
 * helpers return one, ignored by that caller. No invented caller/signature.
 * In the inherited flag helper, Point precedes kind/i/status to reproduce point at sp+84 and kind at sp+80
 * in the unchanged 88-byte frame. The status pointer is evaluated after the
 * real flag selection. Direct indexed UV references recover native induction
 * order. These change no storage capacities and add no locals or dead reads.
 * The inherited volatile resource-count declaration is removed. Explicit
 * 32-bit pointer round-trips preserve native address materialization, without
 * volatile semantics. They assume the native 32-bit address model; original
 * source spelling and declaration order are unknown.
 * Valid status-helper inputs: 0..4 players, backed arrays and scene handles;
 * on the first iteration flags must be zero or include bit 1/2. Other nonzero
 * flags may retain an earlier kind, or read an uninitialized kind initially,
 * as in native. Original names and whole-function arcade donor are unknown.
 * Full gameplay, helper implementations and arbitrary invalid inputs are not
 * behaviorally tested. Context limitations are retained in RESULTS.md.
 */
/* Image-B kind-scene sibling. Genuine consumed player/status kind carriers keep
 * native loads separate; each is refreshed after a helper can mutate it.
 * The direct indexed fields preserve native aliasing and induction order.
 * Original local spellings are unknown. No padding or capacity change.
 * Accepted 80391650 and genuine NONMATCH 80391B00 are preserved unchanged.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Player { u8 unknown000[0x384]; s8 kind, lives; u8 unknown386[6]; s32 flags; u8 unknown390[0x28]; } Player;
typedef struct Status { s16 kind, handle; f32 uv[3][3], position[3]; } Status;
typedef struct Handle { s16 handle, unknown2; f32 uv[3][3], position[3]; } Handle;
typedef struct Camera { f32 uv[3][3], position[3]; u8 unknown30[104]; } Camera;
typedef struct Scene { u8 unknown00[16]; f32 scale; u8 unknown14[48]; } Scene;
typedef struct Point { s16 x, y; } Point;
typedef union Color { u32 word; struct { u8 r,g,b,a; } rgba; } Color;
typedef struct NameQuad { char *name[4]; } NameQuad;
typedef struct ScoreState {
    s16 active, lock;
    f32 started;
    s32 unknown08, score, shown_score, total_score, working;
    u8 unknown1C[4];
    s16 source_counts[10], counts[10];
    u8 unknown48[29];
    s8 message_kind;
    u8 unknown66;
    s8 message_index;
    u8 unknown68[4];
    s32 multiplier, pending, state;
} ScoreState;
typedef struct Effect { s16 state, handle; f32 uv[3][3], position[3], alpha; s16 x,y,from_x,from_y,to_x,to_y; f32 timer; } Effect;
extern s16 D_80151AD0, D_8014A108;
extern s32 D_8014A110, D_80393B74[];
extern Player D_80152818[];
extern Status D_80395C00[], D_80395CD0[];
extern Handle D_80395DA0[];
extern Camera D_80150B70[];
extern Scene D_8012E700[];
extern char *D_80393B68[], *D_80393B40[];
extern u8 D_80140BDC;
extern f32 D_80116178, D_801543CC;
extern volatile f32 D_8002EB94;
extern Point D_80394210[4][4], D_803941D0[4][4];
extern s32 D_803940D0[4][4][2], D_80393FD0[4][4][2], D_80115F28[4][4][2];
extern Color D_803942A4, D_80394270[], D_803942A0;
extern NameQuad D_803942A8;
extern s8 D_8012E67C[];
extern u16 D_801427C0[];
extern ScoreState D_80152038[];
extern Effect D_803950C0[4][10];
extern char D_80394AA8[];
extern u32 state_word_a;
extern s32 D_801170FC;
extern s32 string_copy_format(char *,s8,s8,s32);
extern s32 func_8008E398(s32,f32[3][3],s32,u32);
extern void sound_call_minimal(s16);
extern void model_data_load(s32,s32,u32);
extern void model_transform_setup(s32,s32,u32);
extern void func_8008B32C(f32[3][3],f32[3][3],f32);
extern void func_80090E9C(f32,f32[3][3]);
extern void func_80090F44(f32,f32[3][3]);
extern void car_lights_render(s32,s16 *,Camera *,f32,f32 *);
extern void func_8008E06C(s16,u32 *);
extern void func_80090770(s16,u16);
extern void fcvt_wrapper(char *,char *,...);

s32 func_80391650(void *unused)
{
    Point point;
    s32 kind, i;
    Status *status;
    for (i = 0; i < D_80151AD0; i++) {
        if (D_80152818[i].flags) {
            if (D_80152818[i].flags & 1) kind = 1;
            else if (D_80152818[i].flags & 2) kind = 2;
        } else {
            kind = 0;
        }
        status = &D_80395CD0[i];
        if (kind != status->kind) {
            if (status->handle != -1) {
                sound_call_minimal(status->handle);
                status->handle = -1;
            }
            status->kind = kind;
        }
        if (status->kind && status->handle == -1) {
            status->handle = func_8008E398(
                string_copy_format(D_80393B68[status->kind], 0, *(u8 *)(u32)&D_80140BDC - 1, 1),
                D_80395CD0[i].uv, -1, 0);
            model_data_load(status->handle, 1, 15);
            model_transform_setup(status->handle, 0, D_80393B74[i]);
            D_8012E700[status->handle].scale = 1.5f;
        }
        func_8008B32C(D_80150B70[i].uv, D_80395CD0[i].uv, 0.025f);
        func_80090E9C(D_80116178, D_80395CD0[i].uv);
        point.x = D_80394210[D_80151AD0 - 1][i].x;
        point.y = D_80394210[D_80151AD0 - 1][i].y;
        car_lights_render(i, &point.x, &D_80150B70[i], 2.0f, status->position);
    }
    return 1;
}
s32 func_80391864(void *unused)
{
    Point point;
    s32 i, kind, resource;
    Color color;
    s32 player_kind;
    for (i = 0; i < D_80151AD0; i++) {
        player_kind = D_80152818[i].kind;
        kind = D_80395C00[i].kind;
        if (player_kind != kind) {
            if (D_80395C00[i].handle != -1) {
                sound_call_minimal(D_80395C00[i].handle);
                D_80395C00[i].handle = -1;
                player_kind = D_80152818[i].kind;
            }
            D_80395C00[i].kind = player_kind;
            kind = D_80395C00[i].kind;
        }
        if (kind != 8 && D_80395C00[i].handle == -1) {
            color.word = D_803942A4.word;
            switch (kind) {
            case 0: resource = 216; break;
            case 1: resource = 217; break;
            case 2: resource = 218; break;
            case 3: resource = 219; break;
            case 4: resource = 220; break;
            case 5: resource = 221; break;
            case 6: resource = 222; break;
            case 7: resource = 223; break;
            }
            D_80395C00[i].handle = func_8008E398(D_801427C0[resource], D_80395C00[i].uv, -1, 0x42000);
            func_8008E06C(D_80395C00[i].handle, &color.word);
            D_8012E700[D_80395C00[i].handle].scale = 1.5f;
            model_data_load(D_80395C00[i].handle, 1, 15);
            model_transform_setup(D_80395C00[i].handle, 0, D_80393B74[i]);
            kind = D_80395C00[i].kind;
        }
        if (kind == 7) func_8008B32C(D_80150B70[i].uv, D_80395C00[i].uv, 0.175f);
        else func_8008B32C(D_80150B70[i].uv, D_80395C00[i].uv, 0.069f);
        func_80090E9C(D_80116178, D_80395C00[i].uv);
        point.x = D_803941D0[D_80151AD0 - 1][i].x;
        point.y = D_803941D0[D_80151AD0 - 1][i].y;
        if (D_80395C00[i].kind == 7) car_lights_render(i, &point.x, &D_80150B70[i], 5.75f, D_80395C00[i].position);
        else car_lights_render(i, &point.x, &D_80150B70[i], 2.0f, D_80395C00[i].position);
    }
    return 1;
}

#define gRace D_8014A110
#define kBattle 6
#define kStunt 4
#define gNumWindows D_80151AD0
#define gNumPlayers D_8014A108
#define gGameState state_word_a
#define gPauseState D_801170FC
#define gModelTime D_801543CC
#define gFrameSeconds D_8002EB94
#define gNumModels (*(volatile u8 *)&D_80140BDC)
#define gStuntPlayer D_80152038
#define gCamera D_80150B70
#define ObjInstanceList D_8012E700
#define gCarTeam D_8012E67C
#define whichCam D_80393B74
#define stuntCoin D_803950C0
#define battleCoin D_80395DA0
#define playerColor D_80394270
#define stuntCoinTexNames D_80393B40
#define wrongWayPos D_80115F28
#define stuntCoinPos D_80393FD0
#define fragPos D_803940D0
#define MBOX_FindObject_WORK string_copy_format
#define MBOX_NewObject func_8008E398
#define MBOX_HideObject model_data_load
#define MBOX_ShowObject model_transform_setup
#define ZOID_RemoveObject sound_call_minimal
#define ZOID_SetColor func_8008E06C
#define ZOID_SetModel func_80090770
#define ScaleUV func_8008B32C
#define PitchUV func_80090F44
#define ZOID_WindowTo3D car_lights_render
#define TB_Sprintf fcvt_wrapper
typedef union tRGBA { u8 c[4]; u32 rgba; } tRGBA;
static int animStuntCoins(void *unused);
static int animBattleCoins(void *unused);

void func_80391B00(void) /* UpdateHUDExtraPostCamera */
{
    if (gRace == kBattle) {
        func_80391864(0);
        func_80391650(0);
        animBattleCoins(0);
    }
    if (gRace == kStunt) animStuntCoins(0);
}

static int animStuntCoins(void *unused)
{
    int i, j, k;
    int d, e;
    ScoreState *p;
    Effect *coin;
    void *tex;
    u16 texIdx;
    tRGBA coinColor;
    float alpha;
    char name[32];
    coinColor.rgba = D_803942A0.word;
    if ((gGameState & 0x7c03fffe) || gPauseState) return 1;
    for (i = 0; i < gNumPlayers; i++) {
        p = &gStuntPlayer[i];
        if (p->lock) alpha = (1.0f - (gModelTime - p->started) / 2.69f) * 255.0f;
        else alpha = 255.0f;
        if (alpha < 0.0f) alpha = 0.0f;
        coinColor.c[3] = alpha;
        if (p->active && (gModelTime - p->started < 2.69f || p->working)) {
            for (j = 0; j < 10; j++) {
                if (!p->state) p->counts[j] = p->source_counts[j];
                if (p->counts[j]) {
                    for (k = 0; k < 10; k++) {
                        coin = &stuntCoin[i][k];
                        if (coin->state == j) break;
                        if (coin->state == 11) {
                            TB_Sprintf(name, D_80394AA8, stuntCoinTexNames[j]);
                            if (coin->handle != -1)
                                ZOID_SetModel(coin->handle, MBOX_FindObject_WORK(name, 0, (s8)(gNumModels - 1), 1));
                            else
                                coin->handle = MBOX_NewObject(MBOX_FindObject_WORK(name, 0, (s8)(gNumModels - 1), 1), coin->uv, -1, 0x42000);
                            ObjInstanceList[coin->handle].scale = 1.5f;
                            coin->state = j;
                            coin->timer = 0.5f;
                            d = gNumWindows == 1 ? -28 : gNumWindows == 2 ? -16 : -12;
                            coin->from_x = wrongWayPos[gNumWindows - 1][i][0];
                            coin->from_y = wrongWayPos[gNumWindows - 1][i][1] + d;
                            coin->to_x = stuntCoinPos[gNumWindows - 1][i][0];
                            coin->to_y = stuntCoinPos[gNumWindows - 1][i][1] + d;
                            {
                                Effect *prev;
                                for (k--; k >= 0; k--) {
                                    prev = &stuntCoin[i][k];
                                    d -= gNumWindows == 1 ? 18 : 8;
                                    prev->timer = 0.5f;
                                    prev->from_x = prev->x;
                                    prev->from_y = prev->y;
                                    prev->to_x = stuntCoinPos[gNumWindows - 1][i][0];
                                    prev->to_y = stuntCoinPos[gNumWindows - 1][i][1] + d;
                                }
                            }
                            break;
                        }
                    }
                    if (coin->handle != -1) {
                        coin->timer -= gFrameSeconds;
                        if (coin->timer < 0.0f) coin->timer = 0.0f;
                        coin->x = (coin->from_x - coin->to_x) * (coin->timer / 0.5f) + coin->to_x;
                        coin->y = (coin->from_y - coin->to_y) * (coin->timer / 0.5f) + coin->to_y;
                        ZOID_WindowTo3D(i, &coin->x, &gCamera[i], 2.0f, coin->position);
                        ScaleUV(gCamera[i].uv, coin->uv, gNumWindows == 1 ? 0.005f : gNumWindows == 2 ? 0.0035f : 0.005f);
                        coin->alpha = alpha;
                        coinColor.c[3] = coin->alpha;
                        ZOID_SetColor(coin->handle, (u32 *)&coinColor);
                        MBOX_ShowObject(coin->handle, 0, whichCam[i]);
                    }
                }
            }
        } else if (!p->working) {
            p->active = 0;
            p->lock = 0;
            p->multiplier = 1;
            p->message_index = 0;
            p->message_kind = 0;
            p->shown_score = 0;
            for (j = 0; j < 10; j++) {
                p->counts[j] = 0;
                if (stuntCoin[i][j].handle != -1) {
                    ZOID_RemoveObject(stuntCoin[i][j].handle);
                    stuntCoin[i][j].handle = -1;
                }
                stuntCoin[i][j].state = 11;
            }
        }
    }
    return 1;
}

static int animBattleCoins(void *unused)
{
    s16 screenPos[2];
    int player;
    NameQuad names;
    tRGBA coinColor;
    void *tex;
    u16 texIdx;
    char name[32];
    for (player = 0; player < gNumWindows; player++) {
        if (battleCoin[player].handle == -1) {
            names = D_803942A8;
            coinColor.c[0] = playerColor[gCarTeam[player]].rgba.r;
            coinColor.c[1] = playerColor[gCarTeam[player]].rgba.g;
            coinColor.c[2] = playerColor[gCarTeam[player]].rgba.b;
            coinColor.c[3] = 255;
            battleCoin[player].handle = MBOX_NewObject(MBOX_FindObject_WORK(names.name[gCarTeam[player]], 0, (s8)(gNumModels - 1), 1), battleCoin[player].uv, -1, 0);
            ObjInstanceList[battleCoin[player].handle].scale = 1.5f;
            MBOX_HideObject(battleCoin[player].handle, 1, 15);
            MBOX_ShowObject(battleCoin[player].handle, 0, whichCam[player]);
        }
        ScaleUV(gCamera[player].uv, battleCoin[player].uv, gNumPlayers > 2 ? 0.012f : 0.009f);
        PitchUV(-1.5707963705062866f, battleCoin[player].uv);
        screenPos[0] = fragPos[gNumWindows - 1][player][0];
        screenPos[1] = fragPos[gNumWindows - 1][player][1];
        ZOID_WindowTo3D(player, screenPos, &gCamera[player], 2.0f, battleCoin[player].position);
    }
    return 1;
}
