#!/usr/bin/env python3
"""builder side: gw.py SRC NAME FLAGS TARGETS -> JSON {want,got,masks} on stdout"""
import json, sys, tempfile
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
    res, masks, unres, unv, errs = score.relocate(obj, words, start, end, score.image_symbols())
    got = res[start // 4:end // 4]
    m = {str((k - start) // 4): v for k, v in masks.items() if start <= k < end}
    print(json.dumps(dict(want=want, got=got, masks=m, unres=unres, errs=errs, start=start, end=end)))
