#!/bin/sh
# usage: vb.sh IMG DIR NAME [bscore args]   (overlay variant of vbatch.sh, lane w35e)
img="$1"; dir="$2"; name="$3"; shift 3
here=$(cd "$(dirname "$0")" && pwd)
b=$(basename "$dir")
S="rush2049/scratch/frontier/w35e"
ssh watchman2 "rm -rf ~/$S/vb/$b && mkdir -p ~/$S/vb/$b" || exit 1
scp -q "$here/bscore_o.py" "watchman2:$S/vb/bscore_o.py" || exit 1
tar -C "$dir" -cf - --exclude='*.txt' --exclude='*.tsv' . | ssh watchman2 "tar -C ~/$S/vb/$b -xf -" || exit 1
args=""; for x in "$@"; do args="$args '$(printf %s "$x" | sed "s/'/'\\\\''/g")'"; done
ssh watchman2 "cd ~/$S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 vb/bscore_o.py vb/$b $name --targets asm/us/ovl_$img $args" | tee "$dir/score.txt"
