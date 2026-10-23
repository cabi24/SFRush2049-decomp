#!/bin/sh
# ofor.sh LABEL NAME SPEC [IMG] -- colour-force oracle on snapshot st_LABEL, overlay diff (positional count + diff lines)
cd /home/cburnes/projects/rush2049-decomp
export TAG=w41b SCR=rush2049/scratch/frontier/w41b
. cloud/work/frontier/tools/trace/lib/env.sh; TK=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace
rtk $1 force $2 "'$3'" | grep -v "^proc" | head -${FN:-8}
fetch $1 f.o "$LOUT/f_$1.o" && python3 cloud/work/frontier/w41b/tools/dd.py x $2 --img ${4:-ovl_a} --ctx 0 --obj "$LOUT/f_$1.o" 2>/dev/null | grep -v "0x803c\|,0x0$\|172(at)\|-27412" | head -${DN:-14}
