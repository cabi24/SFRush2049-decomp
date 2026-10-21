"""w36y: MAT3 (GameCube) -> N64 DATA symbol map by aligning the global references of identified function pairs.

Read-only; standard library only; ~30 s on the Pi.

    python3 cloud/work/frontier/w36y/tools/mapdata.py        # writes cloud/work/frontier/w36y/build/votes.json

Pairs: every N64 function with a proven/strong MAT3 identity in cloud/work/frontier/w36n/map_functions.json.
Per pair, both functions' global references (tools/refs.py) are reduced to first-use element lists and aligned with
Needleman-Wunsch (score: equal access class 2, +0.5 same mode, float vs word 0.5, otherwise mismatch -3; gap -0.4;
iteration 2+ adds +4 for an element pair that agrees with an already accepted mapping and -4 for one that
contradicts it). An aligned element pair (GC symbol S at offset o, N64 address A) votes for "S lives at N64 base
A - o" (scope: game-wide address, or per runtime image inside 0x8038A400..). One vote per function pair.
"""
import json
import os
import sys
from collections import Counter, OrderedDict, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import refs as R  # noqa: E402

REPO = R.REPO
OUT = os.path.join(HERE, '..', 'build')

CLS = {'lb': 'b', 'lbu': 'b', 'sb': 'b', 'lh': 'h', 'lhu': 'h', 'sh': 'h', 'lw': 'w', 'sw': 'w', 'lf': 'f', 'sf': 'f',
       'ld': 'd', 'sd': 'd', 'addr': 'a', 'lwl': 'w', 'lwr': 'w'}


def scope_of(pop, a):
    if R.OVL_BASE <= a < 0x80400000 and pop in ('ovl_a', 'ovl_b'):
        return pop
    return 'game'


def load_pairs():
    m = json.load(open(os.path.join(REPO, 'cloud', 'work', 'frontier', 'w36n', 'map_functions.json')))
    pairs = []
    for r in m['rows']:
        if r['confidence'] not in ('proven', 'strong') or r['population'] == 'static':
            continue
        if not r.get('gc_address'):
            continue
        pairs.append(dict(n64='%s:%s' % (r['population'], r['n64_name']), pop=r['population'],
                          gc=int(r['gc_address'], 16), gc_name=r['mat3_key'], gc_file=r['mat3_file'],
                          conf=r['confidence']))
    return pairs


def jump_tables(n64):
    """N64 rodata addresses loaded by `lw` and then used by `jr` (switch tables)."""
    jt = set()
    for pop in ('game', 'ovl_a', 'ovl_b'):
        img = n64.imgs[pop]
        for key in [k for k in n64.refs if k.startswith(pop + ':')]:
            for ref in n64.refs[key]:
                if ref['kind'] != 'lw' or ref['mode'] != 'indexed':
                    continue
                for d in range(4, 24, 4):
                    w = img.word(ref['pc'] + d)
                    if w is not None and (w >> 26) == 0 and (w & 0x3F) == 0x08 and ((w >> 21) & 31) != 31:
                        jt.add(ref['addr'])
                        break
    return jt


def n64_stats(n64, jt):
    """Program-wide per-address stats: stored?, kinds, functions."""
    st = defaultdict(lambda: dict(kinds=Counter(), funcs=set(), stored=False, modes=Counter()))
    for key, lst in n64.refs.items():
        pop = key.split(':')[0]
        for ref in lst:
            sk = (scope_of(pop, ref['addr']), ref['addr'])
            s = st[sk]
            s['kinds'][ref['kind']] += 1
            s['funcs'].add(key)
            s['modes'][ref['mode']] += 1
            if ref['kind'].startswith('s'):
                s['stored'] = True
    return st


def n64_elements(n64, key, st, jt):
    pop = key.split(':')[0]
    lst = n64.refs.get(key) or []
    based = set(r['base'] for r in lst if r['base'] is not None)
    seen = {}
    out = []
    for ref in lst:
        a = ref['addr']
        if a in jt:
            continue
        sc = scope_of(pop, a)
        if n64.is_rodata(pop, a) and ref['mode'] == 'direct' and ref['kind'] in ('lf', 'ld') and \
                not st[(sc, a)]['stored']:
            continue  # float literal
        c = CLS[ref['kind']]
        k = (sc, a, c)
        if k in seen:
            seen[k]['count'] += 1
            continue
        e = dict(scope=sc, addr=a, cls=c, kind=ref['kind'], mode=ref['mode'], base=ref['base'], off=ref['off'],
                 count=1)
        seen[k] = e
        out.append(e)
    return out


def gc_elements(gc, gaddr):
    lst = gc.refs(gaddr) or []
    seen = {}
    out = []
    for ref in lst:
        sym = gc.key(ref['sym'], ref['start'])
        if sym.startswith('@') or sym.startswith('...') or sym.startswith('lbl_') or '$localstatic' in sym and \
                sym.startswith('@'):
            continue
        c = CLS[ref['kind']]
        k = (sym, ref['off'], c)
        if k in seen:
            seen[k]['count'] += 1
            continue
        e = dict(sym=sym, off=ref['off'], cls=c, kind=ref['kind'], mode=ref['mode'], count=1)
        seen[k] = e
        out.append(e)
    return out


def gkey(gc, g):
    return g['sym']


def count_sim(g, n):
    a, b = g.get('count', 1), n.get('count', 1)
    return float(min(a, b)) / max(a, b)


def pair_score(g, n, accepted):
    if g['cls'] == n['cls']:
        s = 2.0 + (0.5 if g['mode'] == n['mode'] else 0.0) + 0.75 * count_sim(g, n)
    elif set((g['cls'], n['cls'])) == set(('f', 'w')):
        s = 0.5
    else:
        s = -3.0
    if accepted:
        acc = accepted.get(g['sym'])
        if acc is not None:
            sc, base = acc
            if sc == n['scope'] and n['addr'] - g['off'] == base:
                s += 4.0
            elif sc == n['scope'] and 0 <= n['addr'] - base < 0x4000 and g['off'] != 0:
                s += 1.0  # same object, different field layout
            else:
                s -= 4.0
        else:
            # N64 address already owned by another GC symbol?
            own = accepted.get(('n64', n['scope'], n['addr']))
            if own is not None and own != g['sym']:
                s -= 2.0
    return s


GAP = -0.4


def align(G, N, accepted):
    n, m = len(G), len(N)
    if not n or not m:
        return []
    # DP
    prev = [j * GAP for j in range(m + 1)]
    back = []
    for i in range(1, n + 1):
        cur = [i * GAP] + [0.0] * m
        bi = [0] * (m + 1)
        bi[0] = 1
        gi = G[i - 1]
        for j in range(1, m + 1):
            d = prev[j - 1] + pair_score(gi, N[j - 1], accepted)
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
            s = pair_score(G[i - 1], N[j - 1], accepted)
            if s > 0:
                out.append((G[i - 1], N[j - 1], s))
            i, j = i - 1, j - 1
        elif b == 1:
            i -= 1
        else:
            j -= 1
    out.reverse()
    return out


def compat(a, b):
    return a == b or set((a, b)) == set(('f', 'w'))


def assign_pair(G, N, accepted):
    """Object-consistent assignment for one function pair.

    NW gives the order prior; then every GC symbol S is given ONE N64 base b (one-to-one, greedy by score), where
    score(S, b) counts S's elements (offset o, class) that find a compatible N64 element at b + o (offset-set
    agreement), +1.5 for each such element pair NW also aligned, +4/-4 for agreement/contradiction with the
    accepted map of the previous iteration. Elements of S that NW aligned elsewhere inside S's object window
    are kept as layout evidence ('nw') when they do not contradict another symbol's assignment."""
    al = align(G, N, accepted)
    nwpair = {}
    for g, nn, sc in al:
        nwpair[id(g)] = nn
    bykey = defaultdict(list)
    for g in G:
        bykey[g['sym']].append(g)
    nidx = defaultdict(list)
    for nn in N:
        nidx[(nn['scope'], nn['addr'])].append(nn)
    cands = []
    for sym, gl in bykey.items():
        bases = set()
        for g in gl:
            for nn in N:
                if compat(g['cls'], nn['cls']):
                    bases.add((nn['scope'], nn['addr'] - g['off']))
        for g in gl:
            nn = nwpair.get(id(g))
            if nn is not None and nn['base'] is not None and compat(g['cls'], nn['cls']):
                bases.add((nn['scope'], nn['base']))
        acc = accepted.get(sym) if accepted else None
        for (sc, b) in bases:
            items = []
            score = 0.0
            for g in gl:
                hit = None
                for nn in nidx.get((sc, b + g['off']), ()):
                    if compat(g['cls'], nn['cls']):
                        hit = nn
                        break
                if hit is not None:
                    w = 1.0 + (0.5 if g['cls'] == hit['cls'] else 0.0) + 0.75 * count_sim(g, hit)
                    if nwpair.get(id(g)) is hit:
                        w += 1.5
                    items.append((g, hit, 'off'))
                    score += w
                    continue
                nn = nwpair.get(id(g))
                if nn is not None and nn['scope'] == sc and nn['base'] == b and compat(g['cls'], nn['cls']):
                    # same N64 object (field displacement off the formed base), field at another offset
                    items.append((g, nn, 'obj'))
                    score += 1.5 + (0.5 if g['cls'] == nn['cls'] else 0.0)
            if not items:
                continue
            if acc is not None:
                score += 4.0 if acc == (sc, b) else -4.0
            else:
                own = accepted.get(('n64', sc, b)) if accepted else None
                if own is not None and own != sym:
                    score -= 4.0
            cands.append((score, len(items), sym, sc, b, items))
    cands.sort(key=lambda t: (-t[0], -t[1], t[2], t[4]))
    used_sym, used_n, used_base = set(), set(), set()
    out = {}
    for score, n, sym, sc, b, items in cands:
        if score < 2.0 or sym in used_sym or (sc, b) in used_base:
            continue
        items = [it for it in items if id(it[1]) not in used_n]
        if not items:
            continue
        used_sym.add(sym)
        used_base.add((sc, b))
        for it in items:
            used_n.add(id(it[1]))
        out[(sym, sc, b)] = items
    # layout evidence: NW-aligned elements of an assigned symbol at another offset (field moved on N64)
    for (sym, sc, b), items in list(out.items()):
        have = set(id(it[0]) for it in items)
        for g in bykey[sym]:
            if id(g) in have:
                continue
            nn = nwpair.get(id(g))
            if nn is None or id(nn) in used_n or nn['scope'] != sc or not compat(g['cls'], nn['cls']):
                continue
            if 0 <= nn['addr'] - b < 0x4000:
                items.append((g, nn, 'nw'))
                used_n.add(id(nn))
    return out


def run(iterations=3):
    n64 = R.N64()
    gc = R.GC()
    pairs = load_pairs()
    jt = jump_tables(n64)
    st = n64_stats(n64, jt)
    elems = []
    for p in pairs:
        G = gc_elements(gc, p['gc'])
        N = n64_elements(n64, p['n64'], st, jt)
        elems.append((p, G, N))
    accepted = {}
    votes = None
    for it in range(iterations):
        votes = defaultdict(lambda: defaultdict(list))  # sym -> (scope, base) -> [evidence]
        offs = defaultdict(lambda: defaultdict(set))  # (sym, scope, base) -> gc off -> n64 addr
        for p, G, N in elems:
            assign = assign_pair(G, N, accepted)
            for (sym, sc, base), items in assign.items():
                votes[sym][(sc, base)].append(dict(n64=p['n64'], gc=p['gc_name'], gc_file=p['gc_file'],
                                                   pair_conf=p['conf'],
                                                   items=[(g['off'], g['kind'], nn['addr'], nn['kind'], nn['mode'], how)
                                                          for g, nn, how in items]))
        # accept: best candidate per symbol with >= 2 distinct N64 functions, 2x runner-up; greedy over N64 bases
        cand = []
        for sym, d in votes.items():
            ranked = sorted(d.items(), key=lambda kv: -len(set(e['n64'] for e in kv[1])))
            top = ranked[0]
            ntop = len(set(e['n64'] for e in top[1]))
            nrun = len(set(e['n64'] for e in ranked[1][1])) if len(ranked) > 1 else 0
            cand.append((ntop, -nrun, sym, top[0], nrun))
        cand.sort(reverse=True)
        accepted = {}
        owner = {}
        for ntop, _, sym, (sc, base), nrun in cand:
            if ntop >= 2 and ntop >= 2 * nrun:
                if (sc, base) in owner:
                    continue
                owner[(sc, base)] = sym
                accepted[sym] = (sc, base)
                accepted[('n64', sc, base)] = sym
        print('iteration %d: %d symbols with votes, %d accepted (>=2 functions)' %
              (it, len(votes), sum(1 for k in accepted if not isinstance(k, tuple))), file=sys.stderr)
    return n64, gc, pairs, st, jt, votes, accepted


def main():
    n64, gc, pairs, st, jt, votes, accepted = run()
    os.makedirs(OUT, exist_ok=True)
    ser = {}
    for sym, d in votes.items():
        ser[sym] = [dict(scope=sc, base='0x%08X' % base, evidence=ev) for (sc, base), ev in
                    sorted(d.items(), key=lambda kv: -len(kv[1]))]
    json.dump(dict(pairs=len(pairs), votes=ser), open(os.path.join(OUT, 'votes.json'), 'w'), indent=0)
    print('pairs %d' % len(pairs))


if __name__ == '__main__':
    main()
