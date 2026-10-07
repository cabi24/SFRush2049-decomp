#!/usr/bin/env python3
"""Compile/score the real A118 source before and after its genuine caller."""
from pathlib import Path
import json
import sys
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
BUILD=ROOT/'build/dot_results_cleanup_20261006'
sys.path.insert(0,str(ROOT))
from tools.cloud import score
base=BUILD/'baseline';base.mkdir(parents=True,exist_ok=True)
(base/'rows.c').write_text((HERE/'rows.c').read_text())
(base/'group.json').write_text(json.dumps(dict(files=['rows.c'],members=['render_replay_ui','render_results_screen'],keep=['render_results_screen'],claims=[],context=[],flags='-g0 -O3 -mips2 -G 0 -non_shared'),indent=2)+'\n')
score.compile_group(base,BUILD/'baseline.o')
for name in ['render_replay_ui','render_results_screen']:
    print('A118 baseline',name,score.compare(BUILD/'baseline.o',name,show=0).summary())
spec=score.compile_group(HERE,BUILD/'candidate.o')
obj=BUILD/'candidate.o';data,secs=score._elf(obj)
for name in spec['members']+spec['context']:
    r=score.compare(obj,name,show=0)
    symbol=next(s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==name)
    words=score.text_words(obj)[symbol['value']//4:(symbol['value']+symbol['size'])//4]
    frame=next((65536-(w&65535) for w in words[:40] if w>>16==0x27BD),0)
    print(f'real caller {name}: {r.differing}/{r.total} differing; extent={symbol["size"]} bytes; frame={frame}; extra={r.extra_words}; unresolved={len(r.unresolved)}; unverified={len(r.unverified)}; errors={len(r.errors)}')
