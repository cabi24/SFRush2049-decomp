#!/bin/bash
# g3.sh FILE.c [extra args]: unit score (tag w15b) of physics_sym, func_800B5688, func_800B59F0, func_800B59E8, func_800B55F4 (last four internal), --neighbours
f=$(realpath "$1"); shift
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w15b --jobs 2 score physics_sym func_800B5688 func_800B59F0 func_800B59E8 func_800B55F4 \
  --internal func_800B5688 --internal func_800B59F0 --internal func_800B59E8 --internal func_800B55F4 --with "$f" --neighbours "$@" 2>&1 | grep -v '^$'
