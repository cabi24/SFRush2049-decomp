#!/bin/bash
# gdiff.sh GROUPDIR FUNC [udiff args]: build group on watchman2 keeping intermediates, fetch g.o, aligned diff of FUNC
d=${1%/}; fn=$2; shift 2
n=$(basename $d)
/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36o/tools/gbuild.sh $d >/dev/null || exit 1
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w36o
scp -q watchman2:rush2049/scratch/frontier/w36o/gb/$n/g.o /home/cburnes/projects/rush2049-decomp/build/trace/w36o/g_$n.o
cd /home/cburnes/projects/rush2049-decomp && TAG=w36o python3 cloud/work/frontier/tools/trace/udiff.py $fn --obj build/trace/w36o/g_$n.o "$@"
