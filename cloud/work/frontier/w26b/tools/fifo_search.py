#!/usr/bin/env python3
"""w25b: FIFO replay of a ugt71 trace + search for the single code-free event that yields a target pop sequence.
usage: fifo_search.py UGT.txt FIRSTDIFF_ALLOC_INDEX 'map' ; map = 't7:t9,t9:t8,t8:t6,t6:t7' (ours->retail after the point)
Model: temps t6 t7 t8 t9 (14 15 24 25) are the only ring members; a FREE returns the register to the tail only if it is
the last FREE of that register before its next ALLOC (usage counts are not traced)."""
import sys, re
N = {14: 't6', 15: 't7', 24: 't8', 25: 't9'}
ev = []
for s in open(sys.argv[1]):
    m = re.match(r'DKWB-FREELIST (\S+) proc=\d+ reg=(\d+) emitted=(\d+) line=(\d+)', s)
    if not m: continue
    k, r, e, l = m.group(1), int(m.group(2)), int(m.group(3)), int(m.group(4))
    if r in N and k in ('ADD', 'ALLOC_GP_RESULT', 'FREE'):
        ev.append([k, N[r], e, l])
# mark real frees: last FREE of r before next ALLOC of r
for i, x in enumerate(ev):
    if x[0] != 'FREE': continue
    real = True
    for y in ev[i + 1:]:
        if y[1] != x[1]: continue
        if y[0] == 'FREE': real = False
        break
    x.append(real)
ev = [x for x in ev if x[0] != 'FREE' or x[4]]
def run(evs):
    q = []; pops = []
    for x in evs:
        if x[0] == 'ADD': q.append(x[1])
        elif x[0] == 'ALLOC_GP_RESULT':
            if x[1] == 'XTRA':
                r = q.pop(0); x.append(r); continue
            if x[1].startswith('XFREE'):
                continue
            r = q.pop(0); pops.append((r, x[3]))
        elif x[0] == 'FREE':
            q.append(x[1])
    return pops
ours = run([list(x) for x in ev])
# sanity: model reproduces ours
real = [(x[1], x[3]) for x in ev if x[0] == 'ALLOC_GP_RESULT']
print('model ok' if [p[0] for p in ours] == [p[0] for p in real] else 'MODEL MISMATCH')
if len(sys.argv) < 4: 
    for i, (p, r) in enumerate(zip(ours, real)): print(i, p, r)
    sys.exit()
k0 = int(sys.argv[2]); mp = dict(a.split(':') for a in sys.argv[3].split(','))
want = [p[0] if i < k0 else mp[p[0]] for i, p in enumerate(real)]
# candidate: extra pop at position a, its free at position b>=a (inserted before event index)
hits = []
for a in range(len(ev) + 1):
    for b in range(a, min(len(ev) + 1, a + 40)):
        evs = [list(x) for x in ev]
        fr = ['FREE', None, 0, 0]
        al = ['ALLOC_GP_RESULT', 'XTRA', 0, 0]
        # simulate with dynamic free of the popped register
        q = []; pops = []; held = None; cur = {}
        for i in range(len(evs) + 1):
            if i == a:
                if not q: break
                held = q.pop(0)
            if i == b and held is not None:
                q.append(held); held = None
            if i == len(evs): break
            x = evs[i]
            if x[0] == 'ADD': q.append(x[1])
            elif x[0] == 'ALLOC_GP_RESULT':
                if not q: break
                r = q.pop(0); pops.append(r); cur[x[1]] = r
            else:
                q.append(cur.get(x[1], x[1]))
        W = int(sys.argv[4]) if len(sys.argv) > 4 else len(want)
        if pops[:W] == want[:W]:
            hits.append((a, b))
for a, b in hits:
    la = ev[a][3] if a < len(ev) else None; lb = ev[b][3] if b < len(ev) else None
    print('extra pop before event %d (line %s, %s) freed before event %d (line %s)' % (a, la, ev[a][:2] if a < len(ev) else '', b, lb))
print(len(hits), 'hits')

# --- delayed / advanced frees: move one real FREE event to another position
def sim(evs):
    q = []; pops = []; cur = {}
    for x in evs:
        if x[0] == 'ADD': q.append(x[1])
        elif x[0] == 'ALLOC_GP_RESULT':
            if not q: return None
            r = q.pop(0); pops.append(r); cur[x[1]] = r
        else:
            q.append(cur.get(x[1], x[1]))
    return pops
W = int(sys.argv[4]) if len(sys.argv) > 4 else len(want)
mv = 0
for i, x in enumerate(ev):
    if x[0] != 'FREE': continue
    for j in range(max(0, i - 30), min(len(ev), i + 60)):
        if j == i: continue
        evs = [list(y) for y in ev]
        y = evs.pop(i); evs.insert(j if j < i else j - 1, y)
        p = sim(evs)
        if p and p[:W] == want[:W]:
            mv += 1; print('move FREE %s (event %d line %s) to before event %d (line %s)' % (x[1], i, x[3], j, ev[j][3]))
print(mv, 'free-move hits')
