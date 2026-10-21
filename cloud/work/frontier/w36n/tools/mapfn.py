#!/usr/bin/env python3
"""w36n: build the MAT3 (GC) -> N64 function map by anchor propagation (read-only analysis).

  python3 cloud/work/frontier/w36n/tools/mapfn.py     # writes ../map_functions.json, ../map_functions.md,
                                                      #        ../renames_proposed.json
Evidence items are (kind, level, source, text); level 3 = proven-grade, 2 = strong-grade, 1 = lead-grade.
  matched   N64 function is locked/grouped C ported from that GC function (level 3 when it is in the lock)
  applied   names.md row (GC name applied by w29n/w30n/w32n with its evidence; level 3 if now matched)
  stated    identity stated by a lane/source with evidence (harvest.json; level 2, or 1 for leads)
  corr      correlation.json confident pair (level 1: strings/imm/calls/size scorer)
  callee    call-graph alignment inside an accepted caller pair: the N64 callee is the only unknown in the
            gap where the GC callee is the only unknown; gap bounded by aligned known pairs on both sides =
            caller's level, open at one end = caller's level - 1; both bounds and caller proven = level 3
  confirm   the same test for an already known pair (independent re-derivation from a caller)
  calleepos same number (2-4) of distinct unknowns with the same call pattern in one gap: positional, level 1
  order     file order: between two strong+ anchors of one GC file, N64 functions in address order map one to
            one onto the GC functions in source-line order (stripped GC functions included), counts equal and
            every size ratio plausible: level 2
  string    a string literal used by exactly one function on each side: level 2
Final confidence: proven = a level-3 item, or two evidence groups at level >= 2 (groups: solid, callee/confirm,
order, string), or callee/confirm from >= 2 distinct callers at level >= 2, or corr + a level-2 group;
strong = any level-2 item; lead = otherwise. Size ratio N64/GC outside [0.3, 4.5] (both > 16 B) caps at strong.
"""
import json
import os
import re
import sys
from collections import OrderedDict, defaultdict, Counter

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
from data import (REPO, n64_population, load_gc, gc_features, gc_file_order, GAME_BASE)  # noqa: E402
import anchors  # noqa: E402

LANE = os.path.dirname(HERE)
LV = {1: 'lead', 2: 'strong', 3: 'proven'}
GROUP = {'matched': 'solid', 'applied': 'solid', 'stated': 'solid', 'callee': 'callee', 'confirm': 'callee',
         'calleepos': 'pos', 'order': 'order', 'sig': 'sig', 'stub': 'order', 'struct': 'sig', 'string': 'string', 'corr': 'corr'}

N64_PLATFORM = re.compile(r'^static:(os|__os|gu|al|n_al|__ll|__d_|__f_|__ul|bzero|bcopy|__|_Printf|_Litob|_Ldtob)')


def is_platform_n64(t):
    return t.startswith('?:') or bool(N64_PLATFORM.match(t))


def size_ok(ns, gs):
    if not ns or not gs or ns <= 16 or gs <= 16:
        return True
    r = float(ns) / gs
    return 0.3 <= r <= 4.5


class Mapper(object):
    def __init__(self):
        self.imgs, self.static, self.pop = n64_population()
        self.calls = self._calls()
        self.gc = load_gc()
        self.order = gc_file_order(self.gc)
        # wrappers N64 inlines (HAL_SetFont = _GetSemaphore; _HAL_SetFont; _ReleaseSemaphore): a GC call to the
        # wrapper also matches an N64 call to its single real callee
        self.alias = {}
        for k, v in self.gc.items():
            if not v['gamesource'] or v['stripped'] or v['size'] > 160:
                continue
            real = set(t for pc, t, kk in v['calls'] if t in self.gc and self.gc[t]['gamesource'] and
                       not t.startswith('_GetSemaphore') and not t.startswith('_ReleaseSemaphore'))
            if len(real) == 1:
                self.alias[k] = real
        self.pos = {}
        for f, ks in self.order.items():
            for i, k in enumerate(ks):
                self.pos[k] = (f, i)
        self.ev = defaultdict(list)
        self.m, self.inv, self.conf = {}, {}, {}
        self.gcallers = defaultdict(set)
        for k, v in self.gc.items():
            for pc, t, kk in v['calls']:
                self.gcallers[t].add(k)
        self.ncallers = defaultdict(set)
        for f, seq in self.calls.items():
            for pc, t, kk in seq:
                self.ncallers[t].add(f)
        self.byname = defaultdict(list)
        for i, v in self.pop.items():
            self.byname[v['name']].append(i)
        # static functions that are called: add to the population (no size; matched unknown)
        sa = {}
        for seq in self.calls.values():
            for pc, t, k in seq:
                if t.startswith('static:') and t not in self.pop:
                    sa[t] = True
        inv_static = {('static:' + n): a for a, n in self.static.items()}
        for t in sa:
            a = inv_static.get(t)
            if a is None and re.match(r'static:func_[0-9A-F]{8}$', t):
                a = int(t[-8:], 16)
            self.pop[t] = dict(pop='static', name=t.split(':', 1)[1], address=a or 0, size=0, matched=None)

    def _calls(self):
        from data import n64_calls
        calls = n64_calls(self.imgs, self.static)
        a, b = self.imgs['ovl_a'].by_addr, self.imgs['ovl_b'].by_addr
        for f, seq in calls.items():
            for i, (pc, t, k) in enumerate(seq):
                if t.startswith('?:'):
                    ad = int(t[2:], 16)
                    ina, inb = ad in a, ad in b
                    if ina and not inb:
                        seq[i] = (pc, 'ovl_a:' + a[ad], k)
                    elif inb and not ina:
                        seq[i] = (pc, 'ovl_b:' + b[ad], k)
        return calls

    # ------------------------------------------------------------------------------------ helpers
    def nid(self, name, pop=None):
        ids = self.byname.get(name, [])
        if pop:
            ids = [i for i in ids if i.startswith(pop + ':')]
        return ids[0] if len(ids) == 1 else None

    def add(self, n, g, kind, level, src, text):
        if g not in self.gc:
            print('unknown GC key %s (for %s)' % (g, n))
            return False
        item = (kind, level, src, text)
        if item not in self.ev[(n, g)]:
            self.ev[(n, g)].append(item)
            return True
        return False

    def nsize(self, n):
        return self.pop.get(n, {}).get('size', 0)

    def gsize(self, g):
        return self.gc[g]['size']

    # ------------------------------------------------------------------------------------ anchors
    def load_anchors(self):
        used = set(t for seq in self.calls.values() for _, t, _ in seq)
        for name in anchors.SEEDS:
            n = 'static:' + name
            if name in self.gc and n in used:
                self.add(n, name, 'applied', 2, '', 'libc/libm/inflate seed: same-named library function')
        for name, (g, text) in anchors.APPLIED.items():
            n = self.nid(name, 'game')
            if n is None:
                print('anchor: no N64 function', name)
                continue
            lv = 3 if self.pop[n]['matched'] and 'matched' in text else 2
            self.add(n, g, 'applied', lv, '', text)
        hp = os.path.join(LANE, 'harvest.json')
        if os.path.exists(hp):
            for r in json.load(open(hp))['rows']:
                n = '%s:%s' % (r['population'], r['n64_name'])
                if n not in self.pop:
                    print('harvest: no N64 function', n)
                    continue
                kind = r['kind']
                if kind == 'matched':
                    lv = 3 if self.pop[n]['matched'] else 2
                elif kind == 'lanematch':
                    kind, lv = 'matched', 3
                elif kind == 'lead':
                    kind, lv = 'stated', 1
                else:
                    lv = 2
                self.add(n, r['gc_key'], kind, lv, '', '%s [%s]' % (r['evidence'], r['where']))
        corr = json.load(open(os.path.join(REPO, 'docs', 'external', 'gc', 'correlation.json')))
        self.corr_rows = {}
        for r in corr['rows']:
            n = ('game:' + r['n64']) if r['image'] == 'game' else '%s:%s' % (r['image'], r['n64'].split(':', 1)[1])
            self.corr_rows[n] = r
            if r['confident'] and not r['seed'] and r['confident'] in self.gc:
                c = [x for x in r['candidates'] if x['gc'] == r['confident']]
                self.add(n, r['confident'], 'corr', 1, '',
                         'correlation conf %.2f: %s' % (c[0]['score'], c[0]['evidence']) if c else 'conf')
        self.load_strings()

    def load_strings(self):
        """string channel: a string used by exactly one N64 function (direct) and one GC function."""
        from tools.gcref import correlate as C
        images, by_addr = C.load_n64(REPO, REPO)
        ext = {ad: n for ad, n in by_addr.items() if not n.startswith('D_')}
        nstr = defaultdict(set)
        for img in images:
            names = dict(ext)
            if img.tag != 'game':
                names.update({ad: n for ad, s, n in images[0].funcs})
            for name, f in C.n64_features(img, names).items():
                nid = ('game:' + name) if img.tag == 'game' else name
                for s in f['strings']:
                    if C.string_info(s) >= 0.8 and len(s) >= 5:
                        nstr[s].add(nid)
        gf = gc_features()
        gstr = defaultdict(set)
        name2key = defaultdict(list)
        for k, v in self.gc.items():
            if v['address'] is not None:
                name2key[(v['address'])].append(k)
        for gname, f in gf.items():
            ks = name2key.get(int(f['address'], 16))
            if not ks:
                continue
            for s in f['strings']:
                gstr[s].add(ks[0])
        self.nstr, self.gstr = nstr, gstr
        for s, ns in nstr.items():
            gs = gstr.get(s)
            if gs and len(ns) == 1 and len(gs) == 1:
                n, g = list(ns)[0], list(gs)[0]
                if n in self.pop and self.gc[g]['gamesource']:
                    self.add(n, g, 'string', 2, s, 'unique string %r' % s[:60])

    # -------------------------------------------------------------------------------- consistency
    def level_of(self, n, g):
        evs = self.ev[(n, g)]
        grp = defaultdict(int)
        callers = set()
        for kind, lv, src, text in evs:
            gname = GROUP[kind]
            grp[gname] = max(grp[gname], lv)
            if gname == 'callee' and lv >= 2:
                callers.add(src)
        top = max(grp.values()) if grp else 0
        strong_groups = [x for x, lv in grp.items() if lv >= 2 and x not in ('pos', 'corr')]
        if top >= 3 or len(strong_groups) >= 2 or len(callers) >= 2 or ('corr' in grp and strong_groups):
            c = 3
        elif top >= 2:
            c = 2
        else:
            c = 1
        if c == 3 and not size_ok(self.nsize(n), self.gsize(g)) and not grp.get('solid', 0) >= 3:
            c = 2
        if 0 < self.gsize(g) <= 16 and self.nsize(n) > 64 and not grp.get('solid', 0) >= 2:
            # the GC body is a stub: position evidence only (strong at most; lead for a big N64 body)
            c = min(c, 1 if self.nsize(n) > 256 else 2)
        if self.cross_pop(n, g) and not grp.get('solid', 0) >= 2:
            c = min(c, 1)
        # inline-shift guard: n's mapped callers are callers of g's callers, never of g -> n is g's caller with g
        # inlined (or another function); not an identity above lead without cited evidence
        if c >= 2 and not grp.get('solid', 0) >= 2 and self.inline_shift(n, g):
            c = 1
        # locality guard: a pair resting on one caller's gap alone, far from every anchor of its GC file, is a lead
        # (N64 regroups some functions -- e.g. all screens' blit callbacks -- so distance alone never rejects)
        if len(callers) < 2 and not any(grp.get(x, 0) >= 2 for x in ('solid', 'sig', 'string', 'order')):
            if self.far(n, g):
                c = min(c, 1)
        return c

    def inline_shift(self, n, g):
        mc = set(self.m[c] for c in self.ncallers.get(n, ()) if c in self.m)
        if not mc:
            return False
        gcs = self.gcallers.get(g, set())
        if mc & gcs:
            return False
        up = set()
        for c in gcs:
            up |= self.gcallers.get(c, set())
        return bool(mc & up)

    def cross_pop(self, n, g):
        """GC static whose mapped GC callers all sit in another N64 population than n."""
        if self.gc[g].get('scope') != 'static' or n not in self.pop:
            return False
        pops = set()
        for c in self.gcallers.get(g, ()):
            x = self.inv.get(c)
            if x and x in self.pop:
                pops.add(self.pop[x]['pop'])
        return bool(pops) and self.pop[n]['pop'] not in pops

    def far(self, n, g):
        """N64 address farther than 0x8000 from every strong anchor of the GC file in this population."""
        p = self.pop.get(n)
        if p is None or p['pop'] == 'static' or not getattr(self, 'anchor_addrs', None):
            return False
        lst = self.anchor_addrs.get((p['pop'], self.gc[g]['file']))
        if not lst or len([a for a in lst if a != p['address']]) < 2:
            return False
        return min(abs(a - p['address']) for a in lst if a != p['address']) > 0x8000

    def compute_spans(self):
        sp = {}
        for n, g in self.m.items():
            if self.conf.get(n, 0) < 2:
                continue
            p = self.pop.get(n)
            if p is None or p['pop'] == 'static':
                continue
            if not any(GROUP[k] in ('solid', 'callee') and lv >= 2 for k, lv, s_, t in self.ev[(n, g)]):
                continue
            k = (p['pop'], self.gc[g]['file'])
            a = p['address']
            if k in sp:
                sp[k] = (min(sp[k][0], a), max(sp[k][1], a), sp[k][2] + 1)
            else:
                sp[k] = (a, a, 1)
        self.spans = sp
        aa = defaultdict(list)
        for n, g in self.m.items():
            if self.conf.get(n, 0) < 2 or n not in self.pop or self.pop[n]['pop'] == 'static':
                continue
            if any(GROUP[k] in ('solid', 'callee') and lv >= 2 for k, lv, s_, t in self.ev[(n, g)]):
                aa[(self.pop[n]['pop'], self.gc[g]['file'])].append(self.pop[n]['address'])
        self.anchor_addrs = aa

    def score_of(self, n, g):
        evs = self.ev[(n, g)]
        grp = defaultdict(int)
        callers = set()
        for kind, lv, src, text in evs:
            grp[GROUP[kind]] = max(grp[GROUP[kind]], lv)
            if GROUP[kind] in ('callee', 'pos'):
                callers.add(src)
        s = sum(grp.values()) * 10 + len(callers)
        if grp.get('solid', 0) >= 2:
            s += 100
        return s

    def settle(self):
        cand = {k: self.score_of(*k) for k in self.ev if self.ev[k]}
        byn, byg = defaultdict(list), defaultdict(list)
        for (n, g), s in cand.items():
            byn[n].append((s, g))
            byg[g].append((s, n))
        self.m, self.inv, self.conf, self.conflicts, self.rejected = {}, {}, {}, [], []
        # greedy matching by score: a pair is taken when both sides are free and no other pair involving a free
        # side has the same score (ties are reported, not guessed)
        order = sorted(cand.items(), key=lambda x: -x[1])
        for (n, g), s in order:
            if n in self.m or g in self.inv:
                continue
            tie_n = [x for (sx, x) in byn[n] if sx == s and x != g and x not in self.inv]
            tie_g = [x for (sx, x) in byg[g] if sx == s and x != n and x not in self.m]
            if tie_n or tie_g:
                self.conflicts.append((n, g, sorted(byn[n], reverse=True)[:3], sorted(byg[g], reverse=True)[:3]))
                continue
            self.m[n], self.inv[g] = g, n
        for n, g in self.m.items():
            self.conf[n] = self.level_of(n, g)
        self.compute_spans()
        for n, g in list(self.m.items()):
            self.conf[n] = self.level_of(n, g)
            if self.conf[n] == 0:
                self.rejected.append((n, g, 'static GC function far outside its file span'))
                del self.m[n]
                del self.inv[g]
                del self.conf[n]

    # ------------------------------------------------------------------------------- propagation
    def align(self, n, g):
        A = [t for pc, t, k in self.calls.get(n, [])]
        B = [t for pc, t, k in self.gc[g]['calls']]
        if not A or not B:
            return None
        la, lb = len(A), len(B)
        S = [[0] * (lb + 1) for _ in range(la + 1)]
        P = [[None] * (lb + 1) for _ in range(la + 1)]
        for i in range(1, la + 1):
            ai = self.m.get(A[i - 1])
            Si, Sp, Pi = S[i], S[i - 1], P[i]
            for j in range(1, lb + 1):
                best, bp = Sp[j], 'u'
                if Si[j - 1] > best:
                    best, bp = Si[j - 1], 'l'
                if ai is not None and (ai == B[j - 1] or ai in self.alias.get(B[j - 1], ())) and Sp[j - 1] + 3 > best:
                    best, bp = Sp[j - 1] + 3, 'd'
                Si[j], Pi[j] = best, bp
        i, j, pairs = la, lb, []
        while i > 0 and j > 0:
            bp = P[i][j]
            if bp == 'd':
                pairs.append((i - 1, j - 1))
                i, j = i - 1, j - 1
            elif bp == 'u':
                i -= 1
            else:
                j -= 1
        pairs.reverse()
        return A, B, pairs

    def _unk_n(self, t, n, allow=None):
        return t != n and not is_platform_n64(t) and (t not in self.m or t == allow)

    def _unk_g(self, t, g, allow=None):
        v = self.gc.get(t)
        return t != g and v is not None and v['gamesource'] and (t not in self.inv or t == allow)

    def propagate_callees(self):
        new = 0
        for n, g in list(self.m.items()):
            cl = self.conf.get(n, 1)
            r = self.align(n, g)
            if not r:
                continue
            A, B, pairs = r
            bounds = [(-1, -1)] + pairs + [(len(A), len(B))]
            nm, gm = n.split(':', 1)[1], g

            def lab(i):
                return A[i].split(':', 1)[1] if 0 <= i < len(A) else ('start' if i < 0 else 'end')

            def emit(x, y, i0, i1, kind):
                closed = i0 >= 0 and i1 < len(A)
                if closed:
                    lv = cl
                    if cl == 3 and self.conf.get(A[i0]) == 3 and self.conf.get(A[i1]) == 3:
                        lv = 3
                    elif lv == 3:
                        lv = 2
                else:
                    lv = max(1, cl - 1)
                if not size_ok(self.nsize(x), self.gsize(y)):
                    lv = min(lv, 1)
                txt = 'caller %s=%s: only unknown between %s and %s (%s gap)' % (
                    nm, gm, lab(i0), lab(i1), 'closed' if closed else 'open')
                return self.add(x, y, kind, lv, n, txt)

            # unknown gaps
            for (i0, j0), (i1, j1) in zip(bounds, bounds[1:]):
                ua = [t for t in A[i0 + 1:i1] if self._unk_n(t, n)]
                ub = [t for t in B[j0 + 1:j1] if self._unk_g(t, g)]
                sa, sb = list(OrderedDict.fromkeys(ua)), list(OrderedDict.fromkeys(ub))
                if not sa or not sb:
                    continue
                if len(sa) == 1 and len(sb) == 1:
                    new += emit(sa[0], sb[0], i0, i1, 'callee')
                elif len(sa) == len(sb) and len(sa) <= 4:
                    if [sa.index(t) for t in ua] == [sb.index(t) for t in ub]:
                        for x, y in zip(sa, sb):
                            new += self.add(x, y, 'calleepos', 1, n, 'caller %s=%s: positional (%d unknowns, same pattern)'
                                            % (nm, gm, len(sa)))
            # confirmations of known pairs: drop one aligned pair, test its gap
            for k in range(len(pairs)):
                (ia, jb) = pairs[k]
                x, y = A[ia], B[jb]
                i0, j0 = pairs[k - 1] if k > 0 else (-1, -1)
                i1, j1 = pairs[k + 1] if k + 1 < len(pairs) else (len(A), len(B))
                ua = set(t for t in A[i0 + 1:i1] if self._unk_n(t, n, allow=x))
                ub = set(t for t in B[j0 + 1:j1] if self._unk_g(t, g, allow=y))
                if ua == {x} and ub == {y}:
                    new += emit(x, y, i0, i1, 'confirm')
        return new

    def gexp(self, g, ncalls=frozenset()):
        """GC callee sequence with same-file statics expanded one level when the N64 side does not call them
        (inlined on N64)."""
        out = []
        f = self.gc[g]['file']
        for pc, t, k in self.gc[g]['calls']:
            v = self.gc.get(t)
            if v is not None and v['file'] == f and v['scope'] == 'static' and t != g and \
                    not (t in self.inv and self.inv[t] in ncalls):
                sub = [tt for pc2, tt, k2 in v['calls']]
                if sub:
                    out.extend(sub)
                    continue
            out.append(t)
        return out

    def seqsim(self, n, g):
        """ordered callee similarity: LCS of n's mapped callees (as GC keys) and g's callee sequence (best of: as is,
        and with same-file statics expanded where N64 may have inlined them), over the longer of the two
        (GameSource callees only). Returns (sim, lcs, len_n, len_g)."""
        seq = self.calls.get(n, [])
        ncalls = frozenset(t for pc, t, k in seq)
        A = [self.m[t] for pc, t, k in seq if t in self.m]
        best = (0.0, 0, len(A), 0)
        if len(A) < 2:
            return best
        for Bsrc in ([t for pc, t, k in self.gc[g]['calls']], self.gexp(g, ncalls)):
            B = [t for t in Bsrc if t in self.inv or (self.gc.get(t) and self.gc[t]['gamesource'])]
            if len(B) < 2:
                continue
            prev = [0] * (len(B) + 1)
            for a in A:
                cur = [0] * (len(B) + 1)
                for j, b in enumerate(B, 1):
                    cur[j] = prev[j - 1] + 1 if (a == b or a in self.alias.get(b, ())) else \
                        (prev[j] if prev[j] > cur[j - 1] else cur[j - 1])
                prev = cur
            l = prev[-1]
            sim = float(l) / max(len(A), len(B))
            if sim > best[0]:
                best = (sim, l, len(A), len(B))
        return best

    def propagate_struct_global(self):
        """unmapped N64 function with >= 4 mapped calls: the unmapped GC function (sharing >= 3 mapped callees) whose
        ordered callee sequence matches best, unique (next <= 0.8 x best), sim >= 0.75, sizes plausible."""
        new = 0
        idx = defaultdict(set)
        for k, v in self.gc.items():
            if not v['gamesource'] or v['stripped'] or k in self.inv:
                continue
            for pc, t, kk in v['calls']:
                if t in self.inv:
                    idx[t].add(k)
            for t in self.gexp(k):
                if t in self.inv:
                    idx[t].add(k)
        for n, seq in self.calls.items():
            if n in self.m or n not in self.pop or self.pop[n]['pop'] == 'static':
                continue
            ms = [self.m[t] for pc, t, k in seq if t in self.m]
            if len(ms) < 4:
                continue
            cnt = Counter()
            for t in set(ms):
                for k in idx.get(t, ()):
                    cnt[k] += 1
            sc = []
            for k, c in cnt.items():
                if c < 3 or not size_ok(self.nsize(n), self.gsize(k)):
                    continue
                sim, l, la, lb = self.seqsim(n, k)
                if sim >= 0.5:
                    sc.append((sim, l, la, lb, k))
            sc.sort(reverse=True)
            if not sc or sc[0][0] < 0.75 or sc[0][1] < 4:
                continue
            if len(sc) > 1 and sc[1][0] > 0.8 * sc[0][0]:
                continue
            sim, l, la, lb, k = sc[0]
            lv = 2
            if self.far(n, k) or self.cross_pop(n, k):
                lv = 1
            new += self.add(n, k, 'struct', lv, '', 'ordered callee structure (global search): %d of %d/%d calls align '
                            '(sim %.2f; next best %.2f)' % (l, la, lb, sim, sc[1][0] if len(sc) > 1 else 0))
        return new

    def propagate_struct(self):
        """(a) confirm accepted pairs by ordered callee structure; (b) resolve multi-unknown caller gaps by it."""
        new = 0
        for n, g in list(self.m.items()):
            if n not in self.pop or self.pop[n]['pop'] == 'static':
                continue
            sim, l, la, lb = self.seqsim(n, g)
            if sim >= 0.7 and l >= 5:
                new += self.add(n, g, 'struct', 2, '', 'ordered callee structure: %d of %d/%d calls align (GC statics expanded '
                                'where N64 inlined them; sim %.2f)' % (l, la, lb, sim))
        for n, g in list(self.m.items()):
            if self.conf.get(n, 0) < 2:
                continue
            r = self.align(n, g)
            if not r:
                continue
            A, B, pairs = r
            bounds = [(-1, -1)] + pairs + [(len(A), len(B))]
            for (i0, j0), (i1, j1) in zip(bounds, bounds[1:]):
                sa = list(OrderedDict.fromkeys(t for t in A[i0 + 1:i1] if self._unk_n(t, n)))
                sb = list(OrderedDict.fromkeys(t for t in B[j0 + 1:j1] if self._unk_g(t, g)))
                if len(sa) < 2 and len(sb) < 2 or not sa or not sb or len(sa) > 8 or len(sb) > 8:
                    continue
                sc = {}
                for x in sa:
                    for y in sb:
                        sim, l, la, lb = self.seqsim(x, y)
                        if sim >= 0.6 and l >= 4 and size_ok(self.nsize(x), self.gsize(y)):
                            sc[(x, y)] = (sim, l, la, lb)
                for (x, y), (sim, l, la, lb) in sc.items():
                    bx = max(v[0] for (xx, yy), v in sc.items() if xx == x)
                    by = max(v[0] for (xx, yy), v in sc.items() if yy == y)
                    ox = [v[0] for (xx, yy), v in sc.items() if xx == x and yy != y]
                    oy = [v[0] for (xx, yy), v in sc.items() if yy == y and xx != x]
                    if sim < bx or sim < by or any(o > 0.8 * sim for o in ox + oy):
                        continue
                    lv = min(self.conf.get(n, 1), 2)
                    new += self.add(x, y, 'callee', lv, n, 'caller %s=%s: in the gap between %s and %s, the unknown whose own '
                                    'calls match (struct sim %.2f, %d calls)' % (n.split(':', 1)[1], g,
                                    A[i0].split(':', 1)[1] if i0 >= 0 else 'start',
                                    A[i1].split(':', 1)[1] if i1 < len(A) else 'end', sim, l))
                    new += self.add(x, y, 'struct', 2, '', 'ordered callee structure: %d of %d/%d calls align (sim %.2f)'
                                    % (l, la, lb, sim))
        return new

    def propagate_signature(self):
        """callee-set signature: an unknown N64 function whose mapped callees (>= 3 distinct) match the callee set
        of exactly one unknown GC function (Jaccard over mapped callees >= 0.6, next best <= 0.6 x best)."""
        new = 0
        gsets = {}
        for k, v in self.gc.items():
            if not v['gamesource'] or v['stripped'] or k in self.inv:
                continue
            cs = set(t for pc, t, kk in v['calls'] if t in self.inv)
            if len(cs) >= 3:
                gsets[k] = cs
        idx = defaultdict(set)
        for k, cs in gsets.items():
            for t in cs:
                idx[t].add(k)
        for n, seq in self.calls.items():
            if n in self.m or n not in self.pop:
                continue
            ms = set(self.m[t] for pc, t, kk in seq if t in self.m)
            if len(ms) < 3:
                continue
            cands = set()
            for t in ms:
                cands |= idx.get(t, set())
            sc = []
            for k in cands:
                inter = len(ms & gsets[k])
                sc.append((float(inter) / len(ms | gsets[k]), inter, k))
            sc.sort(reverse=True)
            if not sc or sc[0][0] < 0.6 or sc[0][1] < 3:
                continue
            if len(sc) > 1 and sc[1][0] > 0.6 * sc[0][0]:
                continue
            j, inter, k = sc[0]
            lv = 2 if size_ok(self.nsize(n), self.gsize(k)) else 1
            if self.far(n, k):
                lv = 1
            new += self.add(n, k, 'sig', lv, '', 'callee set: %d of %d mapped callees shared (Jaccard %.2f; next %.2f)'
                            % (inter, len(ms), j, sc[1][0] if len(sc) > 1 else 0))
        return new

    def propagate_ordc(self):
        """order consistency: the N64 function lies between two anchors of the same GC file (nearest below and
        above in address, both strong+ by non-order evidence) and the GC function lies between them in source order."""
        new = 0
        per = defaultdict(list)
        for n, g in self.m.items():
            if self.conf.get(n, 0) < 2 or n not in self.pop or g not in self.pos:
                continue
            if not any(GROUP[k] in ('solid', 'callee', 'sig', 'string') and lv >= 2 for k, lv, s_, t in self.ev[(n, g)]):
                continue
            p = self.pop[n]
            if p['pop'] == 'static':
                continue
            per[(p['pop'], self.pos[g][0])].append((p['address'], n, g))
        for k in per:
            per[k].sort()
        for n, g in list(self.m.items()):
            p = self.pop.get(n)
            if p is None or p['pop'] == 'static' or g not in self.pos:
                continue
            f, iy = self.pos[g]
            lst = per.get((p['pop'], f), [])
            lo = [x for x in lst if x[0] < p['address'] and x[1] != n]
            hi = [x for x in lst if x[0] > p['address'] and x[1] != n]
            if not lo or not hi:
                continue
            a0, n0, g0 = lo[-1]
            a1, n1, g1 = hi[0]
            if self.pos[g0][1] < iy < self.pos[g1][1]:
                txt = 'file order %s: between anchors %s=%s and %s=%s in both address and source order' % (
                    f, n0.split(':', 1)[1], g0, n1.split(':', 1)[1], g1)
                new += self.add(n, g, 'order', 2, f + '|c', txt)
        return new

    def propagate_stubs(self):
        """deleted inlined statics: the run of 8-byte caller-less stubs right before an anchored N64 function n
        pairs with the GC static callees of g (same file, unmapped) that n does not call (inlined). 1:1 only."""
        new = 0
        if not hasattr(self, 'addr_index'):
            self.addr_index = {}
            for popn in ('game', 'ovl_a', 'ovl_b'):
                lst = sorted((v['address'], i) for i, v in self.pop.items() if v['pop'] == popn)
                self.addr_index[popn] = lst
                for k, (a, i) in enumerate(lst):
                    self.pop[i]['_k'] = k
        for n, g in list(self.m.items()):
            if self.conf.get(n, 0) < 2 or n not in self.pop or self.pop[n]['pop'] == 'static':
                continue
            lst = self.addr_index[self.pop[n]['pop']]
            k = self.pop[n]['_k'] - 1
            run = []
            while k >= 0 and self.pop[lst[k][1]]['size'] == 8:
                run.append(lst[k][1])
                k -= 1
            if not run:
                continue
            ncalls = set(t for pc, t, kk in self.calls.get(n, []))
            f = self.gc[g]['file']
            cand = []
            for pc, t, kk in self.gc[g]['calls']:
                v = self.gc.get(t)
                if v is None or v['file'] != f or v['scope'] != 'static' or t == g:
                    continue
                if t in self.inv and self.inv[t] in ncalls:
                    continue
                if t not in cand:
                    cand.append(t)
            freec = [t for t in cand if t not in self.inv]
            freer = [x for x in run if x not in self.m]
            if len(freec) == 1 and len(freer) == 1 and len(run) == len(cand):
                new += self.add(freer[0], freec[0], 'stub', 2, n,
                                'deleted inlined static: 8-byte stub right before %s=%s; %s is its only uncalled static GC callee'
                                % (n.split(':', 1)[1], g, freec[0]))
        return new

    def ipa_postorder(self, g):
        """Predicted emission order of g's same-file call subtree for an IDO -O3/IPA unit: post-order DFS, callees
        visited from the LAST call site to the first, each function once; g last."""
        f = self.gc[g]['file']
        out, seen = [], set()

        def visit(k, depth=0):
            if k in seen or depth > 40:
                return
            seen.add(k)
            calls = [t for pc, t, kk in self.gc[k]['calls']]
            for t in reversed(calls):
                v = self.gc.get(t)
                if v is not None and v['file'] == f and t != k:
                    visit(t, depth + 1)
            out.append(k)
        visit(g)
        return out

    def propagate_ipa(self):
        """IPA layout: align the predicted post-order of an anchored root's subtree with the N64 functions that end
        at the root in address order (known pairs must agree: >= 2 agreeing, none contradicting)."""
        new = 0
        if not hasattr(self, 'addr_index'):
            self.propagate_stubs()
        for n, g in list(self.m.items()):
            if self.conf.get(n, 0) < 2 or n not in self.pop or self.pop[n]['pop'] == 'static':
                continue
            f = self.gc[g]['file']
            if any(self.gc[c]['file'] == f for c in self.gcallers.get(g, ()) if c in self.gc):
                continue            # not a root of its file's call graph
            S = self.ipa_postorder(g)
            if len(S) < 3:
                continue
            lst = self.addr_index[self.pop[n]['pop']]
            k = self.pop[n]['_k']
            R = [i for a, i in lst[max(0, k - len(S) - 4):k + 1]]
            # alignment: known pairs anchor; contradictions counted
            agree = sum(1 for x in R if x in self.m and self.m[x] in S and x != n)
            contra = sum(1 for x in R[-len(S):] if x in self.m and self.m[x] not in S and
                         self.gc[self.m[x]]['file'] == f)
            if agree < 2 or contra:
                continue
            la, lb = len(R), len(S)
            Sc = [[0] * (lb + 1) for _ in range(la + 1)]
            P = [[None] * (lb + 1) for _ in range(la + 1)]
            for i in range(1, la + 1):
                for j in range(1, lb + 1):
                    best, bp = Sc[i - 1][j], 'u'
                    if Sc[i][j - 1] > best:
                        best, bp = Sc[i][j - 1], 'l'
                    if self.m.get(R[i - 1]) == S[j - 1] and Sc[i - 1][j - 1] + 3 > best:
                        best, bp = Sc[i - 1][j - 1] + 3, 'd'
                    Sc[i][j], P[i][j] = best, bp
            i, j, pairs = la, lb, []
            while i > 0 and j > 0:
                if P[i][j] == 'd':
                    pairs.append((i - 1, j - 1)); i, j = i - 1, j - 1
                elif P[i][j] == 'u':
                    i -= 1
                else:
                    j -= 1
            pairs.reverse()
            if len(pairs) < 3 or pairs[-1] != (la - 1, lb - 1):
                continue
            for (i0, j0), (i1, j1) in zip(pairs, pairs[1:]):
                ua = [x for x in R[i0 + 1:i1] if x not in self.m]
                ub = [y for y in S[j0 + 1:j1] if y not in self.inv]
                if (i1 - i0) != (j1 - j0) or len(ua) != i1 - i0 - 1 or len(ub) != j1 - j0 - 1:
                    continue
                for x, y in zip(ua, ub):
                    stub = self.pop[x]['size'] == 8
                    if not stub and not size_ok(self.nsize(x), self.gsize(y)):
                        continue
                    lv = 2 if len(ua) == 1 else 1
                    new += self.add(x, y, 'order', lv, f + '|ipa', 'IPA layout of %s (post-order, last call first): slot %s'
                                    ' between %s=%s and %s=%s%s' % (g, '%d/%d' % (j0 + 2, lb), R[i0].split(':', 1)[1],
                                    S[j0], R[i1].split(':', 1)[1], S[j1], ' (8-byte stub = inlined)' if stub else ''))
        return new

    def propagate_order(self):
        new = 0
        per = defaultdict(list)
        for n, g in self.m.items():
            if self.conf.get(n, 0) < 2 or n not in self.pop or g not in self.pos:
                continue
            p = self.pop[n]
            if p['pop'] == 'static':
                continue
            per[(p['pop'], self.pos[g][0])].append((p['address'], n, g))
        fb = defaultdict(list)
        for i, v in self.pop.items():
            if v['pop'] != 'static':
                fb[v['pop']].append((v['address'], i))
        for k in fb:
            fb[k].sort()
        self.order_brackets = []
        for (pp, f), lst in per.items():
            lst.sort()
            fl = self.order[f]
            for (a0, n0, g0), (a1, n1, g1) in zip(lst, lst[1:]):
                i0, i1 = self.pos[g0][1], self.pos[g1][1]
                if i1 <= i0:
                    continue
                mid_n = [i for a, i in fb[pp] if a0 < a < a1]
                mid_g = fl[i0 + 1:i1]
                if not mid_n or not mid_g:
                    continue
                self.order_brackets.append((pp, f, n0, g0, n1, g1, len(mid_n), len(mid_g)))
                if len(mid_n) != len(mid_g):
                    continue
                if not all(size_ok(self.nsize(x), self.gsize(y)) or self.gc[y]['stripped'] for x, y in zip(mid_n, mid_g)):
                    continue
                for x, y in zip(mid_n, mid_g):
                    txt = 'file order %s: %d N64 = %d GC between %s=%s and %s=%s' % (
                        f, len(mid_n), len(mid_g), n0.split(':', 1)[1], g0, n1.split(':', 1)[1], g1)
                    new += self.add(x, y, 'order', 2, f, txt)
        return new

    def run(self, rounds=20, quiet=False):
        self.load_anchors()
        self.settle()
        for r in range(rounds):
            a = self.propagate_callees()
            self.settle()
            b = self.propagate_order()
            self.settle()
            b += self.propagate_signature()
            self.settle()
            b += self.propagate_ordc()
            self.settle()
            b += self.propagate_stubs()
            self.settle()
            b += self.propagate_struct()
            self.settle()
            b += self.propagate_struct_global()
            self.settle()
            b += self.propagate_ipa()
            self.settle()
            if not quiet:
                print('round %d: +%d callee, +%d order items; %d pairs %s; conflicts %d' % (
                    r, a, b, len(self.m), dict(Counter(LV[c] for c in self.conf.values())), len(self.conflicts)))
            if a == 0 and b == 0:
                break


if __name__ == '__main__':
    M = Mapper()
    M.run()
    import output
    output.write(M)
