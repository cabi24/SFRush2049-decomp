/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef float f32;

typedef struct {
    s16 x, y, z, pad;
} P3;

typedef struct {
    u16 n;
    P3 *pts;
} PSet;

extern PSet D_8012E5E8[];

s16 func_800D2C10(P3 *pos, s16 set)
{
    PSet *s;
    P3 *p;
    s32 i;
    s16 best;
    f32 dx, dy, dz, d;
    f32 min;
    s32 n;
    s = &D_8012E5E8[set];
    n = s->n;
    min = 1e20f;
    for (i = 0, p = s->pts; i < n; i++, p++) {
        dx = p->x - pos->x;
        dy = p->y - pos->y;
        dz = p->z - pos->z;
        d = dx * dx + dy * dy + dz * dz;
        if (d < min) {
            min = d;
            best = i;
        }
    }
    return best;
}
