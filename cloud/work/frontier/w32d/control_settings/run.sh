#!/bin/sh
# usage: run.sh FILE.c  -- unit-score the control-settings group TU (w32d)
F=$(realpath "$1"); cd /home/cburnes/projects/rush2049-decomp
O=build/trace/w32d.cs.out; mkdir -p build/trace
python3 -m tools.conveyor.pipeline.blob_unit --tag w32d score control_settings control_cell_text control_cell_text3 control_cell_text2 func_800DD0C0 func_800DCD58 func_800DCCE0 --with "$F" --internal control_cell_text --internal control_cell_text3 --internal control_cell_text2 --internal func_800DD0C0 --internal gfx_lock_DC --internal gfx_unlock_DC --internal font_set_DC --internal font_set1_DC --internal pair_set_DC --internal cell_DC --internal fs11_DC --internal TB_CenterText_DC --neighbours $UARGS > $O 2>&1
grep -v '^       ' $O | cut -c1-150
T=cloud/work/frontier/tools/trace
for f in control_settings control_cell_text3 control_cell_text2 func_800DD0C0; do echo "  $f: $(TAG=w32d python3 $T/udiff.py $f --summary | sed 's/; unverified.*//') | ops $(TAG=w32d python3 $T/udiff.py $f --summary --ops | sed 's/.*rows \([0-9]*\).*/\1/')"; done
