#!/bin/sh
# usage: [EXTRA=...] [GREP=regex] vrun.sh NAME FILE.c... -- pretrace each candidate (runs/<NAME>_<stem>) and print
# the unit score line plus the report rows matching GREP (default: additions of a constant).
D=$(pwd); R=/home/cburnes/projects/rush2049-decomp
name=$1; shift
for f in "$@"; do
  st=$(basename "$f" .c)
  echo "=== $st"
  OUT=$R/cloud/work/frontier/w5d/runs/${name}_$st $R/cloud/work/frontier/w5d/tools/pretrace.sh $name "$(cd $D; realpath $f)" ${name}_$st | grep -E "EQUAL|FAIL|rror"
  grep -A1 -E "${GREP:-^ *[0-9]+  add\.J\(var[A-Z]\([0-9]+-[0-9]+\)r?,1\)}" $R/cloud/work/frontier/w5d/runs/${name}_$st/report.txt | grep -v "^--"
done
