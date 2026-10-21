import re,collections,sys,subprocess
def census(lines):
    c=collections.defaultdict(list)
    for l in lines:
        m=re.search(r'\b(sw|lw|sdc1|swc1|lwc1|ldc1|sh|lh|sb|lb)\s+(\S+?),(-?\d+)\(sp\)',l)
        if m: c[int(m.group(3))].append(m.group(1)[0]+':'+m.group(2))
    return c
w=[l for l in open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35b/dis/A0FD8.dis')]
g=[l for l in open('/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/a0got.txt')]
cw,cg=census(w),census(g)
hi=int(sys.argv[1]) if len(sys.argv)>1 else 150
for k in sorted(set(cw)|set(cg)):
    if k>=hi: print(f'{k:4d}  R:{",".join(sorted(set(cw.get(k,[])))):40s} U:{",".join(sorted(set(cg.get(k,[]))))}')
