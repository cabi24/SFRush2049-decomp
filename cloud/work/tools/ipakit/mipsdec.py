#!/usr/bin/env python3
"""mipsdec.py: minimal MIPS-III (VR4300) decoder for static IPA analysis.

decode(word, pc) -> Insn with mnemonic, GPR/FPR def and use sets (as bitmasks),
branch / jump target, delay-slot and branch-likely flags, and a `kind`:

    alu load store branch (conditional) b (unconditional relative) j (absolute)
    jal jalr jr ret trap other

Register bitmask layout: bit 0..31 GPR, bit 32..63 FPR f0..f31, bit 64 hi,
bit 65 lo, bit 66 fcc.  Double-precision FP operands touch the even/odd pair.
Stdlib only; nothing here reads the ROM.  CLI:

    python3 cloud/work/tools/ipakit/mipsdec.py 0x27bdffe8 [0x...]  [--json]
"""
import json
import sys
from collections import namedtuple

GPR = ['zero', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3', 't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
       's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7', 't8', 't9', 'k0', 'k1', 'gp', 'sp', 's8', 'ra']
REGNAMES = GPR + ['f%d' % i for i in range(32)] + ['hi', 'lo', 'fcc']
REGBIT = {n: i for i, n in enumerate(REGNAMES)}
HI, LO, FCC = 1 << 64, 1 << 65, 1 << 66


def m(*names):
    v = 0
    for n in names:
        v |= 1 << REGBIT[n]
    return v


def mask_names(mask):
    return [REGNAMES[i] for i in range(len(REGNAMES)) if mask >> i & 1]


def names_mask(names):
    v = 0
    for n in names:
        v |= 1 << REGBIT[n]
    return v


def g(n):
    return 0 if n == 0 else 1 << n          # $zero is never a real def/use


def f(n, pair=False):
    v = 1 << (32 + n)
    if pair:
        v |= 1 << (32 + (n ^ 1))            # the register pair of a double
    return v


Insn = namedtuple('Insn', 'pc word mnem uses defs kind target delay likely mem imm rs rt rd')
# mem: (base_reg_number, offset) for loads/stores, else None.

_SPECIAL = {0: 'sll', 2: 'srl', 3: 'sra', 4: 'sllv', 6: 'srlv', 7: 'srav', 8: 'jr', 9: 'jalr', 12: 'syscall', 13: 'break',
            15: 'sync', 16: 'mfhi', 17: 'mthi', 18: 'mflo', 19: 'mtlo', 20: 'dsllv', 22: 'dsrlv', 23: 'dsrav',
            24: 'mult', 25: 'multu', 26: 'div', 27: 'divu', 28: 'dmult', 29: 'dmultu', 30: 'ddiv', 31: 'ddivu',
            32: 'add', 33: 'addu', 34: 'sub', 35: 'subu', 36: 'and', 37: 'or', 38: 'xor', 39: 'nor', 42: 'slt',
            43: 'sltu', 44: 'dadd', 45: 'daddu', 46: 'dsub', 47: 'dsubu', 48: 'tge', 49: 'tgeu', 50: 'tlt',
            51: 'tltu', 52: 'teq', 54: 'tne', 56: 'dsll', 58: 'dsrl', 59: 'dsra', 60: 'dsll32', 62: 'dsrl32',
            63: 'dsra32'}
_SHIFT_IMM = {0, 2, 3, 56, 58, 59, 60, 62, 63}
_SHIFT_VAR = {4, 6, 7, 20, 22, 23}
_MULDIV = {24, 25, 26, 27, 28, 29, 30, 31}
_TRAPS = {48, 49, 50, 51, 52, 54}
_REGIMM = {0: 'bltz', 1: 'bgez', 2: 'bltzl', 3: 'bgezl', 8: 'tgei', 9: 'tgeiu', 10: 'tlti', 11: 'tltiu', 12: 'teqi',
           14: 'tnei', 16: 'bltzal', 17: 'bgezal', 18: 'bltzall', 19: 'bgezall'}
_ITYPE = {8: 'addi', 9: 'addiu', 10: 'slti', 11: 'sltiu', 12: 'andi', 13: 'ori', 14: 'xori', 24: 'daddi', 25: 'daddiu'}
_BR2 = {4: 'beq', 5: 'bne', 20: 'beql', 21: 'bnel'}
_BR1 = {6: 'blez', 7: 'bgtz', 22: 'blezl', 23: 'bgtzl'}
_LOADS = {32: 'lb', 33: 'lh', 34: 'lwl', 35: 'lw', 36: 'lbu', 37: 'lhu', 38: 'lwr', 39: 'lwu', 26: 'ldl', 27: 'ldr',
          48: 'll', 52: 'lld', 55: 'ld'}
_STORES = {40: 'sb', 41: 'sh', 42: 'swl', 43: 'sw', 44: 'sdl', 45: 'sdr', 46: 'swr', 63: 'sd'}
_FMT = {16: 's', 17: 'd', 20: 'w', 21: 'l'}
_FARITH = {0: 'add', 1: 'sub', 2: 'mul', 3: 'div'}
_FUN1 = {4: 'sqrt', 5: 'abs', 6: 'mov', 7: 'neg'}
_FCONV_W = {12: 'round.w', 14: 'ceil.w', 13: 'trunc.w', 15: 'floor.w'}
_FCONV_L = {8: 'round.l', 9: 'trunc.l', 10: 'ceil.l', 11: 'floor.l'}
_FCMP = ['f', 'un', 'eq', 'ueq', 'olt', 'ult', 'ole', 'ule', 'sf', 'ngle', 'seq', 'ngl', 'lt', 'nge', 'le', 'ngt']


def _sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def decode(word, pc=0):
    op = word >> 26
    rs, rt, rd = word >> 21 & 31, word >> 16 & 31, word >> 11 & 31
    sa, fn = word >> 6 & 31, word & 63
    imm = _sext16(word & 0xFFFF)
    uses = defs = 0
    kind, target, delay, likely, mem, mn = 'alu', None, False, False, None, '?'

    def I(mn, uses=0, defs=0, kind='alu', target=None, delay=False, likely=False, mem=None):
        return Insn(pc, word, mn, uses, defs, kind, target, delay, likely, mem, imm, rs, rt, rd)

    btarget = (pc + 4 + (imm << 2)) & 0xFFFFFFFF
    if word == 0:
        return I('nop')
    if op == 0:
        mn = _SPECIAL.get(fn)
        if mn is None:
            return I('?special', kind='other')
        if fn in _SHIFT_IMM:
            return I(mn, g(rt), g(rd))
        if fn in _SHIFT_VAR:
            return I(mn, g(rt) | g(rs), g(rd))
        if fn == 8:
            if rs == 31:
                return I('jr', g(rs), 0, 'ret', None, True)
            return I('jr', g(rs), 0, 'jr', None, True)
        if fn == 9:
            return I('jalr', g(rs), g(rd), 'jalr', None, True)
        if fn in (12, 13, 15):
            return I(mn, 0, 0, 'other')
        if fn == 16:
            return I(mn, HI, g(rd))
        if fn == 18:
            return I(mn, LO, g(rd))
        if fn == 17:
            return I(mn, g(rs), HI)
        if fn == 19:
            return I(mn, g(rs), LO)
        if fn in _MULDIV:
            return I(mn, g(rs) | g(rt), HI | LO)
        if fn in _TRAPS:
            return I(mn, g(rs) | g(rt), 0, 'trap')
        return I(mn, g(rs) | g(rt), g(rd))
    if op == 1:
        mn = _REGIMM.get(rt)
        if mn is None:
            return I('?regimm', kind='other')
        if rt >= 8 and rt < 16:
            return I(mn, g(rs), 0, 'trap')
        link = rt >= 16
        lk = rt in (2, 3, 18, 19)
        defs = g(31) if link else 0
        uncond = rt in (1, 17) and rs == 0      # bgez $zero / bal
        return I(mn, g(rs), defs, 'b' if uncond else 'branch', btarget, True, lk)
    if op == 2 or op == 3:
        tgt = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
        if op == 3:
            return I('jal', 0, g(31), 'jal', tgt, True)
        return I('j', 0, 0, 'j', tgt, True)
    if op in _BR2:
        mn = _BR2[op]
        uncond = op == 4 and rs == rt
        return I('b' if uncond and rs == 0 else mn, g(rs) | g(rt), 0, 'b' if uncond else 'branch', btarget, True, op >= 20)
    if op in _BR1:
        return I(_BR1[op], g(rs), 0, 'branch', btarget, True, op >= 20)
    if op in _ITYPE:
        return I(_ITYPE[op], g(rs), g(rt))
    if op == 15:
        return I('lui', 0, g(rt))
    if op in _LOADS:
        u = g(rs) | (g(rt) if op in (34, 38, 26, 27) else 0)
        return I(_LOADS[op], u, g(rt), 'load', mem=(rs, imm))
    if op in _STORES:
        return I(_STORES[op], g(rs) | g(rt), 0, 'store', mem=(rs, imm))
    if op == 56 or op == 60:                   # sc / scd
        return I('sc' if op == 56 else 'scd', g(rs) | g(rt), g(rt), 'store', mem=(rs, imm))
    if op == 47:
        return I('cache', g(rs), 0, 'other')
    if op == 49:
        return I('lwc1', g(rs), f(rt), 'load', mem=(rs, imm))
    if op == 53:
        return I('ldc1', g(rs), f(rt, True), 'load', mem=(rs, imm))
    if op == 57:
        return I('swc1', g(rs) | f(rt), 0, 'store', mem=(rs, imm))
    if op == 61:
        return I('sdc1', g(rs) | f(rt, True), 0, 'store', mem=(rs, imm))
    if op == 16:                               # COP0
        if rs == 0:
            return I('mfc0', 0, g(rt), 'other')
        if rs == 4:
            return I('mtc0', g(rt), 0, 'other')
        return I('cop0', 0, 0, 'other')
    if op == 17:
        return _cop1(I, word, rs, rt, rd, sa, fn, btarget)
    return I('?op%d' % op, 0, 0, 'other')


def _cop1(I, word, rs, rt, rd, sa, fn, btarget):
    if rs == 0:
        return I('mfc1', f(rd), g(rt))
    if rs == 1:
        return I('dmfc1', f(rd, True), g(rt))
    if rs == 2:
        return I('cfc1', 0, g(rt))
    if rs == 4:
        return I('mtc1', g(rt), f(rd))
    if rs == 5:
        return I('dmtc1', g(rt), f(rd, True))
    if rs == 6:
        return I('ctc1', g(rt), 0)
    if rs == 8:
        nm = ['bc1f', 'bc1t', 'bc1fl', 'bc1tl'][rt & 3]
        return I(nm, FCC, 0, 'branch', btarget, True, bool(rt & 2))
    fmt = _FMT.get(rs)
    if fmt is None:
        return I('?cop1', kind='other')
    ft, fs, fd = rt, rd, sa
    dbl = fmt in ('d', 'l')
    if fn in _FARITH:
        return I('%s.%s' % (_FARITH[fn], fmt), f(fs, dbl) | f(ft, dbl), f(fd, dbl))
    if fn in _FUN1:
        return I('%s.%s' % (_FUN1[fn], fmt), f(fs, dbl), f(fd, dbl))
    if fn in _FCONV_W:
        return I('%s.%s' % (_FCONV_W[fn], fmt), f(fs, dbl), f(fd))
    if fn in _FCONV_L:
        return I('%s.%s' % (_FCONV_L[fn], fmt), f(fs, dbl), f(fd, True))
    if fn == 32:
        return I('cvt.s.%s' % fmt, f(fs, dbl), f(fd))
    if fn == 33:
        return I('cvt.d.%s' % fmt, f(fs, dbl), f(fd, True))
    if fn == 36:
        return I('cvt.w.%s' % fmt, f(fs, dbl), f(fd))
    if fn == 37:
        return I('cvt.l.%s' % fmt, f(fs, dbl), f(fd, True))
    if fn >= 48:
        return I('c.%s.%s' % (_FCMP[fn & 15], fmt), f(fs, dbl) | f(ft, dbl), FCC)
    return I('?cop1fn', kind='other')


def decode_words(words, base=0):
    return [decode(w, base + 4 * i) for i, w in enumerate(words)]


def fmt_insn(i):
    return '%08x: %08x %-8s uses=%s defs=%s%s' % (
        i.pc, i.word, i.mnem, ','.join(mask_names(i.uses)), ','.join(mask_names(i.defs)),
        (' -> %08x' % i.target) if i.target is not None else '')


def main(argv):
    as_json = '--json' in argv
    words = [int(a, 0) for a in argv if not a.startswith('--')]
    out = []
    for i, w in enumerate(words):
        d = decode(w, 0x80000000 + 4 * i)
        out.append({'pc': d.pc, 'word': w, 'mnem': d.mnem, 'kind': d.kind, 'uses': mask_names(d.uses),
                    'defs': mask_names(d.defs), 'target': d.target, 'delay': d.delay, 'likely': d.likely})
        if not as_json:
            print(fmt_insn(d))
    if as_json:
        print(json.dumps(out, indent=1))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))


def jump_table(insns, idx, word_at, lo, hi, window=24, maxn=1024):
    """Targets of the switch `jr rX` at insns[idx] (rX != ra), or None when no `lui`-based
    table is found.  Pattern: [sltiu t,idx,N;] lui b,hi; addu b,b,idx; lw x,off(b); jr x.
    Entries are read from the image through word_at(addr) while they fall in [lo, hi)."""
    j = insns[idx]
    x = j.rs
    k = idx - 1
    lw = None
    while k >= 0 and idx - k <= window:
        c = insns[k]
        if c.defs & g(x):
            if c.mnem == 'lw':
                lw = c
            break
        k -= 1
    if lw is None:
        return None
    want = {lw.rs}
    luival = None
    k -= 1
    while k >= 0 and idx - k <= window and luival is None:
        c = insns[k]
        if c.defs & sum(g(r) for r in want):
            if c.mnem == 'lui':
                luival = (c.word & 0xFFFF) << 16
            elif c.mnem in ('addu', 'add', 'or'):
                want |= {c.rs, c.rt}
            elif c.mnem in ('addiu', 'ori'):
                want.add(c.rs)
        k -= 1
    if luival is None:
        return None
    base = (luival + lw.mem[1]) & 0xFFFFFFFF
    n = maxn
    for c in insns[max(0, idx - window):idx]:
        if c.mnem == 'sltiu' and c.imm > 0:
            n = min(n, c.imm)
    out = []
    for e in range(n):
        v = word_at(base + 4 * e)
        if v is None or v & 3 or not (lo <= v < hi):
            break
        out.append(v)
    return out or None
