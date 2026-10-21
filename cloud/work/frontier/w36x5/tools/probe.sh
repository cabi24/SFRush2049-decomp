#!/bin/sh
# usage (builder): probe.sh FILE.c -- uopt block count of entity_ai_pathfind + whether case 0 forwards gSong
S=$HOME/rush2049/scratch/frontier/w36x5
n=$(sh $S/uv.sh "$1" entity_ai_pathfind | grep -o 'entity_ai_pathfind([0-9]*)')
sh $S/gdis.sh "$1" entity_ai_pathfind entity_ai_pathfind >/dev/null
f=$(grep -B6 'R_MIPS_26	func_800979A0' $S/gdwork/dis.txt | grep -c 'lw	a0,0(a0)')
w=$(grep -c '^ *[0-9a-f]*:	[0-9a-f]\{8\}' $S/gdwork/dis.txt)
if [ "$f" = "0" ]; then fw=FWD; else fw=NOFWD; fi
echo "$(basename $1): $n $fw words=$w"
cd $S/uvwork && W5D_LEVEL=3 W5D_PROC=entity_ai_pathfind W5D_OUT=$PWD/w5d ../bin/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.p -t st -l list > uopt3.log 2>&1
echo "   bits(max)=$(grep -o 'bit=[0-9]*' w5d | cut -d= -f2 | sort -n | tail -1) cm=$(grep -c 'bit at=cm' w5d) gc=$(grep -c 'bit at=gc' w5d)"
