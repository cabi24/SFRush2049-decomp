"""w36n: shared loaders for the MAT3 (GameCube rush_d.elf) -> N64 function map.

Read-only. Standard library only. N64 side: build/game_code.bin + build/blob_layout.json (game),
build/ovl/{a,b}/image.bin + asm/us/ovl_{a,b}/extents.json (runtime images A/B), asm/us/blob/symbols.json
(static names). GC side: docs/external/gc/functions.json, callgraph.json, reference/gc/disasm/**/*.s,
reference/gc/features.json, reference/repos/mat3/rush_d_functions.csv.
"""
import csv
import glob
import json
import os
import re
import struct
import sys
from collections import OrderedDict, defaultdict

REPO = '/home/cburnes/projects/rush2049-decomp'
sys.path.insert(0, REPO)
GAME_BASE = 0x80086A50
OVL_BASE = 0x8038A400


def sext16(v):
    return v - 0x10000 if v & 0x8000 else v


# ------------------------------------------------------------------------------------------------ N64
class Img(object):
    def __init__(self, pop, data, base, funcs):
        self.pop, self.data, self.base = pop, data, base
        self.end = base + len(data)
        self.funcs = sorted(funcs)  # (addr, size, name)
        self.by_addr = {a: n for a, s, n in self.funcs}

    def word(self, a):
        o = a - self.base
        return struct.unpack_from('>I', self.data, o)[0] if 0 <= o <= len(self.data) - 4 else None


def load_n64():
    lay = json.load(open(os.path.join(REPO, 'build', 'blob_layout.json')))
    gfun = []
    for reg in lay['regions']:
        for e in reg['entries']:
            if e['kind'] == 'function':
                gfun.append((e['vaddr'], e['size'], e['target_id']))
    game = Img('game', open(os.path.join(REPO, 'build', 'game_code.bin'), 'rb').read(), GAME_BASE, gfun)
    imgs = OrderedDict(game=game)
    for t in 'ab':
        ext = json.load(open(os.path.join(REPO, 'asm', 'us', 'ovl_' + t, 'extents.json')))
        of = [(int(f['address'], 16), f['size'], f['name']) for f in ext['functions']]
        data = open(os.path.join(REPO, 'build', 'ovl', t, 'image.bin'), 'rb').read()
        imgs['ovl_' + t] = Img('ovl_' + t, data, int(ext['base'], 16), of)
    syms = json.load(open(os.path.join(REPO, 'asm', 'us', 'blob', 'symbols.json')))['symbols']
    static = {}
    for n, a in syms.items():
        a = int(a, 16)
        if 0x80000400 <= a < GAME_BASE and not n.startswith('D_') and not n.startswith('g'):
            static.setdefault(a, n)
    imgs['static'] = load_static(static)
    for a, sz, n in imgs['static'].funcs:
        static.setdefault(a, n)
    return imgs, static


def load_static(static_names):
    """Static code (boot segment, ROM 0x1000 -> 0x80000400): function starts from the built ELF's FUNC symbols and
    asm/us/boot_tail/extents.json; names: game-side names (blob symbols.json) first, then symbol_addrs, then ELF."""
    import subprocess
    rom = open(os.path.join(REPO, 'baserom.us.z64'), 'rb').read()
    base, lo, hi = 0x80000400, 0x1000, 0x2F4E0
    data = rom[lo:hi]
    starts = {}
    try:
        out = subprocess.check_output(['readelf', '-sW', os.path.join(REPO, 'build', 'us', 'rush2049.us.elf')]).decode()
        for line in out.splitlines():
            f = line.split()
            if len(f) >= 8 and f[3] == 'FUNC':
                a, sz = int(f[1], 16), int(f[2])
                if base <= a < 0x8000F400 and sz > 0:
                    starts.setdefault(a, (sz, f[7]))
    except Exception:
        pass
    ext = json.load(open(os.path.join(REPO, 'asm', 'us', 'boot_tail', 'extents.json')))
    for fn in ext['functions']:
        a = int(fn['address'], 16)
        starts.setdefault(a, (fn['size'], fn['name']))
    symaddr = {}
    for line in open(os.path.join(REPO, 'symbol_addrs.us.txt')):
        m = re.match(r'\s*([A-Za-z_]\w*)\s*=\s*0x([0-9A-Fa-f]+)\s*;.*type:func', line)
        if m:
            symaddr.setdefault(int(m.group(2), 16), m.group(1))
    funcs = []
    for a in sorted(starts):
        sz, en = starts[a]
        name = static_names.get(a) or symaddr.get(a) or en
        funcs.append((a, sz, name))
    return Img('static', data, base, funcs)


def n64_id(pop, name):
    return '%s:%s' % (pop, name)


def n64_calls(imgs, static):
    """Ordered callee lists: {id: [(pc, target_id, kind)]} with kind 'jal' or 'j' (tail call)."""
    game = imgs['game']
    out = {}
    for pop, img in imgs.items():
        for addr, size, name in img.funcs:
            seq = []
            for pc in range(addr, addr + size, 4):
                w = img.word(pc)
                if w is None:
                    break
                op = w >> 26
                if op not in (2, 3):
                    continue
                t = (pc & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
                if op == 2 and addr <= t < addr + size:
                    continue
                tid = None
                if img.base <= t < img.end and t in img.by_addr:
                    tid = n64_id(pop, img.by_addr[t])
                elif GAME_BASE <= t < game.end and t in game.by_addr:
                    tid = n64_id('game', game.by_addr[t])
                elif t in static:
                    tid = n64_id('static', static[t])
                elif t < GAME_BASE:
                    tid = n64_id('static', 'func_%08X' % t)
                if tid is None:
                    tid = '?:%08X' % t
                seq.append((pc, tid, 'jal' if op == 3 else 'j'))
            out[n64_id(pop, name)] = seq
    return out


def n64_population():
    """{id: dict(pop, name, address, size, matched)}"""
    imgs, static = load_n64()
    blob_lock = json.load(open(os.path.join(REPO, 'blob_matched.lock.json')))
    ovl_lock = json.load(open(os.path.join(REPO, 'ovl_matched.lock.json')))
    st_lock = set(v[0]['target_id'] if isinstance(v, list) else v.get('target_id', k.split(':')[-1])
                  for k, v in json.load(open(os.path.join(REPO, 'matched.lock.json'))).items())
    pop = OrderedDict()
    for p, img in imgs.items():
        for a, s, n in img.funcs:
            if p == 'game':
                m = n in blob_lock
            elif p == 'static':
                m = n in st_lock
            else:
                m = n in ovl_lock.get(p[-1], {})
            pop[n64_id(p, n)] = dict(pop=p, name=n, address=a, size=s, matched=m)
    return imgs, static, pop


# ------------------------------------------------------------------------------------------------- GC
GAMESOURCE_FILES = None


def load_gc():
    """GC functions: {key: dict(name, file, address, size, line, end_line, stripped, gamesource, calls)}.

    key = name, or name@file when the name occurs in several files (callgraph.json convention)."""
    fj = json.load(open(os.path.join(REPO, 'docs', 'external', 'gc', 'functions.json')))['functions']
    cg = json.load(open(os.path.join(REPO, 'docs', 'external', 'gc', 'callgraph.json')))['functions']
    rows = list(csv.DictReader(open(os.path.join(REPO, 'reference', 'repos', 'mat3', 'rush_d_functions.csv'))))
    namecount = defaultdict(set)
    for r in rows:
        namecount[r['name']].add(r['source_file'])
    for k, v in cg.items():
        namecount[k.split('@')[0]].add(v.get('file') or '')
    gc = OrderedDict()

    def key_of(name, file):
        return name if len(namecount[name]) <= 1 else '%s@%s' % (name, file)

    for r in rows:
        k = key_of(r['name'], r['source_file'])
        if k in gc:
            continue
        gs = 'GameSource' in (r['source_path'] or '')
        gc[k] = dict(name=r['name'], file=r['source_file'],
                     address=int(r['address'], 16) if r['address'] and r['stripped'] != '1' else None,
                     size=int(r['size']) if r['size'] else 0,
                     line=int(r['line']) if r['line'] else None,
                     end_line=int(r['end_line']) if r['end_line'] else None,
                     stripped=r['stripped'] == '1', gamesource=gs, calls=[], debug=True,
                     scope=r['scope'])
    for k, v in cg.items():
        if k in gc or (k.split('@')[0] in gc and '@' not in k):
            continue
        name = k.split('@')[0]
        kk = key_of(name, v.get('file') or '')
        if kk in gc:
            continue
        gc[kk] = dict(name=name, file=v.get('file') or '', address=int(v['address'], 16), size=v['size'],
                      line=None, end_line=None, stripped=False, gamesource=False, calls=[], debug=False,
                      scope='global')
    by_addr = {}
    for k, v in gc.items():
        if v['address'] is not None and not v['stripped']:
            by_addr.setdefault(v['address'], k)
    # ordered calls from the disassembly
    for path in glob.glob(os.path.join(REPO, 'reference', 'gc', 'disasm', '*', '*.s')):
        with open(path) as fh:
            head = fh.readline()
            m = re.match(r'/\* (\S+)\s+(\S+)\s+0x([0-9A-Fa-f]+) size 0x([0-9A-Fa-f]+)', head)
            if not m:
                continue
            start, size = int(m.group(3), 16), int(m.group(4), 16)
            k = by_addr.get(start)
            if k is None:
                continue
            seq = []
            for line in fh:
                mm = re.match(r'/\* ([0-9A-F]{8}) [0-9A-F]{4} ([0-9A-F]{8}) \*/', line)
                if not mm:
                    continue
                pc, w = int(mm.group(1), 16), int(mm.group(2), 16)
                if w >> 26 != 18:
                    continue
                li = w & 0x03FFFFFC
                if li & 0x02000000:
                    li -= 0x04000000
                t = (li if (w >> 1) & 1 else pc + li) & 0xFFFFFFFF
                lk = w & 1
                if not lk and start <= t < start + size:
                    continue
                tk = by_addr.get(t)
                seq.append((pc, tk or '?:%08X' % t, 'bl' if lk else 'b'))
            gc[k]['calls'] = seq
    return gc


def gc_features():
    p = os.path.join(REPO, 'reference', 'gc', 'features.json')
    return json.load(open(p))


def gc_file_order(gc):
    """{file: [keys in original source order]} including stripped functions (line order)."""
    per = defaultdict(list)
    for k, v in gc.items():
        if v['line'] is not None:
            per[v['file']].append((v['line'], k))
    return {f: [k for l, k in sorted(x)] for f, x in per.items()}
