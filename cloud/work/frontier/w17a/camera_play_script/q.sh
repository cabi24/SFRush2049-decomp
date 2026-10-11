#!/bin/sh
# q.sh LABEL BODY.c : build full file (prefix+body), unit score, colouring of key webs (TAG=w17a)
cd /home/cburnes/projects/rush2049-decomp
export TAG=w17a; T=cloud/work/frontier/tools/trace; D=cloud/work/frontier/w17a/camera_play_script
b=$2; f=$D/v/$1_full.c; cat $D/prefix.c $b > $f
$T/ctrace.sh camera_play_script $1 $f > $D/v/ct_$1.txt 2>&1
head -1 $D/v/ct_$1.txt | sed 's/^ *//' | cut -c1-90
$T/us.sh camera_play_script $f 2>&1 | tail -1 | grep -o "Error.*\|words.*"
grep -E 'raw10=0xffffffff|type=2 raw10=0x00000001 ' $D/v/ct_$1.txt | awk '{print $2,$3,$4,$5,$6,$8,$10}'
