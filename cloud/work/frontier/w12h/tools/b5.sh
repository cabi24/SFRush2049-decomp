#!/bin/bash
# b5.sh FILE.c... : B59F0 unit score with stand-in callers (tag w12h); prints verdict, rows and frame
export TAG=w12h
T=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace
UARGS="${UARGS:---internal func_800B59F0 --keep zz_caller --keep zz_caller2 --block func_800B59F0}" $T/us.sh func_800B59F0 "$@" 2>&1 | grep -v "^$"
