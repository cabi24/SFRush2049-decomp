import sys,re
seen={}
for ln in open(sys.argv[1]):
    m=re.match(r'(\d+) U(\w+) Mmt \d+ \d+\s+ae7 ([0-9a-f]+) ([0-9a-f]+)',ln.strip())
    if not m: continue
    op,size,off=m.group(2),int(m.group(3),16),int(m.group(4),16)
    if off>=0x80000000: off-=1<<32
    if op=='lda': size=int(m.group(3),16)
    seen.setdefault(off,[size,op,0])
    seen[off][2]+=1
for off in sorted(seen,reverse=True):
    print('%6d size %3d first %s uses %d'%(off,*seen[off]))
