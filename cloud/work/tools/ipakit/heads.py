#!/usr/bin/env python3
"""heads.py: function-head audit over the retail image.

    python3 cloud/work/tools/ipakit/heads.py [--json] [--md cloud/work/unregistered-heads.md]

Finds function heads that live inside the opaque (un-sectioned) runs of the game image:
  * sweep: a run start or the word after a `jr ra` delay slot that has an `addiu sp,sp,-N`
    prologue within 3 instructions, extent found by scan_extent();
  * call evidence: `jal` targets landing in opaque runs (alternate entries head+4 are
    recognised when the caller's delay slot carries the head's first word);
  * pointer evidence: raw words in data/descriptor tables that equal a head address;
and reports which are missing from asm/us/blob/symbols.json.  With --md, it is checked against
the heads table of cloud/work/unregistered-heads.md.
"""
import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from ipakit import ROOT, IMAGE_BASE, load_corpus, Corpus  # noqa: E402
from ipakit import mipsdec  # noqa: E402

MAXWORDS = 4000


def scan_extent(corpus, addr, limit=MAXWORDS):
    """Words in the function starting at addr: walk forward tracking the furthest forward branch /
    jump / switch-table target; the function ends after a `jr ra` delay slot once no target lies
    beyond it.  Returns the word count (>= 2, or 0 when nothing sane is found)."""
    n_img = len(corpus.image)
    o0 = (addr - IMAGE_BASE) >> 2
    hi_addr = min(IMAGE_BASE + 4 * n_img, addr + 4 * limit)
    insns = []
    maxt = addr
    i = 0
    while o0 + i < n_img and i < limit:
        ins = mipsdec.decode(corpus.image[o0 + i], addr + 4 * i)
        insns.append(ins)
        if ins.mnem.startswith('?'):
            return 0
        if ins.target is not None and ins.kind in ('branch', 'b', 'j') and ins.target > ins.pc:
            if ins.target < hi_addr:
                maxt = max(maxt, ins.target)
        if ins.kind == 'jr':
            tb = mipsdec.jump_table(insns, i, corpus.word_at, addr, hi_addr)
            if tb:
                maxt = max(maxt, max(tb))
        if ins.kind == 'ret':
            slot_pc = ins.pc + 4
            if maxt < slot_pc + 4:
                return i + 2
        i += 1
    return 0


def _is_ret(w):
    return w == 0x03E00008


def has_prologue(corpus, addr, within=3):
    for k in range(within):
        w = corpus.word_at(addr + 4 * k)
        if w is None:
            break
        if w >> 16 == 0x27BD and w & 0x8000:
            return -(((w & 0xFFFF) ^ 0x8000) - 0x8000)
        if mipsdec.decode(w, addr + 4 * k).delay:       # do not look past a branch / jr ra
            break
    return 0


def opaque_runs(corpus):
    """[(start, end)] byte ranges of the image covered by no known function."""
    end_img = IMAGE_BASE + 4 * len(corpus.image)
    runs, cur = [], IMAGE_BASE
    for s in corpus.starts:
        f = corpus.by_addr[s]
        if f.discovered:
            continue
        if s > cur:
            runs.append((cur, s))
        cur = max(cur, f.end)
    if cur < end_img:
        runs.append((cur, end_img))
    return runs


def discover_call_heads(corpus):
    """Register jal targets that land in opaque runs as discovered heads; repeat over the new ones."""
    work = list(corpus.funcs.values())
    seen = set()
    while work:
        f = work.pop()
        if id(f) in seen:
            continue
        seen.add(id(f))
        for k, w in enumerate(f.words):
            if w >> 26 != 3:
                continue
            t = 0x80000000 | ((w & 0x3FFFFFF) << 2)
            kind, tf, _ = corpus.resolve(t)
            if kind != 'opaque':
                continue
            head = t
            slot = f.words[k + 1] if k + 1 < len(f.words) else None
            if slot is not None and corpus.word_at(t - 4) == slot and not corpus.resolve(t - 4)[1]:
                head = t - 4            # copied first instruction: alternate entry head+4
            n = scan_extent(corpus, head)
            if n >= 2:
                nf = corpus.add_head(head, nwords=n)
                work.append(nf)


def head_row(corpus, f, callers, ptrs):
    w = f.words
    frame = has_prologue(corpus, f.addr)
    nret = sum(1 for x in w if _is_ret(x))
    return {'name': f.name, 'addr': '0x%08X' % f.addr, 'words': len(w), 'frame': frame, 'jr_ra': nret,
            'jal_callers': sorted(callers.get(f.addr, ())), 'pointer_words': ['0x%08X' % p for p in ptrs.get(f.addr, ())],
            'registered': f.addr in corpus.symbols_by_addr if hasattr(corpus, 'symbols_by_addr') else None}


def audit(corpus=None):
    corpus = corpus or load_corpus(discover=False)
    sym_addrs = set(corpus.symbols.values())
    base_funcs = list(corpus.funcs.values())
    # 1. sweep each opaque run for prologue heads.
    found = {}
    for lo, hi in opaque_runs(corpus):
        a = lo
        while a < hi:
            prev_ret = a == lo or (corpus.word_at(a - 8) == 0x03E00008)
            pr = has_prologue(corpus, a) if prev_ret else 0
            if pr:
                n = scan_extent(corpus, a)
                if n >= 2 and a + 4 * n <= hi:
                    found[a] = {'via': ['sweep'], 'words': n}
                    a += 4 * n
                    continue
            a += 4
    # 1b. frameless heads: a pointer word in a data/descriptor table that targets the word after a
    # `jr ra` delay slot inside an opaque run (no `addiu sp` prologue to sweep for).
    for i, v in enumerate(corpus.image):
        pa = IMAGE_BASE + 4 * i
        if v & 3 or v in found or corpus.resolve(v)[0] != 'opaque' or corpus.containing(pa) is not None:
            continue
        if corpus.word_at(v - 8) != 0x03E00008 or any(a <= v < a + 4 * d['words'] for a, d in found.items()):
            continue
        n = scan_extent(corpus, v)
        if n >= 2:
            found[v] = {'via': ['pointer-head'], 'words': n}
    # 2. call evidence (over known + swept bodies, iterated).
    work = [(f.addr, f.words) for f in base_funcs]
    work += [(a, corpus.words_at(a, d['words'])) for a, d in found.items()]
    done, alt_entries, callers = set(), [], {}
    while work:
        addr, words = work.pop()
        if addr in done:
            continue
        done.add(addr)
        for k, w in enumerate(words):
            if w >> 26 != 3:
                continue
            t = 0x80000000 | ((w & 0x3FFFFFF) << 2)
            pc = addr + 4 * k
            head, is_alt = t, False
            kind, tf, _ = corpus.resolve(t)
            if kind == 'alt':
                alt_entries.append({'head': tf.name, 'caller_pc': '0x%08X' % pc,
                                    'verified_slot': (k + 1 < len(words) and words[k + 1] == tf.words[0])})
                continue
            if kind == 'opaque':
                slot = words[k + 1] if k + 1 < len(words) else None
                if t in found:
                    head = t
                elif (t - 4) in found and slot == corpus.word_at(t - 4):
                    head, is_alt = t - 4, True
                elif slot is not None and corpus.word_at(t - 4) == slot and corpus.resolve(t - 4)[0] == 'opaque' \
                        and scan_extent(corpus, t - 4) >= 2 and (corpus.word_at(t - 12) == 0x03E00008 or has_prologue(corpus, t - 4)):
                    head, is_alt = t - 4, True
                else:
                    head = t
                if head not in found:
                    n = scan_extent(corpus, head)
                    if n < 2:
                        continue
                    found[head] = {'via': [], 'words': n}
                    work.append((head, corpus.words_at(head, n)))
                found[head]['via'].append('jal')
                callers.setdefault(head, set()).add('0x%08X' % pc)
                if is_alt:
                    alt_entries.append({'head': func_name_of(head), 'caller_pc': '0x%08X' % pc, 'verified_slot': True,
                                        'unregistered_head': True})
    # 3. pointer evidence from every non-function word of the image.
    ptrs = {}
    covered = [(f.addr, f.end) for f in base_funcs]
    cov_starts = {a for a, _ in covered}
    lo_img = IMAGE_BASE
    heads_set = set(found)
    for i, v in enumerate(corpus.image):
        if v in heads_set or v in corpus.by_addr:
            pa = IMAGE_BASE + 4 * i
            if corpus.containing(pa) is None:          # a data/descriptor word, not an instruction
                ptrs.setdefault(v, []).append(pa)
    rows = []
    for a in sorted(found):
        d = found[a]
        if a in ptrs:
            d['via'].append('pointer')
        w = corpus.words_at(a, d['words'])
        rows.append({'name': func_name_of(a), 'addr': '0x%08X' % a, 'words': d['words'], 'frame': has_prologue(corpus, a),
                     'jr_ra': sum(1 for x in w if _is_ret(x)), 'evidence': sorted(set(d['via'])),
                     'jal_callers': sorted(callers.get(a, ())), 'pointer_words': ['0x%08X' % p for p in ptrs.get(a, ())],
                     'registered': a in sym_addrs,
                     'contains_registered': sorted(f.name for f in base_funcs if a < f.addr < a + 4 * d['words'])})
    return {'heads': rows, 'alt_entries': alt_entries,
            'opaque_runs': [{'start': '0x%08X' % lo, 'end': '0x%08X' % hi, 'words': (hi - lo) // 4} for lo, hi in opaque_runs(corpus)]}


def func_name_of(a):
    return 'func_%08X' % a


def parse_md_heads(path):
    rows = {}
    for line in Path(path).read_text().splitlines():
        m = re.match(r'\|\s*(func_[0-9A-Fa-f]+)\s*\|\s*0x([0-9A-Fa-f]+)\s*\|\s*(\d+)\s*\|', line)
        if m:
            rows[int(m.group(2), 16)] = int(m.group(3))
    return rows


def compare_md(rep, md):
    want = parse_md_heads(md)
    got = {int(r['addr'], 16): r for r in rep['heads']}
    agree = [a for a in want if a in got and got[a]['words'] == want[a]]
    wrong = [(a, want[a], got[a]['words']) for a in want if a in got and got[a]['words'] != want[a]]
    miss = [a for a in want if a not in got]
    return {'md_heads': len(want), 'agree': len(agree), 'size_mismatch': wrong, 'missing_from_audit': miss,
            'extra_in_audit': sorted(a for a in got if a not in want)}


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--json', action='store_true')
    ap.add_argument('--md', default=str(ROOT / 'cloud/work/unregistered-heads.md'))
    a = ap.parse_args(argv)
    rep = audit()
    cmp_ = compare_md(rep, a.md) if a.md and Path(a.md).exists() else None
    if a.json:
        print(json.dumps({'report': rep, 'vs_md': cmp_}, indent=1))
        return 0
    print('opaque runs: %d (%d words)' % (len(rep['opaque_runs']), sum(r['words'] for r in rep['opaque_runs'])))
    print('heads found in opaque runs: %d (unregistered: %d); alternate entries: %d' % (
        len(rep['heads']), sum(1 for r in rep['heads'] if not r['registered']), len(rep['alt_entries'])))
    for r in rep['heads']:
        print('  %s %5dw frame %-4d ret %d %s%s' % (r['addr'], r['words'], r['frame'], r['jr_ra'], ','.join(r['evidence']),
                                              '' if r['registered'] else '  UNREGISTERED'))
    if cmp_:
        print('vs %s: %s' % (a.md, json.dumps(cmp_)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
