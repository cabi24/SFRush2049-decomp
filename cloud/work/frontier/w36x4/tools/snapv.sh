#!/bin/bash
# usage: snapv.sh LABEL BLAST.c  -> single-file group snapshot (blast.c + ZOID_SetObjectDef appended) for tracing
L=$1; F=$2
T=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad
cp $F $T/snap_$L.c
printf '\nvoid func_80090770(s16 index, u16 value)\n{\n    D_8012E700[index].index = value;\n}\n' >> $T/snap_$L.c
cd /home/cburnes/projects/rush2049-decomp && TAG=w36x4 cloud/work/frontier/tools/trace/snap.sh $L $T/snap_$L.c entity_physics_update,save_write_data,func_80090770
