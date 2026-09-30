import sys,random,re,json,time,os; sys.path.insert(0,'.')
import ev
from multiprocessing import Pool
TMPL=os.environ.get('TMPL','tmpl2.txt')
tmpl=open(TMPL).read()
OPTS=json.load(open(os.environ.get('OPTS','opts.json')))
# tokens present in template
VARTOK=sorted(set(re.findall(r'@([A-Z][A-Za-z0-9]*)@',tmpl))-set(k for k in OPTS))
OPTTOK=sorted(set(re.findall(r'@@([A-Za-z0-9_]+)@@',tmpl)))
VARS=os.environ.get('VARS','i,j,k,x').split(',')
GROUPS=[('Ao','Aj'),('Si','Sx'),('Co','Ci'),('Xj','Xx','Xi'),('Fi','Fj'),('Gi','Gj','Gk'),('Gi','Hj'),('Yi','Yj')]
def valid(m):
    for g in GROUPS:
        vs=[m[t] for t in g if t in m]
        if len(set(vs))!=len(vs): return False
    return True
def render(m):
    t=tmpl
    for k in OPTTOK: t=t.replace('@@%s@@'%k, OPTS[k][m[k]])
    for k in VARTOK: t=t.replace('@%s@'%k, m[k])
    return t
def score(res):
    if not res: return -1
    return 0.2*res['strict']+res.get('exact_r',res['exact'])+res['norm']+res.get('reg',0)/2-3*abs(res['n']-892)
def f(m):
    return m,ev.evaluate(render(m))
def mutate(m,rng):
    m=dict(m)
    keys=list(m)
    FREEKEYS=[k for k in keys if re.fullmatch(os.environ.get('FREE','.*'),k)]
    for _ in range(rng.choice([1,1,2,2,3,4])):
        k=rng.choice(FREEKEYS)
        if k in OPTS: m[k]=rng.randrange(len(OPTS[k]))
        elif re.fullmatch(r'T\d+',k): m[k]=rng.choice(['<','!='])
        elif k in ('Sx','Xx'): m[k]=rng.choice(VARS+['r'])
        else: m[k]=rng.choice(VARS)
    return m
def init(seedmap=None):
    m={}
    for k in OPTTOK: m[k]=0
    for k in VARTOK:
        if re.fullmatch(r'T\d+',k): m[k]='<'
        else: m[k]='i'
    if seedmap: m.update({k:v for k,v in seedmap.items() if k in m})
    return m
if __name__=='__main__':
    seed=int(sys.argv[1]); iters=int(sys.argv[2]); tag=sys.argv[3] if len(sys.argv)>3 else 'x'
    rng=random.Random(seed)
    sm=json.load(open(sys.argv[4])) if len(sys.argv)>4 else None
    cur=init(sm)
    if not valid(cur):
        # fix by random
        while not valid(cur): cur=mutate(cur,rng)
    r0=ev.evaluate(render(cur)); cs=score(r0); print('start',r0,cs,flush=True)
    best=(cs,dict(cur),r0)
    with Pool(4) as pool:
        for it in range(iters):
            cands=[]
            while len(cands)<16:
                m=mutate(cur,rng)
                if valid(m) and m!=cur: cands.append(m)
            out=pool.map(f,cands)
            out=[(score(r),m,r) for m,r in out if r]
            if not out: continue
            out.sort(key=lambda x:-x[0])
            s,m,r=out[0]
            if s>=cs:
                cur,cs=m,s
                if s>best[0]:
                    best=(s,dict(m),r); print(it,'best',r,s,flush=True)
                    json.dump(m,open('best3_%s.json'%tag,'w'))
    print('final',best)
