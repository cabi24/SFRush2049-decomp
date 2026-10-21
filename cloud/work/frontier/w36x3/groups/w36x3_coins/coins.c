/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * particles_update (0x800B7A40, 1,464 B; historical label) = MAT3 animCoins@hud.c (GC 0x8001FC2C) with the
 * per-coin "%d/8" text of animCoinsText@hud.c (GC 0x800204C8) folded into the coin loop, as on the N64 (the
 * N64 function sits in the NumCoinsFound/CountBits translation unit, after func_800B78F0 = NumCoinsFound).
 * Base: lane w36u particles_update/lead_area128.c (w27a body). w36x3 changes:
 *  - `extern volatile u8 D_80140BDC` (gNumModels, volatile per w36h/w36y): retail builds its address
 *    (lui/addiu, lbu 0(reg)) -- 262 -> 10 words;
 *  - MAT3 operand order for x/y (`table + j * spacing`, `table + (n == 1 ? 12 : 8)`) -- 10 -> 0 code words;
 *  - the three strings as their retail data symbols (as func_80108DA8.c's D_80120E34, the next string):
 *    string literals would put own .rodata in two retail regions and make the names[] initializer point at
 *    own .rodata, which the unit/group own-data proofs cannot place;
 *  - HAL_SetFont/_GetSemaphore/_ReleaseSemaphore and TB_SetJust inlined as non-static helpers after the
 *    caller (w36o ShowHUD/animGear shape; _ReleaseSemaphore and HAL_SetFont on one source line).
 * Frame (disclosed shaping): retail frame 256 = cfe named area 128 + 6 spill homes + saves. Without the two
 * items below the named area is 104 (frame 232). +8: `f32 xoffset, yoffset;` (MAT3 animCoins DWARF locals,
 * unused in the N64 body) in the text block; +16: one extra inlined wrapper level at two call sites, written
 * here as HAL_SetTextDepth_Coins around render_helper (= HAL_SetTextDepth). Which two-site helper carries
 * that level is not determinable from the bytes: an extra level on HAL_SetFont or TB_SetJust gives the same
 * 128 and the same code.
 * Callee identities (w36u/w36o): render_helper = HAL_SetTextDepth, slot_state_setup = _HAL_SetFont (s2
 * parameter), string_copy_format = MBOX_FindObject_WORK, func_8008E26C = MBOX_NewObject, model_data_load =
 * MBOX_HideObject, model_transform_setup = MBOX_ShowObject, func_8008B32C = ScaleUV, func_80090E9C = YawUV,
 * car_lights_render = ZOID_WindowTo3D, func_800B78F0 = NumCoinsFound, fcvt_wrapper = TB_Sprintf,
 * dispatch_handler = TB_SetTextColor, state_utility = TB_DrawText; D_801541A8 = gPlayerCoin,
 * D_80150B70 = gCamera, D_8012E710 = ObjInstanceList, D_80116178 = hudObjAngle, D_80394050 = coinStuntPos
 * (image A), D_80115E28 = coinPos, D_80146113 = gOptions[11], player_array[].mode = place_locked,
 * D_80114C48 = whichCam, D_80118E20 = gJust, D_801461D0 = font semaphore queue.
 * gJust is a tentative definition here (as credits_scroll_grp/gr3_d.c and w36o_countdown): with an extern the
 * group compile gives the TB_SetJust(0, 1) stores two `lui at`; the unit needs an extern_data override.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct RecordSlot { s16 id; u8 pad02[2]; f32 pos[9]; u8 obj[12]; } RecordSlot;   /* 52 bytes; +4 transform, +40 */
typedef struct RecordRow { RecordSlot slot[2]; } RecordRow;                                /* 104 bytes */
typedef struct Cam152 { u8 pad[152]; } Cam152;
typedef struct Car952 { u8 pad000[239]; s8 mode; u8 pad0F0[952 - 240]; } Car952;
typedef struct Pair { s32 first, second; } Pair;
typedef struct Model68 { f32 scale; u8 pad[64]; } Model68;
typedef struct Pos { s16 x, y; } Pos;

extern s16 D_80151AD0;
extern s16 D_8014A108;
extern s32 gameplay_mode;
Pair D_80118E20;   /* tentative definition (as credits_scroll_grp/gr3_d.c): see header */
extern RecordRow D_801541A8[];
extern Cam152 D_80150B70[];
extern volatile u8 D_80140BDC;
extern Model68 D_8012E710[];
extern f32 D_80116178;
extern s32 D_80394050[][4][2];
extern s32 D_80115E28[][4][2];
extern s8 D_80146113;
extern Car952 player_array[];
extern s32 D_80114C48[];
extern u8 D_801461D0[];
extern char D_80120E0C[], D_80120E1C[]; /* "GOLDCOING_COIN", "SILVERCOINS_COI" */
extern char D_80120E2C[];               /* "%d/8" */

void render_helper(f32 arg0);
void dispatch_handler(s32);
void state_utility(s16, s16, char *);
s32 string_copy_format(char *arg0, s8 arg1, s8 arg2, s32 arg3);
s32 func_8008E26C(s32 arg0, void *arg1, s16 arg2, s32 arg3);
void model_data_load(s16 arg0, s32 arg1, s32 arg2);
void func_8008B32C(void *arg0, void *arg1, f32 arg2);
void func_80090E9C(f32 arg0, void *arg1);
void car_lights_render(s32 arg0, void *arg1, void *arg2, f32 arg3, void *arg4);
void model_transform_setup(s16 arg0, s32 arg1, s32 arg2);
s32 func_800B78F0(s32 arg0, s32 arg1);
void fcvt_wrapper(char *buf, char *fmt, ...);
extern s32 osRecvMesg(void *, void **, s32);
extern s32 osSendMesg(void *, void *, s32);
s32 slot_state_setup(s32 fontID);

typedef struct OSMesgQueue OSMesgQueue;
void _GetSemaphore_Coins(void);
void _ReleaseSemaphore_Coins(void);
s32 HAL_SetFont_Coins(s32 fontID);
void TB_SetJust_Coins(s32 h, s32 v);
void HAL_SetTextDepth_Coins(f32 d);

s32 particles_update(void *arg0) {
    Pos pos;
    char buf[8];
    s16 i;
    s16 j;

    HAL_SetTextDepth_Coins(0.0f);
    if (D_80151AD0 == 1) {
        HAL_SetFont_Coins(10);
    } else {
        HAL_SetFont_Coins(12);
    }
    TB_SetJust_Coins(0, 1);
    for (i = 0; i < D_80151AD0; i++) {
        for (j = 0; j < 2; j++) {
            if (D_801541A8[i].slot[j].id == -1) {
                char *names[2] = { D_80120E0C, D_80120E1C };
                D_801541A8[i].slot[j].id = func_8008E26C(string_copy_format(names[j], 0, D_80140BDC - 1, 1), D_801541A8[i].slot[j].pos, -1, 0);
                model_data_load(D_801541A8[i].slot[j].id, 1, 15);
                D_8012E710[D_801541A8[i].slot[j].id].scale = 10.0f;
            }
            func_8008B32C(&D_80150B70[i], D_801541A8[i].slot[j].pos,
                          (D_8014A108 == 1) ? 0.03f : (D_8014A108 == 2) ? 0.03f : 0.04f);
            func_80090E9C(D_80116178, D_801541A8[i].slot[j].pos);
            if (gameplay_mode == 4) {
                pos.x = D_80394050[D_80151AD0 - 1][i][0];
                pos.y = D_80394050[D_80151AD0 - 1][i][1] + j * ((D_80151AD0 == 1) ? 15 : (D_80151AD0 == 2) ? 12 : 8);
            } else {
                pos.x = D_80115E28[D_80151AD0 - 1][i][0];
                pos.y = D_80115E28[D_80151AD0 - 1][i][1] + j * ((D_80151AD0 == 1) ? 15 : (D_80151AD0 == 2) ? 12 : 8);
            }
            car_lights_render(i, &pos, &D_80150B70[i], 2.0f, D_801541A8[i].slot[j].obj);
            if (D_80146113 && player_array[i].mode != 1) {
                s16 x;
                s16 y;
                f32 xoffset, yoffset;

                model_transform_setup(D_801541A8[i].slot[j].id, 0, D_80114C48[i]);
                if (gameplay_mode == 4) {
                    x = D_80394050[D_80151AD0 - 1][i][0] + ((D_80151AD0 == 1) ? 12 : 8);
                    y = D_80394050[D_80151AD0 - 1][i][1] + j * ((D_80151AD0 == 1) ? 15 : (D_80151AD0 == 2) ? 12 : 8);
                } else {
                    x = D_80115E28[D_80151AD0 - 1][i][0] + ((D_80151AD0 == 1) ? 12 : 8);
                    y = D_80115E28[D_80151AD0 - 1][i][1] + j * ((D_80151AD0 == 1) ? 15 : (D_80151AD0 == 2) ? 12 : 8);
                }
                fcvt_wrapper(buf, D_80120E2C, func_800B78F0(i, j + 1));
                dispatch_handler(0);
                state_utility(x + 1, y + 1, buf);
                dispatch_handler(1);
                state_utility(x, y, buf);
            } else {
                model_data_load(D_801541A8[i].slot[j].id, 1, 15);
            }
        }
    }
    TB_SetJust_Coins(0, 3);
    HAL_SetTextDepth_Coins(-1.0f);
    return 1;
}

void _GetSemaphore_Coins(void) { osRecvMesg(D_801461D0, (void **)0, 1); }
void _ReleaseSemaphore_Coins(void) { osSendMesg(D_801461D0, (void *)0, 0); } s32 HAL_SetFont_Coins(s32 fontID) { s32 old; _GetSemaphore_Coins(); old = slot_state_setup(fontID); _ReleaseSemaphore_Coins(); return old; }

void TB_SetJust_Coins(s32 h, s32 v)
{
    D_80118E20.first = h;
    D_80118E20.second = v;
}

void HAL_SetTextDepth_Coins(f32 d) { render_helper(d); }
