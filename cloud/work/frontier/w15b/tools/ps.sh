#!/bin/bash
# ps.sh FILE.c... : physics_sym + internal func_800B59F0 in the unit (tag w15b)
export TAG=w15b
T=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace
for f in "$@"; do
UARGS="${UARGS:---internal func_800B59F0}" $T/us.sh physics_sym $f 2>&1 | grep -v "^$"
done
