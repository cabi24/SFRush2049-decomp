#!/usr/bin/env python3
"""w21b: layout-neutral TU edit func_8001671C (and func_80016998) need, applied to a COPY of src/rom.
    python3 lane/tools/tu_edits.py OUTDIR      (run from the scratch root or repo root; writes OUTDIR/<tu>.c)
Each edit is (tu, old text, new text); old must occur exactly once. Proven neutral by building the
edited TU alone and comparing with the original (tusplice.sh on OUTDIR without any splice)."""
import sys
from pathlib import Path

EDITS = []
# lib_16320: hoist the packed ResourceRange typedef (macro main table D_8003DA28) out of the func_80017108
# block to just before the func_8001671C slot, its first user (as lib_1f5b0 hoists its shared records).
# Declarations only; the 17108 block keeps its (now empty) pack pragmas and its externs.
SLOT = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/func_8001671C.s")\n'
TD = "typedef struct ResourceRange { u16 count; u16 first; } ResourceRange;\n"
EDITS += [("lib_16320", "#pragma pack(1)\n" + TD + "#pragma pack(0)\nextern int D_80038608;\n",
           "#pragma pack(1)\n#pragma pack(0)\nextern int D_80038608;\n"),
          ("lib_16320", SLOT,
           "/* Macro main table record, hoisted before its first user (w21b; was in the func_80017108 block). */\n"
           "#pragma pack(1)\n" + TD + "#pragma pack(0)\n" + SLOT)]

out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True)
texts = {}
for tu, old, new in EDITS:
    t = texts.get(tu) or Path(f"src/rom/{tu}.c").read_text()
    assert t.count(old) == 1, (tu, old[:60])
    texts[tu] = t.replace(old, new, 1)
for tu, t in texts.items():
    (out / f"{tu}.c").write_text(t); print("wrote", out / f"{tu}.c")
