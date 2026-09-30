import subprocess
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
void nop_fn(f32*);
'''
bodies={
'a':'''f32 func_8008E0B8(f32 *v) {
    f32 x = v[0]; f32 y = v[1]; f32 z = v[2];
    f32 len; f32 inv; f32 *pz = &z;
    len = sqrtf(x*x + y*y + z*z);
    if (len <= D_8012394C) { len = 0.0f; } else {
        inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; }
    return len;
}''',
'b':'''f32 func_8008E0B8(f32 *v) {
    f32 x = v[0]; f32 y = v[1]; f32 z = v[2];
    f32 len; f32 inv; f32 a[6];
    len = sqrtf(x*x + y*y + z*z);
    if (len <= D_8012394C) { len = 0.0f; } else {
        inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; }
    return len;
}''',
'c':'''typedef struct {f32 x,y,z;} V;
f32 func_8008E0B8(V *v) {
    f32 len; f32 inv; f32 z; volatile f32 a[6];
    z = v->z;
    len = sqrtf(v->x*v->x + v->y*v->y + z*z);
    if (len <= D_8012394C) { len = 0.0f; } else {
        inv = 1.0f / len; v->x = v->x*inv; v->y = v->y*inv; v->z = z*inv; }
    return len;
}''',
}
for flags in ['-O2']:
  for k,s in bodies.items():
    open('t_%s.c'%k,'w').write(hdr+s)
    r=subprocess.run(['python3','tools/cloud/score.py','fn','cloud/work/r5_b/t_%s.c'%k,'func_8008E0B8','--flags','-g0 %s -mips2 -G 0 -non_shared'%flags],capture_output=True,text=True,cwd='../../..')
    print(flags,k,r.stdout.strip().splitlines()[-1] if r.stdout.strip() else r.stderr[-200:])
