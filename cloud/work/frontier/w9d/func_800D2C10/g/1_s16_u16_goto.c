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
    s16 i;
    s16 best;
    f32 v[3];
    f32 d;
    f32 min;
    u16 n;

    s = &D_8012E5E8[set];
    n = s->n;
    min = 1e20f;
    i = 0;
    if (n > 0) {
        p = s->pts;
    loop:
        v[0] = p->x - pos->x;
        v[1] = p->y - pos->y;
        v[2] = p->z - pos->z;
        d = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
        if (d < min) {
            min = d;
            best = i;
        }
        i++;
        p++;
        if (i < n) {
            goto loop;
        }
    }
    return best;
}
