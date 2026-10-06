"""Bounded bit-exact native, GNU-linked, arithmetic and unchanged-host proof."""
import hashlib
from pathlib import Path
import random
import struct
import subprocess

MASK = 0xffffffff
SEED = 0x8011735c
HERE = Path(__file__).resolve().parent

def signed(x):
    return x if x < 0x80000000 else x - 0x100000000

def bits(x):
    return struct.unpack('<I', struct.pack('<f', x))[0]

def real(x):
    return struct.unpack('<f', struct.pack('<I', x))[0]

def rounded(x):
    return real(bits(x))

def oracle(seed, scale):
    next_seed = (1103515245 * seed + 12345) % (1 << 32)
    sample = (next_seed // (1 << 16)) % (1 << 15)
    return next_seed, bits(rounded(float(sample) * real(scale)) / 32768.0)

class Native:
    def __init__(self, words):
        self.words = words
        self.coverage = set()

    def run(self, seed, scale):
        r = [(0x53ab0000 + i * 257) & MASK for i in range(32)]
        r[0] = 0
        f = [0x3f000000 + i for i in range(32)]
        f[12] = scale
        original_r = list(r)
        original_f = list(f)
        writes = []
        lo = None
        returned = False
        for pc, w in enumerate(self.words):
            assert not returned or pc == len(self.words) - 1
            self.coverage.add(pc * 4)
            op, rs, rt, rd, sh, fn = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31, (w >> 6) & 31, w & 63
            imm = w & 65535
            simm = imm if imm < 32768 else imm - 65536
            if op == 15:
                r[rt] = imm << 16
            elif op == 9:
                r[rt] = (r[rs] + simm) & MASK
            elif op == 13:
                r[rt] = r[rs] | imm
            elif op == 12:
                r[rt] = r[rs] & imm
            elif op == 35:
                assert ((r[rs] + simm) & MASK) == SEED
                r[rt] = seed
            elif op == 43:
                assert ((r[rs] + simm) & MASK) == SEED
                seed = r[rt]
                writes.append(SEED)
            elif op == 0 and fn == 25:
                lo = (r[rs] * r[rt]) & MASK
            elif op == 0 and fn == 18:
                assert lo is not None
                r[rd] = lo
            elif op == 0 and fn == 3:
                r[rd] = (signed(r[rt]) >> sh) & MASK
            elif op == 0 and fn == 8:
                assert rs == 31 and pc == len(self.words) - 2
                returned = True
            elif op == 17 and rs == 4:
                f[rd] = r[rt]
            elif op == 17 and rs == 20 and fn == 32:
                f[sh] = bits(float(signed(f[rd])))
            elif op == 17 and rs == 16 and fn == 2:
                f[sh] = bits(real(f[rd]) * real(f[rt]))
            elif op == 17 and rs == 16 and fn == 3:
                f[sh] = bits(real(f[rd]) / real(f[rt]))
            else:
                raise AssertionError('unsupported native instruction at offset %d' % (pc * 4))
            r[0] = 0
        assert returned and writes == [SEED]
        assert all(r[i] == original_r[i] for i in list(range(16, 24)) + [28, 29, 30, 31])
        assert f[20:] == original_f[20:]
        return seed, f[0]

def cases():
    inverse = pow(1103515245, -1, 1 << 32)
    # Every 15-bit rand output, with both signs of the full updated seed.
    for sample in range(32768):
        next_seed = (sample << 16) | ((sample * 4051) & 65535) | ((sample & 1) << 31)
        seed = ((next_seed - 12345) * inverse) & MASK
        for scale in (0.0, 1.0, -1.0, 1000.0):
            yield seed, bits(scale)
    generator = random.Random(0x8b2e4)
    for i in range(4096):
        exponent = generator.randrange(67, 188)
        scale = (generator.randrange(2) << 31) | (exponent << 23) | generator.randrange(1 << 23)
        yield generator.randrange(1 << 32), scale
    for seed in (0, 1, 0xffffffff, 0x80000000, 0x7fffffff, 12345):
        for scale in (0.0, -0.0, 32768.0, -32768.0, 1 / 32768.0, -1 / 32768.0):
            yield seed, bits(scale)

def host(directory, inputs, range_source):
    exe = directory / ('host-' + hashlib.sha256(range_source.read_bytes()).hexdigest()[:12])
    command = ['cc', '-std=c89', '-O2', '-fwrapv', '-ffp-contract=off', '-fsanitize=undefined',
               '-fno-sanitize-recover=all', str(HERE/'rand.c'), str(range_source),
               str(HERE/'semantic_test.c'), '-o', str(exe)]
    subprocess.run(command, check=True, capture_output=True)
    result = subprocess.run([str(exe)], input=inputs, check=True, capture_output=True)
    assert result.stderr == b''
    return result.stdout

def verify(directory, native_words, linked_words):
    inputs = list(cases())
    payload = b''.join(struct.pack('=II', *case) for case in inputs)
    expected = []
    native, linked = Native(native_words), Native(linked_words)
    for case in inputs:
        value = oracle(*case)
        assert native.run(*case) == value
        assert linked.run(*case) == value
        expected.append(value)
    expected_bytes = b''.join(struct.pack('=II', *value) for value in expected)
    actual = host(directory, payload, HERE/'range.c')
    assert actual == expected_bytes
    # Compiled wrong-contract controls must disagree with actual test cases.
    source = (HERE/'range.c').read_text()
    mutations = {
        'wrong_mask': source.replace('& 0x07FFF', '& 0x03FFF'),
        'wrong_denominator': source.replace('32768.0f', '32767.0f'),
    }
    mutations['ignore_scale_sign'] = source.replace('* max)', '* (max < 0.0f ? -max : max))')
    rejected = {}
    for name, text in mutations.items():
        path = directory/(name+'.c'); path.write_text(text)
        output = host(directory, payload, path)
        assert len(output) == len(expected_bytes)
        bad = next(i for i in range(len(inputs)) if output[i*8:i*8+8] != expected_bytes[i*8:i*8+8])
        rejected[name] = {'first_case': bad, 'seed': inputs[bad][0], 'scale_bits': inputs[bad][1]}
    assert native.coverage == linked.coverage == set(range(0, 72, 4))
    return {'cases': len(inputs), 'native_executions': 2*len(inputs),
            'all_32768_random_outputs_covered': True, 'instruction_offsets_executed': len(native.coverage),
            'host_ubsan': True, 'host_wrap_semantics': '-fwrapv', 'linked_replay': True,
            'output_sha256': hashlib.sha256(expected_bytes).hexdigest(),
            'mutation_discriminators': rejected,
            'domain': 'finite normal scales/products plus signed zero; default rounding; excludes NaN, infinity, subnormal arithmetic and overflow'}
