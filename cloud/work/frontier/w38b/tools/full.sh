#!/bin/sh
# full.sh PACKET_DIR ovl_a|ovl_b : score plain, score --claims, object order; saves to PACKET_DIR/../../scores/<name>.txt
set -e
D="$1"; IMG="$2"
T=$(dirname "$0")
N=$(basename "$D")
OUT=$(dirname "$0")/../scores; mkdir -p "$OUT"
LABEL=$(echo "$D" | sed 's#/*$##; s#^.*cloud/work/frontier/w38b/##; s#/#__#g')
{
echo "### score.py group cand/$LABEL --targets asm/us/$IMG"
"$T/rg.sh" "$D" "$IMG"
echo "### score.py group cand/$LABEL --targets asm/us/$IMG --claims"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w38b && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group cand/$LABEL --targets asm/us/$IMG --claims; echo exit=\$?"
echo "### object order (objsyms.py)"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w38b && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 objsyms.py cand/$LABEL $IMG /tmp/w38b_full.o"
} > "$OUT/$N.txt" 2>&1
grep -E "^func_|^  [A-Z0-9]|^exit=|^###|STUB|^[0-9a-f]{6} " "$OUT/$N.txt" | grep -v "^    " | sed 's/(\([0-9]* section-relative relocations unverified\)[^)]*)/(\1)/' | cut -c1-140
