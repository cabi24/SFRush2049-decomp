#!/bin/sh
# usage: uscore.sh FILE.c NAME [more blob_unit args]   (unit score + aligned diff summary)
f="$1"; n="$2"; shift; shift
cd /home/cburnes/projects/rush2049-decomp && python3 -m tools.conveyor.pipeline.blob_unit --tag w36v score $n --with "$f" "$@" 2>&1 | grep -E "EQUAL|FAIL|differ in this|umerge inlined" | head -5
TAG=w36v python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace/udiff.py $n --summary 2>/dev/null
TAG=w36v python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace/udiff.py $n --ops --summary 2>/dev/null
