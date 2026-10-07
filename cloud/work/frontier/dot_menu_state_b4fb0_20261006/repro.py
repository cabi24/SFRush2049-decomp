#!/usr/bin/env python3
"""Compile/score both real source versions; no acceptance or behavior gate."""
from pathlib import Path
import json
import sys
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
BUILD=ROOT/'build/dot_menu_state_b4fb0_20261006'
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=json.loads((HERE/'group.json').read_text())
group=BUILD/'group';group.mkdir(parents=True,exist_ok=True)
(group/'group.json').write_text(json.dumps(spec,indent=2)+'\n')
for name in spec['files']:(group/name).write_text((HERE/name).read_text())
def report(obj,name,label):
    r=score.compare(obj,name,show=0)
    data,secs=score._elf(obj)
    symbol=next(s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==name)
    words=score.text_words(obj)[symbol['value']//4:(symbol['value']+symbol['size'])//4]
    frame=next((65536-(w&65535) for w in words[:40] if w>>16==0x27BD),0)
    print(f'{label} {name}: {r.differing}/{r.total} differing; emitted={symbol["size"]//4} words; frame={frame}; extra={r.extra_words}; unresolved={len(r.unresolved)}; unverified={len(r.unverified)}; errors={len(r.errors)}')
(group/'candidate.c').write_text((HERE/'baseline.c').read_text())
score.compile_group(group,BUILD/'baseline.o')
report(BUILD/'baseline.o','func_800B4FB0','initial reconstruction')
(group/'candidate.c').write_text((HERE/'candidate.c').read_text())
score.compile_group(group,BUILD/'candidate.o')
for name in spec['members']+spec['context']:report(BUILD/'candidate.o',name,'research NONMATCH')
