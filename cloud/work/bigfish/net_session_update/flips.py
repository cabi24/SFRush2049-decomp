import sys,os,json,re; sys.path.insert(0,'.')
import srch3,ev
from multiprocessing import Pool
def region_exact(m, lo, hi):
    t=srch3.render(m)
    try:
        got,masks,st=ev.compile_words(t)
    except BaseException: return None
    want=[w&masks.get(st+4*i,0xFFFFFFFF) for i,w in enumerate(ev.WANT)]
    return sum(1 for i in range(lo,min(hi,len(got))) if got[i]==want[i]), ev.evaluate(t)
def f(args):
    m,lo,hi=args
    return m,region_exact(m,lo,hi)
if __name__=='__main__':
    base=json.load(open(sys.argv[1])); lo=int(sys.argv[2]); hi=int(sys.argv[3])
    cands=[]
    for k in base:
        if k in srch3.OPTS: vals=list(range(len(srch3.OPTS[k])))
        elif re.fullmatch(r'T\d+',k): vals=['<','!=']
        elif k in ('Sx','Xx'): vals=srch3.VARS+['r']
        else: vals=srch3.VARS
        for v in vals:
            if v!=base[k]:
                m=dict(base); m[k]=v
                if srch3.valid(m): cands.append((m,lo,hi))
    with Pool(4) as p: out=p.map(f,cands)
    b0=region_exact(base,lo,hi); print('base',b0)
    res=[]
    for m,r in out:
        if r: res.append((r[0],r[1]['strict'],[ (k,m[k]) for k in base if m[k]!=base[k]][0]))
    res.sort(key=lambda x:(-x[0],-x[1]))
    for x in res[:15]: print(x)
