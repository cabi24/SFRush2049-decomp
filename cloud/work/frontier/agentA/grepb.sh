#!/bin/sh
# usage: grepb.sh DIR NAME FLAGS PATTERN -> for each DIR/v*.c count candidate lines matching PATTERN (right column)
F="$3"
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/agentA/cand/b_$2; mkdir -p ~/rush2049/scratch/frontier/agentA/cand/b_$2"
tar -C "$1" -cf - . | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/agentA/cand/b_$2 -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/agentA && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; ls cand/b_$2/v*.c | xargs -P 4 -I{} sh -c 'python3 cand/d.py {} $2 \"$F\" > {}.out 2>&1; echo \"\$(basename {}): \$(grep -c \"$4\" {}.out) \$(tail -1 {}.out)\"'"
