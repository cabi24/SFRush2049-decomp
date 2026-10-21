#!/bin/bash
# usage: vs.sh BASEDIR VARDIR NAMES 'old==>new' ...   -> make variant, print per-name summary lines (trimmed)
B=$1; V=$2; N=$3; shift 3
python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36h/tools/vt.py $B $V "$@" || exit 1
/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36h/tools/g.sh $V --names $N 2>&1 | cut -c1-90
