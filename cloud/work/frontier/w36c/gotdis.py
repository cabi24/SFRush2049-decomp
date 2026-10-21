#!/usr/bin/env python3
"""gotdis.py WORDS.json [want|got] [start_hex end_hex]: disassemble the got/want words."""
import sys, json, struct, subprocess, tempfile, re
d = json.load(open(sys.argv[1])); which = sys.argv[2] if len(sys.argv) > 2 else 'got'
w = d[which]
with tempfile.NamedTemporaryFile(suffix='.bin') as f:
    f.write(struct.pack(f'>{len(w)}I', *w)); f.flush()
    out = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
lo = int(sys.argv[3], 16) if len(sys.argv) > 3 else 0
hi = int(sys.argv[4], 16) if len(sys.argv) > 4 else 1 << 30
for line in out.splitlines():
    m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
    if m and lo <= int(m.group(1), 16) < hi: print(f'{int(m.group(1),16):04x}  {m.group(3)}')
