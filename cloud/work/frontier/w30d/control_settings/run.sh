#!/bin/sh
# usage: run.sh FILE.c  -- unit-score control_settings + 3 cell handlers (w30d)
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w30d score control_settings control_cell_text control_cell_text3 control_cell_text2 --with "$1" --internal control_cell_text --internal control_cell_text3 --internal control_cell_text2 --internal gfx_lock_DC --internal gfx_unlock_DC --internal font_set_DC --internal font_set1_DC --internal pair_set_DC --internal cell_DC $UARGS 2>&1 | grep -E '^  (EQUAL|FAIL)|locked bodies|rror' | cut -c1-150
T=cloud/work/frontier/tools/trace
for f in control_settings control_cell_text control_cell_text3 control_cell_text2; do echo "  $f: $(TAG=w30d python3 $T/udiff.py $f --summary | sed 's/; unverified.*//') | ops $(TAG=w30d python3 $T/udiff.py $f --summary --ops | sed 's/.*rows \([0-9]*\).*/\1/')"; done
