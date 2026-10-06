import sys,struct
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import OPCODE_NAMES
d=OPCODE_NAMES.index('def')
b=open(sys.argv[1],'rb').read(); w=struct.unpack('>%dI'%(len(b)//4),b[:len(b)//4*4])
print('cfe defs:',[('%08x'%x,'%x'%w[i+2]) for i,x in enumerate(w) if x>>24==d and i+3<len(w) and (x&0xffff)==0])
