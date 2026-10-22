#!/usr/bin/env python3
"""odis.py ovl_a|ovl_b NAME [NAME...]: disassemble overlay functions (w39b helper; tdis.py for overlays).
jal targets named from asm/us/blob/symbols.json and asm/us/<ovl>/symbols.json."""
from pathlib import Path
import json, struct, subprocess, sys, tempfile, re
ROOT = Path(__file__).resolve().parents[5]
img = sys.argv[1]
d = ROOT / 'asm/us' / img
parsed = {}
cur = None
for path in sorted(d.glob('*.s')):
    for line in path.read_text().splitlines():
        m = re.match(r"\.section \.text\.(\S+?),", line.strip())
        if m:
            cur = parsed.setdefault(m.group(1), []); continue
        if line.strip().startswith('.section'):
            cur = None; continue
        m = re.match(r"\s*\.word\s+(0x[0-9A-Fa-f]+)", line)
        if m and cur is not None:
            cur.append(int(m.group(1), 16))
rev = {}
for sf in (ROOT / 'asm/us/blob/symbols.json', d / 'symbols.json'):
    try:
        s = json.load(open(sf)); s = s.get('symbols', s)
        for k, v in s.items():
            if isinstance(v, str): rev.setdefault(int(v, 16), []).append(k)
            elif isinstance(v, int): rev.setdefault(v, []).append(k)
            elif isinstance(v, dict) and 'address' in v: rev.setdefault(int(str(v['address']), 0), []).append(k)
    except Exception as e:
        pass
for name in sys.argv[2:]:
    words = parsed[name]; va = int(name.split('_')[1], 16)
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB',
                              f'--adjust-vma=0x{va:x}', '-M', 'gpr-names=32', f.name], capture_output=True, text=True).stdout
    print(f'== {name} @ 0x{va:08X} ({len(words)} words)')
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if not m: continue
        ins = m.group(3)
        j = re.search(r'jal\s+(0x)?([0-9a-f]+)', ins)
        if j: ins += '   <' + ','.join(rev.get(int(j.group(2), 16), ['?'])) + '>'
        print(f'  {m.group(1)}: {ins}')
