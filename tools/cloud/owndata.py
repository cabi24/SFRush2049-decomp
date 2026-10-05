#!/usr/bin/env python3
"""Verify a compiled function's references to its object's own data sections.

A float literal or a switch jump table lives in the object's own `.rodata`,
and a function-local `static` in its own `.data`. The compiler refers to them
with HI16/LO16 pairs against a *section* symbol, so no symbol name says where
they belong in the game image. The retail words do: at the same instruction
pair the retail function encodes an absolute address. This module reads the
retail bytes at that address and requires them to equal the object's bytes at
the relocation's section offset.

    result = verify(obj, name, want_words, address=..., image=ImageData(...))
    result.sites       # .text offsets of relocation words that are proven
    result.failures    # concrete differences: never a match
    result.unverified  # references that could not be checked: not a match

Rules (see cloud/work/frontier/rodata/README.md):

* Every own-data reference starts a window that runs to the next referenced
  offset of the same section (references from any function in the object
  bound it) or to the section end.
* The compared length is at least the size the instruction loads (8 for
  ldc1/sdc1/ld/sd, else 4) and otherwise the window with trailing zero bytes
  removed, rounded up to a word: IDO pads sections to 16 bytes, and that
  padding is some other owner's data in the image.
* Words of the window that carry R_MIPS_32 relocations (jump-table entries)
  are mapped to image addresses first: a `.text` offset inside the function
  becomes the function's image address plus that offset.
* `.rodata` is laid out per function in the image, so all of one function's
  references into one read-only section must agree on a single section base.
  `.data` is laid out per translation unit; its windows are checked
  independently and reported with their addresses as a distinct class.
* Uninitialised sections (.bss, .sbss) have no bytes in the image, so they
  are verified by ADDRESS only (see `_verify_uninitialised` and
  cloud/work/frontier/bss/README.md). Each referenced object offset is
  placed on its own (per object, like .data), at the one address every
  retail word referring to it encodes; that address range must lie inside a
  known game .bss range (GAME_BSS) and must not overlap another object of
  the function. What this proves: once the objects are placed there, every
  code word that refers to them equals retail. It does not prove the
  objects' sizes, types or initial (zero) contents.

Standard library only: tools/cloud must run from a bare checkout.

    # maintainer: (re)generate the tracked retail data artefact
    python3 tools/cloud/owndata.py generate --image build/game_code.bin
    python3 tools/cloud/owndata.py check --image build/game_code.bin
"""
import argparse
from dataclasses import dataclass, field
import hashlib
import json
import re
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]

READ_ONLY = (".rodata", ".rdata", ".lit4", ".lit8")
WRITABLE = (".data", ".sdata")
OWN_SECTIONS = READ_ONLY + WRITABLE
UNINITIALISED = (".bss", ".sbss")

SHT_PROGBITS, SHT_SYMTAB, SHT_RELA, SHT_NOBITS, SHT_REL = 1, 2, 4, 8, 9
STT_FUNC, STT_SECTION = 2, 3
R_MIPS_32, R_MIPS_HI16, R_MIPS_LO16 = 2, 5, 6
DOUBLEWORD_OPCODES = (0x35, 0x3D, 0x37, 0x3F)      # ldc1, sdc1, ld, sd

# The game's zero-initialised storage: [start, end) image addresses. Nothing
# here is in the ROM; boot code zeroes it. Source: the static boot routine at
# 0x8000233C (work/boot/game_init/README.md) calls
# bzero(0x801249F0, 0x8017A640 - 0x801249F0); the linked game image ends at
# 0x801249F0 (asm/us/blob symbols.json). Add a range only with such evidence.
GAME_BSS = (
    (0x801249F0, 0x8017A640, "game .bss: zeroed by boot code at 0x80002350 (bzero)"),
)

# Bytes a load or store moves, by opcode; anything else that carries a LO16
# (addiu, ori: an address is formed) proves only one byte of the object.
ACCESS_WIDTH = {0x20: 1, 0x24: 1, 0x28: 1,                  # lb lbu sb
                0x21: 2, 0x25: 2, 0x29: 2,                  # lh lhu sh
                0x23: 4, 0x2B: 4, 0x31: 4, 0x39: 4,         # lw sw lwc1 swc1
                0x22: 4, 0x26: 4, 0x2A: 4, 0x2E: 4,         # lwl lwr swl swr
                0x27: 4,                                    # lwu
                0x35: 8, 0x3D: 8, 0x37: 8, 0x3F: 8}         # ldc1 sdc1 ld sd

ARTIFACT_NAME = "opaque.hex"
ARTIFACT_LINE = 32                                  # bytes per line


def section_class(name):
    """'rodata', 'data', 'bss' or None for a section name."""
    if name in READ_ONLY:
        return "rodata"
    if name in WRITABLE:
        return "data"
    if name in UNINITIALISED:
        return "bss"
    return None


def bss_range(address, size):
    """The GAME_BSS entry holding [address, address + size), or None."""
    for start, end, why in GAME_BSS:
        if start <= address and address + size <= end:
            return start, end, why
    return None


def _sext16(value):
    return value - 0x10000 if value & 0x8000 else value


# --- retail bytes ------------------------------------------------------------

class ImageData:
    """Retail bytes by image address, from any number of disjoint runs."""

    def __init__(self, runs):
        merged = []
        for address, data in sorted((int(a), bytes(d)) for a, d in runs):
            if merged and merged[-1][0] + len(merged[-1][1]) == address:
                merged[-1] = (merged[-1][0], merged[-1][1] + data)
            elif merged and merged[-1][0] + len(merged[-1][1]) > address:
                raise ValueError(f"overlapping retail data runs at 0x{address:08X}")
            else:
                merged.append((address, data))
        self.runs = merged

    @classmethod
    def from_image(cls, data, base):
        """The whole linked image (build/game_code.bin) at its base address."""
        return cls([(base, data)])

    @classmethod
    def from_artifact(cls, directory):
        """The tracked artefact written by `generate`, or None if absent.

        An artefact that is present but fails its SHA256SUMS is fatal, exactly
        like the target files: a reference must never verify against data of
        unknown provenance."""
        directory = Path(directory)
        path, sums = directory / ARTIFACT_NAME, directory / "SHA256SUMS"
        if not path.is_file() and not sums.is_file():
            return None
        try:
            expected = dict(reversed(line.split("  ", 1))
                            for line in sums.read_text(encoding="utf-8").splitlines())
            raw = path.read_bytes()
        except (OSError, ValueError) as exc:
            raise SystemExit(f"own-data integrity check failed for {directory}: {exc}")
        if hashlib.sha256(raw).hexdigest() != expected.get(ARTIFACT_NAME):
            raise SystemExit(f"own-data integrity check failed for {path}: SHA-256 mismatch; "
                             "regenerate with tools/cloud/owndata.py generate")
        runs = []
        for line in raw.decode("ascii").splitlines():
            if not line or line.startswith("#"):
                continue
            address, payload = line.split()
            runs.append((int(address, 16), bytes.fromhex(payload)))
        return cls(runs)

    def read(self, address, size):
        """`size` bytes at `address`, or None unless every byte is known."""
        for start, data in self.runs:
            if start <= address and address + size <= start + len(data):
                return data[address - start:address - start + size]
        return None


# --- a minimal ELF32 big-endian reader ---------------------------------------

class _Object:
    def __init__(self, path):
        data = Path(path).read_bytes()
        if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 2:
            raise ValueError(f"{path}: not a 32-bit big-endian ELF")
        shoff, = struct.unpack_from(">I", data, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from(">HHH", data, 0x2E)
        self.data = data
        self.sections = []
        for i in range(shnum):
            name, typ, _flags, _addr, off, size, link, info, _align, _entsize = \
                struct.unpack_from(">10I", data, shoff + i * shentsize)
            self.sections.append(dict(name=name, type=typ, off=off, size=size,
                                      link=link, info=info))
        strings = self.sections[shstrndx]
        for sec in self.sections:
            sec["name"] = self._string(strings, sec["name"])
        self._symbols = {}

    def _string(self, table, offset):
        raw = self.data[table["off"] + offset:table["off"] + table["size"]]
        return raw[:raw.index(b"\0")].decode()

    def raw(self, index):
        sec = self.sections[index]
        if sec["type"] != SHT_PROGBITS:
            return None
        return self.data[sec["off"]:sec["off"] + sec["size"]]

    def symbols(self, index):
        """The symbol table at section `index` (a relocation section's sh_link)."""
        if index not in self._symbols:
            sec = self.sections[index]
            if sec["type"] != SHT_SYMTAB:
                raise ValueError("relocation section does not link to a symbol table")
            names = self.sections[sec["link"]]
            out = []
            for k in range(sec["size"] // 16):
                st_name, value, size, info, _other, shndx = struct.unpack_from(
                    ">IIIBBH", self.data, sec["off"] + 16 * k)
                name = self._string(names, st_name)
                if (info & 0xF) == STT_SECTION and not name and shndx < len(self.sections):
                    name = self.sections[shndx]["name"]
                out.append(dict(name=name, value=value, size=size,
                                type=info & 0xF, section=shndx))
            self._symbols[index] = out
        return self._symbols[index]

    def functions(self):
        """[(name, value, section index)] for defined function symbols."""
        found = []
        for index, sec in enumerate(self.sections):
            if sec["type"] != SHT_SYMTAB:
                continue
            for sym in self.symbols(index):
                if sym["type"] == STT_FUNC and 0 < sym["section"] < len(self.sections):
                    found.append((sym["name"], sym["value"], sym["section"]))
        return found

    def relocations(self, target):
        """[(offset, type, symbol)] of every REL section applying to `target`."""
        out = []
        for sec in self.sections:
            if sec["info"] != target or sec["type"] not in (SHT_REL, SHT_RELA):
                continue
            if sec["type"] == SHT_RELA:
                raise ValueError(f"{sec['name']}: RELA relocations are not supported")
            syms = self.symbols(sec["link"])
            for k in range(sec["size"] // 8):
                offset, info = struct.unpack_from(">II", self.data, sec["off"] + 8 * k)
                if info >> 8 >= len(syms):
                    raise ValueError(f"{sec['name']}: relocation names a missing symbol")
                out.append((offset, info & 0xFF, syms[info >> 8]))
        return out


# --- verification ------------------------------------------------------------

@dataclass
class Reference:
    """One LO16 against an own section, with the HI16 words it completes."""
    section: int
    offset: int             # object offset inside that section
    lo_site: int            # .text offsets
    hi_sites: tuple
    opcode: int             # of the LO16 instruction


@dataclass
class Result:
    sites: set = field(default_factory=set)
    notes: list = field(default_factory=list)
    failures: list = field(default_factory=list)
    unverified: list = field(default_factory=list)
    # {section name: [(object lo, object hi, image address, class)]}
    placements: dict = field(default_factory=dict)
    zero: set = field(default_factory=set)      # (section name, image address)
    references: int = 0
    # Verified .bss references: [(section name, object offset, hi sites,
    # lo site, image address)]; sites are offsets in the function's section.
    bss_sites: list = field(default_factory=list)

    @property
    def ok(self):
        return not self.failures and not self.unverified

    def bases(self):
        """{section name: image address of the section start}, or ValueError
        when one section's windows sit at independent image addresses.

        Zero-initialised sections are left out: they are placed per object
        (see `bss`), never as one section base."""
        out = {}
        for name, windows in self.placements.items():
            if any(cls == "bss" for _lo, _hi, _address, cls in windows):
                continue
            found = {(address - lo) & 0xFFFFFFFF for lo, _hi, address, _cls in windows}
            if len(found) != 1:
                raise ValueError(f"own {name} windows sit at independent image addresses "
                                 f"{sorted(hex(b) for b in found)}")
            out[name] = found.pop()
        return out

    def bss(self):
        """{section name: {object offset: image address}} of verified
        zero-initialised objects."""
        out = {}
        for name, windows in self.placements.items():
            for lo, _hi, address, cls in windows:
                if cls == "bss":
                    out.setdefault(name, {})[lo] = address
        return out


def _locate(obj, name, start, limit):
    """(text section index, start, end) of the function's compared slice."""
    functions = obj.functions()
    mine = [f for f in functions if f[0] == name]
    if not mine:
        raise ValueError(f"{name} is not a defined function in the compiled object")
    _, value, text = mine[0]
    start = value if start is None else start
    size = obj.sections[text]["size"]
    end = min((v for _n, v, s in functions if s == text and v > start), default=size)
    return text, start, min(end, size, start + limit)


def _references(obj, text):
    """Every own-section HI16/LO16 reference in the text section, plus the
    .text offsets of own-section relocations that are not such a pair."""
    words = obj.raw(text)
    refs, stray = [], []
    pending, last = {}, {}
    for offset, rtype, sym in obj.relocations(text):
        if sym["type"] != STT_SECTION or sym["section"] == text:
            continue
        key = id(sym)
        if offset % 4 or offset + 4 > len(words):
            stray.append((offset, sym, "invalid relocation offset"))
            continue
        insn, = struct.unpack_from(">I", words, offset)
        if rtype == R_MIPS_HI16:
            pending.setdefault(key, []).append((offset, insn & 0xFFFF))
        elif rtype == R_MIPS_LO16:
            # A LO16 completes every HI16 waiting for its symbol; a further
            # LO16 with no new HI16 reuses the last one (lui shared by loads).
            his = pending.pop(key, None) or last.get(key)
            if not his:
                stray.append((offset, sym, "LO16 without a HI16"))
                continue
            last[key] = his
            if len({imm for _site, imm in his}) != 1:
                stray.append((offset, sym, "HI16 words disagree"))
                continue
            addend = (his[0][1] << 16) + _sext16(insn & 0xFFFF)
            refs.append((sym, Reference(sym["section"], sym["value"] + addend, offset,
                                        tuple(site for site, _imm in his), insn >> 26)))
        else:
            stray.append((offset, sym, f"relocation type {rtype}"))
    for his in pending.values():
        for site, _imm in his:
            stray.append((site, None, "unpaired HI16"))
    return refs, stray


def own_references(obj, name, start=None, limit=1 << 32):
    """Number of own-section relocation records inside the function (cheap
    test for callers that must not change behaviour when there are none)."""
    obj = _Object(obj)
    text, start, end = _locate(obj, name, start, limit)
    return sum(1 for offset, _rtype, sym in obj.relocations(text)
               if start <= offset < end and sym["type"] == STT_SECTION
               and sym["section"] != text)


def verify(obj, name, want, address=None, image=None, start=None, addresses=None):
    """Check the function's own-data references against retail bytes.

    obj        compiled object (the function may sit in `.text` or any section)
    name       its function symbol
    want       the retail words of the function
    address    the function's image address (needed for jump tables)
    image      ImageData holding retail bytes, or None (nothing verifies)
    start      section offset of the function, when not its symbol value
    addresses  name -> image address for other functions a table may enter
    """
    result = Result()
    try:
        obj = _Object(obj)
        text, start, end = _locate(obj, name, start, len(want) * 4)
        refs, stray = _references(obj, text)
    except (ValueError, struct.error, IndexError) as exc:
        result.failures.append(f"own data: {exc}")
        return result
    functions = sorted((value, fname) for fname, value, section in obj.functions()
                       if section == text)
    in_slice = lambda site: start <= site < end

    def want_word(site):
        return want[(site - start) // 4]

    for site, sym, why in stray:
        if in_slice(site):
            result.references += 1
            target = sym["name"] if sym else "own section"
            result.unverified.append(f"{target} at +0x{site - start:x}: {why}")

    starts = {}                     # section -> every referenced offset
    for _sym, ref in refs:
        starts.setdefault(ref.section, set()).add(ref.offset)

    # Group this function's references by the object window they open.
    windows = {}
    blocked = set()                 # sites that also serve an unproven reference
    for sym, ref in refs:
        sites = ref.hi_sites + (ref.lo_site,)
        if not in_slice(ref.lo_site):
            continue
        result.references += 1
        where = f"{sym['name']}+0x{ref.offset:x} at +0x{ref.lo_site - start:x}"
        if not all(in_slice(site) for site in ref.hi_sites):
            result.unverified.append(f"{where}: its HI16 is outside the function")
            blocked.update(sites)
            continue
        retail = {((want_word(site) & 0xFFFF) << 16) + _sext16(want_word(ref.lo_site) & 0xFFFF)
                  for site in ref.hi_sites}
        if len(retail) != 1:
            result.failures.append(f"own {sym['name']} {where}: the retail HI16 words "
                                   "paired with this LO16 encode different addresses")
            blocked.update(sites)
            continue
        windows.setdefault((ref.section, ref.offset), []).append(
            (retail.pop() & 0xFFFFFFFF, ref, where))

    def text_address(offset):
        """Image address of a .text offset, or (None, reason)."""
        if start <= offset < end:
            return (address + offset - start, None) if address is not None else \
                (None, "the function's image address is unknown")
        owner = None
        for value, fname in functions:
            if value <= offset and value != start:
                owner = (value, fname)
        if owner and addresses is not None:
            base = addresses(owner[1])
            if base is not None:
                return base + offset - owner[0], None
        return None, f"table entry .text+0x{offset:x} is outside the function"

    deltas = {}                     # read-only section -> {delta: where}
    uninitialised = []              # .bss windows, verified by address below
    for (section, offset), uses in sorted(windows.items()):
        sec = obj.sections[section]
        cls = section_class(sec["name"])
        sites = {site for _addr, ref, _where in uses for site in ref.hi_sites + (ref.lo_site,)}
        where = uses[0][2]
        label = f"own {sec['name']}"
        raw = obj.raw(section)
        if cls == "bss" and sec["type"] == SHT_NOBITS:
            uninitialised.append((section, offset, uses, sites))
            continue
        if cls is None or raw is None:
            result.unverified.extend(
                f"{w}: {sec['name']} is not initialised own data and cannot be checked by content"
                for _a, _r, w in uses)
            blocked.update(sites)
            continue
        retail_addresses = {addr for addr, _ref, _where in uses}
        if len(retail_addresses) != 1:
            result.failures.append(
                f"{label}+0x{offset:x}: retail reads this one object at different addresses "
                + ", ".join(f"0x{a:08X}" for a in sorted(retail_addresses)))
            blocked.update(sites)
            continue
        retail_address = retail_addresses.pop()
        if not 0 <= offset < len(raw):
            result.failures.append(f"{label} {where}: offset is outside the section")
            blocked.update(sites)
            continue
        stop = min((s for s in starts[section] if s > offset), default=len(raw))
        stop = min(stop, len(raw))
        got = bytearray(raw[offset:stop])
        problem = None
        entries = 0
        for roff, rtype, rsym in obj.relocations(section):
            if not offset <= roff < stop:
                continue
            if rtype != R_MIPS_32 or roff % 4 or roff + 4 > stop:
                problem = (True, f"unsupported relocation type {rtype} at +0x{roff:x} "
                                 "inside the referenced data")
                break
            addend, = struct.unpack_from(">I", got, roff - offset)
            if rsym["section"] == text:
                target, why = text_address(rsym["value"] + addend)
            else:
                base = addresses(rsym["name"]) if addresses is not None else None
                target, why = (base + addend, None) if base is not None else \
                    (None, f"table entry names {rsym['name']}, which has no image address")
            if target is None:
                problem = (False, why)
                break
            struct.pack_into(">I", got, roff - offset, target & 0xFFFFFFFF)
            entries += 1
        if problem:
            fatal, why = problem
            (result.failures if fatal else result.unverified).append(f"{label} {where}: {why}")
            blocked.update(sites)
            continue
        minimum = max(8 if ref.opcode in DOUBLEWORD_OPCODES else 4 for _a, ref, _w in uses)
        length = max(minimum, (len(bytes(got).rstrip(b"\0")) + 3) & ~3)
        length = min(length, len(got))
        got = bytes(got[:length])
        if image is None:
            result.unverified.extend(f"{w}: no retail data available" for _a, _r, w in uses)
            blocked.update(sites)
            continue
        expected = image.read(retail_address, length)
        if expected is None:
            result.unverified.extend(
                f"{w}: retail bytes at 0x{retail_address:08X} are not available"
                for _a, _r, w in uses)
            blocked.update(sites)
            continue
        if expected != got:
            first = next(i for i in range(0, length, 4) if expected[i:i + 4] != got[i:i + 4])
            kind = f"jump table entry {first // 4}" if entries else f"+0x{first:x}"
            result.failures.append(
                f"{label}+0x{offset:x} referenced at "
                + ", ".join(f"+0x{ref.lo_site - start:x}" for _a, ref, _w in uses)
                + f" differs from retail 0x{retail_address:08X} ({kind}: retail "
                f"{expected[first:first + 4].hex()}, got {got[first:first + 4].hex()}; "
                f"{length} bytes compared)")
            blocked.update(sites)
            continue
        if cls == "rodata":
            deltas.setdefault(section, {}).setdefault(
                (retail_address - offset) & 0xFFFFFFFF, []).append((offset, sites))
        result.placements.setdefault(sec["name"], []).append(
            (offset, offset + length, retail_address, cls))
        if not any(got):
            result.zero.add((sec["name"], retail_address))
        result.sites.update(sites)

    # .rodata is per function in the image: one base per read-only section.
    for section, found in deltas.items():
        if len(found) > 1:
            name_ = obj.sections[section]["name"]
            result.failures.append(
                f"own {name_}: this function's references disagree on the section's image "
                "address (" + ", ".join(
                    f"0x{base:08X} for " + "/".join(f"+0x{o:x}" for o, _s in sorted(uses))
                    for base, uses in sorted(found.items()))
                + "): the literals are not laid out as in retail")
            for uses in found.values():
                for _offset, sites in uses:
                    blocked.update(sites)
            result.placements.pop(name_, None)
    _verify_uninitialised(obj, uninitialised, starts, image, result, blocked)
    result.sites -= blocked
    result.bss_sites = [entry for entry in result.bss_sites
                        if not blocked & set(entry[2] + (entry[3],))]
    result.notes = _notes(result.placements, result.zero)
    return result


def _verify_uninitialised(obj, windows, starts, image, result, blocked):
    """Address-only verification of own .bss references (no bytes exist).

    windows   [(section index, object offset, uses, sites)], `uses` as in
              `verify`: [(retail address, Reference, where)]
    starts    section index -> every object offset any function references

    Each referenced object offset is one object, placed on its own:
      * every retail word pair referring to it encodes the same address
        (otherwise a failure: the source's objects are not retail's);
      * its extent is the gap to the next referenced offset of the section,
        or, for the last one, the widest access made to it (the section's
        tail is IDO alignment padding and proves nothing);
      * that extent lies inside one GAME_BSS range. An address with retail
        bytes (inside the image) is a failure: retail initialises it, so
        the object cannot be zero-initialised. Anything else outside the
        ranges stays unverified;
      * no two objects overlap in the image (a failure).
    A verified object adds a placement (class 'bss') and its sites."""
    candidates = []
    for section, offset, uses, sites in windows:
        sec = obj.sections[section]
        label = f"own {sec['name']}+0x{offset:x}"
        addresses = {addr for addr, _ref, _where in uses}
        at = ", ".join(f"+0x{ref.lo_site:x}" for _a, ref, _w in uses)
        if len(addresses) != 1:
            result.failures.append(
                f"{label} (referenced at {at}): retail reads this one object at different "
                "addresses " + ", ".join(f"0x{a:08X}" for a in sorted(addresses)))
            blocked.update(sites)
            continue
        address = addresses.pop()
        if not 0 <= offset < sec["size"]:
            result.failures.append(f"{label}: offset is outside the section "
                                   f"(0x{sec['size']:x} bytes)")
            blocked.update(sites)
            continue
        following = min((s for s in starts[section] if s > offset), default=None)
        if following is not None and following <= sec["size"]:
            size = following - offset
        else:
            widest = max(ACCESS_WIDTH.get(ref.opcode, 1) for _a, ref, _w in uses)
            size = min(widest, sec["size"] - offset)
        if bss_range(address, size) is None:
            inside = image is not None and image.read(address, 1) is not None
            message = (f"{label} (referenced at {at}): retail address 0x{address:08X} "
                       f"(+{size} bytes) is not in a known game .bss range")
            if inside:
                result.failures.append(message + "; retail has initialised bytes there")
            else:
                result.unverified.append(message)
            blocked.update(sites)
            continue
        candidates.append((address, address + size, sec["name"], offset, uses, sites, label))
    clash = set()
    ordered = sorted(candidates, key=lambda c: (c[0], c[1]))
    for i, first in enumerate(ordered):
        for j in range(i + 1, len(ordered)):
            second = ordered[j]
            if second[0] >= first[1]:
                break
            result.failures.append(
                f"{first[6]} at 0x{first[0]:08X}..0x{first[1]:08X} and {second[6]} at "
                f"0x{second[0]:08X}..0x{second[1]:08X} overlap: distinct objects of the "
                "source would share retail storage")
            clash.update((i, j))
    for i, (address, end, name, offset, uses, sites, _label) in enumerate(ordered):
        if i in clash:
            blocked.update(sites)
            continue
        result.placements.setdefault(name, []).append((offset, offset + end - address,
                                                       address, "bss"))
        result.sites.update(sites)
        for _a, ref, _w in uses:
            result.bss_sites.append((name, offset, ref.hi_sites, ref.lo_site, address))


def _notes(placements, zero=()):
    notes = []
    for name, windows in sorted(placements.items()):
        merged = []
        for lo, hi, address, cls in sorted(windows, key=lambda w: w[2]):
            if cls == "rodata" and merged and merged[-1][1] == address:
                merged[-1][1] = address + hi - lo
            else:
                merged.append([address, address + hi - lo, cls])
        for lo, hi, cls in merged:
            if cls == "bss":
                notes.append(f"own {name} placed at 0x{lo:08X} ({hi - lo} bytes; zero-initialised: "
                             "verified by address only, nothing to compare)")
            elif cls == "data":
                # An all-zero object proves little by content: say so.
                what = "bytes, all zero" if (name, lo) in zero else "bytes"
                notes.append(f"own {name} verified at 0x{lo:08X} ({hi - lo} {what}; "
                             ".data is laid out per translation unit)")
            else:
                notes.append(f"own {name} verified at 0x{lo:08X}..0x{hi:08X}")
    return notes


# --- the tracked artefact ----------------------------------------------------

def opaque_runs(asm_dir, image):
    """[(address, bytes)] for every `.incbin` run of the region files.

    The region files list the image in order: every function as `.word`s and
    every other run as an `.incbin` of the linked image. Walking them checks
    the supplied image against all tracked words on the way, so the runs can
    only come from the image the targets describe."""
    asm_dir = Path(asm_dir)
    base = None
    runs = []
    for path in sorted(asm_dir.glob("*.s")):
        address = None
        for line in path.read_text(encoding="utf-8").splitlines():
            line = line.strip()
            m = re.match(r"/\* region \S+: 0x([0-9A-Fa-f]+)-0x([0-9A-Fa-f]+) \*/", line)
            if m:
                address, region_end = int(m.group(1), 16), int(m.group(2), 16)
                base = address if base is None else min(base, address)
                continue
            m = re.match(r"\.word\s+(0x[0-9A-Fa-f]+)", line)
            if m:
                if address is None:
                    raise SystemExit(f"{path}: data before the region header")
                if image[address - base:address - base + 4] != struct.pack(">I", int(m.group(1), 16)):
                    raise SystemExit(f"{path}: the image differs from the tracked word at "
                                     f"0x{address:08X}; wrong image?")
                address += 4
                continue
            m = re.match(r'\.incbin\s+"[^"]+",\s*(\d+),\s*(\d+)', line)
            if m:
                offset, size = int(m.group(1)), int(m.group(2))
                if address is None or offset != address - base or offset + size > len(image):
                    raise SystemExit(f"{path}: .incbin offset {offset} does not continue the region")
                runs.append((address, image[offset:offset + size]))
                address += size
        if address is not None and address != region_end:
            raise SystemExit(f"{path}: region ends at 0x{address:08X}, header says 0x{region_end:08X}")
    return runs


def render_artifact(runs):
    lines = ["# Retail bytes of the game image's opaque (non-function) runs.",
             "# Generated by tools/cloud/owndata.py generate; do not edit.",
             "# <image address> <bytes, hex>"]
    for address, data in runs:
        for i in range(0, len(data), ARTIFACT_LINE):
            lines.append(f"{address + i:08X} {data[i:i + ARTIFACT_LINE].hex()}")
    return ("\n".join(lines) + "\n").encode("ascii")


def artifact_dir(asm_dir):
    asm_dir = Path(asm_dir)
    return asm_dir.parent / (asm_dir.name + "_data")


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("command", choices=("generate", "check"))
    parser.add_argument("--image", default=str(REPO / "build" / "game_code.bin"))
    parser.add_argument("--asm", default=str(REPO / "asm" / "us" / "blob"))
    parser.add_argument("--out", default=None, help="default: <asm>_data")
    args = parser.parse_args()
    image = Path(args.image).read_bytes()
    try:
        expected = json.loads((Path(args.asm) / "symbols.json").read_text())["image_sha256"]
    except (OSError, ValueError, KeyError):
        expected = None
    if expected and hashlib.sha256(image).hexdigest() != expected:
        raise SystemExit(f"{args.image}: SHA-256 differs from symbols.json image_sha256")
    rendered = render_artifact(opaque_runs(args.asm, image))
    out = Path(args.out) if args.out else artifact_dir(args.asm)
    sums = f"{hashlib.sha256(rendered).hexdigest()}  {ARTIFACT_NAME}\n"
    if args.command == "check":
        same = ((out / ARTIFACT_NAME).is_file()
                and (out / ARTIFACT_NAME).read_bytes() == rendered
                and (out / "SHA256SUMS").read_text() == sums)
        print(f"{out}: " + ("up to date" if same else "MISSING OR STALE"))
        return 0 if same else 1
    out.mkdir(parents=True, exist_ok=True)
    (out / ARTIFACT_NAME).write_bytes(rendered)
    (out / "SHA256SUMS").write_text(sums)
    print(f"wrote {out / ARTIFACT_NAME} ({len(rendered)} bytes) and SHA256SUMS")
    return 0


if __name__ == "__main__":
    sys.exit(main())
