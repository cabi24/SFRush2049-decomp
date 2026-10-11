# homesim.py OR_SPL [extra spec]: simulate f_spilltemps home assignment (bit order, first-fit, conflict = shared
# block) from SAVELOG live-block sets (first evaluation: livbbs + vector). Diagnostic only.
import sys, re
spl = sys.argv[1]
WEBS = [5, 10, 109, 110, 160, 171, 263, 297, 554, 647, 801, 947, 950]
blocks = {}; cur = []
lines = open(spl).read().split('\n')
for k, l in enumerate(lines):
    if not l.startswith('[SAVE] '): continue
    lr = int(re.search(r'lr=(\d+)', l).group(1))
    if ' card=' in l:
        if lr in WEBS and lr not in blocks:
            v = lines[k + 1]; assert v.startswith('[SAVEV] lr=%d:' % lr)
            vec = set(int(x) for x in v.split(':')[1].split() if int(x) < 200)
            blocks[lr] = vec | set(cur)
        cur = []; continue
    cur.append(int(re.search(r'bb=(\d+)', l).group(1)))
def sim(order, area=624, show=True):
    homes = []; res = {}
    for w in order:
        for h, hold in enumerate(homes):
            if all(not (blocks[w] & blocks[o]) for o in hold): hold.append(w); res[w] = h; break
        else: homes.append([w]); res[w] = len(homes) - 1
    off = {w: 728 - (area + 4 * (h + 1)) for w, h in res.items()}
    if show: print(' '.join('%d:%d' % (w, off[w]) for w in order))
    return off
if __name__ == '__main__':
    for w in WEBS: print(w, len(blocks[w]), sorted(blocks[w])[:6], '...')
    sim(WEBS)
    print('conflicts:')
    for w in WEBS: print(w, [o for o in WEBS if o != w and blocks[w] & blocks[o]])
