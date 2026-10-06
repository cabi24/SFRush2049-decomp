#!/bin/sh
# Replays the w12c real-caller component (w11f harness + E0050, w12a DFBA0, w12b D5E64, w12c msh/E05F0).
# No stand-ins: best_times_display's callers (func_800D5E64, mode_select_handler), func_800DED78's caller
# (mode_select_handler) and mode_select_input's callers (func_800DFBA0, func_800E05F0) are all real drafts.
C=cloud/work/frontier/w12c/comp
python3 -m tools.conveyor.pipeline.blob_unit --tag ${TAG:-w12c} score \
  func_800D5E64 best_times_display mode_select_handler func_800DED78 func_800DEF60 func_800DFBA0 mode_select_input func_800E0050 func_800E05F0 \
  --with $C/d5e64.c --with $C/mode.c --with $C/msh.c --with $C/ded78.c \
  --internal best_times_display --internal mode_select_input --internal func_800DFBA0 \
  --internal mode_select_handler --internal func_800DED78 --internal func_800E0050 --internal func_800DEF60 --neighbours "$@"
