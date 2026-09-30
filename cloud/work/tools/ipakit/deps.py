#!/usr/bin/env python3
"""deps.py: register-specific IPA dependency edges, classification and minimal closure.

    python3 cloud/work/tools/ipakit/deps.py classify [NAME...] [--json]     ABI / IPA-leaf / IPA-caller + evidence
    python3 cloud/work/tools/ipakit/deps.py edges NAME... [--json]          register-specific edges touching NAME
    python3 cloud/work/tools/ipakit/deps.py closure NAME... [--json] [--conservative]
    python3 cloud/work/tools/ipakit/deps.py roots [--json]                  address-taken roots and jalr callers
    python3 cloud/work/tools/ipakit/deps.py validate [--json]               precision/recall against locked + drafted groups

Evidence (all from retail words, see analyze.py / clobber.py):
  entry-read R    F reads non-ABI register R at entry      -> edge (F, C, R) for EVERY caller C (C sets R)
  unsaved R       F writes callee-saved R with no save     -> edge (F, C, R) for every caller C, and on up the call
                                                              chain while callers do not restore R themselves
  live-across R   C keeps R live across `jal F`            -> edge (C, F, R), explained iff R is not in F's
                                                              transitive clobber set; F's whole callee subtree is
                                                              needed to explain it (their clobber sets feed F's)
  passes          C calls F and F reads non-ABI registers   -> C needs F (to know what to set)
MINIMAL closure: fixpoint of exactly those rules from the seeds, each function expanded only by ITS OWN evidence.
CONSERVATIVE closure (the existing pipeline/ipa.py discover_groups unit): for a preserver, every callee plus its
whole callee closure; for a callee, its callers; merged on overlap.
"""
import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from ipakit import ROOT, IMAGE_BASE, load_corpus  # noqa: E402
from ipakit import analyze, clobber  # noqa: E402
from ipakit.analyze import CALLEE_SAVED, CALLER_SAVED  # noqa: E402
from ipakit.mipsdec import mask_names, decode_words, REGBIT  # noqa: E402

GROUP_CAP = 1000            # pipeline/ipa.py GROUP_CAP_INSNS


class Model:
    def __init__(self, corpus=None):
        self.corpus = corpus or load_corpus(heads=True)
        self.infos = analyze.analyze_all(self.corpus)
        self.cm = clobber.ClobberMap(self.corpus, self.infos)
        self.callers = {}           # callee -> {caller: [site, ...]}
        for n, i in self.infos.items():
            for s in i.sites:
                if s.callee is not None and s.res in ('func', 'alt', 'interior'):
                    self.callers.setdefault(s.callee, {}).setdefault(n, []).append(s)
        self.addr_taken = self._address_taken()
        self.indirect = {n: [s.pc for s in i.sites if s.kind == 'jalr'] for n, i in self.infos.items()}
        self.indirect = {n: v for n, v in self.indirect.items() if v}

    # -- address-taken roots -----------------------------------------------------------------------
    def _address_taken(self):
        c = self.corpus
        out = {}
        for name, f in c.funcs.items():
            regs = {}
            for d in decode_words(f.words, f.addr):
                if d.mnem == 'lui':
                    regs[d.rt] = (d.word & 0xFFFF) << 16
                    continue
                if d.mnem in ('addiu', 'ori') and d.rs in regs:
                    v = (regs[d.rs] + (d.imm if d.mnem == 'addiu' else d.word & 0xFFFF)) & 0xFFFFFFFF
                    g = c.by_addr.get(v)
                    if g is not None and g.name != name or (g is not None and d.rt != d.rs):
                        out.setdefault(g.name, []).append('code %s+0x%x' % (name, d.pc - f.addr))
                    if d.rt != d.rs:
                        regs.pop(d.rt, None)
                    else:
                        regs[d.rt] = v
                    continue
                for r in range(32):
                    if d.defs >> r & 1 and r in regs:
                        del regs[r]
        for i, v in enumerate(c.image):
            pa = IMAGE_BASE + 4 * i
            g = c.by_addr.get(v)
            if g is not None and c.containing(pa) is None:
                out.setdefault(g.name, []).append('data 0x%08X' % pa)
        return out

    # -- classification ----------------------------------------------------------------------------
    def evidence(self, name):
        i = self.infos[name]
        ev = []
        if i.entry_reads_mask:
            uw = i.entry_reads_mask & ~i.local_defs
            for r in i.entry_reads:
                ev.append({'kind': 'entry-read', 'reg': r, 'strong': bool(uw >> REGBIT[r] & 1),
                           'text': '%s reads %s at entry%s' % (name, r, '' if uw >> REGBIT[r] & 1 else ' (also written later: may be path-correlated)')})
        for r in mask_names(i.unsaved_written):
            ev.append({'kind': 'unsaved', 'reg': r, 'text': '%s writes %s with no save/restore' % (name, r)})
        for s in i.sites:
            if s.callee is not None and s.res in ('func', 'alt'):
                for r in s.live_across:
                    expl = not (self.cm.trans[s.callee] >> REGBIT[r] & 1)
                    ev.append({'kind': 'live-across' if expl else 'unexplained', 'reg': r, 'callee': s.callee, 'pc': s.pc,
                               'explained': expl,
                               'text': '%s keeps %s live across jal %s at 0x%08X%s' % (
                                   name, r, s.callee, s.pc, '' if expl else ' (UNEXPLAINED: callee clobber set contains %s; not counted)' % r)})
                passed = self.infos[s.callee].entry_reads_mask
                if passed:
                    ev.append({'kind': 'passes', 'callee': s.callee, 'pc': s.pc, 'regs': mask_names(passed),
                               'text': '%s sets %s for %s at 0x%08X' % (name, ','.join(mask_names(passed)), s.callee, s.pc)})
            elif s.live_across:
                ev.append({'kind': 'anomaly', 'pc': s.pc, 'regs': s.live_across,
                           'text': '%s: %s live across %s call at 0x%08X (ABI callee: liveness artefact?)' % (
                               name, ','.join(s.live_across), s.res, s.pc)})
        return ev

    def classify(self, name):
        ev = self.evidence(name)
        leaf = any(e['kind'] in ('entry-read', 'unsaved') for e in ev)
        caller = any(e['kind'] in ('live-across', 'passes') for e in ev)
        cls = 'IPA-both' if leaf and caller else 'IPA-leaf' if leaf else 'IPA-caller' if caller else 'ABI'
        return cls, ev

    # -- closure -----------------------------------------------------------------------------------
    def subtree(self, name):
        return self.cm.subtree(name)

    def words(self, names):
        return sum(len(self.corpus.funcs[n].words) for n in names if n in self.corpus.funcs)

    def minimal_closure(self, seeds, mode='direct', max_funcs=400):
        """Functions needed to explain the IPA evidence of `seeds`.

        Returns (roles, notes); roles = {function: [reason, ...]}.  Rules (see module docstring) applied to
        the functions in the `expand` set:
          entry-read R         -> every caller (sets R)                               role: caller
          unsaved R            -> every caller (and up the chain while they do not restore R, modes chain/full)
          live-across R across -> the callee G (its clobber set, which is computed statically from its whole
          jal G                   subtree, is what explains R); mode `full` also adds G's subtree
          passes (G has params)-> G
        Modes: `direct` expands only the seeds (callers/callees are stand-ins or context, nothing more);
        `chain` also expands every caller added (real code that must match too) but not callee summaries;
        `full` expands everything, including callee subtrees (the conservative upper bound)."""
        reasons = {}
        roles = {}
        work = []
        notes = {'indirect_calls': {}, 'address_taken': {}, 'unexplained': [], 'unresolved_targets': [], 'truncated': False,
                 'mode': mode}

        def add(n, why, role, expand):
            if n not in self.infos:
                return
            new = n not in reasons
            if new:
                reasons[n] = []
                roles[n] = set()
                if len(reasons) > max_funcs:
                    notes['truncated'] = True
            if why not in reasons[n]:
                reasons[n].append(why)
            roles[n].add(role)
            if expand and n not in expanded:
                expanded.add(n)
                work.append(n)

        expanded = set()
        for s in seeds:
            add(s, 'seed', 'seed', True)
        expanded_free = set()
        chain = mode in ('chain', 'full')
        full = mode == 'full'

        def need_callers_free(g, reg):
            if (g, reg) in expanded_free:
                return
            expanded_free.add((g, reg))
            bit = 1 << REGBIT[reg]
            for c in sorted(self.callers.get(g, ())):
                add(c, 'free %s for %s' % (reg, g), 'caller', chain)
                if chain and self.cm.trans[c] & bit and not (self.infos[c].saved & self.infos[c].restored & bit):
                    need_callers_free(c, reg)

        while work and not notes['truncated']:
            n = work.pop()
            i = self.infos[n]
            for r in i.entry_reads:
                for c in sorted(self.callers.get(n, ())):
                    add(c, 'sets %s for %s' % (r, n), 'caller', chain)
            # callers that keep registers live across a call to n: their code only exists because n leaves
            # those registers alone, so they are the dependents that pin n's clobber set
            for c, sites in sorted(self.callers.get(n, {}).items()):
                regs = sorted({r for st in sites if st.res in ('func', 'alt') for r in st.live_across
                               if not (self.cm.trans[n] >> REGBIT[r] & 1)})
                if regs and c != n:
                    add(c, 'keeps %s live across calls to %s' % (','.join(regs), n), 'dependent', chain)
            for r in mask_names(i.unsaved_written):
                need_callers_free(n, r)
            for s in i.sites:
                if s.kind == 'jalr':
                    notes['indirect_calls'].setdefault(n, []).append(s.pc)
                if s.callee is None:
                    if s.res in ('opaque', 'interior'):
                        notes['unresolved_targets'].append((n, s.pc, s.target))
                    continue
                cal = s.callee
                if s.live_across and s.res in ('func', 'alt'):
                    live = [r for r in s.live_across if not (self.cm.trans[cal] >> REGBIT[r] & 1)]
                    for r in s.live_across:
                        if r not in live:
                            notes['unexplained'].append((n, s.pc, cal, r))
                    if live:
                        add(cal, 'callee clobber set (%s kept across call)' % ','.join(live), 'callee-summary', full)
                        if full:
                            for g in sorted(self.subtree(cal)):
                                add(g, 'summary of %s' % cal, 'callee-summary', True)
                if self.infos[cal].entry_reads_mask and s.res in ('func', 'alt'):
                    add(cal, 'callee of %s reads %s' % (n, ','.join(mask_names(self.infos[cal].entry_reads_mask))),
                        'callee-params', full)
        for n in reasons:
            if n in self.addr_taken:
                notes['address_taken'][n] = self.addr_taken[n][:3]
        notes['roles'] = {n: sorted(r) for n, r in roles.items()}
        return reasons, notes

    def conservative_closure(self, seeds):
        """pipeline/ipa.py discover_groups semantics: units merged on overlap; preservers pull every callee
        and its whole callee closure; callees pull callers; callers pull callees whose registers they set."""
        calls = {n: {s.callee for s in i.sites if s.callee and s.res in ('func', 'alt')} - {n} for n, i in self.infos.items()}
        special = {n for n, i in self.infos.items() if i.entry_reads_mask}
        preservers = {n for n, i in self.infos.items() if any(s.live_across for s in i.sites)}
        setters = {}
        for c, cs in calls.items():
            for g in cs:
                if g in special:
                    setters.setdefault(g, set()).add(c)

        def closure(x):
            seen, st = set(), [x]
            while st:
                for z in calls.get(st.pop(), ()):
                    if z not in seen:
                        seen.add(z)
                        st.append(z)
            return seen

        def unit(t, seen):
            if t in seen:
                return set()
            seen.add(t)
            u = {t} | setters.get(t, set())
            for callee in calls.get(t, ()):
                if callee in special:
                    u |= unit(callee, seen)
            if t in preservers:
                for x in calls.get(t, ()):
                    u |= {x} | closure(x)
            return u
        out = set()
        for s in seeds:
            out |= unit(s, set())
        return out


def model_cache(_c={}):
    if 'm' not in _c:
        _c['m'] = Model()
    return _c['m']


# -- validation ---------------------------------------------------------------------------------------

def load_groups():
    out = []
    for kind, pattern in (('locked', 'src/blob/groups/*/group.json'), ('drafted', 'cloud/work/ipa-groups/*/group.json')):
        for p in sorted(ROOT.glob(pattern)):
            j = json.loads(p.read_text())
            out.append({'name': p.parent.name, 'kind': kind, 'members': j.get('members', []),
                        'context': j.get('context', []), 'targets': list(j.get('targets', {}))})
    return out


def prf(pred, actual):
    tp = len(pred & actual)
    p = tp / len(pred) if pred else 1.0
    r = tp / len(actual) if actual else 1.0
    return p, r


def validate(model=None, mode='direct'):
    model = model or model_cache()
    rows = []
    for g in load_groups():
        names = set(model.infos)
        members = [m for m in g['members'] if m in names]
        actual = set(members) | {c for c in g['context'] if c in names}
        if not members:
            rows.append({'group': g['name'], 'kind': g['kind'], 'skipped': 'members not in corpus (%s)' % g['members'][:2]})
            continue
        reasons, notes = model.minimal_closure(members, mode)
        mc = set(reasons)
        cons = model.conservative_closure(members)
        p, r = prf(mc, actual)
        cp, cr = prf(cons, actual)
        rows.append({'group': g['name'], 'kind': g['kind'], 'members': len(members), 'actual': len(actual),
                     'minimal': len(mc), 'precision': round(p, 3), 'recall': round(r, 3),
                     'minimal_words': model.words(mc), 'actual_words': model.words(actual),
                     'missing': sorted(actual - mc), 'extra': sorted(mc - actual),
                     'conservative': len(cons), 'cons_precision': round(cp, 3), 'cons_recall': round(cr, 3),
                     'truncated': notes['truncated'], 'unresolved': len(notes['unresolved_targets']),
                     'indirect': len(notes['indirect_calls']), 'address_taken': sorted(notes['address_taken'])})
    return rows


def flagged_stats(model=None, cap=GROUP_CAP, mode='direct'):
    """How many IPA-flagged functions get a finite minimal closure vs the bounded discovery."""
    model = model or model_cache()
    flagged = [n for n in model.infos if model.classify(n)[0] != 'ABI']
    res = {'flagged': len(flagged), 'finite': 0, 'finite_clean': 0, 'within_cap': 0, 'bounded_discovery': 0, 'sizes': [],
           'with_indirect_calls': 0, 'with_address_taken_ipa_conflict': 0}
    # existing bounded discovery (discover_groups): units over `cap` insns dropped, merged units over 2*cap dropped
    size = {n: len(f.words) for n, f in model.corpus.funcs.items()}
    units = {t: model.conservative_closure([t]) for t in flagged}
    kept = {t: u for t, u in units.items() if sum(size.get(m, 0) for m in u) <= cap}
    parent = {}

    def find(x):
        while parent.setdefault(x, x) != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x
    for t, u in kept.items():
        for m in u:
            parent[find(m)] = find(t)
    merged = {}
    for t, u in kept.items():
        merged.setdefault(find(t), set()).update(u)
    covered = set()
    for members in merged.values():
        if sum(size.get(m, 0) for m in members) <= 2 * cap:
            covered |= members
    res['bounded_discovery'] = len(covered & set(flagged))
    for n in flagged:
        reasons, notes = model.minimal_closure([n], mode)
        if not notes['truncated']:
            res['finite'] += 1
            w = model.words(reasons)
            res['sizes'].append(w)
            if w <= cap:
                res['within_cap'] += 1
            if not notes['unresolved_targets']:
                res['finite_clean'] += 1
            if notes['indirect_calls']:
                res['with_indirect_calls'] += 1
            if any(model.infos[k].entry_reads_mask for k in notes['address_taken']):
                res['with_address_taken_ipa_conflict'] += 1
    return res


# -- CLI ----------------------------------------------------------------------------------------------

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('cmd', choices=['classify', 'edges', 'closure', 'roots', 'validate', 'stats'])
    ap.add_argument('names', nargs='*')
    ap.add_argument('--json', action='store_true')
    ap.add_argument('--conservative', action='store_true')
    ap.add_argument('--mode', default='direct', choices=['direct', 'chain', 'full'])
    a = ap.parse_args(argv)
    m = model_cache()
    if a.cmd == 'classify':
        names = a.names or sorted(m.infos)
        rows = {}
        for n in names:
            cls, ev = m.classify(n)
            rows[n] = {'class': cls, 'evidence': ev}
        if a.json:
            print(json.dumps(rows, indent=1))
            return 0
        import collections
        for n in names:
            print('%-28s %s' % (n, rows[n]['class']))
            if a.names:
                for e in rows[n]['evidence']:
                    print('    ' + e['text'])
        if not a.names:
            print(dict(collections.Counter(r['class'] for r in rows.values())))
        return 0
    if a.cmd == 'edges':
        out = []
        for n in a.names:
            i = m.infos[n]
            for e in m.evidence(n):
                if e['kind'] == 'live-across':
                    out.append({'edge': 'live-across', 'from': n, 'to': e['callee'], 'reg': e['reg'], 'explained': e['explained'], 'pc': '0x%08X' % e['pc']})
            for r in i.entry_reads:
                for c in sorted(m.callers.get(n, ())):
                    out.append({'edge': 'arg', 'from': n, 'to': c, 'reg': r})
            for r in mask_names(i.unsaved_written):
                for c in sorted(m.callers.get(n, ())):
                    out.append({'edge': 'unsaved', 'from': n, 'to': c, 'reg': r})
            for c, sites in sorted(m.callers.get(n, {}).items()):
                for s in sites:
                    for r in s.live_across:
                        out.append({'edge': 'live-across', 'from': c, 'to': n, 'reg': r, 'explained': not (m.cm.trans[n] >> REGBIT[r] & 1), 'pc': '0x%08X' % s.pc})
        if a.json:
            print(json.dumps(out, indent=1))
        else:
            for e in out:
                print('%-11s %s -> %s  %s%s' % (e['edge'], e['from'], e['to'], e['reg'], '' if e.get('explained', True) else '  UNEXPLAINED'))
        return 0
    if a.cmd == 'closure':
        if a.conservative:
            cons = sorted(m.conservative_closure(a.names))
            print(json.dumps({'conservative': cons, 'words': m.words(cons)}) if a.json else
                  '%d functions %d words: %s' % (len(cons), m.words(cons), ' '.join(cons)))
            return 0
        reasons, notes = m.minimal_closure(a.names, a.mode)
        if a.json:
            print(json.dumps({'closure': reasons, 'words': m.words(reasons), 'notes': notes}, indent=1, default=list))
            return 0
        print('%d functions %d words (truncated=%s)' % (len(reasons), m.words(reasons), notes['truncated']))
        for n in sorted(reasons):
            print('  %-28s %4dw  %s' % (n, len(m.corpus.funcs[n].words), '; '.join(reasons[n][:3])))
        for k in ('indirect_calls', 'address_taken'):
            if notes[k]:
                print('%s: %s' % (k, json.dumps(notes[k], default=list)))
        for k in ('unresolved_targets', 'unexplained'):
            if notes[k]:
                print('%s: %s' % (k, notes[k]))
        return 0
    if a.cmd == 'roots':
        rep = {'address_taken': {k: v[:4] for k, v in sorted(m.addr_taken.items())},
               'indirect_calls': {k: ['0x%08X' % p for p in v] for k, v in sorted(m.indirect.items())}}
        if a.json:
            print(json.dumps(rep, indent=1))
        else:
            print('%d address-taken functions, %d functions with jalr' % (len(rep['address_taken']), len(rep['indirect_calls'])))
            for k, v in rep['address_taken'].items():
                print('  &%s  %s%s' % (k, v[0], '  (has IPA entry reads: CONFLICT)' if m.infos[k].entry_reads_mask else ''))
            for k, v in rep['indirect_calls'].items():
                print('  jalr in %s: %s' % (k, ' '.join(v)))
        return 0
    if a.cmd == 'validate':
        rows = validate(m, a.mode)
        st = flagged_stats(m, mode=a.mode)
        if a.json:
            print(json.dumps({'groups': rows, 'flagged': {k: v for k, v in st.items() if k != 'sizes'}}, indent=1))
            return 0
        print('%-26s %-7s %3s %4s %4s %5s %5s | %4s %5s %5s' % ('group', 'kind', 'mem', 'act', 'min', 'prec', 'rec', 'cons', 'prec', 'rec'))
        for r in rows:
            if 'skipped' in r:
                print('%-26s %-7s skipped: %s' % (r['group'], r['kind'], r['skipped']))
                continue
            print('%-26s %-7s %3d %4d %4d %5.2f %5.2f | %4d %5.2f %5.2f%s' % (
                r['group'], r['kind'], r['members'], r['actual'], r['minimal'], r['precision'], r['recall'],
                r['conservative'], r['cons_precision'], r['cons_recall'], '  TRUNC' if r['truncated'] else ''))
        lk = [r for r in rows if r['kind'] == 'locked' and 'skipped' not in r]
        if lk:
            print('locked groups: mean precision %.2f recall %.2f (minimal, mode %s); conservative %.2f / %.2f; '
                  'closure contains the whole group in %d/%d' % (
                      sum(r['precision'] for r in lk) / len(lk), sum(r['recall'] for r in lk) / len(lk), a.mode,
                      sum(r['cons_precision'] for r in lk) / len(lk), sum(r['cons_recall'] for r in lk) / len(lk),
                      sum(1 for r in lk if r['recall'] == 1.0), len(lk)))
        print({k: v for k, v in st.items() if k != 'sizes'})
        return 0


if __name__ == '__main__':
    sys.exit(main())
