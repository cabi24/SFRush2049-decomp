#!/usr/bin/env python3
"""rdis.py IMG NAME [N]: disassemble retail overlay function NAME from asm/us/IMG (first N words)."""
import re,subprocess,struct,tempfile,sys,json
img,name=sys.argv[1],sys.argv[2]; n=int(sys.argv[3]) if len(sys.argv)>3 else 10**6
R='/home/cburnes/projects/rush2049-decomp/asm/us/'+img
L=open([R+'/'+f for f in __import__('os').listdir(R) if f.endswith('.s')][0]).read().split('\n')
i=[k for k,l in enumerate(L) if l.startswith(name+':')][0]
w=[]
for l in L[i+1:]:
    m=re.match(r'\s*\.word 0x([0-9A-Fa-f]{8})',l)
    if m: w.append(int(m.group(1),16))
    elif l.startswith('.section') or l.startswith('glabel') : break
w=w[:n]
with tempfile.NamedTemporaryFile(suffix='.bin') as f:
    f.write(struct.pack('>%dI'%len(w),*w)); f.flush()
    out=subprocess.run(['mips-linux-gnu-objdump','-D','-z','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
for l in out.split('\n'):
    if re.match(r'\s+[0-9a-f]+:',l): print(l)
