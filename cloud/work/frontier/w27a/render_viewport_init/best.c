/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Car952 {
    u8 pad000[237];
    s8 finished;            /* 0x0ED */
    u8 pad0EE[860 - 238];
    s8 cam;                 /* 0x35C */
    s8 camMode;             /* 0x35D */
    s8 camModeSave;         /* 0x35E */
    u8 pad35F[952 - 863];
} Car952;
typedef struct Game2056 {
    u8 pad000[556];
    u8 snd[1990 - 556];     /* 0x22C */
    s16 slot;               /* 0x7C6 */
    u8 pad7C8[4];
    s8 type;                /* 0x7CC */
    u8 pad7CD[2056 - 1997];
} Game2056;
typedef struct Player76 { u8 pad00[64]; u16 finishes; u8 pad42[10]; } Player76;
typedef struct Cam152 { u8 pad00[36]; f32 pos[3]; u8 pad30[84]; f32 savepos[3]; u8 pad8C[12]; } Cam152;
typedef struct Info8 { u8 pad0[7]; u8 state; } Info8;
typedef struct Inner5 { u8 pad0[5]; s8 f5; } Inner5;
typedef struct Obj40 { u8 pad0[40]; Inner5 **inner; } Obj40;

extern s32 gameplay_mode;
extern s16 D_8014A108;
extern Player76 D_8014A118[];
extern s8 D_80152744;
extern Car952 player_array[];
extern Game2056 D_8014A250[];
extern u32 D_801174B4;
extern Cam152 D_80150B70[];
extern s8 D_8010FFC0;
extern Obj40 **D_80152698[];
extern u8 D_801141B0[];
extern u32 D_801392D8[];
extern Info8 D_80153E88[];
extern s8 D_80114650;

void race_init_helper(void);
void camera_scene_manager(void);
void func_8038FCE0(void);
void func_80390F60(void);
void func_800D169C(void);
void render_large_objects(void);
void func_800F8EC8(void);
void hud_render(void);
void camera_position_update(void);
void save_write_data(void *arg0, s32 arg1, f32 arg2, s32 arg3);
void func_800D5524(void *arg0);
s32 entity_flags_apply(s32 arg0, s32 arg1, s32 arg2, u8 arg3);
s32 camera_target_track(void *arg0, void *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8, u8 arg9);
void func_800C3578(s32 arg0);
void func_800F8E90(s16 arg0);
void cpak_read(s8);
void race_setup_2(s16);
s32 race_setup_1(void);
void menu_controller_remap(void);
void skid_mark_render(void);

void render_viewport_init(void) {
    s32 i;
    Car952 *car;
    Game2056 *g;
    s16 slot;
    s8 cam;

    race_init_helper();
    if (gameplay_mode == 4) {
        camera_scene_manager();
    } else if (gameplay_mode == 6) {
        func_8038FCE0();
        func_80390F60();
    } else {
        if (gameplay_mode == 5) {
            func_800D169C();
        } else {
            render_large_objects();
        }
        func_800F8EC8();
    }
    hud_render();
    camera_position_update();
    car = player_array;
    g = D_8014A250;
    for (i = 0; i < D_80152744; i++, car++, g++) {
        slot = g->slot;
        if (car->finished < 0) {
            cam = car->cam;
            if (!(D_801174B4 & 8) && slot < D_8014A108) {
                D_8014A118[slot].finishes++;
            }
            if (cam >= 0) {
                D_80150B70[cam].savepos[0] = D_80150B70[cam].pos[0];
                D_80150B70[cam].savepos[1] = D_80150B70[cam].pos[1];
                D_80150B70[cam].savepos[2] = D_80150B70[cam].pos[2];
                car->camModeSave = car->camMode;
                car->camMode = 4;
            }
            save_write_data(&slot, 1, 1.f, 0);
            func_800D5524(g);
            if (g->type == 2) {
                if (gameplay_mode != 2 || D_80152698[g->slot] == 0 || (*D_80152698[g->slot])->inner[0]->f5 < 0) {
                    if (D_8010FFC0) {
                        entity_flags_apply(45, g->slot, 1, 1);
                    }
                }
            } else if (D_8010FFC0) {
                camera_target_track(g->snd, D_801141B0, 400.0f, 0.0f, 1.0f, 0.0f, 45, g->slot, 0, 128);
            }
            if (gameplay_mode == 4) {
                func_800C3578(slot);
            }
            car->finished = 0;
        } else if (car->finished > 0) {
            D_801392D8[slot] &= ~0x20;
            if (car->cam >= 0) {
                car->camMode = car->camModeSave;
                func_800F8E90(car->cam);
            }
            car->finished = 0;
        }
        cpak_read(slot);
        if ((D_80153E88[slot].state == 0 || D_80153E88[slot].state == 6) && (gameplay_mode != 2 || (gameplay_mode == 2 && slot == 0))) {
            race_setup_2(slot);
        }
    }
    race_setup_1();
    menu_controller_remap();
    if (!D_80114650) {
        skid_mark_render();
    }
}
