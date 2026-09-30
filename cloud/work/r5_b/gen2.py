import subprocess
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
bodies={
'a':'''void func_8008E0B8(f32 *v) {
    f32 len; f32 inv; f32 z; f32 p1,p2,p3,p4,p5;
    f32 x = v[0]; f32 y = v[1]; z = v[2];
    len = sqrtf(x*x + y*y + z*z);
    if (len <= D_8012394C) { len = 0.0f; } else {
        inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; }
}''',
'b':'''typedef struct {f32 x,y,z;} V;
void func_8008E0B8(V *v) {
    f32 len; f32 inv; V t; 
    t = *v;
    len = sqrtf(t.x*t.x + t.y*t.y + t.z*t.z);
    if (len <= D_8012394C) { len = 0.0f; } else {
        inv = 1.0f / len; v->x = t.x*inv; v->y = t.y*inv; v->z = t.z*inv; }
}''',
}
for flags in ['-O2','-O1','-O3']:
  for k,s in bodies.items():
    open('t_%s.c'%k,'w').write(hdr+s)
    r=subprocess.run(['python3','tools/cloud/score.py','fn','cloud/work/r5_b/t_%s.c'%k,'func_8008E0B8','--flags','-g0 %s -mips2 -G 0 -non_shared'%flags],capture_output=True,text=True,cwd='../../..')
    print(flags,k,r.stdout.strip().splitlines()[-1] if r.stdout.strip() else r.stderr[-200:])
