# sbs.py OURS.dis : unified diff of mnemonic+operands, retail.raw vs ours (cdis objdump output)
import re, sys, difflib
def ins(path):
    out=[]
    for l in open(path):
        m=re.match(r'\s*([0-9a-f]+):\t[0-9a-f]{8}\s+\t?(.*)', l) or re.match(r'\s*([0-9a-f]+):\t[0-9a-f]+ \t(.*)', l)
        if m: out.append(m.group(2).strip().replace('\t',' '))
    return out
d='/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14p/'
r=ins(d+'retail.raw'); o=ins(sys.argv[1])
sm=difflib.SequenceMatcher(None,r,o,autojunk=False)
print('retail',len(r),'ours',len(o),'matching-ops',sum(b.size for b in sm.get_matching_blocks()))
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    for k in range(max(i2-i1,j2-j1)):
        a=r[i1+k] if i1+k<i2 else ''; b=o[j1+k] if j1+k<j2 else ''
        print(f'{tag:8s} r[{i1+k:3d}] {a:34s} | o[{j1+k:3d}] {b}')
