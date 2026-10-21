#!/bin/bash
# fs.sh FILE.c FN [flags] : push a single source and run score.py fn + aligned diff (image A)
F=$1; FN=$2; FL=${3:-"-g0 -O3 -mips2 -G 0 -non_shared"}
scp -q $F watchman2:rush2049/scratch/frontier/w36g/cand/$FN.c
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36g && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; python3 tools/cloud/score.py fn cand/$FN.c $FN --targets asm/us/ovl_a --flags '$FL' 2>&1 | tail -3; TARGETS=asm/us/ovl_a python3 adiff.py cand/$FN.c $FN --flags '$FL' --mode ${MODE:-norm} | tail -${TAILN:-40}"
