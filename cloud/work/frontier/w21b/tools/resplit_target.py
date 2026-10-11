#!/usr/bin/env python3
"""w19b (Pi, read-only): emulate `reloc_addrs.us.txt` entries + re-split + target refresh for static functions.

    python3 cloud/work/frontier/w19b/tools/resplit_target.py RELOC_FILE OUTDIR fn [fn...]

For each fn: its splat region (targets.index_asm_regions over asm/us) is taken verbatim; every instruction
whose ROM offset is named in RELOC_FILE (reloc_addrs syntax) gets its %hi/%lo operand rewritten to
`%hi(symbol + addend)` / `%lo(symbol + addend)` (what splat/spimdisasm emits for a reloc_addrs entry);
the region is assembled with targets.assemble_region (the inventory's reloc-aware target builder) into
OUTDIR/<fn>_rs.o and passed through targets.gate_target (the inventory's round-trip gate). The addresses
themselves are proven by `reloc_split check --file` (every entry encodes its ROM word). Nothing in the repo or the DB is written.
"""
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.pipeline import reloc_split, targets  # noqa: E402

reloc_file, outdir = Path(sys.argv[1]), Path(sys.argv[2])
outdir.mkdir(parents=True, exist_ok=True)
entries = {}
for _, attrs in reloc_split.parse(reloc_file):
    entries[int(attrs["rom"], 0)] = (attrs["reloc"], attrs["symbol"], int(attrs.get("addend", "0"), 0))
by_name = {r.name: r for r in targets.index_asm_regions().values()}
OP = {"MIPS_HI16": "%hi", "MIPS_LO16": "%lo"}
for fn in sys.argv[3:]:
    region = by_name[fn]
    lines, idx, changed = [], 0, 0
    for line in region.lines:
        t = line.strip()
        is_insn = bool(t) and (t.startswith(".word 0x") or not (t.startswith((".", "/", "glabel", "#")) or t.endswith(":")))
        if is_insn and idx < len(region.words):
            rom = region.vaddr + 4 * idx - targets.STATIC_ROM_DELTA
            idx += 1
            if rom in entries:
                rtype, sym, add = entries[rom]
                expr = f"{sym} + 0x{add:X}" if add >= 0 else f"{sym} - 0x{-add:X}"
                new, n = re.subn(re.escape(OP[rtype]) + r"\(\w+\)", f"{OP[rtype]}({expr})", line)
                assert n == 1, (fn, hex(rom), line)
                line, changed = new, changed + 1
        lines.append(line)
    out = outdir / f"{fn}_rs.o"
    targets.assemble_region(targets.Region(fn, region.vaddr, lines, region.words), fn, out)
    ok, why = targets.gate_target(region.words, out)  # the inventory's round-trip gate (masked words)
    assert idx == len(region.words), (fn, idx, len(region.words))
    print(f"{fn}: {changed} operands renamed, gate_target {'OK' if ok else 'FAIL ' + str(why)} -> {out}")
