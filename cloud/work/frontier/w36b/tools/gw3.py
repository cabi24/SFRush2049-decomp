#!/usr/bin/env python3
"""builder side: gw3.py SRC NAME FLAGS TARGETS -> JSON: want/got words, start, all section bytes, text relocs of NAME, rodata relocs."""
import json, struct, sys, tempfile
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
src, name, flags, tg = sys.argv[1:5]
score.ASM_DIR = Path(tg).resolve()
with tempfile.TemporaryDirectory() as tmp:
    obj = Path(tmp) / 'o.o'
    if Path(src).is_dir(): score.compile_group(Path(src), obj)
    else: score.compile_single(src, flags, obj)
    want = score.targets()[name]
    words = score.text_words(obj)
    fns = score.symbols(obj)
    start = fns[name]
    end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    data, secs = score._elf(obj)
    secbytes = {s['name']: data[s['off']:s['off'] + s['size']].hex() for s in secs if s['name'] in ('.data', '.rodata', '.sdata', '.bss')}
    text = score._text_index(secs)
    idx_name = {i: s['name'] for i, s in enumerate(secs)}
    relocs = []; rorel = []
    for sec in secs:
        if sec['type'] != 9: continue
        syms = score._symbol_table(data, secs, sec['link'])
        target = idx_name.get(sec['info'])
        for k in range(sec['size'] // 8):
            off, info = struct.unpack_from('>II', data, sec['off'] + 8 * k)
            ix, rt = info >> 8, info & 0xFF
            s = syms[ix]
            ent = dict(off=off, t=rt, sym=s['name'], secname=idx_name.get(s['section']), value=s.get('value', 0))
            if target == '.text' and start <= off < end:
                ent['w'] = (off - start) // 4; relocs.append(ent)
            elif target == '.rodata':
                rorel.append(ent)
    print(json.dumps(dict(want=want, got=words[start // 4:end // 4], start=start, fns=fns, secs=secbytes, relocs=relocs, rorel=rorel)))
