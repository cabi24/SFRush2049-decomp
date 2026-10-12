"""Object-offset gate for C-defined ROM data (w25a).

The full-ROM gate is blind to where a ROM TU's object places its own data
symbols: the link adds `build/us/syms.ld` (undefined_syms_auto +
undefined_funcs_auto), whose assignments override every same-named
definition, and the owned-slot ASSERTs pin only a slot's start and size. A TU
whose C-defined `.bss` comes out in a different order than retail (w24s:
lib_1050_sdk, where IDO 5.3 sorts `.bss` by cfe block number mod 256 and a
pre-declared extern keeps its early block number) still builds "ROM
matches!", while every reference the compiler resolves inside the object
would point at the wrong storage.

This check reads the link map and each ROM TU object: for every named symbol
an object defines in a `.bss`/`.data`/`.rodata` input section, its would-be
address (the input section's placement + st_value) must equal the address
the retail symbol tables give it (syms.ld inputs and symbol_addrs.us.txt).
A COMMON symbol in a ROM TU object is refused outright (the linker would
allocate it outside the TU's slot). Symbols without a retail address are
counted, not judged.

    python3 -m tools.conveyor.pipeline.owned_offsets check [--map PATH] [--tu src/rom/X.c ...]

prints one `OWNED-OFFSETS:` line per mismatch and a summary, and exits 1 on
any mismatch. The builder gates run it after `make test` (promote,
promote_batch, owned_storage) and surface the lines with report().
"""
import argparse
import re
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
MARK = "OWNED-OFFSETS:"
MAP = "build/us/rush2049.us.map"
RETAIL_TABLES = ("undefined_syms_auto.us.txt", "undefined_funcs_auto.us.txt", "symbol_addrs.us.txt")
SECTIONS = (".bss", ".data", ".rodata")
OBJ_RE = re.compile(r"^build/us/(src/rom/[\w./-]+)\.o$")
_ASSIGN = re.compile(r"^\s*([A-Za-z_.$][\w.$]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;")
SHN_COMMON = 0xFFF2
_DNAME = re.compile(r"^D_[0-9A-Fa-f]{8}$")
STT_SECTION, STT_FILE = 3, 4


class OffsetError(ValueError):
    pass


def retail_addresses(repo=REPO):
    """{name: {address: [table, ...]}} from the retail symbol tables."""
    out = {}
    for table in RETAIL_TABLES:
        path = Path(repo) / table
        if not path.is_file():
            continue
        for line in path.read_text().splitlines():
            m = _ASSIGN.match(line)
            if m:
                out.setdefault(m.group(1), {}).setdefault(int(m.group(2), 16), []).append(table)
    return out


def map_placements(text):
    """[(object_rel, section, address, size, owned)] for every ROM TU
    `.bss`/`.data`/`.rodata` input section the map places. `owned` is the
    owner name when the section sits between a `rom_owned_<owner>_START`
    marker and its END (the owned-slot linker blocks), else None."""
    out, owned, pending = [], None, None
    for line in text.splitlines():
        m = re.search(r"\brom_owned_(\w+)_START = ABSOLUTE", line)
        if m:
            owned = m.group(1)
            continue
        if re.search(r"\brom_owned_\w+_END = ABSOLUTE", line):
            owned = None
            continue
        if pending is not None:
            parts = line.split()
            if len(parts) == 3 and parts[0].startswith("0x"):
                out.append(_placement(pending, parts, owned))
            pending = None
            continue
        parts = line.split()
        if line.startswith(" ") and parts and parts[0] in SECTIONS:
            if len(parts) == 1:
                pending = parts[0]
            elif len(parts) == 4 and parts[1].startswith("0x"):
                out.append(_placement(parts[0], parts[1:], owned))
    return [p for p in out if p is not None]


def _placement(section, parts, owned):
    addr, size, obj = int(parts[0], 16), int(parts[1], 16), parts[2]
    if not OBJ_RE.match(obj) or size == 0:
        return None
    return (obj, section, addr, size, owned)


def object_symbols(data):
    """{section_name: [(name, value, size, binding, type)]} plus a list of
    COMMON symbol names, from a big-endian ELF32 relocatable."""
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 2:
        raise OffsetError("not a big-endian ELF32 object")
    shoff, = struct.unpack_from(">I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from(">HHH", data, 0x2E)
    secs = [struct.unpack_from(">IIIIIIIIII", data, shoff + i * shentsize) for i in range(shnum)]

    def strz(table_off, off):
        end = data.index(b"\0", table_off + off)
        return data[table_off + off:end].decode("latin-1")

    names = [strz(secs[shstrndx][4], s[0]) for s in secs]
    out, common = {}, []
    for s in secs:
        if s[1] != 2:  # SHT_SYMTAB
            continue
        stroff = secs[s[6]][4]
        for i in range(s[5] // 16):
            st_name, st_value, st_size, st_info, _other, st_shndx = struct.unpack_from(
                ">IIIBBH", data, s[4] + i * 16)
            name = strz(stroff, st_name) if st_name else ""
            kind, bind = st_info & 0xF, st_info >> 4
            if not name or kind in (STT_SECTION, STT_FILE):
                continue
            if st_shndx == SHN_COMMON:
                common.append(name)
            elif 0 < st_shndx < len(names) and names[st_shndx] in SECTIONS:
                out.setdefault(names[st_shndx], []).append((name, st_value, st_size, bind, kind))
    return out, common


def check(repo=REPO, map_path=None, tus=None):
    """(results, problems): results = per placed section dicts; problems =
    printable mismatch strings (empty = pass)."""
    repo = Path(repo)
    map_path = Path(map_path) if map_path else repo / MAP
    if not map_path.is_file():
        raise OffsetError(f"no link map at {map_path}")
    retail = retail_addresses(repo)
    wanted = {str(Path(t)) for t in tus} if tus else None
    placements = map_placements(map_path.read_text())
    results, problems, seen_common = [], [], set()
    for obj, section, addr, size, owned in placements:
        tu = OBJ_RE.match(obj).group(1) + ".c"
        if wanted is not None and tu not in wanted:
            continue
        path = repo / obj
        if not path.is_file():
            problems.append(f"{tu}({section}): object {obj} missing")
            continue
        syms, common = object_symbols(path.read_bytes())
        if common and obj not in seen_common:
            seen_common.add(obj)
            problems.append(f"{tu}: COMMON symbol(s) {', '.join(sorted(common))} would be allocated "
                            "outside the TU's sections (define them, or use extern)")
        row = dict(tu=tu, section=section, address=addr, size=size, owned=owned,
                   checked=0, unlisted=0)
        for name, value, _sz, _bind, _kind in syms.get(section, []):
            if name.startswith(("_asmpp_", ".", "$")):
                continue
            built = addr + value
            if value >= size and not (value == size and _sz == 0):
                problems.append(f"{tu}({section}) {name}: offset {value:#x} outside the "
                                f"placed {size:#x}-byte section")
                continue
            want = retail.get(name)
            if not want and _DNAME.match(name):
                # A splat-style placeholder name is its own retail address.
                want = {int(name[2:], 16): ["name"]}
            if not want:
                row["unlisted"] += 1
                continue
            row["checked"] += 1
            bad = [a for a in want if a != built]
            if bad:
                tables = "; ".join(f"{a:#010x} ({', '.join(want[a])})" for a in sorted(bad))
                problems.append(f"{tu}({section}) {name}: built {built:#010x} (+{value:#x}), "
                                f"retail {tables}")
        results.append(row)
    return results, problems


def report(output, log=print):
    """Surface the builder's OWNED-OFFSETS line(s) on the caller's log."""
    lines = [ln for ln in (output or "").splitlines() if MARK in ln]
    for ln in lines:
        log("builder gate: " + ln.strip())
    return bool(lines)


def gate_command(repo=None):
    """Shell fragment the builder gates append after `make test`."""
    return "python3 -m tools.conveyor.pipeline.owned_offsets check"


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = p.add_subparsers(dest="command", required=True)
    c = sub.add_parser("check")
    c.add_argument("--repo", type=Path, default=REPO)
    c.add_argument("--map", type=Path)
    c.add_argument("--tu", action="append", help="limit to these ROM TUs (src/rom/X.c)")
    c.add_argument("-v", "--verbose", action="store_true", help="one line per placed section")
    a = p.parse_args(argv)
    try:
        results, problems = check(a.repo, a.map, a.tu)
    except OffsetError as exc:
        print(f"{MARK} FAIL {exc}")
        return 1
    if a.verbose:
        for r in results:
            print(f"{MARK} {r['tu']}({r['section']}) @{r['address']:#010x}+{r['size']:#x}"
                  f"{' owned ' + r['owned'] if r['owned'] else ''}: {r['checked']} at retail, "
                  f"{r['unlisted']} unlisted")
    for line in problems:
        print(f"{MARK} MISMATCH {line}")
    owned = [r for r in results if r["owned"]]
    summary = (f"{len(results)} ROM TU data sections ({len(owned)} owned slots), "
               f"{sum(r['checked'] for r in results)} named objects checked against retail addresses "
               f"({sum(r['checked'] for r in owned)} in owned slots), "
               f"{sum(r['unlisted'] for r in results)} without a retail address")
    print(f"{MARK} {'FAIL' if problems else 'OK'} {summary}")
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
