#!/bin/bash
# usage: asmtest.sh LABEL EDITED.s FN  -> add .globl for internal functions, assemble with stock as1 on st_LABEL, score FN
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad
L=$1; F=$2; N=$3; shift 3
sed -E 's/^\t\.ent\t(func_[0-9A-F]+) 2$/\t.globl\t\1\n\t.ent\t\1 2/' $F > $S/_asm_$L.s
scp -q $S/_asm_$L.s watchman2:rush2049/scratch/frontier/w36h/st_$L/_e.s
ssh watchman2 "sh ~/rush2049/scratch/frontier/w36h/tk/tk.sh ~/rush2049/scratch/frontier/w36h $L asm ~/rush2049/scratch/frontier/w36h/st_$L/_e.s >/dev/null 2>&1; cd ~/rush2049/scratch/frontier/w36h && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/gscore.py obj st_$L/asm.o --names $N $*" | grep -v '^compiled'
