#!/usr/bin/env python3
"""Compile/score the actual PR281 context with element and byte traversal."""
from pathlib import Path
import json
import sys
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
BUILD=ROOT/'build/dot_replay_row_match_20261007'
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=json.loads((HERE/'group.json').read_text())
base=BUILD/'baseline';base.mkdir(parents=True,exist_ok=True)
(base/'group.json').write_text(json.dumps(spec,indent=2)+'\n')
for n in spec['files']:(base/n).write_text((HERE/n).read_text())
rows=(base/'rows.c').read_text().replace('for(i=0;i<sizeof(D_80111998[0]);i+=sizeof(Record),record++)','for(i=0;i<17;i++,record++)')
(base/'rows.c').write_text(rows)
score.compile_group(base,BUILD/'baseline.o')
print('PR281 baseline:',score.compare(BUILD/'baseline.o','render_replay_ui',show=0).summary())
spec=score.compile_group(HERE,BUILD/'candidate.o')
obj=BUILD/'candidate.o';data,secs=score._elf(obj)
for name in spec['members']+spec['context']:
    r=score.compare(obj,name,show=0)
    symbol=next(s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==name)
    words=score.text_words(obj)[symbol['value']//4:(symbol['value']+symbol['size'])//4]
    frame=next((65536-(w&65535) for w in words[:40] if w>>16==0x27BD),0)
    print(f'{name}: {r.differing}/{r.total} differing; extent={symbol["size"]} bytes; frame={frame}; extra={r.extra_words}; unresolved={len(r.unresolved)}; unverified={len(r.unverified)}; errors={len(r.errors)}')
