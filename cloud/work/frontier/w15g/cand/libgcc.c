typedef long long s64; typedef unsigned long long u64; typedef unsigned int u32; typedef unsigned short u16;
typedef double f64; typedef float f32;
#ifdef T___muldi3
s64 __muldi3(s64 a, s64 b){ return a*b; }
#endif
#ifdef T___lshrdi3
u64 __lshrdi3(u64 a, u32 s){ return a>>s; }
#endif
#ifdef T___ashldi3
s64 __ashldi3(s64 a, u32 s){ return a<<s; }
#endif
#ifdef T___ashrdi3
s64 __ashrdi3(s64 a, u32 s){ return a>>s; }
#endif
#ifdef T___divdi3
s64 __divdi3(s64 a, s64 b){ return a/b; }
#endif
#ifdef T___moddi3
s64 __moddi3(s64 a, s64 b){ return a%b; }
#endif
#ifdef T___udivdi3
u64 __udivdi3(u64 a, u64 b){ return a/b; }
#endif
#ifdef T___umoddi3
u64 __umoddi3(u64 a, u64 b){ return a%b; }
#endif
#ifdef T___fixdfdi
s64 __fixdfdi(f64 a){ return (s64)a; }
#endif
#ifdef T___fixsfdi
s64 __fixsfdi(f32 a){ return (s64)a; }
#endif
#ifdef T___floatdidf
f64 __floatdidf(s64 a){ return (f64)a; }
#endif
#ifdef T___floatdisf
f32 __floatdisf(s64 a){ return (f32)a; }
#endif
#ifdef T___fixunsdfdi
u64 __fixunsdfdi(f64 a){ return (u64)a; }
#endif
#ifdef T___fixunssfdi
u64 __fixunssfdi(f32 a){ return (u64)a; }
#endif
#ifdef T___floatundidf
f64 __floatundidf(u64 a){ return (f64)a; }
#endif
#ifdef T___floatundisf
f32 __floatundisf(u64 a){ return (f32)a; }
#endif
