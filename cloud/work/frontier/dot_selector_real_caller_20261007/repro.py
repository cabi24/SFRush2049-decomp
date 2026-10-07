#!/usr/bin/env python3
"""Canonical O3 comparison with the same selector and clean font sources."""
from pathlib import Path
import json
import sys
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
BUILD=ROOT/'build/dot_selector_real_caller_20261007'
sys.path.insert(0,str(ROOT))
from tools.cloud import score
base=BUILD/'baseline';base.mkdir(parents=True,exist_ok=True)
for n in ['selector.c','font.c']:(base/n).write_text((HERE/n).read_text())
(base/'group.json').write_text(json.dumps(dict(files=['selector.c','font.c'],members=['func_800D9058'],context=[],keep=['object_create','world_trigger_check','object_byte9_set','func_80096288','func_800D9058'],claims=[],flags='-g0 -O3 -mips2 -G 0 -non_shared'),indent=2)+'\n')
score.compile_group(base,BUILD/'baseline.o')
def report(obj,name,label):
    r=score.compare(obj,name,show=0)
    data,secs=score._elf(obj)
    symbol=next(s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==name)
    words=score.text_words(obj)[symbol['value']//4:(symbol['value']+symbol['size'])//4]
    frame=next((65536-(w&65535) for w in words[:40] if w>>16==0x27BD),0)
    print(f'{label} {name}: {r.differing}/{r.total} differing; extent={symbol["size"]} bytes; frame={frame}; extra={r.extra_words}; unresolved={len(r.unresolved)}; unverified={len(r.unverified)}; errors={len(r.errors)}')
    if r.errors:print('context errors:',r.errors)
report(BUILD/'baseline.o','func_800D9058','frozen kept baseline')
spec=score.compile_group(HERE,BUILD/'candidate.o')
for name in spec['members']+spec['context']:report(BUILD/'candidate.o',name,'real caller research')
