"""Bounded differential execution of authenticated native and compiled C bodies.

Only the integer instructions present in these functions are supported. Unknown
instructions, unmapped reads, out-of-bounds writes and unexpected callees
fail closed. Callees are explicit deterministic boundary models, not recovered
implementations. No instruction arrays are emitted in the receipt.
"""
import hashlib
import json
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

WORK = Path(__file__).resolve().parent
SOURCE = WORK / 'func_800178B0.c'
START = 0x800178B0
STATE = 0x80043EB8
LEVELS = 0x8004F2B8
BANK_A = 0x80100000
BANK_B = 0x80101000
PROGRAM = 0x80102000
DATA = 0x80103000
OPTIONS = 0x80105000
GROUP_MAP = 0x80106000
FADE = 0x80107000
SP = 0x80701000
EXIT = 0x80FFFFFF
MASK = 0xFFFFFFFF


def signed(x):
    return x - 0x100000000 if x & 0x80000000 else x


class Memory:
    def __init__(self):
        self.regions = []

    def map(self, address, size, fill=0):
        assert not any(address < a + len(b) and a < address + size for a, b in self.regions)
        self.regions.append((address, bytearray([fill]) * size))

    def locate(self, address, size):
        for a, b in self.regions:
            if a <= address and address + size <= a + len(b):
                return b, address - a
        raise AssertionError(('unmapped memory', hex(address), size))

    def read(self, address, size):
        b, o = self.locate(address, size)
        return int.from_bytes(b[o:o+size], 'big')

    def write(self, address, size, value):
        b, o = self.locate(address, size)
        b[o:o+size] = (value & ((1 << (8*size))-1)).to_bytes(size, 'big')

    def digest(self):
        h = hashlib.sha256()
        for a, b in self.regions:
            if a != SP - 0x1000:
                h.update(struct.pack('>I', a))
                h.update(b)
        return h.hexdigest()


def fixture(slot, flags, program, master, mutation):
    m = Memory()
    m.map(STATE, 8 * 4088, 0xA5)
    m.map(LEVELS, 64, 0xA5)
    for address, size in [(BANK_A, 8*8), (BANK_B, 8*8), (PROGRAM, 132),
                          (DATA, 0x1000), (OPTIONS, 32), (GROUP_MAP, 8), (FADE, 4),
                          (SP-0x1000, 0x2000)]:
        m.map(address, size)
    for i in range(8):
        m.write(STATE + i*4088 + 4033, 1, int(i >= slot))
    for bank_number, address in enumerate((BANK_A, BANK_B)):
        for i in range(7):
            # Duplicate destination channels exercise last-writer precedence.
            m.write(address+i*8, 4, (0x80000000 | (bank_number << 28) | i*0x10101 | 0xFE))
            m.write(address+i*8+4, 1, 255 if i == 4 else i%3)
            m.write(address+i*8+5, 1, i*17)
        m.write(address+7*8+5, 1, 255)
    for i in range(16):
        for j in range(5):
            m.write(PROGRAM+4+i*8+j, 1, (i*7+j*13+mutation)&127)
    m.write(DATA, 4, 32)
    m.write(DATA+12, 4, 768 if master else 0)
    m.write(DATA+16, 4, 0xFFFFFFFE if master else 120)
    for i in range(64):
        m.write(DATA+32+4*i, 4, 0 if i%3==0 else 512+i*4)
    if flags is not None:
        m.write(OPTIONS, 4, flags)
        m.write(OPTIONS+4, 4, 0x80000001)
        m.write(OPTIONS+8, 4, 0xAAAAAAAA)
        m.write(OPTIONS+12, 2, 0xFFFF if master else 0)
        m.write(OPTIONS+14, 2, 0xFFFF)
        m.write(OPTIONS+16, 1, 255)
        m.write(OPTIONS+18, 1, 0 if mutation==2 else 4)
        m.write(OPTIONS+20, 4, GROUP_MAP)
        m.write(OPTIONS+24, 1, 0 if mutation==2 else 4)
        m.write(OPTIONS+28, 4, FADE)
        for i in range(4):
            m.write(GROUP_MAP+i*2, 1, [0, 63, 17, 63][i])
            m.write(GROUP_MAP+i*2+1, 1, 4+i)
            m.write(FADE+i, 1, 10+i)
    m.write(SP+16, 4, OPTIONS if flags is not None else 0)
    return m, [BANK_A, BANK_B, PROGRAM if program else 0, DATA]


class Machine:
    def __init__(self, words):
        self.words = words
        self.coverage = set()
        self.calls = 0
        self.steps = 0

    def run(self, case):
        slot, flags, program, master, mutation = case
        memory, args = fixture(*case)
        regs = [(0xCCC00000 + i*0x111)&MASK for i in range(32)]
        regs[0] = 0
        regs[4:8] = args
        regs[29], regs[31] = SP, EXIT
        original = regs[:]
        pc, pending = START, None
        trace = []
        for step in range(30000):
            if pc == EXIT:
                assert regs[16:24] == original[16:24]
                assert regs[28:31] == original[28:31]
                self.steps += step
                return regs[2], memory.digest(), trace
            if not START <= pc < START+len(self.words)*4:
                assert pending is None
                a = regs[4:8]
                result = self.hook(pc, a, regs, memory, trace, mutation)
                next_pc = regs[31]
                for i in [1, 2, 3, *range(4, 16), 24, 25]:
                    regs[i] = (0xABC00000 + i*0x101 + len(trace))&MASK
                regs[2] = result
                pc = next_pc
                continue
            assert pc%4 == 0
            self.coverage.add(pc-START)
            w = self.words[(pc-START)//4]
            op, rs, rt, rd, sa, fn = w>>26, (w>>21)&31, (w>>16)&31, (w>>11)&31, (w>>6)&31, w&63
            imm = w&65535
            simm = imm-65536 if imm&32768 else imm
            next_pc, branch, annul = pc+4, None, False
            if op == 0:
                if fn == 0: regs[rd] = (regs[rt]<<sa)&MASK
                elif fn == 2: regs[rd] = regs[rt]>>sa
                elif fn == 8: branch = regs[rs]
                elif fn == 0x21: regs[rd] = (regs[rs]+regs[rt])&MASK
                elif fn == 0x23: regs[rd] = (regs[rs]-regs[rt])&MASK
                elif fn == 0x24: regs[rd] = regs[rs]&regs[rt]
                elif fn == 0x25: regs[rd] = regs[rs]|regs[rt]
                elif fn == 0x2A: regs[rd] = int(signed(regs[rs]) < signed(regs[rt]))
                elif fn == 0x2B: regs[rd] = int(regs[rs] < regs[rt])
                else: raise AssertionError(('special', hex(pc), fn))
            elif op == 3:
                regs[31] = pc+8
                branch = ((pc+4)&0xF0000000)|((w&0x3FFFFFF)<<2)
            elif op == 9: regs[rt] = (regs[rs]+simm)&MASK
            elif op == 10: regs[rt] = int(signed(regs[rs]) < simm)
            elif op == 11: regs[rt] = int(regs[rs] < (simm&MASK))
            elif op == 12: regs[rt] = regs[rs]&imm
            elif op == 13: regs[rt] = regs[rs]|imm
            elif op == 15: regs[rt] = imm<<16
            elif op in (32, 33, 35, 36, 37, 40, 41, 43):
                address = (regs[rs]+simm)&MASK
                size = {32:1, 33:2, 35:4, 36:1, 37:2, 40:1, 41:2, 43:4}[op]
                assert address%size == 0
                if op >= 40: memory.write(address, size, regs[rt])
                else:
                    value = memory.read(address, size)
                    if op in (32,33) and value>>(size*8-1): value -= 1<<(size*8)
                    regs[rt] = value&MASK
            elif op in (4,5,6,7,20,21,22,23):
                cond = {4:regs[rs]==regs[rt], 5:regs[rs]!=regs[rt],
                        6:signed(regs[rs])<=0, 7:signed(regs[rs])>0}[op-16 if op>=20 else op]
                if cond: branch = pc+4+simm*4
                elif op>=20: annul = True
            elif op == 1:
                assert rt in (0,1)
                if (signed(regs[rs])<0) == (rt==0): branch = pc+4+simm*4
            else: raise AssertionError(('opcode', hex(pc), op))
            regs[0] = 0
            if pending is not None:
                assert branch is None and not annul
                next_pc = pending
            elif annul: next_pc = pc+8
            pending, pc = branch, next_pc
        raise AssertionError('instruction bound exceeded')

    def hook(self, pc, a, regs, m, trace, mutation):
        self.calls += 1
        result = 0
        if pc == 0x8001785C:
            trace.append([pc, *a[:2]])
            for i in range(128): m.write(a[0]+i, 1, 255)
            index = 0
            while m.read(a[1]+index*8+5, 1) != 255:
                key = m.read(a[1]+index*8+5, 1)
                assert key < 128 and index < 128
                m.write(a[0]+key, 1, index)
                index += 1
        elif pc == 0x8001C19C:
            trace.append([pc, *a[:2]])
            if mutation == 1 and a[0] == 4:
                m.write(GROUP_MAP+3, 1, 17)
                m.write(OPTIONS+18, 1, 3)
        elif pc == 0x8001B9F8:
            trace.append([pc, *a, m.read(regs[29]+16, 4)])
            if mutation == 1:
                m.write(OPTIONS+16, 1, m.read(OPTIONS+16, 1)-1)
                m.write(OPTIONS+24, 1, 2)
        elif pc == 0x80019A60:
            trace.append([pc, *a[:2]])
            assert m.read(STATE+a[1]*4088+292, 4) == a[0]
        elif pc == 0x80020820:
            trace.append([pc, *a[:2]])
        elif pc == 0x80017720:
            trace.append([pc, *a[:3]])
            if mutation == 1:
                m.write(PROGRAM+5+a[2]*8, 1, (a[2]+77)&127)
        elif pc == 0x80020610:
            trace.append([pc, *a])
            if mutation == 1 and a[0] == 7:
                m.write(PROGRAM+6+a[1]*8, 1, (a[1]+23)&127)
        elif pc == 0x800175B4:
            trace.append([pc, a[0]])
            result = (0xFEDCBA00+a[0])&MASK
            m.write(STATE+a[0]*4088, 4, result)
        else: raise AssertionError(('unexpected callee', hex(pc)))
        return result



def reachable_offsets(words):
    """Conservative integer CFG: both conditional paths, delay slots and returns."""
    seen, states = set(), [(0, None)]
    while states:
        pc, pending = states.pop()
        if (pc, pending) in seen or not 0 <= pc < len(words)*4:
            continue
        seen.add((pc, pending))
        word = words[pc//4]
        op, rs, rt = word>>26, (word>>21)&31, (word>>16)&31
        immediate = word&65535
        immediate = immediate-65536 if immediate&32768 else immediate
        if pending is not None:
            states.append((pending, None))
        elif op == 3:
            states.append((pc+4, pc+8))
        elif op == 0 and word&63 == 8:
            states.append((pc+4, 0xFFFFFFFF))
        elif op in (1,4,5,6,7,20,21,22,23):
            states.append((pc+4, pc+4+immediate*4))
            if not (op == 4 and rs == rt):
                states.append((pc+8, None) if op >= 20 else (pc+4, pc+8))
        else:
            states.append((pc+4, None))
    return {pc for pc, unused in seen}


def run(obj):
    score.ASM_DIR = ROOT/'asm/us/boot_tail'
    target = score.targets()['func_800178B0']
    assert len(target) == 290
    assert hashlib.sha256(struct.pack('>290I', *target)).hexdigest() == 'e2d3b7c8631d3ff01d9ec07b44abeab7d197f2eb0e93c9a68d6a9d1abcaf4b3a'
    raw = score.text_words(obj)
    words, masks, unresolved, unverified, errors = score.relocate(obj, raw, 0, len(raw)*4, score.image_symbols())
    assert not (masks or unresolved or unverified or errors)
    native, candidate = Machine(target), Machine(words)
    cases = [(slot, flags, program, master, mutation)
             for slot in range(9) for flags in [None, *range(32)]
             for program, master in ((False,False),(True,True)) for mutation in (0,1,2)]
    results = []
    for case in cases:
        expected, actual = native.run(case), candidate.run(case)
        assert expected == actual, ('behavior mismatch', case, expected, actual)
        results.append(expected)
    reachable = reachable_offsets(target)
    assert native.coverage == reachable
    return {'status':'PASS', 'cases':len(cases), 'native_reachable_instructions':len(reachable),
            'all_reachable_native_instructions_covered':True, 'native_instruction_coverage':len(native.coverage),
            'native_instruction_count':len(target), 'native_steps':native.steps, 'candidate_steps':candidate.steps,
            'native_call_count':native.calls, 'candidate_call_count':candidate.calls,
            'result_sha256':hashlib.sha256(json.dumps(results, separators=(',',':')).encode()).hexdigest(),
            'uncovered_native_offsets':[i*4 for i in range(len(target)) if i*4 not in native.coverage],
            'scope':'Finite valid banks and offsets, all 32 option-bit combinations, first/middle/last/no free slot, null/present setup and master, zero/nonzero counts, live option/program mutation, caller-save clobbering, complete non-stack memory and callback trace equality. External helpers use explicit boundary models.'}

if __name__ == '__main__':
    print(json.dumps(run(Path(sys.argv[1])), indent=2))
