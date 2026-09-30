import subprocess,itertools
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
decls={
'd1':'f32 a[8]; f32 z = v[2]; f32 *pa = a; f32 *pz = &z; f32 x = v[0]; f32 y = v[1];',
'd2':'f32 a[8]; f32 x = v[0]; f32 y = v[1]; f32 z = v[2]; f32 *pz = &z;',
'd3':'f32 x = v[0]; f32 y = v[1]; f32 z = v[2]; f32 *pz = &z;',
'd4':'f32 a; f32 b; f32 c; f32 d; f32 e; f32 f; f32 g; f32 h; f32 z = v[2]; f32 x = v[0]; f32 y = v[1];  f32 *pa = &a; f32 *pz = &z; f32 *pb = &b;f32 *pc = &c;f32 *pd = &d;f32 *pe = &e;f32 *pf = &f;f32 *pg = &g;f32 *ph = &h;',
}
for dk,d in decls.items():
    s='''f32 func_8008E0B8(f32 *v) {
    f32 len; f32 inv;
    %s
    len = sqrtf(x*x + y*y + z*z);
    if (len <= D_8012394C) { return 0.0f; }
    inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv;
    return len;
}'''%(d)
    open('t_a.c','w').write(hdr+s)
    r=subprocess.run(['python3','tools/cloud/score.py','fn','cloud/work/r5_b/t_a.c','func_8008E0B8','--flags','-g0 -O2 -mips2 -G 0 -non_shared'],capture_output=True,text=True,cwd='../../..')
    print(dk,r.stdout.strip().splitlines()[-1] if r.stdout.strip() else r.stderr[-200:])
