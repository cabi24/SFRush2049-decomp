import subprocess,re,itertools
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
pad='s32 q1; s32 q2; s32 q3; s32 q4; s32 q5; s32 q6;'
decls=['f32 x; f32 y; f32 z; f32 *pz = &z; f32 len; f32 inv;','f32 x; f32 y; f32 z; f32 len; f32 inv; f32 *pz = &z;','f32 *pz; f32 z; f32 x; f32 y; f32 len; f32 inv;','f32 len; f32 inv; f32 x; f32 y; f32 z; f32 *pz = &z;','f32 inv; f32 len; f32 z; f32 *pz = &z; f32 y; f32 x;','f32 z; f32 *pz = &z; f32 x; f32 y; f32 len; f32 inv;','f32 z; f32 *pz = &z; f32 y; f32 x; f32 inv; f32 len;']
stores=['v[0] = x*inv; v[1] = y*inv; v[2] = z*inv;','v[0] = inv*x; v[1] = inv*y; v[2] = inv*z;','v[0] *= inv; v[1] *= inv; v[2] *= inv;']
sums=['x*x + y*y + z*z','z*z + x*x + y*y','x*x + (y*y + z*z)']
pzu=['','*pz = z;','z = *pz;']
loads=['y = v[1]; x = v[0]; z = v[2];','x = v[0]; y = v[1]; z = v[2];','z = v[2]; y = v[1]; x = v[0];','x = v[0]; z = v[2]; y = v[1];']
res=[]
for d,s,sm,l,pu in itertools.product(decls,stores,sums,loads,pzu):
    if pu=='z = *pz;' : continue
    b=hdr+'f32 func_8008E0B8(f32 *v) {\n%s %s\n%s %s\nlen = sqrtf(%s);\nif (len <= D_8012394C) { return 0.0f; } inv = 1.0f / len; %s return len;\n}'%(pad,d,l,pu,sm,s)
    open('t_y.c','w').write(b)
    r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_y.c','func_8008E0B8','--flags','-g0 -O2 -mips2 -G 0 -non_shared'],capture_output=True,text=True,cwd='../../..')
    tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
    m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
    if m_:
        res.append((int(m_.group(2)),int(m_.group(3)),b))
res.sort(key=lambda x:(x[0],x[1]),reverse=True)
print(res[0][:2]); open('best_0B8.c','w').write(res[0][2])
print(len(res))
