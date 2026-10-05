typedef float f32;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef signed char s8;
typedef unsigned char u8;

typedef struct {
    char pad0[0x34];
    f32 pos[3];     /* 0x34 */
    char pad40[4];
    s16 next;       /* 0x44 */
    char pad46[0xA];
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
} Zone; /* 0x68 */

typedef struct {
    u32 flags;
    char pad4[0x40];
} Obj; /* 0x44 */

typedef struct {
    u32 bits[4];
} VisMask;

extern Zone *D_80149B80;
extern s8 D_80149B8C;
extern s16 D_80149D90;
extern s8 D_8014061A;
extern Obj D_8012E700[];
extern u8 D_8011E748[];
extern VisMask D_8011B898[], D_8011BFE8[], D_8011C738[], D_8011CE88[], D_8011D618[], D_8011DC88[];
extern VisMask D_8011E428[], D_8011E438[], D_8011E448[], D_8011E458[], D_8011E468[], D_8011E548[];
extern VisMask D_8011E558[], D_8011E568[], D_8011E578[], D_8011E588[], D_8011E598[], D_8011E5A8[], D_8011E5B8[];
extern u32 D_8011E75C[];

extern void foo();
extern f32 sqrtf(f32);
#pragma intrinsic(sqrtf)


static void func_8009EBB8(f32 *pos, s32 *best, f32 *bestDist, s32 *index, s32 *count) {
    f32 d[3];
    Zone *z;
    f32 dist;

    z = D_80149B80;
    for (;;) {
        d[0] = pos[0] - z->pos[0];
        d[1] = pos[1] - z->pos[1];
        d[2] = pos[2] - z->pos[2];
        if (z->unk50 <= d[0] && d[0] <= z->unk5C && z->unk54 <= d[1] && z->unk58 <= d[2] && d[2] <= z->unk64) {
            dist = sqrtf(d[0] * d[0] + d[2] * d[2]);
            if (z->unk60 * 0.75f < d[1]) {
                dist += d[1] - z->unk54;
            }
            if (*best < 0 || dist < *bestDist) {
                *best = *count;
                *bestDist = dist;
                *index = z - D_80149B80;
            }
        }
        if (z->next < 0) {
            break;
        }
        (*count)++;
        z = &D_80149B80[z->next];
    }
}
void physics_float_calc(s32 arg0, f32 *pos, s32 arg2) {
    s32 count;
    s32 index;
    f32 bestDist;
    s32 best;
    s32 pad[4];
    u32 *mask;
    s32 i;
    s32 n;
    u32 bit;
    Obj *obj;
    s32 dbg;
    s32 w;
    u32 *p;

    dbg = 0;
    count = 0;
    index = 0;
    best = -1;
    if (D_80149B80 == 0) {
        return;
    }
    func_8009EBB8(pos, &best, &bestDist, &index, &count);
    if (best < 0) {
        mask = D_8011E75C;
    } else {
        switch (D_80149B8C) {
        case 0: mask = D_8011B898[best].bits; break;
        case 1: mask = D_8011BFE8[best].bits; break;
        case 2: mask = D_8011C738[best].bits; break;
        case 3: mask = D_8011CE88[best].bits; break;
        case 4: mask = D_8011D618[best].bits; break;
        case 5: mask = D_8011DC88[best].bits; break;
        case 6: mask = D_8011E428[best].bits; break;
        case 7: mask = D_8011E438[best].bits; break;
        case 8: mask = D_8011E448[best].bits; break;
        case 9: mask = D_8011E458[best].bits; break;
        case 10: mask = D_8011E468[best].bits; break;
        case 11: mask = D_8011E548[best].bits; break;
        case 12: mask = D_8011E558[best].bits; break;
        case 13: mask = D_8011E568[best].bits; break;
        case 14: mask = D_8011E578[best].bits; break;
        case 15: mask = D_8011E588[best].bits; break;
        case 16: mask = D_8011E598[best].bits; break;
        case 17: mask = D_8011E5A8[best].bits; break;
        case 18: mask = D_8011E5B8[best].bits; break;
        }
    }
    n = D_8011E748[D_80149B8C];
    bit = 1;
    w = 0;
    for (i = 0; i < n; i++) {
        if (dbg) {
            foo(D_8012E700[D_80149D90 + i].flags);
        }
        if (i != 0 && (i & 0x1F) == 0) {
            w++;
            bit = 1;
        }
        if (((mask[w] & bit) || arg2 != 0) && D_8014061A == 0) {
            D_8012E700[D_80149D90 + i].flags &= 0x7FFFFFFF;
        } else {
            D_8012E700[D_80149D90 + i].flags |= 0x80000000;
        }
        bit += bit;
    }
}
