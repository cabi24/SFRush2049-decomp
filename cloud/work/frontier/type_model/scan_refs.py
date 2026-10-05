"""Scan every game function for absolute-address materialisations.

Linear per-function register tracking (no CFG):
  hi   : register holds lui value
  abs  : register holds a full address (lui+addiu/ori)
  idx  : register holds address/hi plus a scaled index (addu with a non-address reg)
  mul  : register holds k * token (sll/addu/subu chains) -> stride recovery
Emits refs.json: list of dicts
  {f, pc, addr, kind: form|load|store, mn, base, off, indexed, stride}
`addr` is the resolved absolute address; `base`/`off` are set when the access
went through a register holding a formed base (struct-style access).
Also emits regoff.json: per function histogram of reg+offset accesses whose
base register is NOT a known absolute (pointer-based struct access), sp excluded.
"""
import json, collections
from common import *

LOADS = {0x20:"lb",0x21:"lh",0x22:"lwl",0x23:"lw",0x24:"lbu",0x25:"lhu",0x26:"lwr",
         0x31:"lwc1",0x35:"ldc1"}
STORES = {0x28:"sb",0x29:"sh",0x2a:"swl",0x2b:"sw",0x2e:"swr",0x39:"swc1",0x3d:"sdc1"}
CALLER = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25,31]
def sx(v): return v - 0x10000 if v & 0x8000 else v

refs = []; regoff = {}; prefs = []
for vaddr, size, name in FUNCS:
    st = {}            # reg -> tuple
    ver = collections.Counter()
    hist = collections.Counter()
    pend_call = 0
    def tok(r): return ("r", r, ver[r])
    def mul(r):
        s = st.get(r)
        if s and s[0] == "mul": return s[1], s[2]
        return 1, tok(r)
    def setr(r, val=None):
        if r == 0: return
        ver[r] += 1
        if val is None: st.pop(r, None)
        else: st[r] = val
    for pc in range(vaddr, vaddr + size, 4):
        w = word(pc)
        op = w >> 26; rs = (w >> 21) & 31; rt = (w >> 16) & 31; rd = (w >> 11) & 31
        imm = w & 0xFFFF; sa = (w >> 6) & 31; fn = w & 63
        clear_after = pend_call; pend_call = 0
        if op == 0x0F:
            setr(rt, ("hi", imm << 16))
        elif op in (9, 0xD):       # addiu / ori
            s = st.get(rs)
            if s and s[0] == "hi":
                a = (s[1] + (sx(imm) if op == 9 else imm)) & 0xFFFFFFFF
                refs.append(dict(f=name, pc=pc, addr=a, kind="form", mn="addiu" if op==9 else "ori"))
                setr(rt, ("abs", a))
            elif s and s[0] == "abs" and op == 9:
                setr(rt, ("abs", (s[1] + sx(imm)) & 0xFFFFFFFF))
            elif s and s[0] == "idx" and op == 9:
                # lui at; addu at,at,idx; addiu rX,at,lo  -> &array[idx]
                a = (s[1] + sx(imm)) & 0xFFFFFFFF
                if s[3]:
                    refs.append(dict(f=name, pc=pc, addr=a, kind="form", mn="addiu", indexed=True, stride=s[2]))
                setr(rt, ("idx", a, s[2], False))
            else:
                setr(rt)
        elif op in LOADS or op in STORES:
            mn = LOADS.get(op) or STORES[op]
            kind = "load" if op in LOADS else "store"
            s = st.get(rs)
            off = sx(imm)
            if s and s[0] == "hi":
                refs.append(dict(f=name, pc=pc, addr=(s[1]+off)&0xFFFFFFFF, kind=kind, mn=mn))
            elif s and s[0] == "abs":
                refs.append(dict(f=name, pc=pc, addr=(s[1]+off)&0xFFFFFFFF, kind=kind, mn=mn, base=s[1], off=off))
            elif s and s[0] == "idx":
                if s[3]:   # hi-only + index: offset is the low half
                    refs.append(dict(f=name, pc=pc, addr=(s[1]+off)&0xFFFFFFFF, kind=kind, mn=mn, indexed=True, stride=s[2]))
                else:
                    refs.append(dict(f=name, pc=pc, addr=(s[1]+off)&0xFFFFFFFF, kind=kind, mn=mn, base=s[1], off=off, indexed=True, stride=s[2]))
            elif s and s[0] == "ptr":
                prefs.append(dict(f=name, pc=pc, ptr=s[1], off=off, mn=mn, kind=kind))
                hist[(mn, off)] += 1
            elif rs != 29:
                hist[(mn, off)] += 1
            if op in LOADS and op < 0x30:
                src = st.get(rs)
                if mn == "lw" and src and src[0] in ("hi", "abs") :
                    setr(rt, ("ptr", (src[1] + off) & 0xFFFFFFFF))
                else:
                    setr(rt)
        elif op == 0:
            if fn == 0 and w != 0:           # sll
                k, t = mul(rt); setr(rd, ("mul", k << sa, t))
            elif fn in (0x21, 0x23):         # addu / subu
                a, b = st.get(rs), st.get(rt)
                if fn == 0x21 and rt == 0 and a:                 # move
                    setr(rd, a)
                elif fn == 0x21 and rs == 0 and b:
                    setr(rd, b)
                elif fn == 0x21 and a and a[0] in ("hi", "abs") and not (b and b[0] in ("hi","abs","idx")):
                    setr(rd, ("idx", a[1], mul(rt)[0], a[0] == "hi"))
                elif fn == 0x21 and b and b[0] in ("hi", "abs") and not (a and a[0] in ("hi","abs","idx")):
                    setr(rd, ("idx", b[1], mul(rs)[0], b[0] == "hi"))
                elif fn == 0x21 and a and a[0] == "idx" and not (b and b[0] in ("hi","abs","idx")):
                    setr(rd, ("idx", a[1], 0, a[3]))
                elif fn == 0x21 and b and b[0] == "idx" and not (a and a[0] in ("hi","abs","idx")):
                    setr(rd, ("idx", b[1], 0, b[3]))
                else:
                    k1, t1 = mul(rs); k2, t2 = mul(rt)
                    if t1 == t2:
                        setr(rd, ("mul", k1 + k2 if fn == 0x21 else k1 - k2, t1))
                    else:
                        setr(rd)
            elif fn in (8,):                 # jr
                pass
            elif fn == 9:                    # jalr
                pend_call = 1
            elif fn in (0x18,0x19,0x1a,0x1b,0x11,0x13,0x0c,0x0d): pass
            else:
                setr(rd)
        elif op == 3:
            pend_call = 1
        elif op in (8, 0xA, 0xB, 0xC, 0xE):
            setr(rt)
        elif op == 0x11 and rs in (0, 2):    # mfc1/cfc1
            setr(rt)
        if clear_after:                      # delay slot executed; call clobbers
            for r in CALLER: setr(r)
    regoff[name] = [[mn, off, n] for (mn, off), n in sorted(hist.items())]

json.dump(refs, open(OUT / "refs.json", "w"))
json.dump(regoff, open(OUT / "regoff.json", "w"))
json.dump(prefs, open(OUT / "ptr_refs.json", "w"))
print("pointer-global field refs", len(prefs))
import collections
c = collections.Counter(r["kind"] for r in refs)
print("refs", len(refs), dict(c))
def where(a):
    if a < 0x80000400: return "low"
    if a < BASE: return "static(<80086A50)"
    if a < CODE_END: return "game code"
    if a < END: return "game data"
    return "after image (bss)"
print(collections.Counter(where(r["addr"]) for r in refs))
print("distinct addrs", len({r["addr"] for r in refs}))
