"""Fail-closed integer MIPS execution for AA8C entry and its real private callees.

Only authenticated target words are supplied at runtime. No native bytes live here.
The accepted scene-removal and hide services are explicit effect boundaries.
"""
PLAYERS, PHYSICS = 0x80152818, 0x8014A250
CACHE, SLOTS, GROUPS, COLOR = 0x80399118, 0x80399120, 0x80399550, 0x80394884
ROOT, GROUP_CLEAN, SLOT_CLEAN = 0x8038AA8C, 0x8038A95C, 0x8038AA14
CONTINUE, RETURN, STACK = 0x8038AB90, 0xFFFFFFFC, 0x700100
MASK = 0xFFFFFFFF


def signed(value, bits=32):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


REGIONS = ((PLAYERS, 4 * 0x3B8), (PHYSICS, 4 * 0x808),
           (CACHE, 8), (SLOTS, 4 * 0x10C), (GROUPS, 4 * 0x148),
           (COLOR, 4), (STACK - 256, 384))
INITIAL_MEMORY = tuple((base, bytes((i * 29 + 73) & 255 for i in range(size)))
                       for base, size in REGIONS)


class Memory:
    def __init__(self):
        self.regions = [(base, bytearray(data)) for base, data in INITIAL_MEMORY]
        self.reads, self.writes = [], []

    def region(self, address, width):
        assert address % width == 0, ('unaligned', hex(address), width)
        for base, data in self.regions:
            if base <= address and address + width <= base + len(data):
                return data, address - base
        raise AssertionError(('unmapped', hex(address), width))

    def get(self, address, width=4):
        data, offset = self.region(address, width)
        self.reads.append((address, width))
        return int.from_bytes(data[offset:offset + width], 'big')

    def put(self, address, value, width=4):
        data, offset = self.region(address, width)
        value &= (1 << (width * 8)) - 1
        data[offset:offset + width] = value.to_bytes(width, 'big')
        self.writes.append((address, width, value))

    def snapshot(self):
        return tuple((base, bytes(data)) for base, data in self.regions if base != STACK - 256)


class Fixture:
    """Shared fixture/service protocol, not shared native/reference control flow."""
    def __init__(self, player, cached, update, inhibit, handles, extra, mutation=0,
                 state139=67, descriptor_handle=0x9234):
        assert player in range(4) and len(handles) == 5
        self.memory = Memory()
        self.player, self.update, self.mutation = player, update, mutation
        self.descriptor = PLAYERS + player * 0x3B8 + 0x2F0
        self.trace = []
        self.remove_count = 0
        m = self.memory
        for p in range(4):
            d = PLAYERS + p * 0x3B8 + 0x2F0
            m.put(d + 8, p, 2)
        m.put(self.descriptor + 6, descriptor_handle, 2)
        m.put(CACHE + player, cached, 1)
        m.put(PHYSICS + player * 0x808 + 0x640, inhibit, 1)
        for base, stride in ((SLOTS, 0x10C), (GROUPS, 0x148)):
            for i, value in enumerate(handles):
                m.put(base + player * stride + 4 * i, value)
        m.put(GROUPS + player * 0x148 + 0x104, extra)
        m.put(GROUPS + player * 0x148 + 0x139, state139, 1)
        m.reads.clear(); m.writes.clear()

    def service(self, destination, args):
        m, p = self.memory, self.player
        slots, group = SLOTS + p * 0x10C, GROUPS + p * 0x148
        if destination == 0x80090254:
            # Full pre-call state detects early sentinel stores and extra-byte clears.
            observed = tuple(m.get(base + 4 * i) for base in (slots, group) for i in range(5))
            observed += (m.get(group + 0x104), m.get(group + 0x138, 1), m.get(group + 0x139, 1))
            self.trace.append(('remove', signed(args[0]), observed))
            self.remove_count += 1
            if self.mutation and self.remove_count == 1:
                # Adversarial test contract only: the real scene-only remover is
                # narrower. These changes test retained pointers and later reloads.
                for base in (slots, group):
                    m.put(base, 0xCAFE0007)
                    m.put(base + 8, 0x1234FFFF)
                m.put(group + 0x104, -1 if self.mutation == 1 else 0xABCDEFFF)
                m.put(group + 0x139, 51, 1)
                m.put(self.descriptor + 8, (p + 1) % 4, 2)
                m.put(self.descriptor + 6, 0x7FED, 2)
                m.put(CACHE + p, 1 if self.mutation == 1 else 0, 1)
                m.put(PLAYERS + p * 0x3B8 + 0x384, 5, 1)
        elif destination == 0x8008AE8C:
            self.trace.append(('hide', signed(args[0]), args[1], args[2],
                               m.get(PLAYERS + p * 0x3B8 + 0x384, 1),
                               m.get(PLAYERS + p * 0x3B8 + 0x385, 1)))
            assert args[1:] == [0, 15], 'only mode-0 hide is modeled'
        else:
            raise AssertionError(('unmodeled external call', hex(destination)))


class Machine:
    def __init__(self, code, fixture, start=ROOT, raw_player=None):
        self.code, self.fixture, self.memory = code, fixture, fixture.memory
        self.start = start
        self.r = [(0xA5000000 + i * 397) & MASK for i in range(32)]
        self.r[0], self.r[29], self.r[31] = 0, STACK, RETURN
        self.r[4:6] = [fixture.descriptor, fixture.update & MASK]
        if start != ROOT:
            incoming = fixture.player if raw_player is None else raw_player
            self.r[20 if start == GROUP_CLEAN else 4] = incoming & MASK
        self.original = self.r[:]
        self.coverage = set()
        self.branches = set()

    def run(self):
        pc, pending = self.start, None
        for _ in range(2000):
            if pc == RETURN:
                assert self.r[29] == STACK and self.r[28] == self.original[28]
                if self.start == ROOT:
                    assert self.r[16:24] == self.original[16:24]
                return 'return'
            if pc == CONTINUE:
                assert self.start == ROOT and self.r[29] == STACK - 136
                return 'continue'
            assert pc in self.code, ('out-of-scope instruction', hex(pc))
            self.coverage.add(pc)
            word, old_pending = self.code[pc], pending
            pending, next_pc = None, pc + 4
            op, rs, rt, rd = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
            shift, function, immediate = (word >> 6) & 31, word & 63, word & 65535
            offset = signed(immediate, 16)
            r, m = self.r, self.memory
            address = (r[rs] + offset) & MASK
            if op == 0:
                if function == 0: r[rd] = (r[rt] << shift) & MASK
                elif function == 3: r[rd] = (signed(r[rt]) >> shift) & MASK
                elif function == 8: pending = r[rs]
                elif function == 33: r[rd] = (r[rs] + r[rt]) & MASK
                elif function == 35: r[rd] = (r[rs] - r[rt]) & MASK
                elif function == 37: r[rd] = r[rs] | r[rt]
                else: raise AssertionError(('unsupported function', hex(pc), function))
            elif op == 3:
                destination = ((pc + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
                r[31] = pc + 8
                pending = destination if destination in (GROUP_CLEAN, SLOT_CLEAN) else ('call', destination, pc + 8)
            elif op == 9: r[rt] = address
            elif op == 15: r[rt] = immediate << 16
            elif op in (4, 5, 20):
                take = (r[rs] != r[rt]) if op == 5 else (r[rs] == r[rt])
                self.branches.add((pc, take))
                if take: pending = pc + 4 + offset * 4
                elif op == 20: next_pc += 4
            elif op in (32, 33, 35):
                width = {32: 1, 33: 2, 35: 4}[op]
                r[rt] = signed(m.get(address, width), width * 8) & MASK
            elif op in (40, 43): m.put(address, r[rt], 1 if op == 40 else 4)
            else: raise AssertionError(('unsupported opcode', hex(pc), op))
            r[0] = 0
            assert old_pending is None or pending is None, 'control transfer in delay slot'
            if isinstance(old_pending, tuple):
                self.fixture.service(old_pending[1], r[4:7])
                # External ordinary-ABI calls may clobber all caller-saved registers.
                for index in list(range(1, 16)) + [24, 25, 31]:
                    r[index] = (0xBAD00000 + index * 197 + self.fixture.remove_count) & MASK
                pc = old_pending[2]
            else:
                pc = old_pending if old_pending is not None else next_pc
        raise AssertionError('instruction bound exceeded')
