#!/bin/sh
# usage: run.sh FILE.c [LABEL]  -- unit score SetupWaveModel (+func_80097164) and print key colouring webs
f=$(realpath $1); cd /home/cburnes/projects/rush2049-decomp
export TAG=w36x5
T=cloud/work/frontier/tools/trace
lab=${2:-$(basename $1 .c)}
UARGS="--internal SetupWaveModel --internal func_80097164 $XARGS" $T/us.sh SetupWaveModel $f 2>&1 | tail -1
python3 $T/udiff.py func_80097164 --summary 2>&1 | sed 's/^/  ReadWave: /'
if [ -n "$CT" ]; then
  $T/snap.sh $lab >/dev/null 2>&1
  $T/ctrace.sh SetupWaveModel $lab 2>&1 | grep -E "raw10=0x0000000[04] |-> s[0-8] |-> t0 " | awk '{print "  ",$2,$3,$4,$5,$7,$8,$9,$10,$11}'
fi
