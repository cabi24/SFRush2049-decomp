#!/bin/sh
# w26b builder: ugrid.sh DIR FN "OPS1" "OPS2" ... -> for each op list (uedit.py syntax, space separated), edit DIR/r.O,
# run traced ugen+as1 (urun.sh) and print the differing-row count vs tgt/FN.o
S=$HOME/rush2049/scratch/frontier/w27s; D=$1; FN=$2; shift 2
n=0; for ops in "$@"; do n=$((n+1)); E=$D/g$n; mkdir -p $E; cp $D/r.T $E/
  PYTHONPATH=$S/third_party/n64-decomp-workbench/src python3 $S/lane/tools/uedit.py $D/r.O $E/a.O $ops >/dev/null 2>&1 || { echo "$ops: uedit failed"; continue; }
  printf '%-40s %s\n' "$ops" "$(sh $S/lane/tools/urun.sh $E $E/a.O $FN 2>&1 | tail -1)"; done
