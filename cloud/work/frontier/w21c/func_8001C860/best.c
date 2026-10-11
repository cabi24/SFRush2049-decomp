/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
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
