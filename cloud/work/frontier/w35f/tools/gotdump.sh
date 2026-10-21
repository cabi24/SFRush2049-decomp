#!/bin/bash
# usage: gotdump.sh GROUPDIR FUNC OUTFILE   (dumps our compiled FUNC rows, "idx text")
D=$1; F=$2; O=$3
/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35f/tools/g.sh $D --names $F --got $F:0:6000 2>&1 | grep -E '^ *[0-9]+ [a-z]' > $O
wc -l $O
