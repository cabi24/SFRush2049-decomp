#!/usr/bin/env python3
"""fnx.py FILE NAME: print the definition of NAME (from 'TYPE NAME(...)' line followed by '{' to the matching '}' at col 0)."""
import sys, re
lines = open(sys.argv[1]).read().split('\n'); n = sys.argv[2]
for i, l in enumerate(lines):
    if re.match(r'^[A-Za-z_].*\b' + n + r'\s*\(', l) and not l.rstrip().endswith(';') and i + 1 < len(lines) and (lines[i+1].startswith('{') or l.rstrip().endswith('{')):
        j = i
        while not lines[j].startswith('}'): j += 1
        # include preceding comment block
        k = i
        while k > 0 and (lines[k-1].startswith('/*') or lines[k-1].startswith(' *') or lines[k-1].startswith('   ')): k -= 1
        print('\n'.join(lines[k:j+1])); break
