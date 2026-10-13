"""Minimal ELF32 big-endian reader for the GameCube debug build ``rush_d.elf``.

Standard library only. Shared by ``dwarf1.py``, ``disasm.py`` and ``correlate.py``.
"""
import os
import struct

REPO = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
DEFAULT_ELF = os.path.join(REPO, 'rush_d.elf')

SDA_BASE = 0x802AC300    # _SDA_BASE_  (r13, .sdata/.sbss)
SDA2_BASE = 0x802ADAC0   # _SDA2_BASE_ (r2, .sdata2/.sbss2)

STT = {0: 'NOTYPE', 1: 'OBJECT', 2: 'FUNC', 3: 'SECTION', 4: 'FILE'}


class Section(object):
    __slots__ = ('name', 'type', 'flags', 'addr', 'offset', 'size', 'index')

    def __init__(self, **kw):
        for k, v in kw.items():
            setattr(self, k, v)


class Symbol(object):
    __slots__ = ('name', 'value', 'size', 'type', 'bind', 'shndx', 'file')

    def __init__(self, **kw):
        for k, v in kw.items():
            setattr(self, k, v)


class Elf(object):
    def __init__(self, path=DEFAULT_ELF):
        with open(path, 'rb') as f:
            self.data = f.read()
        d = self.data
        if d[:4] != b'\x7fELF' or d[4] != 1 or d[5] != 2:
            raise ValueError('%s: not an ELF32 big-endian file' % path)
        shoff = struct.unpack_from('>I', d, 0x20)[0]
        shentsize, shnum, shstrndx = struct.unpack_from('>HHH', d, 0x2e)
        raw = [struct.unpack_from('>IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
        stroff = raw[shstrndx][4]
        self.sections = []
        for i, r in enumerate(raw):
            self.sections.append(Section(name=self._cstr(stroff + r[0]), type=r[1], flags=r[2],
                                         addr=r[3], offset=r[4], size=r[5], index=i))
        self.by_name = {s.name: s for s in self.sections}
        self._symbols = None

    def _cstr(self, off):
        e = self.data.index(b'\0', off)
        return self.data[off:e].decode('latin-1')

    def section_bytes(self, name):
        s = self.by_name[name]
        return self.data[s.offset:s.offset + s.size]

    def alloc_sections(self):
        """Allocated sections with file contents (SHF_ALLOC, not NOBITS)."""
        return [s for s in self.sections if s.flags & 2 and s.type != 8 and s.size]

    def read(self, addr, n):
        """Bytes at a virtual address, or None outside file-backed sections."""
        for s in self.alloc_sections():
            if s.addr <= addr and addr + n <= s.addr + s.size:
                o = s.offset + addr - s.addr
                return self.data[o:o + n]
        return None

    def section_of(self, addr):
        for s in self.sections:
            if s.flags & 2 and s.size and s.addr <= addr < s.addr + s.size:
                return s.name
        return None

    def cstring(self, addr, maxlen=512):
        """Printable NUL-terminated string at addr (len >= 2), or None."""
        sec = None
        for s in self.alloc_sections():
            if s.addr <= addr < s.addr + s.size:
                sec = s
                break
        if sec is None or sec.name in ('.text', '.init'):
            return None
        o = sec.offset + addr - sec.addr
        end = sec.offset + sec.size
        out = bytearray()
        while o < end and len(out) <= maxlen:
            c = self.data[o]
            if c == 0:
                break
            if not (0x20 <= c < 0x7f or c in (9, 10, 13)):
                return None
            out.append(c)
            o += 1
        if o >= end or self.data[o] != 0 or len(out) < 2:
            return None
        return out.decode('ascii')

    @property
    def symbols(self):
        if self._symbols is None:
            st = self.by_name['.symtab']
            strtab = self.sections[[s for s in self.sections if s.name == '.strtab'][0].index]
            out = []
            cur_file = None
            for i in range(st.size // 16):
                nm, value, size, info, other, shndx = struct.unpack_from('>IIIBBH', self.data, st.offset + i * 16)
                name = self._cstr(strtab.offset + nm) if nm else ''
                typ = STT.get(info & 15, str(info & 15))
                if typ == 'FILE':
                    cur_file = name
                out.append(Symbol(name=name, value=value, size=size, type=typ, bind=info >> 4,
                                  shndx=shndx, file=cur_file if info >> 4 == 0 else None))
            self._symbols = out
        return self._symbols

    def functions(self):
        """Linked FUNC symbols with a size, sorted by address (deduplicated)."""
        seen = {}
        for s in self.symbols:
            if s.type == 'FUNC' and s.size and s.value:
                seen.setdefault(s.value, s)
        return [seen[a] for a in sorted(seen)]
