#!/usr/bin/env python3
"""Print MAT3 function bodies: mat3body.py FILE.c NAME [NAME...]"""
import re, sys
src = open(f'reference/repos/mat3/decomp/src/GameSource/src/{sys.argv[1]}', errors='replace').read()
for name in sys.argv[2:]:
    for m in re.finditer(r'^[^\n;#]*\b' + re.escape(name) + r'\s*\([^;{]*\)\s*\{', src, re.M):
        i = m.end(); depth = 1
        while depth and i < len(src):
            depth += {'{': 1, '}': -1}.get(src[i], 0); i += 1
        line = src.count('\n', 0, m.start()) + 1
        print(f'// {sys.argv[1]}:{line}')
        print(src[m.start():i]); print()
