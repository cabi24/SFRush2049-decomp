"""Classify the opaque data run 0x8010FD7C..end of build/game_code.bin.

Word-level classes (priority order):
  jtbl    word -> mid-function code address, in a run reached by a jr-table load
  fnptr   word -> function start
  codeptr word -> other code address
  str     printable ASCII run (>=4 chars, NUL terminated), padded to 4
  dptr    word -> game data range
  bptr    word -> BSS after image (below 0x80400000)
  sptr    word -> static segment (0x80000400..0x80086A50)
  dbl     8 bytes referenced by ldc1
  flt     referenced by lwc1/swc1
  zero    zero word
  fltlike unreferenced word that decodes to a 'plausible' float
  other   everything else (ints, packed structs, tables)
Then merges into address-ordered sub-ranges and attributes references.
"""
import json, struct, collections, bisect, sys
from common import *
refs = json.load(open(OUT / "refs.json"))
N = (END - DATA_START) // 4
cls = [None] * N
fstarts = {v for v, s, n in FUNCS}
def W(i): return word(DATA_START + 4 * i)
# strings first (byte level)
data = IMG[DATA_START - BASE:]
isstr = bytearray(len(data))
i = 0
while i < len(data):
    if i % 4 == 0:
        j = i
        while j < len(data) and (32 <= data[j] < 127 or data[j] in (9, 10, 13)):
            j += 1
        if j - i >= 4 and j < len(data) and data[j] == 0:
            e = (j + 4) & ~3
            if not any(data[j:e]):
                for k in range(i, e): isstr[k] = 1
                i = e; continue
    i += 4 if i % 4 == 0 else 1
byaddr = collections.defaultdict(list)
for r in refs:
    if DATA_START <= r["addr"] < END: byaddr[r["addr"]].append(r)
for i in range(N):
    a = DATA_START + 4 * i; w = W(i)
    mns = {r["mn"] for r in byaddr.get(a, [])}
    if "ldc1" in mns:
        cls[i] = "dbl"
        if i + 1 < N: cls[i + 1] = "dbl"
        continue
    if cls[i]: continue
    if isstr[4 * i]:
        # a 4-char 'string' could be a pointer/float; require not referenced as float
        cls[i] = "str"; continue
    if mns & {"lwc1", "swc1"}: cls[i] = "flt"
    elif w == 0: cls[i] = "zero"
    elif BASE <= w < CODE_END and w % 4 == 0:
        cls[i] = "fnptr" if w in fstarts else "codeptr"
    elif DATA_START <= w < END: cls[i] = "dptr"
    elif END <= w < 0x80400000: cls[i] = "bptr"
    elif 0x80000400 <= w < BASE: cls[i] = "sptr"
    else:
        e = (w >> 23) & 0xFF
        if 0x67 <= e <= 0x9E and (w & 0xFFF) == 0: cls[i] = "fltlike"
        else: cls[i] = "other"
# jump tables: runs of codeptr (>=2) -> jtbl
i = 0
while i < N:
    if cls[i] == "codeptr":
        j = i
        while j < N and cls[j] == "codeptr": j += 1
        if j - i >= 2:
            for k in range(i, j): cls[k] = "jtbl"
        i = j
    else: i += 1
tot = collections.Counter(cls)
print("total bytes", 4 * N)
for k, v in tot.most_common(): print(f"  {k:8s} {4*v:7d}  {100*v/N:5.1f}%")
json.dump(cls, open(OUT / "data_classes.json", "w"))
# coarse map: 0x200-byte windows with dominant classes
print("\ncoarse map (0x400 windows): addr, composition")
for s in range(0, N, 256):
    c = collections.Counter(cls[s:s + 256])
    a = DATA_START + 4 * s
    nref = sum(len(byaddr.get(DATA_START + 4 * k, [])) for k in range(s, min(N, s + 256)))
    print(f"{a:08X} refs={nref:4d} " + " ".join(f"{k}:{4*v}" for k, v in c.most_common(5)))
