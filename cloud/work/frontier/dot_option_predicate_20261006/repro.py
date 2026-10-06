#!/usr/bin/env python3
"""Canonical real-caller comparison; the historical stand-ins are not used."""
from pathlib import Path
import json
import sys
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
BUILD=ROOT/'build/dot_option_predicate_20261006'
sys.path.insert(0,str(ROOT))
from tools.cloud import score
base=BUILD/'baseline';base.mkdir(parents=True,exist_ok=True)
(base/'predicate.c').write_text((HERE/'predicate.c').read_text())
(base/'group.json').write_text(json.dumps(dict(files=['predicate.c'],members=['func_800D8078'],keep=['func_800D8078'],claims=[],context=[],flags='-g0 -O3 -mips2 -G 0 -non_shared'),indent=2)+'\n')
score.compile_group(base,BUILD/'baseline.o')
print('honest kept baseline:',score.compare(BUILD/'baseline.o','func_800D8078',show=0).summary())
spec=score.compile_group(HERE,BUILD/'candidate.o')
obj=BUILD/'candidate.o';data,secs=score._elf(obj)
for name in spec['members']+spec['context']:
    r=score.compare(obj,name,show=0)
    symbol=next(s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==name)
    print(f'{name}: {r.differing}/{r.total} differing; extent={symbol["size"]} bytes; extra={r.extra_words}; unresolved={len(r.unresolved)}; unverified={len(r.unverified)}; errors={len(r.errors)}')
    if r.errors:print('context errors:',r.errors)
