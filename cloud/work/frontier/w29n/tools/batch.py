#!/usr/bin/env python3
"""w30n: one batch of renames.json as its own rename table.

    python3 cloud/work/frontier/w29n/tools/batch.py MAP.json BATCH OUT.json

renames.json is the single source of truth for every game rename; each accepted
row carries the `batch` (lane) that applied it (w29n, w30n, ...). The tools that
act on a tree which does not yet carry a rename (the inventory, symbol_addrs,
sources, the lock re-key, CLAIMS) must see only the batch being applied: an
earlier batch's old names no longer exist and they refuse on them. OUT gets the
same document with `renames` reduced to the accepted rows of BATCH (refuses on
an empty batch). Tools that only rename tokens (rebind_packets, followups,
gamesyms) keep reading the whole table; they are idempotent on applied rows.
"""
import json
import sys
from pathlib import Path


def main():
    src, batch, out = sys.argv[1], sys.argv[2], sys.argv[3]
    doc = json.loads(Path(src).read_text())
    rows = [r for r in doc["renames"]
            if r.get("status", "accepted") == "accepted" and r.get("batch") == batch]
    if not rows:
        sys.exit(f"refusing: no accepted rows in batch {batch!r}")
    Path(out).parent.mkdir(parents=True, exist_ok=True)
    Path(out).write_text(json.dumps(dict(doc, renames=rows), indent=1) + "\n")
    kinds = {}
    for r in rows:
        kinds[r.get("kind", "function")] = kinds.get(r.get("kind", "function"), 0) + 1
    print(f"batch {batch}: {len(rows)} rows ({', '.join(f'{n} {k}' for k, n in sorted(kinds.items()))}) -> {out}")


if __name__ == "__main__":
    main()
