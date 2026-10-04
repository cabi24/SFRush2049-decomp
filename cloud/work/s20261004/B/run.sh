#!/bin/bash
# usage: run.sh <groupdir-name>  (syncs dir to watchman2 scratch and scores)
D=$1; S=rush2049/scratch/s20261004_B
ssh watchman2 "mkdir -p ~/$S/work/$D"; rsync -a /home/cburnes/projects/rush2049-decomp/cloud/work/s20261004/B/$D/ watchman2:~/$S/work/$D/
ssh watchman2 "cd ~/$S && SHOW=${SHOW:-12} IDO_DIR=~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group work/$D ${@:2}"
