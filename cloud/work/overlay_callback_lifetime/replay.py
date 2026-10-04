"""Bounded native replay, not a game/emulator or a residency proof.

Reads authenticated existing assets locally; never exports instructions. Only
three explicitly allowed bodies may execute. External calls are supplied stubs.
Unsupported instructions, uninitialized reads, or escaped execution fail closed.
"""
from pathlib import Path
import hashlib
import zlib

BASE = 0x80086A50
GAME_SHA256 = 'bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d'
RANGES = ((0x8009079C, 0x800908A0), (0x800B0618, 0x800B066C),
          (0x800B0868, 0x800B08DC))
HEAD, FREE, COUNT, PEAK = 0x801391F0, 0x801392C8, 0x8012E66C, 0x8012E678
STOP = 0x1000


def load_game(root):
    data = (Path(root) / 'assets/us/data.bin').read_bytes()
    stream = zlib.decompressobj(-15)
    game = stream.decompress(data[0xB0CB10 - 0x10000:])
    if not stream.eof or hashlib.sha256(game).hexdigest() != GAME_SHA256:
        raise ValueError('game image authentication failed')
    return game


def signed(value, bits=32):
    return (value & ((1 << (bits - 1)) - 1)) - (value & (1 << (bits - 1)))


class Replay:
    def __init__(self, game, hooks=None):
        self.game = game
        self.memory = {}
        self.r = [0] * 32
        self.r[29] = 0x90010000
        self.r[31] = STOP
        self.hooks = hooks or {}
        self.calls = []
        self.steps = 0

    def read(self, address, size):
        return int.from_bytes(bytes(self.memory[address+i] for i in range(size)), 'big')

    def write(self, address, value, size):
        for i, byte in enumerate((value & ((1 << (size*8))-1)).to_bytes(size, 'big')):
            self.memory[address+i] = byte

    def instruction(self, pc):
        if not any(lo <= pc < hi for lo, hi in RANGES) or pc % 4:
            raise ValueError('execution outside bounded bodies: %08x' % pc)
        return int.from_bytes(self.game[pc-BASE:pc-BASE+4], 'big')

    def execute(self, pc):
        self.steps += 1
        if self.steps > 10000:
            raise ValueError('step bound exceeded')
        w = self.instruction(pc)
        op, rs, rt, rd, fn = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, w & 63
        imm = signed(w & 65535, 16)
        target = None
        annul = False
        if w == 0:
            pass
        elif op == 0 and fn in (8, 9):
            target = self.r[rs]
            if fn == 9:
                self.r[rd] = pc + 8
        elif op == 0 and fn == 37:
            self.r[rd] = self.r[rs] | self.r[rt]
        elif op == 0 and fn == 42:
            self.r[rd] = int(signed(self.r[rs]) < signed(self.r[rt]))
        elif op == 3:
            target = ((pc + 4) & 0xF0000000) | ((w & 0x3FFFFFF) << 2)
            self.r[31] = pc + 8
        elif op in (4, 5, 20, 21):
            equal = self.r[rs] == self.r[rt]
            taken = equal if op in (4, 20) else not equal
            target = pc + 4 + imm * 4 if taken else pc + 8
            annul = op in (20, 21) and not taken
        elif op == 1 and rt == 0:
            target = pc + 4 + imm*4 if signed(self.r[rs]) < 0 else pc + 8
        elif op == 9:
            self.r[rt] = (self.r[rs] + imm) & 0xFFFFFFFF
        elif op == 15:
            self.r[rt] = (w & 65535) << 16
        elif op in (33, 35):
            size = 2 if op == 33 else 4
            value = self.read((self.r[rs] + imm) & 0xFFFFFFFF, size)
            self.r[rt] = (signed(value, 16) if size == 2 else value) & 0xFFFFFFFF
        elif op in (41, 43):
            self.write((self.r[rs] + imm) & 0xFFFFFFFF, self.r[rt], 2 if op == 41 else 4)
        else:
            raise ValueError('unsupported instruction at %08x' % pc)
        self.r[0] = 0
        return target, annul

    def run(self, entry):
        pc = entry
        while pc != STOP:
            target, annul = self.execute(pc)
            if target is None:
                pc += 4
                continue
            if not annul:
                nested, _ = self.execute(pc + 4)
                if nested is not None:
                    raise ValueError('control transfer in delay slot')
            if target in self.hooks:
                self.calls.append((target, self.r[4], self.r[5], self.r[6]))
                self.hooks[target](self)
                target = self.r[31]
            pc = target
        return self
