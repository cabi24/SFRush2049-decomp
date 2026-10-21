#!/usr/bin/env python3
"""objdiff.py OBJ NAME [--norm]: aligned diff of NAME in an existing object vs the image-A retail words (runs on the Pi)."""
import sys, json, subprocess
from pathlib import Path
ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score
score.ASM_DIR = ROOT / 'asm/us/ovl_a'
obj = Path(sys.argv[1]); name = sys.argv[2]
words = score.text_words(obj); fn = score.symbols(obj)
start = fn[name]
end = min((o for o in fn.values() if o > start), default=len(words)*4)
want = score.targets()[name]
resolved, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, score.image_symbols())
m = {str((o - start)//4): v for o, v in masks.items() if start <= o < end}
out = ROOT / 'build/trace/w36c/_objdiff.json'
json.dump({'got': resolved[start//4:end//4], 'want': want, 'masks': m}, open(out, 'w'))
gw = resolved[start//4:end//4]
bad = sum(1 for i, w in enumerate(want) if i >= len(gw) or ((gw[i] ^ w) & m.get(str(i), 0xFFFFFFFF)))
print('STRICT', bad)
args = sys.argv[3] if len(sys.argv) > 3 else ''
subprocess.run([sys.executable, str(Path(__file__).parent / 'sbsdiff.py'), str(out), name, args])
