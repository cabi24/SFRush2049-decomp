#!/bin/bash
# vtry.sh TEMPLATE_FUNC.c VARIANT_DIR [N]: for each file in VARIANT_DIR (full function sources) build group (mk.py on base g3 context) and print differing-row count (words mode)
# usage: vtry.sh BASEGROUPDIR VARDIR
BASE=$1; VD=$2
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35c
for f in $VD/*.c; do
  n=$(basename $f .c)
  python3 tools/mk.py $BASE $f /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/4b393cd0-4116-4767-90e0-4ddf67d9ff98/scratchpad/vg_$n >/dev/null
done
