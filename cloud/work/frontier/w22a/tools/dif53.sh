#!/bin/sh
# w22a (builder): dif.sh but compiled with IDO 5.3 (diagnostic only)
TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
src=$1; fn=$2; d=$(mktemp -d)
python3 -c "
import sys; sys.path[:0]=['.']
from tools.conveyor.seeds import context as c
open('$d/r.c','w').write(c.reduced_tu(open('$src').read(),'$fn'))"
$TK/ido/cc -c -g0 -O2 -mips2 -G 0 -non_shared -I include -I include/PR -I $TK/shim -o $d/r.o $d/r.c || exit 1
o() { mips-linux-gnu-objdump -dr -j .text $1 | sed -n '/^ *[0-9a-f]*:/p' | sed 's/^ *[0-9a-f]*:\s*//' | sed 's/\s\+/ /g'; }
o tgt/$fn.o > $d/t; o $d/r.o > $d/m
diff -y -W 140 $d/t $d/m | ${PAGER_CMD:-cat}; rm -rf $d
