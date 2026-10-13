#!/usr/bin/env python3
"""w29n: rename game-function definition lines in symbol_addrs.us.txt.

    python3 cloud/work/frontier/w29n/tools/symaddr_rename.py MAP.json

static_rename.rewrite_symbols writes a libultra comment; this does the same
one-pass rename for the game rows of MAP.json that symbol_addrs defines (each
exactly once, at the row's vram), with a comment naming the old label and the
table. Rows symbol_addrs does not define are skipped (their name comes from the
inventory). `dup-addr` comment lines are left alone.
"""
import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[5]
DEF = re.compile(r"^(\s*)([A-Za-z_]\w*)(\s*=\s*)(0x[0-9A-Fa-f]+)(\s*;)(.*)$")


def main():
    rows = [r for r in json.loads(Path(sys.argv[1]).read_text())["renames"]
            if r.get("status", "accepted") == "accepted"]
    by_old = {r["old"]: r for r in rows}
    news = {r["new"] for r in rows}
    path = REPO / "symbol_addrs.us.txt"
    out, seen = [], {}
    for line in path.read_text().splitlines(keepends=True):
        m = DEF.match(line.rstrip("\n"))
        if m and m.group(2) in news:
            sys.exit(f"refusing: {m.group(2)} already defined")
        if m and m.group(2) in by_old:
            r = by_old[m.group(2)]
            if int(m.group(4), 16) != int(r["vram"], 16):
                sys.exit(f"refusing: {m.group(2)} defined at {m.group(4)}, table {r['vram']}")
            seen[r["old"]] = seen.get(r["old"], 0) + 1
            kind = "type:func " if "type:func" in m.group(6) else ""
            nl = "\n" if line.endswith("\n") else ""
            line = (f"{m.group(1)}{r['new']}{m.group(3)}{m.group(4)}{m.group(5)} // {kind}{r['source']}"
                    f" (renamed from {r['old']}, w29n; cloud/work/frontier/w29n/renames.json){nl}")
        out.append(line)
    if any(n > 1 for n in seen.values()):
        sys.exit(f"refusing: defined twice: {[k for k, n in seen.items() if n > 1]}")
    path.write_text("".join(out))
    print(f"symbol_addrs: {len(seen)} definitions renamed: {', '.join(sorted(seen))}")


if __name__ == "__main__":
    main()
