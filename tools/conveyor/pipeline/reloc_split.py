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

Two more relocation-only differences are naming facts of the same kind (w20c):

* An unpaired site. splat could not pair a `lui`/`lw` the compiler emitted
  for a symbol, so the target carries the raw words and no relocation, while
  the IDO object relocates against the symbol. The entry names the symbol
  (`reloc:MIPS_HI16 symbol:D_80038292`); allowed only where the candidate's
  field is zero and the target word encodes exactly that symbol's half.
* A constant splat paired as an address. A `lui at,0x8000` (float->u32
  conversion) followed by an unrelated `addiu ...,63` was named `D_8000003F`;
  the IDO object has no relocation there. The entry renders the constant
  (`reloc:CUSTOM_CONSTANT_HI symbol:0x80000000` -> `(0x80000000 >> 16)`,
  `reloc:CUSTOM_CONSTANT_LO symbol:0x3F` -> `(0x3F & 0xFFFF)`, spimdisasm's own
  constant syntax); allowed only where the candidate's field equals the
  target symbol's half.

Sites the candidate relocates against one of its own sections (`.rodata`: a
jump table the function owns) are not naming splits: they need an owned
data slot (`owned_data prepare`). `derive` lists them as comments and
proposes no entry for them.
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
CONSTANT_TYPES = {"R_MIPS_HI16": "CUSTOM_CONSTANT_HI", "R_MIPS_LO16": "CUSTOM_CONSTANT_LO"}
_ENTRY_RE = re.compile(r"(\w+):(\S+)")


class SplitError(Exception):
    """A relocation difference that is not a pure base+addend naming split."""


def hi_half(address):
    return ((address + 0x8000) >> 16) & 0xFFFF


def lo_half(address):
    return address & 0xFFFF


def encodes(reloc_type, word, address):
    """True iff a HI16/LO16 instruction word's immediate encodes `address`
    (for the CUSTOM_CONSTANT types, the constant as spimdisasm renders it:
    `(value >> 16)` without the %hi carry, `(value & 0xFFFF)`)."""
    field = word & 0xFFFF
    if reloc_type == "MIPS_HI16":
        return word >> 26 == 0x0F and field == hi_half(address)  # lui
    if reloc_type == "MIPS_LO16":
        return field == lo_half(address)
    if reloc_type == "CUSTOM_CONSTANT_HI":
        return word >> 26 == 0x0F and field == (address >> 16) & 0xFFFF
    if reloc_type == "CUSTOM_CONSTANT_LO":
        return field == lo_half(address)
    return False


def _constant(text):
    """The integer a CUSTOM_CONSTANT entry's `symbol:` attribute spells, else None."""
    try:
        return int(text, 0)
    except ValueError:
        return None


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
    is the same absolute address named two ways. Raises SplitError otherwise.
    Own-section sites (see module docstring) come back with type None."""
    sites = []
    for offset in sorted(set(target_relocs) | set(cand_relocs)):
        t_type, t_sym = target_relocs.get(offset, (None, None))
        c_type, c_sym = cand_relocs.get(offset, (None, None))
        if t_type is None or c_type is None:
            sites.append(_unpaired_site(offset, t_type, t_sym, c_type, c_sym,
                                        target_words, cand_words, resolve))
            continue
        if t_type != c_type:
            raise SplitError(f"+{offset:#x}: type {t_type} != {c_type}")
        if t_sym == c_sym:
            continue
        if c_sym.startswith(".") and t_type in TYPES:
            sites.append((offset, None, c_sym, 0, t_sym))  # owned section, not a name
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


def _unpaired_site(offset, t_type, t_sym, c_type, c_sym, target_words, cand_words, resolve):
    """A HI16/LO16 relocation present on one side only (module docstring)."""
    rtype = t_type or c_type
    if rtype not in TYPES:
        raise SplitError(f"+{offset:#x}: {rtype} only in the "
                         f"{'target' if t_type else 'candidate'} ({t_sym or c_sym})")
    if offset // 4 >= min(len(target_words), len(cand_words)):
        raise SplitError(f"+{offset:#x}: outside the function")
    t_word, c_word = target_words[offset // 4], cand_words[offset // 4]
    if (t_word ^ c_word) & ~0xFFFF & 0xFFFFFFFF:
        raise SplitError(f"+{offset:#x}: instruction differs ({t_word:08x} vs {c_word:08x})")
    half = hi_half if rtype == "R_MIPS_HI16" else lo_half
    if c_type is None:
        # Target names a split symbol over a plain constant field.
        address = resolve(t_sym)
        if address is None:
            raise SplitError(f"+{offset:#x}: unresolved {t_sym}")
        inplace = t_word & 0xFFFF  # REL: the target's own addend (`%lo(sym + 0x1)`)
        if rtype == "R_MIPS_HI16":
            address += inplace << 16
        else:
            address += inplace - 0x10000 if inplace & 0x8000 else inplace
        address &= 0xFFFFFFFF
        field = c_word & 0xFFFF
        if rtype == "R_MIPS_HI16":
            if field != (address >> 16) & 0xFFFF or field != hi_half(address):
                raise SplitError(f"+{offset:#x}: constant {field:#06x} is not %hi({t_sym})")
            value = field << 16
        else:
            if field != lo_half(address):
                raise SplitError(f"+{offset:#x}: constant {field:#06x} is not %lo({t_sym})")
            value = field
        name = t_sym if not inplace else f"{t_sym} + {inplace:#x}" if rtype == "R_MIPS_LO16" else t_sym
        return (offset, CONSTANT_TYPES[rtype], f"0x{value:X}", 0, name)
    # Candidate relocates where the target left raw words (an unpaired lui/load).
    address = resolve(c_sym)
    if address is None:
        raise SplitError(f"+{offset:#x}: unresolved {c_sym}")
    if c_word & 0xFFFF:
        raise SplitError(f"+{offset:#x}: {c_sym} carries an in-place addend; name it by hand")
    if t_word & 0xFFFF != half(address):
        raise SplitError(f"+{offset:#x}: target field {t_word & 0xFFFF:#06x} is not "
                         f"{'%hi' if rtype == 'R_MIPS_HI16' else '%lo'}({c_sym})")
    return (offset, TYPES[rtype], c_sym, 0, "an unpaired raw immediate")


def derive(vaddr, target_o, cand_o, resolve=targetsmod._resolve_symbol):
    """reloc_addrs lines for one function at `vaddr` (see module docstring)."""
    sites = derive_sites(text_relocs(target_o), text_words(target_o),
                         text_relocs(cand_o), text_words(cand_o), resolve)
    lines = []
    for off, rtype, base, addend, split in sites:
        rom = vaddr + off - targetsmod.STATIC_ROM_DELTA
        if rtype is None:
            lines.append(f"// rom:0x{rom:X} {split} -> {base}: own section, needs an owned data slot")
        elif rtype in CONSTANT_TYPES.values():
            lines.append(f"rom:0x{rom:X} reloc:{rtype} symbol:{base} // was {split}")
        else:
            lines.append(f"rom:0x{rom:X} reloc:{rtype} symbol:{base} "
                         f"addend:{'-' if addend < 0 else ''}0x{abs(addend):X} // was {split}")
    return lines


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
        if rtype.startswith("CUSTOM_CONSTANT_"):
            base = _constant(symbol)
            if base is None or addend:
                problems.append((n, f"{rtype} needs a numeric symbol and no addend"))
                continue
        else:
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
