"""w36y: locate initialised GC data objects in the N64 images by content (tools/content.py search).

    python3 cloud/work/frontier/w36y/tools/contentscan.py   # writes build/content_hits.json (~1-2 min)
"""
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import content as C  # noqa: E402
import refs as R  # noqa: E402


def main():
    gc = R.GC()
    E, N = C._init()
    gj = json.load(open(os.path.join(R.REPO, 'docs', 'external', 'gc', 'globals.json')))['globals']
    files = {}
    for g in gj:
        files.setdefault((g['name'], int(g['address'], 16)), set()).add(g['file'])
    out = []
    done = set()
    for name, lst in gc.syms.items():
        for s in lst:
            a, sz = s['address'], s['size']
            if (a, name) in done or name.startswith('@') or name.startswith('...') or sz < 8:
                continue
            done.add((a, name))
            if E.section_of(a) is None:
                continue
            fs = files.get((name, a))
            if not fs:
                continue  # not in the debug info of a GameSource/NGC unit (SDK/MSL data)
            hits = {}
            g = C.search(a, sz, 'game')
            if g:
                hits['game'] = g
            for t in ('ovl_a', 'ovl_b'):
                h = C.search(a, sz, t, only=True)
                if h:
                    hits[t] = h
            if hits:
                out.append(dict(name=name, gc_address='0x%08X' % a, size=sz, files=sorted(fs),
                                hits={k: ['0x%08X' % x for x in v] for k, v in hits.items()}))
    json.dump(dict(rows=out), open(os.path.join(HERE, '..', 'build', 'content_hits.json'), 'w'), indent=1)
    uniq = [r for r in out if sum(len(v) for v in r['hits'].values()) == 1]
    print('%d objects with hits, %d unique' % (len(out), len(uniq)))


if __name__ == '__main__':
    main()
