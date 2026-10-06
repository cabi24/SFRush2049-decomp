#!/bin/bash
# eu.sh VARIANT.c... : header (hdr2.h) + body -> unit score of entity_update (tag w12a), one line per variant
R=/home/cburnes/projects/rush2049-decomp; W=$R/cloud/work/frontier/w12a; export TAG=w12a
for b in "$@"; do
  mkdir -p $W/_u; out=$W/_u/$(basename $b); cat $W/${HDR:-hdr2.h} > $out; cat $b >> $out
  UARGS="--with $W/w1g_part2.c" $R/cloud/work/frontier/tools/trace/us.sh entity_update $out | sed "s|^|$(basename $b) |"
done
