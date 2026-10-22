#!/usr/bin/env python3
"""nb.py NAME [N]: N game functions before/after NAME in address order, with size, matched flag
and MAT3 identity (w36n map + overrides). Read-only."""
import json, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(ROOT / 'cloud/work/frontier/w39a/tools'))
sys.argv, args = sys.argv[:1], sys.argv[1:]
import importlib.util
spec = importlib.util.spec_from_file_location('fx', ROOT / 'cloud/work/frontier/w39a/tools/fx.py')
fx = importlib.util.module_from_spec(spec); spec.loader.exec_module(fx)
T = fx.T; syms = fx.syms
fns = sorted((syms[n], n) for n in T if n in syms)
name = args[0]; k = int(args[1]) if len(args) > 1 else 8
idx = [i for i, (a, n) in enumerate(fns) if n == name][0]
for a, n in fns[max(0, idx - k): idx + k + 1]:
    print(f"{'>>' if n == name else '  '} {a:08X} {4*len(T[n]):6d} {fx.label(a)}")
