#!/bin/sh
scp -q "$1" watchman2:rush2049/scratch/frontier/w2g/cand/_s.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w2g && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/syms.py cand/_s.c"
