/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800D2C10 (0x800D2C10): index of the point nearest to pos (squared distance on s16
 * coordinates converted to float) in point set D_8012E5E8[set] ({u16 n; P3 *pts}, 8-byte P3).
 * Returns the index as s16; if the set is empty (or no point is closer than 1e20) the result
 * is the uninitialised `best`, exactly as retail (which is why `best` lives on the stack, sp+6).
 * Own .rodata literal 1e20f (0x8012417C) verified by the scorer.
 *
 * Shaping quirks (w9d), each needed:
 *  - goto loop: a for/while/do loop with an s32 counter is unrolled by -O3; an s16 counter adds
 *    sll/sra narrowing; the loop bound re-reads D_8012E5E8[set].n (CSE'd with the guard load);
 *  - no named pointer to the set record (a `PSet *s` local swaps v0/v1);
 *  - `s32 pad0[4]`, an unused 16-byte local, gives retail's 32-byte frame with best at sp+6.
 *    It is a filler, not recovered structure.
 */
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
    P3 *p;
    s32 i;
    s32 pad0[4];
    s16 best;
    f32 dx, dy, dz, d;
    f32 min;

    min = 1e20f;
    i = 0;
    if (D_8012E5E8[set].n > 0) {
        p = D_8012E5E8[set].pts;
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
        if (i < D_8012E5E8[set].n) {
            goto loop;
        }
    }
    return best;
}
