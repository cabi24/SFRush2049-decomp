#!/usr/bin/env python3
"""ours.py FN [OBJ] : print our compiled FN (unit.o of $TAG by default) with offsets; --want prints retail."""
import sys, os
sys.path.insert(0, '/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace')
from pathlib import Path
import udiff
score = udiff.score
args = [a for a in sys.argv[1:] if not a.startswith('--')]
fn = args[0]
obj = Path(args[1]) if len(args) > 1 else udiff.REPO / 'build/blob_unit' / os.environ.get('TAG', 'w36s') / 'unit.o'
if '--want' in sys.argv:
    got = score.targets()[fn]
else:
    words = score.text_words(obj); fns = score.symbols(obj)
    start = fns[fn]
    end = min((o for o in fns.values() if o > start), default=len(words) * 4)
    res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, score.image_symbols())
    got = res[start // 4:end // 4]
for i, x in enumerate(udiff.dis(got)):
    print('+%03x  %s' % (i * 4, x))
