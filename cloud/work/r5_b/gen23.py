import subprocess,re,itertools
hdr='''typedef signed int s32; typedef float f32;
float sqrtf(float);
#pragma intrinsic (sqrtf)
extern f32 D_8012394C;
'''
pad='s32 q1; s32 q2; s32 q3; s32 q4; s32 q5; s32 q6;'
tails={
'r1':'''if (len <= D_8012394C) { r = 0.0f; } else { r = len; inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; } return r;''',
'r2':'''if (len > D_8012394C) { r = len; inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; } else { r = 0.0f; } return r;''',
'r3':'''r = len; if (len <= D_8012394C) { r = 0.0f; } else { inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; } return r;''',
'r4':'''r = 0.0f; if (len > D_8012394C) { r = len; inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; } return r;''',
'r5':'''if (len <= D_8012394C) { r = 0.0f; goto out; } inv = 1.0f / len; v[0] = x*inv; v[1] = y*inv; v[2] = z*inv; r = len; out: return r;''',
}
res=[]
for tk,t in tails.items():
  for rd in ['f32 r;','']:
    for lo in [('x','y','z'),('y','x','z')]:
      loads=' '.join('%s = v[%d];'%(n,'xyz'.index(n)) for n in lo)
      b=hdr+'f32 func_8008E0B8(f32 *v) {\n%s f32 x; f32 y; f32 z; f32 *pz = &z; f32 len; f32 inv; %s\n%s\nlen = sqrtf(x*x + y*y + z*z);\n%s\n}'%(pad,rd or 'f32 r;',loads,t)
      open('t_w.c','w').write(b)
      r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_w.c','func_8008E0B8','--flags','-g0 -O2 -mips2 -G 0 -non_shared'],capture_output=True,text=True,cwd='../../..')
      tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
      m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
      if m_: res.append((int(m_.group(2)),int(m_.group(3)),m_.group(1),tk,lo))
      else: print(tk,tt[:100])
res.sort(key=lambda x:(x[0],x[1]),reverse=True)
for x in res[:6]: print(x)
