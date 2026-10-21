#!/usr/bin/env python3
"""rocheck.py GROUPDIR NAME : compare the function's .rodata literals (from our compile) with image A bytes.
For each hi/lo reloc pair against .rodata, retail address comes from the target words; our value is
.rodata[addend]; print mismatches and a summary."""
import json, struct, subprocess, sys
from pathlib import Path
IMG = open('/home/cburnes/projects/rush2049-decomp/build/ovl/a/image.bin', 'rb').read()
BASE = 0x8038a400
S = 'rush2049/scratch/frontier/w36b'
d, name = sys.argv[1], sys.argv[2]
gn = 'G_' + Path(d).name
subprocess.run(['ssh', 'watchman2', f'mkdir -p {S}/cand/{gn}'], check=True)
subprocess.run(['scp', '-q', d + '/group.c', d + '/group.json', f'watchman2:{S}/cand/{gn}/'], check=True)
cmd = (f"cd {S} && IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido "
       f"python3 gw2.py cand/{gn} {name} '-g0 -O3 -mips2 -G 0 -non_shared' asm/us/ovl_a")
p = subprocess.run(['ssh', 'watchman2', cmd], capture_output=True, text=True)
if p.returncode: print(p.stderr[-2000:]); sys.exit(1)
j = json.loads(p.stdout)
want, got = j['want'], j['got']
ro = bytes.fromhex(j['rodata'])
relocs = j['relocs']
his = {}
los = []
for r in relocs:
    if r['t'] == 5: his[r['w']] = r
    elif r['t'] == 6: los.append(r)
def simm(x): return x - 0x10000 if x & 0x8000 else x
bad = 0; ok = 0; skipped = 0
for lo in los:
    if lo['sec'] == 0 or lo['type'] == 2: continue
    w = want[lo['w']]
    lo16 = simm(w & 0xffff)
    rs = (w >> 21) & 31
    # find the hi: nearest earlier lui with rt == rs
    hiw = None
    for k in range(lo['w'] - 1, max(-1, lo['w'] - 40), -1):
        x = want[k]
        if (x >> 26) == 0x0f and ((x >> 16) & 31) == rs and (x & 0xffff) in (0x803b, 0x803c):
            hiw = k; break
    if hiw is None: skipped += 1; continue
    addr = ((want[hiw] & 0xffff) << 16) + lo16
    addr &= 0xffffffff
    # our rodata section: the reloc on a section symbol, addend = lo16 field of OUR word (+ hi adjust)
    ow = got[lo['w']]
    oadd = simm(ow & 0xffff)
    ohi = got[hiw] & 0xffff if hiw in his else 0
    off = oadd  # .rodata+off (small positive)
    op = (w >> 26)
    size = 8 if op in (0x35, 0x3d) else 4   # ldc1/sdc1
    ours = ro[off:off + size]
    retail = IMG[addr - BASE: addr - BASE + size]
    if ours == retail: ok += 1
    else:
        bad += 1
        def fl(b):
            return struct.unpack(">f", b[:4])[0] if len(b) >= 4 else None
        print(f'MISMATCH word {lo["w"]}: ours .rodata+{off:#x} = {ours.hex()}  retail {addr:#x} = {retail.hex()}  ({fl(ours)} vs {fl(retail)})')
print(f'{name}: rodata sites ok={ok} bad={bad} skipped={skipped}')
# pool dump
offs = sorted({simm(got[lo['w']] & 0xffff) for lo in los if lo['sec'] != 0 and lo['type'] != 2})
if offs:
    lo_, hi_ = offs[0], offs[-1]
    print('our pool (offset: float):', ' '.join(f'{o:#x}:{struct.unpack(">f", ro[o:o+4])[0]:.7g}' for o in range(lo_, hi_ + 4, 4) if o + 4 <= len(ro)))
