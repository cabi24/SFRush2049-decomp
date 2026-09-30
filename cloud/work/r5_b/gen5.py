import subprocess,itertools
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
tails={
't1':'''    if (len <= D_8012394C) { r = 0.0f; } else { r = len;
        inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; }
    return r;''',
't2':'''    if (len <= D_8012394C) { return 0.0f; }
    inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv;
    return len;''',
't3':'''    if (len > D_8012394C) { r = len;
        inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; } else r = 0.0f;
    return r;''',
}
pads=['','f32 pa[6];','volatile f32 pa[6];','f32 p1,p2,p3,p4,p5,p6;']
for (tk,t),pd in [(("t2",tails["t2"]),"")]:
    s='''f32 func_8008E0B8(f32 *v) {
    f32 x = v[0]; f32 y = v[1]; f32 z = v[2];
    f32 len; f32 inv; f32 r; f32 *pz = &z; %s
    len = sqrtf(x*x + y*y + z*z);
%s
}'''%(pd,t)
    open('t_a.c','w').write(hdr+s)
    r=subprocess.run(['python3','tools/cloud/score.py','fn','cloud/work/r5_b/t_a.c','func_8008E0B8','--flags','-g0 -O2 -mips2 -G 0 -non_shared'],capture_output=True,text=True,cwd='../../..')
    print(tk,pd,r.stdout.strip().splitlines()[-1] if r.stdout.strip() else r.stderr[-200:])
