#!/usr/bin/env python3
"""w26b: layout-neutral TU edit func_8001A658 needs, applied to a COPY of src/rom (pattern of w25f/tools/tu_edits.py).
    python3 cloud/work/frontier/w26b/tools/tu_edits.py OUTDIR    (run from the repo/scratch root; writes OUTDIR/lib_1a660.c)
lib_1a660: hoist `typedef struct ChannelState {...} ChannelState;` + `extern ChannelState D_8004F300[32];` (the
func_8001B9F8 block's context, two lines after func_8001B9F8's predecessor) above the func_8001A658 slot.
func_8001A658 uses both; with them in the prefix, --prefix-dedup drops them from its preamble and the TU has a
single declaration of each. Use OUTDIR=src/rom to apply in place."""
import sys
from pathlib import Path

CS = ("typedef struct ChannelState {u32 word00,word04,flags08,pending0C,unknown10;u8 kind14;u8 unknown15[3];"
      "u32 value18;u8 unknown1C[12];} ChannelState;\nextern ChannelState D_8004F300[32];\n")
SLOT = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1a660/func_8001A658.s")\n'
out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True)
t = Path("src/rom/lib_1a660.c").read_text()
assert t.count(CS) == 1 and t.count(SLOT) == 1 and t.index(SLOT) < t.index(CS)
t = t.replace(CS, "", 1).replace(SLOT, CS + SLOT, 1)
(out / "lib_1a660.c").write_text(t); print("wrote", out / "lib_1a660.c")
