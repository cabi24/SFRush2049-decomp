typedef float f32; typedef double f64;
extern f32 D_801238E4;
extern f32 D_801238E8;
extern f32 D_801238EC;
extern f32 D_801238F0;
extern f32 func_8008C5E0(f32 x);
f32 func_8008C680(f32 arg0)
{
  f32 r, t;
  /*@3: if (arg0 < D_801238E4) { return func_8008C5E0(arg0); } if (D_801238E8 < arg0) { return D_801238EC - func_8008C5E0(1.0f / arg0); } return func_8008C5E0((arg0 - 1.0f) / (arg0 + 1.0f)) + D_801238F0; || if (arg0 < D_801238E4) r = func_8008C5E0(arg0); else if (D_801238E8 < arg0) r = D_801238EC - func_8008C5E0(1.0f / arg0); else r = func_8008C5E0((arg0 - 1.0f) / (arg0 + 1.0f)) + D_801238F0; return r; || if (arg0 < D_801238E4) return func_8008C5E0(arg0); else if (D_801238E8 < arg0) return D_801238EC - func_8008C5E0(1.0f / arg0); else return D_801238F0 + func_8008C5E0((arg0 - 1.0f) / (arg0 + 1.0f)); || if (arg0 < D_801238E4) return func_8008C5E0(arg0); if (arg0 > D_801238E8) return D_801238EC - func_8008C5E0(1.0f / arg0); return func_8008C5E0((arg0 - 1.0f) / (arg0 + 1.0f)) + D_801238F0; || if (arg0 < D_801238E4) return func_8008C5E0(arg0); if (D_801238E8 < arg0) return D_801238EC - func_8008C5E0(1 / arg0); t = arg0 + 1.0f; return func_8008C5E0((arg0 - 1.0f) / t) + D_801238F0; || if (arg0 < D_801238E4) return func_8008C5E0(arg0); if (D_801238E8 < arg0) return D_801238EC - func_8008C5E0(1.0f / arg0); t = (arg0 - 1.0f); return func_8008C5E0(t / (arg0 + 1.0f)) + D_801238F0; */
}
