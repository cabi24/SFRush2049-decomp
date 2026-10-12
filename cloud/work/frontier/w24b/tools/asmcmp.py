#!/usr/bin/env python3
"""w23c triage (builder, scratch root): compare a compiled object's FN against a nonmatching .s (no inventory target).
    python3 asmcmp.py OBJ FN ASM.s
Target words come from the .s comments (/* ROM VRAM WORD */) and its %hi/%lo/jal symbol operands; ours from
objdump -dr. Relocated words compare with the immediate masked plus the relocation symbol; others exactly."""
import re, subprocess, sys
obj, fn, asm = sys.argv[1:4]
t = []
for l in open(asm):
    m = re.search(r'/\* [0-9A-F]+ [0-9A-F]+ ([0-9A-F]{8}) \*/\s+(\S+)\s*(.*)', l)
    if not m: continue
    w, op, args = int(m.group(1), 16), m.group(2), m.group(3)
    s = re.search(r'%(?:hi|lo)\((\w+)\)', args)
    sym = s.group(1) if s else (args.strip() if op == 'jal' else None)
    t.append((w, sym))
out = subprocess.run(['mips-linux-gnu-objdump', '-dr', '-j', '.text', obj], capture_output=True, text=True).stdout
rows, cur, infn = [], None, False
for l in out.splitlines():
    h = re.match(r'^[0-9a-f]+ <(\w+)>:', l)
    if h: infn = h.group(1) == fn; continue
    if not infn: continue
    m = re.match(r'\s*[0-9a-f]+:\s+([0-9a-f]{8})\s', l)
    if m: rows.append([int(m.group(1), 16), None]); continue
    m = re.match(r'\s*[0-9a-f]+:\s+R_MIPS_\w+\s+(\S+)', l)
    if m and rows: rows[-1][1] = m.group(1).split('+')[0]
while rows and rows[-1][0] == 0 and len(rows) > len(t): rows.pop()
diff = 0
for i in range(max(len(t), len(rows))):
    a = t[i] if i < len(t) else (None, None); b = rows[i] if i < len(rows) else (None, None)
    if a[0] is None or b[0] is None: ok = False
    elif a[1] or b[1]:
        mask = 0xFC000000 if (a[0] >> 26) in (2, 3) else 0xFFFF0000
        ok = (a[0] & mask) == (b[0] & mask) and a[1] == b[1]
    else: ok = a[0] == b[0]
    if not ok:
        diff += 1
        print('%3d %08x %-22s | %08x %s' % (i, a[0] or 0, a[1] or '', b[0] or 0, b[1] or ''))
print('%s: target %d words, ours %d, differing %d' % (fn, len(t), len(rows), diff))
