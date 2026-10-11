# bmap.py OR_SPL CT_TXT: block -> webs with occurrences there (u/d/weight), from SAVELOG records (first
# evaluation of each live range) joined with ctrace web descriptions. Diagnostic only.
import sys, re, collections
spl, ct = sys.argv[1], sys.argv[2]
desc = {}
for l in open(ct):
    m = re.match(r'p1 w(\d+) .*type=(\d) raw10=(0x[0-9a-f]+) bb=(-?\d+)', l)
    if m: desc[int(m.group(1))] = 't%s:%s' % (m.group(2), m.group(3))
seen = set(); cur = []; blk = collections.defaultdict(list)
for l in open(spl):
    if not l.startswith('[SAVE] '): continue
    lr = int(re.search(r'lr=(\d+)', l).group(1))
    if ' card=' in l:
        if lr not in seen:
            seen.add(lr)
            for b, u, d, w in cur: blk[b].append('w%d(%s)%s%s/%s' % (lr, desc.get(lr, '?'), u, d, w))
        cur = []; continue
    m = re.search(r'bb=(\d+) u=(\d+) d=(\d+) w=(\d+)', l)
    cur.append((int(m.group(1)), m.group(2), m.group(3), m.group(4)))
for b in sorted(blk): print(b, ' '.join(blk[b]))
