#!/usr/bin/env python3
"""Canonical compile/score only; no native bytes or assembly are emitted."""
from pathlib import Path
import sys
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT))
from tools.cloud import score
out=ROOT/'build/dot_menu_geometry_20261006/group.o'
out.parent.mkdir(parents=True,exist_ok=True)
spec=score.compile_group(HERE,out)
data,secs=score._elf(out)
for name in spec['members']+spec['context']:
    result=score.compare(out,name,show=0)
    symbol=next(s for i,sec in enumerate(secs) if sec['type']==2
                for s in score._symbol_table(data,secs,i) if s['name']==name)
    words=score.text_words(out)[symbol['value']//4:(symbol['value']+symbol['size'])//4]
    frame=next((65536-(w&65535) for w in words[:40] if w>>16==0x27BD),0)
    print(f'{name}: {result.differing}/{result.total} differing; emitted={symbol["size"]//4} words; frame={frame}; extra={result.extra_words}; unresolved={len(result.unresolved)}; unverified={len(result.unverified)}; errors={len(result.errors)}')
