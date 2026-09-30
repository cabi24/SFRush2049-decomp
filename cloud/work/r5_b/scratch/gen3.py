import subprocess
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
bodies={
'a':'''f32 func_8008E0B8(f32 *v) {
    f32 len = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    f32 inv;
    if (len <= D_8012394C) { len = 0.0f; } else {
        inv = 1.0f / len; v[0] *= inv; v[1] *= inv; v[2] *= inv; }
    return len;
}''',
'b':'''f32 func_8008E0B8(f32 *v) {
    f32 x = v[0]; f32 y = v[1]; f32 z = v[2];
    f32 len = sqrtf(x*x + y*y + z*z);
    f32 inv;
    if (len <= D_8012394C) { return 0.0f; }
    inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv;
    return len;
}''',
'c':'''f32 func_8008E0B8(f32 *v) {
    f32 len = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    f32 inv;
    if (len > D_8012394C) { 
        inv = 1.0f / len; v[0] *= inv; v[1] *= inv; v[2] *= inv; } else { len = 0.0f; }
    return len;
}''',
}
for flags in ['-O2','-O1','-O3']:
  for k,s in bodies.items():
    open('t_%s.c'%k,'w').write(hdr+s)
    r=subprocess.run(['python3','tools/cloud/score.py','fn','cloud/work/r5_b/t_%s.c'%k,'func_8008E0B8','--flags','-g0 %s -mips2 -G 0 -non_shared'%flags],capture_output=True,text=True,cwd='../../..')
    print(flags,k,r.stdout.strip().splitlines()[-1] if r.stdout.strip() else r.stderr[-200:])
