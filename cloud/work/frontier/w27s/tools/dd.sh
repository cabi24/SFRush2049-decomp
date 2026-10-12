#!/bin/sh
# w27s builder: dd.sh SRC FN [TARGET] -> lock-style compile (reduced TU, toolkit $TK, the source's flags line) and a
# side-by-side objdump -dr diff vs tgt/TARGET.o (default FN). Run from the scratch root.
S=$HOME/rush2049/scratch/frontier/w27s; SRC=$1; FN=$2; T=${3:-$2}
TK=${TK:-$HOME/rush2049/cache/toolkits/8bdf0caebc9ada9612b9dae9422948e37bd119e663e9dda8bb8fdbb7acd96682}
D=$(mktemp -d $S/dd.XXXX)
fl=$(head -1 "$SRC" | sed -n 's@/\* flags: \(.*\) \*/@\1@p'); fl=${fl:--g0 -O2 -mips2 -G 0 -non_shared}
python3 -c "
import sys; sys.path[:0]=['$S']
from tools.conveyor.seeds import context as c
open('$D/r.c','w').write(c.reduced_tu(open('$SRC').read(),'$FN'))"
(cd $D && $TK/ido/cc -c $fl -I $S/include -I $S/include/PR -I $TK/shim -o r.o r.c) || exit 1
rows() { mips-linux-gnu-objdump -dr -j .text "$1" | sed -n '/^ *[0-9a-f]*:/p' | sed 's/^ *[0-9a-f]*:\s*//' | sed 's/\s\+/ /g'; }
rows $S/tgt/$T.o > $D/t.rows; rows $D/r.o > $D/r.rows
diff -y -W 130 $D/t.rows $D/r.rows | grep -n -E '\s[|<>]\s|\s<$|\s>$' | head -${N:-40}
echo "rows: want $(wc -l < $D/t.rows) got $(wc -l < $D/r.rows)"
rm -rf $D
