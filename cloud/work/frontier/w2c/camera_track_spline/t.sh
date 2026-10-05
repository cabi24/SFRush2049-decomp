#!/bin/sh
# usage: t.sh BODY.c [full.py args] -- head.h + BODY + stand-in caller, aligned diff of camera_track_spline
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w2c/camera_track_spline
b="$1"; shift
(echo "/* flags: -g0 -O3 -mips2 -G 0 -non_shared */"; cat head.h; cat "$b"; cat standin.c) > _t.c
../kfull.sh _t.c camera_track_spline camera_update "$@"
