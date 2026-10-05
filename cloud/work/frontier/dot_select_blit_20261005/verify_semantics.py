#!/usr/bin/env python3
"""Compare protected native MIPS, an independent arithmetic oracle, and host C.

No retail instructions are written to the receipt. Callee effects are synthetic:
RenameBlit replaces Info and destroys caller-save GPRs; UpdateBlit records a call.
This proves the caller under those contracts, not either callee implementation.
"""
import ctypes
import hashlib
import itertools
from pathlib import Path
import random
import struct
import subprocess
import sys

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

FN = 'stat_race_update'
MASK = 0xffffffff
OWNER, FIRST, SECOND, NAME, STACK, STOP = 0x1000, 0x2000, 0x3000, 0x4000, 0x6000, 0xfffffff0


def signed(v, bits=32):
    v &= (1 << bits) - 1
    return v - (1 << bits) if v & (1 << (bits - 1)) else v


def quotient(a, b):
    assert b
    return (-1 if (a < 0) != (b < 0) else 1) * (abs(a) // abs(b))


def oracle(case):
    present, info, width, index, h, v, flip, replacement, new_width = case
    result = [-30000, 1234, -5678, 30000, 111, 222, 17, flip, -7, 19, 0, 0, int(bool(info))]
    if not present:
        return result
    if not info or not width:
        result[10] = 1
        result[12] = 2 if replacement else 0
        if not replacement:
            return result
        width = new_width
    cells = quotient(width, h) or 1
    row = quotient(index, cells)
    column = index - row * cells
    top = signed(row * v, 16)
    if flip:
        left = (width - quotient(width, h) * h) + (cells - column - 1) * h
    else:
        left = column * h
    left = signed(left, 16)
    result[:4] = [top, signed(top + v - 1, 16), left, signed(left + h - 1, 16)]
    result[11] = 1
    return result


class Native:
    def __init__(self, words=None):
        self.addresses = score.image_symbols()
        self.start = self.addresses[FN]
        words = score.targets()[FN] if words is None else words
        self.code = {self.start + 4*i: w for i, w in enumerate(words)}
        self.coverage = set()

    def run(self, case):
        present, info, width, index, h, v, flip, replacement, new_width = case
        regions = {OWNER: bytearray(56), FIRST: bytearray(36), SECOND: bytearray(36), STACK: bytearray(512)}
        def mem(address, size, value=None):
            assert address % size == 0, 'unaligned access'
            for base, data in regions.items():
                off = address - base
                if 0 <= off and off + size <= len(data):
                    if value is None:
                        return int.from_bytes(data[off:off+size], 'big')
                    data[off:off+size] = (value & ((1 << (size*8))-1)).to_bytes(size, 'big')
                    return
            raise AssertionError('access outside synthetic regions')
        mem(OWNER, 4, NAME); mem(OWNER+8, 4, FIRST if info else 0)
        mem(FIRST+16, 2, width); mem(SECOND+16, 2, new_width)
        for off, value in zip((20,22,28,30,32,34), (111,222,-30000,1234,-5678,30000)):
            mem(OWNER+off, 2, value)
        for off, value in zip((24,25,26,27), (17,flip,-7,19)):
            mem(OWNER+off, 1, value)
        original = bytes(regions[OWNER])
        r = [0]*32
        r[4:8] = [OWNER if present else 0, index & MASK, h & MASK, v & MASK]
        r[29], r[31] = STACK+256, STOP
        saved = [0x51e00000+i for i in range(9)]
        for reg, val in zip(list(range(16,24))+[30], saved): r[reg] = val
        pc, pending, hi, lo, steps = self.start, None, 0, 0, 0
        renames = updates = 0
        while pc != STOP:
            if pc in (self.addresses['func_800EF5B0'], self.addresses['Input_ApplyPadConfig']):
                assert pending is None and r[4] == OWNER
                if pc == self.addresses['func_800EF5B0']:
                    assert r[5:7] == [NAME, 1]
                    renames += 1
                    mem(OWNER+8, 4, SECOND if replacement else 0)
                else:
                    updates += 1
                for reg in list(range(1,16))+[24,25]: r[reg] = 0xc10b0000+reg
                pc = r[31]
                continue
            assert pc in self.code and steps < 500
            self.coverage.add(pc-self.start)
            w = self.code[pc]; op = w >> 26; rs = (w >> 21)&31; rt = (w >> 16)&31
            rd = (w >> 11)&31; sh = (w >> 6)&31; fn = w&63; imm = w&65535
            si = signed(imm,16); address = (r[rs]+si)&MASK
            old, pending = pending, None
            next_pc = pc+4
            if not w: pass
            elif op == 0:
                if fn == 0: r[rd] = r[rt] << sh
                elif fn == 8: pending = r[rs]
                elif fn == 13: return {'trap': (w >> 16)&0x3ff}
                elif fn == 16: r[rd] = hi
                elif fn == 18: r[rd] = lo
                elif fn == 25:
                    product = r[rs]*r[rt]; lo, hi = product&MASK, product>>32
                elif fn == 26:
                    a, b = signed(r[rs]), signed(r[rt])
                    # MIPS div executes before compiler-inserted trap guards.
                    if b:
                        q = quotient(a,b); lo, hi = q&MASK, (a-q*b)&MASK
                    else: lo, hi = 0, 0
                elif fn == 33: r[rd] = r[rs]+r[rt]
                elif fn == 35: r[rd] = r[rs]-r[rt]
                elif fn == 37: r[rd] = r[rs]|r[rt]
                else: raise AssertionError(('unsupported special',fn))
            elif op in (2,3):
                if op == 3: r[31] = pc+8
                pending = ((pc+4)&0xf0000000)|((w&0x3ffffff)<<2)
            elif op in (4,5,20,21):
                take = (r[rs] == r[rt]) if op in (4,20) else (r[rs] != r[rt])
                if take: pending = pc+4+4*si
                elif op in (20,21): next_pc += 4
            elif op == 9: r[rt] = address
            elif op == 15: r[rt] = imm << 16
            elif op == 35: r[rt] = mem(address,4)
            elif op == 36: r[rt] = mem(address,1)
            elif op == 37: r[rt] = mem(address,2)
            elif op == 33: r[rt] = signed(mem(address,2),16)
            elif op == 41: mem(address,2,r[rt])
            elif op == 43: mem(address,4,r[rt])
            else: raise AssertionError(('unsupported opcode',op))
            r[:] = [x&MASK for x in r]; r[0] = 0
            pc = old if old is not None else next_pc
            steps += 1
        assert r[29] == STACK+256 and r[31] == STOP
        assert [r[i] for i in list(range(16,24))+[30]] == saved
        allowed = set(range(8,12)) | set(range(28,36))
        assert all(i in allowed or b == original[i] for i,b in enumerate(regions[OWNER]))
        ptr = mem(OWNER+8,4)
        return [signed(mem(OWNER+i,2),16) for i in (28,30,32,34,20,22)] + [
            mem(OWNER+24,1), mem(OWNER+25,1), signed(mem(OWNER+26,1),8), mem(OWNER+27,1),
            renames, updates, 0 if ptr == 0 else (1 if ptr == FIRST else 2)]


def cases():
    # Null owner, missing textures, zero quotient, negative signed indices/steps,
    # flipped partial rows, and writes that narrow before the next coordinate.
    out = [(p,i,w,x,h,v,f,r,nw) for p,i,w,x,h,v,f,r,nw in itertools.product(
        (0,1),(0,1),(0,1,127,65535),(-37,0,32767),(-129,1,256),(-3,16),(0,1),(0,1),(0,73))]
    rng = random.Random(0xFE5B0)
    for _ in range(2000):
        h = rng.randint(-512,512) or 1
        out.append((1,rng.randrange(2),rng.randrange(65536),rng.randint(-65535,65535),h,
                    rng.randint(-64,256),rng.choice((0,1,255)),rng.randrange(2),rng.randrange(65536)))
    return out


def host_function(directory, source=None, label='host'):
    output = directory/(label+'.so')
    cmd = ['cc','-std=c89','-O1','-Wall','-Wextra','-Werror','-shared','-fPIC',
           '-fsanitize=undefined','-fno-sanitize-recover=all']
    if source: cmd.append('-DCANDIDATE="'+str(Path(source).resolve())+'"')
    subprocess.run(cmd+[str(HERE/'semantic_test.c'),'-o',str(output)],check=True,capture_output=True,text=True)
    lib = ctypes.CDLL(str(output))
    fn = lib.run_case
    fn.argtypes = [ctypes.c_int]*9+[ctypes.POINTER(ctypes.c_int)]
    fn.restype = None
    def run(case):
        out = (ctypes.c_int*13)(); fn(*case,out); return list(out)
    return run


def verify(directory, linked_words=None):
    samples = cases(); native = Native(); linked = Native(linked_words) if linked_words is not None else None
    host = host_function(directory)
    for case in samples:
        expected = oracle(case)
        assert native.run(case) == expected, ('native',case)
        assert host(case) == expected, ('host',case)
        if linked: assert linked.run(case) == expected, ('linked',case)
    traps = [((1,1,1,3,0,1,0,1,1),7), ((1,1,1,-2147483648,-1,1,0,1,1),6)]
    for case,code in traps:
        assert native.run(case) == {'trap':code}
        if linked: assert linked.run(case) == {'trap':code}
    source = (ROOT/'cloud/matches/stat_race_update.c').read_text()
    changes = {
        'reverse_column': ('secPerRow - col - 1','secPerRow - col'),
        'zero_quotient': ('secPerRow = 1;','secPerRow = 2;'),
        'signed_index': ('row = index / secPerRow;','row = (unsigned int)index / secPerRow;'),
        'wrong_flip': ('if (blt->Flip) {','if (!blt->Flip) {'),
        'missing_update': ('Input_ApplyPadConfig(blt);','(void)blt;'),
    }
    discriminators = {}
    for label,(before,after) in changes.items():
        assert source.count(before) == 1
        path = directory/(label+'.c'); path.write_text(source.replace(before,after))
        changed = host_function(directory,path,label)
        for case in samples:
            if changed(case) != oracle(case):
                discriminators[label] = list(case); break
        assert label in discriminators, 'mutation was not detected: '+label
    return {'cases':len(samples),'native_trap_cases':len(traps),'executed_instruction_offsets':len(native.coverage),
            'native_words':len(native.code),'host_ubsan':True,'linked_replay':linked is not None,
            'case_sha256':hashlib.sha256(repr(samples).encode()).hexdigest(),
            'mutation_discriminators':discriminators,
            'limits':'Synthetic caller contracts; no callee behavior, gameplay-domain, image or ROM claim. Undefined host divisions/overflow excluded; native zero/overflow traps checked separately.'}
