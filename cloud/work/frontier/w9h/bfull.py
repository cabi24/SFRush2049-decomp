#!/usr/bin/env python3
"""bfull.py DIR FN --keep a,b [--flags F]: (builder, agent copy) compile each DIR/*.c as a one-file -O3 group,
print: differing aligned rows (disassembly text compare, relocation-insensitive for lui/lo of own rodata ignored),
got/want words, strict differing words. Sorted by rows."""
import sys, argparse, tempfile, difflib, json, struct, subprocess, re
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor
sys.path.insert(0, 'tools/cloud')
import score

def dis(words):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack('>%dI' % len(words), *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-z', '-b', 'binary', '-m', 'mips:4300', '-EB', f.name],
                             capture_output=True, text=True).stdout
    r = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m: r.append(re.sub(r'\s+', ' ', m.group(3)))
    return r

def one(src, a, want, dw, syms):
    try:
        with tempfile.TemporaryDirectory() as t:
            obj = Path(t) / 'o.o'
            g = Path(t) / 'g'; g.mkdir()
            (g / 'group.c').write_text(src.read_text())
            (g / 'group.json').write_text(json.dumps({'files': ['group.c'], 'keep': a.keep.split(','), 'flags': a.flags}))
            try:
                score.compile_group(g, obj)
            except SystemExit:
                return (99999, src.name + ' COMPILE FAIL')
            words = score.text_words(obj); fns = score.symbols(obj)
            start = fns[a.fn]
            end = min((o for o in fns.values() if o > start), default=len(words) * 4)
            res, masks, unresolved, unverified, errors = score.relocate(obj, words, start, end, syms)
            got = res[start // 4:end // 4]
        while got and got[-1] == 0 and len(got) > len(want): got.pop()
        def nw(w):
            op = w >> 26
            if op == 0x0F and ((w >> 16) & 31) == 1: return w & 0xFFFF0000
            if op == 0x31 and ((w >> 21) & 31) == 1: return w & 0xFFFF0000
            return w
        sm = difflib.SequenceMatcher(None, [nw(w) for w in want], [nw(w) for w in got], autojunk=False)
        rows = 0
        for tag, i1, i2, j1, j2 in sm.get_opcodes():
            if tag != 'equal': rows += max(i2 - i1, j2 - j1)
        strict = sum(1 for i in range(max(len(got), len(want))) if i >= len(got) or i >= len(want) or got[i] != want[i])
        miss = len(want) - sum(b.size for b in sm.get_matching_blocks())
        return (miss, '%-28s miss %4d  rows %4d  got %4d  strict %4d' % (src.name, miss, rows, len(got), strict))
    except Exception as e:
        return (99998, src.name + ' ERR ' + repr(e)[:100])

def main():
  ap = argparse.ArgumentParser(); ap.add_argument('dir'); ap.add_argument('fn')
  ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared'); ap.add_argument('--keep', required=True); ap.add_argument('--top', type=int, default=60)
  a = ap.parse_args()
  want = score.targets()[a.fn]; dw = dis(want); syms = score.image_symbols()
  files = sorted(Path(a.dir).glob('*.c'))
  with ThreadPoolExecutor(2) as ex:
    res = list(ex.map(lambda f: one(f, a, want, dw, syms), files))
  res.sort(key=lambda r: r[0])
  for r in res[:a.top]: print(r[1])

if __name__ == '__main__': main()