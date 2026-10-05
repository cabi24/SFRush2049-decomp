#!/bin/sh
# pp.sh file.c [-Dflags] -> preprocess to scratch x.c and aligned-diff; prints count
f=$1; shift
cpp -P "$@" $f | grep -v '^#pragma GCC' > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/x.c; sed -i '1i float fabsf(float);\n#pragma intrinsic (fabsf)' /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/x.c; $(dirname $0)/f.sh /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/x.c 1 --all
