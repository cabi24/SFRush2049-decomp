/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* PROVISIONAL (4 words off + disclosed frame residual), runtime image B only: 0x8038A408..0x8038A8CC, 1220 bytes.
 * newWorldDamage(pos, slot) -- MAT3 worlddam.c newWorldDamage (GC 0x8005E9DC) with the static findDamageSlot
 * (GC 0x8005E880) inlined by umerge (its deleted-procedure stub is B:8038A400).  Place a 25-slot ring of 4-vertex
 * ground decals (scorch marks) under the car: bail if any wheel's airdist (model[slot], +0x5EC) > 8, drop four
 * corners 3 units above pos +-6 in x/z, snap each onto the road with camera_trigger_check (= RoadPosition, 0.25 above
 * the hit), reject the decal when any edge is steeper than 3 or longer than sqrt(200), then recycle/create the ring
 * poly (func_8008C874 = ZOID_HidePoly, func_800A78BC = ZOID_NewPoly, func_8008C074 = ZOID_UpdatePoly) and fade the
 * four older decals.  MAT3's trailing ZOID_SetPolyFlags is absent in the N64 body.
 * Types/offsets: ring D_80399A70 {s8 full; u16 oldest; poly *polys[25]} (0x68), D_80399AD8 = damTexture,
 * D_80394888 = dummyVtx[4][3], D_803948B8 = damColor[5], model[] stride 0x808.
 * EDGE_CHECK follows the Dreamcast reconstruction (reference/repos/dreamcast/src/worlddam.c): ONE function-level
 * `delta[3]` + `edge_length2` (that is what leaves only the LAST edge's three stores in retail), `fabsf` intrinsic.
 *
 * Frame: retail 248 bytes.  Without the ten unused scalars below the same source compiles to a 208-byte frame
 * (Udef 148 B + 3 spill temps -> 160 -> 208); with 10 of them 188 -> 200 -> 248 (nine give 240, ten or eleven give
 * 248).  They are declared after `m`, i.e. between the caller's function-level locals and the inlined callee's
 * (the same place block-scope locals of the original body would be allocated: the GC macro declares block-scope
 * floats that DWARF omits).  Owner unknown; EXACT frame residual only, no behaviour.
 * Remaining 4 words: as1 puts `addiu v1,v1,-26000` (the address of the ring) right after its `lui` instead of last in
 * the block, ahead of the three delta stores (+0x310..+0x324); independent of how the 10 words are declared and of ~500
 * source variants (macro operand order, comparison direction, pad shape, static order).
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

#define EDGE_CHECK(a, b) { \
    delta[0] = (a)[0] - (b)[0]; \
    delta[1] = (a)[1] - (b)[1]; \
    delta[2] = (a)[2] - (b)[2]; \
    if (fabsf(delta[1]) > 3.0f) return; \
    edge_length2 = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2]; \
    if (edge_length2 > 200.0f) return; \
}

static s32 findDamageSlot(void)
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

void func_8038A408(f32 *pos, s32 slot)
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
    EDGE_CHECK(xyz[0], xyz[1]);
    EDGE_CHECK(xyz[1], xyz[2]);
    EDGE_CHECK(xyz[2], xyz[3]);
    EDGE_CHECK(xyz[3], xyz[0]);
    damslot = findDamageSlot();
    func_8008C074(D_80399A70.polys[damslot], 0, xyz, 0, D_803948B8, 0x60f, 0);
}
