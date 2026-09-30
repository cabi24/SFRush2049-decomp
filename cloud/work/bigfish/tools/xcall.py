import re,sys
# after each jal, registers in caller-save set (t0-t9,a1-a3?,v1,f4-f10,f16-f18) read before written within next straight-line block (up to next branch target ignoring); approximation: linear, stop at next jal.
f=sys.argv[1]
rows=[]
for l in open(f+'.dis'):
    m=re.match(r'\s+([0-9a-f]+):\s+(\S+)\s*(.*)',l)
    if m: rows.append((m.group(1),m.group(2),re.sub(r'<.*','',m.group(3)).strip(),m.group(3)))
CS=re.compile(r'^(t\d|v1|a[123]|\$f(?:[4-9]|10|1[6-9]))$')
hits={}
for i,(a,op,ar,full) in enumerate(rows):
    if op!='jal': continue
    tgt=re.search(r'<([^>]*)>',full); tgt=tgt.group(1) if tgt else '?'
    w=set(); 
    for (a2,op2,ar2,_) in rows[i+2:i+40]:
        regs=re.findall(r'\$f\d+|\b(?:v[01]|a[0-3]|t\d|s\d|ra)\b',ar2)
        if op2=='jal': break
        if not regs: continue
        if op2.startswith('s') and op2[1] in 'wbhd' and op2!='sll' or op2.startswith('b') or op2.startswith('c.') or op2.startswith('swc1') or op2.startswith('sdc1'): src,dst=regs,[]
        elif op2.startswith('mtc1'): src,dst=regs[:1],regs[1:]
        else: dst,src=regs[:1],regs[1:]
        for r in src:
            if CS.match(r) and r not in w: hits.setdefault(tgt,set()).add((r,a))
        w|=set(dst)
        if op2 in('jr',) or op2.startswith('b') and op2 not in('bnezl','beqzl'): break
for t,v in hits.items(): print(f,'after call to',t,'reads caller-save w/o write:',sorted(v)[:5])
