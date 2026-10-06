#!/bin/bash
# usage: udef.sh CAND.c -- score in unit then print cfe Udef (candidate file) and merged/opt Udef of func_8008E408
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/u; mkdir -p $S
T=$(dirname $(realpath $0)); b=$(basename $1 .c)
$T/sc.sh $1 | head -1
scp -q watchman2:rush2049/scratch/frontier/unit/w10h/stage/c_$b.u $S/c_$b.u
python3 - $S/c_$b.u <<'PY'
import sys,struct
sys.path.insert(0,'/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import OPCODE_NAMES
d=OPCODE_NAMES.index('def')
b=open(sys.argv[1],'rb').read(); w=struct.unpack('>%dI'%(len(b)//4),b[:len(b)//4*4])
print('cfe defs:',[('%08x'%x,'%x'%w[i+2]) for i,x in enumerate(w) if x>>24==d and i+3<len(w) and (x&0xffff)==0])
PY
FILT=${FILT:-0x40000,0x800,0x400,0x200} $T/getu.sh $b; grep -h Udef $S/$b.merged.ud $S/$b.opt.ud | awk '{print "def",$2,$(NF-1),$NF}'
$T/sp.sh | tail -1
