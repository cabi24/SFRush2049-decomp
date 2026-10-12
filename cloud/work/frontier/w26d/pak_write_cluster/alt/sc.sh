#!/bin/sh
# usage: sc.sh FN file.c...   (unit score with the cluster's internal set)
D=$(pwd); fn=$1; shift; L=""
for f in "$@"; do case $f in /*) ;; *) f=$D/$f;; esac; L="$L $f"; done
cd /home/cburnes/projects/rush2049-decomp
export TAG=w26d
export UARGS="--internal func_800A1BB4 --internal func_800A1C6C --internal func_800A1DD4 --internal track_data_decompress --internal pfs_queue_init --internal pfs_lock --internal pfs_unlock $XARGS"
exec cloud/work/frontier/tools/trace/us.sh $fn $L
