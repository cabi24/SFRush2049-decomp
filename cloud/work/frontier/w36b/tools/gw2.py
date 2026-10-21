#!/usr/bin/env python3
"""builder side: gw2.py SRC NAME FLAGS TARGETS -> JSON with rodata bytes + reloc list for NAME."""
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
    ro = next((s for s in secs if s['name'] == '.rodata'), None)
    rodata = data[ro['off']:ro['off'] + ro['size']].hex() if ro else ''
    text = score._text_index(secs)
    relocs = []
    for sec in secs:
        if sec['type'] != 9 or sec['info'] != text: continue
        syms = score._symbol_table(data, secs, sec['link'])
        for k in range(sec['size'] // 8):
            off, info = struct.unpack_from('>II', data, sec['off'] + 8 * k)
            if not start <= off < end: continue
            idx, rt = info >> 8, info & 0xFF
            s = syms[idx]
            relocs.append(dict(w=(off - start) // 4, t=rt, sym=s['name'], sec=s['section'], type=s['type']))
    print(json.dumps(dict(want=want, got=words[start // 4:end // 4], rodata=rodata, relocs=relocs)))
