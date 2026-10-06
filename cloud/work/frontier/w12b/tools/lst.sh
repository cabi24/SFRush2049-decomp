#!/bin/sh
# lst.sh LABEL FUNC : snapshot last w12b unit build, print FUNC's ugen listing (no .loc) to stdout
cd /home/cburnes/projects/rush2049-decomp
export TAG=w12b; T=cloud/work/frontier/tools/trace
$T/snap.sh $1 >/dev/null 2>&1; $T/as1t.sh $1 $2 >/dev/null 2>&1
grep -v "\.loc" build/trace/w12b/fn_$1.$2.s
