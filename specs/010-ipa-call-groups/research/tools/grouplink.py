"""grouplink: place each member slice of a multi-function IDO object at its own
image address, resolving relocations (S4 spike).

slices: {target_id: (obj_offset, rom_vaddr, size_bytes)}
Returns {target_id: bytes}. Supports R_MIPS_26 and HI16/LO16 (REL) in .text.
"""
import re
import struct
import subprocess

READELF = "mips-linux-gnu-readelf"
OBJCOPY = "mips-linux-gnu-objcopy"


def _text(obj):
    out = subprocess.run([OBJCOPY, "-O", "binary", "-j", ".text", obj, "/dev/stdout"],
                         capture_output=True, check=True).stdout
    return bytearray(out)


def _symbols(obj):
    """{name: (value, section_index)} for defined symbols; undefined -> None."""
    out = subprocess.run([READELF, "-sW", obj], capture_output=True, text=True, check=True).stdout
    syms = {}
    for line in out.splitlines():
        parts = line.split()
        if len(parts) >= 8 and parts[0].rstrip(":").isdigit():
            value, ndx, name = int(parts[1], 16), parts[6], parts[7]
            syms[name] = None if ndx == "UND" else (value, ndx)
    return syms


def _relocs(obj):
    out = subprocess.run([READELF, "-rW", obj], capture_output=True, text=True, check=True).stdout
    rels, section = [], None
    for line in out.splitlines():
        m = re.match(r"Relocation section '(\.rel\.\S+)'", line)
        if m:
            section = m.group(1)
            continue
        m = re.match(r"\s*([0-9a-f]{8})\s+[0-9a-f]{8}\s+(R_MIPS_\w+)\s+([0-9a-f]{8})\s+(\S+)", line)
        if m and section == ".rel.text":
            rels.append((int(m.group(1), 16), m.group(2), int(m.group(3), 16), m.group(4)))
    return rels


def link(obj, slices, extern):
    """extern: {name: address} for symbols outside the object."""
    text = _text(obj)
    syms = _symbols(obj)
    text_ndx = next(v[1] for k, v in syms.items() if v and k in slices)

    def text_addr(offset):
        for tid, (off, vaddr, size) in slices.items():
            if off <= offset < off + size:
                return vaddr + (offset - off)
        raise KeyError(f"text offset 0x{offset:x} is in no member slice")

    def resolve(name, symval, addend):
        """Absolute address of (symbol + addend)."""
        if name == ".text" or (syms.get(name) and syms[name][1] == text_ndx):
            base = 0 if name == ".text" else syms[name][0]
            return text_addr(base + addend)
        if name in extern:
            return extern[name] + addend
        raise KeyError(f"unresolved symbol {name}")

    word = lambda o: struct.unpack(">I", text[o:o + 4])[0]
    put = lambda o, w: text.__setitem__(slice(o, o + 4), struct.pack(">I", w & 0xFFFFFFFF))
    rels = _relocs(obj)
    pending_hi = []
    for offset, rtype, symval, name in rels:
        insn = word(offset)
        if rtype == "R_MIPS_26":
            addend = (insn & 0x03FFFFFF) << 2
            target = resolve(name, symval, addend)
            put(offset, (insn & 0xFC000000) | ((target >> 2) & 0x03FFFFFF))
        elif rtype == "R_MIPS_HI16":
            pending_hi.append((offset, name, insn))
        elif rtype == "R_MIPS_LO16":
            lo = struct.unpack(">h", struct.pack(">H", insn & 0xFFFF))[0]
            his = [h for h in pending_hi if h[1] == name]
            hi_imm = (his[0][2] & 0xFFFF) if his else 0
            value = resolve(name, symval, (hi_imm << 16) + lo)
            for h_off, _, h_insn in his:
                put(h_off, (h_insn & 0xFFFF0000) | (((value + 0x8000) >> 16) & 0xFFFF))
            pending_hi = [h for h in pending_hi if h[1] != name]
            put(offset, (insn & 0xFFFF0000) | (value & 0xFFFF))
        else:
            raise ValueError(f"unsupported relocation {rtype} at 0x{offset:x}")
    return {tid: bytes(text[off:off + size]) for tid, (off, vaddr, size) in slices.items()}
