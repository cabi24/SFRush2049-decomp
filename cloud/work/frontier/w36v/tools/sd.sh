#!/bin/sh
# usage: sd.sh cand.c NAME [udiff args...]   standalone -O3 compile on watchman2, aligned diff on the Pi
src="$1"; name="$2"; shift; shift
FLAGS=${FLAGS:-"-g0 -O3 -mips2 -G 0 -non_shared"}
b=$(basename "$src" .c)
OUT=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/sd
mkdir -p $OUT
scp -q "$src" watchman2:rush2049/scratch/frontier/w36v/cand/sd_$b.c || exit 1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36v/cand && rm -f sd_$b.o && \$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc -c $FLAGS -o sd_$b.o sd_$b.c > sd_$b.log 2>&1 || grep -i -B1 -A3 error sd_$b.log | head -30" 
scp -q watchman2:rush2049/scratch/frontier/w36v/cand/sd_$b.o $OUT/$b.o || exit 1
python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace/udiff.py $name --obj $OUT/$b.o "$@"
