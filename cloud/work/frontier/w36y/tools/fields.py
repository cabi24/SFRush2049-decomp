"""w36y: struct field-offset evidence (GC field -> N64 offset) for MAT3 struct types.

Read-only; standard library only.

    python3 cloud/work/frontier/w36y/tools/fields.py     # writes build/field_votes.json

Channel P (pointer parameters): for every w36n proven/strong function pair, each GC parameter whose type is a pointer
to a struct/union is followed through register copies, stack spills and `addi` adjustments on both sides
(GC: r3.. by argument order; N64: the o32 a-register slot of the same argument); the field accesses (displacement,
access class, use count) are aligned per parameter (identical offset + compatible class scores high, otherwise
first-use order), and every aligned pair votes "GC field off -> N64 off" for that struct type.
Channel G (global objects): GC offsets inside element 0 / variable-index accesses of a placed global (map_data.json
rows: field_offsets_gc_to_n64) vote for the element struct type.
"""
import json
import os
import re
import sys
from collections import Counter, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import refs as R  # noqa: E402
import mapdata as M  # noqa: E402

REPO = R.REPO
TYPES_H = os.path.join(REPO, 'docs', 'external', 'gc', 'types.h')


def parse_types():
    """{name: dict(kind, size, fields=[dict(off, size, decl, name)])} from docs/external/gc/types.h."""
    out = {}
    cur = None
    for line in open(TYPES_H):
        m = re.match(r'^(struct|union) (\S+) \{ /\* size 0x([0-9a-fA-F]+) \*/', line)
        if m:
            cur = dict(kind=m.group(1), size=int(m.group(3), 16), fields=[])
            out[m.group(2)] = cur
            continue
        if cur is not None:
            if line.startswith('};'):
                cur = None
                continue
            m = re.match(r'^\s+(.+?);\s*/\* 0x([0-9a-fA-F]+) size 0x([0-9a-fA-F]+)', line)
            if m:
                decl = m.group(1)
                nm = re.findall(r'([A-Za-z_]\w*)\s*(?:\[[^\]]*\]\s*)*(?::\s*\d+)?$', decl)
                cur['fields'].append(dict(off=int(m.group(2), 16), size=int(m.group(3), 16), decl=decl,
                                          name=nm[-1] if nm else decl))
    return out


def struct_of(typestr):
    """'struct tCarData *' -> 'tCarData' (one level of pointer to struct/union), else None."""
    if not typestr:
        return None
    m = re.match(r'^(?:const\s+|volatile\s+)*(struct|union)\s+(\S+)\s*\*$', typestr.strip())
    return m.group(2) if m else None


def elem_struct(typestr):
    """'struct X [6]' / 'struct X' / 'struct X [4][3]' -> 'X' (global object element type)."""
    if not typestr:
        return None
    m = re.match(r'^(?:struct|union)\s+(\S+)(\s*\[\d+\])*$', typestr.strip())
    return m.group(1) if m else None


FLOATS = ('float', 'double')


def arg_regs(params):
    """[(param index, GC reg 'r3'.., N64 reg number or None)] for the integer/pointer arguments."""
    out = []
    gi = 3
    slot = 0
    all_float = True
    for i, p in enumerate(params):
        t = (p.get('type') or '').strip()
        isf = t in FLOATS
        isd = t == 'double'
        if isf:
            if isd and slot % 2:
                slot += 1
            slot += 2 if isd else 1
            continue
        nreg = 4 + slot if slot < 4 else None
        out.append((i, 'r%d' % gi, nreg))
        gi += 1
        slot += 1
        all_float = False
    return out


def gc_param_accesses(path, regs):
    """{param index: [(off, cls, kind)] in order} from a GC disassembly file; regs: {GC reg number: param idx}."""
    st = {r: ('p', i, 0) for r, i in regs.items()}
    stack = {}
    out = defaultdict(list)
    with open(path) as fh:
        fh.readline()
        for line in fh:
            m = R.LINE_RE.match(line)
            if not m:
                continue
            mn = m.group(3).rstrip('.')
            ops = m.group(4)
            parts = [p.strip() for p in ops.split(',')] if ops else []
            if mn == 'bl':
                for r in [0] + list(range(3, 13)):
                    st.pop(r, None)
                continue
            dest = None
            newval = None
            if mn in R.GC_LOADS or mn in R.GC_STORES:
                mm = re.match(r'(-?0x[0-9a-fA-F]+|-?\d+)\((r\d+)\)', parts[1]) if len(parts) > 1 else None
                if mm:
                    b = R._reg(mm.group(2))
                    d = int(mm.group(1), 0)
                    if b == 1:
                        if mn in R.GC_STORES and not mn.startswith('stf'):
                            v = st.get(R._reg(parts[0]))
                            if v:
                                stack[d] = v
                            else:
                                stack.pop(d, None)
                        elif mn in R.GC_LOADS and not mn.startswith('lf'):
                            dest = R._reg(parts[0])
                            newval = stack.get(d)
                    else:
                        v = st.get(b)
                        if v:
                            k = R.GC_LOADS.get(mn) or R.GC_STORES.get(mn)
                            out[v[1]].append((v[2] + d, M.CLS[k], k))
                        if mn in R.GC_LOADS and not mn.startswith('lf'):
                            dest = R._reg(parts[0])
                elif mn in R.GC_LOADS and not mn.startswith('lf'):
                    dest = R._reg(parts[0])
            elif mn == 'mr':
                dest = R._reg(parts[0])
                newval = st.get(R._reg(parts[1]))
            elif mn == 'addi' and len(parts) == 3 and '@' not in ops:
                dest = R._reg(parts[0])
                v = st.get(R._reg(parts[1]))
                try:
                    d = int(parts[2], 0)
                except ValueError:
                    d = None
                if v and d is not None:
                    newval = ('p', v[1], v[2] + d)
            elif parts and not mn.startswith(R.NO_DEST) and not mn.startswith('f') and not mn.startswith('ps_'):
                dest = R._reg(parts[0])
            if dest is not None:
                if newval is None:
                    st.pop(dest, None)
                else:
                    st[dest] = newval
    return out


def n64_param_accesses(img, addr, size, regs):
    """{param index: [(off, cls, kind)]} from N64 MIPS; regs: {N64 reg number: param idx}."""
    st = {r: ('p', i, 0) for r, i in regs.items()}
    stack = {}
    out = defaultdict(list)
    pending = False
    for pc in range(addr, addr + size, 4):
        w = img.word(pc)
        if w is None:
            break
        op = w >> 26
        rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        imm = R.sext16(w & 0xFFFF)
        dest = None
        newval = None
        clob = False
        if op in R.LOADS or op in R.STORES:
            if rs == 29:
                if op == 0x2B:
                    v = st.get(rt)
                    if v:
                        stack[imm] = v
                    else:
                        stack.pop(imm, None)
                elif op == 0x23:
                    dest, newval = rt, stack.get(imm)
                elif op in R.LOADS and op not in (0x31, 0x35):
                    dest = rt
            else:
                v = st.get(rs)
                if v:
                    if op in R.LOADS:
                        k, sg = R.LOADS[op]
                        kind = {'b': 'lb', 'h': 'lh', 'w': 'lw', 'f': 'lf', 'd': 'ld', 'wl': 'lwl', 'wr': 'lwr'}[k] + (
                            'u' if sg == 'u' else '')
                    else:
                        kind = 's' + R.STORES[op]
                    out[v[1]].append((v[2] + imm, M.CLS[kind], kind))
                if op in R.LOADS and op not in (0x31, 0x35):
                    dest = rt
        elif op == 0x09:
            dest = rt
            v = st.get(rs)
            if v and rs != 29:
                newval = ('p', v[1], v[2] + imm)
        elif op == 0:
            f = w & 0x3F
            if f in (0x21, 0x25) and (rt == 0 or rs == 0):
                dest, newval = rd, st.get(rs if rt == 0 else rt)
            elif f == 0x09:
                dest, clob = rd, True
            elif f in (0x08, 0x18, 0x19, 0x1A, 0x1B, 0x0C, 0x0D):
                pass
            else:
                dest = rd
        elif op == 0x03:
            clob = True
        elif op in (0x08, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F):
            dest = rt
        elif op == 0x11 and rs in (0, 2):
            dest = rt
        if dest:
            if newval is None:
                st.pop(dest, None)
            else:
                st[dest] = newval
        if pending:
            for r in R.CALLER_SAVED:
                st.pop(r, None)
            pending = False
        if clob:
            pending = True
    return out


def dedup(lst):
    seen = {}
    out = []
    for off, cls, kind in lst:
        k = (off, cls)
        if k in seen:
            seen[k]['count'] += 1
            continue
        e = dict(off=off, cls=cls, kind=kind, count=1)
        seen[k] = e
        out.append(e)
    return out


def align_fields(G, N, expect=None):
    def sc(g, n):
        if not M.compat(g['cls'], n['cls']):
            return -3.0
        s = 1.0 + 0.5 * M.count_sim(g, n) + (0.3 if g['cls'] == n['cls'] else 0)
        if (expect(g['off']) if expect else g['off']) == n['off']:
            s += 3.0
        return s
    n, m = len(G), len(N)
    if not n or not m:
        return []
    GAP = -0.4
    prev = [j * GAP for j in range(m + 1)]
    back = []
    for i in range(1, n + 1):
        cur = [i * GAP] + [0.0] * m
        bi = [1] * (m + 1)
        for j in range(1, m + 1):
            d = prev[j - 1] + sc(G[i - 1], N[j - 1])
            u = prev[j] + GAP
            l = cur[j - 1] + GAP
            if d >= u and d >= l:
                cur[j], bi[j] = d, 0
            elif u >= l:
                cur[j], bi[j] = u, 1
            else:
                cur[j], bi[j] = l, 2
        back.append(bi)
        prev = cur
    i, j = n, m
    out = []
    while i > 0 and j > 0:
        b = back[i - 1][j]
        if b == 0:
            if sc(G[i - 1], N[j - 1]) > 0:
                out.append((G[i - 1], N[j - 1]))
            i, j = i - 1, j - 1
        elif b == 1:
            i -= 1
        else:
            j -= 1
    return out


def hx(v):
    return '-0x%X' % -v if v < 0 else '0x%X' % v


def align_consistent(G, N):
    """Two alignments (identical offsets favoured / pure first-use order); keep the one whose deltas
    (N64 off - GC off) agree best: a layout that differs from GC shows one shifted delta in order, while an
    identical layout shows delta 0 with the offset bonus."""
    a1 = align_fields(G, N)
    a2 = align_fields_order(G, N)

    def score(al):
        if not al:
            return (0, 0)
        c = Counter(n['off'] - g['off'] for g, n in al)
        top = c.most_common(1)[0][1]
        return (top - (len(c) - 1), len(al))
    return a1 if score(a1) >= score(a2) else a2


def align_fields_order(G, N):
    saved = M.compat
    out = []
    n, m = len(G), len(N)
    if not n or not m:
        return out

    def sc(g, nn):
        if not M.compat(g['cls'], nn['cls']):
            return -3.0
        return 1.0 + 0.5 * M.count_sim(g, nn) + (0.3 if g['cls'] == nn['cls'] else 0)
    GAP = -0.4
    prev = [j * GAP for j in range(m + 1)]
    back = []
    for i in range(1, n + 1):
        cur = [i * GAP] + [0.0] * m
        bi = [1] * (m + 1)
        for j in range(1, m + 1):
            d = prev[j - 1] + sc(G[i - 1], N[j - 1])
            u = prev[j] + GAP
            l = cur[j - 1] + GAP
            if d >= u and d >= l:
                cur[j], bi[j] = d, 0
            elif u >= l:
                cur[j], bi[j] = u, 1
            else:
                cur[j], bi[j] = l, 2
        back.append(bi)
        prev = cur
    i, j = n, m
    while i > 0 and j > 0:
        b = back[i - 1][j]
        if b == 0:
            if sc(G[i - 1], N[j - 1]) > 0:
                out.append((G[i - 1], N[j - 1]))
            i, j = i - 1, j - 1
        elif b == 1:
            i -= 1
        else:
            j -= 1
    return out


def make_prior(votes):
    """{T: expect(go)} from the pass-1 order-preserving chain (delta of the nearest lower chain point)."""
    import types_out as TO
    pri = {}
    for T, d in votes.items():
        ser = {hx(go): {hx(no): fs for no, fs in dd.items()} for go, dd in d.items()}
        keep, rej = TO.chain(ser)
        pts = sorted((go, no - go) for go, (no, n) in keep.items() if n >= 2)
        if not pts:
            continue

        def expect(go, pts=pts):
            best = None
            for g0, dl in pts:
                if g0 <= go:
                    best = dl
                else:
                    break
            return go + (best if best is not None else 0)
        pri[T] = expect
    return pri


def main():
    v1 = collect(None)
    v2 = collect(make_prior(v1[0]))
    write(*v2)


def collect(prior):
    types = parse_types()
    n64 = R.N64()
    gc = R.GC()
    fj = json.load(open(os.path.join(REPO, 'docs', 'external', 'gc', 'functions.json')))['functions']
    gfun = {}
    for f in fj:
        gfun[int(f['address'], 16)] = f
    pairs = M.load_pairs()
    votes = defaultdict(lambda: defaultdict(lambda: defaultdict(set)))  # T -> gc off -> n64 off -> {fn}
    kinds = defaultdict(set)  # (T, n64 off) -> kinds
    used = 0
    for p in pairs:
        f = gfun.get(p['gc'])
        if not f or not f['params']:
            continue
        regs = arg_regs(f['params'])
        sp = [(i, g, n) for i, g, n in regs if struct_of(f['params'][i].get('type'))]
        if not sp:
            continue
        path = gc.files.get(p['gc'])
        if not path:
            continue
        pop, name = p['n64'].split(':', 1)
        img = n64.imgs[pop]
        ent = [x for x in img.funcs if x[2] == name]
        if not ent:
            continue
        a, s, _ = ent[0]
        greg = {int(g[1:]): i for i, g, n in sp}
        nreg = {n: i for i, g, n in sp if n is not None}
        GA = gc_param_accesses(path[0], greg)
        NA = n64_param_accesses(img, a, s, nreg)
        used += 1
        for i, g, n in sp:
            if n is None:
                continue
            T = struct_of(f['params'][i]['type'])
            G = dedup(GA.get(i, []))
            N = dedup(NA.get(i, []))
            al = align_fields(G, N, prior[T]) if prior and T in prior else align_consistent(G, N)
            for ge, ne in al:
                votes[T][ge['off']][ne['off']].add('%s(%s)' % (p['n64'], f['params'][i]['name']))
                kinds[(T, ne['off'])].add(ne['kind'])
    # channel G: placed globals of struct type, re-aligned per function pair (GC accesses of the symbol vs N64
    # accesses inside the placed N64 object), offsets inside element 0 / variable-index accesses only
    md = json.load(open(os.path.join(HERE, '..', 'map_data.json')))['rows']
    placed = {}
    for r in md:
        if r['confidence'] not in ('proven', 'strong'):
            continue
        T = elem_struct(r['mat3_type'])
        if not T or T not in types:
            continue
        sc = 'game' if r['population'] in ('game', 'static-data') else r['population']
        ext = r.get('n64_extent_capped') or r['mat3_size'] or types[T]['size']
        placed[r['mat3_key']] = (T, sc, int(r['n64_address'], 16), ext, r['mat3_name'])
    jt = M.jump_tables(n64)
    st = M.n64_stats(n64, jt)
    for p in pairs:
        G = M.gc_elements(gc, p['gc'])
        keys = set(g['sym'] for g in G) & set(placed)
        if not keys:
            continue
        N = M.n64_elements(n64, p['n64'], st, jt)
        for k in keys:
            T, sc, B, ext, nm = placed[k]
            esz = types[T]['size']
            g2 = [dict(g) for g in G if g['sym'] == k and 0 <= g['off'] < esz and g['cls'] != 'a']
            n2 = []
            for nn in N:
                if nn['scope'] == sc and B <= nn['addr'] < B + ext and nn['cls'] != 'a':
                    x = dict(nn)
                    x['off'] = nn['addr'] - B
                    n2.append(x)
            al = align_fields(g2, n2, prior[T]) if prior and T in prior else align_consistent(g2, n2)
            for ge, ne in al:
                votes[T][ge['off']][ne['off']].add('global %s@0x%08X %s' % (nm, B, p['n64']))
    return votes, kinds, used


def write(votes, kinds, used):
    ser = {}
    for T, d in votes.items():
        ser[T] = {hx(go): {hx(no): sorted(fs) for no, fs in dd.items()} for go, dd in sorted(d.items())}
    json.dump(dict(pairs_with_struct_params=used, votes=ser,
                   kinds={'%s+0x%X' % k: sorted(v) for k, v in kinds.items()}),
              open(os.path.join(HERE, '..', 'build', 'field_votes.json'), 'w'), indent=0)
    print('pairs with struct params: %d; struct types with votes: %d' % (used, len(votes)))


if __name__ == '__main__':
    main()
