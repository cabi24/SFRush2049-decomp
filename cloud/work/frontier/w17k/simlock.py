#!/usr/bin/env python3
"""w17k: a SIMULATED lockfile for promote_batch dry runs (never matched.lock.json).
Copies matched.lock.json and adds, for each function, the entry
`lock add cloud/work/frontier/w17g/sources/FN.c:FN --flags "-g0 -O2 -mips2 -G 0 -non_shared"
--toolkit TOOLKIT` would write after a score-0 verify (score71.log is that verify,
replayed with the same toolkit bundle). COMPILER="" writes legacy (5.3-style) entries.
    python3 cloud/work/frontier/w17k/simlock.py OUT.json TOOLKIT COMPILER fn [fn...]"""
import json
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.pipeline import lock as lockmod  # noqa: E402

out, toolkit, compiler = Path(sys.argv[1]), sys.argv[2], sys.argv[3]
entries = lockmod.load_lock()
for fn in sys.argv[4:]:
    rel = f"cloud/work/frontier/w17g/sources/{fn}.c"
    entry = {"body_sha256": lockmod.body_sha(rel, fn), "target_id": fn,
             "flagset": "-g0 -O2 -mips2 -G 0 -non_shared", "verified": "score0",
             "toolkit_sha": toolkit, "verified_at": time.strftime("%Y-%m-%d")}
    assert entry["body_sha256"], fn
    if compiler:
        entry["compiler"] = compiler
    entries[f"{rel}:{fn}"] = entry
lockmod.save_lock(entries, out)
print(f"wrote {out}: {len(sys.argv) - 4} simulated entries ({compiler or 'legacy'})")
