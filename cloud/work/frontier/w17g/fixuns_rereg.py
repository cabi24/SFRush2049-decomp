#!/usr/bin/env python3
"""w17g: dry-run (or apply to a SCRATCH DB copy) the reloc-aware re-registration of __fixunsdfdi/__fixunssfdi with
the one-line assembler change they need: targets.assemble_region's isolated prelude lacks `.set gp=64`, so GNU as
(-mabi=32) expands the 64-bit `ld $t7, %lo(gInfinity)($t7)` as a macro and refuses ("macro used $at after
.set noat"). The ROM build's asm-processor prelude.inc has `.set gp=64`. This script patches the function
IN MEMORY ONLY and runs targets_rename. Usage: python3 fixuns_rereg.py --data SCRATCH_DIR apply NAMES [--dry-run]"""
import inspect, sys
from pathlib import Path
sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.pipeline import targets, targets_rename  # noqa: E402
src = inspect.getsource(targets.assemble_region)
assert '".set noat",' in src
src = src.replace('".set noat",', '".set noat",\n        ".set gp=64",', 1)
exec(compile(src, targets.__file__, "exec"), targets.__dict__)
sys.exit(targets_rename.main(sys.argv[1:]) if hasattr(targets_rename, "main") else 2)
