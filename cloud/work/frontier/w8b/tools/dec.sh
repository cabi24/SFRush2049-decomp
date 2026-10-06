#!/bin/sh
# dec.sh LABEL [N] -- print colouring decisions (order, web, save, nocs, totalsave, reg) from runs/LABEL/cdx
grep -E "p1dec" /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w8b/runs/$1/cdx | awk '{for(i=1;i<=NF;i++) if($i ~ /^(web|save|nocs|totalsave|bestreg|decision|forbidden0)=/) printf "%s ",$i; print ""}' | grep -n .
