#!/bin/sh
# usage: vbatch.sh LANE DIR NAME [bscore args: --flags "..." --top N --keep a,b --jobs 2 --sort strict|mnem]
# Copies DIR/*.c to watchman2:~/rush2049/scratch/frontier/LANE/vb/<DIR basename>/, scores every file as
# function NAME with bscore.py (best first), and saves the ranking to DIR/score.txt on the Pi.
# The lane scratch must already exist (copied from base, with tools/cloud synced).
lane="$1"; dir="$2"; name="$3"; shift 3
here=$(cd "$(dirname "$0")" && pwd)
b=$(basename "$dir")
S="rush2049/scratch/frontier/$lane"
ssh watchman2 "rm -rf ~/$S/vb/$b && mkdir -p ~/$S/vb/$b" || exit 1
scp -q "$here/bscore.py" "watchman2:$S/vb/bscore.py" || exit 1
tar -C "$dir" -cf - --exclude='*.txt' --exclude='*.tsv' . | ssh watchman2 "tar -C ~/$S/vb/$b -xf -" || exit 1
args=""; for x in "$@"; do args="$args '$(printf %s "$x" | sed "s/'/'\\\\''/g")'"; done
ssh watchman2 "cd ~/$S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 vb/bscore.py vb/$b $name $args" | tee "$dir/score.txt"
