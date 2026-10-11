#!/bin/sh
# vt.sh LABEL BODY.c : prefix+body -> ctrace (key webs 85/156/162/515/146) + unit rows (TAG=w18d)
cd /home/cburnes/projects/rush2049-decomp
export TAG=w18d; T=cloud/work/frontier/tools/trace; D=cloud/work/frontier/w18d/camera_play_script
f=$D/v/$1_full.c; cat $D/prefix.c $2 > $f
$T/ctrace.sh camera_play_script $1 $f > $D/v/ct_$1.txt 2>&1
r=$($T/us.sh camera_play_script $f 2>&1 | tail -1 | grep -o "Error.*\|words.*")
k=$(grep -E 'p1 w(85|156|162|515) ' $D/v/ct_$1.txt | awk '{printf "%s:%s:%s ", $2, substr($3,6), $NF=="" ? "" : $0 ~ /->/ ? $(NF-3) : ""}')
echo "$1: $r | $k"
