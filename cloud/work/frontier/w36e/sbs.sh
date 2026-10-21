#!/bin/bash
# usage: sbs.sh group groups/DIR NAME   |  sbs.sh fn FILE.c NAME [flags]
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w36e
B=watchman2:rush2049/scratch/frontier/w36e
SP=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/w36e
rsync -aq --exclude 'v/' --exclude '*.pyc' $L/ $B/lane/ || exit 1
IDO='IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido'
if [ "$1" = group ]; then
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w36e && $IDO python3 lane/gotwords.py group lane/$2 $3 lane_words.json" || exit 1
else
  FL="${4:--g0 -O3 -mips2 -G 0 -non_shared}"
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w36e && $IDO python3 lane/gotwords.py fn lane/$2 $3 lane_words.json '$FL'" || exit 1
fi
scp -q watchman2:rush2049/scratch/frontier/w36e/lane_words.json $SP/words.json
python3 $L/sbsdiff.py $SP/words.json "$3" "${SBS_ARGS}"
