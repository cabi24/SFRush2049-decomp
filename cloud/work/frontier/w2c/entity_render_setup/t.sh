#!/bin/sh
# usage: t.sh BODY.c [udiff args] -- head.h + BODY scored in the whole-program unit, aligned diff
cd /home/cburnes/projects/rush2049-decomp
d=cloud/work/frontier/w2c/entity_render_setup
(echo "/* flags: -g0 -O3 -mips2 -G 0 -non_shared */"; cat $d/head.h; cat $d/$1) > $d/_t.c
shift
cloud/work/frontier/w2c/us.sh $d/_t.c entity_render_setup "$@"
