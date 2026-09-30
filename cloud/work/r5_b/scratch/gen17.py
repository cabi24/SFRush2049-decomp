import subprocess,re
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
res=[]
for n in range(1,12):
  for k in range(0,n):
    b=hdr+'''f32 func_8008E0B8(f32 *v) {
    f32 loc[%d]; f32 *p = loc;
    f32 x = v[0]; f32 y = v[1]; f32 len; f32 inv;
    loc[%d] = v[2];
    len = sqrtf(x*x + y*y + loc[%d]*loc[%d]);
    if (len <= D_8012394C) { return 0.0f; }
    inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = loc[%d]*inv;
    return len;
}'''%(n,k,k,k,k)
    open('t_k.c','w').write(b)
    r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_k.c','func_8008E0B8','--flags','-g0 -O2 -mips2 -G 0 -non_shared'],capture_output=True,text=True,cwd='../../..')
    tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
    m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
    if m_: res.append((int(m_.group(2)),int(m_.group(3)),m_.group(1),n,k))
res.sort(reverse=True)
for x in res[:6]: print(x)
