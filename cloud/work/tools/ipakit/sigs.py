#!/usr/bin/env python3
"""sigs.py: parameter signature inference from the retail words (stdlib only).

    python3 cloud/work/tools/ipakit/sigs.py NAME... [--json] [--evidence]
    python3 cloud/work/tools/ipakit/sigs.py --all [--json]
    python3 cloud/work/tools/ipakit/sigs.py --validate [--json] [--only matches|groups|blob]

One C prototype per function with a confidence and the evidence behind it.  This is the order/width
information m2c gets wrong for IPA functions (m2c sorts extra parameters by register NAME).

Evidence used (all static, from the game image):
  * registers read at entry: O32 inputs a0-a3 / f12 / f14 (incl. pass-through to callees, via analyze.py) and the
    non-ABI registers an IPA callee reads (t*, s*, f16.. ...).
  * HOME-SLOT stores `sw/swc1/sdc1/sh/sb R, frame+4k(sp)`: the slot word k is the parameter's position, whatever
    register R is (`sw t2,4(sp)` = parameter 1; `sw a0,24(sp)` = parameter 6).  Loads from `frame+16+` read
    stack-passed parameters.
  * width: `sll 16/sra 16` = s16, `sll 24/sra 24` = s8, `andi 0xff` = u8, `andi 0xffff` = u16 (also `srl` forms);
    caller side: a caller that prototype-converts an argument narrows it before the `jal` (same patterns).
  * float vs int: f12/f14 or non-ABI f-regs read at entry, `swc1`/`sdc1` home stores, `mtc1 aN` as first use;
    double when a `.d` op, `sdc1`/`ldc1` touches the register.  Pointer: the register is a load/store base.
  * return type: a caller reads v0 (int class) or f0 (float) after the call; none of them does -> void; functions
    nobody calls fall back on what the last instructions before `jr ra` write.
Ordering model (validated on the matched sources, see --validate): word slot known from a home store wins; an
unstored ABI register keeps its natural slot (a0=0 .. a3=3, f12=0, f14=1); the remaining registers (non-ABI, or an
ABI register whose natural slot is taken) fill the lowest free slots in register order at, v*, t*, s*, f*.
"""
import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from ipakit import ROOT, load_corpus  # noqa: E402
from ipakit import analyze, mipsdec  # noqa: E402
from ipakit.mipsdec import REGBIT, REGNAMES, mask_names  # noqa: E402

SP, RA = REGBIT['sp'], REGBIT['ra']
A_REGS = ['a0', 'a1', 'a2', 'a3']
ABI_INT_SLOT = {'a0': 0, 'a1': 1, 'a2': 2, 'a3': 3}
REG_ORDER = (['at', 'v0', 'v1'] + ['t%d' % i for i in range(10)] + ['s%d' % i for i in range(8)] + ['s8']
             + ['f%d' % i for i in range(32)])
CALLEE_SAVED_NAMES = set(['s%d' % i for i in range(8)] + ['s8', 'ra'] + ['f%d' % i for i in range(20, 32)])
ARG_CALL_BITS = 0
for _r in ('a0', 'a1', 'a2', 'a3', 'f12', 'f13', 'f14', 'f15'):
    ARG_CALL_BITS |= 1 << REGBIT[_r]
ARG_KILL = ARG_CALL_BITS | (1 << REGBIT['v0']) | (1 << REGBIT['v1'])
USE_HINT = False
HINT_MIN_SITES = 1
HINT_AGREE = False
MAX_SLOT = 12                       # word slots considered (stack-passed parameters)
WIDTH_C = {'w': 's32', 'u32': 'u32', 's16': 's16', 'u16': 'u16', 's8': 's8', 'u8': 'u8'}


class Param:
    __slots__ = ('slot', 'reg', 'kind', 'width', 'source', 'evidence', 'guessed')

    def __init__(self, reg=None, slot=None, kind='int', width='w', source='', guessed=False):
        self.slot, self.reg, self.kind, self.width, self.source = slot, reg, kind, width, source
        self.evidence = []
        self.guessed = guessed

    @property
    def cls(self):
        """Comparison class: f32 f64 ptr s8 u8 s16 u16 w."""
        if self.kind in ('f32', 'f64', 'ptr'):
            return self.kind
        return self.width

    def ctype(self):
        if self.kind == 'f32':
            return 'f32'
        if self.kind == 'f64':
            return 'f64'
        if self.kind == 'ptr':
            return 'void *'
        return WIDTH_C.get(self.width, 's32')

    def to_json(self):
        return {'slot': self.slot, 'reg': self.reg, 'type': self.ctype(), 'source': self.source,
                'guessed': self.guessed, 'evidence': self.evidence}


class Sig:
    def __init__(self, name):
        self.name = name
        self.params = []            # ordered
        self.ret = 'void'           # void | s32 | f32 | f64 | ptr | unknown
        self.ret_evidence = []
        self.confidence = 'low'
        self.notes = []
        self.ipa_regs = []          # non-ABI registers that carry parameters
        self.arity_callers = None   # (min, max) argument count seen at call sites
        self.override = {}          # param index -> C type text (groupgen borrows m2c's pointer types)

    def ret_ctype(self):
        return {'ptr': 'void *', 'unknown': 's32'}.get(self.ret, self.ret)

    @staticmethod
    def m2c_name(p, i):
        """Parameter name m2c uses in a body: a0..a3 -> arg0..arg3, non-ABI register -> ipa_<reg>, stack -> arg<slot>."""
        if p.reg in A_REGS:
            return 'arg%d' % A_REGS.index(p.reg)
        if p.reg in ('f12', 'f14'):
            return 'arg%d' % p.slot
        if p.reg is None:
            return 'arg%d' % p.slot if p.source.startswith('stack-load') else 'unused%d' % p.slot
        return 'ipa_%s' % ('fp' if p.reg == 's8' else p.reg)

    def prototype(self, names='arg'):
        """`names`: 'arg' -> arg0.. by position; 'm2c' -> arg<N> for aN, ipa_<reg> for non-ABI registers (what the
        m2c IPA mode writes in a body); 'reg' is an alias of 'm2c'; 'none' -> types only."""
        ps = []
        for i, p in enumerate(self.params):
            t = self.override.get(i, p.ctype())
            if names == 'none':
                ps.append(t)
                continue
            n = self.m2c_name(p, i) if names in ('m2c', 'reg') else 'arg%d' % i
            ps.append('%s%s%s' % (t, '' if t.endswith('*') else ' ', n))
        return '%s%s%s(%s);' % (self.ret_ctype(), '' if self.ret_ctype().endswith('*') else ' ', self.name,
                                ', '.join(ps) if ps else 'void')

    def to_json(self):
        return {'name': self.name, 'prototype': self.prototype(), 'confidence': self.confidence, 'ret': self.ret,
                'ret_evidence': self.ret_evidence, 'ipa_regs': self.ipa_regs, 'notes': self.notes,
                'arity_callers': self.arity_callers, 'params': [p.to_json() for p in self.params]}


# ---------------------------------------------------------------------------------------------------------
# per-function scan
# ---------------------------------------------------------------------------------------------------------

def _g(n):
    return 0 if n == 0 else 1 << n


def _sa(d):
    return d.word >> 6 & 31


def _ddef(ins):
    """ddef[i]: registers DEFINITELY written on every (forward) path reaching instruction i.  A register not in
    ddef[i] may still hold its entry value."""
    n = len(ins)
    pc2i = {d.pc: i for i, d in enumerate(ins)}
    inc = [None] * (n + 1)              # incoming definitely-defined mask per index
    inc[0] = 0
    for i in range(n):
        cur = inc[i]
        if cur is None:                 # unreachable so far (after an unconditional transfer): assume nothing known
            cur = 0
        d = ins[i]
        out = cur | d.defs
        if d.kind in ('jal', 'jalr'):
            out |= ARG_KILL
        prev = ins[i - 1] if i else None
        is_slot = prev is not None and prev.delay
        fall = True
        if is_slot:
            if prev.kind in ('b', 'j', 'ret', 'jr'):
                fall = False
            if prev.target is not None and prev.kind in ('b', 'j', 'branch'):
                t = pc2i.get(prev.target)
                if t is not None and t > i:
                    inc[t] = out if inc[t] is None else (inc[t] & out)
        if fall and i + 1 <= n:
            nxt = i + 1
            inc[nxt] = out if inc[nxt] is None else (inc[nxt] & out)
        elif not fall and inc[i + 1 if i + 1 <= n else n] is None:
            pass
    for i in range(n + 1):
        if inc[i] is None:
            inc[i] = 0
    return inc


def _narrow_at(ins, i, reg_n):
    """Narrowing pattern starting at instruction i reading register number reg_n: 's16' 'u16' 's8' 'u8' or None."""
    d = ins[i]
    if d.mnem == 'sll' and d.rt == reg_n:
        sa = _sa(d)
        if sa in (16, 24):
            for j in range(i + 1, min(i + 5, len(ins))):
                e = ins[j]
                if e.mnem in ('sra', 'srl') and e.rt == d.rd and _sa(e) == sa:
                    sign = e.mnem == 'sra'
                    return ('s' if sign else 'u') + ('16' if sa == 16 else '8')
                if e.defs & _g(d.rd) and e.mnem not in ('sra', 'srl'):
                    break
    elif d.mnem == 'andi' and d.rs == reg_n:
        m = d.word & 0xFFFF
        if m == 0xFF:
            return 'u8'
        if m == 0xFFFF:
            return 'u16'
    return None


def _store_width(mnem):
    return {'sb': 'u8', 'sh': 'u16'}.get(mnem)


def scan_function(corpus, f, info, callee_arity=None):
    """Raw per-register parameter evidence of one function (no ordering yet).

    -> dict(regs={reg: Param-ish dict}, frame=int)"""
    ins = mipsdec.decode_words(f.words, f.addr)
    n = len(ins)
    frame = info.frame
    if not frame:
        for d in ins:                   # the prologue may be scheduled late (after address setup); take the first adjustment
            if d.mnem == 'addiu' and d.rs == 29 and d.rt == 29 and d.imm < 0:
                frame = -d.imm
                break
            if d.kind == 'ret':
                break
    dd = _ddef(ins)
    evid = {}                           # reg name -> dict(kind,width,slot,ptr,use,...)

    def ent(r):
        return evid.setdefault(r, {'slot': None, 'kind': 'int', 'width': 'w', 'ptr': False, 'dbl': False, 'fl': False,
                                   'narrow': None, 'store_narrow': None, 'notes': [], 'stack': False})

    live = info.entry_live_mask
    reads = info.entry_reads_mask
    # registers the CFG liveness says are real inputs
    for r in mask_names(live & ~analyze.STRIP):
        if r in A_REGS or r in ('f12', 'f13', 'f14', 'f15') or (reads >> REGBIT[r] & 1):
            if r in ('f13', 'f15'):
                e = ent('f12' if r == 'f13' else 'f14')
                e['dbl'] = True
                continue
            ent(r)
    for r in mask_names(reads):
        ent(r)
    # stack-argument-area loads and home stores; narrowing / base use of original registers
    callee_arity = callee_arity or {}
    alias = {}                          # register number -> original param register name it is a copy/offset of
    for i, d in enumerate(ins):
        orig = lambda rn: not (dd[i] >> rn & 1)        # noqa: E731  register number rn may be original here
        if d.mem is not None and d.mem[0] in alias and d.kind in ('load', 'store') and d.mem[0] != 29:
            evid[alias[d.mem[0]]]['ptr'] = True
        if d.kind == 'jal' and d.target is not None:
            res, cf, altf = corpus.resolve(d.target)
            if cf is not None:
                for k, ar in enumerate(A_REGS):
                    if ar in evid and not evid[ar]['stack'] and orig(REGBIT[ar]):
                        evid[ar].setdefault('pass', []).append((cf.name, k))
        # copies: move / addiu keep the original pointer alive in another register
        for rn in range(1, 32):
            if d.defs >> rn & 1:
                alias.pop(rn, None)
        if d.kind == 'alu':
            src = None
            if d.mnem in ('or', 'addu', 'daddu') and d.rt == 0 and d.rs:
                src = d.rs
            elif d.mnem in ('addiu', 'ori') and d.rs and abs(d.imm) < 0x100:
                src = d.rs
            if src is not None:
                name = alias.get(src) or (REGNAMES[src] if REGNAMES[src] in evid and orig(src) else None)
                dst = d.rd if d.mnem in ('or', 'addu', 'daddu') else d.rt
                if name and dst and dst != src:
                    alias[dst] = name
        if d.mem is not None and d.mem[0] == 29:
            off = d.mem[1] - frame
            if d.kind == 'store':
                src = d.rt
                if d.mnem in ('swc1', 'sdc1'):
                    srcn = 32 + src
                else:
                    srcn = src
                if 0 <= off < 4 * MAX_SLOT and srcn not in (0, SP, RA) and orig(srcn) and d.mnem in (
                        'sw', 'sh', 'sb', 'swc1', 'sdc1'):
                    rname = REGNAMES[srcn]
                    if rname in CALLEE_SAVED_NAMES and off < 16 and rname != 'ra' and not (rname in evid):
                        continue            # prologue save of a callee-saved register into the frame, not a param
                    if rname in A_REGS or rname in ('f12', 'f14') or rname in evid or reads >> srcn & 1 or (
                            rname not in CALLEE_SAVED_NAMES and (live >> srcn & 1)):
                        e = ent(rname)
                        if e['slot'] is None:
                            e['slot'] = off // 4
                            e['notes'].append('home-store %s %d(sp) -> slot %d' % (d.mnem, d.mem[1], off // 4))
                            if d.mnem == 'swc1':
                                e['fl'] = True
                            if d.mnem == 'sdc1':
                                e['fl'] = True
                                e['dbl'] = True
                            sw = _store_width(d.mnem)
                            if sw:
                                e['store_narrow'] = sw
            elif d.kind == 'load' and off >= 16 and off < 4 * MAX_SLOT and d.mnem in (
                    'lw', 'lwc1', 'ldc1', 'lh', 'lhu', 'lb', 'lbu'):
                slot = off // 4
                key = 'stack%d' % slot
                e = ent(key)
                e['slot'], e['stack'] = slot, True
                e['notes'].append('load %s %d(sp) = stack argument slot %d' % (d.mnem, d.mem[1], slot))
                if d.mnem == 'lwc1':
                    e['kind'] = 'f32'
                elif d.mnem == 'ldc1':
                    e['kind'] = 'f64'
                elif d.mnem in ('lh',):
                    e['width'] = 's16'
                elif d.mnem == 'lhu':
                    e['width'] = 'u16'
                elif d.mnem == 'lb':
                    e['width'] = 's8'
                elif d.mnem == 'lbu':
                    e['width'] = 'u8'
            continue
        # uses of original values
        uses = d.uses
        for rn in range(64):
            if not (uses >> rn & 1):
                continue
            rname = REGNAMES[rn]
            if rname not in evid or evid[rname]['stack'] or not orig(rn):
                continue
            e = evid[rname]
            if rn < 32:
                nw = _narrow_at(ins, i, rn)
                if nw and e['narrow'] is None:
                    e['narrow'] = nw
                if d.mem is not None and d.mem[0] == rn and d.kind in ('load', 'store'):
                    e['ptr'] = True
                if d.mnem == 'mtc1' and d.rt == rn:
                    e['fl'] = True
                    e['notes'].append('mtc1 at 0x%08X: float passed in integer register' % d.pc)
            else:
                if d.mnem.endswith('.d') or d.mnem in ('sdc1',) or d.mnem.startswith('c.') and d.mnem.endswith('.d'):
                    e['dbl'] = True
                e['fl'] = True
    # a non-ABI register that is read at entry AND written later may be a path-correlated false positive of the
    # liveness (camera_reset/t2); keep it only with a home slot or when the body never writes it
    strong = reads & ~info.local_defs
    for r in list(evid):
        e = evid[r]
        if e['stack'] or r in A_REGS or r in ('f12', 'f13', 'f14', 'f15'):
            continue
        if e['slot'] is None and not (strong >> REGBIT[r] & 1):
            del evid[r]
    return evid, frame, ins


def _caller_windows(corpus, infos):
    """{callee: [ {reg: class-or-None, ...'stack': {slot: cls}} per site ]} from the code just before each jal."""
    sites = {}
    for cname, cf in corpus.funcs.items():
        info = infos.get(cname)
        if info is None or cf.tail_of:
            continue
        ins = mipsdec.decode_words(cf.words, cf.addr)
        pc2i = {d.pc: i for i, d in enumerate(ins)}
        targets = set()
        for d in ins:
            if d.target is not None and d.kind in ('branch', 'b'):
                t = pc2i.get(d.target)
                if t is not None:
                    targets.add(t)
        for s in info.sites:
            if s.callee is None or s.kind != 'jal' or s.res not in ('func', 'alt'):
                continue
            j = pc2i.get(s.pc)
            if j is None:
                continue
            w = {'regs': {}, 'stack': {}, 'caller': cname, 'pc': s.pc}
            # the delay slot runs before the callee, then walk back to the previous call / branch / label
            k = j + 1
            order = [k] if k < len(ins) else []
            k = j - 1
            steps = 0
            while k >= 0 and steps < 24:
                order.append(k)
                if ins[k].kind in ('jal', 'jalr', 'ret', 'jr', 'b', 'j', 'branch') and k != j:
                    order.pop()
                    break
                if k in targets:
                    break
                k -= 1
                steps += 1
            seen = set()
            for k in order:
                d = ins[k]
                if d.kind == 'store' and d.mem and d.mem[0] == 29 and 16 <= d.mem[1] < 16 + 4 * 8:
                    slot = d.mem[1] // 4
                    w['stack'].setdefault(slot, d.mnem)
                for rn in range(64):
                    if d.defs >> rn & 1 and rn not in seen:
                        rname = REGNAMES[rn]
                        if rname in A_REGS or rname in ('f12', 'f14') or rname in ('f13', 'f15'):
                            seen.add(rn)
                            w['regs'][rname] = _def_class(ins, k, rn)
                        elif rname not in ('at', 'v0', 'v1', 'sp', 'ra') and rn not in seen:
                            seen.add(rn)
                            w['regs'][rname] = None
            sites.setdefault(s.callee, []).append(w)
    return sites


def _def_class(ins, k, rn):
    """What the instruction that defines an argument register says about its type."""
    d = ins[k]
    if d.mnem == 'sra' and _sa(d) in (16, 24):
        return 's16' if _sa(d) == 16 else 's8'
    if d.mnem == 'srl' and _sa(d) in (16, 24):
        return 'u16' if _sa(d) == 16 else 'u8'
    if d.mnem == 'andi':
        m = d.word & 0xFFFF
        if m == 0xFF:
            return 'u8'
        if m == 0xFFFF:
            return 'u16'
    if d.mnem in ('lwc1', 'mov.s', 'add.s', 'sub.s', 'mul.s', 'div.s', 'neg.s', 'abs.s', 'cvt.s.w', 'cvt.s.d', 'mtc1'):
        return 'f32'
    if d.mnem in ('ldc1', 'mov.d', 'add.d', 'sub.d', 'mul.d', 'div.d', 'cvt.d.s', 'cvt.d.w'):
        return 'f64'
    if d.mnem == 'lh':
        return 's16'
    if d.mnem == 'lhu':
        return 'u16'
    if d.mnem == 'lb':
        return 's8'
    if d.mnem == 'lbu':
        return 'u8'
    return None


def _site_arity(w):
    """Argument count one call site sets up (None when it sets none)."""
    regs = w['regs']
    k = 0
    while k < 4 and A_REGS[k] in regs:
        k += 1
    if k:
        if k == 4:
            extra = [s for s in w['stack'] if s >= 4]
            if extra:
                k = max(extra) + 1
        return k
    if 'f12' in regs:
        return 2 if 'f14' in regs else 1
    return 0


# ---------------------------------------------------------------------------------------------------------
# ordering and assembly
# ---------------------------------------------------------------------------------------------------------

def _reg_rank(r):
    return REG_ORDER.index(r) if r in REG_ORDER else 99


def build_sig(name, evid, use_callers=True, site_windows=None, ret_info=None, arity_hint=None):
    sig = Sig(name)
    params = {}
    for r, e in evid.items():
        p = Param(reg=None if e['stack'] else r, kind='int', width='w')
        p.slot = e['slot']
        if e['stack']:
            p.kind, p.width = e['kind'], e['width']
            p.source = 'stack-load'
            p.evidence = list(e['notes'])
            params[r] = p
            continue
        if r.startswith('f'):
            p.kind = 'f64' if e['dbl'] else 'f32'
            p.source = 'fp-reg'
        else:
            if e['fl']:
                p.kind = 'f32'
            elif e['narrow']:
                p.width = e['narrow']
            elif e['store_narrow'] and e['ptr'] is False and False:
                p.width = e['store_narrow']
            elif e['ptr']:
                p.kind = 'ptr'
            p.source = 'abi-reg' if r in A_REGS else 'ipa-reg'
        p.evidence = list(e['notes'])
        if e['narrow']:
            p.evidence.append('narrowing %s at entry' % e['narrow'])
        if e['ptr'] and p.kind == 'ptr':
            p.evidence.append('used as load/store base')
        params[r] = p
    # --- site-derived hints (caller setup) ---------------------------------------------------------------
    sites = site_windows or []
    if sites:
        counts = [_site_arity(w) for w in sites]
        sig.arity_callers = (min(counts), max(counts))
        for w in sites:
            for r, c in w['regs'].items():
                if r in params and c and c[0] in 'su' and c[1:] in ('8', '16') and params[r].width == c:
                    params[r].evidence.append('caller %08X narrows %s to %s too' % (w['pc'], r, c))
    # --- slot assignment ---------------------------------------------------------------------------------
    taken = {}

    def occupy(p, s):
        words = 2 if p.kind == 'f64' else 1
        for w in range(s, s + words):
            taken[w] = p

    def is_free(s, words=1):
        return all(w not in taken and w < MAX_SLOT for w in range(s, s + words))

    slotted = [(r, p) for r, p in params.items() if p.slot is not None]
    for r, p in sorted(slotted, key=lambda rp: (rp[1].slot, _reg_rank(rp[0]))):
        if p.slot in taken:
            # two registers home-stored to one slot: keep the first, treat the other as a plain register
            p.slot = None
            continue
        occupy(p, p.slot)
        p.source += '+slot'
    nonslot = {r: p for r, p in params.items() if p.slot is None}
    # natural slots of the ABI registers
    f12 = nonslot.get('f12')
    f14 = nonslot.get('f14')
    natural = {}
    # An IPA function's a-registers are not bound to their natural slots (IDO picks the first register above the
    # outgoing argument registers): natural only while it stays inside the parameter count.
    is_ipa = any(p.reg and p.reg not in A_REGS and p.reg not in ('f12', 'f14') for p in params.values())
    arity_est = max([len(params)] + [p.slot + 1 for p in params.values() if p.slot is not None])
    for r in A_REGS:
        if r in nonslot and (not is_ipa or ABI_INT_SLOT[r] < arity_est):
            natural[r] = ABI_INT_SLOT[r]
    if f12 is not None:
        natural['f12'] = 0
    if f14 is not None:
        natural['f14'] = 2 if (f12 is not None and f12.kind == 'f64') else 1
    rest = []
    for r, p in sorted(nonslot.items(), key=lambda rp: _reg_rank(rp[0]) if not (rp[0] in natural) else -1):
        s = natural.get(r)
        words = 2 if p.kind == 'f64' else 1
        if s is not None and is_free(s, words):
            p.slot = s
            occupy(p, s)
            p.source += '+natural'
        else:
            rest.append((r, p))
    # the others fill the lowest free slots in register order (ints first, then floats)
    rest.sort(key=lambda rp: _reg_rank(rp[0]))
    for r, p in rest:
        words = 2 if p.kind == 'f64' else 1
        s = 0
        while not is_free(s, words):
            s += 1
        p.slot = s
        p.guessed = True
        p.source += '+filled'
        occupy(p, s)
    ordered = sorted(params.values(), key=lambda p: p.slot)
    # arity: gaps below the highest slot are unused parameters
    out = []
    pos = 0
    for p in ordered:
        while pos < p.slot:
            g = Param(reg=None, slot=pos, source='gap')
            g.evidence.append('unused parameter (slot %d has no register evidence)' % pos)
            g.guessed = True
            out.append(g)
            pos += 1
        out.append(p)
        pos = p.slot + (2 if p.kind == 'f64' else 1)
    # call sites that set up more registers than the callee reads: unused trailing parameters.  Only when EVERY
    # site agrees (minimum over sites) and the callee does not use them.
    if arity_hint:
        have = pos
        if arity_hint > have and arity_hint <= 4:
            for s in range(have, arity_hint):
                g = Param(reg=None, slot=s, source='caller-setup')
                g.evidence.append('every call site sets up %d arguments' % arity_hint)
                g.guessed = True
                out.append(g)
    sig.params = out
    sig.ipa_regs = sorted((p.reg for p in out if p.reg and p.reg not in A_REGS
                           and p.reg not in ('f12', 'f14')), key=_reg_rank)
    return sig


def _ret_type(name, info, sites_of_callers, cf_ins):
    """(ret, evidence, strength) from callers first, then the function's own epilogue."""
    ev = []
    used_v = used_f = 0
    ncall = 0
    for caller, s in sites_of_callers:
        ncall += 1
        if 'v0' in s.ret_use or 'v1' in s.ret_use and False:
            used_v += 1
        if 'f0' in s.ret_use:
            used_f += 1
    if ncall:
        if used_f and used_f >= used_v:
            return 'f32', ['%d/%d call sites read f0' % (used_f, ncall)], 'callers'
        if used_v:
            return 's32', ['%d/%d call sites read v0' % (used_v, ncall)], 'callers'
    # own epilogue: what do the last instructions before jr ra define (v0 / f0)?
    last = None
    for i, d in enumerate(cf_ins):
        if d.kind == 'ret':
            last = i
    own = None
    if last is not None:
        window = cf_ins[max(0, last - 7):last + 2]
        for d in reversed(window):
            if d.kind in ('jal', 'jalr'):
                break
            if d.defs >> REGBIT['f0'] & 1:
                own = 'f32'
                break
            if d.defs >> REGBIT['v0'] & 1 and d.mnem != 'lw' or (d.defs >> REGBIT['v0'] & 1 and d.mem and d.mem[0] != 29):
                own = 's32'
                break
    if ncall:
        if own:
            return 'void', ['%d call sites ignore the return value; epilogue writes %s' % (ncall, own)], 'callers-none'
        return 'void', ['%d call sites ignore the return value' % ncall], 'callers-none'
    if own:
        return own, ['no callers; epilogue writes %s' % ('f0' if own == 'f32' else 'v0')], 'own'
    return 'void', ['no callers; epilogue writes no return register'], 'own'


class SigModel:
    def __init__(self, corpus=None, infos=None):
        self.corpus = corpus or load_corpus(heads=True)
        self.infos = infos or analyze.analyze_all(self.corpus)
        self.windows = _caller_windows(self.corpus, self.infos)
        self.callers = {}
        for n, i in self.infos.items():
            for s in i.sites:
                if s.callee is not None and s.res in ('func', 'alt'):
                    self.callers.setdefault(s.callee, []).append((n, s))
        self._sigs = {}
        self._raw = {}

    def raw(self, name):
        if name not in self._raw:
            f = self.corpus.funcs[name]
            self._raw[name] = scan_function(self.corpus, f, self.infos[name])
        return self._raw[name]

    def sig(self, name):
        if name in self._sigs:
            return self._sigs[name]
        f = self.corpus.funcs[name]
        info = self.infos[name]
        evid, frame, ins = self.raw(name)
        wins = [w for w in self.windows.get(name, []) if w['caller'] != name]
        hint = None
        if wins:
            counts = [_site_arity(w) for w in wins]
            hint = min(counts)
            if len(wins) < HINT_MIN_SITES or (HINT_AGREE and min(counts) != max(counts)):
                hint = None
        self._busy = getattr(self, '_busy', set())
        self._busy.add(name)
        for r, e in evid.items():
            for callee, slot in e.get('pass', ()):
                if callee in self._busy or callee not in self.infos or callee == name:
                    continue
                cs = self.sig(callee)
                for cp in cs.params:
                    if cp.slot == slot and cp.kind == 'ptr' and not e['fl'] and not e['narrow']:
                        e['ptr'] = True
                        e['notes'].append('passed through to %s arg %d (pointer)' % (callee, slot))
        self._busy.discard(name)
        sig = build_sig(name, evid, site_windows=wins, arity_hint=hint if USE_HINT else None)
        sig.ret, sig.ret_evidence, strength = _ret_type(name, info, [(c, s) for c, s in self.callers.get(name, [])
                                                                     if c != name], ins)
        # confidence
        guessed = sum(1 for p in sig.params if p.guessed and p.source.startswith(('ipa', 'gap')))
        if any(p.guessed and p.source.endswith('filled') for p in sig.params) and sig.ipa_regs and guessed > 1:
            sig.confidence = 'low'
            sig.notes.append('IPA register order guessed from register order (no home slot for %d)' % guessed)
        elif sig.ipa_regs and any(p.guessed for p in sig.params):
            sig.confidence = 'medium'
        elif not sig.params and strength in ('own',):
            sig.confidence = 'medium'
        else:
            sig.confidence = 'high' if strength != 'own' or sig.params else 'medium'
        if sig.params and strength == 'own':
            sig.confidence = 'medium' if sig.confidence == 'high' else sig.confidence
        if info.unresolved:
            sig.notes.append('unresolved call targets: arity may be understated')
        self._sigs[name] = sig
        return sig

    def all(self):
        return {n: self.sig(n) for n in sorted(self.infos) if not self.corpus.funcs[n].tail_of}


def model_cache(_c={}):
    if 'm' not in _c:
        _c['m'] = SigModel()
    return _c['m']


# ---------------------------------------------------------------------------------------------------------
# ground truth (matching sources) and validation
# ---------------------------------------------------------------------------------------------------------

_TYPE_CLASS = [
    (re.compile(r'\b(f32|float|F32)\b'), 'f32'),
    (re.compile(r'\b(f64|double)\b'), 'f64'),
]
_NARROW = {'s8': 's8', 'char': 's8', 'S08': 's8', 'vs8': 's8', 'u8': 'u8', 'U08': 'u8', 'vu8': 'u8',
           's16': 's16', 'short': 's16', 'S16': 's16', 'vs16': 's16', 'u16': 'u16', 'U16': 'u16', 'vu16': 'u16'}


def type_class(t):
    t = t.strip()
    if '*' in t or '[' in t:
        return 'ptr'
    for rx, c in _TYPE_CLASS:
        if rx.search(t):
            return c
    toks = re.findall(r'[A-Za-z_]\w*', re.sub(r'\b(const|volatile|register|static|struct|union|enum|signed|extern)\b', '', t)) \
        or []
    if 'unsigned' in t and ('char' in t):
        return 'u8'
    if 'unsigned' in t and 'short' in t:
        return 'u16'
    if 'signed' in t and 'char' in t:
        return 's8'
    for tk in toks:
        if tk in _NARROW:
            return _NARROW[tk]
    return 'w'


def _split_params(text):
    out, depth, cur = [], 0, ''
    for ch in text:
        if ch == '(':
            depth += 1
        elif ch == ')':
            depth -= 1
        if ch == ',' and depth == 0:
            out.append(cur)
            cur = ''
        else:
            cur += ch
    if cur.strip():
        out.append(cur)
    return [p.strip() for p in out]


def parse_definition(src, name):
    """(ret_type, [(type_text, pname)], kr) of the first DEFINITION of `name` in C text, or None.
    kr is True for `f()` / K&R style (arity unknown)."""
    text = re.sub(r'/\*.*?\*/', ' ', src, flags=re.S)
    text = re.sub(r'//[^\n]*', ' ', text)
    rx = re.compile(r'(?:^|\n)([ \t]*[A-Za-z_][\w \t\*]*?)\b%s[ \t]*\(([^;{}]*?)\)\s*\{' % re.escape(name))
    m = rx.search(text)
    if not m:
        return None
    ret = m.group(1).strip()
    if re.match(r'^(return|else|if|while|for|switch|sizeof)\b', ret) or ret.endswith(('=', '.', '>')):
        return None
    ret = re.sub(r'\b(static|extern|inline|__inline|local|INLINE)\b', '', ret).strip()
    raw = m.group(2).strip()
    if raw == '':
        return ret, [], True
    if raw == 'void':
        return ret, [], False
    plist = _split_params(raw)
    if plist and not re.search(r'[A-Za-z_]\w*\s+\**\s*[A-Za-z_]\w*|\*', plist[0]) and ' ' not in plist[0].strip():
        return ret, [], True
    ps = []
    for p in plist:
        if p == '...':
            continue
        mm = re.match(r'^(.*?)([A-Za-z_]\w*)\s*(\[[^\]]*\])?$', p)
        if mm and mm.group(1).strip():
            t = mm.group(1) + (mm.group(3) or '')
            ps.append((t, mm.group(2)))
        else:
            ps.append((p, ''))
    return ret, ps, False


def reg_of_name(pname, idx):
    """Register an m2c-style parameter name pins: arg0..arg3 -> a0..a3, ipa_<reg> -> <reg>."""
    m = re.fullmatch(r'arg([0-3])', pname)
    if m:
        return 'a' + m.group(1)
    m = re.fullmatch(r'ipa_(\w+)', pname)
    if m:
        return m.group(1)
    return None


def ground_truth(only=None):
    """{name: dict(ret, params[(type,name)], src)} from matched sources."""
    gt = {}
    root = ROOT
    srcs = []
    if only in (None, 'matches'):
        for p in sorted((root / 'cloud' / 'matches').glob('*.c')):
            srcs.append(('matches', p, None))
    if only in (None, 'groups'):
        for d in sorted((root / 'cloud' / 'work' / 'ipa-groups').iterdir()):
            jp = d / 'group.json'
            if not jp.exists():
                continue
            j = json.loads(jp.read_text())
            claims = j.get('claims') or []
            for fn in j.get('files', []):
                if (d / fn).exists():
                    srcs.append(('groups', d / fn, set(claims)))
    if only in (None, 'blob'):
        for p in sorted((root / 'src' / 'blob').glob('*.c')):
            srcs.append(('blob', p, None))
    for kind, path, claims in srcs:
        try:
            text = path.read_text(errors='replace')
        except OSError:
            continue
        if kind in ('matches', 'blob'):
            names = [path.stem]
            # every other definition in the file whose name is a game function is ground truth as well
            names += re.findall(r'(?:^|\n)[A-Za-z_][\w \t\*]*?\b((?:func|MP|camera|car|audio|physics)\w*)\s*\([^;{}]*\)\s*\{', text)
        else:
            names = sorted(claims)
        for nm in dict.fromkeys(names):
            if nm in gt:
                continue
            pd = parse_definition(text, nm)
            if pd is None:
                continue
            ret, ps, kr = pd
            gt[nm] = {'ret': ret, 'params': ps, 'kr': kr, 'src': '%s:%s' % (kind, path.name), 'kind': kind}
    return gt


def _relax(c):
    return 'w' if c == 'ptr' else c


def _family(c):
    return {'f32': 'f', 'f64': 'd'}.get(c, 'i')


def _ret_class(t):
    t = t.strip()
    if t == 'void':
        return 'void'
    c = type_class(t)
    return {'f32': 'f32', 'f64': 'f64'}.get(c, 'int')


def validate(model=None, only=None):
    model = model or model_cache()
    gt = ground_truth(only)
    rows = []
    for nm, g in sorted(gt.items()):
        if nm not in model.corpus.funcs or model.corpus.funcs[nm].tail_of:
            continue
        sig = model.sig(nm)
        tc = [type_class(t) for t, _ in g['params']]
        pc = [p.cls for p in sig.params]
        row = {'name': nm, 'src': g['src'], 'kind': g['kind'], 'truth': '%s %s(%s)' % (g['ret'], nm, ', '.join(t for t, _ in g['params'])),
               'pred': sig.prototype(), 'conf': sig.confidence, 'ipa': bool(sig.ipa_regs), 'kr': g['kr']}
        row['arity_ok'] = None if g['kr'] else len(tc) == len(pc)
        row['arity_under'] = None if g['kr'] else len(pc) < len(tc)
        row['arity_over'] = None if g['kr'] else len(pc) > len(tc)
        if not g['kr'] and len(tc) == len(pc):
            row['fam_order_ok'] = [_family(c) for c in tc] == [_family(c) for c in pc]
            row['cls_ok'] = [_relax(c) for c in tc] == [_relax(c) for c in pc]
            row['cls_strict_ok'] = tc == pc
            row['per_param'] = len(tc)
            row['per_param_ok'] = sum(1 for a, b in zip(tc, pc) if a == b)
            row['per_param_relaxed_ok'] = sum(1 for a, b in zip(tc, pc) if _relax(a) == _relax(b))
        rt, pr = _ret_class(g['ret']), _ret_class(sig.ret_ctype())
        row['ret_ok'] = rt == pr
        row['ret_truth'], row['ret_pred'] = rt, pr
        # register order check against m2c-style names
        pinned = [(i, reg_of_name(pn, i)) for i, (_, pn) in enumerate(g['params']) if reg_of_name(pn, i)]
        ipa_pinned = [(i, r) for i, r in pinned if r not in A_REGS]
        if ipa_pinned:
            pred_idx = {p.reg: i for i, p in enumerate(sig.params) if p.reg}
            row['reg_pinned'] = len(pinned)
            row['reg_pinned_ok'] = sum(1 for i, r in pinned if pred_idx.get(r) == i)
            row['reg_order_exact'] = row['reg_pinned_ok'] == len(pinned)
        rows.append(row)
    return rows


def summarize(rows):
    def frac(sel, key):
        xs = [r for r in sel if r.get(key) is not None]
        return (sum(1 for r in xs if r[key]), len(xs))

    out = {}
    groups = {'all': rows, 'matches': [r for r in rows if r['kind'] == 'matches'], 'groups': [r for r in rows if r['kind'] == 'groups'],
              'blob': [r for r in rows if r['kind'] == 'blob'], 'ipa_only': [r for r in rows if r['ipa']],
              'abi_only': [r for r in rows if not r['ipa']]}
    for gname, sel in groups.items():
        if not sel:
            continue
        pp = sum(r.get('per_param', 0) for r in sel)
        out[gname] = {
            'functions': len(sel),
            'arity': frac(sel, 'arity_ok'), 'arity_under': frac(sel, 'arity_under'), 'arity_over': frac(sel, 'arity_over'),
            'int_float_order (arity ok)': frac(sel, 'fam_order_ok'),
            'class relaxed (ptr~int)': frac(sel, 'cls_ok'), 'class strict': frac(sel, 'cls_strict_ok'),
            'params strict': (sum(r.get('per_param_ok', 0) for r in sel), pp),
            'params relaxed': (sum(r.get('per_param_relaxed_ok', 0) for r in sel), pp),
            'return class': frac(sel, 'ret_ok'),
            'ipa register order exact': frac(sel, 'reg_order_exact'),
        }
    by = {}
    for r in rows:
        if r.get('arity_ok') is None:
            continue
        b = by.setdefault(r['conf'], [0, 0])
        b[1] += 1
        if r['arity_ok'] and r.get('cls_ok'):
            b[0] += 1
    out['by_confidence (arity and relaxed class exact)'] = {c: tuple(v) for c, v in sorted(by.items())}
    return out


def _fmt_frac(v):
    a, b = v
    return '%d/%d (%s)' % (a, b, ('%.1f%%' % (100.0 * a / b)) if b else 'n/a')


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('names', nargs='*')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('--json', action='store_true')
    ap.add_argument('--evidence', action='store_true')
    ap.add_argument('--validate', action='store_true', help='compare with the matched sources')
    ap.add_argument('--only', choices=['matches', 'groups', 'blob'])
    ap.add_argument('--misses', action='store_true', help='with --validate: list the wrong ones')
    a = ap.parse_args(argv)
    model = model_cache()
    if a.validate:
        rows = validate(model, a.only)
        summ = summarize(rows)
        if a.json:
            print(json.dumps({'summary': summ, 'rows': rows}, indent=1))
            return 0
        for g, s in summ.items():
            if g.startswith('by_conf'):
                print('== %s' % g)
                for c, v in s.items():
                    print('   %-10s %s' % (c, _fmt_frac(v)))
                continue
            print('== %s (%d functions)' % (g, s['functions']))
            for k, v in s.items():
                if k != 'functions':
                    print('   %-28s %s' % (k, _fmt_frac(v)))
        if a.misses:
            for r in rows:
                if r['arity_ok'] is False or r.get('cls_ok') is False or r['ret_ok'] is False:
                    print('\n%s [%s conf=%s]\n   truth: %s\n   pred : %s' % (r['name'], r['src'], r['conf'], r['truth'], r['pred']))
        return 0
    names = sorted(model.all()) if a.all else a.names
    out = []
    for n in names:
        if n not in model.infos:
            print('%s: unknown function' % n, file=sys.stderr)
            continue
        s = model.sig(n)
        if a.json:
            out.append(s.to_json())
            continue
        print('%s   /* %s */' % (s.prototype(), s.confidence))
        if a.evidence:
            for p in s.params:
                print('    slot %-2s %-8s %-7s %s' % (p.slot, p.reg or '-', p.ctype(), '; '.join(p.evidence) or p.source))
            print('    ret %s: %s' % (s.ret, '; '.join(s.ret_evidence)))
            for nt in s.notes:
                print('    note: %s' % nt)
    if a.json:
        print(json.dumps(out, indent=1))
    return 0


if __name__ == '__main__':
    sys.exit(main())
