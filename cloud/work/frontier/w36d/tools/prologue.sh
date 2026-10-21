#!/bin/bash
# prologue.sh VARIANT_DIR : print race-loop prologue of func_8038CD14 (between saveTrackNum store and first jal after it)
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36d || exit 1
./tools/sbs.sh group $1 func_8038CD14 > /dev/null 2>&1
python3 - <<'PY'
import json,struct,subprocess,tempfile,re
d=json.load(open('/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/words.json'))
def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32',f.name],capture_output=True,text=True).stdout
    return [re.sub(r'\s+',' ',m.group(3)) for m in (re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', l) for l in out.splitlines()) if m]
g=dis(d['got'])
# find third saveTrackNum store region: after 'slti at,s?,6'
for i,x in enumerate(g):
    if re.match(r'slti at,\w+,6$', x):
        print(' | '.join(g[i:i+14])); break
PY
