#!/bin/sh
# laforce.sh FILE.c... : for look_at_point variants: build, snapshot, find f (var -8) and r-expr FP webs, force f=f0 r=f2, print rows
cd /home/cburnes/projects/rush2049-decomp
export TAG=w12f; T=cloud/work/frontier/tools/trace; W=cloud/work/frontier/w12f
for f in "$@"; do
  b=$(basename $f .c); ff=$(cd $W/camera_look_at_point; realpath $f)
  $W/tools/fl.sh camera_look_at_point $ff | tail -1 | sed "s/^/$b natural: /"
  $T/snap.sh la_$b >/dev/null 2>&1
  ct=$($T/ctrace.sh camera_look_at_point la_$b 2>&1)
  wf=$(echo "$ct" | grep 'raw10=0xfffffff8' | grep '\$f' | awk '{print $2}')
  wr=$(echo "$ct" | grep '\$f' | grep 'type=4' | sort -t= -k2 -rn | head -1 | awk '{print $2}')
  echo "$b: f=$wf r=$wr"
  [ -n "$wf" ] && [ -n "$wr" ] && $T/force.sh la_$b camera_look_at_point "p1:$wr=c25,p1:$wf=c24" 2>&1 | grep -E "declined|want" | sed "s/^/$b forced: /"
done
