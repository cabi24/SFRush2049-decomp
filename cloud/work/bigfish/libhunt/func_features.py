import sys,json,struct,pickle
sys.path.insert(0,'/home/user/SFRush2049-decomp/tools/cloud'); import score
syms={k:int(v,16) for k,v in json.load(open('/home/user/SFRush2049-decomp/asm/us/blob/symbols.json'))['symbols'].items()}
rev={}
for k,v in syms.items(): rev.setdefault(v,k)
T=score.targets()
rows=[]
for n,w in T.items():
    a=syms.get(n)
    if a is None or not (0x80086A50<=a<0x80125000): continue
    gl=set(); jals=[]; fl=0; pf=0; jr=0; lo=0
    lui={}
    for i,x in enumerate(w):
        op=x>>26
        if op==0xF:
            hi=x&0xffff; lui[(x>>16)&31]=hi
            if 0x8000<=hi<0x8020: gl.add(hi)
        if op==3: jals.append(rev.get((x&0x3ffffff)<<2|0x80000000,'?'))
        if op==0 and (x&0x3f)==8 and ((x>>21)&31)!=31: jr+=1
        if op in(0x31,0x39,0x35,0x3d) or (op==0x11): fl+=1
    rows.append(dict(n=n,a=a,w=len(w),gl=sorted(gl),jals=jals,fl=fl,jr=jr))
rows.sort(key=lambda r:r['a'])
pickle.dump(rows,open(sys.argv[1],'wb'))
print(len(rows))
