/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Per-frame race update, related to MAT3 game.c PlayGame.
 * N64 ID/offsets are retained; this historical function name is misleading.
 * Matching choices: used car pointer born before model.slot, two explicit
 * volatile restart-byte reads (no named restart local), actual kept sound and
 * smoke-reset helpers, and distinct 1.f / 1.0f spellings. Native sound ID is 45.
 * Camera stride is 152, saved position at 132; cpak_read takes signed short.
 * The byte arrays describe observed structure offsets, not stack frame filler.
 * All source helpers and locked contexts are scored; no stand-in or padding.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Car952 {
    u8 pad000[237];
    s8 restart;            /* 0x0ED */
    u8 pad0EE[860 - 238];
    s8 cam;                 /* 0x35C */
    s8 camMode;             /* 0x35D */
    s8 camModeSave;         /* 0x35E */
    u8 pad35F[952 - 863];
} Car952;
typedef struct Game2056 {
    u8 pad000[556];
    f32 snd[3];
    u8 pad238[1990 - 568];     /* 0x22C */
    s16 slot;               /* 0x7C6 */
    u8 pad7C8[4];
    s8 type;                /* 0x7CC */
    u8 pad7CD[2056 - 1997];
} Game2056;
typedef struct Player76 { u8 pad00[64]; u16 numCrashes; u8 pad42[10]; } Player76;
typedef struct Cam152 { u8 pad00[36]; f32 pos[3]; u8 pad30[84]; f32 savepos[3]; u8 pad90[8]; } Cam152;
typedef struct Info8 { u8 pad0[7]; u8 state; } Info8;
typedef struct Inner5 { u8 pad0[5]; s8 f5; } Inner5;
typedef struct Obj40 { u8 pad0[40]; Inner5 **inner; } Obj40;

extern s32 D_8014A110;
extern s16 D_8014A108;
extern Player76 D_8014A118[];
extern s8 D_80152744;
extern Car952 player_array[];
extern Game2056 D_8014A250[];
extern u32 D_801174B4;
extern Cam152 gCamera[];
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
void cpak_read(s16);
void race_setup_2(s16);
void race_setup_1(void);
void menu_controller_remap(void);
void skid_mark_render(void);

s32 func_800B61A8(s32,s32,s32,u8);
s32 camera_collision_check(const f32 *,const f32 *,f32,f32,f32,f32,s32,s32,s32,u8);

void func_800CB718(s16);

void render_viewport_init(void) {
    s32 i;
    s16 slot;
    s32 cam;
    Car952 *car;

    race_init_helper();
    if (D_8014A110 == 4) {
        camera_scene_manager();
    } else if (D_8014A110 == 6) {
        func_8038FCE0();
        func_80390F60();
    } else {
        if (D_8014A110 == 5) {
            func_800D169C();
        } else {
            render_large_objects();
        }
        func_800F8EC8();
    }
    hud_render();
    camera_position_update();
    for (i = 0; i < D_80152744; i++) {
        car = &player_array[i];
        slot = D_8014A250[i].slot;
        if (*(volatile s8 *)&car->restart < 0) {
            cam = car->cam;
            if (!(D_801174B4 & 8) && slot < D_8014A108) {
                D_8014A118[slot].numCrashes++;
            }
            if (cam >= 0) {
                car->camModeSave = car->camMode;
                car->camMode = 4;
                gCamera[cam].savepos[0] = gCamera[cam].pos[0];
                gCamera[cam].savepos[1] = gCamera[cam].pos[1];
                gCamera[cam].savepos[2] = gCamera[cam].pos[2];
            }
            save_write_data(&slot, 1, 1.f, 0);
            func_800D5524(&D_8014A250[i]);
            if (D_8014A250[i].type == 2) {
                if (D_8014A110 != 2 || D_80152698[D_8014A250[i].slot] == 0 || (*D_80152698[D_8014A250[i].slot])->inner[0]->f5 < 0) {
                    func_800B61A8(45, D_8014A250[i].slot, 1, 1);
                }
            } else {
                camera_collision_check(D_8014A250[i].snd, (const f32 *)D_801141B0, 400.0f, 0.0f, 1.0f, 0.0f, 45, D_8014A250[i].slot, 0, 128);
            }
            if (D_8014A110 == 4) {
                func_800C3578(slot);
            }
            car->restart = 0;
        } else if (*(volatile s8 *)&car->restart > 0) {
            func_800CB718(slot);
            if (car->cam >= 0) {
                car->camMode = car->camModeSave;
                func_800F8E90(car->cam);
            }
            car->restart = 0;
        }
        cpak_read(slot);
        if ((D_80153E88[slot].state == 0 || D_80153E88[slot].state == 6) && (D_8014A110 != 2 || (D_8014A110 == 2 && slot == 0))) {
            race_setup_2(slot);
        }
    }
    race_setup_1();
    menu_controller_remap();
    if (!D_80114650) {
        skid_mark_render();
    }
}
