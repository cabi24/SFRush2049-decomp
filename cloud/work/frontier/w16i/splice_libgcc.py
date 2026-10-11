#!/usr/bin/env python3
"""w16i scratch-only replay of the libgcc promotion (run from a SCRATCH COPY of the repo root).

    python3 cloud/work/frontier/w16i/splice_libgcc.py

1. Writes the lock entries `lock add SRC:FN --flags "-g0 -O1 -mips3 -32 -G 0 -non_shared"`
   would record for the 16 libgcc functions (score evidence: check.py on the builder,
   cloud/work/frontier/w16i/libgcc_scores.log) into this copy's matched.lock.json.
2. Regenerates src/rom/opt_overrides.mk (layout.write_opt_overrides) from those pins.
3. Runs promote_batch.py's own plan() + splice() with w16g's ctx/ctx_ll.jsonl over
   0xe4f0 and 0xa5f0 (no gate, no publish, no commit; the copy is then built on the
   builder scratch with `make COMPILER=ido && make test`).
Refuses to run inside a git checkout, so it can never touch a real tree.
"""
import json
import sys
from pathlib import Path

ROOT = Path.cwd()
if (ROOT / ".git").exists():
    sys.exit("refusing: run from a scratch copy without .git, not a checkout")
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(ROOT / "cloud/work/boot_tail_promotion"))
from tools.conveyor.pipeline import layout as layoutmod  # noqa: E402
from tools.conveyor.pipeline import lock as lockmod  # noqa: E402
import promote_batch  # noqa: E402

LL = "-g0 -O1 -mips3 -32 -G 0 -non_shared"
TK = "d4c39cbc85750cd02d3494f318c65e770bbf7d4b10aae137bcf5b5f149137c5b"
SRC = "cloud/work/frontier/w16g/sources"
FNS = ("__lshrdi3 __umoddi3 __udivdi3 __ashldi3 __umoddi3_alt __divdi3 __muldi3 __qdivrem "
       "__moddi3 __ashrdi3 __fixdfdi __fixsfdi __floatdidf __floatdisf __floatundidf "
       "__floatundisf").split()
CTX = Path("cloud/work/frontier/w16g/ctx/ctx_ll.jsonl")

entries = lockmod.load_lock()
for fn in FNS:
    spec = f"{SRC}/{fn}.c:{fn}"
    entries[spec] = {"body_sha256": lockmod.body_sha(ROOT / SRC / f"{fn}.c", fn),
                     "target_id": fn, "flagset": LL, "verified": "score0",
                     "toolkit_sha": TK, "verified_at": "2026-10-11"}
lockmod.save_lock(entries)
layoutmod.write_opt_overrides()
print(open("src/rom/opt_overrides.mk").read().strip().splitlines()[-3:])

promote_batch.CONTEXT_PATH = CTX
context = {json.loads(l)["function"]: json.loads(l) for l in CTX.read_text().splitlines()}
cands = promote_batch.plan(["0xe4f0", "0xa5f0"], context)
print(f"plan: {len(cands)} candidates: {' '.join(c['fn'] for c in cands)}")
tus = promote_batch.splice(cands)
print("spliced:", " ".join(str(t.relative_to(ROOT)) for t in tus))
