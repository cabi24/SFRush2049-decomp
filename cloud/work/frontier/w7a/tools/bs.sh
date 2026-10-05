#!/bin/sh
# usage: bs.sh NAME [bscore args] FILE.c... -- bscore a set of candidates on the builder (w7a scratch)
name=$1; shift
args=""
while [ "$#" -gt 0 ]; do case "$1" in *.c) break;; *) args="$args '$1'"; shift;; esac; done
d=b_$$
ssh watchman2 "mkdir -p ~/rush2049/scratch/frontier/w7a/cand/$d"
for f in "$@"; do cat "$f" | ssh watchman2 "cat > ~/rush2049/scratch/frontier/w7a/cand/$d/$(basename $f)"; done
ssh watchman2 "cd ~/rush2049/scratch/frontier/w7a && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/bscore.py cand/$d $name --flags '-g0 -O3 -mips2 -G 0 -non_shared' $args; rm -rf cand/$d"
