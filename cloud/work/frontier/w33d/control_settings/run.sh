#!/bin/sh
# usage: run.sh FILE.c -- unit-score the control-settings group TU (w33d). Helpers are renamed *_DC -> *_DQ
# on the fly so they do not collide with the locked codex_control_settings_a4 group.c copies.
F=$(realpath "$1"); cd /home/cburnes/projects/rush2049-decomp
O=build/trace/w33d.cs.out; mkdir -p build/trace
S=build/trace/w33d_cand.c; sed 's/_DC\b/_DQ/g' "$F" > $S
I=""; for h in $(grep -o '\b[A-Za-z0-9_]*_DQ\b' $S | sort -u); do I="$I --internal $h"; done
python3 -m tools.conveyor.pipeline.blob_unit --tag w33d score control_settings control_cell_text control_cell_text3 control_cell_text2 func_800DD0C0 func_800DCD58 func_800DCCE0 --with "$S" --internal control_cell_text --internal control_cell_text3 --internal control_cell_text2 --internal func_800DD0C0 $I --neighbours $UARGS > $O 2>&1
grep -v '^       ' $O | cut -c1-150
T=cloud/work/frontier/tools/trace
for f in control_settings; do echo "  $f: $(TAG=w33d python3 $T/udiff.py $f --summary | sed 's/; unverified.*//') | ops $(TAG=w33d python3 $T/udiff.py $f --summary --ops | sed 's/.*rows \([0-9]*\).*/\1/')"; done
