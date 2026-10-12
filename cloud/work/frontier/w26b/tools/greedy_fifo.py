#!/usr/bin/env python3
"""w26b: greedy FIFO explanation of a retail temp-pop sequence.  greedy_fifo.py UGT 'retail pops (space sep, in alloc order)' [RING]
Replays ugen's FREELIST events (w25b fifo_search model: a FREE is real only if it is the last FREE of that register
before its next ALLOC). At each ALLOC_GP_RESULT, if the queue head is not retail's register, inserts hidden pops
(pop head + free to tail, i.e. an allocation whose instruction as1 deletes) until it is, and reports where."""
import sys, re
NAMES = {10:'t2',11:'t3',12:'t4',13:'t5',14:'t6',15:'t7',24:'t8',25:'t9'}
ev = []
for s in open(sys.argv[1]):
    m = re.match(r'DKWB-FREELIST (\S+) proc=\d+ reg=(\d+) emitted=(\d+) line=(\d+)', s)
    if m and int(m.group(2)) in NAMES and m.group(1) in ('ADD', 'ALLOC_GP_RESULT', 'FREE', 'REMOVE'):
        ev.append([m.group(1), NAMES[int(m.group(2))], int(m.group(3)), int(m.group(4))])
for i, x in enumerate(ev):
    if x[0] != 'FREE': continue
    x.append(next((y[0] != 'FREE' for y in ev[i+1:] if y[1] == x[1]), True))
ev = [x for x in ev if x[0] != 'FREE' or x[4]]
want = sys.argv[2].split()
q, k, hidden, m = [], 0, [], {}
for x in ev:
    if x[0] == 'ADD': q.append(x[1])
    elif x[0] == 'REMOVE': q.remove(x[1])
    elif x[0] == 'FREE':
        if x[1] in m: q.append(m.pop(x[1]))
    else:
        w = want[k] if k < len(want) else None
        ours = x[1]
        if w is not None and w not in q:
            print(f'line {x[3]} alloc #{k}: retail {w} is not free (queue {q}) -> held'); break
        n = 0
        while w is not None and q[0] != w:
            hidden.append((x[3], q[0])); q.append(q.pop(0)); n += 1
        r = q.pop(0); m[ours] = r
        print(f'line {x[3]:3d} alloc #{k:2d}: ours {ours} retail {r}' + (f'  <- {n} hidden pop(s) before' if n else ''))
        k += 1
print('hidden pops:', hidden)
