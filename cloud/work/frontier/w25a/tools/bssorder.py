#!/usr/bin/env python3
"""w25a: predict an IDO TU's .bss order from cfe's block numbers (research tool).

IDO's cfe numbers every symbol-table entry ("block number") in the order it
first meets it; the .bss section is sorted by block number mod 256 (ties: top
level before function statics, then block number), variables referenced from
.data/.rodata first. Method and parser: zeldaret/oot tools/fix_bss.py and
tools/ido_block_numbers.py (CC0; the parser is vendored here unchanged).

Usage (builder, from a repo root; IDO_CC = the cc the Makefile uses):
  python3 bssorder.py TU.c [-- extra cc flags]
The TU goes through asm-processor first (its GLOBAL_ASM stand-ins take block
numbers too), then `cc <flags> -Hf` stops after cfe and leaves NAME.T / NAME.B.
Prints every .bss variable: block, block%256, size, predicted offset.
"""
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import ido_block_numbers as ibn  # noqa: E402

REPO = Path.cwd()
sys.path.insert(0, str(REPO / "tools/asm-processor"))
import asm_processor  # noqa: E402

FLAGS = ["-c", "-G", "0", "-mips2", "-O1", "-non_shared", "-Iinclude", "-Iinclude/PR",
         "-D_LANGUAGE_C", "-Wab,-r4300_mul", "-Xcpluscomm", "-g1"]


def bss_variables(symtab, ucode):
    init = set(op.i1 for op in ucode if op.opcode_name == "init")
    out, refd, fn = [], [], None
    for op in ucode:
        if op.opcode_name in ("gsym", "lsym", "fsym"):
            b = op.i1
            if b in init:
                continue
            top = op.opcode_name != "fsym"
            name = symtab[b].name if top else f"{fn}::{symtab[b].name}"
            size = op.args[0]
            align = 8 if size >= 8 else 1 << op.lexlev
            out.append(dict(block=b, top=top, name=name, size=size, align=align))
        elif op.opcode_name == "init" and op.dtype == 10:
            refd.append(op.const)
        elif op.opcode_name == "ent":
            fn = symtab[op.i1].name
    for v in out:
        v["refd"] = v["block"] in refd
    first = [v for v in out if v["refd"]]
    first.sort(key=lambda v: refd.index(v["block"]))
    rest = [v for v in out if not v["refd"]]
    rest.sort(key=lambda v: (v["block"] % 256, not v["top"], v["block"] if v["top"] else -v["block"]))
    off = 0
    for v in first + rest:
        off = (off + v["align"] - 1) & ~(v["align"] - 1)
        v["offset"] = off
        off += v["size"]
    return first + rest


def main():
    args = sys.argv[1:]
    extra = []
    if "--" in args:
        i = args.index("--")
        extra, args = args[i + 1:], args[:i]
    tu = Path(args[0]).resolve()
    cc = os.environ.get("IDO_CC", str(REPO / "tools/ido-static-recomp/build/out/cc"))
    flags = FLAGS + extra
    with tempfile.TemporaryDirectory(prefix="bssorder") as tmp:
        pp = Path(tmp) / tu.name
        sys.path.insert(0, str(REPO / "tools/asm-processor"))
        from compiler_flags import processor_flags
        with pp.open("wb") as f:
            asm_processor.run(processor_flags(flags) + [str(tu)], outfile=f)
        cmd = [cc] + flags + ["-I", str(tu.parent), "-I", str(REPO / "include"), "-I", str(REPO / "include/PR"),
                              "-Hf", pp.name]
        subprocess.run(cmd, check=True, cwd=tmp)
        stem = pp.stem
        symtab = ibn.parse_symbol_table((Path(tmp) / f"{stem}.T").read_bytes())
        ucode = ibn.parse_ucode((Path(tmp) / f"{stem}.B").read_bytes())
    for v in bss_variables(symtab, ucode):
        print(f"{v['offset']:#06x} {v['name']:<28} block={v['block']:<5} mod={v['block'] % 256:<4} "
              f"size={v['size']:#x}{' refd' if v['refd'] else ''}")


if __name__ == "__main__":
    main()
