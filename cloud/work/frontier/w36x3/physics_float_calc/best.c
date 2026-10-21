/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * physics_float_calc = WorldCullObjects (MAT3 world.c, GC 0x8005E2F4) with the static WorldDistance (GC 0x8005E0FC)
 * inlined (its deleted body is the caller-less `jr ra; nop` at 0x8009EBB8). Base: w36u best.c.
 * STATE: NONMATCH, 39 aligned rows (w36u: 86), same size (294 words), frame 136/136.
 * w36x3 changes: gTrackNum is D_8014978C (w36u used D_80149B8C; retail `lb a1,-26740(a1)`); MAT3 loop form
 * (`ptr[w]`, `ObjInstanceList[Worlddlidx + i]`, the dead ptr_no_clip clause) with ptr kept in a register by two
 * compiled-out `if (ptr) {}` reads in the loop (without them ptr's save is 0 and the eligibility gate leaves it in
 * memory, L55); `w` declared before ptr (ptr's home 100 as retail); the deleted debug call guarded by ptr_no_clip
 * (no dbg local needed).
 * Residual: dv homes 52/56/60 (retail 60/64/68: the inline area is 8 B larger), the ?: clause's cfe temp and the
 * dead `w` web take v0 so mask/i are v1/a1 (retail v0/v1), gTrackNum a0 (retail a1), IVs i*68/w*4 a2/a3 (retail a3/a2).
 */
typedef float f32;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef signed char s8;
typedef unsigned char u8;

typedef struct {
    char name[16];          /* 0x00 */
    f32 uvs[4][3];          /* 0x10 mat; pos = uvs[3] at 0x34 */
    u32 dlflags;            /* 0x40 */
    s16 next;               /* 0x44 */
    s16 child;              /* 0x46 */
    s16 sfx1;               /* 0x48 */
    s16 sfx2;               /* 0x4A */
    s32 tidx;               /* 0x4C */
    f32 min[3];             /* 0x50 */
    f32 max[3];             /* 0x5C */
} tModelPart; /* 0x68 */

typedef struct {
    u32 flags;
    char pad4[0x40];
} ObjInstance; /* 0x44 on the N64 */

extern tModelPart *D_80149B80;      /* WorldBase */
extern s8 D_8014978C;               /* gTrackNum */
extern s16 D_80149D90;              /* Worlddlidx */
extern s8 D_8014061A;               /* gInvisibleTrack */
extern ObjInstance D_8012E700[];    /* ObjInstanceList */
extern u8 D_8011E748[];             /* trackParts */
extern u32 D_8011E75C[];            /* showAllParts */
extern u32 D_8011B898[][4], D_8011BFE8[][4], D_8011C738[][4], D_8011CE88[][4], D_8011D618[][4], D_8011DC88[][4];
extern u32 D_8011E428[][4], D_8011E438[][4], D_8011E448[][4], D_8011E458[][4], D_8011E468[][4], D_8011E548[][4];
extern u32 D_8011E558[][4], D_8011E568[][4], D_8011E578[][4], D_8011E588[][4], D_8011E598[][4], D_8011E5A8[][4];
extern u32 D_8011E5B8[][4];

void ShowCull();
float sqrtf(float);
#pragma intrinsic(sqrtf)

static void WorldDistance(tModelPart *Wnode, f32 *pos, s32 *best, f32 *dist, s32 *widx, s32 *index)
{
    f32 dv[3];
    f32 d;

    for (;;) {
        dv[0] = pos[0] - Wnode->uvs[3][0];
        dv[1] = pos[1] - Wnode->uvs[3][1];
        dv[2] = pos[2] - Wnode->uvs[3][2];

        if (Wnode->min[0] <= dv[0] && dv[0] <= Wnode->max[0] && Wnode->min[1] <= dv[1] && Wnode->min[2] <= dv[2] &&
            dv[2] <= Wnode->max[2]) {
            d = sqrtf(dv[0] * dv[0] + dv[2] * dv[2]);
            if (dv[1] > 0.75f * Wnode->max[1]) {
                d += dv[1] - Wnode->min[1];
            }
            if (*best < 0 || d < *dist) {
                *best = *index;
                *widx = Wnode - D_80149B80;
                *dist = d;
            }
        }
        if (Wnode->next < 0) {
            break;
        }
        (*index)++;
        Wnode = D_80149B80 + Wnode->next;
    }
}

void physics_float_calc(s32 unused, f32 *pos, s32 show)
{
    s32 dlindex;
    s32 windex;
    f32 dist;
    s32 best;
    s32 parts;
    s32 i;
    u32 mask;
    s32 w;
    u32 *ptr;
    u32 *ptr_no_clip;

    dlindex = 0;
    windex = 0;
    best = -1;
    ptr_no_clip = 0;
    if (!D_80149B80) {
        return;
    }
    WorldDistance(D_80149B80, pos, &best, &dist, &windex, &dlindex);
    if (best < 0) {
        ptr = D_8011E75C;
    } else {
        switch (D_8014978C) {
        case 0: ptr = D_8011B898[best]; break;
        case 1: ptr = D_8011BFE8[best]; break;
        case 2: ptr = D_8011C738[best]; break;
        case 3: ptr = D_8011CE88[best]; break;
        case 4: ptr = D_8011D618[best]; break;
        case 5: ptr = D_8011DC88[best]; break;
        case 6: ptr = D_8011E428[best]; break;
        case 7: ptr = D_8011E438[best]; break;
        case 8: ptr = D_8011E448[best]; break;
        case 9: ptr = D_8011E458[best]; break;
        case 10: ptr = D_8011E468[best]; break;
        case 11: ptr = D_8011E548[best]; break;
        case 12: ptr = D_8011E558[best]; break;
        case 13: ptr = D_8011E568[best]; break;
        case 14: ptr = D_8011E578[best]; break;
        case 15: ptr = D_8011E588[best]; break;
        case 16: ptr = D_8011E598[best]; break;
        case 17: ptr = D_8011E5A8[best]; break;
        case 18: ptr = D_8011E5B8[best]; break;
        default: break;
        }
    }
    parts = D_8011E748[D_8014978C];
    mask = 1;
    w = 0;
    for (i = 0; i < parts; i++) {
        if (ptr) {}
        if (i && !(i & 31)) {
            mask = 1;
            w++;
        }
        if (ptr_no_clip) {
            ShowCull(&D_8012E700[D_80149D90 + i]);
        }
        if (((mask & ptr[w]) || (ptr_no_clip ? mask & ptr_no_clip[w] : 0) || show) && !D_8014061A) {
            D_8012E700[D_80149D90 + i].flags &= ~0x80000000;
        } else {
            D_8012E700[D_80149D90 + i].flags |= 0x80000000;
        }
        mask <<= 1;
        if (ptr) {}
    }
}
