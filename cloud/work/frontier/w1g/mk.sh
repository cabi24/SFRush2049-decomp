#!/bin/bash
# mk.sh <group> <partsdir> [gf args]: concatenates parts into groups/<group>/group.c and runs gf.sh
g=$1; d=$2; shift; shift
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w1g
cat $d/*.c > groups/$g/group.c
[ $# -gt 0 ] && ./gf.sh $g "$@"
