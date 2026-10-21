"""w36y: N64 struct layouts with MAT3 field names -> types.md and types_n64.h (C89 draft, NOT installed).

    python3 cloud/work/frontier/w36y/tools/types_out.py

Inputs: build/field_votes.json (tools/fields.py), docs/external/gc/types.h (GC fields/offsets), map_data.json.
Per GC field: every vote "GC off -> N64 off" inside the field gives a delta (N64 - GC); the field's delta is the
best-supported one (ties -> 'ambiguous'). Unvoted fields between two voted fields with the same delta inherit it
("interp"). N64 element sizes: KNOWN_STRIDES (lane evidence) or GC size + the delta of the last voted field.
"""
import json
import os
import re
import sys
from collections import Counter, OrderedDict, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import fields as F  # noqa: E402

LANE = os.path.abspath(os.path.join(HERE, '..'))
REPO = F.REPO

# N64 element sizes stated by matched work (GC size in comment)
KNOWN_STRIDES = {
    'tCarData': (0x3B8, 'gGameCar stride 0x3B8 (lane RESULTS: "gGameCar 0x80152818, stride 0x3B8"); GC 0x3CC'),
    'tModelData': (0x808, 'model[] stride 0x808 (several lanes); GC 0x808'),
    '__anon_3d1b': (0x4C, 'gPlayers: input_rec0 0x8014A118 / input_rec1 0x8014A164 (GAME_SYMBOLS) and w31d '
                          '"gPlayers[i] + 0x4C (76-B stride)"; GC 0x50'),
    '__anon_d40e': (0x8, 'gLink: w36s "8-byte N64 records, owner at +7"; GC 0xA'),
    '__anon_1b1b': (0x98, 'gCamera stride 0x98 (names.md, ShowCountdown); GC 0x98'),
}


def used_by():
    """{struct name: 'used by' line} from types.h."""
    out = {}
    last = None
    for line in open(F.TYPES_H):
        m = re.match(r'/\* used by: (.*) \*/', line)
        if m:
            last = m.group(1)
            continue
        m = re.match(r'^(struct|union) (\S+) \{', line)
        if m:
            out[m.group(2)] = last
            last = None
        elif not line.startswith('/*'):
            last = None if line.strip() else last
    return out


def field_at(T, off):
    for f in T['fields']:
        if f['off'] <= off < f['off'] + max(f['size'], 1):
            return f
    return None


def chain(votes):
    """Max-weight chain of (gc off, n64 off) votes with both offsets strictly increasing (field order is preserved
    between the two builds); returns {gc off: n64 off} for the chain members and the rejected votes."""
    pts = []
    for go, d in votes.items():
        for no, fs in d.items():
            pts.append((int(go, 16), int(no, 16), len(fs)))
    pts = [p for p in pts if p[0] >= 0 and p[1] >= 0]
    pts.sort()
    n = len(pts)
    best = [0] * n
    prev = [-1] * n
    for i in range(n):
        best[i] = pts[i][2]
        for j in range(i):
            if pts[j][0] < pts[i][0] and pts[j][1] < pts[i][1] and best[j] + pts[i][2] > best[i]:
                best[i] = best[j] + pts[i][2]
                prev[i] = j
    if not n:
        return {}, []
    i = max(range(n), key=lambda k: best[k])
    keep = {}
    while i >= 0:
        keep[pts[i][0]] = (pts[i][1], pts[i][2])
        i = prev[i]
    rejected = [p for p in pts if keep.get(p[0], (None,))[0] != p[1]]
    return keep, rejected


def layout(name, T, votes):
    """Per-field rows: dict(f, delta, votes, status, alts, ev)."""
    keep, rejected = chain(votes)
    per = defaultdict(Counter)
    ev = defaultdict(set)
    rej = defaultdict(list)
    for go, (no, n) in keep.items():
        f = field_at(T, go)
        if f is None:
            continue
        per[f['off']][no - go] += n
        for x in votes.get(hx(go), {}).get(hx(no), []):
            ev[f['off']].add(x)
    for go, no, n in rejected:
        f = field_at(T, go)
        if f is not None:
            rej[f['off']].append('%+d x%d' % (no - go, n))
    rows = []
    for f in T['fields']:
        c = per.get(f['off'])
        if c:
            ranked = c.most_common()
            delta, n = ranked[0]
            amb = len(ranked) > 1 and ranked[1][1] * 2 > n
            status = 'ambiguous' if amb else ('same' if delta == 0 else 'moved')
            rows.append(dict(f=f, delta=delta, votes=sum(c.values()), status=status,
                             alts=['%+d x%d' % a for a in ranked[1:3]] + rej[f['off']][:2],
                             ev=sorted(ev[f['off']])[:4]))
        else:
            rows.append(dict(f=f, delta=None, votes=0, status='unseen', alts=rej[f['off']][:3], ev=[]))
    voted = [i for i, r in enumerate(rows) if r['status'] in ('same', 'moved')]
    for a, b in zip(voted, voted[1:]):
        if rows[a]['delta'] == rows[b]['delta']:
            for i in range(a + 1, b):
                if rows[i]['status'] == 'unseen':
                    rows[i]['delta'] = rows[a]['delta']
                    rows[i]['status'] = 'interp'
    return rows


def hx(v):
    return '-0x%X' % -v if v < 0 else '0x%X' % v


def c_decl(decl, newname=None):
    d = decl
    d = d.replace('signed char', 's8').replace('unsigned char', 'u8').replace('unsigned short', 'u16')
    d = d.replace('signed short', 's16').replace('unsigned int', 'u32').replace('float', 'f32')
    d = re.sub(r'\bint\b', 's32', d).replace('double', 'f64').replace('char ', 's8 ')
    d = re.sub(r'\bstruct __anon_\w+\s*\*', 'void *', d)
    d = re.sub(r'\bunion __anon_\w+\s*\*', 'void *', d)
    d = re.sub(r'\bstruct __anon_\w+', 'u8 /* anon struct */', d)
    d = re.sub(r'\bunion __anon_\w+', 'u8 /* anon union */', d)
    d = re.sub(r'enum @\S+', 's32', d)
    return d


def main():
    types = F.parse_types()
    ub = used_by()
    fv = json.load(open(os.path.join(LANE, 'build', 'field_votes.json')))
    votes = fv['votes']
    md = json.load(open(os.path.join(LANE, 'map_data.json')))['rows']
    globals_of = defaultdict(list)
    for r in md:
        T = F.elem_struct(r['mat3_type'])
        if T and r['confidence'] in ('proven', 'strong'):
            globals_of[T].append('%s %s %s (%s)' % (r['mat3_name'], r['population'], r['n64_address'], r['confidence']))
    ranked = sorted(votes.items(), key=lambda kv: -sum(len(fs) for d in kv[1].values() for fs in d.values()))
    L = ['# N64 struct layouts with MAT3 field names (lane w36y, wave 36)', '',
         'Generated by `cloud/work/frontier/w36y/tools/types_out.py` from `build/field_votes.json` '
         '(tools/fields.py: pointer-parameter accesses of w36n proven/strong function pairs + aligned global '
         'accesses of map_data.json). Field NAMES from MAT3 (docs/external/gc/types.h, GC offsets), N64 OFFSETS '
         'from N64 code. Status: **same** = N64 accesses at the GC offset; **moved** = N64 offset differs '
         '(delta = N64 - GC); **interp** = no access seen, between two voted fields with the same delta; '
         '**ambiguous** = competing deltas; unseen = no evidence (assume nothing). `votes` = aligned accesses '
         '(function-parameter or global) supporting the row. Votes are filtered to the max-weight chain in which GC and '
         'N64 offsets both increase (field order is the same in both builds); votes off the chain are listed as '
         '"other votes" (delta x count). A same-offset vote is weaker evidence than a moved one (the aligner favours '
         'identical offsets of the same width); single-vote rows are leads.', '',
         'C89 draft of the well-evidenced structs: `types_n64.h` (NOT installed; offsets in comments; unknown '
         'spans as `u8 unk_XXX[n]`).', '']
    hdr = ['/* types_n64.h -- DRAFT (lane w36y, wave 36). NOT installed; not included by any build.',
           ' * N64 layouts of MAT3 (GameCube) structs: field names from MAT3 types.h, offsets from N64 code',
           ' * (cloud/work/frontier/w36y/types.md has the evidence per field). Only fields with N64 evidence',
           ' * (same/moved) or interpolated between two such fields are placed; everything else is unk_ padding.',
           ' * Generated by cloud/work/frontier/w36y/tools/types_out.py. C89. */',
           '#ifndef W36Y_TYPES_N64_H', '#define W36Y_TYPES_N64_H', '',
           'typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;',
           'typedef signed int s32; typedef unsigned int u32; typedef float f32; typedef double f64;', '']
    summary = []
    for name, v in ranked:
        T = types.get(name)
        if T is None:
            continue
        nvotes = sum(len(fs) for d in v.values() for fs in d.values())
        if nvotes < 4:
            continue
        rows = layout(name, T, v)
        moved = [r for r in rows if r['status'] == 'moved']
        seen = [r for r in rows if r['status'] in ('same', 'moved', 'ambiguous')]
        stride = KNOWN_STRIDES.get(name)
        last = [r for r in rows if r['status'] in ('same', 'moved', 'interp')]
        if stride:
            n64size, sz_ev = stride
        elif last and last[-1]['f'] is T['fields'][-1]:
            n64size, sz_ev = T['size'] + last[-1]['delta'], 'GC size + delta of the last field (%+d)' % last[-1]['delta']
        else:
            n64size, sz_ev = None, 'unknown (last field not seen)'
        label = ub.get(name) or ''
        summary.append((name, label, T['size'], n64size, len(seen), len(T['fields']), len(moved), nvotes))
        L.append('## %s %s' % (T['kind'], name))
        L.append('')
        L.append('- used by (GC): %s' % (label or '-'))
        if globals_of.get(name):
            L.append('- N64 globals of this type (map_data.json): %s' % '; '.join(globals_of[name][:6]))
        L.append('- GC size 0x%X; N64 size %s (%s)' % (T['size'], ('0x%X' % n64size) if n64size else '?', sz_ev))
        L.append('- fields with N64 evidence: %d of %d (%d moved); %d aligned accesses' % (
            len(seen), len(T['fields']), len(moved), nvotes))
        L.append('')
        L.append('| GC off | field | GC size | N64 off | status | votes | evidence (first) |')
        L.append('|---|---|---|---|---|---|---|')
        for r in rows:
            f = r['f']
            no = '0x%X' % (f['off'] + r['delta']) if r['delta'] is not None else ''
            alts = (' (other votes %s)' % ', '.join(r['alts'])) if r['alts'] else ''
            L.append('| 0x%X | `%s` | 0x%X | %s | %s%s | %d | %s |' % (
                f['off'], f['decl'], f['size'], no, r['status'] + (' %+d' % r['delta'] if r['status'] == 'moved'
                                                                   else ''), alts, r['votes'],
                (r['ev'][0] if r['ev'] else '').replace('|', '/')))
        L.append('')
        # C draft
        if nvotes >= 8 and seen and T['kind'] == 'struct':
            place = []
            for r in rows:
                if r['status'] in ('same', 'moved', 'interp') and f is not None:
                    o = r['f']['off'] + r['delta']
                    if o >= 0:
                        place.append((o, r))
            place.sort(key=lambda t: t[0])
            hdr.append('/* %s: GC %s %s (size 0x%X); N64 size %s. used by: %s */' % (
                name, T['kind'], name, T['size'], ('0x%X' % n64size) if n64size else '?', label))
            hdr.append('typedef %s N64_%s {' % (T['kind'], name.lstrip('_')))
            pos = 0
            for o, r in place:
                f = r['f']
                if o < pos:
                    hdr.append('    /* overlap: %s at N64 0x%X (GC 0x%X) skipped */' % (f['name'], o, f['off']))
                    continue
                if o > pos:
                    hdr.append('    u8 unk_%03X[0x%X];' % (pos, o - pos))
                hdr.append('    %s; /* N64 0x%03X, GC 0x%03X, %s */' % (c_decl(f['decl']), o, f['off'], r['status']))
                pos = o + f['size']
            if n64size and n64size > pos:
                hdr.append('    u8 unk_%03X[0x%X];' % (pos, n64size - pos))
            hdr.append('} N64_%s; /* size %s */' % (name.lstrip('_'), ('0x%X' % n64size) if n64size else '?'))
            hdr.append('')
    hdr.append('#endif /* W36Y_TYPES_N64_H */')
    S = ['', '## Summary (struct, GC users, GC size, N64 size, fields seen/total, moved, votes)', '',
         '| struct | used by (GC) | GC size | N64 size | seen/total | moved | votes |', '|---|---|---|---|---|---|---|']
    for name, label, gs, ns, se, tot, mv, nv in summary:
        S.append('| [%s](#%s-%s) | %s | 0x%X | %s | %d/%d | %d | %d |' % (
            name, 'struct', name.lower(), (label or '')[:60], gs, ('0x%X' % ns) if ns else '?', se, tot, mv, nv))
    L = L[:8] + S + [''] + L[8:]
    open(os.path.join(LANE, 'types.md'), 'w').write('\n'.join(L) + '\n')
    open(os.path.join(LANE, 'types_n64.h'), 'w').write('\n'.join(hdr) + '\n')
    print('types.md: %d structs' % len(summary))


if __name__ == '__main__':
    main()
