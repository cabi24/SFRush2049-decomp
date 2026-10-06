#!/bin/bash
# usage: us.sh BODY.c NAME... [blob_unit args] : hdr.h + body -> blob_unit score (tag w11b), with w1g's
# matched func_800AD4C8/func_800C3AD0/input_process_controller (w1g_part.c) added to the unit
W=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w11b
b=$1; shift
mkdir -p $W/_u; out=$W/_u/$(basename $b)
cat $W/${HDR:-hdr2.h} > $out; grep -v '#include "../hdr2.h"' $b >> $out
cd /home/cburnes/projects/rush2049-decomp && python3 -m tools.conveyor.pipeline.blob_unit --tag w11b score "$@" --with $out --with $W/${PART:-w1g_part2.c} 2>&1
