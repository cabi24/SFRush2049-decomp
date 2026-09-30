#!/usr/bin/env python3
"""near.py SRC.c FN [--flags "..."]: instruction-aligned closeness of compiled FN vs retail.
Reports: size, strict word diffs, aligned exact-word matches (LCS via difflib),
aligned opcode-shape matches (registers/immediates ignored). Reloc words are
compared after tools/cloud/score.py relocation resolution."""
import sys, argparse, tempfile, difflib
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud'))
import score

def shape(w):
    op = w >> 26
    if op == 0: return (0, w & 0x3f)
    if op == 1: return (1, (w >> 16) & 0x1f)
    if op == 0x11: return (op, (w >> 21) & 0x1f, w & 0x3f if (w >> 21) & 0x1f >= 16 else 0)
    return (op,)

def main():
    ap = argparse.ArgumentParser(); ap.add_argument('src'); ap.add_argument('fn')
    ap.add_argument('--flags', default='-g0 -O2 -mips2 -G 0 -non_shared')
    a = ap.parse_args()
    want = score.targets()[a.fn]
    with tempfile.TemporaryDirectory() as t:
        obj = Path(t) / 'o.o'
        score.compile_single(a.src, a.flags, obj)
        words = score.text_words(obj); fns = score.symbols(obj)
        start = fns[a.fn]
        end = min((o for o in fns.values() if o > start), default=len(words) * 4)
        res, masks, *_ = score.relocate(obj, words, start, end, score.image_symbols())
        got = res[start // 4:end // 4]
    # mask reloc fields
    gm = [g & masks.get(start + 4 * i, 0xFFFFFFFF) for i, g in enumerate(got)]
    wm = [w & masks.get(start + 4 * i, 0xFFFFFFFF) for i, w in enumerate(want)]
    strict = sum(1 for i in range(min(len(gm), len(wm))) if gm[i] == wm[i])
    sm = difflib.SequenceMatcher(None, wm, gm, autojunk=False)
    ex = sum(b.size for b in sm.get_matching_blocks())
    sm2 = difflib.SequenceMatcher(None, [shape(w) for w in want], [shape(w) for w in got], autojunk=False)
    sh = sum(b.size for b in sm2.get_matching_blocks())
    n = len(want)
    print(f'{a.fn}: want {n} words, got {len(got)} ({len(got)-n:+d}); strict-equal {strict}; '
          f'aligned exact {ex} ({100*ex/n:.0f}%); aligned shape {sh} ({100*sh/n:.0f}%)  flags: {a.flags}')
main()
