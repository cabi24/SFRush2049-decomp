import subprocess,re,itertools
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
res=[]
pads=['s32 q1; s32 q2; s32 q3; s32 q4; s32 q5; s32 q6;','f32 q1; f32 q2; f32 q3; f32 q4; f32 q5; f32 q6;']
for pk,pad in enumerate(pads):
 for pos in range(0,5):
  base=['f32 len;','f32 inv;','f32 z;','f32 *pz = &z;']
  # insert pad at pos
  ds=base[:pos]+[pad]+base[pos:]
  for ret in ['a','b']:
    tail={'a':'if (len <= D_8012394C) { return 0.0f; } inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; return len;',
          'b':'if (len <= D_8012394C) { len = 0.0f; } else { inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; } return len;'}[ret]
    b=hdr+'''f32 func_8008E0B8(f32 *v) {
    %s
    f32 x = v[0]; f32 y = v[1]; z = v[2];
    len = sqrtf(x*x + y*y + z*z);
    %s
}'''%(' '.join(ds),tail)
    open('t_s.c','w').write(b)
    r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_s.c','func_8008E0B8','--flags','-g0 -O2 -mips2 -G 0 -non_shared'],capture_output=True,text=True,cwd='../../..')
    tt=r.stdout.strip().splitlines()[-1]
    m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
    res.append((int(m_.group(2)),int(m_.group(3)),pk,pos,ret))
res.sort(reverse=True)
print(res[:8])
