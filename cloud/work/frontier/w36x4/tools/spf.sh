#!/bin/bash
# spf.sh FILE FUNC : standalone got sp map vs want sp map
F=$1; N=$2
T=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad
/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36x4/tools/fs.sh $F $N --got $N:0:3000 > $T/sp_got.txt 2>&1
cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/tools/tdis.py $N | awk 'NR>1{$1=""; print NR-2" "$0}' | sed 's/^\([0-9]*\)  */\1 /' > $T/sp_want.txt
python3 cloud/work/frontier/w36x4/tools/spmap.py $T/sp_want.txt > $T/spw.txt
python3 cloud/work/frontier/w36x4/tools/spmap.py $T/sp_got.txt > $T/spg.txt
paste -d'|' <(awk '{printf "%-40s\n",$0}' $T/spw.txt) $T/spg.txt
