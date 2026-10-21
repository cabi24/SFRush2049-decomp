"""w36n: held-out check. Two folds over the cited anchors (harvest.json + names.md rows): each half is removed
from the anchors, the map is rebuilt from the rest, and the removed pairs are compared with what the engine finds."""
import json, os, random, sys
import mapfn, anchors
HERE = os.path.dirname(os.path.abspath(__file__))
LANE = os.path.dirname(HERE)
rows = json.load(open(os.path.join(LANE, 'harvest.json')))['rows']
truth = [('%s:%s' % (r['population'], r['n64_name']), r['gc_key']) for r in rows if r['kind'] != 'lead']
truth += [('game:' + n, g) for n, (g, t) in anchors.APPLIED.items()]
truth = sorted(set(truth))
random.seed(36)
random.shuffle(truth)
half = len(truth) // 2
folds = [truth[:half], truth[half:]]
tot = {'proven': [0, 0, 0], 'strong': [0, 0, 0], 'lead': [0, 0, 0]}
missing = 0
for k, held in enumerate(folds):
    heldn = set(n for n, g in held)
    orig_rows = list(rows)
    keep = [r for r in rows if '%s:%s' % (r['population'], r['n64_name']) not in heldn]
    json.dump({'rows': keep}, open(os.path.join(LANE, 'harvest.json'), 'w'), indent=1)
    saved = dict(anchors.APPLIED)
    for n in list(anchors.APPLIED):
        if 'game:' + n in heldn:
            del anchors.APPLIED[n]
    try:
        M = mapfn.Mapper(); M.run(quiet=True)
    finally:
        json.dump({'rows': orig_rows}, open(os.path.join(LANE, 'harvest.json'), 'w'), indent=1)
        anchors.APPLIED.clear(); anchors.APPLIED.update(saved)
    for n, g in held:
        got = M.m.get(n)
        if got is None:
            missing += 1
            continue
        c = mapfn.LV[M.conf[n]]
        tot[c][0] += 1
        if got == g:
            tot[c][1] += 1
        else:
            tot[c][2] += 1
            print('fold %d WRONG %s: truth %s, engine %s (%s)' % (k, n, g, got, c))
print('held-out anchors: %d; not recovered: %d' % (len(truth), missing))
for c, (a, ok, bad) in tot.items():
    print('  %-6s %3d recovered: %3d correct, %2d different' % (c, a, ok, bad))
