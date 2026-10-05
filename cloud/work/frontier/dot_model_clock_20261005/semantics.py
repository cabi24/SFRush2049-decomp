"""Bounded raw-native, GNU-linked, arithmetic-oracle and unmodified host-C checks."""
import ctypes
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
FN = 'func_800E762C'
MASK = 0xffffffff
BASE = 0x8014A250
TICKS = 0x80143FF4
TIME = 0x801543CC
STEP = 0x8002AFB8
MODE = 0x8014A110
STOP = 0xfffffff0

def signed(v):
    v &= MASK
    return v - (1 << 32) if v & (1 << 31) else v

def bits(v): return struct.unpack('>I', struct.pack('>f', v))[0]
def value(v): return struct.unpack('>f', struct.pack('>I', v))[0]
def f32(v): return value(bits(v))
def pack(ws): return struct.pack('>%dI' % len(ws), *ws)

def initial(c):
    memory = {TICKS: bytearray(pack([123])), TIME: bytearray(pack([bits(567.0)])),
              STEP: bytearray(pack([c[2]])), MODE: bytearray(pack([c[1]])),
              BASE: bytearray(b'\xa5' * (2056 * 6))}
    for i in range(6): memory[BASE][2056*i+1816:2056*i+1820] = pack([c[3+i]])
    return memory

def result(memory): return bytes(memory[TICKS] + memory[TIME] + memory[BASE])

def oracle(c):
    memory = initial(c)
    ticks = signed(-c[0]); step = value(c[2]); time = f32(f32(ticks) * step)
    memory[TICKS][:] = pack([ticks & MASK]); memory[TIME][:] = pack([bits(time)])
    for i in range(6):
        rate = value(c[3+i])
        if signed(c[1]) != 2 or i == 0 or rate == 0.0:
            n = ticks; rate_bits = c[2]
        else:
            ratio = f32(time / rate)
            rounded = f32(ratio - 0.5 if ratio < 0.0 else ratio + 0.5)
            assert -2147483648 <= rounded < 2147483648
            n = int(rounded); rate_bits = c[3+i]
        memory[BASE][2056*i+1808:2056*i+1820] = pack([n & MASK, bits(time), rate_bits])
    return result(memory)

class Native:
    def __init__(self, words):
        self.start = 0x800E762C
        self.code = {self.start + 4*i: w for i, w in enumerate(words)}
        self.coverage = set()
    def run(self, c):
        memory = initial(c)
        r = [0xC1000000+i for i in range(32)]
        f = [bits(.25+i) for i in range(32)]
        saved_r = r[16:24] + [r[28], r[29], r[30]]
        saved_f = f[20:]
        r[0] = 0; r[4] = c[0]; r[31] = STOP
        reads = []; writes = []
        def mem(a, data=None):
            assert a % 4 == 0
            for base, region in memory.items():
                off = a-base
                if 0 <= off <= len(region)-4:
                    if data is None:
                        reads.append(a)
                        return int.from_bytes(region[off:off+4], 'big')
                    region[off:off+4] = pack([data & MASK]); writes.append(a); return
            raise AssertionError(('out of bounds', hex(a)))
        pc = self.start; pending = None; condition = False; count = 0
        while pc != STOP:
            assert pc in self.code and count < 1000, hex(pc)
            self.coverage.add(pc-self.start)
            w = self.code[pc]; op = w >> 26; rs = (w >> 21) & 31; rt = (w >> 16) & 31
            rd = (w >> 11) & 31; sh = (w >> 6) & 31; fn = w & 63
            imm = w & 65535; si = imm-65536 if imm & 32768 else imm
            address = (r[rs]+si) & MASK
            old, pending = pending, None; next_pc = pc+4
            if not w: pass
            elif op == 0:
                if fn == 8: pending = r[rs]
                elif fn == 33: r[rd] = r[rs]+r[rt]
                elif fn == 35: r[rd] = r[rs]-r[rt]
                elif fn == 37: r[rd] = r[rs] | r[rt]
                else: raise AssertionError(('special', fn))
            elif op in (4, 5, 20, 21):
                take = (r[rs] == r[rt]) if op in (4, 20) else (r[rs] != r[rt])
                if take: pending = pc+4+4*si
                elif op in (20, 21): next_pc += 4
            elif op == 9: r[rt] = address
            elif op == 15: r[rt] = imm << 16
            elif op == 35: r[rt] = mem(address)
            elif op == 43: mem(address, r[rt])
            elif op == 49: f[rt] = mem(address)
            elif op == 57: mem(address, f[rt])
            elif op == 17:
                if rs == 0: r[rt] = f[rd]
                elif rs == 4: f[rd] = r[rt]
                elif rs == 8:
                    take = condition if rt & 1 else not condition
                    if take: pending = pc+4+4*si
                    elif rt & 2: next_pc += 4
                elif rs == 20:
                    assert fn == 32; f[sh] = bits(signed(f[rd]))
                elif rs == 16:
                    x, y = value(f[rd]), value(f[rt])
                    if fn == 0: f[sh] = bits(x+y)
                    elif fn == 1: f[sh] = bits(x-y)
                    elif fn == 2: f[sh] = bits(x*y)
                    elif fn == 3: f[sh] = bits(x/y)
                    elif fn == 13:
                        assert -2147483648 <= x < 2147483648
                        f[sh] = int(x) & MASK
                    elif fn == 50: condition = x == y
                    elif fn == 60: condition = x < y
                    else: raise AssertionError(('float', fn))
                else: raise AssertionError(('cop1', rs))
            else: raise AssertionError(('opcode', op))
            r = [v & MASK for v in r]; r[0] = 0
            pc = old if old is not None else next_pc; count += 1
        assert r[16:24] + [r[28], r[29], r[30]] == saved_r
        assert f[20:] == saved_f and r[31] == STOP
        assert reads.count(STEP) == reads.count(MODE) == 1
        assert writes[:2] == [TICKS, TIME]
        expected = [TICKS, TIME]
        for i in range(6):
            expected += [BASE+2056*i+1808, BASE+2056*i+1812]
            if signed(c[1]) != 2 or i == 0 or value(c[3+i]) == 0:
                expected += [BASE+2056*i+1816]
        assert writes == expected
        return result(memory)

def reachable_offsets(words):
    """Conservative branch/delay-slot CFG, with all unknown conditions forked."""
    code = {4*i:w for i,w in enumerate(words)}
    todo = [(0,None)]; states = set(); reached = set()
    while todo:
        pc,pending = todo.pop()
        if (pc,pending) in states or pc is None: continue
        states.add((pc,pending)); assert pc in code, pc
        reached.add(pc); w=code[pc]; op=w>>26
        rs=(w>>21)&31;rt=(w>>16)&31;imm=w&65535;si=imm-65536 if imm&32768 else imm
        if pending is not None:
            if pending != -1:todo.append((pending,None))
            continue
        if op == 0 and (w&63)==8:
            assert rs == 31;todo.append((pc+4,-1))
        elif op in (4,5,20,21) or (op==17 and rs==8):
            likely=op in (20,21) or (op==17 and bool(rt&2))
            outcomes=[True,False]
            if op in (4,5,20,21) and rs==rt:outcomes=[op in (4,20)]
            for taken in outcomes:
                if taken:todo.append((pc+4,pc+4+4*si))
                elif likely:todo.append((pc+8,None))
                else:todo.append((pc+4,pc+8))
        else:todo.append((pc+4,None))
    return reached

def cases():
    out = []
    rates = [0.0, -0.0, .5, 1.0, 2.0, -.5, -1.0, -2.0]
    for ticks, mode, step in itertools.product([0, 1, 2, 3, 17, 1001, MASK, MASK-1, MASK-2], [0, 1, 2, 3, MASK], [.5, .02, .016666668, -.5]):
        for k in range(8):
            out.append([ticks, mode, bits(step)] + [bits(rates[(i+k) % 8]) for i in range(6)])
    for tick in [0x7fffffff, 0x80000000, 0x80000001]:
        for mode in [0, MASK]: out.append([tick, mode, bits(.02)] + [bits(0)]*6)
    rng = random.Random(0xE762C)
    for _ in range(2048):
        out.append([rng.randint(-100000,100000)&MASK, rng.choice([0,2,2,2,3]), bits(rng.choice([.02,.016666668,.5,-.5]))] + [bits(rng.choice(rates)) for i in range(6)])
    return out

def host(directory, source=None, label='host'):
    so = directory/(label+'.so')
    command = ['cc','-std=c89','-O1','-Wall','-Wextra','-Werror','-shared','-fPIC',
               '-ffp-contract=off','-fsanitize=undefined','-fno-sanitize-recover=all']
    if source: command.append('-DCANDIDATE="'+str(source.resolve())+'"')
    subprocess.run(command + [str(HERE/'host.c'),'-o',str(so)],check=True,capture_output=True,text=True)
    library = ctypes.CDLL(str(so)); fn = library.run_case
    fn.argtypes = [ctypes.POINTER(ctypes.c_uint32),ctypes.POINTER(ctypes.c_uint32)]
    fn.restype = None
    def run(c):
        output = (ctypes.c_uint32*(2+2056*6//4))()
        fn((ctypes.c_uint32*9)(*c),output)
        # The output table contains opaque bytes, so endian-swap only numeric words.
        raw = bytearray(ctypes.string_at(ctypes.addressof(output),ctypes.sizeof(output)))
        for off in [0,4]+[8+2056*i+j for i in range(6) for j in (1808,1812,1816)]:
            raw[off:off+4] = int.from_bytes(raw[off:off+4],sys.byteorder).to_bytes(4,'big')
        return bytes(raw)
    return run

def verify(directory, linked_words):
    samples = cases(); native = Native(score.targets()[FN]); linked = Native(linked_words)
    compiled = host(directory)
    for c in samples:
        want = oracle(c)
        assert native.run(c) == want, ('native', c)
        assert linked.run(c) == want, ('linked', c)
        assert compiled(c) == want, ('host', c)
    source = (HERE/'candidate.c').read_text()
    changes = {
        'wrong_sign': ('(s32)(0u - ticks)', '(s32)ticks'),
        'first_car_exception_removed': (' || model == D_8014A250', ''),
        'wrong_round_negative': ('ratio - 0.5f', 'ratio + 0.5f'),
        'rate_overwritten': ('ratio = time / model->step;', 'ratio = time / model->step; model->step = step;'),
        'five_cars': ('model != D_8014A250 + 6', 'model != D_8014A250 + 5'),
    }
    rejected = {}
    for label,(before,after) in changes.items():
        assert source.count(before) == 1
        path = directory/(label+'.c'); path.write_text(source.replace(before,after))
        changed = host(directory,path,label)
        witness = next((c for c in samples if changed(c) != oracle(c)), None)
        assert witness is not None, label
        rejected[label] = witness
    reachable = reachable_offsets(score.targets()[FN])
    assert native.coverage == reachable
    return {'cases':len(samples),'native_cfg_reachable_words':len(reachable),'native_executed_instruction_offsets':sorted(native.coverage),
            'native_covered_words':len(native.coverage),'native_total_words':57,
            'gnu_linked_covered_words':len(linked.coverage),'host_ubsan':'passed',
            'native_and_linked_access_order':'passed','o32_preservation':'passed',
            'rejected_mutants':rejected,
            'domain':'Finite floats; every float-to-s32 conversion defined. Raw tick negation covers 32-bit wrap boundaries.'}
