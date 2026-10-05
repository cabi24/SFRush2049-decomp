#!/bin/sh
# unit.sh SRC.c [names...] -- score knot members in the whole-program unit (tag w6a), all members internal
R=/home/cburnes/projects/rush2049-decomp
src=$(realpath $1); shift
N=${*:-render_display_list func_80099B30 particle_system track_collision_wall func_8009F058 func_8009C3F8}
cd $R && python3 -m tools.conveyor.pipeline.blob_unit --tag ${TAG:-w6a} score $N --with $src --internal render_display_list --internal func_80099B30 --internal particle_system --internal track_collision_wall --internal func_8009F058 --internal func_8009C3F8 $UEXTRA 2>&1 | grep -E "EQUAL|FAIL|differ in this unit|blob_unit score|rror"
