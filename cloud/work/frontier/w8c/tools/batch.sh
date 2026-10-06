#!/bin/sh
# usage: batch.sh NAME FLAGS file.c... -- last-line aligned diff for each candidate (2 parallel)
name="$1"; flags="$2"; shift; shift
d=cand/b_$name
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w8c/$d; mkdir -p ~/rush2049/scratch/frontier/w8c/$d"
scp -q "$@" watchman2:rush2049/scratch/frontier/w8c/$d/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8c && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; ls $d/*.c | xargs -P2 -I{} sh -c 'echo \"{}: \$(python3 tools/full.py {} $name --flags \"$flags\" 2>&1 | tail -1)\"' | sort"
