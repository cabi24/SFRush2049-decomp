#!/bin/sh
# w23b (Pi): 5.3 traced-uopt colouring summary (callee cost + address/const webs) of lane-relative SRC FN
# usage: sh tools/cost53.sh SRC FN LABEL
cd /home/cburnes/projects/rush2049-decomp
W=cloud/work/frontier/w23b; export TAG=w23b; T=cloud/work/frontier/tools/trace
sh $W/tools/pp.sh $1 $2 $W/$2/v/pp_$3.c >/dev/null && $T/snap.sh $3 $W/$2/v/pp_$3.c $2 >/dev/null 2>&1 && $T/ctrace.sh $2 $3 2>&1 | grep -E "${PAT:-type=1|split|-> s}" | sed 's/raw10=.*//' 
