#!/usr/bin/env python3
"""Run cloud/work/boot_tail_promotion/context_check.py (unchanged) with a ROM-TU
flag variant, for segments whose TU compiles at a non-default level:
    CTX_OPT=-O1 python3 ctx.py OUT.jsonl fn=src ...        (opt_overrides.mk -O1 TUs)
    CTX_ISA="-mips3 -32" CTX_OPT=-O1 python3 ctx.py ...    (libgcc TU; needs a new override)
Run from the repo/scratch root (context_check uses Path.cwd())."""
import os, sys
from pathlib import Path
sys.path.insert(0, str(Path.cwd() / "cloud/work/boot_tail_promotion"))
import context_check as cc  # noqa: E402
opt = os.environ.get("CTX_OPT", "-O2")
isa = os.environ.get("CTX_ISA", "-mips2").split()
flags = list(cc.CFLAGS)
i = flags.index("-mips2"); flags[i:i + 1] = isa
flags[flags.index("-O2")] = opt
cc.CFLAGS = flags
print("context_check CFLAGS:", " ".join(f for f in flags if not f.startswith("-I")))
cc.main()
