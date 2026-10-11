#!/usr/bin/env python3
"""w20b: layout-neutral TU edit func_8001536C / func_800154A4 need, applied to a COPY of src/rom.
    python3 lane/tools/tu_edits.py OUTDIR      (run from the scratch root or repo root; writes OUTDIR/<tu>.c)
Each edit is (tu, old text, new text); old must occur exactly once. Proven neutral by building the
edited TU alone and comparing with the original (tusplice.sh on OUTDIR without any splice)."""
import sys
from pathlib import Path

EDITS = []
# lib_15a10: the group-stack depth D_80038390 is a signed short (func_8001536C `lh` + `slti 128`,
# func_800154A4 `lh` + sll/sra decrement); the func_80014E10 block (a store of 0, `sh` either way)
# declares it unsigned short, which would conflict with the two sources' `extern short` at their slots.
EDITS += [("lib_15a10", "extern unsigned short D_80038390;\n", "extern short D_80038390;\n")]

out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True)
texts = {}
for tu, old, new in EDITS:
    t = texts.get(tu) or Path(f"src/rom/{tu}.c").read_text()
    assert t.count(old) == 1, (tu, old[:60])
    texts[tu] = t.replace(old, new, 1)
for tu, t in texts.items():
    (out / f"{tu}.c").write_text(t); print("wrote", out / f"{tu}.c")
