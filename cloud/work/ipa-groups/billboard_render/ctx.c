typedef signed int s32;
/* Context stand-in for func_800F1210 (454 words retail: state change, IPA param s2,
 * uses s0-s8 and $f20/$f22). Written so that the callee pushes the caller to save
 * all of s0-s8 and two float registers; not a match of the retail body. */
extern s32 D_80140B20, D_80140B24, D_80140B28, D_80140B2C, D_80140B30, D_80140B34, D_80140B38, D_80140B3C, D_80140B40, D_80140B44;
extern float F_80140B50, F_80140B54, F_80140B58;
extern void ext_a(s32, s32, s32, s32);
extern void ext_f(float, float);
void func_800F1210(s32 st)
{
    s32 a = D_80140B24, b = D_80140B28, c = D_80140B2C, d = D_80140B30, e = D_80140B34, f = D_80140B38, g = D_80140B3C, h = D_80140B40, i = D_80140B44;
    float x = F_80140B50, y = F_80140B54;
    if (st == D_80140B20) return;
    ext_a(a, b, c, d);
    ext_a(e, f, g, st);
    ext_f(x, y);
    D_80140B20 = st + a + b + c + d + e + f + g + h + i;
    ext_a(a, b, c, d);
    ext_a(e, f, g, h + i);
    ext_f(x * y, F_80140B58 + x);
}
