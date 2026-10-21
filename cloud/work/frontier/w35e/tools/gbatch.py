#!/usr/bin/env python3
"""gbatch.py (builder-side, run from lane scratch root) BASEGROUP_DIR VARIANT_DIR FN IMG [--jobs N] [--top N]
BASEGROUP_DIR holds group.c/group.json; the text of group.c from `ROOT_MARKER` on is replaced by each VARIANT_DIR/*.c.
Prints strict-differing words, frame (addiu sp) and words for the best variants."""
import sys, tempfile, json, shutil, argparse, difflib, re
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, 'tools/cloud')
import score
ap = argparse.ArgumentParser(); ap.add_argument('base'); ap.add_argument('vdir'); ap.add_argument('fn'); ap.add_argument('img')
ap.add_argument('--jobs', type=int, default=2); ap.add_argument('--top', type=int, default=25); ap.add_argument('--marker', default='s32 func_803AD57C(void *callback_context)')
a = ap.parse_args()
score.ASM_DIR = Path(f'asm/us/ovl_{a.img}').resolve()
want = score.targets()[a.fn]; syms = score.image_symbols()
base = Path(a.base); src = (base / 'group.c').read_text(); j = src.index(a.marker); head = src[:j]
spec = json.loads((base / 'group.json').read_text())
def one(f):
    try:
        with tempfile.TemporaryDirectory() as t:
            g = Path(t) / 'g'; g.mkdir()
            (g / 'group.c').write_text(head + f.read_text()); (g / 'group.json').write_text(json.dumps(spec))
            obj = Path(t) / 'o.o'
            try: score.compile_group(g, obj)
            except SystemExit as e: return (9999, 0, f.name + ' COMPILE FAIL ' + str(e)[:120].replace('\n', ' '))
            words = score.text_words(obj); fns = score.symbols(obj)
            start = fns[a.fn]
            end = min((o for o in fns.values() if o > start), default=len(words) * 4)
            res, masks, unres, unver, errs = score.relocate(obj, words, start, end, syms)
            got = res[start // 4:end // 4]
            gm = [x & masks.get(start + 4 * i, 0xFFFFFFFF) for i, x in enumerate(got)]
            wm = [x & masks.get(start + 4 * i, 0xFFFFFFFF) for i, x in enumerate(want)]
            strict = sum(1 for i in range(max(len(gm), len(wm))) if i >= len(gm) or i >= len(wm) or gm[i] != wm[i])
            sm = difflib.SequenceMatcher(None, wm, gm, autojunk=False)
            ne = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != 'equal')
            frame = (got[0] & 0xFFFF) ^ 0xFFFF
            return (ne, strict, f.name + f' words={len(got)} frame={0x10000 - (got[0] & 0xFFFF) if got[0] >> 16 == 0x27bd else "?"}')
    except Exception as e:
        return (9998, 0, f.name + ' ERR ' + repr(e)[:100])
files = sorted(Path(a.vdir).glob('*.c'))
with ThreadPoolExecutor(a.jobs) as ex: res = list(ex.map(one, files))
res.sort()
for r in res[:a.top]: print('nonequal %4d  strict %4d  %s' % r)
print(len(res), 'variants')
