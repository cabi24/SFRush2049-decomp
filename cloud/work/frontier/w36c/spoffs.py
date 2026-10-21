#!/usr/bin/env python3
"""spoffs.py words.json : sp-relative offset census, want vs got"""
import sys, json, struct, subprocess, tempfile, re, collections
d = json.load(open(sys.argv[1]))
def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        return subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
for k in ('want','got'):
    c = collections.defaultdict(list)
    for line in dis(d[k]).splitlines():
        m = re.search(r'\t(\w+)\s+([\$\w]+),(-?\d+)\(sp\)', line)
        if m: c[int(m.group(3))].append(m.group(1)[0]+':'+m.group(2))
        m = re.search(r'addiu\s+(\w+),sp,(-?\d+)', line)
        if m: c[int(m.group(2))].append('addr:'+m.group(1))
    print(k, ' '.join(f'{o}[{",".join(sorted(set(v)))}]' for o, v in sorted(c.items())))
