typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
f32 func_8008E0B8(f32 *v) {
s32 q1; s32 q2; s32 q3; s32 q4; s32 q5; s32 q6; f32 x; f32 y; f32 z; f32 *pz = &z; f32 sq; f32 len; f32 inv;
x = v[0]; z = v[2]; y = v[1];
len = sqrtf(x*x + y*y + z*z);
if (len > D_8012394C) { inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; return len; } return 0.0f;
}