/* camera_victory (0x800C6404, 1468 B): per-car wall/obstacle collision against the track's polygon quadtree
 * (the label is historical). For each of the 4 wheels, floor the (x, z) position and look up its quadtree
 * leaf (handbrake_apply); collect the distinct leaves; unpack each leaf's polygon list (func_800ADCE0) and run
 * camera_play_script on every new non-floor polygon (flag 0x4000 marks visited). Then, for wall polygons
 * (0x2000), test each wheel against the polygon (func_800C3AD0); on a hit project it (steering_sensitivity),
 * and when the wheel moved into the wall push it out (func_800C36A0). Finally the wheel positions become the
 * previous positions. w15f reconstruction from the retail disassembly (first draft of this function).
 * STATUS: NOT a match. Unit: 250/367 words differ, 368 vs 367 words, frame 584 = retail, 150 aligned rows;
 * by mnemonic only ~30 rows. Residual is register colouring: retail keeps `car` in s4 between the internal
 * calls (reloaded from its home sp+584 after each), ours splits every piece of car (ctrace: callee regs all
 * forbidden: car is live across func_800C3AD0, whose IPA register parameters are s0,s2,s4-s8,f20, and
 * func_800C36A0's s1,s2); plus loop 1's exit test (retail slti 4i<16, ours li/bne) and the loop-1 s-reg order.
 * Frame/slot facts used: 12 named slots above `poly` (sp+184); the two FLOOR ternaries are cfe temps that take
 * the two bottom named slots; `p` (one pointer variable shared by the leaf lookup and the loop-2 polygon)
 * gives retail's split of loop-2/loop-4 polygon webs (loop 4's `poly` is homed at sp+184 as in retail). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)

typedef struct QNode {
    s16 next;
    u8 pad2;
    u8 mask;
    s16 x0;
    s16 x1;
    s16 y0;
    s16 y1;
    u16 child[4];
} QNode;

typedef struct Poly {
    u16 type;
    u16 cnt;
    u8 body[0x12];
    u16 off;
} Poly;

typedef struct CCol {
    Poly *poly;
    f32 m[9];
    s16 idx;
    s16 idx2;
    f32 k;
} CCol;

typedef struct VCar {
    u8 p0[628];
    f32 pos[4][3];      /* 628: wheel positions */
    f32 old[4][3];      /* 676: previous wheel positions */
    u8 p724[204];
    QNode *node[4];     /* 928 */
} VCar;

extern Poly *D_801497F8;
extern u8 *D_80152460;
extern s8 D_801174C4[];

QNode *handbrake_apply(QNode *n, s16 x, s16 y, s16 *out);
void func_800ADCE0(u8 *input, s32 count, u16 *front, u32 marker);
void camera_play_script(void *car, Poly *poly, CCol *col);
s16 func_800C3AD0(Poly *poly, f32 *wp, f32 *q, f32 *pt, f32 *bound, s16 *outIdx, f32 *mat, f32 zmin);
void steering_sensitivity(s32 arg0, u16 idx, f32 *position, f32 *outPosition, f32 (*outMatrix)[3], f32 threshold);
void func_800A61B0(f32 *in, f32 *out, f32 *mat);
void func_800C36A0(void *car, CCol *col);

#define FLOOR(v) (((v) < 0.0f && (f32)(s32)(v) != (v)) ? (v) - 1.0f : (v))

void camera_victory(VCar *car)
{
    QNode *cells[4];
    s16 cellIdx[4];
    u16 polys[142];
    s32 npoly;
    s32 ncells = 0;
    s32 j;
    s32 i;
    s32 k;
    s16 out;
    u8 cnt;
    CCol col = { 0 };
    s32 unique;
    u32 off;
    u8 *q;
    Poly *p;
    Poly *poly;
    f32 bound;
    f32 pt[3];
    f32 w[3];
    s16 outIdx;

    for (j = 0; j < 4; j++) {
        p = (Poly *)handbrake_apply(car->node[j], (s32)FLOOR(car->pos[j][0]), (s32)FLOOR(car->pos[j][2]), &out);
        if (p != NULL) {
            unique = 1;
            for (i = ncells - 1; i >= 0; i--) {
                if ((QNode *)p == cells[i] && out == cellIdx[i]) {
                    unique = 0;
                    break;
                }
            }
            if (unique) {
                cellIdx[ncells] = out;
                cells[ncells] = (QNode *)p;
                ncells++;
            }
        }
    }
    npoly = 0;
    for (j = 0; j < ncells; j++) {
        off = cells[j]->child[cellIdx[j]];
        if (cells[j]->mask & (16 << cellIdx[j])) {
            off |= 0x10000;
        }
        if (off != 0) {
            q = D_80152460 + off;
            cnt = *q++;
            func_800ADCE0(q, cnt, &polys[npoly], 0);
            for (i = npoly; i < cnt + npoly; i++) {
                p = &D_801497F8[polys[i]];
                if (col.poly == p || (p->type & 0x4000) || (p->type & 0x2000) || (p->type & 0xF) == 15) {
                    continue;
                }
                camera_play_script(car, p, &col);
                p->type |= 0x4000;
            }
            npoly += cnt;
        }
    }
    if (col.poly != NULL) {
        func_800C36A0(car, &col);
    }
    for (j = 0; j < npoly; j++) {
        poly = &D_801497F8[polys[j]];
        poly->type &= ~0x4000;
        if (poly->type & 0x2000) {
            for (i = 0; i < 4; i++) {
                bound = 250.0f;
                if (func_800C3AD0(poly, car->pos[D_801174C4[i]], car->old[D_801174C4[i]], pt, &bound, &outIdx, col.m, -5.0f) != 0) {
                    steering_sensitivity((s32)poly, outIdx, car->pos[i], pt, (f32 (*)[3])col.m, 0.0f);
                    bound = pt[1];
                    if (bound < -5.0f || bound >= 0.0f) {
                        continue;
                    }
                    pt[0] = car->pos[D_801174C4[i]][0] - car->old[D_801174C4[i]][0];
                    pt[1] = car->pos[D_801174C4[i]][1] - car->old[D_801174C4[i]][1];
                    pt[2] = car->pos[D_801174C4[i]][2] - car->old[D_801174C4[i]][2];
                    func_800A61B0(pt, w, col.m);
                    if (w[1] >= 0.0f) {
                        continue;
                    }
                    col.poly = poly;
                    col.idx = i;
                    col.idx2 = i;
                    col.k = -bound;
                    func_800C36A0(car, &col);
                }
            }
        }
    }
    for (k = 0; k < 4; k++) {
        car->old[k][0] = car->pos[k][0];
        car->old[k][1] = car->pos[k][1];
        car->old[k][2] = car->pos[k][2];
    }
}
