#!/usr/bin/env python3
"""udump.py FILE.u : tolerant ucode opcode dump (workbench framing; unknown words skipped)."""
import sys,struct
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench import ucode as U
b=open(sys.argv[1],'rb').read(); w=struct.unpack('>%dI'%(len(b)//4),b)
c=0
while c<len(w):
    h=w[c]; op=h>>24
    if op>=len(U.OPCODE_NAMES): c+=1; continue
    n=U.OPCODE_NAMES[op]; dt=(h>>16)&0x1f; bl=U._base_word_length(n); tl=bl
    if n in U._HAS_CONSTANT:
        tl+=2
        if dt in U._VARIABLE_CONSTANT_DTYPES or n=="comm":
            pw=(w[c+bl]+3)//4; pw+=pw&1; tl+=pw
    print("%5d %-5s dt=%-2d mt=%d %s"%(c,n,dt,(h>>21)&7," ".join("%x"%x for x in w[c+1:c+tl])))
    c+=tl
