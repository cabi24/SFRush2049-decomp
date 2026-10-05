#!/usr/bin/env python3
"""Unmatched game functions with own-.bss-like references (read-only scan).

    PYTHONPATH=. python3 cloud/work/frontier/bss/scan_bss.py [--md]

Input: cloud/work/frontier/type_model/refs.json (every absolute address a game
function forms, loads or stores; scan_refs.py) and the current lock. An
address inside owndata.GAME_BSS is EXCLUSIVE to a function when no other game
function references it: a function-local static, or a file-scope object only
that function uses. An address-level test cannot see object extents: an
exclusive address may also be one field of a shared struct or array that
only this function touches at that offset. Tier A (every .bss address the
function references is exclusive) is the strongest signal; tier B (some
exclusive, some shared) is a lead only. Such references could only be written as `extern` before
the .bss rule; when retail's code reflects `static` (uopt alias analysis, as
in func_800F0674) only a static reproduces it. Shared addresses whose every
referencing function lies in one small address window are reported per
window as possible file-scope statics of one translation unit (group splice).
"""
import json
import sys
from collections import defaultdict
from pathlib import Path

from tools.cloud import owndata
from tools.conveyor.pipeline import blob_layout

REPO = Path(__file__).resolve().parents[4]
REFS = REPO / "cloud" / "work" / "frontier" / "type_model" / "refs.json"


def main():
    md = "--md" in sys.argv
    refs = json.loads(REFS.read_text())
    lock = json.loads((REPO / "blob_matched.lock.json").read_text())
    doc = blob_layout.load()
    extents = {e["target_id"]: e for r in doc["regions"] for e in r["entries"]
               if e["kind"] == "function"}
    users = defaultdict(set)
    per_func = defaultdict(set)
    for ref in refs:
        if owndata.bss_range(ref["addr"], 1):
            users[ref["addr"]].add(ref["f"])
            per_func[ref["f"]].add(ref["addr"])
    rows = []
    for name, addrs in per_func.items():
        if name in lock or name not in extents:
            continue
        own = sorted(a for a in addrs if users[a] == {name})
        if own:
            rows.append((name, extents[name]["vaddr"], extents[name]["size"], own,
                         len(addrs) - len(own)))
    rows.sort(key=lambda r: (r[4] > 0, r[1]))      # tier A first, then by address
    # shared by a few unmatched neighbours only (one TU's file-scope statics?)
    tu = []
    for addr, fs in sorted(users.items()):
        if len(fs) < 2 or any(f in lock or f not in extents for f in fs):
            continue
        spots = sorted(extents[f]["vaddr"] for f in fs)
        if spots[-1] - spots[0] <= 0x2000:
            tu.append((addr, sorted(fs, key=lambda f: extents[f]["vaddr"])))
    total = sum(r[2] for r in rows)
    if md:
        print("| Tier | Function | Address | Bytes | Exclusive .bss addresses | Shared .bss addresses |")
        print("|---|---|---|---:|---|---:|")
        for name, vaddr, size, own, shared in rows:
            if shared and len(own) < 3:
                continue                    # tier B with one or two: noise
            shown = ", ".join(f"0x{a:08X}" for a in own[:4]) + (" …" if len(own) > 4 else "")
            print(f"| {'B' if shared else 'A'} | `{name}` | 0x{vaddr:08X} | {size} | {shown} ({len(own)}) | {shared} |")
        print(f"\n{len(rows)} unmatched functions, {total} bytes, reference at least one exclusive "
              f".bss address ({sum(1 for r in rows if not r[4])} in tier A); tier B rows with fewer "
              "than three exclusive addresses are not shown.")
        print(f"{len(tu)} more .bss addresses are shared only by unmatched functions within "
              "0x2000 bytes of each other (possible file-scope statics of one TU).")
    else:
        for name, vaddr, size, own, shared in rows:
            print(f"{name:28s} 0x{vaddr:08X} {size:6d}  own {len(own):3d}  shared {shared:3d}  "
                  + " ".join(f"{a:08X}" for a in own[:6]))
        print(f"{len(rows)} functions, {total} bytes; TU-candidate shared addresses: {len(tu)}")
        for addr, fs in tu[:40]:
            print(f"  {addr:08X}: {' '.join(fs)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
