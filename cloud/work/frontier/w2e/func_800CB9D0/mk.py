#!/usr/bin/env python3
"""mk.py OUTDIR cand1.c [cand2.c ...]: wrap each candidate (a complete file defining func_800CB9D0) as a
two-file -O3 group dir with the real heap context (ctx.c, from cloud/work/ipa-groups/codex_heap_move_a120)."""
import sys, json, os
from pathlib import Path
here = Path(__file__).resolve().parent
ctx = (here / 'ctx.c').read_text()
spec = {"members": ["func_800CB9D0"], "files": ["ctx.c", "cand.c"],
        "keep": ["audio_effect_process", "synced_model_render", "MP_TargetSpeed", "assign_default_paths", "stat_race_end",
                 "audio_buffer_sync", "object_counter_decrement", "object_counter_increment", "func_800CB9D0", "func_800A47C0"],
        "flags": "-g0 -O3 -mips2 -G 0 -non_shared", "unprototyped": [], "context": []}
out = Path(sys.argv[1])
for c in sys.argv[2:]:
    d = out / Path(c).stem; d.mkdir(parents=True, exist_ok=True)
    (d / 'ctx.c').write_text(ctx); (d / 'cand.c').write_text(Path(c).read_text())
    (d / 'group.json').write_text(json.dumps(spec))
