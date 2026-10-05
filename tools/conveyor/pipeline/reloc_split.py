"""Relocation symbol splits: name a %hi/%lo site as base+addend (splat reloc_addrs).

    python3 -m tools.conveyor.pipeline.reloc_split derive FN --candidate cand.o [--target target.o]
    python3 -m tools.conveyor.pipeline.reloc_split check [--file reloc_addrs.us.txt]

splat names every address that a %hi/%lo pair reaches as its own `D_` symbol.
When the original code reached that address as `base+offset` (an array element,
a field, or an array's end pointer), the reloc-aware target relocates against
the split name while a byte-identical IDO object relocates against `base` with
the offset in the instruction field. The linked words are equal, but the
relocations (symbol, in-place addend) differ, so no true score 0 exists.

The decomp-standard fix is per-instruction relocation naming through splat's
`reloc_addrs.us.txt` (`rom:... reloc:MIPS_HI16 symbol:base addend:0x...`),
followed by a re-split. Only the listed instructions change. The symbol itself
stays, as do other references to it. The scorer keeps comparing symbol+addend.

`derive` proposes entries for one function. It pairs the target and candidate
relocations by offset and type. An entry is proposed only where the symbols
differ, the target's in-place field is zero (a plain split name), and the
candidate's in-place field encodes exactly the addend that makes
address(base) + addend == address(split name). A wrong global or a wrong
addend is refused. `check` validates every entry against the ROM word, so a
listed name can never change the bytes it describes.
"""
import argparse
import re
import struct
import subprocess
import sys
from pathlib import Path

from . import targets as targetsmod

REPO = targetsmod.REPO
RELOC_ADDRS = REPO / "reloc_addrs.us.txt"
TYPES = {"R_MIPS_HI16": "MIPS_HI16", "R_MIPS_LO16": "MIPS_LO16"}
_ENTRY_RE = re.compile(r"(\w+):(\S+)")


class SplitError(Exception):
    """A relocation difference that is not a pure base+addend naming split."""


def hi_half(address):
    return ((address + 0x8000) >> 16) & 0xFFFF


def lo_half(address):
    return address & 0xFFFF


def encodes(reloc_type, word, address):
    """True iff a HI16/LO16 instruction word's immediate encodes `address`."""
    field = word & 0xFFFF
    if reloc_type == "MIPS_HI16":
        return word >> 26 == 0x0F and field == hi_half(address)  # lui
    if reloc_type == "MIPS_LO16":
        return field == lo_half(address)
    return False


def _objdump(flag, obj):
    return subprocess.run(["mips-linux-gnu-objdump", flag, str(obj)],
                          capture_output=True, text=True, check=True).stdout


def text_relocs(obj):
    """{offset: (type, symbol)} for the .text relocations of an object."""
    out, in_text = {}, False
    for line in _objdump("-r", obj).splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            in_text = "[.text]" in line
            continue
        parts = line.split()
        if not in_text or len(parts) != 3:
            continue
        try:
            offset = int(parts[0], 16)
        except ValueError:
            continue
        out[offset] = (parts[1], parts[2])
    return out


def text_words(obj):
    from ..jobs import scoring
    return scoring._parse_text_words(_objdump("-dz", obj))


def derive_sites(target_relocs, target_words, cand_relocs, cand_words, resolve):
    """[(offset, type, base, addend, split_name)] proving each differing site
    is the same absolute address named two ways. Raises SplitError otherwise."""
    if sorted(target_relocs) != sorted(cand_relocs):
        raise SplitError("relocation offsets differ")
    sites = []
    for offset in sorted(target_relocs):
        t_type, t_sym = target_relocs[offset]
        c_type, c_sym = cand_relocs[offset]
        if t_type != c_type:
            raise SplitError(f"+{offset:#x}: type {t_type} != {c_type}")
        if t_sym == c_sym:
            continue
        if t_type not in TYPES:
            raise SplitError(f"+{offset:#x}: {t_type} {t_sym} != {c_sym}")
        t_addr, c_addr = resolve(t_sym), resolve(c_sym)
        if t_addr is None or c_addr is None:
            raise SplitError(f"+{offset:#x}: unresolved {t_sym if t_addr is None else c_sym}")
        if target_words[offset // 4] & 0xFFFF:
            raise SplitError(f"+{offset:#x}: target field already carries an addend")
        addend = t_addr - c_addr
        field = cand_words[offset // 4] & 0xFFFF
        expected = hi_half(addend) if t_type == "R_MIPS_HI16" else lo_half(addend)
        if field != expected:
            raise SplitError(f"+{offset:#x}: {c_sym} field {field:#06x} does not encode "
                             f"{t_sym} ({c_sym}+{addend:#x} needs {expected:#06x})")
        sites.append((offset, TYPES[t_type], c_sym, addend, t_sym))
    return sites


def derive(vaddr, target_o, cand_o, resolve=targetsmod._resolve_symbol):
    """reloc_addrs lines for one function at `vaddr` (see module docstring)."""
    sites = derive_sites(text_relocs(target_o), text_words(target_o),
                         text_relocs(cand_o), text_words(cand_o), resolve)
    return [f"rom:0x{vaddr + off - targetsmod.STATIC_ROM_DELTA:X} reloc:{rtype} "
            f"symbol:{base} addend:0x{addend:X} // was {split}"
            for off, rtype, base, addend, split in sites]


def parse(path=RELOC_ADDRS):
    """[(line_no, {attr: value})] for each entry in a reloc_addrs file."""
    entries = []
    if not Path(path).is_file():
        return entries
    for n, line in enumerate(Path(path).read_text().splitlines(), 1):
        body = line.split("//")[0].strip()
        if body:
            entries.append((n, dict(_ENTRY_RE.findall(body))))
    return entries


def check(path=RELOC_ADDRS, rom=None, resolve=targetsmod._resolve_symbol):
    """[(line_no, problem)]: every entry must name a resolvable symbol whose
    address plus addend is exactly what the ROM instruction encodes."""
    rom = rom if rom is not None else targetsmod.BASEROM.read_bytes()
    problems, seen = [], set()
    for n, attrs in parse(path):
        try:
            offset = int(attrs["rom"], 0)
            rtype, symbol = attrs["reloc"], attrs["symbol"]
            addend = int(attrs.get("addend", "0"), 0)
        except (KeyError, ValueError) as exc:
            problems.append((n, f"malformed entry: {exc}"))
            continue
        if offset in seen:
            problems.append((n, f"duplicate rom {offset:#x}"))
        seen.add(offset)
        base = resolve(symbol)
        if base is None:
            problems.append((n, f"{symbol} does not resolve"))
            continue
        if offset % 4 or offset + 4 > len(rom):
            problems.append((n, f"rom {offset:#x} is not an instruction in the ROM"))
            continue
        (word,) = struct.unpack_from(">I", rom, offset)
        if not encodes(rtype, word, base + addend):
            problems.append((n, f"{symbol}+{addend:#x} is not what {word:08X} encodes ({rtype})"))
    return problems


def _target_object(fn, data):
    from ..client import DEFAULT_DATA
    from ..coordinator import db as dbmod
    from ..coordinator.store import BlobStore
    data = Path(data or DEFAULT_DATA)
    row = dbmod.connect(data / "conveyor.db").execute(
        "SELECT address, target_o_sha FROM n64_target WHERE target_id=?", (fn,)).fetchone()
    if row is None or not row["target_o_sha"]:
        sys.exit(f"no target object for {fn}")
    return row["address"], BlobStore(data / "blobs").get(row["target_o_sha"])


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("derive")
    p.add_argument("function")
    p.add_argument("--candidate", required=True, help="IDO object of the byte-identical body")
    p.add_argument("--target", help="target object (default: the inventory's)")
    p.add_argument("--vaddr", help="function vaddr (default: the inventory's)")
    p.add_argument("--data", default=None)
    p = sub.add_parser("check")
    p.add_argument("--file", default=str(RELOC_ADDRS))
    args = parser.parse_args()

    if args.command == "check":
        problems = check(args.file)
        for n, why in problems:
            print(f"  {args.file}:{n}: {why}", file=sys.stderr)
        if problems:
            sys.exit(f"{len(problems)} reloc_addrs entr{'y' if len(problems) == 1 else 'ies'} rejected")
        print(f"all {len(parse(args.file))} reloc_addrs entries encode their ROM words")
        return
    if args.target and args.vaddr:
        vaddr, target = int(args.vaddr, 0), args.target
    else:
        vaddr, target = _target_object(args.function, args.data)
        target = args.target or target
        vaddr = int(args.vaddr, 0) if args.vaddr else vaddr
    try:
        lines = derive(vaddr, target, args.candidate)
    except SplitError as exc:
        sys.exit(f"{args.function}: not a naming split: {exc}")
    for line in lines:
        print(f"{line} ({args.function})")


if __name__ == "__main__":
    main()
