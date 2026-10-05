#!/bin/sh
# usage: dump.sh file.c [flags]  -- compile on builder and objdump
src="$1"; shift
FL="${*:--g0 -O2 -mips2 -G 0 -non_shared}"
scp -q "$src" watchman2:rush2049/scratch/frontier/w1e/cand/_t.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w1e/cand && TK=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 && \$TK/ido/cc -c $FL -Wab,-r4300_mul -o _t.o _t.c && (LD_LIBRARY_PATH=\$TK/lib \$TK/bin/objdump -dr _t.o 2>/dev/null || mips-linux-gnu-objdump -dr _t.o)"
