#!/usr/bin/env python3
"""Deleted-procedure stubs: retail vs a whole-program object.

    python3 wp/stubs.py [OBJ LO HI]

Counts retail bodies that are only `jr ra; nop` and have no jal caller, and
(optionally) prints retail order next to an object's emitted order for an
address range, marking 2-word stubs.
"""
import sys
from pathlib import Path
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import wp  # noqa: E402

callers, addr = wp.real_callgraph()
tg = wp.score.targets()
lock = wp.load_lock()


def is_stub(w):
    return w[:2] == [0x03E00008, 0] and len(w) <= 2


stubs = [n for n, w in tg.items() if is_stub(w)]
nocall = [n for n in stubs if not callers.get(n)]
print(f"retail: {len(tg)} functions, {sum(len(w) for w in tg.values())} words")
print(f"retail `jr ra; nop` bodies: {len(stubs)}; with no jal caller: {len(nocall)}; "
      f"of those locked as empty C functions: {sum(n in lock for n in nocall)}")
if len(sys.argv) > 3:
    obj, lo, hi = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3], 16)
    inv = {a: n for n, a in addr.items() if n in tg}
    print("RETAIL order:")
    print("  " + " ".join(n + ("[STUB]" if is_stub(tg[n]) else "")
                          for a, n in sorted(inv.items()) if lo <= a < hi))
    syms = wp.score.symbols(obj)
    words = wp.score.text_words(obj)
    order = sorted(syms.items(), key=lambda kv: kv[1])
    out = []
    for (n, o), (n2, o2) in zip(order, order[1:] + [("END", len(words) * 4)]):
        out.append(n + ("[STUB]" if is_stub(words[o // 4:o2 // 4]) else ""))
    print("OBJECT order:")
    print("  " + " ".join(out))
