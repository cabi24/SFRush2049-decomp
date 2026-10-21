#!/bin/sh
# usage: gdiff_ee.sh FILE.c -- aligned diff of explosion_effect compiled in the gr3-context group (w36x3 scratch)
scp -q "$1" watchman2:rush2049/scratch/frontier/w36x3/cand/g_ee/ee.c && ssh watchman2 "cd ~/rush2049/scratch/frontier/w36x3 && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 - <<'PY'
import sys, json, tempfile, difflib, re, struct, subprocess
sys.path.insert(0,'tools/cloud'); import score
from pathlib import Path
g=Path('cand/g_ee'); fn='explosion_effect'
with tempfile.TemporaryDirectory() as t:
    obj=Path(t)/'o.o'; score.compile_group(g,obj)
    words=score.text_words(obj); sy=score.symbols(obj); st=sy[fn]; en=min((o for o in sy.values() if o>st),default=len(words)*4)
    res,masks,unres,unver,err=score.relocate(obj,words,st,en,score.image_symbols()); got=res[st//4:en//4]
want=score.targets()[fn]
while got and got[-1]==0 and len(got)>len(want): got.pop()
def dis(ws):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack('>%dI'%len(ws),*ws)); f.flush()
        out=subprocess.run(['mips-linux-gnu-objdump','-D','-z','-b','binary','-m','mips:4300','-EB',f.name],capture_output=True,text=True).stdout
    return [re.sub(r'\s+',' ',m.group(3)) for m in (re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)',l) for l in out.splitlines()) if m]
dw,dg=dis(want),dis(got); sm=difflib.SequenceMatcher(None,want,got,autojunk=False); n=0
for tag,i1,i2,j1,j2 in sm.get_opcodes():
    if tag=='equal': continue
    for k in range(max(i2-i1,j2-j1)):
        n+=1; a=dw[i1+k] if i1+k<i2 else ''; b=dg[j1+k] if j1+k<j2 else ''
        print('| +%03x  %-30s %s'%((i1+k)*4 if i1+k<i2 else 0,a,b))
print('rows',n,'want',len(want),'got',len(got))
PY"
