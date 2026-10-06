#!/bin/sh
# vrun.sh FN file... : unit-score each variant, print rows/frame
for f in "$@"; do printf "%s: " $(basename $f); EXTRA="$EXTRA" /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w12f/tools/fl.sh $FN $f | tail -1; done
