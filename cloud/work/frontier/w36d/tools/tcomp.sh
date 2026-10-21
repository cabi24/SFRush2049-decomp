#!/bin/bash
# tcomp.sh DIR : compile a test group dir on the builder and print the disassembly
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36d || exit 1
rsync -aq $1/ watchman2:rush2049/scratch/frontier/w36d/tt/$1/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36d && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 -c \"
import sys; sys.path.insert(0,'tools/cloud'); import score
from pathlib import Path
score.compile_group(Path('tt/$1'), Path('tt/$1/o.o'))
\" && mips-linux-gnu-objdump -d -r tt/$1/o.o | grep -v '^$' | sed -n '1,400p'"
