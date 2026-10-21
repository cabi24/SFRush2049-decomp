#!/bin/bash
# usage: asmtry.sh LABEL EDITED.s NAME  -- assemble edited listing, diff against retail ovl_a
cd /home/cburnes/projects/rush2049-decomp
export TAG=w36b SCR=rush2049/scratch/frontier/w36b
cloud/work/frontier/tools/trace/asm.sh $1 $2 $3 >/dev/null 2>&1
python3 cloud/work/frontier/w36b/tools/dd.py x $3 --obj build/trace/w36b/asm_$1.o 2>/dev/null | grep -v unresolved | grep -v "lui .*,0x0$\|addiu .*,0x0$" | head -${4:-30}
