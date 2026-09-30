#!/bin/bash
# sbs.sh file.c fn [flags]: aligned diff of compiled vs target (mnemonics+operands)
cd /home/user/SFRush2049-decomp/cloud/work/r5_f
./dis.sh "$1" "${3:--g0 -O2 -mips2 -G 0 -non_shared}" | grep -P '^\s+[0-9a-f]+:' | cut -f3- | sed 's/\s\+/ /g;s/ <.*//' > /tmp/claude-0/got.txt 2>/dev/null
python3 ../tools/tdis.py $2 | grep -P '^\s+[0-9a-f]+:' | sed 's/^[^:]*: *//;s/\s\+/ /g;s/ <.*//' > /tmp/claude-0/want.txt
diff -y -W 100 /tmp/claude-0/want.txt /tmp/claude-0/got.txt | head -${4:-80}
