/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800F8EC8 -- N64 descendant of arcade CheckCPs() (game/checkpoint.c:866, scp.c:860).
 * For every car (count D_801543CA): accumulate the distance driven since last frame into
 * CarData.distance (+0x108) and remember the position (D_80152218[car]); for a live car
 * (player type 0 or 6, not crashed) whose model is not resurrecting (moving == -1), take the
 * side of its next checkpoint's plane; on a side change inside the checkpoint radius report the
 * interpolated crossing time (time - frame_dt * plane / (plane - last_plane)) through
 * race_countdown_display (arcade PassedCP); keep the plane value per car (D_801527E8).
 * Then, unless D_8014A110 == 1: world_gravity_apply (arcade find_maxpath_intervals),
 * func_800D1AB0 (the place sort), expire the first-place timer after 3 s, and start the
 * first-place effect (arcade SOUND(S_LEADERLIGHT) = func_800B61A8(16, car, 1, 1), the locked
 * sound wrapper, inlined by umerge).
 *
 * Shaping (each one measured in the whole-program unit):
 *  - the model pointer goes through an integer cast, (Model *)((u32)D_8014A250 + ...): with
 *    &D_8014A250[index] ugen emits `.noalias $4,$sp` and as1 hoists the m->next_cp reloads and the
 *    m->side store across the diff[] stack stores (57 -> 28 words);
 *  - calling func_800B61A8 instead of an open-coded `if (D_8010FFC0) entity_flags_apply(...)`:
 *    gives the argument set-up before the test, `li a3,1` and the inverted branch, and also
 *    removes three hoisted invariants from s6-s8 (244 -> 58 words);
 *  - `if (plane < 0) side = -1; else side = 1;` (the empty-else `b`), `diff . normal` in
 *    x, y, z order, D_801543CA / D_8002EB90 / D_8002EB94 / D_80153FD2 volatile;
 *  - the crossing time goes through one named local assigned twice (cent_dist), which puts
 *    the quotient and the product in $f0;
 *  - frame: arcade declarations reordered; cent_dist and an unused `s16 i` above diff[3],
 *    the scalars between diff[3] and move[3], an unused zvec[3] below (168 bytes, diff at
 *    148, move at 112).
 */
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
    s16 i;
    f32 diff[3];
    s16 index;
    s32 side;
    f32 plane;
    CarData *gc;
    f32 *prev;
    Model *m;
    f32 move[3];
    f32 zvec[3];

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
            m = (Model *)((u32)D_8014A250 + index * sizeof(Model));
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
                        cent_dist = plane / (plane - D_801527E8[index]);
                        cent_dist = D_8002EB94 * cent_dist;
                        race_countdown_display(m, gc->time - cent_dist);
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
