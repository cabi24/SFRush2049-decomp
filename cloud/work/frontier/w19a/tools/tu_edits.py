#!/usr/bin/env python3
"""w19a: layout-neutral TU edits some w19a sources need, applied to a COPY of src/rom.
    python3 lane/tools/tu_edits.py OUTDIR      (run from the scratch root or repo root; writes OUTDIR/<tu>.c)
Each edit is (tu, old text, new text); old must occur exactly once. Proven neutral by building the
edited TU alone and comparing with the original (tusplice.sh on OUTDIR without any splice)."""
import sys
from pathlib import Path

EDITS = []
# lib_22300: hoist func_80021B9C's MacroState / MacroCommand typedefs (2 lines) above the
# func_8002193C slot: func_8002193C is defined with the TU's later prototype
# `u8 func_8002193C(MacroState *, MacroCommand *)` (func_80021B9C's block), so both types must be
# visible at its slot.
MS = ("typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;\n"
      "typedef struct MacroCommand { u32 word[2]; } MacroCommand;\n")
S193 = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_22300/func_8002193C.s")\n'
EDITS += [("lib_22300", MS, ""), ("lib_22300", S193, MS + S193)]

out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True)
texts = {}
for tu, old, new in EDITS:
    t = texts.get(tu) or Path(f"src/rom/{tu}.c").read_text()
    assert t.count(old) == 1, (tu, old[:60])
    texts[tu] = t.replace(old, new, 1)
for tu, t in texts.items():
    (out / f"{tu}.c").write_text(t); print("wrote", out / f"{tu}.c")
