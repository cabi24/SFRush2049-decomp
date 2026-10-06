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
    s32 i;
    P3 *p;
    f32 min;
    PSet *s;
    f32 dx, dy, dz, d;
    s32 pad0[3];
    s16 best;

    s = &D_8012E5E8[set];
    min = 1e20f;
    i = 0;
    if (s->n > 0) {
        p = s->pts;
    loop:
        dx = p->x - pos->x;
        dy = p->y - pos->y;
        dz = p->z - pos->z;
        d = dx * dx + dy * dy + dz * dz;
        if (d < min) {
            min = d;
            best = i;
        }
        i++;
        p++;
        if (i < s->n) {
            goto loop;
        }
    }
    return best;
}
