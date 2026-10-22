#!/bin/sh
# ofd.sh ovl_a|ovl_b|blob FILE.c FN [oudiff args]: compile FILE.c standalone on the builder, aligned diff of FN.
IMG="$1"; F="$2"; FN="$3"; shift 3
R=rush2049/scratch/frontier/w39b
SP=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/be6c3ef7-ec10-405a-a401-0caaa1bc1bac/scratchpad
B=$(basename "$F" .c)
scp -q "$F" watchman2:$R/cand/ofd_$B.c
scp -q /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w39b/tools/ofc.py watchman2:$R/ofc.py
ssh watchman2 "cd ~/$R && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 ofc.py cand/ofd_$B.c /tmp/w39b_ofd.o" || exit 1
scp -q watchman2:/tmp/w39b_ofd.o $SP/ofd_$B.o
cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/frontier/w39b/tools/oudiff.py $IMG $FN --obj $SP/ofd_$B.o "$@"
