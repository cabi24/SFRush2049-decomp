#!/bin/bash
# ida.sh BODY.c... : hdr_ipcorder.h + body -> unit score of input_deadzone_apply with w1g_part_ipcorder.c (tag w12a)
R=/home/cburnes/projects/rush2049-decomp; W=$R/cloud/work/frontier/w12a; export TAG=w12a
for b in "$@"; do
  mkdir -p $W/_u; out=$W/_u/$(basename $b); cat $W/ida/hdr_ipcorder.h > $out; cat $b >> $out
  UARGS="--with $W/ida/w1g_part_ipcorder.c" $R/cloud/work/frontier/tools/trace/us.sh input_deadzone_apply $out | sed "s|^|$(basename $b) |"
done
