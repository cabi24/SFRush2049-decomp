/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B, 0x8038A408..0x8038A8CC (1220 bytes): newWorldDamage(pos, slot) with the static findDamageSlot inlined
 * by umerge (its deleted-procedure stub is B:8038A400).  Places a 25-slot ring of 4-vertex scorch decals under the
 * car: bail if any wheel's airdist (model[slot] +0x5EC) > 8, drop four corners 3 units above pos +-6 in x/z, snap
 * each onto the road with camera_trigger_check (= RoadPosition, 0.25 above the hit), reject the decal when any edge
 * is steeper than 3 or longer than sqrt(200), then recycle/create the ring poly (func_8008C874 = ZOID_HidePoly,
 * func_800A78BC = ZOID_NewPoly, func_8008C074 = ZOID_UpdatePoly) and fade the four older decals.
 * Donor: MAT3 GameCube decomp, GameSource/src/worlddam.c newWorldDamage (GC 0x8005E9DC) + findDamageSlot
 * (GC 0x8005E880); MAT3's trailing ZOID_SetPolyFlags is absent in the N64 body.
 * Builds on lane w35e (cloud/work/frontier/w35e/func_8038A408/best.c, 4/305): types, offsets, the single
 * function-level delta[3]/edge_length2 (Dreamcast reconstruction shape: only the LAST edge's three delta stores
 * survive in retail) and the frame arithmetic below are w35e's.
 * w36j lever (closes the last 4 words): the four edge checks are written out, each final test as a multi-line
 * `if (...) {\n return;\n }`.  as1's cross-block hoist pulls the ring address (`la D_80399A70`, emitted with the
 * line of the last if's fall-through) into the block of the delta stores; as1 breaks equal-priority ties by
 * source line, so the address must carry a later line than the stores (a one-line macro gives them all one line).
 * Types/offsets: ring D_80399A70 {s8 full; u16 oldest; poly *polys[25]} (0x68), D_80399AD8 = damTexture,
 * D_80394888 = dummyVtx[4][3], D_803948B8 = damColor[5], model[] stride 0x808.
 * Disclosed frame residual (Wave 12/16 rule): retail frame 248 bytes; without the ten unused s32 below the same
 * source compiles to 208 (Udef 148 B + 3 spill temps); nine give 240, ten give 248.  They sit after `m` (between
 * the caller's locals and the inlined callee's).  Owner unknown (no second inlined static exists: the only stub is
 * B:8038A400); not recovered names, no behaviour.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;

typedef struct tPoly tPoly;
typedef struct surftyp surftyp;
typedef struct tRGBA { u8 c[4]; } tRGBA;

typedef struct tModelData {
    u8 unknown000[0x5ec];
    f32 airdist[4];
    u8 unknown5fc[0x808 - 0x5fc];
} tModelData;

typedef struct DamageRing {
    s8 full;
    u16 oldest;
    tPoly *polys[25];
} DamageRing;

extern tModelData D_8014A250[];
extern DamageRing D_80399A70;
extern u16 D_80399AD8;
extern f32 D_80394888[4][3];
extern tRGBA D_803948B8[5];

extern surftyp *camera_trigger_check(f32 *, f32 *, f32 (*)[3]);
extern void func_8008C874(tPoly *);
extern tPoly *func_800A78BC(s32, f32 (*)[3], u16, tRGBA *, u16, s32);
extern void func_8008C074(tPoly *, s32, f32 (*)[3], u16, tRGBA *, u16, s32);

extern f32 fabsf(f32);
#pragma intrinsic(fabsf)


s32 func_8038A400(void)
{
    s32 i;
    s32 j;
    s32 fade;

    if (D_80399A70.full) {
        func_8008C874(D_80399A70.polys[D_80399A70.oldest]);
        i = D_80399A70.oldest;
        if (++D_80399A70.oldest >= 25) {
            D_80399A70.oldest = 0;
        }
    } else {
        i = D_80399A70.oldest;
        D_80399A70.polys[i] = func_800A78BC(4, D_80394888, D_80399AD8, D_803948B8, 0x820f, 1);
        if (++D_80399A70.oldest >= 25) {
            D_80399A70.oldest = 0;
            D_80399A70.full = 1;
        }
    }
    for (j = 1; j < 5; j++) {
        fade = i + j;
        if (fade >= 25) {
            fade -= 25;
        }
        if (D_80399A70.polys[fade]) {
            func_8008C074(D_80399A70.polys[fade], 0, 0, 0, &D_803948B8[j], 0, 0);
        }
    }
    return i;
}

void func_8038A408(f32 *pos, s32 slot) /* newWorldDamage */
{
    f32 newpos[3];
    f32 uvs[3][3];
    f32 xyz[4][3];
    f32 delta[3];
    f32 edge_length2;
    s32 damslot;
    s32 i;
    tModelData *m;
    s32 unused_0, unused_1, unused_2, unused_3, unused_4, unused_5, unused_6, unused_7, unused_8, unused_9;

    m = &D_8014A250[slot];
    if (m->airdist[0] > 8.0f || m->airdist[1] > 8.0f || m->airdist[2] > 8.0f || m->airdist[3] > 8.0f) {
        return;
    }
    for (i = 0; i < 4; i++) {
        xyz[i][0] = pos[0];
        xyz[i][1] = pos[1];
        xyz[i][2] = pos[2];
        xyz[i][1] += 3.0f;
        switch (i) {
        case 0:
            xyz[0][0] += -6.0f;
            xyz[0][2] += 6.0f;
            break;
        case 1:
            xyz[1][0] += -6.0f;
            xyz[1][2] += -6.0f;
            break;
        case 2:
            xyz[2][0] += 6.0f;
            xyz[2][2] += -6.0f;
            break;
        case 3:
            xyz[3][0] += 6.0f;
            xyz[3][2] += 6.0f;
            break;
        }
        if (camera_trigger_check(xyz[i], newpos, uvs)) {
            xyz[i][0] = newpos[0];
            xyz[i][1] = newpos[1];
            xyz[i][2] = newpos[2];
            xyz[i][1] += 0.25f;
        }
    }
    delta[0] = xyz[0][0] - xyz[1][0];
    delta[1] = xyz[0][1] - xyz[1][1];
    delta[2] = xyz[0][2] - xyz[1][2];
    if (fabsf(delta[1]) > 3.0f) return;
    edge_length2 = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    if (edge_length2 > 200.0f) {
        return;
    }
    delta[0] = xyz[1][0] - xyz[2][0];
    delta[1] = xyz[1][1] - xyz[2][1];
    delta[2] = xyz[1][2] - xyz[2][2];
    if (fabsf(delta[1]) > 3.0f) return;
    edge_length2 = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    if (edge_length2 > 200.0f) {
        return;
    }
    delta[0] = xyz[2][0] - xyz[3][0];
    delta[1] = xyz[2][1] - xyz[3][1];
    delta[2] = xyz[2][2] - xyz[3][2];
    if (fabsf(delta[1]) > 3.0f) return;
    edge_length2 = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    if (edge_length2 > 200.0f) {
        return;
    }
    delta[0] = xyz[3][0] - xyz[0][0];
    delta[1] = xyz[3][1] - xyz[0][1];
    delta[2] = xyz[3][2] - xyz[0][2];
    if (fabsf(delta[1]) > 3.0f) return;
    edge_length2 = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
    if (edge_length2 > 200.0f) {
        return;
    }
    damslot = func_8038A400();
    func_8008C074(D_80399A70.polys[damslot], 0, xyz, 0, D_803948B8, 0x60f, 0);
}
