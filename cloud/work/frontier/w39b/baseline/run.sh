#!/bin/sh
# Re-score each prior best in the current unit (w39b baseline). Serial, tag w39b.
cd /home/cburnes/projects/rush2049-decomp
export TAG=w39b SCR=rush2049/scratch/frontier/w39b
T=cloud/work/frontier/tools/trace
F=cloud/work/frontier
run() { n=$1; f=$2; shift 2; echo "## $n $f UARGS=$*"; UARGS="$*" sh $T/us.sh $n $F/$f 2>&1 | tail -1; }
run func_8010D680 w36w/func_8010D680/best.c
run func_8010D3C0 w36w/func_8010D3C0/best.c
run race_position_update w36w/race_position_update/best.c
run object_bytes_sum_global w36w/text/best_object_bytes_sum_global.c
run object_bytes23_sum w38a/tb/d1.c
run state_utility w38a/tbdraw/t1.c
run physics_float_calc w36x3/physics_float_calc/best.c
run particle_lifetime_set w37b/cbg/best.c
run func_8010C974 w36s/func_8010C974/best.c
run pak_delete_file_retry w36v/pak_delete_file_retry/best.c --internal func_800A1E94
run controls_animText w36k/anim/best.c
run controls_animButtonText w36k/anim/v/b3.c
run func_8010F218 w33b/videoscr/best.c
run func_8010EA08 w33b/videoscr/best.c
run CreatePlayer w32c/CreatePlayer/lead_f2.c
