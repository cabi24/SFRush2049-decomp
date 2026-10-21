/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
extern Pair D_80118E20;
extern RecordRow D_801541A8[];
extern Cam152 D_80150B70[];
extern u8 D_80140BDC;
extern Model68 D_8012E710[];
extern f32 D_80116178;
extern s32 D_80394050[][4][2];
extern s32 D_80115E28[][4][2];
extern s8 D_80146113;
extern Car952 player_array[];
extern s32 D_80114C48[];
extern u8 D_801461D0[];

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
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osSendMesg(void *, void *, s32);
extern s32 slot_state_setup(s32);

static void font_lock(void) {
    osRecvMesg(D_801461D0, 0, 1);
}

static void font_unlock(void) {
    osSendMesg(D_801461D0, 0, 0);
}

static s32 font_select(s32 sel) {
    s32 old;
    font_lock();
    old = slot_state_setup(sel);
    font_unlock();
    return old;
}

static void pair_set(s16 first, s16 second) {
    Pair *p = &D_80118E20;
    p->first = first;
    p->second = second;
}

s32 particles_update(void *arg0) {
    Pos pos;
    char buf[8];
    s16 i;
    s16 j;

    render_helper(0.0f);
    if (D_80151AD0 == 1) {
        font_select(10);
    } else {
        font_select(12);
    }
    pair_set(0, 1);
    for (i = 0; i < D_80151AD0; i++) {
        for (j = 0; j < 2; j++) {
            if (D_801541A8[i].slot[j].id == -1) {
                char *names[2] = { "GOLDCOING_COIN", "SILVERCOINS_COI" };
                D_801541A8[i].slot[j].id = func_8008E26C(string_copy_format(names[j], 0, D_80140BDC - 1, 1), D_801541A8[i].slot[j].pos, -1, 0);
                model_data_load(D_801541A8[i].slot[j].id, 1, 15);
                D_8012E710[D_801541A8[i].slot[j].id].scale = 10.0f;
            }
            func_8008B32C(&D_80150B70[i], D_801541A8[i].slot[j].pos,
                          (D_8014A108 == 1) ? 0.03f : (D_8014A108 == 2) ? 0.03f : 0.04f);
            func_80090E9C(D_80116178, D_801541A8[i].slot[j].pos);
            if (gameplay_mode == 4) {
                pos.x = D_80394050[D_80151AD0 - 1][i][0];
                pos.y = ((D_80151AD0 == 1) ? 15 : (D_80151AD0 == 2) ? 12 : 8) * j + D_80394050[D_80151AD0 - 1][i][1];
            } else {
                pos.x = D_80115E28[D_80151AD0 - 1][i][0];
                pos.y = ((D_80151AD0 == 1) ? 15 : (D_80151AD0 == 2) ? 12 : 8) * j + D_80115E28[D_80151AD0 - 1][i][1];
            }
            car_lights_render(i, &pos, &D_80150B70[i], 2.0f, D_801541A8[i].slot[j].obj);
            if (D_80146113 && player_array[i].mode != 1) {
                s16 x;
                s16 y;

                model_transform_setup(D_801541A8[i].slot[j].id, 0, D_80114C48[i]);
                if (gameplay_mode == 4) {
                    x = ((D_80151AD0 == 1) ? 12 : 8) + D_80394050[D_80151AD0 - 1][i][0];
                    y = ((D_80151AD0 == 1) ? 15 : (D_80151AD0 == 2) ? 12 : 8) * j + D_80394050[D_80151AD0 - 1][i][1];
                } else {
                    x = ((D_80151AD0 == 1) ? 12 : 8) + D_80115E28[D_80151AD0 - 1][i][0];
                    y = ((D_80151AD0 == 1) ? 15 : (D_80151AD0 == 2) ? 12 : 8) * j + D_80115E28[D_80151AD0 - 1][i][1];
                }
                fcvt_wrapper(buf, "%d/8", func_800B78F0(i, j + 1));
                dispatch_handler(0);
                state_utility(x + 1, y + 1, buf);
                dispatch_handler(1);
                state_utility(x, y, buf);
            } else {
                model_data_load(D_801541A8[i].slot[j].id, 1, 15);
            }
        }
    }
    pair_set(0, 3);
    render_helper(-1.0f);
    return 1;
}
