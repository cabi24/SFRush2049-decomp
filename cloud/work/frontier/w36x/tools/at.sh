#!/bin/bash
# usage: at.sh LABEL EDITED.s NAME [IMG] [N] -- assemble edited listing in st_LABEL, print summary + diff lines (non-equal only)
cd /home/cburnes/projects/rush2049-decomp
export TAG=w36x SCR=rush2049/scratch/frontier/w36x
cloud/work/frontier/tools/trace/asm.sh $1 $2 $3 >/dev/null 2>&1
python3 cloud/work/frontier/w36x/tools/dd.py x $3 --img ${4:-ovl_a} --ctx 1 --obj build/trace/w36x/asm_$1.o 2>/dev/null | grep -v "lui .*,0x0$\|addiu .*,0x0$" | head -${5:-30}
