#!/bin/sh
# ucheck.sh FN FILE [extra args]: unit-score FN with FILE, print verdict + count of loads of the TU .data statics
cd /home/cburnes/projects/rush2049-decomp
fn=$1; f=$2; shift 2
python3 -m tools.conveyor.pipeline.blob_unit --tag w31c score $fn --with $f "$@" --neighbours 2>&1 | grep -E "(EQUAL|FAIL) $fn" | cut -c1-90
mips-linux-gnu-objdump -dr build/blob_unit/w31c/unit.o | awk -v f="<$fn>:" 'index($0,f){p=1} p&&/^$/{exit} p' > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad/$fn.dis
