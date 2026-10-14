"""Call graph of the game blob from asm/us/blob/*.s words: direct jal edges, function-address
references (lui/addiu pairs hitting a function start = address taken), jalr sites.
Usage: cg.py PATH-FROM FN [TARGET]  -> prints chains from FN's callees to TARGET (BFS),
treating jalr as 'may call any address-taken function'."""
import glob, re, sys, json, collections
syms = json.load(open('asm/us/blob/symbols.json'))['symbols']
funcs = {}  # name -> list of (addr, word)
order = []
for path in sorted(glob.glob('asm/us/blob/blob_*.s')):
    cur = None
    for line in open(path):
        m = re.match(r'\.section \.text\.(\S+),', line)
        if m:
            cur = m.group(1); funcs[cur] = []; order.append(cur); continue
        m = re.match(r'\s+\.word 0x([0-9A-Fa-f]{8})', line)
        if m and cur:
            funcs[cur].append(int(m.group(1), 16))
start = {}
for n in funcs:
    if n in syms: start[int(syms[n], 16)] = n
callees = collections.defaultdict(set); taken = set(); jalr = set()
for n, ws in funcs.items():
    if n not in syms: continue
    base = int(syms[n], 16)
    hi = {}
    for i, w in enumerate(ws):
        op = w >> 26
        if op == 3:
            t = ((base + 4*i) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
            callees[n].add(start.get(t, hex(t)))
        elif op == 0 and (w & 0x3F) == 9:
            jalr.add(n)
        elif op == 15:
            hi[(w >> 16) & 31] = (w & 0xFFFF) << 16
        elif op in (9, 8):  # addiu/addi
            rs = (w >> 21) & 31
            if rs in hi:
                v = (hi[rs] + ((w & 0xFFFF) ^ 0x8000) - 0x8000) & 0xFFFFFFFF
                if v in start and start[v] != n or (v in start and start[v]==n):
                    taken.add(start[v])
if __name__ == '__main__':
    fn = sys.argv[1]; target = sys.argv[2] if len(sys.argv) > 2 else fn
    print('address-taken functions:', len(taken)); print('target taken?', target in taken)
    # BFS
    prev = {}; q = collections.deque()
    for c in callees[fn]:
        prev[c] = fn; q.append(c)
    while q:
        x = q.popleft()
        if x == target:
            chain = [x]
            while chain[-1] != fn: chain.append(prev[chain[-1]])
            print(' <- '.join(chain)); break
        nxt = set(callees.get(x, ()))
        if x in jalr: nxt |= {'*' + t for t in []} | taken
        for y in nxt:
            if y not in prev: prev[y] = x; q.append(y)
    else:
        print('no path')
    if len(sys.argv) > 3:
        # direct-only path
        prev = {}; q = collections.deque(callees[fn]); [prev.setdefault(c, fn) for c in callees[fn]]
        while q:
            x = q.popleft()
            if x == target:
                chain=[x]
                while chain[-1]!=fn: chain.append(prev[chain[-1]])
                print('direct:', ' <- '.join(chain)); break
            for y in callees.get(x, ()):
                if y not in prev: prev[y]=x; q.append(y)
        else: print('direct: no path')
