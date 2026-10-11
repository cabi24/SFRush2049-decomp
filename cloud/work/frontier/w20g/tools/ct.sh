#!/bin/sh
# ct.sh FILE.c LABEL: unit build + snapshot + ctrace; print the decisions around the constant-5 web (model = type=4 nocs>=45)
f=$(realpath $1); L=$2
cd /home/cburnes/projects/rush2049-decomp; export TAG=w20g; T=cloud/work/frontier/tools/trace
$T/us.sh entity_spawn_init $f | cut -c1-120; $T/snap.sh $L >/dev/null 2>&1
$T/ctrace.sh entity_spawn_init $L 2>/dev/null | grep -n '' | grep -E 'type=2 raw10=0x00000005 |nocs=4[5-9] .*type=4 ' | head -4 | cut -c1-150
