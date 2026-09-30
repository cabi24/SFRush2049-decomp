#!/usr/bin/env python3
"""clobber.py: registers each function may write and does not restore, locally and transitively.

    python3 cloud/work/tools/ipakit/clobber.py NAME... [--json] [--explain REG]
    python3 cloud/work/tools/ipakit/clobber.py --all [--json]

local(F)      = registers F's own body writes (home-slot/save restores excluded), minus callee-saved registers F
                saves and restores.
transitive(F) = (local(F) | transitive(callees)) minus what F saves and restores, computed to a fixpoint over the
                call graph.  Calls whose target is not a known game function (static library code, jalr,
                unresolved) are assumed ABI: they clobber every caller-saved register and ra, and the function
                is marked `unknown` (its clobber set is a lower bound for the registers it keeps, an upper bound
                for the ones it leaves alone only when no unknown callee is reached).
`--explain REG` names the functions whose own code writes REG in the transitive subtree (why REG is clobbered).
"""
import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from ipakit import load_corpus  # noqa: E402
from ipakit import analyze  # noqa: E402
from ipakit.analyze import CALLER_SAVED, CALLEE_SAVED, RA, STRIP  # noqa: E402
from ipakit.mipsdec import mask_names, REGBIT  # noqa: E402

ABI_CLOBBER = CALLER_SAVED | RA


class ClobberMap:
    def __init__(self, corpus, infos):
        self.corpus, self.infos = corpus, infos
        self.local, self.trans, self.unknown, self.callees = {}, {}, {}, {}
        for name, i in infos.items():
            keep = i.saved & i.restored
            self.local[name] = i.local_defs & ~keep & ~STRIP
            cs, unk = set(), False
            for s in i.sites:
                if s.callee is not None and s.res in ('func', 'alt'):
                    cs.add(s.callee)
                else:
                    unk = True
            self.callees[name] = cs
            self.unknown[name] = unk
            self.trans[name] = self.local[name] | (ABI_CLOBBER if unk else 0)
        changed = True
        while changed:
            changed = False
            for name, i in infos.items():
                keep = i.saved & i.restored
                t = self.trans[name]
                for c in self.callees[name]:
                    t |= self.trans.get(c, 0) & ~keep
                if t != self.trans[name]:
                    self.trans[name] = t
                    changed = True
        # an unknown callee anywhere below makes the function unknown too
        unk = dict(self.unknown)
        changed = True
        while changed:
            changed = False
            for name in infos:
                if not unk[name] and any(unk.get(c) for c in self.callees[name]):
                    unk[name] = True
                    changed = True
        self.reaches_unknown = unk

    def writers(self, name, reg):
        """Functions in name's call subtree (including itself) whose own code writes `reg` unrestored."""
        bit = 1 << REGBIT[reg]
        seen, stack, out = set(), [name], []
        while stack:
            f = stack.pop()
            if f in seen:
                continue
            seen.add(f)
            if self.local.get(f, 0) & bit:
                out.append(f)
            stack.extend(self.callees.get(f, ()))
        return sorted(out)

    def subtree(self, name):
        seen, stack = set(), [name]
        while stack:
            f = stack.pop()
            if f in seen:
                continue
            seen.add(f)
            stack.extend(self.callees.get(f, ()))
        return seen

    def to_json(self, name):
        return {'name': name, 'local': mask_names(self.local[name]), 'transitive': mask_names(self.trans[name]),
                'callees': sorted(self.callees[name]), 'unknown_callee': self.unknown[name],
                'reaches_unknown': self.reaches_unknown[name]}


def build(corpus=None, infos=None):
    corpus = corpus or load_corpus()
    infos = infos or analyze.analyze_all(corpus)
    return ClobberMap(corpus, infos)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('names', nargs='*')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('--json', action='store_true')
    ap.add_argument('--explain', metavar='REG')
    a = ap.parse_args(argv)
    cm = build()
    names = sorted(cm.local) if a.all else a.names
    if a.json:
        print(json.dumps([cm.to_json(n) for n in names], indent=1))
        return 0
    for n in names:
        if n not in cm.local:
            print('%s: unknown function' % n)
            continue
        def show(mask):
            return ' '.join(x for x in mask_names(mask) if x not in ('ra',)) or '-'
        print('%-28s local: %s' % (n, show(cm.local[n])))
        print('%-28s trans: %s%s' % ('', show(cm.trans[n]), '   [reaches unknown/ABI callee]' if cm.reaches_unknown[n] else ''))
        if a.explain:
            print('%-28s %s written by: %s' % ('', a.explain, ', '.join(cm.writers(n, a.explain)) or '(nobody; only via ABI/unknown callee)'))
    if a.all:
        print('%d functions; %d reach an unknown callee' % (len(names), sum(1 for n in names if cm.reaches_unknown[n])))
    return 0


if __name__ == '__main__':
    sys.exit(main())
