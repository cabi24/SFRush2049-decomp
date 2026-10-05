"""Arcade-ancestry probe: distinctive float literals in arcade game/*.c that also
appear in the N64 .rodata (owned by one function) -> candidate function pairs.
A literal is 'distinctive' if it is not representable as lui-only (low 16 bits
nonzero) and occurs in <=2 arcade source files."""
import re, struct, json, collections
from common import *
own = json.load(open(OUT / "rodata_owners.json"))["owner"]
n64 = collections.defaultdict(set)
for a, f in own.items():
    w = word(int(a, 16))
    if not func_at(w): n64[w].add(f)
arc = collections.defaultdict(set)
G = REPO / "reference/repos/rushtherock/game"
num = re.compile(r"(?<![\w.])(\d+\.\d*(?:[eE][-+]?\d+)?|\.\d+(?:[eE][-+]?\d+)?|\d+[eE][-+]?\d+)[fF]?")
for p in sorted(G.glob("*.c")):
    text = re.sub(r"/\*.*?\*/", "", p.read_text(errors="replace"), flags=re.S)
    for m in num.finditer(text):
        try: v = float(m.group(1))
        except ValueError: continue
        try: w = struct.unpack(">I", struct.pack(">f", v))[0]
        except OverflowError: continue
        if w & 0xFFFF: arc[w].add(p.name)
common_ = {w for w in n64 if w in arc}
print("distinct non-lui float literals: N64 rodata", len(n64), " arcade game/*.c", len(arc), " in both", len(common_))
rare = sorted(w for w in common_ if len(arc[w]) <= 2 and len(n64[w]) <= 3)
print("shared literals that are rare on both sides:", len(rare))
for w in rare:
    v = struct.unpack(">f", struct.pack(">I", w))[0]
    print(f"  {v:<14.8g} arcade {sorted(arc[w])}  N64 {sorted(n64[w])}")
