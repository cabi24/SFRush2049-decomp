/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800F6928 @ 0x800F6928, 400 bytes: piecewise-linear table lookup, then publish the result.
 * D_80114750 is a table of (x, y) float pairs terminated by an x >= 32000.0f sentinel. Find the
 * first entry whose x is not below `value` (or the sentinel); when it is not the first entry,
 * linearly interpolate y between the previous and that entry. The tail is the same code as the
 * locked render_helper (store D_80114744; when positive set bit 0x10 of D_80149B88 and pass the
 * value, converted f32 -> u32 -> u16, to func_8008A644, otherwise clear the bit). No arcade
 * ancestor identified.
 *
 * Strict MATCH at -O3 and at -O2 (single file), EQUAL in the whole-program unit (blob_unit).
 * No own rodata (32000.0f is an lui immediate, 0.0f is mtc1 zero).
 *
 * Shaping facts (each needed):
 * - the three differences are named locals `a, b, d` assigned in the order d, b, a: this puts them
 *   in $f18/$f16/$f14 as in retail (written inline they become ugen temps in other registers);
 * - the final step is two statements, `value = a * b / d; value += y0;`: one expression gives
 *   `add.s y0, q` where retail has `add.s q, y0`;
 * - the plain `while` loop with `i = 0` reproduces retail's unfolded `sll t6,zero,3` address.
 */
typedef float f32;
typedef int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef struct { f32 x, y; } Point;
extern Point D_80114750[];
extern f32 D_80114744;
extern u32 D_80149B88;
extern void func_8008A644(u16);

void func_800F6928(f32 value) {
    s32 i;
    f32 a, b, d;

    i = 0;
    while (value > D_80114750[i].x && D_80114750[i].x < 32000.0f) {
        i++;
    }
    if (i > 0) {
        d = D_80114750[i].x - D_80114750[i - 1].x;
        b = D_80114750[i].y - D_80114750[i - 1].y;
        a = value - D_80114750[i - 1].x;
        value = a * b / d;
        value += D_80114750[i - 1].y;
    }
    D_80114744 = value;
    if (value > 0.0f) {
        D_80149B88 |= 0x10;
        func_8008A644(value);
    } else {
        D_80149B88 &= ~0x10;
    }
}
