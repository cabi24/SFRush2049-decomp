# blk.py OR_SPL B [B...]: for each block, list the live ranges (first evaluation) whose block set contains it, with
# their card/nocs/tot; and the occurrence blocks around. Diagnostic only.
import sys,re
spl=sys.argv[1]; want=[int(x) for x in sys.argv[2:]]
lines=open(spl).read().split('\n'); vec={}; summ={}; cur=[]; occ={}
for k,l in enumerate(lines):
    if not l.startswith('[SAVE] '): continue
    lr=int(re.search(r'lr=(\d+)',l).group(1))
    if ' card=' in l:
        v=lines[k+1]
        if v.startswith('[SAVEV] lr=%d:'%lr) and lr not in vec:
            vec[lr]=set(int(x) for x in v.split(':')[1].split())|set(b for b,_ in cur); summ[lr]=l.split('] ')[1]
            for b,u in cur: occ.setdefault(b,[]).append('%d:%s'%(lr,u))
        cur=[]; continue
    m=re.search(r'bb=(\d+) u=(\d+) d=(\d+)',l); cur.append((int(m.group(1)),m.group(2)+m.group(3)))
for b in want:
    print('block',b,'occ:',occ.get(b,[]))
    for lr in sorted(vec):
        if b in vec[lr]: print('   ',summ[lr])
