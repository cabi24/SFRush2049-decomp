#!/bin/sh
# usage: batchg.sh DIR NAME GROUPJSON -- each DIR/*.c becomes group.c of an -O3 group (with GROUPJSON) scored on the builder (agentB copy)
ssh -o BatchMode=yes watchman2 "rm -rf ~/rush2049/scratch/frontier/agentB/batchg/$2; mkdir -p ~/rush2049/scratch/frontier/agentB/batchg/$2/src"
tar -C "$1" -cf - . | ssh -o BatchMode=yes watchman2 "tar -C ~/rush2049/scratch/frontier/agentB/batchg/$2/src -xf -"
scp -q "$3" watchman2:rush2049/scratch/frontier/agentB/batchg/$2/group.json
ssh -o BatchMode=yes watchman2 "cd ~/rush2049/scratch/frontier/agentB && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; ls batchg/$2/src/*.c | xargs -P 4 -I{} sh -c 'b=\$(basename {} .c); mkdir -p batchg/$2/g_\$b; cp {} batchg/$2/g_\$b/group.c; cp batchg/$2/group.json batchg/$2/g_\$b/; echo \"\$b \$(python3 tools/cloud/score.py group batchg/$2/g_\$b 2>&1 | tail -1)\"' | sort -k2 -n"
