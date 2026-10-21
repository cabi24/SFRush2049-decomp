#!/usr/bin/env python3
"""On builder: gotwords.py (group DIR | fn FILE.c NAME FLAGS) NAME OUT.json [--targets DIR]
dump relocation-resolved words of NAME as compiled."""
import sys, json
sys.path.insert(0, 'tools/cloud')
import score
from pathlib import Path
import tempfile, os
mode = sys.argv[1]
score.ASM_DIR = Path(os.environ.get('TGT','asm/us/ovl_a')).resolve()
obj = Path(tempfile.mkdtemp()) / 'o.o'
if mode == 'group':
    score.compile_group(Path(sys.argv[2]), obj); name = sys.argv[3]; out = sys.argv[4]
else:
    score.compile_single(sys.argv[2], sys.argv[4], obj); name = sys.argv[3]; out = sys.argv[5]
words = score.text_words(obj); fn = score.symbols(obj)
start = fn[name]
end = min((o for o in fn.values() if o > start), default=len(words)*4)
want = score.targets()[name]
resolved, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, score.image_symbols())
m = {str((o - start)//4): v for o, v in masks.items() if start <= o < end}
json.dump({'got': resolved[start//4:end//4], 'want': want, 'masks': m}, open(out, 'w'))
