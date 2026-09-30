import sys,re,json,random,subprocess,os,shutil
ROOT='/home/user/SFRush2049-decomp'
which=sys.argv[1]; seed=int(sys.argv[2]); iters=int(sys.argv[3])
random.seed(seed)
GD=f'{ROOT}/cloud/work/r5_g/clb_{which}_{seed}'
os.makedirs(GD,exist_ok=True)
shutil.copy(f'{ROOT}/cloud/work/r5_g/grp2/group.json',GD+'/group.json')
BASE=open(f'{ROOT}/cloud/work/ipa-groups/func_800AD4C8/group.c').read()
def extract(S,name):
    return re.search(r'^s16 '+name+r'\([^;\n]*\{\n.*?\n\}\n',S,re.S|re.M).group(0)
if which=='ipc':
    V=open(f'{ROOT}/cloud/work/r5_g/ipc_v2.c').read(); fn='input_process_controller'
    names={'idx':'u16 idx[20];','vp':'f32 vp[3];','vq':'f32 vq[3];','va':'f32 va[3];','ve':'f32 ve[3];','v0':'f32 v0[3];','vprev':'f32 vprev[3];','vd':'f32 vd[3];','f1':'volatile f32 f1;','f2':'volatile f32 f2;','k':'u32 k;','n':'u32 n;','res':'s32 res;','t':'f32 t;','d':'f32 d;'}
    order=['idx','vp','vq','va','ve','v0','vprev','vd','f1','f2','k','n','res','t','d']
else:
    V=open(f'{ROOT}/cloud/work/r5_g/c3_base.c').read(); fn='func_800C3AD0'
    names={'va':'f32 va[3];','vb':'f32 vb[3];','vc':'f32 vc[3];','vd':'f32 vd[3];','ve':'f32 ve[3];','idx':'u16 idx[20];','f1':'volatile f32 f1;','f2':'volatile f32 f2;','k':'u32 k;','n':'u32 n;','res':'s32 res;'}
    order=['va','vb','vc','vd','ve','idx','f1','f2','k','n','res']
dstart=V.index('    '+list(names.values())[0]) if False else None
# find declaration block: lines between signature and first blank line
lines=V.split('\n')
sig=0
blk_start=1
blk_end=next(i for i,l in enumerate(lines) if l.strip()=='' and i>1)
extra_pool=['s32 w1;','s32 w2;']
def body(order,extras):
    decl=[]
    allv=order[:]
    out=[]
    for x in allv:
        if x.startswith('w'):
            out.append('    s32 '+x+';')
        else: out.append('    '+names[x])
    L=[lines[0]]+out+lines[blk_end:]
    s='\n'.join(L)
    for w in extras:
        pass
    return s
def use_w(s,ws):
    for w in ws:
        s=s.replace('    return res;\n}','    return res + %s - %s;\n}'%(w,w),1) if which=='ipc' else s
        s=s.replace('    res = 1;','    %s = flag;\n    res = 1;'%w,1) if which=='ipc' else s.replace('    res = 1;','    %s = n;\n    res = 1;'%w,1) if False else s
    return s
def evaluate(order):
    ws=[x for x in order if x.startswith('w')]
    b=body(order,ws)
    if which=='ipc':
        for w in ws:
            b=b.replace('    res = 1;','    %s = flag;\n    res = 1;'%w,1).replace('    return res;\n}','    return res + %s - %s;\n}'%(w,w),1)
    else:
        for w in ws:
            b=b.replace('    res = 1;','    %s = poly->type;\n    res = 1;'%w,1).replace('    return res;\n}','    return res + %s - %s;\n}'%(w,w),1)
    S=BASE.replace(extract(BASE,fn),b)
    open(GD+'/group.c','w').write(S)
    r=subprocess.run(['python3',ROOT+'/cloud/work/r5_g/gsbs.py',GD,fn,'--as1=-r4300_mul','--hi','9999'],capture_output=True,text=True).stdout
    m=re.search(r'aligned differing rows: (\d+)',r)
    if not m: return 9999
    return int(m.group(1))
cur=order[:]; cs=evaluate(cur); print('start',cs,flush=True)
best=(cs,cur[:])
for it in range(iters):
    c=cur[:]
    r=random.random()
    if r<0.6:
        i=random.randrange(len(c)); x=c.pop(i); c.insert(random.randrange(len(c)+1),x)
    elif r<0.8 and len([x for x in c if x.startswith('w')])<2:
        w='w1' if 'w1' not in c else 'w2'; c.insert(random.randrange(len(c)+1),w)
    elif r<0.9 and any(x.startswith('w') for x in c):
        c=[x for x in c if not x.startswith('w')] if random.random()<.5 else [x for x in c if x!=[y for y in c if y.startswith('w')][-1]]
    else:
        i,j=random.sample(range(len(c)),2); c[i],c[j]=c[j],c[i]
    s=evaluate(c)
    if s<=cs:
        if s<cs: print(it,s,c,flush=True)
        cur,cs=c,s
        if s<best[0]: best=(s,c[:])
print('best',best,flush=True)
