"""Shared loaders for the type-model survey (read-only against the repo)."""
import json, struct
from pathlib import Path
REPO = Path(__file__).resolve().parents[4]
OUT = Path(__file__).resolve().parent
BASE = 0x80086A50
IMG = (REPO / "build/game_code.bin").read_bytes()
END = BASE + len(IMG)
LAYOUT = json.loads((REPO / "build/blob_layout.json").read_text())
LOCK = json.loads((REPO / "blob_matched.lock.json").read_text())
FUNCS = []      # (vaddr, size, name)
OPAQUE = []     # (vaddr, size)
for region in LAYOUT["regions"]:
    for e in region["entries"]:
        if e["kind"] == "function":
            FUNCS.append((e["vaddr"], e["size"], e["target_id"]))
        else:
            OPAQUE.append((e["vaddr"], e["size"]))
FUNCS.sort()
MATCHED = set(LOCK)
DATA_START = max(OPAQUE)[0]          # 0x8010FD7C
CODE_END = DATA_START
def word(addr):
    return struct.unpack_from(">I", IMG, addr - BASE)[0]
def func_at(addr):
    import bisect
    i = bisect.bisect_right(FUNCS, (addr, 1 << 40, "")) - 1
    if i >= 0 and FUNCS[i][0] <= addr < FUNCS[i][0] + FUNCS[i][1]:
        return FUNCS[i]
    return None
