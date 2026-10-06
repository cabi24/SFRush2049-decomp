#!/bin/bash
# usage: sc.sh cand.c [NAME...]  -- score candidate in the whole-program unit (tag w10h); prints result + differing offsets summary
R=/home/cburnes/projects/rush2049-decomp; f=$(realpath $1); shift; N=${@:-func_8008E408}
cd $R && python3 -m tools.conveyor.pipeline.blob_unit --tag w10h --jobs 2 score $N --with $f 2>&1 | grep -E "MATCH|FAIL|EQUAL|equal|image .* unit|differ:" | head -${SCN:-8} | cut -c1-160
