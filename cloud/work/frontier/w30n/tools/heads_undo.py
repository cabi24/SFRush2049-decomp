#!/usr/bin/env python3
"""w30n: undo/redo a head registration on a parsed target table (for re-pin proofs).

A registered head (cloud/work/frontier/w30n/heads.json) replaces, in the
asm/us/blob target table {name: [words]}, the inventory rows it retired
(suffix labels such as highscore_entry_anim, marked extent_conflict:<head> by
`closure run`) with one function spanning them. `undo(table)` gives the table
as it was before: the head removed, each retired label restored with the
head's own words from its address to the head's end (the retired row ended at
the same `jr ra` delay slot; its recorded word count is checked). Receipts
that pinned a digest of the whole table are re-pinned only when the digest of
`undo(today)` equals the pinned value.
"""
import json
from pathlib import Path

REPO = Path(__file__).resolve().parents[5]
HEADS = REPO / "cloud/work/frontier/w30n/heads.json"


def heads(path=HEADS):
    return json.loads(Path(path).read_text())["registered"]


def undo(table, path=HEADS):
    table = dict(table)
    for head in heads(path):
        name, base, count = head["name"], int(head["vram"], 16), head["words"]
        if name not in table:
            raise SystemExit(f"refusing: {name} is not in the target table (not registered here)")
        words = table.pop(name)
        if len(words) != count:
            raise SystemExit(f"refusing: {name} has {len(words)} words, heads.json says {count}")
        for tail in head["retires"]:
            if tail["name"] in table:
                raise SystemExit(f"refusing: retired {tail['name']} is still a target")
            start = (int(tail["vram"], 16) - base) // 4
            restored = words[start:]
            if len(restored) != tail["words"]:
                raise SystemExit(f"refusing: {tail['name']} would get {len(restored)} words, "
                                 f"its row had {tail['words']}")
            table[tail["name"]] = restored
    return table
