#!/bin/sh
# nt.sh NAME CAND LABEL PROC -- trace; print phase/web/numintf/colour compactly
sh /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w10b/tools/ctrace.sh "$@" | awk '{print $1, $2, $5, $8, $10, $11}' | tr '\n' ';'; echo
