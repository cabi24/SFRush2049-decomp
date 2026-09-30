import subprocess,itertools,sys
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
vars_={}
vars_['v1']='''void func_8008E0B8(f32 *v) {
    f32 x = v[0]; f32 y = v[1]; volatile f32 z = v[2]; volatile f32 pad[4];
    f32 len = sqrtf(x*x + y*y + z*z);
    if (len <= D_8012394C) { len = 0.0f; } else {
        f32 inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; }
}'''
vars_['v2']='''void func_8008E0B8(f32 *v) {
    f32 x = v[0]; f32 y = v[1]; f32 z = v[2]; volatile f32 zz; f32 len; f32 inv;
    zz = z;
    len = sqrtf(x*x + y*y + zz*zz);
    if (len <= D_8012394C) { len = 0.0f; } else {
        inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = zz*inv; }
}'''
vars_['v3']='''void func_8008E0B8(f32 *v) {
    f32 x = v[0]; f32 y = v[1]; volatile f32 z = v[2]; volatile f32 pad[4];
    f32 len = sqrtf(x*x + y*y + z*z);
    if (len > D_8012394C) {
        f32 inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; }
    else len = 0.0f;
}'''
for k,s in vars_.items():
    open('t_%s.c'%k,'w').write(hdr+s)
    r=subprocess.run(['python3','tools/cloud/score.py','fn','cloud/work/r5_b/t_%s.c'%k,'func_8008E0B8','--flags','-g0 -O2 -mips2 -G 0 -non_shared'],capture_output=True,text=True,cwd='../../..')
    print(k,r.stdout.strip().splitlines()[-1] if r.stdout.strip() else r.stderr[-200:])
