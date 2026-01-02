/* Types */
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef double f64;

/* Best version found - uses goto for bne instruction */
s32 __isnan(f64 x)
{
    u32 hi, exp;

    hi = ((u32 *)&x)[0];
    exp = (hi << 1) >> 21;

    if (exp != 0x7FF) goto ret_zero;

    ((u16 *)&x)[0] &= 0x800F;
    if (x == 0.0) return 1;

ret_zero:
    return 0;
}
