#!/bin/sh
# Replays the w12i real-caller component (w12c comp + w12i mode.c: func_800E0048 helper, E05F0 frame/tail/loop shaping).
# No stand-ins: best_times_display's callers (func_800D5E64, mode_select_handler), func_800DED78's caller
# (mode_select_handler) and mode_select_input's callers (func_800DFBA0, func_800E05F0) are all real drafts.
C=cloud/work/frontier/w13b/comp
python3 -m tools.conveyor.pipeline.blob_unit --tag ${TAG:-w13b} score \
  func_800D5E64 best_times_display mode_select_handler func_800DED78 func_800DEF60 func_800DFBA0 mode_select_input func_800E0048 func_800E0050 func_800E05F0 \
  --with $C/d5e64.c --with ${MODE:-$C/mode.c} --with $C/msh.c --with $C/ded78.c \
  --internal best_times_display --internal mode_select_input --internal func_800DFBA0 \
  --internal mode_select_handler --internal func_800DED78 --internal func_800E0050 --internal func_800DEF60 --internal func_800E0048 --neighbours "$@"
