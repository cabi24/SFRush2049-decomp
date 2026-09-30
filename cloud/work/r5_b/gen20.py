import subprocess,re
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
res=[]
for n in range(0,10):
  for arr in (0,1):
    pre=''
    if n:
        pre = ('f32 pre[%d]; f32 *pp = pre;'%n) if arr else ' '.join('f32 q%d; f32 *pq%d = &q%d;'%(i,i,i) for i in range(n))
    b=hdr+'''f32 func_8008E0B8(f32 *v) {
    f32 len; f32 inv; %s
    f32 z; f32 *pz = &z;
    f32 x = v[0]; f32 y = v[1]; z = v[2];
    len = sqrtf(x*x + y*y + z*z);
    if (len <= D_8012394C) { return 0.0f; }
    inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv;
    return len;
}'''%pre
    open('t_o.c','w').write(b)
    r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_o.c','func_8008E0B8','--flags','-g0 -O2 -mips2 -G 0 -non_shared'],capture_output=True,text=True,cwd='../../..')
    tt=r.stdout.strip().splitlines()[-1]
    m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
    print(n,arr,m_.groups() if m_ else tt)
