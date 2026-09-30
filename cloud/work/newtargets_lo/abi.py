import json,re
funcs=json.load(open('dis.json'))
def R(x):
    return x.replace('$','').strip()
res=[]
for nm,ls in funcs.items():
    written={'a0','a1','a2','a3','sp','ra','zero','gp','v0','v1','at','k0','k1','f0','f1','f2','f3','f12','f13','f14','f15','fp'}
    bad=set();calls=0
    for l in ls:
        ins=l.split(':',1)[1].strip().split('#')[0]
        ins=re.sub(r'<.*>','',ins).strip()
        p=ins.split(None,1); op=p[0]
        ops=p[1] if len(p)>1 else ''
        args=[x.strip() for x in ops.split(',')] if ops else []
        if op=='jal': calls+=1
        regs=[R(m) for m in re.findall(r'\$?[a-z]+\d*(?=\)|,|$)',ops)]
        toks=re.findall(r'\$?([a-z]+\d+|zero|ra|sp|gp|at|fp)\b',ops)
        store=op[:2] in('sw','sh','sb','sd') or op in('swc1','sdc1','swl','swr')
        branch=op[0]=='b' or op in('j','jr','jal','jalr')
        if store or branch or not args:
            srcs=toks; dst=[]
        else:
            dst=toks[:1]; srcs=toks[1:]
        for s in srcs:
            if re.fullmatch(r't[0-9]|s[0-7]|f\d+',s) and s not in written: bad.add(s)
        for d in dst: written.add(d)
    res.append((len(ls),nm,calls,sorted(bad)))
for r in sorted(res): print(r)
