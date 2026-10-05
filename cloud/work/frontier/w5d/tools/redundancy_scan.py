#!/usr/bin/env python3
"""Whole-unit scan: occurrences that are fully available at block entry (level-25 avin) and computed
there (level-3 antlocs) but NOT deleted by PRE.  Used to establish that uopt always deletes fully
redundant arithmetic/load occurrences (only comparisons are kept).

    # on the builder, in a snapshot dir st_X (all procedures):
    #   W5D_LEVEL=3  W5D_OUT=$PWD/all.w5d ../uopt/uopt ... merged opt -t st.run -l all3.list
    #   W5D_LEVEL=25 ../uopt/uopt ... merged opt -t st.run -l all25.list
    redundancy_scan.py all3.list all25.list all.w5d [blob_matched.lock.json]
"""
import json, os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import cmparse


def split(path):
    procs, cur = {}, None
    for line in open(path, errors='replace').read().splitlines():
        m = re.search(r'SECONDS IN LOCAL OPTIMIZATION OF (\S+)$', line)
        if m:
            cur = m.group(1); procs[cur] = []
        if cur:
            procs[cur].append(line)
    return procs


def main(l3, l25, w5d, lock=None):
    P3, P25 = split(l3), split(l25)
    matched = set(json.load(open(lock))) if lock else set()
    bits = {}
    for line in open(w5d, errors='replace'):
        if line.startswith('[W5D] bit at=cm'):
            f = dict(re.findall(r'(\w+)=(\S*)', line))
            bits.setdefault(f['proc'], {})[int(f['bit'])] = line.split(' text=', 1)[1].strip()
    kept = 0
    for p in P3:
        if p not in P25:
            continue
        t3, n3, _ = cmparse.parse(P3[p]); _, n25, _ = cmparse.parse(P25[p])
        for node, v in n25.items():
            for b in (v.get('avin', set()) & n3.get(node, {}).get('antlocs', set())) - n3.get(node, {}).get('delete', set()):
                tx = bits.get(p, {}).get(b, '?')
                if tx.startswith(('var', 'str.', 'istr.', 'cg1')):
                    continue
                kept += 1
                print('%s %-28s node=%-3d bit=%-4d %s' % ('M' if p in matched else '-', p, node, b, tx))
    print('%d fully-available occurrences kept' % kept, file=sys.stderr)


if __name__ == '__main__':
    main(*sys.argv[1:5])
