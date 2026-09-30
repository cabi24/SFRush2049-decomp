import sys,random,re,json,time,os,math; sys.path.insert(0,'.')
import ev
from multiprocessing import Pool
import srch3
from srch3 import *
def main():
    seed=int(sys.argv[1]); iters=int(sys.argv[2]); tag=sys.argv[3]
    rng=random.Random(seed)
    starts=[json.load(open(p)) for p in sys.argv[4:]] or [None]
    cur=init(starts[0])
    while not valid(cur): cur=mutate(cur,rng)
    r0=ev.evaluate(render(cur)); cs=score(r0); print('start',r0,cs,flush=True)
    best=(cs,dict(cur),r0); stag=0; T=6.0
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
            if s>=cs or rng.random()<math.exp((s-cs)/T):
                cur,cs=m,s
            if s>best[0]:
                best=(s,dict(m),r); stag=0; print(it,'best',r,s,flush=True)
                json.dump(m,open('best4_%s.json'%tag,'w'))
            else: stag+=1
            if stag>40:
                stag=0
                if rng.random()<0.5: cur=dict(best[1]); cs=best[0]
                else:
                    cur=init(rng.choice(starts))
                    for _ in range(rng.randrange(3,12)): cur=mutate(cur,rng)
                    while not valid(cur): cur=mutate(cur,rng)
                    rr=ev.evaluate(render(cur)); cs=score(rr) if rr else -1
                print(it,'restart',cs,flush=True)
    print('final',best)
main()
