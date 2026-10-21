/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
    if (/*@{c1*/fabsf(delta[1]) > 3.0f/*@| 3.0f < fabsf(delta[1]) @}*/) return; \
    edge_length2 = /*@{c2*/delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2]/*@| delta[2] * delta[2] + delta[1] * delta[1] + delta[0] * delta[0] @| delta[0] * delta[0] + (delta[1] * delta[1] + delta[2] * delta[2]) @}*/; \
    if (/*@{c3*/edge_length2 > 200.0f/*@| 200.0f < edge_length2 @}*/) return; \
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
    /*@{p*/s32 pad[10];/*@| s32 pad[11]; @| s32 a0,a1,a2,a3,a4,a5,a6,a7,a8,a9; @| f32 pad[10]; @}*/

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
    /*@{u*/func_8008C074(D_80399A70.polys[damslot], 0, xyz, 0, D_803948B8, 0x60f, 0);/*@| func_8008C074(D_80399A70.polys[damslot], 0, &xyz[0], 0, &D_803948B8[0], 0x60f, 0); @}*/
}
