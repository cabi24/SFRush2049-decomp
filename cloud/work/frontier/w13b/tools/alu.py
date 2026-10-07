#!/usr/bin/env python3
"""alu.py E.LOG : annotate a w13b uopt_al trace (ALTRACE=1 ALU=1) with ucode opcode names."""
import sys, re
sys.path.insert(0, '/home/cburnes/projects/rush2049-decomp/third_party/n64-decomp-workbench/src')
from decomp_workbench.ucode import OPCODE_NAMES
for line in open(sys.argv[1]):
    m = re.match(r'\s*U op=(\d+) b1=(\d+) w1=(\w+) w2=(\w+)', line)
    if m:
        op = int(m.group(1)); b1 = int(m.group(2))
        nm = OPCODE_NAMES[op] if op < len(OPCODE_NAMES) else '?%d' % op
        print('    %-6s dt/mt=%3d w1=%s w2=%s' % (nm, b1, m.group(3), m.group(4)))
    else:
        print(line.rstrip())
