import subprocess,re,itertools
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
res=[]
pad='s32 q1; s32 q2; s32 q3; s32 q4; s32 q5; s32 q6;'
decl_orders=[['x','y','z','len','inv','sq'],['len','inv','x','y','z','sq'],['x','y','len','inv','z','sq'],['sq','len','inv','x','y','z'],['z','x','y','len','inv','sq'],['inv','len','z','y','x','sq'],['x','y','z','sq','len','inv']]
sqforms={'s1':'len = sqrtf(x*x + y*y + z*z);','s2':'sq = x*x + y*y + z*z; len = sqrtf(sq);','s3':'sq = x*x; sq += y*y; sq += z*z; len = sqrtf(sq);','s4':'len = sqrtf(x*x + y*y + z*z);'}
loadorders=[('x','y','z'),('y','x','z'),('z','y','x'),('y','z','x'),('x','z','y')]
for do,(sk,sf),lo,ret in itertools.product(decl_orders,sqforms.items(),loadorders,['a','b']):
    dd=[]
    for n in do:
        if n=='z': dd.append('f32 z; f32 *pz = &z;')
        elif n=='len': dd.append('f32 len;')
        else: dd.append('f32 %s;'%n)
    dd.insert(0,pad)
    loads=' '.join('%s = v[%d];'%(n,'xyz'.index(n)) for n in lo)
    if 'sq' not in sf and 'sq' in do: pass
    tail={'a':'if (len <= D_8012394C) { return 0.0f; } inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; return len;',
          'b':'if (len > D_8012394C) { inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; return len; } return 0.0f;'}[ret]
    b=hdr+'f32 func_8008E0B8(f32 *v) {\n%s\n%s\n%s\n%s\n}'%(' '.join(dd),loads,sf,tail)
    open('t_u.c','w').write(b)
    r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_u.c','func_8008E0B8','--flags','-g0 -O2 -mips2 -G 0 -non_shared'],capture_output=True,text=True,cwd='../../..')
    tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
    m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
    if m_: res.append((int(m_.group(2)),int(m_.group(3)),do,sk,lo,ret))
res.sort(key=lambda x:(x[0],x[1]),reverse=True)
for x in res[:6]: print(x)
