/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;

typedef struct {
    u8 pad0[4];
    f32 time;               /* +0x004 */
    f32 pos[3];             /* +0x008 */
    u8 pad14[0xEE - 0x14];
    s8 blocked;             /* +0x0EE */
    u8 padEF[0x108 - 0xEF];
    f32 distance;           /* +0x108 */
    u8 pad10C[0x358 - 0x10C];
    s8 crashed;             /* +0x358 */
    u8 pad359[0x3B8 - 0x359];
} CarData;                  /* 0x3B8 */

typedef struct {
    u8 pad0[0x6C4];
    s16 moving;             /* +0x6C4 */
    u8 pad6C6[0x7E4 - 0x6C6];
    s16 next_cp;            /* +0x7E4 */
    u8 pad7E6[3];
    s8 side;                /* +0x7E9 */
    u8 pad7EA[0x808 - 0x7EA];
} Model;                    /* 0x808 */

typedef struct {
    f32 pos[3];
    f32 normal[3];
    s32 radius;
    u8 pad1C[0x50 - 0x1C];
} CheckPoint;               /* 0x50 */

typedef struct {
    u8 pad0[7];
    u8 type;
} Player;                   /* 8 */

extern CarData D_80152818[];
extern Model D_8014A250[];
extern f32 D_80152218[][3];
extern CheckPoint D_80151CF4[];
extern Player D_80153E88[];
extern f32 D_801527E8[];
extern volatile s16 D_801543CA;
extern s32 D_8014A110;
extern f32 D_80152018;
extern volatile s16 D_80153FD2;
extern s8 D_80152031;
extern u8 D_8014A118;
extern u32 D_801174B4;
extern s8 D_8010FFC0;
extern volatile f32 D_8002EB90;
extern volatile f32 D_8002EB94;
extern f32 func_8008B3C8(f32 *v);
extern void race_countdown_display(Model *m, f32 t);
extern void world_gravity_apply(void);
extern void func_800D1AB0(void);
extern s32 func_800B61A8(s32 arg0, s32 arg1, s32 arg2, u8 arg3);

void func_800F8EC8(void)
{
    f32 cent_dist;
    f32 diff[3];
    f32 move[3];
    s16 index;
    s32 side;
    f32 plane;
    CarData *gc;
    f32 *prev;
    Model *m;

    for (index = 0; index < D_801543CA; index++) {
        gc = &D_80152818[index];
        prev = D_80152218[index];
        move[0] = gc->pos[0] - prev[0];
        move[1] = gc->pos[1] - prev[1];
        move[2] = gc->pos[2] - prev[2];
        gc->distance += func_8008B3C8(move);
        prev[0] = gc->pos[0];
        prev[1] = gc->pos[1];
        prev[2] = gc->pos[2];
        if ((D_80153E88[index].type == 0 || D_80153E88[index].type == 6) && gc->crashed == 0) {
            m = &D_8014A250[index];
            if (m->moving == -1) {
                diff[0] = gc->pos[0] - D_80151CF4[m->next_cp].pos[0];
                diff[1] = gc->pos[1] - D_80151CF4[m->next_cp].pos[1];
                diff[2] = gc->pos[2] - D_80151CF4[m->next_cp].pos[2];
                plane = diff[0] * D_80151CF4[m->next_cp].normal[0] + diff[1] * D_80151CF4[m->next_cp].normal[1] + diff[2] * D_80151CF4[m->next_cp].normal[2];
                if (plane < 0.0f) {
                    side = -1;
                } else {
                    side = 1;
                }
                if (side != m->side) {
                    m->side = side;
                    if (diff[0] * diff[0] + diff[2] * diff[2] < D_80151CF4[m->next_cp].radius) {
                        race_countdown_display(m, gc->time - D_8002EB94 * (plane / (plane - D_801527E8[index])));
                    }
                }
                D_801527E8[index] = plane;
            }
        }
    }
    if (D_8014A110 == 1) {
        return;
    }
    world_gravity_apply();
    func_800D1AB0();
    if (D_80152018 != 0.0f && D_8002EB90 - D_80152018 > 3.0f) {
        D_80152018 = 0.0f;
    }
    if (D_80153FD2 == 1 && D_80152818[D_8014A118].blocked == 0 && !(D_801174B4 & 8)) {
        if (D_80152031 == 0 && D_80152018 == 0.0f) {
            D_80152018 = D_8002EB90;
            func_800B61A8(16, D_8014A118, 1, 1);
        }
        D_80152031 = 1;
    } else {
        D_80152031 = 0;
    }
}
