typedef long long s64; typedef unsigned long long u64; typedef unsigned int u32; typedef short s16;
#ifdef T___lshrdi3
u64 __lshrdi3(u64 a, s64 s){ return a>>s; }
#endif
#ifdef T___ashldi3
s64 __ashldi3(s64 a, s64 s){ return a<<s; }
#endif
#ifdef T___ashrdi3
s64 __ashrdi3(s64 a, s64 s){ return a>>s; }
#endif
#ifdef T___moddi3
s64 __moddi3(s64 a, s64 b){ s64 r = a % b; if ((r < 0 && b > 0) || (r > 0 && b < 0)) r += b; return r; }
#endif
#ifdef T___umoddi3_alt
u64 __umoddi3_alt(u64 a, u64 b){ return a%b; }
#endif
#ifdef T___qdivrem
void __qdivrem(u64 *q, u64 *r, u64 a, s16 b){ *q = a/b; *r = a%b; }
#endif
#ifdef T___fixunsdfdi
typedef double f64;
u64 __fixunsdfdi(f64 a){ return (u64)a; }
#endif
