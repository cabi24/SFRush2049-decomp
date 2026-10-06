import sys, re
lines=open(sys.argv[1]).read().splitlines()
name=re.match(r'== (\S+)', lines[0]).group(1)
ins=[]
for l in lines[1:]:
    m=re.match(r'\s*([0-9a-f]+):\s+(\S+)\s*(.*)', l)
    if m: ins.append((int(m.group(1),16), m.group(2), m.group(3)))
addrs={a for a,_,_ in ins}
targets=set()
regs='zero|at|v0|v1|a0|a1|a2|a3|t0|t1|t2|t3|t4|t5|t6|t7|t8|t9|s0|s1|s2|s3|s4|s5|s6|s7|s8|fp|k0|k1|gp|sp|ra'
out=[]
for a,op,args in ins:
    args=re.sub(r'\s*<[^>]*>','',args)
    if op in ('jal','j') :
        m=re.search(r'<([^>,]+)', [l for l in lines if l.strip().startswith('%x:'%a)][0])
        tgt=m.group(1) if m else 'func_%08X'%int(args,16)
        if tgt=='?' : tgt='func_%08X'%int(args,16)
        args=tgt
    else:
        m=re.search(r'0x(800[0-9a-f]{5})$',args)
        if m and int(m.group(1),16) in addrs and (op.startswith('b') ):
            t=int(m.group(1),16); targets.add(t); args=args[:m.start()]+'.L%08X'%t
    args=re.sub(r'\b(%s)\b'%regs, r'$\1', args)
    out.append((a,'%s %s'%(op,args)))
print('.set noat\n.set noreorder\n\nglabel %s'%name)
for a,s in out:
    if a in targets: print('.L%08X:'%a)
    print('    /* %08X */ %s'%(a,s))
