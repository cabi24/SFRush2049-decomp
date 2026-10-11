#!/bin/sh
# us.sh BILLBOARD.c [F0914.c]: unit-score billboard_render + F207C + F1930 with the w15f/bb context.
# Runs blob_unit from $WT (a clean worktree of HEAD) while the main tree is mid-integration; defaults to the repo.
B=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w15f/bb
WT=${WT:-/home/cburnes/projects/rush2049-decomp}
BB=$(realpath "$1"); F09=$(realpath "${2:-$B/diag/f0914_standin.c}")
cd "$WT" && python3 -m tools.conveyor.pipeline.blob_unit --tag w15f score billboard_render func_800F207C func_800F1930 func_800F0914 \
  --with "$BB" --with $B/func_800F207C.c --with $B/func_800F1930.c --with $B/func_800F0F44.c --with $B/func_800F1210.c --with "$F09" \
  --internal func_800F207C --internal func_800F1930 --internal func_800F0914 --internal func_800F1210 --internal func_800F0F44 \
  --neighbours $UARGS 2>&1 | grep -v "^  *+0x"
