#!/bin/sh
# usage: gd.sh SRC.c FN [full.py args] -- aligned diff of FN compiled in a single-file -O3 group
# (keep list = the stand-in callers zz_caller,zz_caller2,camera_update_c,select_screen_update unless KEEP= is set)
src="$1"; name="$2"; shift; shift
K=${KEEP:-zz_caller,zz_caller2,camera_update_c,select_screen_update}
scp -q "$src" watchman2:rush2049/scratch/frontier/w8d/cand/_g_$name.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w8d && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/full.py cand/_g_$name.c $name --flags '-g0 -O3 -mips2 -G 0 -non_shared' --keep $K $*"
