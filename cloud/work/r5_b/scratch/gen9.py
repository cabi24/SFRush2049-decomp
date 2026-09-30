import subprocess,itertools,re
exec(open('gen8.py').read().split('res=[]')[0])
decls=['s32 i; s16 idx; Ent *e;','s16 idx; s32 i; Ent *e;','Ent *e; s32 i; s16 idx;','s32 i; Ent *e; s16 idx;','s32 i; Ent *e;','Ent *e; s16 idx; s32 i;','s16 idx; Ent *e; s32 i;']
ptypes=[('s32','s32','s16','s32'),('s16','s32','s16','s32'),('u16','s32','s16','s32'),('s32','s32','s16','u32'),('s32','s32','s32','s32')]
tails={'T1':'idx = i; render_mode_select(idx, c); return idx;','T2':'render_mode_select(i, c); return (s16)i;','T3':'render_mode_select(i, c); return i;'}
res=[]
for dk,d in enumerate(decls):
 for pt in ptypes:
  for tk,t in tails.items():
   for F in ['-O2','-O3']:
    if 'idx' not in d and tk=='T1': continue
    s=hdr+'''s32 func_8008E26C(%s a, %s b, %s c, %s d) {
    %s
    %s
    %s
    %s
    %s
}'''%(pt+(d,loops['L1'],mids['M1'],stores['S1'],t))
    open('t_c.c','w').write(s)
    r=subprocess.run(['python3','cloud/work/bigfish/near.py','cloud/work/r5_b/t_c.c','func_8008E26C','--flags','-g0 %s -mips2 -G 0 -non_shared'%F],capture_output=True,text=True,cwd='../../..')
    tt=r.stdout.strip().splitlines()[-1] if r.stdout.strip() else 'ERR'
    m_=re.search(r'got (\d+).*strict-equal (\d+); aligned exact (\d+)',tt)
    if m_: res.append((int(m_.group(3)),int(m_.group(2)),m_.group(1),dk,pt,tk,F))
res.sort(reverse=True)
for x in res[:12]: print(x)
