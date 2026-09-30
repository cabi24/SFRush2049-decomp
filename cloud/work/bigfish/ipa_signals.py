import re,sys,subprocess,collections
names=sys.argv[1:]
R=re.compile(r'\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)\b')
def dis(n):
    out=subprocess.run(['python3','/home/user/SFRush2049-decomp/cloud/work/tools/tdis.py',n],capture_output=True,text=True).stdout
    return [re.match(r'\s+([0-9a-f]+):\s+(\S+)\s*(.*)',l).groups() for l in out.splitlines() if re.match(r'\s+[0-9a-f]+:',l)]
for n in names:
    ins=dis(n)
    saved=set(); written=set(); nonabi=[]; wset=set(['zero','sp','ra'])
    stop=False
    for a,op,ops in ins:
        ops=re.sub(r'<.*','',ops); rs=R.findall(ops)
        st=op in('sw','sh','sb','sd')
        if st and 'sp' in ops.split(',')[-1] and rs and rs[0].startswith('s') : saved.add(rs[0])
        if op in ('jal','jalr','jr') : stop=True
        dst=[] if (st or op.startswith(('b','j','mult','div','mt'))) else rs[:1]
        srcs=rs if (st or op.startswith(('b','j','mult','div','mt'))) else rs[1:]
        if not stop:
            for r in srcs:
                if r not in wset and (r[0] in 'ts' or r in('v0','v1','at')) and not (st and r in saved|{r} and 'sp' in ops.split(',')[-1] and r[0]=='s'):
                    nonabi.append((a,r))
        for r in dst:
            wset.add(r)
            if r.startswith('s') and r!='sp': written.add(r)
    unsaved=sorted(written-saved)
    print(f'{n:28s} words={len(ins):4d} nonABI-entry-reads={sorted(set(r for _,r in nonabi))} s-written-unsaved={unsaved}')
