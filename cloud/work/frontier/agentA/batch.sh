#!/bin/sh
# usage: batch.sh DIR NAME [FLAGS] -> scores every DIR/v*.c (4 parallel), prints aligned-diff summary
F="${3:--g0 -O2 -mips2 -G 0 -non_shared}"
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/agentA/cand/b_$2; mkdir -p ~/rush2049/scratch/frontier/agentA/cand/b_$2"
tar -C "$1" -cf - . | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/agentA/cand/b_$2 -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/agentA && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; ls cand/b_$2/v*.c | xargs -P 4 -I{} sh -c 'echo \"\$(basename {}): \$(python3 cand/d.py {} $2 \"$F\" 2>&1 | tail -1)\"'"
