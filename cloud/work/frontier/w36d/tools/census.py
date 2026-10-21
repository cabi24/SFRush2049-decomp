#!/usr/bin/env python3
import json,struct,subprocess,tempfile,re,collections,sys
d=json.load(open(sys.argv[1]))
def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
    return [re.sub(r'\s+',' ',m.group(3)) for m in (re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', l) for l in out.splitlines()) if m]
for nm,ws in (('want',d['want']),('got',d['got'])):
    c=collections.defaultdict(list)
    for i,ins in enumerate(dis(ws)):
        m=re.match(r'(\w+) ([^,]+),(-?\d+)\(sp\)',ins)
        if m: c[int(m.group(3))].append(m.group(1)[0]+('f' if 'c1' in m.group(1) else ''))
        m=re.match(r'addiu (\w+),sp,(-?\d+)',ins)
        if m: c[int(m.group(2))].append('A')
    print(nm, ' '.join(f'{k}:{"".join(sorted(set(v)))}' for k,v in sorted(c.items())))
