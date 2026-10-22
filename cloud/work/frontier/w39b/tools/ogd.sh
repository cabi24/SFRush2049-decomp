#!/bin/sh
# ogd.sh ovl_a|ovl_b DIR FN [udiff args]: compile group DIR on the builder (objsyms.py), fetch the object, aligned diff
# of FN against the overlay target (udiff.py logic with ASM_DIR = asm/us/<img>). w39b helper.
IMG="$1"; D="$2"; FN="$3"; shift 3
R=rush2049/scratch/frontier/w39b
LABEL=$(echo "$D" | sed 's#/*$##; s#^.*cloud/work/frontier/w39b/##; s#/#__#g')
SP=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/be6c3ef7-ec10-405a-a401-0caaa1bc1bac/scratchpad
ssh watchman2 "rm -rf ~/$R/cand/$LABEL && mkdir -p ~/$R/cand/$LABEL" && scp -q "$D"/* watchman2:$R/cand/$LABEL/
ssh watchman2 "cd ~/$R && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 objsyms.py cand/$LABEL $IMG /tmp/w39b_ogd.o >/dev/null" || exit 1
scp -q watchman2:/tmp/w39b_ogd.o $SP/ogd_$LABEL.o
cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/frontier/w39b/tools/oudiff.py $IMG $FN --obj $SP/ogd_$LABEL.o "$@"
