"""w36y: global-reference extraction for the MAT3 (GameCube) -> N64 DATA symbol map.

Read-only, standard library only.

N64 side: every function of the game image and runtime images A/B (loaders shared with lane w36n:
cloud/work/frontier/w36n/tools/data.py, imported read-only) is decoded; `lui` high halves are carried through
`addiu`/`addu`/moves, and every load/store/address formation that resolves to a fixed address is recorded with its
access kind (b/h/w/f/d + signedness, store/load/addr), its mode (direct, indexed = an index register was added to
the base, field = displacement off a register holding a formed address) and, for field accesses, the base address.

GC side: the symbolised disassembly (reference/gc/disasm/<file>/<function>.s; relocations are exact) gives
`sym@sda21`, `sym@ha`/`sym@l` references; the same register tracking gives field and indexed accesses. Symbol starts
and sizes come from reference/repos/mat3/decomp/config/2049LE/symbols.txt.
"""
import glob
import json
import os
import re
import struct
import sys
from collections import OrderedDict, defaultdict

REPO = '/home/cburnes/projects/rush2049-decomp'
sys.path.insert(0, os.path.join(REPO, 'cloud', 'work', 'frontier', 'w36n', 'tools'))
import data as w36n_data  # noqa: E402  (read-only reuse of w36n's loaders)

GAME_BASE = 0x80086A50
OVL_BASE = 0x8038A400

# ------------------------------------------------------------------------------------------------ N64 decode
LOADS = {0x20: ('b', 's'), 0x24: ('b', 'u'), 0x21: ('h', 's'), 0x25: ('h', 'u'), 0x23: ('w', ''),
         0x31: ('f', ''), 0x35: ('d', ''), 0x22: ('wl', ''), 0x26: ('wr', '')}
STORES = {0x28: 'b', 0x29: 'h', 0x2B: 'w', 0x39: 'f', 0x3D: 'd', 0x2A: 'wl', 0x2E: 'wr'}
CALLER_SAVED = set(range(1, 16)) | {24, 25, 31}


def sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def n64_refs_of(img, addr, size, lo_ok):
    """Ordered references of one N64 function.

    Returns a list of dicts: pc, addr (absolute address accessed or formed), kind ('lb','lbu','lh','lhu','lw',
    'lf','ld','sb','sh','sw','sf','sd','addr'), mode ('direct','indexed','field'), base (formed base for field
    accesses, else None), off (displacement for field accesses)."""
    regs = {}
    out = []
    end = addr + size
    pc = addr
    pending_clobber = False
    while pc < end:
        w = img.word(pc)
        if w is None:
            break
        op = w >> 26
        rs = (w >> 21) & 31
        rt = (w >> 16) & 31
        rd = (w >> 11) & 31
        imm = w & 0xFFFF
        clobber_after = False
        dest = None
        newval = None
        if op == 0x0F:  # lui
            dest, newval = rt, ('hi', imm << 16, False)
        elif op == 0x09:  # addiu
            v = regs.get(rs)
            dest = rt
            if v and v[0] == 'hi':
                a = (v[1] + sext16(imm)) & 0xFFFFFFFF
                if lo_ok(a):
                    newval = ('addr', a, v[2])
                    out.append(dict(pc=pc, addr=a, kind='addr', mode='indexed' if v[2] else 'direct', base=None,
                                    off=0))
            elif v and v[0] == 'addr':
                newval = ('addr', (v[1] + sext16(imm)) & 0xFFFFFFFF, v[2], v[3] if len(v) > 3 else v[1])
        elif op in LOADS or op in STORES:
            v = regs.get(rs)
            if v:
                if v[0] == 'hi':
                    a = (v[1] + sext16(imm)) & 0xFFFFFFFF
                    mode, base, off = ('indexed' if v[2] else 'direct'), None, 0
                else:
                    a = (v[1] + sext16(imm)) & 0xFFFFFFFF
                    mode, base, off = ('indexed' if v[2] else 'field'), (v[3] if len(v) > 3 else v[1]), sext16(imm)
                if lo_ok(a):
                    if op in LOADS:
                        k, sg = LOADS[op]
                        kind = {'b': 'lb', 'h': 'lh', 'w': 'lw', 'f': 'lf', 'd': 'ld', 'wl': 'lwl', 'wr': 'lwr'}[k]
                        if sg == 'u':
                            kind += 'u'
                    else:
                        kind = 's' + STORES[op]
                    out.append(dict(pc=pc, addr=a, kind=kind, mode=mode, base=base, off=off))
            if op in LOADS and op not in (0x31, 0x35):
                dest = rt
        elif op == 0:  # SPECIAL
            funct = w & 0x3F
            if funct in (0x21, 0x25, 0x20):  # addu / or / add
                a_, b_ = regs.get(rs), regs.get(rt)
                dest = rd
                if rt == 0 and a_:
                    newval = a_
                elif rs == 0 and b_:
                    newval = b_
                elif a_ and not b_:
                    newval = a_[:2] + (True,) + a_[3:]
                elif b_ and not a_:
                    newval = b_[:2] + (True,) + b_[3:]
            elif funct in (0x08, 0x09):  # jr / jalr
                if funct == 0x09:
                    dest = rd
                    clobber_after = True
            elif funct in (0x18, 0x19, 0x1A, 0x1B, 0x0C, 0x0D):
                pass
            else:
                dest = rd
        elif op == 0x03:  # jal
            clobber_after = True
        elif op in (0x08, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E):  # addi slti sltiu andi ori xori
            dest = rt
        elif op == 0x11:  # COP1: mfc1/cfc1 write GPR rt
            if rs in (0, 2):
                dest = rt
        if dest:
            if newval is None:
                regs.pop(dest, None)
            else:
                regs[dest] = newval
        if pending_clobber:
            for r in CALLER_SAVED:
                regs.pop(r, None)
            pending_clobber = False
        if clobber_after:
            pending_clobber = True  # after the delay slot
        pc += 4
    return out


class N64(object):
    def __init__(self):
        self.imgs, self.static = w36n_data.load_n64()
        game = self.imgs['game']
        # data ranges: static data 0x80000400..GAME_BASE text excluded by checking words? keep any address that
        # is not inside a function body of a code image.
        self.code = []
        for pop, img in self.imgs.items():
            if pop == 'static':
                continue
            if img.funcs:
                a0 = img.funcs[0][0]
                a1 = max(a + s for a, s, n in img.funcs)
                self.code.append((pop, a0, a1))
        self.refs = OrderedDict()
        for pop in ('game', 'ovl_a', 'ovl_b'):
            img = self.imgs[pop]
            for a, s, n in img.funcs:
                self.refs['%s:%s' % (pop, n)] = n64_refs_of(img, a, s, self.lo_ok)
        self.text_end = {'game': 0x8010FD7C, 'ovl_a': 0x803AF978, 'ovl_b': 0x80393B34}
        self.img_end = {p: self.imgs[p].end for p in ('game', 'ovl_a', 'ovl_b')}

    @staticmethod
    def lo_ok(a):
        return 0x80000000 <= a < 0x80800000

    def is_rodata(self, pop, a):
        """True for the image's own read-only/initialised area (after text, inside the image)."""
        if GAME_BASE <= a < self.img_end['game']:
            return a >= self.text_end['game']
        if pop in ('ovl_a', 'ovl_b') and OVL_BASE <= a < self.img_end[pop]:
            return a >= self.text_end[pop]
        return False


# ------------------------------------------------------------------------------------------------- GC decode
GC_SYMS_TXT = os.path.join(REPO, 'reference', 'repos', 'mat3', 'decomp', 'config', '2049LE', 'symbols.txt')
GC_DISASM = os.path.join(REPO, 'reference', 'gc', 'disasm')
LINE_RE = re.compile(r'/\* ([0-9A-F]{8}) [0-9A-F]{4} ([0-9A-F]{8}) \*/\s+(\S+)\s*(.*?)\s*(?:#\s*(.*))?$')
RELOC_RE = re.compile(r'([A-Za-z_$@.][\w$@.]*)@(sda21|ha|l|h)')
GC_LOADS = {'lbz': 'lbu', 'lha': 'lh', 'lhz': 'lhu', 'lwz': 'lw', 'lfs': 'lf', 'lfd': 'ld',
            'lbzu': 'lbu', 'lhau': 'lh', 'lhzu': 'lhu', 'lwzu': 'lw', 'lfsu': 'lf', 'lfdu': 'ld'}
GC_STORES = {'stb': 'sb', 'sth': 'sh', 'stw': 'sw', 'stfs': 'sf', 'stfd': 'sd',
             'stbu': 'sb', 'sthu': 'sh', 'stwu': 'sw', 'stfsu': 'sf', 'stfdu': 'sd'}
GC_XLOADS = {'lbzx': 'lbu', 'lhax': 'lh', 'lhzx': 'lhu', 'lwzx': 'lw', 'lfsx': 'lf', 'lfdx': 'ld',
             'stbx': 'sb', 'sthx': 'sh', 'stwx': 'sw', 'stfsx': 'sf', 'stfdx': 'sd'}
NO_DEST = ('st', 'cmp', 'b', 'mt', 'cr', 'dcb', 'icb', 'sync', 'isync', 'tw', 'psq_st', 'fcmp', 'mcrf')


def load_gc_syms():
    syms = {}
    for line in open(GC_SYMS_TXT):
        m = re.match(r'(\S+) = (\.\w+):0x([0-9A-Fa-f]+); // type:(\w+)(?: size:0x([0-9A-Fa-f]+))?(?: scope:(\w+))?', line)
        if not m:
            continue
        name, sec, a, typ, sz, scope = m.groups()
        if typ == 'function':
            continue
        syms.setdefault(name, []).append(dict(name=name, section=sec, address=int(a, 16), size=int(sz or '0', 16),
                                              scope=scope))
    return syms


def gc_refs_file(path, symstart):
    """(header, refs) of one GC function file."""
    regs = {}
    out = []
    with open(path) as fh:
        head = fh.readline()
        for line in fh:
            m = LINE_RE.match(line)
            if not m:
                continue
            pc = int(m.group(1), 16)
            mn = m.group(3)
            ops = m.group(4)
            cmt = m.group(5)
            parts = [p.strip() for p in ops.split(',')] if ops else []
            rel = RELOC_RE.search(ops or '')
            dest = None
            newval = None
            mnb = mn.rstrip('.')
            if mnb == 'bl':
                for r in [0] + list(range(3, 13)):
                    regs.pop(r, None)
                continue
            if rel and rel.group(1).startswith('@'):
                rel = None  # compiler float/string constant
            if rel:
                sym, kind_r = rel.group(1), rel.group(2)
                full = int(cmt.split()[0], 16) if cmt and cmt.split()[0].startswith('0x') else None
                if kind_r == 'ha' and mnb == 'lis':
                    dest = _reg(parts[0])
                    newval = ('ha', sym)
                elif kind_r in ('l', 'sda21'):
                    if mnb in ('addi', 'li', 'la'):
                        dest = _reg(parts[0])
                        a = full
                        if a is not None:
                            st0 = symstart(sym, a)
                            newval = ('addr', sym, a - st0, False, st0)
                            out.append(dict(pc=pc, sym=sym, start=st0, off=a - st0, kind='addr', mode='direct'))
                    elif mnb in GC_LOADS or mnb in GC_STORES:
                        a = full
                        if a is not None:
                            k = GC_LOADS.get(mnb) or GC_STORES.get(mnb)
                            st0 = symstart(sym, a)
                            out.append(dict(pc=pc, sym=sym, start=st0, off=a - st0, kind=k, mode='direct'))
                        if mnb in GC_LOADS and not mnb.startswith('lf'):
                            dest = _reg(parts[0])
                    else:
                        if parts and not mnb.startswith(NO_DEST):
                            dest = _reg(parts[0])
            else:
                if mnb in GC_LOADS or mnb in GC_STORES:
                    mm = re.match(r'(-?0x[0-9a-fA-F]+|-?\d+)\((r\d+)\)', parts[1]) if len(parts) > 1 else None
                    if mm:
                        b = _reg(mm.group(2))
                        v = regs.get(b)
                        if v and v[0] == 'addr':
                            d = int(mm.group(1), 0)
                            k = GC_LOADS.get(mnb) or GC_STORES.get(mnb)
                            out.append(dict(pc=pc, sym=v[1], start=v[4], off=v[2] + d, kind=k,
                                            mode='indexed' if v[3] else 'field'))
                    if mnb in GC_LOADS and not mnb.startswith('lf'):
                        dest = _reg(parts[0])
                elif mnb in GC_XLOADS:
                    a_, b_ = regs.get(_reg(parts[1])), regs.get(_reg(parts[2]))
                    v = a_ if (a_ and a_[0] == 'addr') else (b_ if (b_ and b_[0] == 'addr') else None)
                    if v:
                        out.append(dict(pc=pc, sym=v[1], start=v[4], off=v[2], kind=GC_XLOADS[mnb], mode='indexed'))
                    if mnb.startswith('l') and not mnb.startswith('lf'):
                        dest = _reg(parts[0])
                elif mnb == 'mr':
                    dest = _reg(parts[0])
                    newval = regs.get(_reg(parts[1]))
                elif mnb == 'add':
                    dest = _reg(parts[0])
                    a_, b_ = regs.get(_reg(parts[1])), regs.get(_reg(parts[2]))
                    v = a_ if (a_ and a_[0] == 'addr') else (b_ if (b_ and b_[0] == 'addr') else None)
                    if v:
                        newval = (v[0], v[1], v[2], True, v[4])
                elif mnb == 'addi' and len(parts) == 3:
                    dest = _reg(parts[0])
                    v = regs.get(_reg(parts[1]))
                    try:
                        d = int(parts[2], 0)
                    except ValueError:
                        d = None
                    if v and v[0] == 'addr' and d is not None:
                        newval = ('addr', v[1], v[2] + d, v[3], v[4])
                elif parts and not mnb.startswith(NO_DEST) and not mnb.startswith('f') and not mnb.startswith('ps_'):
                    dest = _reg(parts[0])
            if dest is not None:
                if newval is None:
                    regs.pop(dest, None)
                else:
                    regs[dest] = newval
    return head, out


def _reg(s):
    m = re.match(r'r(\d+)$', s.strip()) if s else None
    return int(m.group(1)) if m else None


class GC(object):
    def __init__(self):
        self.syms = load_gc_syms()
        self.by_addr = {}
        for n, lst in self.syms.items():
            for s in lst:
                self.by_addr.setdefault(s['address'], s)
        self.files = {}
        for path in glob.glob(os.path.join(GC_DISASM, '*', '*.s')):
            with open(path) as fh:
                head = fh.readline()
            m = re.match(r'/\* (\S+)\s+(\S+)\s+0x([0-9A-Fa-f]+) size 0x([0-9A-Fa-f]+)', head)
            if m:
                self.files[int(m.group(3), 16)] = (path, m.group(1), m.group(2), int(m.group(4), 16))
        self._cache = {}
        gj = json.load(open(os.path.join(REPO, 'docs', 'external', 'gc', 'globals.json')))['globals']
        self.types = {}
        for g in gj:
            self.types.setdefault((g['name'], int(g['address'], 16)), g)

    def symstart(self, sym, a):
        cands = self.syms.get(sym) or []
        best = None
        for s in cands:
            if s['address'] <= a < s['address'] + max(s['size'], 1):
                return s['address']
            if best is None or abs(s['address'] - a) < abs(best - a):
                best = s['address']
        return best if best is not None else a

    def sym_info(self, sym, start):
        for s in self.syms.get(sym, []):
            if s['address'] == start:
                return s
        return None

    def type_of(self, sym, start):
        return self.types.get((sym, start))

    def refs(self, gc_addr):
        if gc_addr in self._cache:
            return self._cache[gc_addr]
        f = self.files.get(gc_addr)
        r = gc_refs_file(f[0], self.symstart)[1] if f else None
        if r:
            for x in r:
                self.reresolve(x)
        self._cache[gc_addr] = r
        return r

    def reresolve(self, x):
        """CodeWarrior addresses neighbouring statics off one base register (`namePanes@l` + 0x1B0): an offset
        outside the symbol's extent is re-attributed to the symbol that contains the absolute address."""
        info = self.sym_info(x['sym'], x['start'])
        size = info['size'] if info else 0
        if size and 0 <= x['off'] < size:
            return
        a = x['start'] + x['off']
        import bisect
        if not hasattr(self, '_sorted'):
            self._sorted = sorted((s['address'], s['size'], s['name']) for lst in self.syms.values() for s in lst
                                  if s['size'])
            self._keys = [t[0] for t in self._sorted]
        i = bisect.bisect_right(self._keys, a) - 1
        if i >= 0:
            sa, ss, sn = self._sorted[i]
            if sa <= a < sa + ss:
                x['sym'], x['start'], x['off'] = sn, sa, a - sa
                x['reresolved'] = True


def _gc_key(self, sym, start):
    """Unique key of a GC data symbol: the name, or name@0xADDR when several symbols share the name."""
    if len(self.syms.get(sym, [])) <= 1:
        return sym
    return '%s@0x%08X' % (sym, start)


GC.key = _gc_key
