import subprocess,re,itertools
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
names=['x','y','z','len','inv','pz']
decl={'x':'f32 x;','y':'f32 y;','z':'f32 z;','len':'f32 len;','inv':'f32 inv;','pz':'f32 *pz;'}
res=[]
for perm in itertools.permutations(names):
  for loadorder in [('x','y','z'),('y','x','z')]:
    ds=' '.join(decl[n] for n in perm)
    loads=' '.join('%s = v[%d];'%(n,'xyz'.index(n)) for n in loadorder)
    b=hdr+'''f32 func_8008E0B8(f32 *v) {
    %s
    %s pz = &z;
    len = sqrtf(x*x + y*y + z*z);
    if (len <= D_8012394C) { return 0.0f; }
    inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv;
    return len;
}'''%(ds,loads)
    open('t_l.c','w').write(b)
    r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_l.c','func_8008E0B8','--flags','-g0 -O2 -mips2 -G 0 -non_shared'],capture_output=True,text=True,cwd='../../..')
    tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
    m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
    if m_: res.append((int(m_.group(2)),int(m_.group(3)),m_.group(1),perm,loadorder))
res.sort(reverse=True)
for x in res[:6]: print(x)
