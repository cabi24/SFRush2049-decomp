#!/bin/sh
# usage: ulist.sh NAME CAND.c LABEL -- score NAME with CAND in the unit (tag w7b), then on the builder run ugen -l
# on the stage's opt (copied st) into ~/rush2049/scratch/frontier/w7b/ul_LABEL/{out.s,fn.s}, and the traced as1 -R
# on the stage's gen into ul_LABEL/as1r.log (object compared with the stage's unit.o). fn.s = NAME's listing.
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
name=$1; cand=$(cd $D; realpath $2); lab=$3
python3 -m tools.conveyor.pipeline.blob_unit --tag w7b score $name --with "$cand" 2>&1 | grep -E "EQUAL|FAIL" | head -1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w7b && rm -rf ul_$lab && mkdir ul_$lab && S=../unit/w7b/stage && T=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && cp \$S/opt \$S/st ul_$lab/ && cd ul_$lab && cp st st2 && \$T/ugen -G 0 -mips2 -EB -g0 -O3 opt -o gen -l out.s -t st -temp ugtmp >/dev/null 2>&1; awk '/^\t.ent\t$name /,/^\t.end\t$name\$/' out.s > fn.s; ../as1/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 -R gen -o r.o -t st2 > as1r.log 2>&1; cmp r.o ../../unit/w7b/stage/unit.o && echo as1-identical; wc -l fn.s"
