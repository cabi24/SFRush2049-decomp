#!/bin/sh
# mkg.sh NAME : assemble group NAME.c (accepted sound ctx), NAME_b.c (slot ctx), NAME_c.c (credits), NAME_d.c (timer)
cd /home/cburnes/projects/rush2049-decomp/cloud/work/s20261004/E
N=$1
cp ../../../../src/blob/groups/codex_sound_channel_extra/group.c $N.c
cp src/ctx_slot.c ${N}_b.c
cat src/common.h src/data_decls.h src/helper.h ${CRED:-src/credits.c} > ${N}_c.c
cat src/common.h src/data_decls.h src/helper.h ${TIMER:-src/timer.c} > ${N}_d.c
[ -n "$EXTRA_E" ] && cp $EXTRA_E ${N}_e.c
[ -n "$EXTRA_F" ] && cp $EXTRA_F ${N}_f.c
true
