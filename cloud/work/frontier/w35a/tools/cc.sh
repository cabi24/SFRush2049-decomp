#!/bin/bash
# cc.sh <local.c> [flags]: compile on builder, print objdump -dr
F=$1; shift
FL=${@:--g0 -O3 -mips2 -G 0 -non_shared}
scp -q $F watchman2:rush2049/scratch/frontier/w35a/cand/_t.c
ssh watchman2 "cd ~/rush2049/scratch/frontier/w35a/cand && \$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido/cc -c $FL -Wab,-r4300_mul -o _t.o _t.c 2>&1 | head -20; mips-linux-gnu-objdump -dr -M gpr-names=32 _t.o | sed -n '7,200p'"
