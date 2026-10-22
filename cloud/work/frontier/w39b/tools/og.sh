#!/bin/sh
# og.sh ovl_a|ovl_b DIR [FN]: copy group DIR to the builder (cand/<label>) and run score.py group; print summary lines
# (and FN's verdict). w39b helper; serial use only.
IMG="$1"; D="$2"; FN="$3"
R=rush2049/scratch/frontier/w39b
LABEL=$(echo "$D" | sed 's#/*$##; s#^.*cloud/work/frontier/w39b/##; s#/#__#g')
ssh watchman2 "rm -rf ~/$R/cand/$LABEL && mkdir -p ~/$R/cand/$LABEL" && scp -q "$D"/* watchman2:$R/cand/$LABEL/
ssh watchman2 "cd ~/$R && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/$LABEL --targets asm/us/$IMG" > /tmp/claude-og-$LABEL.txt 2>&1
echo "rc=$? $LABEL"
grep -E "^[A-Za-z_0-9]+:$|^  [A-Z]" /tmp/claude-og-$LABEL.txt | paste - - | sed 's/(\([0-9]* section-relative relocations unverified\)[^)]*)/(\1)/' | cut -c1-150
