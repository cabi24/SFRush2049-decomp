#!/usr/bin/env python3
"""w19c: write a SIMULATED lockfile (never matched.lock.json) = the current lock plus the three
score-0 entries `lock add` would write for the w16g sources, for promote_batch --dry-run --lockfile.
Usage (repo root): python3 cloud/work/frontier/w19c/simlock.py OUT.json"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.pipeline import lock  # noqa: E402

S = "cloud/work/frontier/w16g/sources/"
ENTRIES = [("osAiSetNextBuffer", "-g0 -O2 -mips2 -G 0 -non_shared"),
           ("__fixunsdfdi", "-g0 -O1 -mips3 -32 -G 0 -non_shared"),
           ("__fixunssfdi", "-g0 -O1 -mips3 -32 -G 0 -non_shared")]
out = Path(sys.argv[1]).resolve()
if out == (lock.REPO / "matched.lock.json").resolve():
    sys.exit("refusing to write matched.lock.json")
e = lock.load_lock()
for fn, flags in ENTRIES:
    e[S + fn + ".c:" + fn] = {"body_sha256": lock.body_sha(lock.REPO / (S + fn + ".c"), fn),
                              "target_id": fn, "flagset": flags, "verified": "score0",
                              "toolkit_sha": "SIMULATED", "verified_at": "simulated"}
lock.save_lock(e, out)
print(f"{out}: {len(e)} entries")
