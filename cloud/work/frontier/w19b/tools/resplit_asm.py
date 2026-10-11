#!/usr/bin/env python3
"""w19b: apply reloc_addrs entries to splat GLOBAL_ASM files IN A SCRATCH COPY (what `make extract` would emit
after adding them to reloc_addrs.us.txt): the %hi/%lo operand of each listed ROM offset becomes
`%hi(symbol + 0xN)` / `%hi(symbol - 0xN)`.   python3 resplit_asm.py RELOC_FILE file.s [file.s...]"""
import re, sys
entries = {}
for line in open(sys.argv[1]):
    body = line.split("//")[0]
    a = dict(re.findall(r"(\w+):(\S+)", body))
    if a:
        entries[int(a["rom"], 0)] = ({"MIPS_HI16": "%hi", "MIPS_LO16": "%lo"}[a["reloc"]], a["symbol"], int(a.get("addend", "0"), 0))
for path in sys.argv[2:]:
    out, n = [], 0
    for line in open(path).read().split("\n"):
        m = re.match(r"\s*/\* ([0-9A-F]+) [0-9A-F]+ [0-9A-F]+ \*/", line)
        if m and int(m.group(1), 16) in entries:
            op, sym, add = entries[int(m.group(1), 16)]
            expr = f"{sym} + 0x{add:X}" if add >= 0 else f"{sym} - 0x{-add:X}"
            line, k = re.subn(re.escape(op) + r"\(\w+\)", f"{op}({expr})", line)
            assert k == 1, line
            n += 1
        out.append(line)
    open(path, "w").write("\n".join(out))
    print(f"{path}: {n} operands renamed")
