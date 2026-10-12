#!/usr/bin/env python3
"""w23c: layout-neutral TU edit func_80015A0C and func_80015D68 need, applied to a COPY of src/rom.
    python3 lane/tools/tu_edits.py OUTDIR      (run from the scratch root or repo root; writes OUTDIR/<tu>.c)
Each edit is (tu, old text, new text); old must occur exactly once. The packed record typedefs (and their table
externs) of the layer table D_8003CE20 (func_80015C0C block) and the curve/keymap table D_80038610 (func_80015F28
block) move to just before the func_80015A0C / func_80015D68 slots, their first users (as w21b hoisted
ResourceRange). Declarations only. Proven neutral by building the edited TU alone (tusplice.sh, no splice)."""
import sys
from pathlib import Path

EDITS = []
CE20 = ("#pragma pack(1)\n"
        "typedef struct { unsigned int payload; u16 id; u16 type; u16 references; u16 metadata; } RecordFields_8003CE20;\n"
        "#pragma pack()\n"
        "extern int D_8003CE18;\n"
        "typedef union { RecordFields_8003CE20 fields; unsigned int words[3]; } RecordStorage_8003CE20;\n"
        "extern RecordStorage_8003CE20 D_8003CE20[];\n")
T610 = ("#pragma pack(1)\n"
        "typedef struct { unsigned int payload; u16 id; u16 references; } RecordFields_80038610;\n"
        "#pragma pack()\n"
        "extern int D_80038608;\n"
        "typedef union { RecordFields_80038610 fields; unsigned int words[2]; } RecordStorage_80038610;\n"
        "extern RecordStorage_80038610 D_80038610[];\n")
for slot, block in (("func_80015A0C", CE20), ("func_80015D68", T610)):
    SLOT = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_16320/%s.s")\n' % slot
    EDITS += [("lib_16320", " */\n" + block + "int func_", " */\nint func_"),
              ("lib_16320", SLOT, "/* Table record, hoisted before its first user (w23c). */\n" + block + SLOT)]

out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True)
texts = {}
for tu, old, new in EDITS:
    t = texts.get(tu) or Path(f"src/rom/{tu}.c").read_text()
    assert t.count(old) == 1, (tu, old[:80])
    texts[tu] = t.replace(old, new, 1)
for tu, t in texts.items():
    (out / f"{tu}.c").write_text(t); print("wrote", out / f"{tu}.c")
