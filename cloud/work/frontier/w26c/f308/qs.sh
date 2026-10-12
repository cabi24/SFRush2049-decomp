#!/bin/sh
# qs.sh SWD.c [EFFECTS.c]: score save_write_data with a swd variant (rows + frame)
D=/home/cburnes/projects/rush2049-decomp; W=$D/cloud/work/frontier/w26c/f308
SWD=$(realpath $1) $W/q.sh save_write_data ${2:-$W/best_effects_w26c.c} | sed "s#^[^:]*:#$(basename $1):#"
