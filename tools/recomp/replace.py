#!/usr/bin/env python3
"""Function replacement for the recomp host (Phase 2).

A replacement swaps one recompiled function for a body built from C we
choose (a matched `src/blob/*.c`, an `src/rom`/`src/ovl_*` body or a
`cloud/work/**/best.c` draft). `tools/recomp/replacements.json` lists them;
`gen_config.py --replace-set SET` (or `--replace ID,...`) builds them into the
recompiler run. Two modes:

  ido     (default) the C is compiled for the N64 with the IDO the project
          matches with (5.3 for game/overlay code, 7.1 for the boot segment;
          flags from the file's `/* flags: ... */` header), linked by ld.lld
          at REPL_VRAM (0x80800000, above the game's 8 MiB, inside the
          runtime's 512 MiB rdram) against the inventory's addresses, and
          appended to the synthetic ROM as a fifth N64Recomp section `.repl`.
          N64Recomp recompiles it with the rest, so the body sees exact N64
          memory semantics (struct layout, 32-bit pointers, byte order):
          nothing in the source changes. Every function the image defines is
          named `repl__<ID>__<name>`.
  native  the C is compiled for the HOST (clang, x86-64) and called through a
          generated wrapper `native__<ID>(uint8_t* rdram, recomp_context* ctx)`
          that unpacks the o32 arguments (a0-a3, stack slots, f12/f14) from
          the prototype and stores the return value. Pointer arguments are
          translated to host pointers into rdram. LIMITS (why ido is the
          default): rdram is stored word-swapped (N64ModernRuntime MEM_*:
          bytes at addr^3, halfwords at addr^2), host pointers are 8 bytes,
          and game globals/callees are not visible to host code. Only leaf
          bodies that touch memory as aligned 32-bit words through their
          pointer arguments run correctly; the generator refuses pointers to
          1/2/8-byte scalars, and a body that calls a game function or names
          a game global fails to link. The frame oracle is the check.

Either way the original function stays in the recompiled set and a
[[patches.hook]] at its entry delegates and returns:
    { rush_repl_count(rdram, K); repl__ID__NAME(rdram, ctx); return; }
(gen_config.replacement_hooks), so direct calls, LOOKUP_FUNC dispatch and
the section table all reach the replacement; the runtime counts the calls
(oracle log "R" lines). gen_config also writes rush2049_repl.h (names, and
where the image sits in the synthetic ROM / rdram for the runtime's copy).

Stdlib only, Python 3.9+. The compilers run on the recomp host (rocky).
"""
import json
import re
import shlex
import struct
import subprocess
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
MANIFEST = REPO / "tools" / "recomp" / "replacements.json"
REPL_VRAM = 0x80800000
POPULATIONS = ("static", "game", "ovl_a", "ovl_b")
DEFAULT_FLAGS = {"static": "-g0 -O2 -mips2 -G 0 -non_shared",
                 "game": "-g0 -O2 -mips2 -G 0 -non_shared",
                 "ovl_a": "-g0 -O2 -mips2 -G 0 -non_shared",
                 "ovl_b": "-g0 -O2 -mips2 -G 0 -non_shared"}
DEFAULT_COMPILER = {"static": "ido7.1", "game": "ido5.3", "ovl_a": "ido5.3", "ovl_b": "ido5.3"}
_ADDR_NAME = re.compile(r"^(?:D|func|jtbl|sym|B|L|lbl|data|bss|rodata)_([0-9A-Fa-f]{8})$")
_IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
_FLAGS = re.compile(r"/\*\s*flags:\s*([^*(]*?)\s*(?:\(|\*/)")


class ReplaceError(RuntimeError):
    """The replacement cannot be built without guessing."""


# ------------------------------------------------------------------ manifest

def load_manifest(path=MANIFEST):
    doc = json.loads(Path(path).read_text())
    entries = doc.get("entries", {})
    for rid, e in entries.items():
        if not _IDENT.match(rid):
            raise ReplaceError(f"replacement id {rid!r} is not a C identifier")
        mode = e.get("mode", "ido")
        need = ("function", "population") + (("source",) if mode != "capture" else ())
        for key in need:
            if key not in e:
                raise ReplaceError(f"replacement {rid}: missing {key}")
        if e["population"] not in POPULATIONS:
            raise ReplaceError(f"replacement {rid}: unknown population {e['population']}")
        if mode not in ("ido", "native", "capture"):
            raise ReplaceError(f"replacement {rid}: mode must be ido, native or capture")
        if mode == "capture" and not (e.get("source") or e.get("prototype")):
            raise ReplaceError(f"replacement {rid}: a capture probe needs a prototype or a source")
    for name, ids in doc.get("sets", {}).items():
        for rid in ids:
            if rid not in entries:
                raise ReplaceError(f"set {name}: unknown replacement {rid}")
    return doc


def select(doc, set_name=None, ids=None):
    """[(id, entry)] for a set and/or explicit ids; one entry per function."""
    chosen = []
    if set_name:
        if set_name not in doc.get("sets", {}):
            raise ReplaceError(f"unknown replacement set {set_name}")
        chosen += doc["sets"][set_name]
    for rid in ids or ():
        if rid not in doc["entries"]:
            raise ReplaceError(f"unknown replacement {rid}")
        chosen.append(rid)
    out, seen = [], {}
    for rid in chosen:
        e = doc["entries"][rid]
        key = (e["population"], e["function"])
        if key in seen and seen[key] != rid:
            raise ReplaceError(f"{e['function']} replaced twice ({seen[key]}, {rid})")
        if key not in seen:
            seen[key] = rid
            out.append((rid, dict(e, mode=e.get("mode", "ido"))))
    return out


def source_flags(text, population):
    """IDO flags from the file's `/* flags: ... */` header (first one)."""
    m = _FLAGS.search(text)
    return m.group(1).strip() if m else DEFAULT_FLAGS[population]


# ------------------------------------------------------------------ symbols

def address_table(functions, symbols, population):
    """{name: vram} visible to a body of `population`: static and game
    functions and data symbols, plus that overlay's own (overlay names are
    unique only per image). Inventory names, before identity renames:
    sources call functions by the inventory's names, the jal targets bind by
    address."""
    table = {}
    pops = ["static", "game"] + ([population] if population.startswith("ovl_") else [])
    for pop in pops:
        for name, vram in symbols.get(pop, {}).items():
            table.setdefault(name, vram)
        for f in functions.get(pop, []):
            table[f["name"]] = f["vram"]
    return table


def resolve(names, table):
    """({name: vram}, [unresolved]) for undefined symbols of the objects."""
    out, missing = {}, []
    for name in sorted(set(names)):
        if name in table:
            out[name] = table[name]
            continue
        m = _ADDR_NAME.match(name)
        if m:
            out[name] = int(m.group(1), 16)
        else:
            missing.append(name)
    return out, missing


def render_linker_script(defs, vram=REPL_VRAM):
    lines = ["/* Generated by tools/recomp/replace.py: replacement image for the recomp host. */",
             "SECTIONS {",
             "  . = 0x%08X;" % vram,
             "  .repl : { *(.text*) *(.rodata*) *(.rdata*) *(.data*) *(.sdata*) *(.sbss*) *(.scommon*)"
             " *(.bss*) *(COMMON) }",
             "  /DISCARD/ : { *(.mdebug*) *(.options) *(.reginfo) *(.pdr) *(.comment)"
             " *(.MIPS.abiflags) *(.gnu.attributes) *(.note*) }",
             "}"]
    for name, addr in sorted(defs.items()):
        lines.append("%s = 0x%08X;" % (name, addr))
    return "\n".join(lines) + "\n"


# ------------------------------------------------------------------ ELF32 (big-endian MIPS)

def read_elf(data):
    """Sections and symbols of a big-endian ELF32 file (relocatable or linked).
    Returns {"sections": [{name, type, addr, offset, size}], "symbols":
    [{name, value, size, type, bind, shndx, section}]}."""
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 2:
        raise ReplaceError("not a big-endian ELF32 file")
    (e_shoff,) = struct.unpack_from(">I", data, 0x20)
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from(">HHH", data, 0x2E)
    raw = []
    for i in range(e_shnum):
        name, typ, _flags, addr, off, size, link, info, _align, entsize = struct.unpack_from(
            ">IIIIIIIIII", data, e_shoff + i * e_shentsize)
        raw.append({"name_off": name, "type": typ, "addr": addr, "offset": off, "size": size,
                    "link": link, "info": info, "entsize": entsize})

    def cstr(off):
        end = data.index(b"\0", off)
        return data[off:end].decode("latin-1")

    shstr = raw[e_shstrndx]["offset"] if e_shnum else 0
    sections = []
    for s in raw:
        sections.append({"name": cstr(shstr + s["name_off"]), "type": s["type"], "addr": s["addr"],
                         "offset": s["offset"], "size": s["size"]})
    symbols = []
    symtab_names = {}
    symtab_index = {}
    for i, s in enumerate(raw):
        if s["type"] != 2:  # SHT_SYMTAB
            continue
        strtab = raw[s["link"]]["offset"]
        for j in range(s["size"] // 16):
            name, value, size, info, _other, shndx = struct.unpack_from(">IIIBBH", data, s["offset"] + j * 16)
            symbols.append({"name": cstr(strtab + name), "value": value, "size": size,
                            "type": info & 0xF, "bind": info >> 4, "shndx": shndx,
                            "section": sections[shndx]["name"] if 0 < shndx < len(sections) else None})
            symtab_names[(i, j)] = symbols[-1]["name"]
            symtab_index[(i, j)] = symbols[-1]
    # Symbols some relocation uses (SHT_REL = 9, SHT_RELA = 4): an undefined
    # symbol no relocation uses (IDO keeps `fabsf` declared under
    # #pragma intrinsic but emits abs.s) needs no address.
    referenced = set()
    relocs = []
    for s in raw:
        if s["type"] in (9, 4):
            entsize = 8 if s["type"] == 9 else 12
            target = sections[s["info"]]["name"] if 0 < s["info"] < len(sections) else None
            for k in range(s["size"] // entsize):
                off, info = struct.unpack_from(">II", data, s["offset"] + k * entsize)
                name = symtab_names.get((s["link"], info >> 8))
                if name:
                    referenced.add(name)
                relocs.append({"section": target, "offset": off, "type": info & 0xFF,
                               "sym": symtab_index.get((s["link"], info >> 8)), "rela": s["type"] == 4})
    return {"sections": sections, "symbols": symbols, "referenced": referenced, "relocs": relocs}


def undefined_symbols(elf):
    """Undefined symbols that a relocation uses (others need no address)."""
    ref = elf.get("referenced")
    return sorted({s["name"] for s in elf["symbols"] if s["shndx"] == 0 and s["name"]
                   and (ref is None or s["name"] in ref)})


def defined_functions(elf, section=".repl"):
    """[{name, vram, size, local}] FUNC symbols in `section`, by address."""
    out = {}
    for s in elf["symbols"]:
        if s["type"] == 2 and s["section"] == section and s["size"] > 0:
            out.setdefault(s["value"], {"name": s["name"], "vram": s["value"], "size": s["size"],
                                        "local": s["bind"] == 0})
    return [out[k] for k in sorted(out)]


def section_bytes(data, elf, name=".repl"):
    for s in elf["sections"]:
        if s["name"] == name:
            if s["type"] == 8:  # NOBITS
                return s["addr"], b"\0" * s["size"]
            return s["addr"], bytes(data[s["offset"]:s["offset"] + s["size"]])
    raise ReplaceError(f"linked image has no {name} section")


def retarget_self_calls(raw, base, own, orig, ranges=None):
    """(bytes, count): every `jal own` in the code of the image chunk `raw`
    (linked at `base`; `ranges` = [(vram, size)] of its functions, default
    the whole chunk) rewritten to `jal orig`."""
    buf = bytearray(raw)
    n = 0
    offs = range(0, len(buf) - 3, 4) if ranges is None else \
        [o for v, size in ranges for o in range(v - base, v - base + size, 4) if 0 <= o <= len(buf) - 4]
    for off in offs:
        (w,) = struct.unpack_from(">I", buf, off)
        if w >> 26 == 3:
            target = ((base + off + 4) & 0xF0000000) | ((w & 0x03FFFFFF) << 2)
            if target == own:
                struct.pack_into(">I", buf, off, (3 << 26) | ((orig >> 2) & 0x03FFFFFF))
                n += 1
    return bytes(buf), n


# ------------------------------------------------------------------ static binding (Phase 3)

R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6
_WRITABLE = (".data", ".bss", ".sdata", ".sbss")


def _sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def _word(buf, off):
    return struct.unpack_from(">I", buf, off)[0]


def hilo_pairs(elf, text):
    """[(sym, [hi sites], lo site, addend)] for the .text HI16/LO16
    relocations of an IDO object (REL: the addend is in the instructions;
    one HI16 may serve several LO16s, which follow it)."""
    out, pending = [], []
    for r in elf["relocs"]:
        if r["section"] != ".text" or r["sym"] is None:
            continue
        if r["type"] == R_MIPS_HI16:
            pending.append(r)
        elif r["type"] == R_MIPS_LO16:
            his = [h for h in pending if h["sym"] is r["sym"]]
            lo = _word(text, r["offset"]) & 0xFFFF
            if his:
                hi = _word(text, his[-1]["offset"]) & 0xFFFF
                out.append((r["sym"], [h["offset"] for h in his], r["offset"], (hi << 16) + _sext16(lo)))
                pending = [h for h in pending if h["sym"] is not r["sym"]] + his[-1:]
            elif out and out[-1][0] is r["sym"]:      # another LO16 of the last pair's HI16
                prev = out[-1]
                hi = _word(text, prev[1][-1]) & 0xFFFF
                out.append((r["sym"], prev[1], r["offset"], (hi << 16) + _sext16(lo)))
    return out


def _static_key(sym, addend):
    """(section, offset) of a local writable data reference, else None."""
    if sym["bind"] != 0 or not (sym["section"] or "").startswith(_WRITABLE):
        return None
    base = 0 if sym["type"] == 3 else sym["value"]       # STT_SECTION: offset is the addend
    return (sym["section"], base + addend)


def _local_name_at(elf, section, offset):
    """(name, start) of the local object covering section+offset, if any."""
    for s in elf["symbols"]:
        if s["section"] == section and s["bind"] == 0 and s["type"] in (0, 1) and s["name"] \
                and s["value"] <= offset < s["value"] + max(s["size"], 1):
            return s["name"], s["value"]
    return None


def static_bindings(elf, function, func_obj, func_retail, image_word, named=None):
    """{(section, offset): (address, how)} for a replaced body's function-local
    statics (local symbols in .data/.bss/.sdata/.sbss), and [(key, why)] it
    could not bind. Sources of the address, in order:
      manifest  entry "statics": {name: address} (drafts)
      name      the static is named by address (D_XXXXXXXX)
      retail    the replaced function's own HI16/LO16 pair, decoded from the
                RETAIL image at the same offset from the function start; used
                only where both instruction words match the object's
                outside their immediates (matched code, or a matching
                stretch of a draft)
    `image_word(vram)` reads the retail image (None outside it)."""
    text_sec = next((s for s in elf["sections"] if s["name"] == ".text"), None)
    if text_sec is None:
        return {}, []
    named = named or {}
    out, missing = {}, []
    pairs = hilo_pairs(elf, elf["_data"][text_sec["offset"]:text_sec["offset"] + text_sec["size"]])
    text = elf["_data"][text_sec["offset"]:text_sec["offset"] + text_sec["size"]]
    lo_f, hi_f = func_obj["value"], func_obj["value"] + func_obj["size"]
    for sym, his, lo, addend in pairs:
        key = _static_key(sym, addend)
        if key is None:
            continue
        if key in out:
            continue
        where = _local_name_at(elf, key[0], key[1]) if sym["type"] == 3 else (sym["name"], sym["value"])
        if where and where[0] in named:
            out[key] = (int(named[where[0]], 0) + key[1] - where[1], "manifest")
            continue
        if where:
            m = _ADDR_NAME.match(where[0])
            if m:
                out[key] = (int(m.group(1), 16) + key[1] - where[1], "name")
                continue
        if func_retail is not None and all(lo_f <= o < hi_f for o in his + [lo]):
            words = []
            for o in his + [lo]:
                r = image_word(func_retail + o - lo_f)
                words.append(r)
            if None not in words and all((w & 0xFFFF0000) == (_word(text, o) & 0xFFFF0000)
                                         for w, o in zip(words, his + [lo])):
                target = ((words[-2] & 0xFFFF) << 16) + _sext16(words[-1] & 0xFFFF)
                out[key] = (target & 0xFFFFFFFF, "retail")
                continue
        missing.append((key, where[0] if where else None))
    # keys every pair needs, for keys bound through another pair of the same static
    return out, [(k, n) for k, n in missing if k not in out]


def apply_static_bindings(raw, chunk_vram, text_base, elf, bindings):
    """Patch every HI16/LO16 pair of the object's .text whose static is bound,
    in the linked chunk `raw` (at chunk_vram; the object's .text at text_base).
    Returns (bytes, sites patched)."""
    text_sec = next(s for s in elf["sections"] if s["name"] == ".text")
    text = elf["_data"][text_sec["offset"]:text_sec["offset"] + text_sec["size"]]
    buf = bytearray(raw)
    n = 0
    for sym, his, lo, addend in hilo_pairs(elf, text):
        key = _static_key(sym, addend)
        if key is None or key not in bindings:
            continue
        target = bindings[key][0]
        for o in his:
            at = text_base + o - chunk_vram
            struct.pack_into(">I", buf, at, (_word(buf, at) & 0xFFFF0000) | (((target + 0x8000) >> 16) & 0xFFFF))
        at = text_base + lo - chunk_vram
        struct.pack_into(">I", buf, at, (_word(buf, at) & 0xFFFF0000) | (target & 0xFFFF))
        n += 1
    return bytes(buf), n


def repl_name(rid, name):
    return "repl__%s__%s" % (rid, name)


def name_functions(rid, entry, funcs):
    """Recompiler names for the functions one replacement's object defines:
    the replaced function and anything else it kept (static helpers)."""
    out = []
    for f in funcs:
        out.append(dict(f, recomp_name=repl_name(rid, f["name"]), id=rid,
                        replaces=f["name"] == entry["function"]))
    if not any(f["replaces"] for f in out):
        raise ReplaceError(f"replacement {rid}: the object does not define {entry['function']}")
    return out


# ------------------------------------------------------------------ native wrappers (o32)

_SCALARS = {
    "void": ("void", 0), "s8": ("int8_t", 1), "u8": ("uint8_t", 1), "char": ("int8_t", 1),
    "signed char": ("int8_t", 1), "unsigned char": ("uint8_t", 1),
    "s16": ("int16_t", 2), "u16": ("uint16_t", 2), "short": ("int16_t", 2),
    "unsigned short": ("uint16_t", 2), "s32": ("int32_t", 4), "u32": ("uint32_t", 4),
    "int": ("int32_t", 4), "unsigned int": ("uint32_t", 4), "unsigned": ("uint32_t", 4),
    "long": ("int32_t", 4), "unsigned long": ("uint32_t", 4), "BOOL": ("int32_t", 4),
    "S32": ("int32_t", 4), "U32": ("uint32_t", 4), "S16": ("int16_t", 2), "U16": ("uint16_t", 2),
    "S08": ("int8_t", 1), "U08": ("uint8_t", 1),
    "f32": ("float", 4), "float": ("float", 4), "F32": ("float", 4),
    "f64": ("double", 8), "double": ("double", 8),
    "s64": ("int64_t", 8), "u64": ("uint64_t", 8), "long long": ("int64_t", 8),
    "unsigned long long": ("uint64_t", 8),
}
_WORD_POINTEES = {"s32", "u32", "int", "unsigned int", "unsigned", "f32", "float", "F32", "S32",
                  "U32", "BOOL", "long", "unsigned long", "void"}


def parse_type(text):
    """{c: host C type, size, float, ptr, pointee} for a parameter/return type."""
    t = " ".join(text.replace("const ", "").replace("volatile ", "").split())
    if t.endswith("*"):
        base = t.rstrip("*").strip()
        return {"c": "void*", "size": 4, "float": False, "ptr": True, "pointee": base,
                "depth": len(t) - len(t.rstrip("*"))}
    if t not in _SCALARS:
        raise ReplaceError(f"native: unsupported type {text!r} (scalars and pointers only)")
    c, size = _SCALARS[t]
    return {"c": c, "size": size, "float": c in ("float", "double"), "ptr": False, "pointee": None}


def parse_prototype(proto):
    """(return type, name, [(type, name)]) from `RET NAME(T a, U *b)`."""
    m = re.match(r"^\s*(.+?)\s*\b([A-Za-z_][A-Za-z0-9_]*)\s*\((.*)\)\s*;?\s*$", proto, re.S)
    if not m:
        raise ReplaceError(f"native: cannot parse prototype {proto!r}")
    ret, name, args = m.group(1), m.group(2), m.group(3).strip()
    params = []
    if args and args != "void":
        for i, a in enumerate(args.split(",")):
            a = a.strip()
            if a == "..." or "(" in a:
                raise ReplaceError(f"native: unsupported parameter {a!r} in {name}")
            if "[" in a:   # array parameter = pointer to its element type
                am = re.match(r"^(.*?)\b([A-Za-z_][A-Za-z0-9_]*)\s*(\[.*\])$", a)
                if not am:
                    raise ReplaceError(f"native: cannot parse array parameter {a!r} in {name}")
                params.append((am.group(1).strip() + " *", am.group(2)))
                continue
            pm = re.match(r"^(.*?[\s*])([A-Za-z_][A-Za-z0-9_]*)$", a)
            ptype, pname = (pm.group(1), pm.group(2)) if pm and pm.group(1).strip() else (a, "arg%d" % i)
            params.append((ptype.strip(), pname))
    return ret, name, params


def o32_layout(params):
    """Where each argument lives under the o32 ABI (as N64Recomp's ctx holds it):
    [{"kind": "gpr", "reg": 4..7} | {"kind": "gpr64", "hi": r, "lo": r+1} |
     {"kind": "fpr", "reg": 12|14} | {"kind": "stack", "offset": 16+} ...].
    Rules: arguments occupy 4-byte slots (8-byte types take an aligned pair);
    slots 0-3 are a0-a3, the rest the caller's stack at sp+offset; the first
    two arguments go in f12/f14 instead when they are floating point and every
    argument before them is too."""
    out, slot, fp_ok = [], 0, True
    for i, (ptype, _name) in enumerate(params):
        t = parse_type(ptype)
        size = 8 if t["size"] == 8 else 4
        if size == 8:
            slot = (slot + 7) & ~7
        if t["float"] and fp_ok and i < 2:
            out.append({"kind": "fpr", "reg": 12 + 2 * i, "type": t})
        else:
            fp_ok = False
            if slot < 16:
                if size == 8:
                    out.append({"kind": "gpr64", "hi": 4 + slot // 4, "lo": 5 + slot // 4, "type": t})
                else:
                    out.append({"kind": "gpr", "reg": 4 + slot // 4, "type": t})
            else:
                out.append({"kind": "stack", "offset": slot, "type": t})
        if not t["float"]:
            fp_ok = False
        slot += size
    return out


def check_native_fit(ret, params, allow_struct_pointers=False):
    """Refuse prototypes the word-swapped rdram view cannot serve."""
    for ptype, pname in [(ret, "return")] + list(params):
        t = parse_type(ptype)
        if t["ptr"]:
            if t["depth"] > 1:
                raise ReplaceError(f"native: {pname} is a pointer to pointer (8-byte host pointers)")
            if t["pointee"] not in _WORD_POINTEES and not allow_struct_pointers:
                raise ReplaceError(f"native: {pname} points to {t['pointee']!r}; only 32-bit scalars "
                                   "are byte-order safe in the word-swapped rdram (use mode ido)")


def render_wrapper(rid, ret, name, params, body_name):
    """C source of native__<rid>(rdram, ctx): unpack, call body_name, store return."""
    lay = o32_layout(params)
    rt = parse_type(ret)
    lines = ["/* Generated by tools/recomp/replace.py (native mode): %s */" % name,
             "#include <stdint.h>", "#include <string.h>", '#include "recomp.h"', "",
             "static void* rush_host_ptr(uint8_t* rdram, uint32_t a) {",
             "    return a ? (void*)(rdram + (a & 0x1FFFFFFFu)) : (void*)0;",
             "}", ""]
    proto_params = ", ".join(parse_type(p)["c"] for p, _ in params) or "void"
    lines.append("%s %s(%s);" % (rt["c"], body_name, proto_params))
    lines += ["", 'void native__%s(uint8_t* rdram, recomp_context* ctx) {' % rid]
    args = []
    for i, ((ptype, pname), loc) in enumerate(zip(params, lay)):
        t = loc["type"]
        var = "a%d" % i
        if loc["kind"] == "fpr":
            src = "ctx->f%d.%s" % (loc["reg"], "d" if t["size"] == 8 else "fl")
            lines.append("    %s %s = %s;" % (t["c"], var, src))
        elif loc["kind"] in ("gpr", "stack"):
            raw = ("(uint32_t)ctx->r%d" % loc["reg"] if loc["kind"] == "gpr"
                   else "(uint32_t)MEM_W(%d, ctx->r29)" % loc["offset"])
            lines.append("    uint32_t %s_w = %s;" % (var, raw))
            if t["ptr"]:
                lines.append("    void* %s = rush_host_ptr(rdram, %s_w);" % (var, var))
            elif t["float"]:
                lines.append("    float %s; memcpy(&%s, &%s_w, 4);" % (var, var, var))
            else:
                lines.append("    %s %s = (%s)%s_w;" % (t["c"], var, t["c"], var))
        else:  # gpr64 (or a double in a register pair / two stack words)
            lines.append("    uint64_t %s_w = ((uint64_t)(uint32_t)ctx->r%d << 32) | (uint32_t)ctx->r%d;"
                         % (var, loc["hi"], loc["lo"]))
            if t["float"]:
                lines.append("    double %s; memcpy(&%s, &%s_w, 8);" % (var, var, var))
            else:
                lines.append("    %s %s = (%s)%s_w;" % (t["c"], var, t["c"], var))
        args.append(var)
    call = "%s(%s)" % (body_name, ", ".join(args))
    if rt["c"] == "void":
        lines.append("    %s;" % call)
    elif rt["ptr"]:
        lines += ["    uint8_t* r = (uint8_t*)%s;" % call,
                  "    ctx->r2 = r ? (gpr)(int32_t)(uint32_t)(0x80000000u + (uint32_t)(r - rdram)) : 0;"]
    elif rt["c"] == "float":
        lines.append("    ctx->f0.fl = %s;" % call)
    elif rt["c"] == "double":
        lines.append("    ctx->f0.d = %s;" % call)
    elif rt["size"] == 8:
        lines += ["    uint64_t r = (uint64_t)%s;" % call,
                  "    ctx->r2 = (gpr)(int32_t)(uint32_t)(r >> 32);",
                  "    ctx->r3 = (gpr)(int32_t)(uint32_t)r;"]
    else:
        lines.append("    ctx->r2 = (gpr)(int32_t)(%s)%s;" % (rt["c"], call))
    lines += ["}", ""]
    return "\n".join(lines)


def render_native_body(source_path, function, body_name):
    """The body TU: the unchanged source with the function renamed."""
    return ("/* Generated by tools/recomp/replace.py (native mode). */\n"
            "#define %s %s\n#include \"%s\"\n" % (function, body_name, Path(source_path).resolve()))


def find_prototype(text, function):
    m = re.search(r"(?m)^([A-Za-z_][\w\s\*]*?\b%s\s*\([^)]*\))\s*\{" % re.escape(function), text)
    if not m:
        raise ReplaceError(f"native: no definition of {function} found to take the prototype from")
    return " ".join(m.group(1).split())


# ------------------------------------------------------------------ per-call capture (Phase 3)

# Location codes the runtime reads (runtime/src/capture.cpp, rush2049_repl.h):
#   4..7 a0-a3 | 100+n single fN | 200+n double fN (two words) | 300+n gpr pair rN:rN+1
#   1000+o stack word at sp+o | 2000+o stack double/pair at sp+o (two words)
# Return codes: 0 none | 2 v0 | 3 v0:v1 | 100 f0 single | 200 f0 double.
CAP_GPR, CAP_FPR1, CAP_FPR2, CAP_GPR2, CAP_STK1, CAP_STK2 = 0, 100, 200, 300, 1000, 2000
CAP_MAX_LOCS = 12
_REG_NAMES = {4: "a0", 5: "a1", 6: "a2", 7: "a3"}


def _lenient_type(text):
    """parse_type, but an unknown non-pointer typedef counts as a 32-bit int
    (enum/BOOL-like names in drafts); struct-by-value is not expected."""
    try:
        return parse_type(text)
    except ReplaceError:
        return {"c": "int32_t", "size": 4, "float": False, "ptr": False, "pointee": None}


_GPR_NAMES = {"v0": 2, "v1": 3, "a0": 4, "a1": 5, "a2": 6, "a3": 7, "t0": 8, "t1": 9, "t2": 10, "t3": 11,
              "t4": 12, "t5": 13, "t6": 14, "t7": 15, "s0": 16, "s1": 17, "s2": 18, "s3": 19, "s4": 20,
              "s5": 21, "s6": 22, "s7": 23, "t8": 24, "t9": 25, "gp": 28, "sp": 29, "fp": 30, "s8": 30}


def explicit_capture(regs, ret="v0"):
    """Capture layout from register names (entry "capture_args": a callee with
    a non-o32 convention, e.g. IDO -O3 passing a static's arguments in
    callee-saved registers): a0..a3, s0..s7, t0..t9, v0/v1 | f0..f31 (single)
    | fN:d (double) | sp+N (stack word at entry)."""
    args = []
    for r in regs:
        m = re.match(r"^(?:(\w+?)=)?(.+)$", r)
        label, reg = m.group(1), m.group(2)
        if reg in _GPR_NAMES:
            code = CAP_GPR + _GPR_NAMES[reg]
        elif re.match(r"^f([0-9]|[12][0-9]|3[01])(:d)?$", reg):
            n = int(reg[1:].split(":")[0])
            code = (CAP_FPR2 if reg.endswith(":d") else CAP_FPR1) + n
        elif re.match(r"^sp\+(\d+)$", reg):
            code = CAP_STK1 + int(reg[3:])
        else:
            raise ReplaceError(f"capture_args: unknown register {reg!r}")
        args.append(["%s:%s" % (reg, label) if label else reg, code])
    if len(args) > CAP_MAX_LOCS:
        raise ReplaceError(f"capture_args: at most {CAP_MAX_LOCS} locations")
    rcode = {"void": 0, "-": 0, "v0": 2, "v0:v1": 3, "f0": 100, "f0:d": 200}.get(ret)
    if rcode is None:
        raise ReplaceError(f"capture_ret: unknown {ret!r}")
    return {"args": args, "ret": rcode, "known": True, "prototype": "explicit"}


def capture_layout(text, function, prototype=None, explicit=None, explicit_ret="v0"):
    """What the runtime records at entry/exit of `function` (per-call capture):
    {"args": [[label, code]], "ret": code, "known": bool, "prototype": str}.
    Only the argument registers and stack words the prototype uses are
    recorded (a replaced caller may leave other registers different); the
    return register only for non-void functions. No prototype found: a0-a3
    and v0, known=False. `explicit` (register names) overrides the prototype."""
    if explicit:
        return explicit_capture(explicit, explicit_ret)
    try:
        proto = prototype or find_prototype(text, function)
        ret, name, params = parse_prototype(proto)
        if name != function:
            raise ReplaceError("prototype names %s" % name)
    except ReplaceError:
        return {"args": [[_REG_NAMES[r], CAP_GPR + r] for r in range(4, 8)], "ret": 2, "known": False,
                "prototype": None}
    lay = _layout_lenient(params)
    args = []
    for (_ptype, pname), loc in zip(params, lay):
        t = loc["type"]
        if loc["kind"] == "gpr":
            args.append(["%s:%s" % (_REG_NAMES[loc["reg"]], pname), CAP_GPR + loc["reg"]])
        elif loc["kind"] == "gpr64":
            args.append(["%s:%s:%s" % (_REG_NAMES[loc["hi"]], _REG_NAMES[loc["lo"]], pname),
                         CAP_GPR2 + loc["hi"]])
        elif loc["kind"] == "fpr":
            args.append(["f%d:%s" % (loc["reg"], pname),
                         (CAP_FPR2 if t["size"] == 8 else CAP_FPR1) + loc["reg"]])
        else:
            args.append(["sp+%d:%s" % (loc["offset"], pname),
                         (CAP_STK2 if t["size"] == 8 else CAP_STK1) + loc["offset"]])
    if len(args) > CAP_MAX_LOCS:
        args = args[:CAP_MAX_LOCS]
    rt = _lenient_type(ret)
    if rt["c"] == "void":
        rcode = 0
    elif rt["float"]:
        rcode = 200 if rt["size"] == 8 else 100
    elif rt["size"] == 8 and not rt["ptr"]:
        rcode = 3
    else:
        rcode = 2
    return {"args": args, "ret": rcode, "known": True, "prototype": proto}


def _layout_lenient(params):
    """o32_layout with unknown non-pointer typedefs taken as 32-bit ints."""
    fixed = []
    for ptype, pname in params:
        t = " ".join(ptype.replace("const ", "").replace("volatile ", "").split())
        if not t.endswith("*") and t not in _SCALARS:
            t = "s32"
        fixed.append((t, pname))
    return o32_layout(fixed)


def capture_entries(selected, repo=REPO):
    """Stock-build capture (gen_config --capture-set): the same functions and
    order as a replacement set, but no replacement; each entry hook records
    the call and runs the recompiled original (capture mode)."""
    out = []
    for rid, e in selected:
        src = (repo / e["source"]).resolve() if e.get("source") else None
        text = src.read_text(errors="replace") if src and src.is_file() else ""
        out.append({"id": rid, "function": e["function"], "population": e["population"],
                    "mode": "capture", "call": None, "source": e.get("source"),
                    "probe": e.get("mode") == "capture",
                    "capture": capture_layout(text, e["function"], e.get("prototype"),
                                                    e.get("capture_args"), e.get("capture_ret", "v0"))})
    return out


# ------------------------------------------------------------------ build

def build(selected, functions, symbols, work_dir, toolkits, ld="ld.lld", repo=REPO, run=subprocess.run,
          images=None):
    """Compile and link the selected replacements.

    toolkits = {"ido5.3": DIR, "ido7.1": DIR} (each with ido/cc). Returns
    {"image": {"vram", "bytes"} or None, "functions": [{name, vram, size}],
     "replaced": [{id, function, population, mode, call, source}],
     "native": [{id, wrapper, body}]} with every name final."""
    work = Path(work_dir).resolve()
    work.mkdir(parents=True, exist_ok=True)
    objects, replaced, native = [], [], []
    for rid, e in selected:
        if e["mode"] == "capture":
            # Probe: not replaced; its entry hook only records the call (the
            # original runs), so a callee of a replaced body shows its
            # arguments in both builds.
            replaced += capture_entries([(rid, e)], repo)
            continue
        src = (repo / e["source"]).resolve()
        if not src.is_file():
            raise ReplaceError(f"replacement {rid}: source {e['source']} not found")
        text = src.read_text(errors="replace")
        if e["mode"] == "native":
            proto = e.get("prototype") or find_prototype(text, e["function"])
            ret, name, params = parse_prototype(proto)
            if name != e["function"]:
                raise ReplaceError(f"replacement {rid}: prototype names {name}, not {e['function']}")
            check_native_fit(ret, params, e.get("allow_struct_pointers", False))
            body = "rush_native_body__" + rid
            ndir = work / "native"
            ndir.mkdir(exist_ok=True)
            (ndir / (rid + "_wrap.c")).write_text(render_wrapper(rid, ret, name, params, body))
            (ndir / (rid + "_body.c")).write_text(render_native_body(src, e["function"], body))
            native.append({"id": rid, "wrapper": "native__" + rid})
            replaced.append({"id": rid, "function": e["function"], "population": e["population"],
                             "mode": "native", "call": "native__" + rid, "source": e["source"],
                             "capture": capture_layout(text, e["function"], e.get("prototype"),
                                                    e.get("capture_args"), e.get("capture_ret", "v0"))})
            continue
        compiler = e.get("compiler") or DEFAULT_COMPILER[e["population"]]
        if compiler not in toolkits:
            raise ReplaceError(f"replacement {rid}: no toolkit for {compiler} (--ido53/--ido71)")
        flags = e.get("flags") or source_flags(text, e["population"])
        obj = work / (rid + ".o")
        cmd = [str(Path(toolkits[compiler]) / "ido" / "cc"), "-c"] + shlex.split(flags) + \
              ["-Xcpluscomm", "-I", str(repo / "include"), "-I", str(src.parent), "-o", str(obj), str(src)]
        proc = run(cmd, capture_output=True, text=True, cwd=str(work))
        if proc.returncode != 0 or not obj.is_file():
            raise ReplaceError(f"replacement {rid}: IDO failed:\n{(proc.stderr or proc.stdout)[:2000]}")
        elf = read_elf(obj.read_bytes())
        elf["_data"] = obj.read_bytes()
        table = address_table(functions, symbols, e["population"])
        res, missing = resolve(undefined_symbols(elf), table)
        if missing:
            raise ReplaceError(f"replacement {rid}: unresolved symbols {', '.join(missing)}")
        objects.append((rid, dict(e, _text=text), obj, elf, res))
    # Each object is linked on its own at the next aligned address (no name can
    # collide between replacements; static helpers belong to their object),
    # and the chunks are concatenated into one image at REPL_VRAM.
    image, funcs, chunks, cursor = None, [], [], REPL_VRAM
    for rid, e, obj, elf, res in objects:
        defined = {s["name"] for s in elf["symbols"] if s["shndx"] != 0 and s["name"]}
        defs = {n: a for n, a in res.items() if n not in defined}
        # Data the source DEFINES (matched files define their unit's globals):
        # bind it to the game's copy (a linker-script assignment overrides the
        # object's definition), else the replacement would keep private state.
        private = []
        for sym in elf["symbols"]:
            if sym["name"] and sym["bind"] == 1 and sym["type"] in (0, 1) and sym["shndx"] != 0 \
                    and (sym["section"] or "").startswith((".data", ".bss", ".sdata", ".sbss")) \
                    or (sym["name"] and sym["shndx"] == 0xFFF2):
                table = address_table(functions, symbols, e["population"])
                addr = table.get(sym["name"])
                m = _ADDR_NAME.match(sym["name"])
                if addr is None and m:
                    addr = int(m.group(1), 16)
                if addr is None:
                    private.append(sym["name"])
                else:
                    defs[sym["name"]] = addr
        script = work / (rid + ".ld")
        script.write_text(render_linker_script(defs, cursor))
        out = work / (rid + ".elf")
        cmd = [ld, "-m", "elf32btsmip", "--gc-sections", "-e", e["function"], "-u", e["function"],
               "-T", str(script), "-o", str(out), str(obj)]
        proc = run(cmd, capture_output=True, text=True, cwd=str(work))
        if proc.returncode != 0 or not out.is_file():
            raise ReplaceError(f"replacement {rid}: ld.lld failed:\n{(proc.stderr or proc.stdout)[:2000]}")
        data = out.read_bytes()
        lelf = read_elf(data)
        vram, raw = section_bytes(data, lelf)
        if vram < cursor:
            raise ReplaceError(f"replacement {rid}: linked at 0x{vram:08X}, below 0x{cursor:08X}")
        if vram > cursor:   # the object's section alignment moved it up: pad
            chunks.append(b"\0" * (vram - cursor))
            cursor = vram
        named = name_functions(rid, e, defined_functions(lelf))
        # Static binding: the body's function-local statics live at their
        # retail addresses (else a private copy in the image: an escaping
        # address, func_800E7710's SI message, or state shared with an
        # unreplaced function of the unit would differ).
        func_obj = next((x for x in elf["symbols"] if x["name"] == e["function"] and x["type"] == 2
                         and x["section"] == ".text"), None)
        func_lnk = next((f["vram"] for f in named if f["replaces"]), None)
        bound, unbound, patched = {}, [], 0
        if func_obj is not None and func_lnk is not None:
            img = (images or {}).get(e["population"])

            def image_word(addr, img=img):
                if img is None or not img[0] <= addr <= img[0] + len(img[1]) - 4:
                    return None
                return struct.unpack_from(">I", img[1], addr - img[0])[0]
            bound, unbound = static_bindings(elf, e["function"], func_obj,
                                             address_table(functions, symbols, e["population"]).get(e["function"]),
                                             image_word, e.get("statics"))
            raw, patched = apply_static_bindings(raw, vram, func_lnk - func_obj["value"], elf, bound)
        # A recursive body calls itself inside the object (jal to its own
        # linked address), which would bypass the entry hook: retarget those
        # jals to the original entry so every call is counted and captured
        # as in the stock build (model_transform_setup).
        orig = address_table(functions, symbols, e["population"]).get(e["function"])
        own = [f["vram"] for f in named if f["replaces"]]
        if orig is not None and own:
            raw, self_calls = retarget_self_calls(raw, vram, own[0], orig,
                                                  [(f["vram"], f["size"]) for f in named])
        else:
            self_calls = 0
        funcs += [{"name": f["recomp_name"], "vram": f["vram"], "size": f["size"]} for f in named]
        replaced.append({"id": rid, "function": e["function"], "population": e["population"],
                         "mode": "ido", "call": repl_name(rid, e["function"]), "source": e["source"],
                         "private_data": sorted(private), "self_calls_retargeted": self_calls,
                         "statics_bound": sorted("%s+0x%X=0x%08X(%s)" % (k[0], k[1], a, how)
                                                 for k, (a, how) in bound.items()),
                         "statics_private": sorted("%s+0x%X%s" % (k[0], k[1], " " + n if n else "")
                                                   for k, n in unbound),
                         "static_sites_patched": patched,
                         "capture": capture_layout(e["_text"], e["function"], e.get("prototype"),
                                                    e.get("capture_args"), e.get("capture_ret", "v0"))})
        raw = raw + b"\0" * (-len(raw) % 16)
        chunks.append(raw)
        cursor += len(raw)
    if chunks:
        image = {"vram": REPL_VRAM, "bytes": b"".join(chunks)}
    order = {rid: i for i, (rid, _e) in enumerate(selected)}
    replaced.sort(key=lambda r: order[r["id"]])
    return {"image": image, "functions": sorted(funcs, key=lambda f: f["vram"]), "replaced": replaced,
            "native": native}
