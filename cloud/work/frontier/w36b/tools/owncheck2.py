#!/usr/bin/env python3
"""owncheck2.py SRC(dir|file) NAME [img] : verify NAME's own .data/.rodata references (and jump tables) against image A/B bytes.
Each hi/lo pair against a section symbol: our offset = (hi<<16)+lo from our words (+symbol value), retail address from the
target words; compares the bytes the instruction accesses (lw/sw 4, lh/sh 2, lb/sb 1, l/s.s 4, addiu: 4 bytes or the whole
string up to NUL for .rodata strings). Jump-table words (rodata relocs against .text) are compared as targets relative to NAME."""
import json, struct, subprocess, sys
from pathlib import Path
src, name = sys.argv[1], sys.argv[2]
img = sys.argv[3] if len(sys.argv) > 3 else 'a'
IMG = open(f'/home/cburnes/projects/rush2049-decomp/build/ovl/{img}/image.bin', 'rb').read()
BASE = 0x8038A400
S = 'rush2049/scratch/frontier/w36b'
if Path(src).is_dir():
    gn = 'O_' + Path(src).name
    subprocess.run(['ssh', 'watchman2', f'mkdir -p {S}/cand/{gn}'], check=True)
    subprocess.run('scp -q %s/*.c %s/group.json watchman2:%s/cand/%s/' % (src, src, S, gn), shell=True, check=True)
    tgt = f'cand/{gn}'
else:
    subprocess.run(['scp', '-q', src, f'watchman2:{S}/cand/{name}.c'], check=True); tgt = f'cand/{name}.c'
cmd = (f"cd {S} && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido "
       f"python3 gw3.py {tgt} {name} '-g0 -O3 -mips2 -G 0 -non_shared' asm/us/ovl_{img}")
p = subprocess.run(['ssh', 'watchman2', cmd], capture_output=True, text=True)
if p.returncode: print(p.stderr[-2000:]); sys.exit(1)
j = json.loads(p.stdout)
want, got, secs = j['want'], j['got'], {k: bytes.fromhex(v) for k, v in j['secs'].items()}
func_addr = None
import re
sa = json.load(open(f'/home/cburnes/projects/rush2049-decomp/asm/us/ovl_{img}/symbols.json'))['symbols']
func_addr = int(sa[name], 16) if name in sa else int(name.split('_')[1], 16)
def simm(x): return x - 0x10000 if x & 0x8000 else x
his = {}
ok = bad = 0
pending_hi = {}
for r in j['relocs']:
    if r['secname'] not in ('.data', '.rodata', '.bss'): continue
    w = r['w']
    if r['t'] == 5:
        pending_hi[r['secname']] = w; continue
    if r['t'] != 6: continue
    hw = pending_hi.get(r['secname'])
    if hw is None: print('no hi for', r); continue
    ours = ((got[hw] & 0xFFFF) << 16) + simm(got[w] & 0xFFFF) + r['value']
    retail = ((want[hw] & 0xFFFF) << 16) + simm(want[w] & 0xFFFF)
    op = want[w] >> 26
    size = {0x23: 4, 0x2b: 4, 0x31: 4, 0x39: 4, 0x21: 2, 0x25: 2, 0x29: 2, 0x20: 1, 0x24: 1, 0x28: 1, 0x35: 8, 0x3d: 8}.get(op)
    sec = secs.get(r['secname'], b'')
    if r['secname'] == '.bss':
        print(f'  .bss+{ours:#x} at w{w}: retail {retail:#x} (not checkable)'); continue
    if size is None:  # address formation: compare 4 bytes, or a string
        o = ours; a = retail - BASE
        s1 = sec[o:o+64].split(b'\0')[0]
        if r['secname'] == '.rodata' and len(s1) >= 2 and all(32 <= c < 127 for c in s1):
            size = len(s1) + 1
        else: size = 4
    mine = sec[ours:ours+size]; theirs = IMG[retail-BASE:retail-BASE+size]
    if mine == theirs: ok += 1
    else:
        bad += 1; print(f'  MISMATCH w{w} {r["secname"]}+{ours:#x} size {size}: ours {mine.hex()} retail {retail:#x} {theirs.hex()}')
# jump tables
jt_ok = jt_bad = 0
fns = j['fns']; start = j['start']
for r in j['rorel']:
    if r['secname'] != '.text': continue
    off = r['off']
    ourword = struct.unpack('>I', secs['.rodata'][off:off+4])[0]
    # our target text offset relative to NAME
    rel = ourword + r['value'] - start
    if rel < 0 or rel >= len(got) * 4: continue
    # find the retail word: search the jump table base from the lui/addiu (lw) pair referencing this rodata offset
    jt_ok += 0
    r['rel'] = rel
tables = {}
for r in j['relocs']:
    pass
print(f'{name}: own data refs ok={ok} bad={bad}')
# jump table check: for each lw-from-.rodata reference of the form lui at / addu / lw t,lo(at), compare entries
for r in j['relocs']:
    if r['secname'] != '.rodata' or r['t'] != 6: continue
    w = r['w']; op = want[w] >> 26
    if op != 0x23: continue
    hw = None
    for k in range(w - 1, max(0, w - 6), -1):
        if (want[k] >> 26) == 0x0f: hw = k; break
    if hw is None: continue
    ours = ((got[hw] & 0xFFFF) << 16) + simm(got[w] & 0xFFFF) + r['value']
    retail = ((want[hw] & 0xFFFF) << 16) + simm(want[w] & 0xFFFF)
    if not (0 <= ours < len(secs['.rodata'])): continue
    # is it a jump table? check rorel entries at ours
    ent = [x for x in j['rorel'] if x['secname'] == '.text' and ours <= x['off'] < ours + 4 * 64]
    if not ent: continue
    n = 0; good = 0
    for x in sorted(ent, key=lambda e: e['off']):
        k = (x['off'] - ours) // 4
        if x['off'] != ours + 4 * k: continue
        if k != n: break
        ourw = struct.unpack('>I', secs['.rodata'][x['off']:x['off']+4])[0] + x['value'] - start
        rw = struct.unpack('>I', IMG[retail - BASE + 4*k: retail - BASE + 4*k + 4])[0] - func_addr
        good += (ourw == rw); n += 1
    print(f'  jump table at .rodata+{ours:#x} (retail {retail:#x}): {good}/{n} entries match')
