#!/bin/sh
# Replays the w11f real-caller component in the whole-program unit (run from the repo root on the Pi).
# No stand-ins: best_times_display's callers (func_800D5E64, mode_select_handler), func_800DED78's caller
# (mode_select_handler) and mode_select_input's callers (func_800DFBA0, func_800E05F0) are all real drafts.
C=cloud/work/frontier/w11f/comp
python3 -m tools.conveyor.pipeline.blob_unit --tag ${TAG:-w11f} score \
  func_800D5E64 best_times_display mode_select_handler func_800DED78 func_800DFBA0 mode_select_input func_800E05F0 \
  --with $C/d5e64.c --with $C/mode.c --with $C/msh.c --with $C/ded78.c \
  --internal best_times_display --internal mode_select_input --internal func_800DFBA0 \
  --internal mode_select_handler --internal func_800DED78 --neighbours "$@"
