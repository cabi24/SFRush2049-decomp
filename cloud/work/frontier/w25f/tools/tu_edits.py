#!/usr/bin/env python3
"""w25f: layout-neutral TU edit func_8001F13C needs, applied to a COPY of src/rom (pattern of w19a/tools/tu_edits.py).
    python3 lane/tools/tu_edits.py OUTDIR      (scratch root or repo root; writes OUTDIR/lib_1f5b0.c)
lib_1f5b0: hoist the ChannelLink typedef + `extern ChannelLink D_80050440[32];` (func_8001F6EC's context, the two
lines right after the func_8001F13C slot) above that slot. func_8001F13C uses both; with them in the prefix,
--prefix-dedup drops them from its preamble and the TU has a single declaration of each."""
import sys
from pathlib import Path

EDITS = []
CL = ("typedef struct ChannelLink { u8 previous; u8 next; u16 flags; } ChannelLink;\n"
      "extern ChannelLink D_80050440[32];\n")
S13C = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001F13C.s")\n'
EDITS += [("lib_1f5b0", S13C + CL, CL + S13C)]

out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True)
texts = {}
for tu, old, new in EDITS:
    t = texts.get(tu) or Path(f"src/rom/{tu}.c").read_text()
    assert t.count(old) == 1, (tu, old[:60])
    texts[tu] = t.replace(old, new, 1)
for tu, t in texts.items():
    (out / f"{tu}.c").write_text(t); print("wrote", out / f"{tu}.c")
