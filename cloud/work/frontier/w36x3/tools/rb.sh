#!/bin/sh
# usage: rb.sh DIR FN[,FN2] [rscore args]  -- batch aligned-row scoring on the builder (w36x3 scratch)
dir="$1"; fns="$2"; shift 2
b=$(basename "$dir"); S=rush2049/scratch/frontier/w36x3
here=$(cd "$(dirname "$0")" && pwd)
ssh watchman2 "rm -rf ~/$S/vb/$b && mkdir -p ~/$S/vb/$b" || exit 1
scp -q "$here/rscore.py" "watchman2:$S/vb/rscore.py" || exit 1
[ -n "$CTX" ] && { ssh watchman2 "rm -rf ~/$S/vb/ctx_$b && mkdir -p ~/$S/vb/ctx_$b"; scp -q $CTX/*.c "watchman2:$S/vb/ctx_$b/"; set -- "$@" --ctx "vb/ctx_$b"; }
tar -C "$dir" -cf - --exclude='*.txt' --exclude='*.tsv' . | ssh watchman2 "tar -C ~/$S/vb/$b -xf -" || exit 1
args=""; for x in "$@"; do args="$args '$(printf %s "$x" | sed "s/'/'\\\\''/g")'"; done
ssh watchman2 "cd ~/$S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido nice python3 vb/rscore.py vb/$b $fns $args" | tee "$dir/rows.txt"
