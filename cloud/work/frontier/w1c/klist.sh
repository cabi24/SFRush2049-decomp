#!/bin/sh
# usage: klist.sh file.c [flags] -- ugen listing (pre-as1) from cc -K on the builder, .loc stripped
src="$1"; shift
FL="${*:--g0 -O3 -mips2 -G 0 -non_shared}"
scp -q "$src" watchman2:rush2049/scratch/frontier/w1c/cand/k/x.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w1c/cand/k && rm -f u.out.* x.[A-Zs] x.o && TK=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 && \$TK/ido/cc -c -K $FL -Wab,-r4300_mul -o x.o x.c && cat u.out.s x.s 2>/dev/null | grep -v '^[[:space:]]*\.loc'"
