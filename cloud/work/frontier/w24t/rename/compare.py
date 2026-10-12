#!/usr/bin/env python3
"""w24t: prove a compiled ultralib object's .text is the retail ROM's bytes.

    python3 cloud/work/frontier/w24t/rename/compare.py OBJ VRAM [ROM]

Compares the object's whole .text (every function, in object order) with
the ROM words at VRAM (boot segment: rom = vram - 0x80000400 + 0x1000).
Relocated fields are compared by VALUE: R_MIPS_26 targets and HI16/LO16
pairs are resolved from the ROM words and reported with the project symbol
defined at that address (symbol_addrs.us.txt), next to the ultralib name the
object uses. Section-relative relocations (statics, local data) must resolve
to one consistent section base. Every other bit must be equal.
Prints one row per function and per relocation; exit 0 iff identical.
"""
import re
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(REPO / "tools" / "recomp"))
import replace as rp  # noqa: E402  (stdlib ELF reader with relocations)

R_26, R_HI, R_LO = 4, 5, 6


def project_symbols():
    out = {}
    for line in (REPO / "symbol_addrs.us.txt").read_text().splitlines():
        m = re.match(r"^\s*([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", line)
        if m:
            out.setdefault(int(m.group(2), 16), []).append(m.group(1))
    return out


def main(obj_path, vram, rom_path=None):
    rom = Path(rom_path or REPO / "baserom.us.z64").read_bytes()
    data = Path(obj_path).read_bytes()
    elf = rp.read_elf(data)
    text = next(s for s in elf["sections"] if s["name"] == ".text")
    words = struct.unpack_from(">%dI" % (text["size"] // 4), data, text["offset"])
    rom_off = vram - 0x80000400 + 0x1000
    retail = struct.unpack_from(">%dI" % len(words), rom, rom_off)
    syms = project_symbols()
    relocs = sorted((r for r in elf["relocs"] if r["section"] == ".text"), key=lambda r: r["offset"])
    masked = {}
    ok = True
    rows = []
    bases = {}
    pending_hi = {}
    for r in relocs:
        i = r["offset"] // 4
        sym = r["sym"] or {}
        name = sym.get("name") or sym.get("section") or "?"
        local = sym.get("type") == 3 or not sym.get("name")        # STT_SECTION
        if r["type"] == R_26:
            masked[i] = 0x03FFFFFF
            target = 0x80000000 | ((retail[i] & 0x03FFFFFF) << 2)
            if local:
                want_off = (words[i] & 0x03FFFFFF) << 2
                base = target - want_off
                bases.setdefault(name, set()).add(base)
                rows.append((i, "jal", name + "+0x%X" % want_off, target))
            else:
                rows.append((i, "jal", name, target))
        elif r["type"] == R_HI:
            masked[i] = 0xFFFF
            pending_hi.setdefault((name, local), []).append(i)
        elif r["type"] == R_LO:
            masked[i] = 0xFFFF
            his = pending_hi.get((name, local)) or []
            if not his:
                print("  LO16 without HI16 at +0x%X (%s)" % (4 * i, name))
                ok = False
                continue
            h = his[-1]
            lo = retail[i] & 0xFFFF
            addr = ((retail[h] & 0xFFFF) << 16) + (lo - 0x10000 if lo & 0x8000 else lo)
            olo = words[i] & 0xFFFF
            oaddr = ((words[h] & 0xFFFF) << 16) + (olo - 0x10000 if olo & 0x8000 else olo)
            if local:
                bases.setdefault(name, set()).add((addr - oaddr) & 0xFFFFFFFF)
                rows.append((i, "hi/lo", "%s+0x%X" % (name, oaddr), addr))
            else:
                rows.append((i, "hi/lo", name + ("+0x%X" % oaddr if oaddr else ""), addr - oaddr))
    diffs = [i for i, (a, b) in enumerate(zip(words, retail)) if (a ^ b) & ~masked.get(i, 0) & 0xFFFFFFFF]
    funcs = sorted((s for s in elf["symbols"] if s["type"] == 2 and s["section"] == ".text"),
                   key=lambda s: s["value"])
    print("%s .text %d bytes vs ROM 0x%X (vram 0x%08X)" % (Path(obj_path).name, 4 * len(words), rom_off, vram))
    for f in funcs:
        a = vram + f["value"]
        print("  function %-24s +0x%03X size 0x%03X -> 0x%08X  project: %s"
              % (f["name"], f["value"], f["size"], a, ",".join(syms.get(a, ["(none)"]))))
    seen = set()
    for i, kind, name, addr in rows:
        key = (kind, name, addr)
        if key in seen:
            continue
        seen.add(key)
        print("  reloc %-5s %-28s -> 0x%08X  project: %s" % (kind, name, addr, ",".join(syms.get(addr, ["(none)"]))))
    for name, b in sorted(bases.items()):
        print("  section %s base %s" % (name, ",".join("0x%08X" % x for x in sorted(b))))
        if len(b) != 1:
            ok = False
    if diffs:
        ok = False
        for i in diffs[:20]:
            print("  DIFF +0x%03X obj %08X rom %08X" % (4 * i, words[i], retail[i]))
    print("RESULT: %s (%d words, %d relocated fields compared by value, %d differing words)"
          % ("IDENTICAL" if ok else "DIFFERENT", len(words), len(masked), len(diffs)))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1], int(sys.argv[2], 16), *(sys.argv[3:4])))
