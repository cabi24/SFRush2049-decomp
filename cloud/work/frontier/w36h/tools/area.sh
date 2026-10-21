#!/bin/bash
# usage: area.sh FILE.c [FN]  -> stage groups/ovlb with FILE as shots.c; print cfe area, spill offsets, callee cost, frame/score
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36h
F=$1; N=${2:-func_8038FCE0}
L=ar$$
D=$W/v/$L; rm -rf $D; cp -r $W/groups/ovlb $D; cp $F $D/shots.c
$W/tools/tr.sh stage $D $L >/dev/null
echo "$(basename $F): $(ssh watchman2 "sh ~/rush2049/scratch/frontier/w36h/tk/tk.sh ~/rush2049/scratch/frontier/w36h $L spill $N" 2>&1 | grep -E '^AREA f_readnxtinst' | tail -1 | grep -o 'area=[0-9]*') cost=$(ssh watchman2 "sh ~/rush2049/scratch/frontier/w36h/tk/tk.sh ~/rush2049/scratch/frontier/w36h $L cdx $N" 2>&1 | grep -m1 'kind=callee' | grep -o 'cost=[0-9.]*' | cut -d= -f2) $($W/tools/g.sh $D --names $N 2>&1 | head -1 | sed 's/.*ALIGNED/ALIGNED/')"
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w36h/st_$L"; rm -rf $D
