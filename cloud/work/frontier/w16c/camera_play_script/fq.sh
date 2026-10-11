#!/bin/sh
# fq.sh LABEL BODY.c [udiff args] : q.sh, then force the -1 web to t5 on that snapshot
cd /home/cburnes/projects/rush2049-decomp
export TAG=w16c; T=cloud/work/frontier/tools/trace; D=cloud/work/frontier/w16c/camera_play_script
l=$1; b=$2; shift 2
$D/q.sh $l $b
w=$(grep 'raw10=0xffffffff' $D/v/ct_$l.txt | grep '^p1' | head -1 | awk '{print $2}')
$T/force.sh $l camera_play_script "p1:$w=c12" ${@:---norm} > $D/v/f_$l.txt 2>&1
tail -1 $D/v/f_$l.txt
