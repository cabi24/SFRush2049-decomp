#!/bin/sh
# synk.sh file.c: -O3 -K compile on builder; print frames and merged Mmt layout of every proc
T=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad
f=$1; b=$(basename $f .c)
ssh watchman2 "mkdir -p ~/rush2049/scratch/frontier/w33a/syn/k_$b"
scp -q $f watchman2:rush2049/scratch/frontier/w33a/syn/k_$b/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w33a/syn/k_$b; IDO=~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; \$IDO/cc -K -c -O3 -mips2 -G 0 -non_shared -g0 $b.c -o $b.o 2>&1 | grep -v warn | head -3; mips-linux-gnu-objdump -d $b.o | awk '/^[0-9a-f]+ <.*>:/{n=\$2} /addiu\tsp,sp,-/{print n, \$NF}'"
scp -q watchman2:rush2049/scratch/frontier/w33a/syn/k_$b/u.out.M $T/syn/$b.M
python3 $T/tools/mmt.py $T/syn/$b.M ${2:-}
