import sys,random,re,json,time; sys.path.insert(0,'.')
import ev
from multiprocessing import Pool
tmpl=open('tmpl.txt').read()
BASE=dict(L1='i',Ao='i',Aj='j',Bi='i',Si='i',Sx='x',Co='i',Ci='j',Xj='j',Xx='x',Xi='i',Fi='i',Fj='j',Gi='i',Gj='j',Gk='k',Hj='j',Zi='i',Yi='i',Yj='j',Wi='i',T1='<',T2='!=',T3='<',T4='<',T5='<',T6='<',T7='<')
GROUPS=[('Ao','Aj'),('Si','Sx'),('Co','Ci'),('Xj','Xx','Xi'),('Fi','Fj'),('Gi','Gj','Gk'),('Gi','Hj'),('Yi','Yj')]
VARS=['i','j','k','x']
def valid(m):
    for g in GROUPS:
        vs=[m[t] for t in g]
        if len(set(vs))!=len(vs): return False
    return True
def render(m):
    t=tmpl
    for k,v in m.items(): t=t.replace('@%s@'%k,v)
    return t
def score(res):
    if not res: return -1
    return res['exact']+res['norm']-3*abs(res['n']-892)
def f(m):
    r=ev.evaluate(render(m)); return m,r
def mutate(m,rng):
    m=dict(m)
    for _ in range(rng.choice([1,1,1,2,3])):
        k=rng.choice(list(m))
        if k[0]=='T' and k[1:].isdigit(): m[k]=rng.choice(['<','!='])
        else: m[k]=rng.choice(VARS)
    return m
if __name__=='__main__':
    seed=int(sys.argv[1]); iters=int(sys.argv[2]); rng=random.Random(seed)
    cur=dict(BASE); r0=ev.evaluate(render(cur)); cs=score(r0); print('start',r0,cs,flush=True)
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
                    json.dump(m,open('best_map_%d.json'%seed,'w'))
    print('final',best)
