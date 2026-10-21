#!/bin/bash
# run.sh DIR... : compile each group on builder, print function sizes
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36e || exit 1
rsync -aq inl/ watchman2:rush2049/scratch/frontier/w36e/inl/
for d in "$@"; do
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36e && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 -c \"
import sys; sys.path.insert(0,'tools/cloud')
import score
from pathlib import Path
score.compile_group(Path('inl/$d'), 'inl/$d.o')
\" && mips-linux-gnu-nm -S -n inl/$d.o 2>/dev/null | python3 -c 'import sys; print(\" \".join(f\"{p[3]}:{int(p[1],16)}\" for p in (l.split() for l in sys.stdin) if len(p)==4 and p[2]==\"T\"))'; echo" | sed "s/^/$d: /"
done
