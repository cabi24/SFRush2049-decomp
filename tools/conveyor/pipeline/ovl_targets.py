"""Targets for the two runtime code images loaded at 0x8038A400 (R16).

    python3 -m tools.conveyor.pipeline.ovl_targets generate [--image a|b ...]
    python3 -m tools.conveyor.pipeline.ovl_targets report

The cartridge holds two raw-deflate code images besides the game image,
pointed to by D_8002B020 (image A, front end) and D_8002B024 (image B, race
HUD/stunts/weapons). Both are loaded at the same address, so a name such as
func_8038D798 is only meaningful per image. Each image gets its own target
directory in the scorer's format (tools/cloud/score.py --targets):

    asm/us/ovl_<x>/ovl_<x>_8038a400.s   one .text.<name> section of .words per function
    asm/us/ovl_<x>/symbols.json         game symbols + this image's functions
    asm/us/ovl_<x>/extents.json         every function's extent and start evidence
    asm/us/ovl_<x>/SHA256SUMS

Function discovery is deterministic. Tile the text from the base with the
game image's extent scanner (targets.scan_extent): each tile ends at the
first eligible `jr $ra`, and zero padding is skipped. Text ends where a tile
no longer scans. A tile starts a function when there is evidence for it: the
image base, a `jal` target (from this image, or from the game image when the
address starts a tile here), a lui/addiu or data-word reference outside a
switch table, a stack prologue, or a bare `jr $ra; nop` (an empty function).
A tile with none of those is merged into the preceding function. If a
recognised switch table reaches it, the merge is proven; otherwise it is
listed under `unproven_merges`. Call targets that land inside a function
are reported, never silently dropped.
"""
import argparse
import hashlib
import json
import re
import struct
import sys
import zlib
from pathlib import Path

from . import targets as targetsmod

REPO = targetsmod.REPO
BASEROM = targetsmod.BASEROM
BASE = 0x8038A400
GAME_BASE = targetsmod.GAME_CODE_BASE
GAME_ROM = 0xB0CB10
STATIC_DELTA = 0x80000400 - 0x1000
# ROM offset of the boot-segment word holding each image's ROM address.
POINTERS = {"a": 0x8002B020, "b": 0x8002B024}
OUT = REPO / "asm" / "us"
GAME_SYMBOLS = OUT / "blob" / "symbols.json"
JR_RA = 0x03E00008


def word(buf, off):
    return struct.unpack_from(">I", buf, off)[0]


def inflate(rom, offset):
    z = zlib.decompressobj(-15)
    data = z.decompress(rom[offset:offset + 0x100000])
    if not z.eof:
        raise ValueError(f"raw deflate at 0x{offset:X} did not end")
    return data


def image_rom_offset(rom, image):
    return word(rom, POINTERS[image] - STATIC_DELTA)


def jal_targets(buf, lo, hi):
    out = set()
    for off in range(0, len(buf) - 3, 4):
        w = word(buf, off)
        if w >> 26 == 3:
            target = (w & 0x3FFFFFF) << 2 | 0x80000000
            if lo <= target < hi:
                out.add(target)
    return out


def hilo_targets(buf, base, lo, hi):
    """Addresses built by lui rX,hi + addiu rY,rX,lo within six instructions."""
    out = set()
    for off in range(0, len(buf) - 4, 4):
        w = word(buf, off)
        if w >> 26 != 0x0F:
            continue
        reg = (w >> 16) & 31
        for nxt in range(off + 4, min(off + 28, len(buf)), 4):
            v = word(buf, nxt)
            if v >> 26 == 0x09 and (v >> 21) & 31 == reg:
                imm = v & 0xFFFF
                addr = (((w & 0xFFFF) << 16) + (imm - 0x10000 if imm & 0x8000 else imm)) & 0xFFFFFFFF
                if lo <= addr < hi and addr % 4 == 0:
                    out.add(addr)
                break
    return out


def switch_tables(img, text_end, base=BASE, start=None):
    """{table address: [destinations]} for IDO `jr rX` dispatches in the text.

    Recognises lui rB,hi ... lw rX,lo(rB) ... jr rX, with the entry count
    from the nearest preceding `sltiu rT,rI,N`."""
    tables = {}
    for off in range((start or base) - base, text_end - base, 4):
        w = word(img, off)
        if w & 0xFC1FFFFF != 0x00000008 or (w >> 21) & 31 == 31:
            continue
        reg = (w >> 21) & 31
        load = upper = count = None
        for back in range(off - 4, max(-4, off - 48), -4):
            v = word(img, back)
            if load is None and v >> 26 == 0x23 and (v >> 16) & 31 == reg:
                load = v
            elif load is not None and upper is None and v >> 26 == 0x0F \
                    and (v >> 16) & 31 == (load >> 21) & 31:
                upper = v
            elif count is None and v >> 26 == 0x0B:
                count = v & 0xFFFF
        if load is None or upper is None or not count:
            continue
        lo = load & 0xFFFF
        table = (((upper & 0xFFFF) << 16) + (lo - 0x10000 if lo & 0x8000 else lo)) & 0xFFFFFFFF
        if not (base <= table and table + 4 * count <= base + len(img)):
            continue
        tables[table] = [word(img, table - base + 4 * i) for i in range(count)]
    return tables


def _reenters(img, base, owner, tile, size):
    """True when the tile and the function before it are one routine: the tile
    branches or jumps back into it, or it jumps (j) to the tile."""
    for pc in range(tile, tile + size, 4):
        w = word(img, pc - base)
        target = targetsmod.branch_target(w, pc)
        if target is None and w >> 26 == 2:
            target = (w & 0x3FFFFFF) << 2 | (pc & 0xF0000000)
        if target is not None and owner <= target < tile:
            return True
    return any(word(img, pc - base) >> 26 == 2
               and ((word(img, pc - base) & 0x3FFFFFF) << 2 | (pc & 0xF0000000)) == tile
               for pc in range(owner, tile, 4))


def discover(img, game, base=BASE, start=None, stop=None):
    """Function extents in img (mapped at base), tiling [start, stop).

    The runtime images tile from their base to where scanning stops; the boot
    tail passes the boot segment with start/stop around its CPU code."""
    start = base if start is None else start
    end = base + len(img) if stop is None else stop
    tiles, pc = [], start
    while pc < end:
        n = targetsmod.scan_extent(img, pc, base=base)
        if not isinstance(n, int):
            break
        tiles.append([pc, n * 4])
        pc += n * 4
        while pc < end and word(img, pc - base) == 0:
            pc += 4
    text_end = tiles[-1][0] + tiles[-1][1]
    starts = {t[0] for t in tiles}

    internal = jal_targets(img[:text_end - base], start, text_end)
    external = {t for t in jal_targets(game, start, text_end) if t in starts}
    tables = switch_tables(img, text_end, base, start)
    in_tables = {a for entries in tables.values() for a in entries}
    table_words = {t + 4 * i for t, entries in tables.items() for i in range(len(entries))}
    data_refs = {word(img, off) for off in range(text_end - base, len(img) - 3, 4)
                 if off + base not in table_words
                 and start <= word(img, off) < text_end and word(img, off) % 4 == 0}
    code_refs = hilo_targets(img[:text_end - base], base, start, text_end)

    functions, unproven, switch_merged = [], [], []
    for tile_start, size in tiles:
        evidence = []
        if tile_start == BASE and base == BASE:
            evidence.append("image_base")
        if tile_start in internal:
            evidence.append("jal")
        if tile_start in external:
            evidence.append("game_jal")
        if tile_start in code_refs:
            evidence.append("hilo_ref")
        if tile_start in data_refs:
            evidence.append("data_ref")
        if any(word(img, tile_start - base + 4 * i) >> 16 == 0x27BD
               for i in range(min(4, size // 4))):
            evidence.append("prologue")   # IDO may schedule a global load first
        if size == 8 and word(img, tile_start - base) == JR_RA and word(img, tile_start - base + 4) == 0:
            evidence.append("empty_function")
        joins = functions and (tile_start in in_tables or _reenters(
            img, base, functions[-1]["address"], tile_start, size))
        if evidence or not joins:
            if not evidence:
                evidence.append("standalone")   # no control flow from or to the previous function
            functions.append({"address": tile_start, "size": size, "evidence": evidence})
            continue
        previous = functions[-1]
        gap = tile_start - (previous["address"] + previous["size"])
        previous["size"] += gap + size
        (switch_merged if tile_start in in_tables else unproven).append(tile_start)

    bounds = [(f["address"], f["address"] + f["size"]) for f in functions]
    fn_starts = {f["address"] for f in functions}
    inside = sorted(t for t in internal | external if t not in fn_starts
                    and any(lo < t < hi for lo, hi in bounds))
    return {
        "text_end": text_end,
        "functions": functions,
        "switch_tables": len(tables),
        "switch_merged": switch_merged,
        "unproven_merges": unproven,
        "call_targets_inside_functions": inside,
        "game_calls_here": sorted(external),
    }


def name(address):
    return f"func_{address:08X}"


BOOT_BASE = 0x80000400
BOOT_ROM = (0x1000, 0x2F4E0)          # IPL3 copies this to BOOT_BASE; BSS follows
BOOT_TAIL = (0x8000F3A4, 0x80028000)  # CPU code past the counted static range
SYMBOL_ADDRS = REPO / "symbol_addrs.us.txt"


def static_symbols():
    out = {}
    for line in SYMBOL_ADDRS.read_text().splitlines():
        m = re.match(r"\s*([A-Za-z_]\w*)\s*=\s*0x([0-9A-Fa-f]+)\s*;", line)
        if m:
            out.setdefault(m.group(1), f"0x{int(m.group(2), 16):08X}")
    return out


def load(image, rom, game):
    """(img, base, found, provenance) for one target set."""
    if image == "boot_tail":
        img = rom[BOOT_ROM[0]:BOOT_ROM[1]]
        found = discover(img, game, base=BOOT_BASE, start=BOOT_TAIL[0], stop=BOOT_TAIL[1])
        return img, BOOT_BASE, found, {
            "rom_range": f"0x{BOOT_ROM[0]:X}-0x{BOOT_ROM[1]:X}",
            "note": "boot segment tail past the counted static code (ROM 0x10000); "
                    "starts at __osPfsRWInode, whose static target is cut at ROM 0x10000"}
    rom_offset = image_rom_offset(rom, image)
    img = inflate(rom, rom_offset)
    return img, BASE, discover(img, game), {
        "rom_offset": f"0x{rom_offset:06X}", "pointer": f"0x{POINTERS[image]:08X}"}


def directory(image):
    return "boot_tail" if image == "boot_tail" else f"ovl_{image}"


def render(image, img, found, base=BASE):
    first = found["functions"][0]["address"]
    title = "boot segment tail" if image == "boot_tail" else f"runtime image {image.upper()}"
    lines = [f"/* {title}: 0x{first:08X}-0x{found['text_end']:08X} text,"
             f" generated by tools.conveyor.pipeline.ovl_targets */",
             ".set noreorder", ".set noat", ""]
    for fn in found["functions"]:
        label = name(fn["address"])
        lines += [f'.section .text.{label}, "ax", @progbits', f".globl {label}", f"{label}:"]
        off = fn["address"] - base
        lines += [f"    .word 0x{word(img, off + i):08X}" for i in range(0, fn["size"], 4)]
        lines.append("")
    return "\n".join(lines)


def generate(image, rom, game, out_root=OUT):
    img, base, found, provenance = load(image, rom, game)
    out = Path(out_root) / directory(image)
    out.mkdir(parents=True, exist_ok=True)
    for stale in out.glob("*.s"):
        stale.unlink()
    first = found["functions"][0]["address"] if image == "boot_tail" else base
    region = out / f"{directory(image)}_{first:08x}.s"
    region.write_text(render(image, img, found, base))
    symbols = dict(json.loads(GAME_SYMBOLS.read_text())["symbols"])
    if image == "boot_tail":
        symbols.update(static_symbols())
    symbols.update({name(f["address"]): f"0x{f['address']:08X}" for f in found["functions"]})
    label = "BOOT_TAIL" if image == "boot_tail" else image.upper()
    (out / "symbols.json").write_text(json.dumps({
        "generated_by": "python3 -m tools.conveyor.pipeline.ovl_targets generate",
        "image": label,
        "image_sha256": hashlib.sha256(img).hexdigest(),
        "base": f"0x{base:08X}",
        "symbols": dict(sorted(symbols.items())),
    }, indent=1) + "\n")
    extents = {"image": label, **{k: v for k, v in provenance.items() if k != "note"},
               "size": len(img), "image_sha256": hashlib.sha256(img).hexdigest(),
               "base": f"0x{base:08X}"}
    if "note" in provenance:
        extents["note"] = provenance["note"]
    extents.update({
        "text_end": f"0x{found['text_end']:08X}",
        "switch_tables": found["switch_tables"],
        "switch_merged": [f"0x{a:08X}" for a in found["switch_merged"]],
        "unproven_merges": [f"0x{a:08X}" for a in found["unproven_merges"]],
        "call_targets_inside_functions": [f"0x{a:08X}" for a in found["call_targets_inside_functions"]],
        "game_calls_here": [f"0x{a:08X}" for a in found["game_calls_here"]],
        "functions": [{"name": name(f["address"]), "address": f"0x{f['address']:08X}",
                       "size": f["size"], "evidence": f["evidence"]} for f in found["functions"]],
    })
    (out / "extents.json").write_text(json.dumps(extents, indent=1) + "\n")
    sums = [f"{hashlib.sha256(p.read_bytes()).hexdigest()}  {p.name}"
            for p in sorted(out.iterdir()) if p.name != "SHA256SUMS"]
    (out / "SHA256SUMS").write_text("\n".join(sums) + "\n")
    return out, found


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest="command", required=True)
    g = sub.add_parser("generate")
    g.add_argument("--image", action="append", choices=sorted(POINTERS) + ["boot_tail"])
    sub.add_parser("report")
    args = parser.parse_args()
    rom = BASEROM.read_bytes()
    game = inflate(rom, GAME_ROM)
    for image in (getattr(args, "image", None) or sorted(POINTERS) + ["boot_tail"]):
        if args.command == "generate":
            out, found = generate(image, rom, game)
            where = out.relative_to(REPO)
        else:
            found, where = load(image, rom, game)[2], "(not written)"
        fns = found["functions"]
        print(f"{directory(image)}: {len(fns)} functions, "
              f"{found['text_end'] - fns[0]['address']} text bytes, "
              f"{found['switch_tables']} switch tables, "
              f"{len(found['switch_merged'])} switch blocks merged, "
              f"{len(found['unproven_merges'])} unproven merges, "
              f"{len(found['call_targets_inside_functions'])} call targets inside functions, "
              f"{len(found['game_calls_here'])} game call targets -> {where}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
