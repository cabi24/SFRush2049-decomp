#!/bin/sh
# asmdis.sh LABEL FILE.s : assemble an edited listing (stock as0+as1, -O3 flags) in snapshot st_LABEL, print objdump of .text
export TAG=w12b
. /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace/lib/env.sh
TK=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace
f=$(realpath "$2"); scp -q "$f" "$BUILDER:$SCR/st_$1/edit.s" && rtk $1 asm "~/$SCR/st_$1/edit.s" >/dev/null && fetch $1 asm.o "$LOUT/asmd_$1.o" && mips-linux-gnu-objdump -d -z --no-show-raw-insn "$LOUT/asmd_$1.o" | grep -E "^ +[0-9a-f]+:" | sed 's/^ *//'
