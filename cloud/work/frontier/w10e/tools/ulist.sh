#!/bin/sh
# usage: ulist.sh NAME CAND.c LABEL -- score NAME with CAND in the unit (tag w10e), then ugen -l the stage's opt
# on the builder into ~/rush2049/scratch/frontier/w10e/ul_LABEL/fn.s (pre-as1 listing of NAME), printed here.
D=$(pwd); cd /home/cburnes/projects/rush2049-decomp
name=$1; cand=$(cd $D; realpath $2); lab=$3
python3 -m tools.conveyor.pipeline.blob_unit --tag w10e score $name --with "$cand" 2>&1 | grep -E "EQUAL|FAIL" | head -1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w10e && rm -rf ul_$lab && mkdir ul_$lab && S=../unit/w10e/stage && T=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && cp \$S/opt \$S/st ul_$lab/ && cd ul_$lab && \$T/ugen -G 0 -mips2 -EB -g0 -O3 opt -o gen -l out.s -t st -temp ugtmp >/dev/null 2>&1; awk '/^\t.ent\t$name /,/^\t.end\t$name\$/' out.s > fn.s; cat fn.s"
