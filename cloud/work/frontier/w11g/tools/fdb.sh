#!/bin/sh
# usage: fdb.sh DIR NAME [full.py args] -- aligned-diff summary (rows) of every DIR/*.c on the builder (-O3), 2 jobs
dir="$1"; name="$2"; shift; shift
b=$(basename $dir)
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w11g/cand/b_$b" && scp -q -r "$dir" watchman2:rush2049/scratch/frontier/w11g/cand/b_$b && ssh watchman2 "cd ~/rush2049/scratch/frontier/w11g && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; ls cand/b_$b/*.c | xargs -P 2 -I{} sh -c 'echo \"{} \$(python3 tools/full.py {} $name --flags \"-g0 -O3 -mips2 -G 0 -non_shared\" $* 2>&1 | tail -1)\"'" | sed 's#cand/b_[^/]*/##' | sort -t';' -k2 -V
