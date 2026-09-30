typedef signed int s32;
typedef unsigned int u32;

extern s32 D_8014A248;
extern void func_80086A50();

/* stand-in callers (real: func_8008705C, func_800878E0, object_render, func_8008A46C, sound_init) */
void caller_a(s32 m) { func_80086A50(m); func_80086A50(D_8014A248); }
void caller_b(s32 m) { func_80086A50(m + 1); }
