#!/bin/sh
# Run on the builder inside a scratch copy (see README.md). Scores the three
# blocked natural-literal sources and their deliberately wrong copies with the
# scorer given as $1 (default tools/cloud/score.py, i.e. the patched copy).
SCORER=${1:-tools/cloud/score.py}
export IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
FLAGS="-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul"
D=cloud/work/frontier/rodata/cand
for n in camera_blend_between func_800DE860 func_800EC270 random_int; do
    for f in "$D/$n.c" "$D/wrong_$n.c"; do
        [ -f "$f" ] || continue
        echo "== $f"
        python3 "$SCORER" fn "$f" "$n" --flags "$FLAGS"
        echo "exit=$?"
    done
done
