#!/bin/sh
# vb2.sh DIR NAME [args]: like vbatch.sh for lane w14oa, but uses bsc2.py (prints differing offsets/words)
dir="$1"; name="$2"; shift 2
here=$(cd "$(dirname "$0")" && pwd)
b=$(basename "$dir"); S="rush2049/scratch/frontier/w14oa"
ssh watchman2 "rm -rf ~/$S/vb/$b && mkdir -p ~/$S/vb/$b" || exit 1
scp -q "$here/bsc2.py" "watchman2:$S/vb/bsc2.py" || exit 1
tar -C "$dir" -cf - --exclude='*.txt' --exclude='*.tsv' . | ssh watchman2 "tar -C ~/$S/vb/$b -xf -" || exit 1
args=""; for x in "$@"; do args="$args '$(printf %s "$x" | sed "s/'/'\\\\''/g")'"; done
ssh watchman2 "cd ~/$S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 vb/bsc2.py vb/$b $name $args" | tee "$dir/score.txt"
