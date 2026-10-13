#!/usr/bin/env python3
"""w29n: CLAIMS.md follows the rename: each line's first mention of an old function name
becomes `NEW (OLD)`, later mentions `NEW`; the w29n row is added once.

    python3 cloud/work/frontier/w29n/tools/claims.py MAP.json
"""
import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[5]
CLAIMS = REPO / "cloud/work/CLAIMS.md"
ROW = ("| game | symbol renames (old -> new -> GC source in `docs/external/gc/names.md`; table "
       "`cloud/work/frontier/w29n/renames.json`, replayed by `cloud/work/frontier/w29n/replay.sh`) | w29n | "
       "2026-10-13 | applied last in wave 29: lock re-keyed and re-pinned by pure-rename proof, image/unit/ROM gates "
       "exact; needs the Conveyor inventory rename (`w29n/tools/db_rename.py --apply`) |\n")


def main():
    t = {r["old"]: r["new"] for r in json.loads(Path(sys.argv[1]).read_text())["renames"]
         if r.get("status", "accepted") == "accepted" and r.get("kind") == "function"}
    rx = re.compile(r"(?<![\w/.])(%s)(?![\w/]|\.c\b|\.s\b)" % "|".join(map(re.escape, t)))
    out = []
    for line in CLAIMS.read_text().splitlines(keepends=True):
        if line.startswith("| game | symbol renames"):
            continue
        seen = set()

        def sub(m):
            o = m.group(1)
            if o in seen:
                return t[o]
            seen.add(o)
            return f"{t[o]} ({o})"
        out.append(rx.sub(sub, line))
    idx = max(i for i, l in enumerate(out) if l.startswith("| ovl_a / ovl_b | everything else"))
    out.insert(idx + 1, ROW)
    CLAIMS.write_text("".join(out))
    print("CLAIMS.md renamed")


if __name__ == "__main__":
    main()
