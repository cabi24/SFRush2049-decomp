import sys,re,subprocess,shutil,os
sys.path.insert(0,'/home/user/SFRush2049-decomp/cloud/work/r5_g')
ROOT='/home/user/SFRush2049-decomp'
GD=ROOT+'/cloud/work/r5_g/grp4'
os.makedirs(GD,exist_ok=True)
shutil.copy(ROOT+'/cloud/work/ipa-groups/func_800AD4C8/group.json',GD+'/group.json')
BASE=open(ROOT+'/cloud/work/ipa-groups/func_800AD4C8/group.c').read()
C3=open(ROOT+'/cloud/work/r5_g/c3_base.c').read()
def extract(S,name):
    return re.search(r'^s16 '+name+r'\([^;\n]*\{\n.*?\n\}\n',S,re.S|re.M).group(0)
T={'va':'f32 va[3];','vb':'f32 vb[3];','vc':'f32 vc[3];','vd':'f32 vd[3];','ve':'f32 ve[3];','idx':'u16 idx[20];','f1':'volatile f32 f1;','f2':'volatile f32 f2;','k':'u32 k;','n':'u32 n;','res':'s32 res;'}
lines=C3.split('\n')
blk_end=next(i for i,l in enumerate(lines) if l.strip()=='' and i>1)
def build(order,src=C3):
    ls=src.split('\n'); be=next(i for i,l in enumerate(ls) if l.strip()=='' and i>1)
    return '\n'.join([ls[0]]+['    '+(T[x] if x in T else x) for x in order]+ls[be:])
def evalsrc(body):
    S=BASE.replace(extract(BASE,'func_800C3AD0'),body)
    open(GD+'/group.c','w').write(S)
    r=subprocess.run(['python3',ROOT+'/cloud/work/r5_g/gsbs.py',GD,'func_800C3AD0','--as1=-r4300_mul','--hi','9999'],capture_output=True,text=True).stdout
    m=re.search(r'aligned differing rows: (\d+)',r)
    ours=' '.join(l.split('|',1)[1] for l in r.splitlines() if '|' in l)
    return (int(m.group(1)) if m else None), sorted(set(int(x) for x in re.findall(r'(\d+)\(sp\)',ours) if int(x)<100))[:8], re.findall(r'addiu a2,sp,(\d+)',ours)[:1]
