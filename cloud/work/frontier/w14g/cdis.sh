#!/bin/sh
# cdis.sh FILE.c FN [FLAGS]: compile FILE.c on the builder (w14g scratch, standalone -O3), fetch the object, disassemble FN.
S=rush2049/scratch/frontier/w14g
F=${3:-"-g0 -O3 -mips2 -G 0 -non_shared"}
B=$(mktemp -d)
scp -q "$1" "watchman2:$S/cand_dis.c" || exit 1
ssh watchman2 "cd ~/$S && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 -c \"
import sys; sys.path.insert(0,'tools/cloud'); import score
score.compile_single('cand_dis.c', '$F', 'cand_dis.o')
\"" >/dev/null || exit 1
scp -q "watchman2:$S/cand_dis.o" "$B/o.o" && mips-linux-gnu-objdump -d -r -EB -M gpr-names=32 "$B/o.o" | awk -v fn="$2" '$0 ~ "<"fn">:" {p=1} p && /^$/ {exit} p'
rm -rf "$B"
