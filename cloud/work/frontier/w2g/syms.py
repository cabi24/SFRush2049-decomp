#!/usr/bin/env python3
"""syms.py SRC.c [flags]: (builder) compile single and print function offsets in the object."""
import sys, tempfile
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
fl = sys.argv[2] if len(sys.argv) > 2 else '-g0 -O3 -mips2 -G 0 -non_shared'
with tempfile.TemporaryDirectory() as t:
    obj = Path(t) / 'o.o'
    score.compile_single(sys.argv[1], fl, obj)
    for n, o in sorted(score.symbols(obj).items(), key=lambda x: x[1]): print('%06x %s' % (o, n))
    print('%06x END' % (len(score.text_words(obj)) * 4))
