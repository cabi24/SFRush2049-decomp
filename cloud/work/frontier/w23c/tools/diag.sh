#!/bin/sh
# w23c (builder): workbench diagnose SRC FN
sh lane/tools/obj.sh $1 $2 lane/cand.o 2>/dev/null && python3 tools/workbench.py diagnose tgt/$2.o lane/cand.o --function $2 --objdump mips-linux-gnu-objdump
