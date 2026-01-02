typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef double f64;

/* Try: Direct cast without union, force stack with volatile */
s32 __isnan(f64 x) {
    volatile f64 tmp = x;
    u32 hi = *(u32 *)&tmp;
    u32 exp = (hi << 1) >> 21;
    
    if (exp != 0x7FF) return 0;
    
    *(u16 *)&tmp &= 0x800F;
    if (tmp == 0.0) return 1;
    return 0;
}
