#!/bin/sh
# usage: bu.sh file.c NAME... [-- extra blob_unit args]  -- whole-program unit score, prints summary lines
f="$1"; shift
cd /home/cburnes/projects/rush2049-decomp && python3 -m tools.conveyor.pipeline.blob_unit --tag w3b score "$@" --with "$f" 2>&1 | grep -E "EQUAL|FAIL|words differ|error|Error|equal;" 
