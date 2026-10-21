#!/bin/bash
# area.sh SEGFILE [LABEL] : build group from SEGFILE, snapshot with trace toolkit, print AREA/first TMP lines for D6A4
cd "$(dirname "$0")/.."
export TAG=w35a SCR=rush2049/scratch/frontier/w35a
L=${2:-a}
python3 - "$1" "$L" <<'PY'
import sys
from pathlib import Path
W=Path('.').resolve()
head=(W/'parts/head.c').read_text(); tail=(W/'parts/tail.c').read_text()
d=W/'tmp'/('area_'+sys.argv[2]); d.mkdir(parents=True,exist_ok=True)
(d/'group.c').write_text(head+Path(sys.argv[1]).read_text().rstrip('\n')+'\n\n'+tail)
PY
T=../tools/trace
$T/snap.sh $L tmp/area_$L/group.c func_803A1EAC >/dev/null 2>&1
$T/spill.sh $L ${3:-func_8039D6A4} 2>&1 | grep "^AREA\|^\[TMP\] spill" | head -${N:-12}
