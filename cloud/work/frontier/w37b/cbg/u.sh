#!/bin/sh
c=$(readlink -f $1); shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w37b score particle_lifetime_set --with $c --internal ZOID_ShowPoly_bg "$@" 2>&1 | grep -v "defined by" | cut -c1-250 | head -${N:-12}
