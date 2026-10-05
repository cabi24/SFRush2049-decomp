"""Per-element reference profile for candidate arrays: how many referenced
addresses fall in element k and what share of their offsets are also indexed
field offsets (helps decide the true element count)."""
import json, collections, sys
from common import *
refs = [r for r in json.load(open(OUT / "refs.json")) if r["addr"] >= DATA_START]
formed = {r["addr"] for r in refs if r["kind"] == "form"}
CANDS = [(0x80152818,0x3b8),(0x8014A250,0x808),(0x80144030,0x304),(0x8012E700,0x44),(0x8014A0C8,0x4c),
         (0x80151FC8,0x78),(0x80140808,0x78),(0x80150B7C,0x98),(0x80154660,0x190),(0x8017A510,0x48),
         (0x80151CF4,0x50),(0x801406C0,0x3c),(0x80139320,0x40),(0x801526A8,0xc),(0x80149A78,0x20),(0x80156D38,0x14),(0x80140420,0x54),(0x80111998,0x440)]
for b, S in CANDS:
    idx = {(r["addr"] - b) % S for r in refs if r.get("indexed") and r.get("stride") == S and b <= r["addr"] < b + 64 * S}
    line = []
    for k in range(0, 12):
        el = {r["addr"] for r in refs if b + k * S <= r["addr"] < b + (k + 1) * S}
        nf = len({r["f"] for r in refs if b + k * S <= r["addr"] < b + (k + 1) * S})
        ok = sum(1 for a in el if (a - b) % S in idx)
        line.append(f"{k}:{len(el)}a/{ok}ok/{nf}f")
    ends = sorted((a - b) // S for a in formed if a > b and (a - b) % S == 0 and (a - b) // S <= 70)
    print(f"{b:08X} S={S:#x} idx_offs={len(idx)} end_ptr_k={ends}\n   " + " ".join(line))
