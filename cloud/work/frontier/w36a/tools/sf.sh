#!/bin/bash
# usage: sf.sh DIR FN [FN..]  -- score group DIR, print the summary line of each FN
d=$1; shift
out=$(/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36a/tools/rg.sh $d)
for f in "$@"; do echo "$(basename $d) $f: $(echo "$out" | awk -v f="$f:" '$1==f{p=1;next} p&&/^  [^ ]/{print; exit}' | cut -c1-50)"; done
