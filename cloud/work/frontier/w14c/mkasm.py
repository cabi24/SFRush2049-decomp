import re,sys
# convert tdis output (addr\tmnem ops) to gas-style text with labels for m2c
src=sys.argv[1]; name=sys.argv[2]; out=sys.argv[3]
lines=[]
for raw in open(src):
    m=re.match(r'\s*([0-9a-f]{8}):\s+(.*)',raw)
    if m: lines.append((int(m.group(1),16),m.group(2).strip()))
targets=set()
for a,t in lines:
    for tg in re.findall(r'0x([0-9a-f]{8})',t):
        targets.add(int(tg,16))
out_l=[".set noreorder",".set noat",".globl %s"%name,"%s:"%name]
for a,t in lines:
    if a in targets: out_l.append("L%08x:"%a)
    t=re.sub(r'\s+<[^>]*>','',t)
    def rep(mm):
        v=int(mm.group(1),16)
        if v in targets and not ('jal' in t): return "L%08x"%v
        return mm.group(0)
    if t.startswith('jal'):
        t=re.sub(r'jal 0x([0-9a-f]{8})',lambda mm: 'jal func_%s'%mm.group(1).upper() if False else 'jal func_'+mm.group(1).upper(),t)
    else:
        t=re.sub(r'0x([0-9a-f]{8})',rep,t)
    out_l.append("    "+t.replace("\t"," "))
open(out,"w").write("\n".join(out_l)+"\n")
