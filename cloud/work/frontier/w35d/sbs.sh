#!/bin/bash
# usage: sbs.sh group groups/DIR NAME   |  sbs.sh fn FILE.c NAME [flags]
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w35d
B=watchman2:rush2049/scratch/frontier/w35d
SP=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/4b393cd0-4116-4767-90e0-4ddf67d9ff98/scratchpad
rsync -aq --exclude 'v/' --exclude '*.pyc' $L/ $B/lane/ || exit 1
IDO='IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido'
if [ "$1" = group ]; then
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w35d && $IDO python3 lane/gotwords.py group lane/$2 $3 lane_words.json" || exit 1
else
  FL="${4:--g0 -O3 -mips2 -G 0 -non_shared}"
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w35d && $IDO python3 lane/gotwords.py fn lane/$2 $3 lane_words.json '$FL'" || exit 1
fi
scp -q watchman2:rush2049/scratch/frontier/w35d/lane_words.json $SP/words.json
python3 $L/sbsdiff.py $SP/words.json "$3" "${SBS_ARGS}"
