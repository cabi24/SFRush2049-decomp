#!/bin/bash
# odump.sh groups/DIR NAME : compile group on builder and print objdump of NAME (relocs inline)
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w36x2
rsync -aq --exclude 'dis/' --exclude '*.pyc' --exclude 'v/' $L/ watchman2:rush2049/scratch/frontier/w36x2/lane/ || exit 1
P=lane/$1; case "$1" in v/*) P=$1;; esac
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36x2 && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 -c \"
import sys; sys.path.insert(0,'tools/cloud'); import score
from pathlib import Path
score.compile_group(Path('$P'), Path('/tmp/w36x2_od.o'))
\" && mips-linux-gnu-objdump -d -r --no-show-raw-insn /tmp/w36x2_od.o | awk '/<$2>:/{p=1;next} p&&/^[0-9a-f]+ <.*>:/{exit} p'"
