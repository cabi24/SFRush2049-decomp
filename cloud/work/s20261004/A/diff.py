#!/usr/bin/env python3
"""diff.py GROUP_DIR FUNC [FUNC..] : build group (score.compile_group) and print side-by-side want/got disasm."""
import sys, struct, subprocess, tempfile, json
from pathlib import Path
ROOT = Path(__file__).resolve().parents[1] / 'repo'
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack('>%dI' % len(words), *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump','-D','-b','binary','-m','mips:4300','-EB','-M','gpr-names=32,reg-names=numeric' if 0 else 'gpr-names=32', f.name], capture_output=True, text=True).stdout
    r=[]
    for l in out.splitlines():
        p=l.split('\t')
        if len(p)>=3 and p[0].strip().endswith(':'): r.append(' '.join(p[2:]))
    return r
g=Path(sys.argv[1])
with tempfile.TemporaryDirectory() as t:
    o=Path(t)/'o.o'
    if g.suffix=='.c': score.compile_single(g, sys.argv[2] if sys.argv[2].startswith('-') else score.DEFAULT_FLAGS, o)
    else: score.compile_group(g,o)
    fns=score.symbols(o); words=score.text_words(o)
    tg=score.targets()
    for n in [a for a in sys.argv[2:] if not a.startswith('-')]:
        st=fns[n]; end=min([x for x in fns.values() if x>st] or [len(words)*4])
        got=dis(words[st//4:end//4]); want=dis(tg[n])
        print('==',n,len(want),len(got))
        for i in range(max(len(got),len(want))):
            a=want[i] if i<len(want) else ''; b=got[i] if i<len(got) else ''
            print('%3x %s %-34s %s'%(i*4,' ' if a==b else '*',a,b))
