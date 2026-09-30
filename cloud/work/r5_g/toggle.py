import sys,pickle,random,subprocess,re,json,os,shutil
R='/home/user/SFRush2049-decomp/cloud/work/r5_g/'
def make_eval(gdir, fn, members=None):
    def ev(src):
        open(gdir+'/group.c','w').write(src)
        r=subprocess.run(['python3',R+'gsbs.py',gdir,fn,'--as1=-r4300_mul','--hi','9999'],capture_output=True,text=True).stdout
        m=re.search(r'aligned differing rows: (\d+) size (\d+)',r)
        return (int(m.group(1)) if m else 9999)
    return ev
def climb(base, sites, ev, iters, seed, log=print):
    # sites: list of lists of alternative strings; base contains alternative[0] of each (as given)
    random.seed(seed)
    cur=[0]*len(sites)
    def build(sel):
        s=base
        for i,alts in enumerate(sites):
            s=s.replace(alts[0],alts[sel[i]],1) if sel[i] else s
        return s
    cs=ev(build(cur)); log('start',cs)
    best=(cs,cur[:])
    for it in range(iters):
        c=cur[:]
        for _ in range(random.choice([1,1,2])):
            i=random.randrange(len(sites)); c[i]=random.randrange(len(sites[i]))
        s=ev(build(c))
        if s<=cs:
            if s<cs: log(it,s,c)
            cur,cs=c,s
            if s<best[0]: best=(s,c[:])
    log('best',best)
    return best,build(best[1])
