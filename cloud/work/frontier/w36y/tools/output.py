"""w36y: build map_data.json / map_data.md / renames_proposed.json from the alignment votes (tools/mapdata.py).

    python3 cloud/work/frontier/w36y/tools/output.py

Read-only except for the lane directory. Standard library only.
"""
import json
import os
import re
import sys
from collections import Counter, OrderedDict, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import mapdata as M  # noqa: E402
import refs as R  # noqa: E402

REPO = R.REPO
LANE = os.path.abspath(os.path.join(HERE, '..'))
WIDTH = {'b': 1, 'h': 2, 'w': 4, 'f': 4, 'd': 8, 'a': 1}
N64T = {'lb': 's8', 'lbu': 'u8', 'lh': 's16', 'lhu': 'u16', 'lw': 'word', 'lf': 'f32', 'ld': 'f64',
        'sb': '8-bit', 'sh': '16-bit', 'sw': 'word', 'sf': 'f32', 'sd': 'f64', 'addr': 'address'}


NOTE = ('MAT3/GC -> N64 data-symbol map. Placement channels: (1) code: aligned global references of w36n proven/strong '
        'function pairs (object-consistent assignment, iterated); (2) content: the GC object\'s initial bytes found '
        'uniquely in the N64 image (pointer words compared through their string targets). confidence: proven = '
        '>=2 distinct N64 functions with solid evidence and >=2 GC functions (objects > 8 B also need >=2 aligned '
        'offsets or >=3 functions), 2x the runner-up, no stronger rival; or code + content agreement; or exact '
        'content of >= 6 non-zero words / 2 strings. strong = one clean proven-pair function, a weaker majority, '
        'or a short content hit. lead = the rest (incl. placements inside another symbol\'s proven object). '
        'Overlay addresses are per image. field_offsets_gc_to_n64 maps the GC offsets seen in aligned accesses to '
        'N64 offsets. Not applied anywhere.')
HEADER = ('Placement channels: **code** (aligned global references of w36n proven/strong function pairs) and '
          '**content** (the GC object\'s initial bytes found uniquely in the N64 image). Precision against 109 '
          'identities other lanes stated (tools/evaluate.py): see RESULTS.md. Use proven rows as identities, strong '
          'rows as very likely, lead rows as hints. ALWAYS take field offsets from the N64 column / types.md, not from GC. '
          'A third channel adds identities other lanes stated (column `lane` in the JSON). `vol` is a heuristic '
          '(confirmed for gSeconds, gFrameSeconds, gNumCars, gNumModels). Struct layouts: types.md.\n')


def hx(v):
    return '-0x%X' % -v if v < 0 else '0x%X' % v


def current_names():
    """{(scope, addr): name} from asm/us/blob/symbols.json (+ GAME_SYMBOLS via the generated table) and the
    runtime images' symbols.json (own-range addresses only)."""
    names = {}
    s = json.load(open(os.path.join(REPO, 'asm', 'us', 'blob', 'symbols.json')))['symbols']
    for n, a in s.items():
        a = int(a, 16)
        k = ('game', a)
        if k not in names or names[k].startswith('D_'):
            names[k] = n
    for t in 'ab':
        s = json.load(open(os.path.join(REPO, 'asm', 'us', 'ovl_' + t, 'symbols.json')))['symbols']
        for n, a in s.items():
            a = int(a, 16)
            if a >= R.OVL_BASE and a < 0x80400000:
                names.setdefault(('ovl_' + t, a), n)
    try:
        sys.path.insert(0, REPO)
        from tools.conveyor.pipeline import disasm as D
        for a, n in D.GAME_SYMBOLS.items():
            names[('game', a)] = n
    except Exception as e:  # pragma: no cover
        print('GAME_SYMBOLS not loaded: %s' % e, file=sys.stderr)
    return names


def locks():
    blob = set(json.load(open(os.path.join(REPO, 'blob_matched.lock.json'))).keys())
    ovl = json.load(open(os.path.join(REPO, 'ovl_matched.lock.json')))
    matched = set('game:' + n for n in blob)
    for t in 'ab':
        for n in ovl.get(t, {}):
            matched.add('ovl_%s:%s' % (t, n))
    return matched


def volatile_like(n64, scope, addr):
    """Scalar accessed through a freshly formed address with displacement 0 (IDO volatile pattern)."""
    f0 = dr = 0
    for key, lst in n64.refs.items():
        pop = key.split(':')[0]
        for r in lst:
            if r['addr'] != addr or M.scope_of(pop, addr) != scope or r['kind'] == 'addr':
                continue
            if r['mode'] == 'field' and r['off'] == 0 and r['base'] == addr:
                f0 += 1
            elif r['mode'] == 'direct':
                dr += 1
    return f0, dr


def build():
    n64, gc, pairs, st, jt, votes, accepted = M.run()
    names = current_names()
    matched = locks()
    gj = json.load(open(os.path.join(REPO, 'docs', 'external', 'gc', 'globals.json')))['globals']
    gtype = {}
    for g in gj:
        gtype.setdefault((g['name'], int(g['address'], 16)), g)
    gfiles = defaultdict(set)
    for g in gj:
        gfiles[(g['name'], int(g['address'], 16))].add(g['file'])
    # per-address function sets (program wide)
    addr_funcs = defaultdict(set)
    for key, lst in n64.refs.items():
        pop = key.split(':')[0]
        for r in lst:
            addr_funcs[(M.scope_of(pop, r['addr']), r['addr'])].add(key)
    by_scope = defaultdict(list)
    for (sc, a) in addr_funcs:
        by_scope[sc].append(a)
    for sc in by_scope:
        by_scope[sc].sort()

    rows = []
    for key, cands in votes.items():
        sym = key.split('@0x')[0]
        start = int(key.split('@0x')[1], 16) if '@0x' in key else (gc.syms[sym][0]['address'] if sym in gc.syms else 0)
        info = gc.sym_info(sym, start) or {}
        gsize = info.get('size', 0)
        ranked = sorted(cands.items(), key=lambda kv: (-len(set(e['n64'] for e in kv[1])), kv[0][1]))
        (sc, base), ev = ranked[0]
        cluster = [(b, e) for (s2, b), e in ranked if s2 == sc and (b == base or (gsize > 8 and abs(b - base) < gsize))]
        others = [(s2, b, e) for (s2, b), e in ranked if not (s2 == sc and (b == base or (gsize > 8 and abs(b - base) < gsize)))]
        support = set(e['n64'] for b, e2 in cluster for e in e2)
        # (1) prefer the N64 address aligned with GC offset 0 (the object start) when it is well supported
        off0 = defaultdict(set)
        item_addrs = set()
        for b, e2 in cluster:
            for e in e2:
                for goff, gk, na, nk, nmode, how in e['items']:
                    item_addrs.add(na)
                    if goff == 0:
                        off0[na].add(e['n64'])
        if off0:
            a0, fs0 = max(off0.items(), key=lambda kv: (len(kv[1]), -kv[0]))
            if a0 != base and len(fs0) * 2 >= len(set(e['n64'] for e in ev)):
                base = a0
        # (2) snap a base that is misaligned for the GC object or belongs to another N64 object
        al = 4 if (gsize >= 4 and gsize % 4 == 0) else (2 if gsize % 2 == 0 and gsize >= 2 else 1)
        foreign = (sc, base) in addr_funcs and not (addr_funcs[(sc, base)] & support) and base not in item_addrs
        if (base % al or foreign) and item_addrs:
            lower = [a for a in item_addrs if a >= base]
            if lower:
                base = min(lower)
        f_top = set(e['n64'] for e in ev)
        g_top = set(e['gc'] for e in ev)
        f_all = support
        g_all = set(e['gc'] for b, e2 in cluster for e in e2)
        # functions whose evidence is more than one address formation
        f_solid = set(e['n64'] for b, e2 in cluster for e in e2
                      if len(e['items']) >= 2 or any(it[1] != 'addr' for it in e['items']))
        pair_conf = dict((e['n64'], e['pair_conf']) for b, e2 in cluster for e in e2)
        run = max([len(set(e['n64'] for e in e2)) for s2, b, e2 in others] or [0])
        # field remaps: gc off -> n64 off (distinct functions per mapping; 'nw' = order-only evidence)
        fields = defaultdict(lambda: defaultdict(dict))  # gc off -> n64 off -> {fn: weight}
        kinds = Counter()
        W = {'off': 3.0, 'obj': 1.0, 'nw': 0.5}
        for b, e2 in cluster:
            for e in e2:
                for goff, gk, na, nk, nmode, how in e['items']:
                    d = fields[goff][na - base]
                    d[e['n64']] = max(d.get(e['n64'], 0), W.get(how, 0.5))
                    if na == base:
                        kinds[nk] += 1
        remap = {}
        remap_n = {}
        for goff, c in sorted(fields.items()):
            noff, fs = max(c.items(), key=lambda kv: (sum(kv[1].values()), len(kv[1])))
            if sum(fs.values()) < 1.0:
                continue  # order-only evidence from one function
            if noff < -0x100:
                continue
            remap[goff] = noff
            remap_n[goff] = len(fs)
        diff = {g: n for g, n in remap.items() if g != n}
        gt = gtype.get((sym, start)) or {}
        rows.append(dict(key=key, sym=sym, start=start, gsize=gsize, scope=sc, base=base, f_top=f_top, g_top=g_top,
                         f_all=f_all, g_all=g_all, run=run, remap_n=remap_n, f_solid=f_solid, pair_conf=pair_conf, remap=remap, diff=diff, kinds=kinds, ev=ev,
                         gtype=gt.get('type'), gfile=sorted(gfiles.get((sym, start), [])) or [gt.get('file')],
                         gsection=info.get('section'), others=others))
    # conflicts: one N64 base claimed by several GC symbols -> keep the best supported
    claim = defaultdict(list)
    for r in rows:
        claim[(r['scope'], r['base'])].append(r)
    for k, lst in claim.items():
        lst.sort(key=lambda r: (-len(r['f_all']), -len(r['f_top'])))
        for r in lst[1:]:
            r['conflict'] = lst[0]['key']
        lst[0]['rivals'] = [r['key'] for r in lst[1:]]
    out = []
    for r in rows:
        nt, na, ns = len(r['f_top']), len(r['f_all']), len(r['f_solid'])
        rival_strong = r.get('rivals') and any(len(x['f_all']) * 2 > na for x in claim[(r['scope'], r['base'])][1:])
        if r.get('conflict'):
            conf = 'lead'
        elif ((ns >= 2 and ((r['gsize'] or 0) <= 8 or ns >= 3 or len(r['remap']) >= 2)) or
              (na >= 4 and len(r['g_all']) >= 3)) and len(r['g_all']) >= 2 and na >= 2 * r['run'] and not rival_strong:
            conf = 'proven'
        elif ns >= 2 and na > r['run']:
            conf = 'strong'
        elif ns == 1 and r['run'] == 0 and not r.get('rivals'):
            its = [it for e in r['ev'] for it in e['items']]
            exact = [it for it in its if it[5] in ('off', 'obj') and M.CLS[it[1]] == M.CLS[it[3]] and it[1] != 'addr']
            proven_pair = any(v == 'proven' for v in r['pair_conf'].values())
            conf = 'strong' if exact and proven_pair else 'lead'
        else:
            conf = 'lead'
        # N64 type at offset 0
        nk = r['kinds']
        n64_kinds = sorted(nk)
        loads = [k for k in nk if k.startswith('l')]
        n64_type = None
        if loads:
            n64_type = N64T[sorted(loads, key=lambda k: -nk[k])[0]]
        elif nk:
            n64_type = N64T[sorted(nk, key=lambda k: -nk[k])[0]]
        f0, dr = volatile_like(n64, r['scope'], r['base'])
        vol = (f0 >= 2 and f0 >= dr) if (r['gsize'] or 0) <= 8 else False
        # extent / users
        ext = max([r['gsize'] or 0] + [no + 4 for no in r['remap'].values()])
        users = set()
        for a in by_scope[r['scope']]:
            if r['base'] <= a < r['base'] + max(ext, 1):
                users |= addr_funcs[(r['scope'], a)]
        unmatched = sorted(u for u in users if u not in matched)
        cur = names.get((r['scope'], r['base']))
        evid = []
        for e in r['ev'][:6]:
            its = '; '.join('%s+0x%X %s ~ %08X %s' % (r['sym'], it[0], it[1], it[2], it[3]) for it in e['items'][:3])
            evid.append('%s = %s@%s: %s' % (e['n64'], e['gc'], e['gc_file'], its))
        row = OrderedDict(
            population=('static-data' if r['scope'] == 'game' and r['base'] < M.R.GAME_BASE else
                        ('game' if r['scope'] == 'game' else r['scope'])),
            n64_address='0x%08X' % r['base'],
            n64_symbol=cur,
            mat3_name=r['sym'],
            mat3_key=r['key'],
            mat3_file=','.join(x for x in r['gfile'] if x),
            mat3_type=r['gtype'],
            mat3_size=r['gsize'],
            mat3_section=r['gsection'],
            gc_address='0x%08X' % r['start'],
            n64_type=n64_type,
            n64_access=n64_kinds,
            n64_size_observed=ext,
            volatile=vol,
            volatile_evidence='%d per-use formed-address loads/stores vs %d folded' % (f0, dr),
            field_offsets_gc_to_n64={hx(g): hx(n) for g, n in sorted(r['remap'].items())},
            field_support={hx(g): r['remap_n'][g] for g in sorted(r['remap'])},
            layout_differs=bool(r['diff']),
            confidence=conf,
            support_functions=len(r['f_all']),
            support_functions_solid=len(r['f_solid']),
            support_gc_functions=len(r['g_all']),
            runner_up_support=r['run'],
            conflict_with=r.get('conflict'),
            rivals=r.get('rivals') or None,
            users=len(users),
            unmatched_users=len(unmatched),
            unmatched_user_names=unmatched[:12],
            evidence=evid,
            alternatives=['%s 0x%08X (%d fn)' % (s2, b, len(set(x['n64'] for x in e2))) for s2, b, e2 in r['others'][:3]],
        )
        out.append(row)
    out = merge_content(out, gc, n64, names, matched, addr_funcs, by_scope, gtype, gfiles)
    merge_harvest(out, gc, names, gtype)
    overlap_check(out)
    cap_users(out, addr_funcs, by_scope, matched)
    out.sort(key=lambda r: (r['population'] != 'static-data' and r['population'] != 'game', r['population'],
                            int(r['n64_address'], 16)))
    return out


def merge_harvest(out, gc, names, gtype):
    """Channel L: identities other lanes stated (build/harvest.json, tools/harvest.py), MAT3 names only."""
    hv = json.load(open(os.path.join(LANE, 'build', 'harvest.json')))['rows']
    byname = defaultdict(list)
    for r in out:
        byname[r['mat3_name']].append(r)
    for c in hv:
        name = c['name']
        if name not in gc.syms or c['scope'] == 'ovl':
            continue
        a = int(c['address'], 16)
        sc = c['scope']
        pop = pop_of(sc, a)
        src = 'lane: %s' % ', '.join(c['sources'][:3])
        rs = [r for r in byname.get(name, []) if r['population'] == pop or
              (pop in ('game', 'static-data') and r['population'] in ('game', 'static-data'))]
        same = [r for r in rs if int(r['n64_address'], 16) == a]
        if same:
            for r in same:
                r['lane'] = src
                r['evidence'] = r['evidence'] + [src]
                if r['confidence'] == 'strong':
                    r['confidence'] = 'proven'
                elif r['confidence'] == 'lead':
                    r['confidence'] = 'strong'
            continue
        inside = [r for r in rs if int(r['n64_address'], 16) < a < int(r['n64_address'], 16) +
                  max(r['mat3_size'] or 0, r['n64_size_observed'] or 0)]
        if inside:
            for r in inside:
                r['lane_field_claims'] = (r.get('lane_field_claims') or []) + ['0x%08X (%s)' % (a, c['sources'][0])]
            continue
        unique = len(gc.syms[name]) == 1
        if rs and unique and all(r['confidence'] != 'proven' for r in rs):
            r = rs[0]
            r['alternatives'] = ['code/content said %s %s (%s)' % (r['population'], r['n64_address'], r['confidence'])] + (
                r['alternatives'] or [])
            r['n64_address'] = '0x%08X' % a
            r['population'] = pop
            r['n64_symbol'] = names.get((sc, a))
            r['confidence'] = 'strong'
            r['lane'] = src
            r['evidence'] = [src] + r['evidence']
            continue
        if rs and any(r['confidence'] == 'proven' for r in rs) and unique:
            for r in rs:
                r['lane_disagrees'] = '0x%08X (%s)' % (a, c['sources'][0])
            continue
        # new row (statics with a shared name: the GC object is not known -> key name@lane)
        key = gc.key(name, gc.syms[name][0]['address']) if unique else '%s@lane:%s' % (name, pop)
        st0 = gc.syms[name][0]['address']
        gt = gtype.get((name, st0)) or {} if unique else {}
        out.append(OrderedDict(
            population=pop, n64_address='0x%08X' % a, n64_symbol=names.get((sc, a)), mat3_name=name, mat3_key=key,
            mat3_file=gt.get('file') or '', mat3_type=gt.get('type'),
            mat3_size=gc.syms[name][0]['size'] if unique else None, mat3_section=gc.syms[name][0]['section'],
            gc_address='0x%08X' % st0 if unique else None, n64_type=None, n64_access=[], n64_size_observed=0,
            volatile=False, volatile_evidence='', field_offsets_gc_to_n64={}, field_support={}, layout_differs=False,
            confidence='strong', support_functions=0, support_functions_solid=0, support_gc_functions=0,
            runner_up_support=0, conflict_with=None, rivals=None, users=0, unmatched_users=0,
            unmatched_user_names=[], evidence=[src], alternatives=[], lane=src))
        byname[name].append(out[-1])


def overlap_check(out):
    """A non-proven placement inside a proven object of another symbol is demoted (conflict 'inside X')."""
    import bisect
    proven = sorted((r['population'], int(r['n64_address'], 16), max(r['mat3_size'] or 0, r['n64_size_observed'] or 0),
                     r['mat3_key']) for r in out if r['confidence'] == 'proven')
    for r in out:
        if r['confidence'] == 'proven':
            continue
        a = int(r['n64_address'], 16)
        for pop, b, size, key in proven:
            if pop == r['population'] and key != r['mat3_key'] and b <= a < b + size and (b != a or True):
                r['conflict_with'] = 'inside %s (%s 0x%08X, %d B)' % (key, pop, b, size)
                r['confidence'] = 'lead'
                break


def cap_users(out, addr_funcs, by_scope, matched):
    """Recount users with each object's extent capped at the next placed object of the same population."""
    rows = sorted([r for r in out if r['confidence'] in ('proven', 'strong')],
                  key=lambda r: (r['population'], int(r['n64_address'], 16)))
    for r in out:
        if r['confidence'] not in ('proven', 'strong'):
            r['n64_extent_capped'] = max(r['mat3_size'] or 0, 1)
    for i, r in enumerate(rows):
        a = int(r['n64_address'], 16)
        size = max(r['mat3_size'] or 0, r['n64_size_observed'] or 0, 1)
        nxt = None
        for r2 in rows[i + 1:]:
            if r2['population'] != r['population']:
                break
            b2 = int(r2['n64_address'], 16)
            if b2 > a:
                nxt = b2
                break
        end = a + size if nxt is None else min(a + size, nxt)
        sc = 'game' if r['population'] in ('game', 'static-data') else r['population']
        users = set()
        for x in by_scope[sc]:
            if a <= x < end:
                users |= addr_funcs[(sc, x)]
        unmatched = sorted(u for u in users if u not in matched)
        r['users'], r['unmatched_users'], r['unmatched_user_names'] = len(users), len(unmatched), unmatched[:12]
        r['n64_extent_capped'] = end - a


def file_populations():
    """{GC file: set(populations)} from the w36n function map (proven/strong rows)."""
    m = json.load(open(os.path.join(REPO, 'cloud', 'work', 'frontier', 'w36n', 'map_functions.json')))
    fp = defaultdict(Counter)
    for r in m['rows']:
        if r['confidence'] in ('proven', 'strong') and r.get('mat3_file'):
            fp[r['mat3_file']][r['population']] += 1
    return {f: set(p for p, n in c.items() if n * 5 >= sum(c.values())) for f, c in fp.items()}


def pop_of(scope, a):
    if scope == 'game':
        return 'static-data' if a < R.GAME_BASE else 'game'
    return scope


def merge_content(out, gc, n64, names, matched, addr_funcs, by_scope, gtype, gfiles):
    import content as C
    hits = json.load(open(os.path.join(LANE, 'build', 'content_hits.json')))['rows']
    fpop = file_populations()
    byk = {}
    for r in out:
        byk[r['mat3_key']] = r
    for h in hits:
        sym, start = h['name'], int(h['gc_address'], 16)
        key = gc.key(sym, start)
        want = set()
        for f in h['files']:
            want |= fpop.get(f, set())
        cands = []
        for sc, lst in h['hits'].items():
            for a in lst:
                a = int(a, 16)
                if not want or sc in want or (sc == 'game' and ('game' in want or 'static' in want)):
                    cands.append((sc, a))
        if len(cands) != 1:
            continue
        sc, a = cands[0]
        chk = C.check(start, h['size'], sc, a) or (0, 0, 0)
        cstat = 'content: %d/%d non-zero words equal (%d string pointers same text), unique in %s' % (
            chk[0], chk[1], chk[2], sc)
        strong_content = chk[1] >= 6 and chk[0] == chk[1] or chk[2] >= 2
        r = byk.get(key)
        if r is not None:
            if r['population'] in ('ovl_a', 'ovl_b'):
                rsc = r['population']
            else:
                rsc = 'game'
            if rsc == sc and int(r['n64_address'], 16) == a:
                r['content'] = cstat
                if r['confidence'] == 'strong' or (r['confidence'] == 'lead' and not r['conflict_with']):
                    r['confidence'] = 'proven' if r['confidence'] == 'strong' or strong_content else 'strong'
                continue
            # content disagrees with the code alignment
            note = 'code alignment said %s %s (%s, %d fn)' % (r['population'], r['n64_address'], r['confidence'],
                                                              r['support_functions'])
            if r['confidence'] == 'proven' and not strong_content:
                r['content'] = 'content hit elsewhere: %s 0x%08X (%s); kept code placement' % (sc, a, cstat)
                continue
            r['alternatives'] = [note] + (r['alternatives'] or [])
            r['n64_address'] = '0x%08X' % a
            r['population'] = pop_of(sc, a)
            r['n64_symbol'] = names.get((sc, a))
            r['content'] = cstat
            r['confidence'] = 'proven' if strong_content else 'strong'
            r['evidence'] = ['content: ' + cstat] + r['evidence']
            continue
        gt = gtype.get((sym, start)) or {}
        info = gc.sym_info(sym, start) or {}
        users = set()
        for x in by_scope[sc]:
            if a <= x < a + max(h['size'], 1):
                users |= addr_funcs[(sc, x)]
        unmatched = sorted(u for u in users if u not in matched)
        out.append(OrderedDict(
            population=pop_of(sc, a), n64_address='0x%08X' % a, n64_symbol=names.get((sc, a)), mat3_name=sym,
            mat3_key=key, mat3_file=','.join(h['files']), mat3_type=gt.get('type'), mat3_size=h['size'],
            mat3_section=info.get('section'), gc_address=h['gc_address'], n64_type=None, n64_access=[],
            n64_size_observed=h['size'], volatile=False, volatile_evidence='', field_offsets_gc_to_n64={},
            layout_differs=False, confidence='proven' if strong_content else 'strong', support_functions=0,
            support_functions_solid=0, support_gc_functions=0, runner_up_support=0, conflict_with=None, rivals=None,
            users=len(users), unmatched_users=len(unmatched), unmatched_user_names=unmatched[:12],
            evidence=['content: ' + cstat], alternatives=[], content=cstat))
    return out


def main():
    rows = build()
    final = os.environ.get('W36Y_FINAL')
    json.dump(dict(generated_by='cloud/work/frontier/w36y/tools/output.py (w36y, wave 36)',
                   note='MAT3/GC -> N64 data-symbol map from aligned global references of w36n proven/strong '
                        'function pairs. confidence: proven = >=2 distinct N64 functions (and >=2 GC functions) '
                        'vote the same base, 2x the runner-up, no stronger rival; strong = one clean function or '
                        'a weaker majority; lead = otherwise. Not applied anywhere.',
                   rows=rows), open(os.path.join(LANE, 'build', 'map_data_auto.json'), 'w'), indent=1)
    c = Counter((r['population'], r['confidence']) for r in rows)
    print(sorted(c.items()))
    if final:
        json.dump(dict(generated_by='cloud/work/frontier/w36y/tools/output.py (w36y, wave 36)',
                       note=NOTE, rows=rows), open(os.path.join(LANE, 'map_data.json'), 'w'), indent=1)
        write_md(rows, os.path.join(LANE, 'map_data.md'), HEADER)



def write_md(rows, path, header_extra=''):
    def fo(r):
        d = r['field_offsets_gc_to_n64']
        diffs = ['%s->%s' % (g, n) for g, n in d.items() if g != n]
        return (', '.join(diffs[:4]) + (' ...' if len(diffs) > 4 else '')) if diffs else ''

    L = []
    L.append('# MAT3 (GameCube) -> N64 data-symbol map (lane w36y, wave 36)')
    L.append('')
    L.append('Generated by `cloud/work/frontier/w36y/tools/output.py`; machine-readable rows in `map_data.json`. '
             'Not applied anywhere. Overlay addresses (0x8038A400..) are per image (population ovl_a / ovl_b).')
    L.append('')
    L.append(header_extra)
    c = Counter((r['population'], r['confidence']) for r in rows)
    L.append('| population | proven | strong | lead |')
    L.append('|---|---|---|---|')
    for p in ('static-data', 'game', 'ovl_a', 'ovl_b'):
        L.append('| %s | %d | %d | %d |' % (p, c[(p, 'proven')], c[(p, 'strong')], c[(p, 'lead')]))
    L.append('')
    L.append('Columns: `fn` = distinct N64 functions whose aligned references vote this base (GC functions in '
             'parentheses), `unm` = unmatched N64 functions that reference the object, `vol` = IDO volatile '
             'pattern (per-use formed address + 0(reg) access), `layout` = GC field offset -> N64 offset where they '
             'differ (from aligned accesses).')
    L.append('')
    L.append('## High value: proven/strong objects referenced by the most unmatched N64 functions')
    L.append('')
    L.append('| N64 address | current | MAT3 name | MAT3 type | N64 access | vol | conf | fn | unm | layout differs |')
    L.append('|---|---|---|---|---|---|---|---|---|---|')
    hv = sorted([r for r in rows if r['confidence'] in ('proven', 'strong') and r['unmatched_users']],
                key=lambda r: -r['unmatched_users'])[:60]
    for r in hv:
        L.append('| %s %s | %s | **%s** | `%s` | %s | %s | %s | %d (%d) | %d | %s |' % (
            r['population'], r['n64_address'], r['n64_symbol'] or '', r['mat3_name'], r['mat3_type'] or '',
            '/'.join(r['n64_access']), 'V' if r['volatile'] else '', r['confidence'], r['support_functions'],
            r['support_gc_functions'], r['unmatched_users'], fo(r)))
    L.append('')
    for pop in ('static-data', 'game', 'ovl_a', 'ovl_b'):
        L.append('## %s (by address)' % pop)
        L.append('')
        L.append('| N64 address | current | MAT3 name | MAT3 file | MAT3 type (size) | N64 access | vol | conf | fn | unm | '
                 'layout differs | first evidence |')
        L.append('|---|---|---|---|---|---|---|---|---|---|---|---|')
        for r in rows:
            if r['population'] != pop:
                continue
            ev = r['evidence'][0] if r['evidence'] else ''
            L.append('| %s | %s | %s | %s | `%s` (%s) | %s | %s | %s | %d (%d) | %d | %s | %s |' % (
                r['n64_address'], r['n64_symbol'] or '', r['mat3_name'], r['mat3_file'], r['mat3_type'] or '',
                r['mat3_size'], '/'.join(r['n64_access']), 'V' if r['volatile'] else '', r['confidence'] +
                (' (conflict: %s)' % r['conflict_with'] if r['conflict_with'] else ''),
                r['support_functions'], r['support_gc_functions'], r['unmatched_users'], fo(r),
                ev.replace('|', '\\|')[:160]))
        L.append('')
    open(path, 'w').write('\n'.join(L) + '\n')


if __name__ == '__main__':
    main()
