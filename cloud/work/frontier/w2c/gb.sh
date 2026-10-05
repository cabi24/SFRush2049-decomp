#!/bin/sh
# usage: gb.sh DIR NAME [bscore args] -- DIR/*.c are single-file groups (slot_sound + candidate); score NAME on the builder
dir="$1"; name="$2"; shift; shift
KEEP=slot_value_get,display_list_alloc,player_state_get,slot_deactivate,mode_byte2_set,object_type_byte2_get,object_type_byte3_get,mode_byte_set,$name
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w2c/cand/b_$name; mkdir -p ~/rush2049/scratch/frontier/w2c/cand/b_$name"
tar -C "$dir" -cf - . | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/w2c/cand/b_$name -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w2c && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 cand/bscore.py cand/b_$name $name --flags '-g0 -O3 -mips2 -G 0 -non_shared' --keep $KEEP $*"
