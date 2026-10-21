#!/usr/bin/env python3
"""gotdis.py [want|got] : disassemble words.json from last sbs run"""
import sys, json, struct, subprocess, tempfile, re
d = json.load(open('/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/w36e/words.json'))
w = d[sys.argv[1] if len(sys.argv) > 1 else 'got']
with tempfile.NamedTemporaryFile(suffix='.bin') as f:
    f.write(struct.pack(f'>{len(w)}I', *w)); f.flush()
    out = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
for line in out.splitlines():
    m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
    if m: print(f'{int(m.group(1),16):04x}  {m.group(3)}')
