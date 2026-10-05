#!/bin/sh
# usage: gbatch.sh DIR NAME -> DIR contains g*/group.c+group.json; prints last score line per group
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/agentA/cand/g_$2; mkdir -p ~/rush2049/scratch/frontier/agentA/cand/g_$2"
tar -C "$1" -cf - . | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/agentA/cand/g_$2 -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/agentA && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; ls -d cand/g_$2/g* | xargs -P 4 -I{} sh -c 'echo \"\$(basename {}): \$(python3 tools/cloud/score.py group {} 2>&1 | tail -1)\"'"
