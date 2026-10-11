/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* func_8001C860 - lib_1cf90 (IDO 7.1). MusyX snd3d.c CalcEmitter (an older revision than the GameCube
 * decomp doldecomp/ttyd libs/musyx: no listener volPosOff, no negative volPush branch, the surround
 * pan.z terms are double (1.0 / -1.0 literals), pan.y += p.y). For emitter EM, walk the listener list
 * D_8004FD54: distance -> volume falloff (maxDis 0x24, maxVol 0x28, minVol 0x2C, volPush 0x30) summed into
 * *vol; doppler from the relative velocity one frame (1/60 s) ahead; rotate the emitter position into
 * listener space (func_80024BF0 = salApplyMatrix), z -> surround pan, normalise (func_80024C9C =
 * salNormalizeVector), x/y pan; outputs are averaged over the listener count.
 * Callers: func_8001D1F4 (locked), func_8001DDE0 (lib_1cf90).
 * Types: the TU's prototype (first parameter StateNode *, declared after the slot) read through a private
 * emitter view by a cast macro EM (type view, no code); D_8004FD54 `LinkNode *` as the TU declares it,
 * cast to a private listener view (offsets = the TU's SpatialState_8001D764: mat 0x48, surround front/back
 * 0x78/0x7C, sound speed 0x80, volume 0x84). Private Vec_/Emitter_/Listener_ names because the TU's
 * `Vector` typedef follows the slot.
 * Shaping and disclosures:
 * - `if (n) {}` at the top of the listener loop (compiled out): a holder use of the listener count. Without
 *   it the four callee-saved webs n, &p, vol, doppler are coloured vol=s2, doppler=s3, n=s4, &p=s5 (true=75,
 *   register numbers only); retail has n=s2, &p=s3, vol=s4, doppler=s5. Declaration/statement order, loop
 *   shape and a pointer variable for &p do not move it (lane RESULTS.md).
 * - Local declaration order panz, pany, panx, n (retail stack homes: panx sp+0x80, pany sp+0x84, p sp+0xA0);
 *   the emitter is the parameter itself (a converted local copy takes a slot and moves p to sp+0x9C).
 * - `1.f / 60.f` is a natural literal: IDO puts it in the TU's .rodata; retail has it at 0x8002D900
 *   (0x3C888889, global .rodata). Lockable only with an owned .rodata slot 0x8002D900, 4-byte logical extent
 *   (the TU's own D_8002D904/908/90C follow it), w20c's owned_data prepare recipe. `extern const float
 *   D_8002D900` changes the code (the load is not rematerialised in the loop preheader: true=4805).
 * - Flags: -Wab,-r4300_mul is required (retail has the R4300 nop between back-to-back mul.s); the ROM-TU
 *   Makefile rule passes it for every TU, but lib_1cf90's lock flagset is the plain one (layout._segment_flagset
 *   needs one flagset per segment): see lane RESULTS.md.
 * No unused locals, no pad arrays, no false prototypes, no implicit declarations. */
#include "types.h"
#include "boot_tail_sample_contract.h"
typedef struct Vec_8001C860 { float x, y, z; } Vec_8001C860;
typedef struct Emitter_8001C860 {
    StateNode *next, *previous;
    u32 flags;
    Vec_8001C860 pos, dir;
    float maxDis, maxVol, minVol, volPush;
} Emitter_8001C860;
typedef struct Listener_8001C860 {
    struct Listener_8001C860 *next, *previous;
    u32 flags;
    Vec_8001C860 pos, dir, heading, right, up;
    float mat[12];
    float surroundDisFront, surroundDisBack, soundSpeed, vol;
} Listener_8001C860;
extern LinkNode *D_8004FD54;
extern float sqrtf(float);
extern void func_80024BF0(const void *, const void *, void *);
extern float func_80024C9C(void *);
#define EM ((Emitter_8001C860 *)em)
void func_8001C860(StateNode *em, float *vol, float *doppler, float *xPan, float *yPan, float *zPan)
{
    Listener_8001C860 *li;
    Vec_8001C860 d;
    Vec_8001C860 v;
    Vec_8001C860 p;
    float relspeed;
    float distance;
    float new_distance;
    float ft;
    float vd;
    float panz;
    float pany;
    float panx;
    u32 n;

    ft = 1.f / 60.f;
    *vol = 0.f;
    *doppler = 1.f;
    panx = pany = panz = 0.f;
    for (n = 0, li = (Listener_8001C860 *)D_8004FD54; li != NULL; li = li->next, ++n) {
        if (n) {}
        d.x = EM->pos.x - li->pos.x;
        d.y = EM->pos.y - li->pos.y;
        d.z = EM->pos.z - li->pos.z;
        distance = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
        if (EM->maxDis >= distance) {
            vd = distance / EM->maxDis;
            *vol += li->vol * (EM->minVol + (EM->maxVol - EM->minVol) *
                                (1.f - ((1.f - EM->volPush) * vd + EM->volPush * vd * vd)));
            if (!(EM->flags & 0x80000)) {
                if ((EM->flags & 0x8) || (li->flags & 1)) {
                    v.x = li->dir.x - EM->dir.x;
                    v.y = li->dir.y - EM->dir.y;
                    v.z = li->dir.z - EM->dir.z;
                    relspeed = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
                    if (relspeed > 0.f) {
                        d.x = (EM->pos.x + EM->dir.x * ft) - (li->pos.x + li->dir.x * ft);
                        d.y = (EM->pos.y + EM->dir.y * ft) - (li->pos.y + li->dir.y * ft);
                        d.z = (EM->pos.z + EM->dir.z * ft) - (li->pos.z + li->dir.z * ft);
                        new_distance = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
                        if (new_distance < distance) {
                            *doppler = li->soundSpeed / (li->soundSpeed - relspeed);
                        } else {
                            *doppler = li->soundSpeed / (li->soundSpeed + relspeed);
                        }
                    }
                }
                if (distance != 0.f) {
                    func_80024BF0(&li->mat, &EM->pos, &p);
                    if (p.z >= 0.f) {
                        panz += p.z < li->surroundDisFront ? p.z / li->surroundDisFront : 1.0;
                    } else {
                        panz += -li->surroundDisBack < p.z ? p.z / li->surroundDisBack : -1.0;
                    }
                    if (p.x != 0.0 || p.y != 0.0 || p.z != 0) {
                        func_80024C9C(&p);
                    }
                    panx += p.x;
                    pany += p.y;
                }
            }
        }
    }
    if (n != 0) {
        *xPan = panx / n;
        *yPan = pany / n;
        *zPan = panz / n;
    }
}
