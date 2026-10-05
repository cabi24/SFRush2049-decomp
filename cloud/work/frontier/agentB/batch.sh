#!/bin/sh
# usage: batch.sh DIR NAME [FLAGS] -- score every DIR/*.c on the builder (agentB copy) with near.py (aligned + strict), one line each
FL="${3:--g0 -O2 -mips2 -G 0 -non_shared}"
ssh -o BatchMode=yes watchman2 "rm -rf ~/rush2049/scratch/frontier/agentB/batch/$2; mkdir -p ~/rush2049/scratch/frontier/agentB/batch/$2"
tar -C "$1" -cf - . | ssh -o BatchMode=yes watchman2 "tar -C ~/rush2049/scratch/frontier/agentB/batch/$2 -xf -"
ssh -o BatchMode=yes watchman2 "cd ~/rush2049/scratch/frontier/agentB && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; ls batch/$2/*.c | xargs -P 4 -I{} sh -c 'echo \"\$(basename {}) \$(python3 cloud/work/bigfish/near.py {} $2 --flags \"$FL\" 2>&1 | tail -1 | sed -e \"s/^[^;]*: want/want/\" -e \"s/ flags:.*//\")\"' | sort -t' ' -k12 -n -r"
