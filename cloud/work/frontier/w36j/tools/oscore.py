#!/usr/bin/env python3
"""oscore.py OBJ FN IMG(a|b) [-v]: score an existing .o against the overlay target (Pi-side, no IDO); -v prints aligned diff."""
import sys, struct, subprocess, tempfile, difflib, re
from pathlib import Path
REPO = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(REPO / 'tools/cloud'))
import score
obj, fn, img = sys.argv[1:4]
score.ASM_DIR = (REPO / 'asm/us' / f'ovl_{img}').resolve()
want = score.targets()[fn]
words = score.text_words(Path(obj)); fns = score.symbols(Path(obj))
start = fns[fn]; end = min((o for o in fns.values() if o > start), default=len(words) * 4)
res, masks, unres, unver, errs = score.relocate(Path(obj), words, start, end, score.image_symbols())
got = res[start // 4:end // 4]
wm = [w & masks.get(start + 4 * i, 0xFFFFFFFF) for i, w in enumerate(want)]
gm = [g & masks.get(start + 4 * i, 0xFFFFFFFF) for i, g in enumerate(got)]
sm = difflib.SequenceMatcher(None, wm, gm, autojunk=False)
ne = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != 'equal')
strict = sum(1 for i in range(max(len(gm), len(wm))) if i >= len(gm) or i >= len(wm) or gm[i] != wm[i])
print(f'{fn}: want {len(want)} got {len(got)} words; strict {strict}; non-equal-run words {ne}')
if '-v' in sys.argv:
    def dis(ws):
        with tempfile.NamedTemporaryFile(suffix='.bin') as f:
            f.write(struct.pack(f'>{len(ws)}I', *ws)); f.flush()
            out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-z', '-b', 'binary', '-m', 'mips:4300', '-EB', '-M', 'gpr-names=32', f.name], capture_output=True, text=True).stdout
        return [m.group(3) for m in (re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', l) for l in out.splitlines()) if m]
    wd, gd = dis(want), dis(got)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal': continue
        print(f'--- want[{i1}:{i2}] (+0x{i1*4:x}) got[{j1}:{j2}] {tag}')
        for k in range(max(i2 - i1, j2 - j1)):
            a = wd[i1 + k] if i1 + k < i2 else ''; b = gd[j1 + k] if j1 + k < j2 else ''
            print(f'   {a:48s} | {b}')
