#!/usr/bin/env python3
"""Readable pre-colouring report for one procedure from a w5d uopt run (pretrace.sh).

    prereport.py DIR PROC [--all] [--bits 80,81] [--json out.json]

DIR holds `list` (uopt -l listing at W5D_LEVEL=3), `w5d` (W5D_OUT) and optionally `cdx`
(globalcolor trace).  For every expression bit (= ichain = web number in the colouring trace):

    bit  expression text
         where it occurs  (antloc: computed in block before its operands change; avloc: still
                           available at block exit)
         PRE result       (delete: occurrence replaced by a reference to the CSE temporary;
                           insert: computation inserted at that block's exit)
         register sets    (@ iscolored 1 = live-range candidate built for it, ...)
         colouring        (p1/p2 decision, save, nocs, register)

A bit whose PRE result is empty and that is not in `@ iscolored` stays a ugen temp at each
use (retail's "recomputed every time").  Constants and addresses get bits only when
register-allocation preparation (makelivranges) decides they need a live range: see the
`regcand` table (decision 1 = live range, 0 = rematerialised at the use).
"""
import argparse, json, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import cmparse

FIELD = re.compile(r'(\w+)=(\S*)')


def fields(line):
    return dict(FIELD.findall(line))


def load(d, proc):
    text = open(os.path.join(d, 'list'), errors='replace').read()
    lines = cmparse.section(text, proc)
    table, nodes, sets = cmparse.parse(lines)
    bits, regcand, ordinal = {}, [], None
    for line in open(os.path.join(d, 'w5d'), errors='replace'):
        if 'proc=%s ' % proc not in line:
            continue
        f = fields(line)
        if line.startswith('[W5D] bit '):
            b = int(f['bit'])
            ent = bits.setdefault(b, {})
            ent[f['at']] = line.split(' text=', 1)[1].strip()
            ent['kind'] = int(f['kind'])
        elif line.startswith('[W5D] regcand '):
            regcand.append({'what': f['what'], 'bb': int(f['bb']), 'ctx': int(f['ctx']),
                            'decision': int(f['decision']), 'text': line.split(' text=', 1)[1].strip()})
        elif line.startswith('[W5D] ordinal '):
            ordinal = int(f['cdx_proc'])
    webs = {}
    cdx = os.path.join(d, 'cdx')
    if ordinal is not None and os.path.exists(cdx):
        for line in open(cdx, errors='replace'):
            if ' proc=%d ' % ordinal not in line:
                continue
            f = fields(line)
            m = re.match(r'\[CDX\] (p[12]dec|p[12]color|webdetail) ', line)
            if not m:
                continue
            key = (f.get('phase'), int(f['web']))
            w = webs.setdefault(key, {'phase': f.get('phase'), 'web': int(f['web'])})
            if m.group(1).endswith('dec'):
                w.update(save=float(f['save']), nocs=int(f['nocs']), decision=f['decision'],
                         sym=int(f['sym']))
            elif m.group(1).endswith('color'):
                w['reg'] = f['reg']
            else:
                w['sym'] = int(f['sym'])
    return {'proc': proc, 'table': table, 'nodes': nodes, 'sets': sets, 'bits': bits,
            'regcand': regcand, 'ordinal': ordinal, 'webs': webs}


def bit_rows(r):
    """bit -> summary dict (text, occurrence blocks, PRE result, sets, webs)."""
    rows = {}
    allbits = set(r['bits']) | set(r['table'])
    for b in sorted(allbits):
        ent = r['bits'].get(b, {})
        text = ent.get('cm') or ent.get('gc') or (r['table'].get(b, ('', '', ''))[2])
        row = {'bit': b, 'text': text, 'kind': ent.get('kind')}
        for vec in ('antlocs', 'avlocs', 'insert', 'delete', 'subinsert', 'subdelete'):
            row[vec] = [n for n in sorted(r['nodes']) if b in r['nodes'][n].get(vec, ())]
        row['sets'] = sorted(k for k, v in r['sets'].items() if b in v)
        row['webs'] = [w for w in r['webs'].values() if w.get('sym') == b]
        rows[b] = row
    return rows


def fmt_row(row):
    s = '%4d  %s' % (row['bit'], row['text'])
    parts = []
    if row['antlocs']:
        parts.append('antloc ' + ','.join(map(str, row['antlocs'])))
    if row['delete']:
        parts.append('DELETE ' + ','.join(map(str, row['delete'])))
    if row['insert']:
        parts.append('INSERT ' + ','.join(map(str, row['insert'])))
    if row['sets']:
        parts.append(' '.join(x.replace('@ ', '').replace(' ', '') for x in row['sets']))
    for w in row['webs']:
        parts.append('%s:w%d save=%.2f nocs=%s %s->%s' % (w['phase'], w['web'], w.get('save', -1),
                                                       w.get('nocs'), w.get('decision'), w.get('reg', '-')))
    if parts:
        s += '\n        ' + ' | '.join(parts)
    return s


def report(r, all_bits=False, only=None):
    out = ['procedure %s  (colouring ordinal %s)' % (r['proc'], r['ordinal'])]
    rows = bit_rows(r)
    out.append('\n== expression bits (variables omitted unless --all) ==')
    for b, row in rows.items():
        if only and b not in only:
            continue
        if not all_bits and not only and row['kind'] == 3 and not row['webs']:
            continue
        if not all_bits and not only and not (row['antlocs'] or row['webs'] or row['sets']):
            continue
        out.append(fmt_row(row))
    out.append('\n== register candidates for constants / addresses (makelivranges) ==')
    agg = {}
    for c in r['regcand']:
        k = (c['what'], c['text'], c['ctx'], c['decision'])
        agg.setdefault(k, []).append(c['bb'])
    for (what, text, ctx, dec), bbs in sorted(agg.items(), key=lambda kv: (kv[0][1], kv[0][2])):
        out.append('  %-8s %-22s ctx=%d %-14s blocks %s' % (what, text, ctx,
                   'LIVE-RANGE' if dec else 'remat', ','.join(map(str, bbs))))
    return '\n'.join(out)


if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('dir'); ap.add_argument('proc')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('--bits', default='')
    ap.add_argument('--json')
    a = ap.parse_args()
    r = load(a.dir, a.proc)
    only = set(int(x) for x in a.bits.split(',') if x) or None
    print(report(r, a.all, only))
    if a.json:
        rows = bit_rows(r)
        json.dump({'proc': a.proc, 'rows': list(rows.values()), 'regcand': r['regcand']},
                  open(a.json, 'w'), indent=1, default=list)
