import sys,re,random,subprocess,os,shutil,pickle
R='/home/user/SFRush2049-decomp/cloud/work/r5_g/'
ROOT='/home/user/SFRush2049-decomp'
def sites_from(body, extra=()):
    """commutation sites for simple binary ops; returns list of (orig, alt) in body order"""
    term=r'[A-Za-z_]\w*(?:\[[^\]\[]+\])?(?:->\w+)?'
    pat=re.compile(r'(?<![\w\]\)])('+term+r') ([*+]) ('+term+r')(?![\w\[\(])')
    out=[]; seen=set()
    for m in pat.finditer(body):
        o=m.group(0); a=f'{m.group(3)} {m.group(2)} {m.group(1)}'
        if o in seen: continue
        seen.add(o)
        out.append((m.start(),o,a))
    return out
def run(fn, gdir, base_fn_text, getbase, seed, iters, tag):
    """base_fn_text: function text. getbase(newtext)->full group source"""
    random.seed(seed)
    os.makedirs(gdir,exist_ok=True)
    shutil.copy(ROOT+'/cloud/work/ipa-groups/func_800AD4C8/group.json',gdir+'/group.json')
    def ev(text):
        open(gdir+'/group.c','w').write(getbase(text))
        r=subprocess.run(['python3',R+'gsbs.py',gdir,fn,'--as1=-r4300_mul','--hi','9999'],capture_output=True,text=True).stdout
        m=re.search(r'aligned differing rows: (\d+)',r)
        return int(m.group(1)) if m else 9999
    # sites by occurrence index so repeated identical strings are separate
    term=r'[A-Za-z_]\w*(?:\[[^\]\[]+\])?(?:->\w+)?'
    pat=re.compile(r'(?<![\w\]\)])('+term+r') ([*+]) ('+term+r')(?![\w\[\(])')
    spans=[(m.start(),m.end(),m.group(0),f'{m.group(3)} {m.group(2)} {m.group(1)}') for m in pat.finditer(base_fn_text)]
    n=len(spans)
    def build(sel):
        s=base_fn_text; 
        for (st,en,o,a),f in sorted(zip(spans,sel),key=lambda x:-x[0][0]):
            if f: s=s[:st]+a+s[en:]
        return s
    cur=[0]*n; cs=ev(build(cur)); print(tag,'start',cs,'sites',n,flush=True)
    best=(cs,cur[:])
    for it in range(iters):
        c=cur[:]
        for _ in range(random.choice([1,1,1,2,3])):
            i=random.randrange(n); c[i]^=1
        s=ev(build(c))
        if s<=cs:
            if s<cs: print(tag,it,s,flush=True)
            cur,cs=c,s
            if s<best[0]:
                best=(s,c[:]); pickle.dump(build(c),open(R+f'auto_{tag}.pkl','wb'))
    print(tag,'best',best[0],flush=True)
    pickle.dump(build(best[1]),open(R+f'auto_{tag}.pkl','wb'))
