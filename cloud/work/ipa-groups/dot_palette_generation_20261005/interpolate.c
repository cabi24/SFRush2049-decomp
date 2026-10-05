/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* RGB5551 interpolation. Complete genuine palette caller is in palette.c.
 * Internal O3 context restricts temporary allocation to the native t6-t9 ring.
 * At ratio zero / >=255, return the entire corresponding endpoint unchanged.
 * In-between (including negative ratios), multiply/add wrap modulo 2^32,
 * shift as signed, retain the three channel masks, and force alpha to one.
 * Unsigned inverse subtraction also defines INT_MIN without signed overflow.
 */
typedef int s32;
typedef unsigned int u32;

u32 func_800B0EA0(s32 ratio, u32 left, u32 right)
{
    u32 inverse, red, green, blue;
    if (ratio == 0) return left;
    if (ratio >= 255) return right;
    inverse = 255U - (u32)ratio;
    red = ((s32)((left & 0xF800) * inverse + (right & 0xF800) * (u32)ratio) >> 8) & 0xF800;
    green = ((s32)((left & 0x7C0) * inverse + (right & 0x7C0) * (u32)ratio) >> 8) & 0x7C0;
    blue = ((s32)((left & 0x3E) * inverse + (right & 0x3E) * (u32)ratio) >> 8) & 0x3E;
    return red | green | blue | 1;
}
