#!/bin/sh
# usage (builder): gdis.sh FILE.c KEEP[,KEEP] FUNC -- single-file -O3 group (as score.py group), disassemble FUNC
S=$HOME/rush2049/scratch/frontier/w36x5
TK=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5
D=$S/gdwork; rm -rf $D; mkdir -p $D; cp "$1" $D/f.c
python3 - "$D" "$2" <<'PY'
import json,sys
d,keep=sys.argv[1],sys.argv[2].split(',')
json.dump({"members":keep,"files":["f.c"],"keep":keep,"flags":"-g0 -O3 -mips2 -G 0 -non_shared"},open(d+'/group.json','w'))
PY
cd $S && IDO_DIR=$TK/ido python3 -c "
import sys; sys.path.insert(0,'tools/cloud'); import score
from pathlib import Path
score.compile_group(Path('$D'), Path('$D/out.o'))
" || exit 1
LD_LIBRARY_PATH=$TK/lib $TK/bin/objdump -d --disassemble=$3 -M no-aliases $D/out.o 2>/dev/null | sed -n '1,2000p' > $D/dis.txt || LD_LIBRARY_PATH=$TK/lib $TK/bin/objdump -d $D/out.o > $D/dis.txt
wc -l < $D/dis.txt
