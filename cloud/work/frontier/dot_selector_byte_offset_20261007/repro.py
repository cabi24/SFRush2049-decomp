#!/usr/bin/env python3
"""Compare two address/subtraction forms in the same authentic caller context."""
from pathlib import Path
import json
import sys
ROOT=Path(__file__).resolve().parents[4]
HERE=Path(__file__).resolve().parent
BUILD=ROOT/'build/dot_selector_byte_offset_20261007'
sys.path.insert(0,str(ROOT))
from tools.cloud import score
base=BUILD/'baseline';base.mkdir(parents=True,exist_ok=True)
spec=json.loads((HERE/'group.json').read_text())
for n in spec['files']:(base/n).write_text((HERE/n).read_text())
(base/'group.json').write_text(json.dumps(spec,indent=2)+'\n')
source=(base/'selector.c').read_text()
source=source.replace('*(s32 *)((unsigned char *)D_80113E8C + D_80151AD0 * 96)', 'D_80113E8C[D_80151AD0 * 24]')
source=source.replace('height = (s32)((u32)height - 24U);\n    D_8014A10A = height / 16;', 'D_8014A10A = (s32)((u32)height - 24U) / 16;')
(base/'selector.c').write_text(source)
score.compile_group(base,BUILD/'baseline.o')
def report(obj,name,label):
    r=score.compare(obj,name,show=0)
    data,secs=score._elf(obj)
    symbol=next(s for i,sec in enumerate(secs) if sec['type']==2 for s in score._symbol_table(data,secs,i) if s['name']==name)
    words=score.text_words(obj)[symbol['value']//4:(symbol['value']+symbol['size'])//4]
    frame=next((65536-(w&65535) for w in words[:40] if w>>16==0x27BD),0)
    print(f'{label} {name}: {r.differing}/{r.total} differing; extent={symbol["size"]} bytes; frame={frame}; extra={r.extra_words}; unresolved={len(r.unresolved)}; unverified={len(r.unverified)}; errors={len(r.errors)}')
    if r.errors:print('context errors:',r.errors)
report(BUILD/'baseline.o','func_800D9058','PR289 baseline')
spec=score.compile_group(HERE,BUILD/'candidate.o')
for name in spec['members']+spec['context']:report(BUILD/'candidate.o',name,'byte-offset research')
