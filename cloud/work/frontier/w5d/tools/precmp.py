#!/usr/bin/env python3
"""Compare two pretrace runs of the same procedure by expression text (bit numbers differ).

    precmp.py RUN_A RUN_B PROC

Prints expressions present in only one run, expressions whose PRE result (number of deleted /
inserted occurrences), candidate sets or colouring differ, and register-candidate decisions for
constants/addresses that differ."""
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import prereport


def key_rows(run, proc):
    r = prereport.load(run, proc)
    rows = prereport.bit_rows(r)
    by = {}
    for row in rows.values():
        if row['kind'] == 3 and not row['webs']:
            continue
        by.setdefault(row['text'], []).append(row)
    rc = {}
    for c in r['regcand']:
        rc.setdefault((c['what'], c['text'], c['ctx']), [0, 0])[c['decision']] += 1
    return by, rc


def sig(rows):
    out = []
    for row in rows:
        regs = ','.join('%s:%s' % (w['phase'], w.get('reg', '-')) for w in row['webs'])
        out.append('occ=%d del=%d ins=%d %s %s' % (len(row['antlocs']), len(row['delete']), len(row['insert']),
                   '/'.join(x.replace('@ ', '').replace(' ', '') for x in row['sets']), regs))
    return '; '.join(out)


def main(a, b, proc):
    A, rcA = key_rows(a, proc)
    B, rcB = key_rows(b, proc)
    print('== expressions: A=%s  B=%s ==' % (a, b))
    for t in sorted(set(A) | set(B)):
        sa, sb = sig(A.get(t, [])), sig(B.get(t, []))
        if sa != sb:
            print('  %s\n      A: %s\n      B: %s' % (t, sa or '(absent)', sb or '(absent)'))
    print('== constant/address register candidates [remat, live-range] counts ==')
    for k in sorted(set(rcA) | set(rcB)):
        if rcA.get(k) != rcB.get(k):
            print('  %-8s %-24s ctx=%d  A=%s B=%s' % (k[0], k[1], k[2], rcA.get(k), rcB.get(k)))


if __name__ == '__main__':
    main(*sys.argv[1:4])
