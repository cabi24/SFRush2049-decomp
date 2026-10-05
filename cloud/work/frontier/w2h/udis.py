#!/usr/bin/env python3
"""udis.py NAME [unit.o]: side-by-side of the unit object's NAME against retail (aligned by difflib on mnemonic text)."""
import sys,subprocess,re,json,struct,tempfile,difflib
ROOT='/home/cburnes/projects/rush2049-decomp'
sys.path.insert(0,ROOT+'/tools/cloud'); import score
name=sys.argv[1]; obj=sys.argv[2] if len(sys.argv)>2 else ROOT+'/build/blob_unit/w2h/unit.o'
out=subprocess.run(['mips-linux-gnu-objdump','-dr','-z','-M','gpr-names=32','--disassemble='+name,obj],capture_output=True,text=True).stdout
got=[]; rel=set()
for l in out.splitlines():
    m=re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)',l)
    if m: got.append(re.sub(r'\s+',' ',m.group(3)).split(' <')[0]); continue
    if re.match(r'\s*[0-9a-f]+: R_MIPS',l) and got: rel.add(len(got)-1)
words=score.targets()[name]
with tempfile.NamedTemporaryFile(suffix='.bin') as f:
    f.write(struct.pack('>%dI'%len(words),*words)); f.flush()
    o=subprocess.run(['mips-linux-gnu-objdump','-D','-z','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
want=[]
for l in o.splitlines():
    m=re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)',l)
    if m: want.append(re.sub(r'\s+',' ',m.group(3)))
def key(s): 
    s=re.sub(r'\b(j|jal|b\w*)\s+(.*,)?0x[0-9a-f]+',lambda m:m.group(1)+' '+(m.group(2) or '')+'L',s)
    return s
sm=difflib.SequenceMatcher(None,[key(x) for x in want],[key(x) for x in got],autojunk=False)
n=0
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal':
        continue
    for k in range(max(i2-i1,j2-j1)):
        a=want[i1+k] if i1+k<i2 else ''
        b=got[j1+k] if j1+k<j2 else ''
        if a and b:
            za=re.sub(r'-?(0x)?[0-9a-f]+(\(|$)',r'N\2',a); zb=re.sub(r'-?(0x)?[0-9a-f]+(\(|$)',r'N\2',b)
            if za==zb and (re.search(r',0\(|,0x0$|,0$',b) or a.split()[0] in ('jal','j') or a.split()[0].startswith('b') or (j1+k) in rel): continue
        print('| +%03x  %-34s %s'%((i1+k)*4,a,b)); n+=1
print('want %d got %d differing rows %d'%(len(want),len(got),n))
