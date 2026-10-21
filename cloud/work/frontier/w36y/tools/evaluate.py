"""w36y: compare the automatic map (build/map_data_auto.json) with harvested lane claims (build/harvest.json) and the
GC-named GAME_SYMBOLS (w29n/w30n batches). Prints agreement per confidence grade and every disagreement."""
import json
import os
import sys
from collections import Counter, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = '/home/cburnes/projects/rush2049-decomp'
sys.path.insert(0, REPO)


def truth():
    t = []
    for c in json.load(open(os.path.join(HERE, '..', 'build', 'harvest.json')))['rows']:
        t.append((c['scope'], int(c['address'], 16), c['name'], 'lane:' + c['sources'][0]))
    from tools.conveyor.pipeline import disasm as D
    for a, n in D.GAME_SYMBOLS.items():
        if not n.startswith('D_') and n[0] in 'gnc' and n[1:2].isupper() or n in ('namePanes', 'codePane'):
            t.append(('game', a, n, 'GAME_SYMBOLS'))
    return t


def main():
    rows = json.load(open(os.path.join(HERE, '..', 'build', 'map_data_auto.json')))['rows']
    bym = defaultdict(list)
    for r in rows:
        bym[r['mat3_name']].append(r)
    stats = Counter()
    seen = set()
    for scope, a, name, src in truth():
        if (scope, a, name) in seen:
            continue
        seen.add((scope, a, name))
        rs = bym.get(name)
        if not rs:
            stats['missing'] += 1
            continue
        hit = [r for r in rs if int(r['n64_address'], 16) == a and (scope == 'ovl' or r['population'] in (
            scope, 'static-data', 'game'))]
        if hit:
            stats['ok_' + hit[0]['confidence']] += 1
        else:
            best = sorted(rs, key=lambda r: -r['support_functions'])[0]
            stats['diff_' + best['confidence']] += 1
            print('DIFF %-6s 0x%08X %-20s (%s) -> %s' % (scope, a, name, src, ', '.join(
                '%s %s %s/%d' % (r['population'], r['n64_address'], r['confidence'], r['support_functions'])
                for r in rs[:4])))
    print(dict(stats))


if __name__ == '__main__':
    main()
