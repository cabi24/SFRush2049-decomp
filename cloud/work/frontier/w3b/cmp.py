#!/usr/bin/env python3
"""cmp.py FN [--full]: side-by-side diff of FN in build/blob_unit/w3b/unit.o vs retail (tdis)."""
import sys, subprocess, re, difflib
fn = sys.argv[1]
root = '/home/cburnes/projects/rush2049-decomp'
out = subprocess.run(['mips-linux-gnu-objdump', '-d', '-r', '--no-show-raw-insn', root + '/build/blob_unit/w3b/unit.o'], capture_output=True, text=True).stdout
lines = []; on = False
for l in out.splitlines():
    if re.match(r'^[0-9a-f]+ <%s>:' % re.escape(fn), l): on = True; continue
    if on and re.match(r'^[0-9a-f]+ <', l): break
    if on:
        if 'R_MIPS' in l:
            if lines: lines[-1] += '  ' + l.split()[-1]
            continue
        m = re.match(r'\s+([0-9a-f]+):\s+(\S+)\s*(.*)', l)
        if m: lines.append((m.group(2) + ' ' + re.sub(r'\s*<.*>', '', m.group(3))).strip())
t = subprocess.run(['python3', root + '/cloud/work/tools/tdis.py', fn], capture_output=True, text=True).stdout.splitlines()[1:]
tl = []
for l in t:
    m = re.match(r'\s+([0-9a-f]+):\s+(\S+)\s*(.*)', l)
    if m: tl.append((m.group(2) + ' ' + re.sub(r'\s*<.*>', '', m.group(3))).strip())
def norm(s): return re.sub(r'0x[0-9a-f]+|-?\d+', 'N', s.split('  ')[0])
a = [norm(x) for x in tl]; b = [norm(x) for x in lines]
sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
full = '--full' in sys.argv
for op, i1, i2, j1, j2 in sm.get_opcodes():
    if op == 'equal' and not full:
        # show exact differences inside equal-shaped runs
        for k in range(i2 - i1):
            x, y = tl[i1 + k], lines[j1 + k].split('  ')[0]
            if x.replace(' ', '') != y.replace(' ', '') and not ('lui' in x or 'addiu' in x and 'R_' in lines[j1+k]) :
                pass
        continue
    n = max(i2 - i1, j2 - j1)
    for k in range(n):
        x = tl[i1 + k] if i1 + k < i2 else ''
        y = lines[j1 + k] if j1 + k < j2 else ''
        print('%4d %-40s | %s' % (i1 + k, x, y))
    print('-----')
print('retail %d, ours %d, aligned-equal %d' % (len(tl), len(lines), sum(i2 - i1 for op, i1, i2, j1, j2 in sm.get_opcodes() if op == 'equal')))
