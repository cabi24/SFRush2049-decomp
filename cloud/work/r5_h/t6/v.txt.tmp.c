typedef float f32; typedef double f64;
extern f32 D_801238E4;
extern f32 D_801238E8;
extern f32 D_801238EC;
extern f32 D_801238F0;
extern f32 func_8008C5E0(f32 x);

f32 func_8008C680(f32 arg0)
{

 if (arg0 >= D_801238E4) {
 if (D_801238E8 >= arg0) { return func_8008C5E0((arg0 - 1.0) / (arg0 + 1.0)) + D_801238F0; }
 return D_801238EC - func_8008C5E0(1.0f / arg0);
}
 return func_8008C5E0(arg0);
}