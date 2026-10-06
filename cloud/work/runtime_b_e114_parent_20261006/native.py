"""Fail-closed bounded execution of three complete private-ABI native children.

Also executes their complete, independently GNU-linked ordinary-ABI C objects.
External service calls are explicit effect models and caller-save poison points.
Finite binary32 multiplication is rounded once; FCSR/NaN payloads are excluded.
No original target words or asset bytes are stored here.
"""
import struct

RECORD, OBJECTS, STACK, RETURN = 0x100000, 0x110000, 0x700080, 0xFFFFFFFC
POOL, RESOURCES, IDENTITY = 0x80394F70, 0x80399B18, 0x8011418C
MASK = 0xFFFFFFFF


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value >> (bits - 1) else value


def bits(value):
    return struct.unpack('>I', struct.pack('>f', value))[0]


def real(value):
    return struct.unpack('>f', struct.pack('>I', value))[0]


class Machine:
    def __init__(self, code, start, rodata, kind, flags, mode, mutation,
                 private, name, resource=0, parent=-1, create_flags=0,
                 result=-1, null_allocation=False):
        self.code, self.start = code, start
        self.memory = {}
        self.state_addresses = set()
        self.trace, self.coverage = [], set()
        self.name, self.kind, self.mutation = name, kind, mutation
        self.resource, self.result = resource, result
        self.null_allocation = null_allocation
        self.r = [(0xA5000000+i*397)&MASK for i in range(32)]
        self.f = [bits(1.0+i*0.125) for i in range(32)]
        self.r[0], self.r[29], self.r[31] = 0, STACK, RETURN
        self.original_f = list(self.f)
        for base, size in [(RECORD, 104), (OBJECTS, 180), (RESOURCES, 68), (IDENTITY, 36)]:
            self.memory.update({base+i: 0 for i in range(size)})
            self.state_addresses.update(range(base, base+size))
        self.memory.update({STACK+i: (i*13)&255 for i in range(-128, 96)})
        self.writable = set(self.memory)
        for address, data in rodata:
            for i, value in enumerate(data): self.memory[address+i] = value
        self.put(RECORD+6, kind, 1); self.put(RECORD+7, flags, 1)
        self.put(RECORD+0x60, OBJECTS); self.put(RECORD+0x64, OBJECTS+60)
        for i, value in enumerate([0x12348000, 0x7654FFFF, 0x24687FFF]):
            self.put(OBJECTS+i*60+4, value)
        for i in range(3):
            self.put(RECORD+0x20+i*4, bits((i*3-8)*0.125))
            for j in range(3):
                self.put(OBJECTS+8+(3*i+j)*4, bits((i*9+j*2-40)*0.125))
                self.put(RECORD+0x38+(3*i+j)*4, bits((i*9+j*2-80)*0.125))
                self.put(IDENTITY+(3*i+j)*4, bits((i*9+j*2-120)*0.125))
        for i in range(17): self.put(RESOURCES+4*i, i*1009-811)
        if name == 'func_8038D200':
            if private: self.r[17], self.r[4] = RECORD, mode&MASK
            else: self.r[4:6] = [RECORD, mode&MASK]
        elif name == 'func_8038D328':
            args = [resource, parent&MASK, create_flags, mode&MASK]
            if private: self.r[17:21] = args
            else: self.r[4:8] = args
        else:
            self.r[17 if private else 4] = RECORD
        self.original = list(self.r)
        self.private = private

    def get(self, address, width=4):
        assert address % width == 0, ('unaligned read', hex(address), width)
        assert all(address+i in self.memory for i in range(width)), ('unmapped read', hex(address))
        return int.from_bytes(bytes(self.memory[address+i] for i in range(width)), 'big')

    def put(self, address, value, width=4):
        assert address % width == 0, ('unaligned write', hex(address), width)
        assert all(address+i in self.memory for i in range(width)), ('unmapped write', hex(address))
        assert all(address+i in self.writable for i in range(width)), ('read-only write', hex(address))
        raw = (value & ((1 << (8*width))-1)).to_bytes(width, 'big')
        for i, byte in enumerate(raw): self.memory[address+i] = byte

    def call(self, target):
        r, f = self.r, self.f
        ret = 0xBAD00002
        if target == 0x8008D6B0:
            self.trace.append(('copy', r[4], r[5]))
            for i in range(9): self.put(r[5]+4*i, self.get(r[4]+4*i))
            if self.mutation:
                self.put(RECORD+6, 0, 1); self.put(RECORD+7, 0x92, 1)
                self.put(RECORD+0x60, OBJECTS+120)
        elif target == 0x80090F44:
            self.trace.append(('pitch', f[12], r[5]))
        elif target == 0x8008E3C0:
            self.trace.append(('allocate', r[4]))
            if self.mutation:
                a = RESOURCES+4*self.resource; self.put(a, self.get(a)+701)
            ret = 0 if self.null_allocation else OBJECTS
        elif target == 0x8008E398:
            self.trace.append(('create', *r[4:8])); ret = self.result&MASK
        elif target == 0x80090254:
            self.trace.append(('delete', r[4]))
        elif target == 0x800AFA84:
            self.trace.append(('release', r[4], r[5]))
            if self.mutation and self.kind == 3 and r[5] == OBJECTS+60:
                self.put(RECORD+0x60, OBJECTS+120); self.put(RECORD+6, 5, 1)
        else: raise AssertionError(('unknown external call', hex(target)))
        for i in list(range(1,16))+[24,25]: r[i] = (0xBAD00000+i*79)&MASK
        for i in range(20): f[i] = 0x7FC00000+i
        r[2] = ret

    def run(self):
        pc, pending = self.start, None
        for steps in range(500):
            if pc == RETURN: break
            assert pc in self.code, ('unknown pc', hex(pc))
            self.coverage.add(pc)
            w = self.code[pc]; op = w >> 26
            rs, rt, rd, sh = (w>>21)&31, (w>>16)&31, (w>>11)&31, (w>>6)&31
            imm, si = w&65535, signed(w, 16)
            r, f = self.r, self.f
            address = (r[rs]+si)&MASK
            old_pending, pending, next_pc = pending, None, pc+4
            if op == 0:
                fn = w&63
                if fn == 0: r[rd] = (r[rt] << sh)&MASK
                elif fn == 3: r[rd] = (signed(r[rt]) >> sh)&MASK
                elif fn == 0x21: r[rd] = (r[rs]+r[rt])&MASK
                elif fn == 0x25: r[rd] = r[rs] | r[rt]
                elif fn == 8: pending = r[rs]
                else: raise AssertionError(('SPECIAL', hex(pc), hex(w)))
            elif op == 3:
                r[31] = pc+8
                pending = ('call', ((pc+4)&0xF0000000)|((w&0x3FFFFFF)<<2), pc+8)
            elif op == 9: r[rt] = address
            elif op == 11: r[rt] = int(r[rs] < (si&MASK))
            elif op == 12: r[rt] = r[rs]&imm
            elif op == 15: r[rt] = imm << 16
            elif op in (4,5,20,21):
                take = (r[rs] == r[rt]) if op in (4,20) else (r[rs] != r[rt])
                if take: pending = pc+4+si*4
                elif op in (20,21): next_pc += 4
            elif op in (32,33,35,36):
                width = 4 if op == 35 else 2 if op == 33 else 1
                val = self.get(address,width)
                r[rt] = (signed(val,width*8)&MASK) if op in (32,33) else val
            elif op == 43: self.put(address,r[rt])
            elif op == 49: f[rt] = self.get(address)
            elif op == 57: self.put(address,f[rt])
            elif op == 17 and rs == 16 and w&63 == 2:
                f[sh] = bits(real(f[rd])*real(f[rt]))
            else: raise AssertionError(('opcode', hex(pc), hex(w)))
            r[0] = 0
            assert old_pending is None or pending is None, 'control transfer in delay slot'
            if isinstance(old_pending, tuple):
                self.call(old_pending[1]); pc = old_pending[2]
            else: pc = old_pending if old_pending is not None else next_pc
        else: raise AssertionError('step limit')
        assert self.r[29] == STACK and self.r[28] == self.original[28]
        assert self.f[20:] == self.original_f[20:]
        if not self.private:
            assert self.r[16:24] == self.original[16:24] and self.r[30] == self.original[30]
        result = self.r[2] if self.name == 'func_8038D328' else None
        state = bytes(self.memory[a] for a in sorted(self.state_addresses))
        return result, state, self.trace


def verify(targets, native_data, candidate_code, candidate_starts, candidate_data):
    coverage = {name: set() for name in candidate_starts}
    candidate_coverage = {name: set() for name in candidate_starts}
    count = 0
    def check(name, kind=0, flags=0, mode=0, mutation=0, **extra):
        nonlocal count
        address = int(name[-8:],16)
        words = targets[name]
        original = Machine({address+4*i:w for i,w in enumerate(words)}, address,
                           native_data, kind, flags, mode, mutation, True, name, **extra)
        compiled = Machine(candidate_code, candidate_starts[name], candidate_data,
                           kind, flags, mode, mutation, False, name, **extra)
        assert original.run() == compiled.run(), (name,kind,flags,mode,mutation,extra)
        coverage[name].update(original.coverage)
        candidate_coverage[name].update(compiled.coverage)
        count += 1
    for mutation in (0,1):
        for kind in (0,1,2,3,4,5,6,7,8,9,255):
            for flags in range(256):
                for mode in (-1,0,1,2,3): check('func_8038D200',kind,flags,mode,mutation)
        for resource in range(17):
            for mode in (0,-7):
                for parent in (-1,0,32767,-32768):
                    for flags in (0,0x2000,0x2080,0x800000):
                        for result in (-32768,-1,0,32767):
                            check('func_8038D328', mode=mode, mutation=mutation,
                                  resource=resource,parent=parent,create_flags=flags,result=result)
        for kind in range(256): check('func_8038E088',kind=kind,mutation=mutation)
    # Failure-domain checks: no allocator-success fiction or implicit zero memory.
    name='func_8038D328'; address=int(name[-8:],16)
    bad=Machine({address+4*i:w for i,w in enumerate(targets[name])},address,
                native_data,0,0,0,0,True,name,null_allocation=True)
    try: bad.run()
    except AssertionError as exc: assert 'unmapped' in str(exc)
    else: raise AssertionError('null allocation unexpectedly succeeded')
    invalid=Machine({address:0xFFFFFFFF},address,native_data,0,0,0,0,True,name)
    try: invalid.run()
    except AssertionError as exc: assert 'opcode' in str(exc)
    else: raise AssertionError('unknown opcode accepted')
    for name, covered in coverage.items():
        address=int(name[-8:],16)
        assert covered == {address+4*i for i in range(len(targets[name]))}, (name,'native coverage')
    return {'paired_fixtures':count,'native_instruction_coverage':{n:len(c) for n,c in coverage.items()},
            'candidate_instruction_coverage':{n:len(c) for n,c in candidate_coverage.items()},
            'negative_controls':['null allocation refused','unknown opcode refused']}
