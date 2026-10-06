#!/bin/sh
# usage: ugu.sh NAME CAND.c [blob_unit args] -- unit score (tag w11d), then traced ugen on the unit stage;
# prints NAME's free-list events (proc ordinal from .ent order) interleaved with nothing; listing in $OUT/fn.s
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
name=$1; cand=$(cd $D; realpath $2); shift; shift
OUT=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/w11d/ugu_$name; mkdir -p $OUT
python3 -m tools.conveyor.pipeline.blob_unit --tag w11d score $name "$@" --with "$cand" 2>&1 | grep -E "(EQUAL|FAIL) $name" | head -1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w11d && rm -rf ugu && mkdir ugu && cp ../unit/w11d/stage/opt ../unit/w11d/stage/st ugu/ && cd ugu && DKWB_UGEN_TRACE=1 ../ugen/ugen -G 0 -mips2 -EB -g0 -O3 opt -o gen -l out.s -t st -temp ugtmp > trace.txt 2>&1; grep -v DKWB-CALL trace.txt | grep DKWB > ev.txt; grep -n '^	.ent	' out.s | cut -f3 | cut -d' ' -f1 > ents.txt; awk '/^\t.ent\t$name /,/^\t.end\t$name\$/' out.s > fn.s"
scp -q watchman2:rush2049/scratch/frontier/w11d/ugu/ev.txt watchman2:rush2049/scratch/frontier/w11d/ugu/ents.txt watchman2:rush2049/scratch/frontier/w11d/ugu/fn.s $OUT/
p=$(grep -n -x "$name" $OUT/ents.txt | head -1 | cut -d: -f1); p=$((p-1))
echo "proc ordinal $p"
awk -v p=$p '/PROC BEGIN/{split($3,a,"="); cur=a[2]} cur==p' $OUT/ev.txt > $OUT/fnev.txt
wc -l $OUT/fnev.txt
