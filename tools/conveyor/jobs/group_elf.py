"""Read one function of an IDO whole-program group object as linked words.

A group search compiles a whole IPA call group (cc -j, uld -kp, umerge, uopt,
ugen, as1) and needs to score ONE function of it against its retail bytes. The
object's words carry zeroed relocation fields; the retail words are linked. This
module reads the ELF32 big-endian object directly (a node has no binutils),
applies the relocations that resolve to known image addresses, and returns the
target function's words as they would sit in the image.

Fields it cannot resolve (data in the unit's own .data/.rodata, a call into a
stand-in, a symbol the image map lacks) take the retail word's field at the
same position, so they never count as a difference. That is a search signal
only: the splice gate (blob_group) compares the real linked bytes.
"""
import struct

R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 4, 5, 6


class ObjectError(Exception):
    pass


def _u32(data, off):
    return struct.unpack_from(">I", data, off)[0]


def _u16(data, off):
    return struct.unpack_from(">H", data, off)[0]


class Obj:
    """The parts of an ELF32-BE relocatable a group scorer needs."""

    def __init__(self, data):
        if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 2:
            raise ObjectError("not an ELF32 big-endian object")
        self.data = data
        shoff, shentsize = _u32(data, 0x20), _u16(data, 0x2E)
        shnum, shstrndx = _u16(data, 0x30), _u16(data, 0x32)
        raw = []
        for i in range(shnum):
            o = shoff + i * shentsize
            raw.append(struct.unpack_from(">10I", data, o))
        names = raw[shstrndx]
        self.sections = []          # (name, type, offset, size, link, info)
        for name, typ, _fl, _addr, off, size, link, info, _al, _es in raw:
            self.sections.append((self._cstr(names[4] + name), typ, off, size, link, info))
        self.text_index = self._find(".text")
        self.text = self._bytes(self.text_index)
        self.symbols = self._read_symbols()
        self.relocs = self._read_relocs()

    def _cstr(self, off):
        end = self.data.index(b"\0", off)
        return self.data[off:end].decode()

    def _find(self, name):
        for i, sec in enumerate(self.sections):
            if sec[0] == name:
                return i
        raise ObjectError(f"no {name} section")

    def _bytes(self, index):
        _, _, off, size, _, _ = self.sections[index]
        return self.data[off:off + size]

    def _read_symbols(self):
        """[(name, value, size, shndx)] in symbol-table order."""
        try:
            idx = next(i for i, s in enumerate(self.sections) if s[1] == 2)   # SHT_SYMTAB
        except StopIteration:
            raise ObjectError("no symbol table")
        _, _, off, size, link, _ = self.sections[idx]
        strtab = self.sections[link][2]
        out = []
        for o in range(off, off + size, 16):
            name, value, sz, _info, _other, shndx = struct.unpack_from(">IIIBBH", self.data, o)
            out.append((self._cstr(strtab + name) if name else "", value, sz, shndx))
        return out

    def _read_relocs(self):
        """[(text offset, type, symbol index)] from .rel.text."""
        try:
            idx = self._find(".rel.text")
        except ObjectError:
            return []
        _, _, off, size, _, _ = self.sections[idx]
        out = []
        for o in range(off, off + size, 8):
            r_offset, r_info = struct.unpack_from(">II", self.data, o)
            out.append((r_offset, r_info & 0xFF, r_info >> 8))
        return out

    def functions(self):
        """{name: text offset} for named symbols defined in .text, in offset order."""
        found = {name: value for name, value, _, shndx in self.symbols
                 if name and shndx == self.text_index}
        return dict(sorted(found.items(), key=lambda kv: kv[1]))


def _sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def target_words(obj, target, size, slices, symbols, retail):
    """The target function's words as linked in the image.

    obj      Obj of the compiled group
    target   name of the function to read
    size     retail size in bytes (words beyond it are kept only if non-zero)
    slices   {function name: image vaddr} for every function of the group
    symbols  {name: image address} for everything else the group can name
    retail   the retail words (bytes), used for fields that cannot be resolved
    """
    funcs = obj.functions()
    if target not in funcs:
        raise ObjectError(f"{target} is not defined in the group object")
    start = funcs[target]
    later = [v for v in funcs.values() if v > start]
    end = min(later + [len(obj.text)])
    words = bytearray(obj.text[start:end])
    keep = len(words)
    while keep > size and not any(words[keep - 4:keep]):
        keep -= 4
    del words[max(keep, min(size, len(words))):]

    order = sorted((off, name) for name, off in funcs.items())

    def text_addr(offset):
        owner = None
        for off, name in order:
            if off <= offset:
                owner = (off, name)
            else:
                break
        if owner is None or owner[1] not in slices:
            return None
        return slices[owner[1]] + (offset - owner[0])

    def resolve(sym, addend):
        name, value, _, shndx = sym
        if shndx == obj.text_index:
            return text_addr(value + addend) if name == "" or name in funcs else None
        if name and name in symbols:
            return symbols[name] + addend
        return None

    def word(o):
        return _u32(words, o)

    def put(o, w):
        struct.pack_into(">I", words, o, w & 0xFFFFFFFF)

    def retail_word(o):
        return _u32(retail, o) if o + 4 <= len(retail) else None

    def borrow(o, mask):
        """Take the masked field from the retail word at this position."""
        r = retail_word(o)
        put(o, (word(o) & ~mask) | ((r or 0) & mask))

    rels = sorted((o - start, t, s) for o, t, s in obj.relocs if start <= o < start + len(words))
    pending = []                       # (word offset, symbol index)
    for off, typ, sidx in rels:
        sym = obj.symbols[sidx]
        insn = word(off)
        if typ == R_MIPS_26:
            target_addr = resolve(sym, (insn & 0x03FFFFFF) << 2)
            if target_addr is None:
                borrow(off, 0x03FFFFFF)
            else:
                put(off, (insn & 0xFC000000) | ((target_addr >> 2) & 0x03FFFFFF))
        elif typ == R_MIPS_HI16:
            pending.append((off, sidx))
        elif typ == R_MIPS_LO16:
            his = [h for h, s in pending if s == sidx]
            pending = [(h, s) for h, s in pending if s != sidx]
            hi_imm = (word(his[0]) & 0xFFFF) if his else 0
            value = resolve(sym, (hi_imm << 16) + _sext16(insn & 0xFFFF))
            if value is None:
                for h in his:
                    borrow(h, 0xFFFF)
                borrow(off, 0xFFFF)
            else:
                for h in his:
                    put(h, (word(h) & 0xFFFF0000) | (((value + 0x8000) >> 16) & 0xFFFF))
                put(off, (insn & 0xFFFF0000) | (value & 0xFFFF))
    for h, _ in pending:                       # a HI16 whose LO16 left the slice
        borrow(h, 0xFFFF)
    return bytes(words)
