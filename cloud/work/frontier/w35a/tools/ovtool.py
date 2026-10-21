#!/usr/bin/env python3
"""ovtool.py (run in the builder scratch root) -- compile an overlay candidate/group, score it, and dump
aligned disassemblies of ours vs target.

  ovtool.py fn   FILE NAME... [--flags F] [--targets asm/us/ovl_a] [--out DIR]
  ovtool.py group DIR NAME... [--targets asm/us/ovl_a] [--out DIR]   (NAME... default: members+context)
Writes DIR/NAME.ours.txt, NAME.tgt.txt, NAME.diff; prints the scorer summary line per name.
"""
import argparse, difflib, json, os, re, shlex, struct, subprocess, sys, tempfile
from pathlib import Path
sys.path.insert(0, str(Path.cwd() / 'tools' / 'cloud'))
import score

def objdump(words, va):
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB',
                              f'--adjust-vma=0x{va:x}', '-M', 'gpr-names=32', f.name],
                             capture_output=True, text=True).stdout
    res = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if m: res.append((int(m.group(1), 16), int(m.group(2), 16), re.sub(r'\s+', ' ', m.group(3))))
    return res

def listing(words, va, syms, rev, masks=None):
    if masks:
        words = [(w & masks.get(va + 4*i, 0xFFFFFFFF)) for i, w in enumerate(words)]
    def nm(a):
        if a in rev: return rev[a][0]
        return None
    luis = {}
    lines = []
    ins_list = objdump(words, va)
    for addr, w, ins in ins_list:
        off = addr - va
        note = ''
        j = re.match(r'jal (0x)?([0-9a-f]+)', ins)
        if j:
            t = int(j.group(2), 16); note = ' <' + (nm(t) or f'0x{t:x}') + '>'
            ins = 'jal'
        # normalise branch targets to relative offsets
        b = re.match(r'(b\w*|beq\w*|bne\w*|bgez\w*|bltz\w*|bgtz\w*|blez\w*|bc1\w*) (.*)0x([0-9a-f]+)$', ins)
        if b:
            t = int(b.group(3), 16)
            ins = f'{b.group(1)} {b.group(2)}.{(t-addr)//4:+d}'
        ml = re.match(r'lui (\w+),(0x[0-9a-f]+|\d+)', ins)
        if ml: luis[ml.group(1)] = int(ml.group(2), 0) << 16
        mo = re.match(r'(addiu|lw|lh|lhu|lb|lbu|sw|sh|sb|lwc1|swc1|ori) (\w+),(-?\d+|0x[0-9a-f]+)\((\w+)\)', ins)
        if mo and mo.group(4) in luis:
            a = (luis[mo.group(4)] + int(mo.group(3), 0)) & 0xffffffff
            note = f' ; {nm(a) or hex(a)}'
        mo2 = re.match(r'addiu (\w+),(\w+),(-?\d+)', ins)
        if mo2 and mo2.group(2) in luis:
            a = (luis[mo2.group(2)] + int(mo2.group(3))) & 0xffffffff
            note = f' ; {nm(a) or hex(a)}'
        lines.append(f'{off//4:4d} {w:08x} {ins}{note}')
    return lines

def skel(l):
    t = l.split(None, 2)[2]
    t = re.sub(r'(0x[0-9a-f]+|-?\d+)(\(|$)', r'#\2', t)
    t = re.sub(r',(0x[0-9a-f]+|-?\d+)$', ',#', t)
    return t

def pair(tgt, ours, ours_masked_idx):
    """Return (tgt_lines, ours_lines) as plain text; target words aligned to masked ours words are rewritten to the
    ours text so relocations do not show as diffs."""
    a = [skel(l) for l in tgt]; b = [skel(l) for l in ours]
    sm = difflib.SequenceMatcher(None, a, b, autojunk=False)
    t2 = [l.split(None, 2)[2] for l in tgt]; o2 = [l.split(None, 2)[2] for l in ours]
    for blk in sm.get_matching_blocks():
        for k in range(blk.size):
            if blk.b + k in ours_masked_idx:
                t2[blk.a + k] = o2[blk.b + k]
    return t2, o2

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('mode'); ap.add_argument('src'); ap.add_argument('names', nargs='*')
    ap.add_argument('--flags', default='-g0 -O3 -mips2 -G 0 -non_shared')
    ap.add_argument('--targets', default='asm/us/ovl_a')
    ap.add_argument('--out', default=None)
    a = ap.parse_args()
    score.ASM_DIR = Path(a.targets).resolve()
    out = Path(a.out or (a.src + '.out' if a.mode == 'group' else str(Path(a.src).with_suffix('.out')))); out.mkdir(exist_ok=True)
    obj = out / 'out.o'
    names = list(a.names)
    if a.mode == 'fn':
        score.compile_single(a.src, a.flags, obj)
        context = []
    else:
        spec = score.compile_group(Path(a.src), obj)
        if not names: names = list(spec['members']) + list(spec.get('context', []))
    syms = score.image_symbols()
    rev = {}
    for k, v in syms.items(): rev.setdefault(v, []).append(k)
    # also include blob symbols for game callees
    try:
        bs = json.load(open('asm/us/blob/symbols.json'))['symbols']
        for k, v in bs.items():
            v = int(v, 16)
            if v not in rev: rev[v] = [k]
    except Exception: pass
    words = score.text_words(obj)
    fns = score.symbols(obj)
    for name in names:
        try:
            r = score.compare(obj, name, show=0)
        except SystemExit as e:
            print(f'{name}: NOT SCORED ({e})'); continue
        print(f'{name}: {r.summary()}')
        want = score.targets()[name]
        start = fns[name]
        end = min((o for o in fns.values() if o > start), default=len(words) * 4)
        ow = words[start // 4:end // 4]
        resolved, masks, *_ = score.relocate(obj, words, start, end, syms)
        ow = resolved[start // 4:end // 4]
        # mask relocated own-section words so they are shown as raw
        va = syms[name]
        mk = {k: v for k, v in masks.items() if start <= k < end}
        ours = listing(ow, va, syms, rev, {va + (k - start): v for k, v in mk.items()})
        tgt = listing(want, va, syms, rev)
        (out / f'{name}.ours.txt').write_text('\n'.join(ours) + '\n')
        (out / f'{name}.tgt.txt').write_text('\n'.join(tgt) + '\n')
        mi = {(k - start) // 4 for k in mk}
        t2, o2 = pair(tgt, ours, mi)
        d = difflib.unified_diff(t2, o2, 'tgt', 'ours', lineterm='', n=2)
        (out / f'{name}.diff').write_text('\n'.join(d) + '\n')
        print(f'   words ours {len(ow)} target {len(want)}')

if __name__ == '__main__':
    main()
