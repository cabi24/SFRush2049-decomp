#!/usr/bin/env python3
"""Recompile and independently verify the complete 80087110 rectangle body.

Only source, scripts and scalar/hash evidence are publishable. Original bytes,
linked objects and diagnostic artifacts stay under ignored build/. This does
not change locks, splice a body, or establish image/compression/cartridge proof.
"""
from pathlib import Path
import argparse
import contextlib
import ctypes
import hashlib
import importlib.util
import io
import itertools
import json
import random
import re
import shlex
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
spec = importlib.util.spec_from_file_location('rect_native', Path(__file__).with_name('native_machine.py'))
machine = importlib.util.module_from_spec(spec)
spec.loader.exec_module(machine)
NAME = 'func_80087110'
SIZE = 1780
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
DEFAULT = ROOT / 'cloud/work/frontier/w4a/func_80087110/best.c'


def sha(data):
    return hashlib.sha256(data).hexdigest()


def run(args):
    result = subprocess.run(list(map(str, args)), text=True, capture_output=True)
    if result.returncode:
        raise RuntimeError('command failed: ' + shlex.join(result.args) + '\n' + result.stdout + result.stderr)
    return result.stdout


def extent(obj):
    entries = [line.split() for line in run(['mips-linux-gnu-nm', '-S', obj]).splitlines()]
    matches = [entry for entry in entries if len(entry) == 4 and entry[3] == NAME]
    assert len(matches) == 1 and matches[0][2] == 'T', 'invalid ELF function symbol'
    return int(matches[0][0], 16), int(matches[0][1], 16)


def independent_target():
    directory = ROOT / 'asm/us/blob'
    manifest = dict((filename, digest) for digest, filename in
                    (line.split('  ') for line in (directory / 'SHA256SUMS').read_text().splitlines()))
    sections, words = set(), None
    for filename, digest in manifest.items():
        data = (directory / filename).read_bytes()
        assert sha(data) == digest, 'protected target integrity failure'
        if not filename.endswith('.s'):
            continue
        current = None
        for line in data.decode().splitlines():
            match = re.match(r'\.section \.text\.(\S+?),', line)
            if match:
                current = match[1]
                assert current not in sections, 'duplicate native section'
                sections.add(current)
                if current == NAME:
                    words = []
            elif line.startswith('.section'):
                current = None
            match = re.match(r'\s*\.word\s+(0x[\da-fA-F]+)', line)
            if match and current == NAME:
                words.append(int(match[1], 16))
    assert words is not None and len(words) * 4 == SIZE, 'wrong native extent'
    assert words == score.targets()[NAME], 'native parser disagreement'
    return words, manifest


def exact_match(size, linked, target):
    return size == SIZE == len(target) * 4 and linked == target


def check_linked_placement(address, size, expected_address, expected_size):
    assert (address, size) == (expected_address, expected_size), 'linked placement/extent changed'


def inspect(obj, build):
    target, manifest = independent_target()
    symbols = score.image_symbols()
    offset, size = extent(obj)
    definitions = [line.split()[-1] for line in run(['mips-linux-gnu-nm', '--defined-only', obj]).splitlines() if ' T ' in line or ' t ' in line]
    assert definitions == [NAME] and offset == 0, 'standalone proof requires exactly one text function'
    assert size > 0 and size % 4 == 0, 'invalid ELF extent'
    undefined = [line.split()[-1] for line in run(['mips-linux-gnu-nm', '-u', obj]).splitlines()]
    assert set(undefined) == set(machine.GLOBALS + ['D_80149438']), 'unexpected external dependency'
    # No own literal section is needed for this integer-only leaf. Fail closed if
    # a candidate introduces one instead of claiming address-only verification.
    sections = run(['mips-linux-gnu-readelf', '-SW', obj])
    assert not re.search(r'\s\.(?:data|rodata|sdata|sbss|bss)\s', sections), 'new own-data section requires separate proof'
    relocations = run(['mips-linux-gnu-readelf', '-rW', obj])
    relocation_types = re.findall(r'\bR_MIPS_\w+', relocations)
    assert set(relocation_types) <= {'R_MIPS_HI16', 'R_MIPS_LO16'}, 'unexpected relocation type'
    base = symbols[NAME] - offset
    linker = 'OUTPUT_ARCH(mips)\nENTRY(' + NAME + ')\nSECTIONS { .text ' + hex(base)
    linker += ' : SUBALIGN(4) { *(.text) } }\n'
    linker += '\n'.join(name + ' = ' + hex(symbols[name]) + ';' for name in undefined)
    (build / 'link.ld').write_text(linker)
    linked_obj = build / 'linked.elf'
    run(['mips-linux-gnu-ld', '-T', build / 'link.ld', '-o', linked_obj, obj])
    run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', linked_obj, build / 'linked.bin'])
    address, linked_size = extent(linked_obj)
    check_linked_placement(address, linked_size, symbols[NAME], size)
    raw = (build / 'linked.bin').read_bytes()
    padding = raw[offset + size:]
    assert len(padding) < 16 and not any(padding), 'non-padding bytes after function extent'
    body = raw[offset:offset + size]
    assert len(body) == size, 'truncated linked body'
    linked = list(struct.unpack('>' + str(size // 4) + 'I', body))
    all_words = score.text_words(obj)
    relocated, masks, unresolved, unverified, errors = score.relocate(obj, all_words, offset, offset + size, symbols)
    assert not (masks or unresolved or unverified or errors), 'incomplete relocation proof'
    assert relocated[offset // 4:(offset + size) // 4] == linked, 'GNU/project relocator disagreement'
    with contextlib.redirect_stdout(io.StringIO()):
        comparison = score.compare(obj, NAME, show=0)
    residual = [hex(i * 4) for i in range(max(len(linked), len(target)))
                if i >= len(linked) or i >= len(target) or linked[i] != target[i]]
    return {'target_bytes': SIZE, 'elf_function_bytes': size, 'linked_address': hex(address),
            'linked_text_bytes': len(raw), 'alignment_padding_bytes': len(raw) - size,
            'alignment_padding_all_zero': True, 'exact_extent': size == SIZE, 'byte_equal': linked == target,
            'accepted_exact_match': exact_match(size, linked, target),
            'project_scorer_differing_words': comparison.differing,
            'project_scorer_target_words': comparison.total,
            'project_scorer_extra_words': comparison.extra_words,
            'project_scorer_accepted': comparison.accepted(), 'residual_offsets': residual,
            'gnu_linker_equals_project_relocator': True, 'unresolved': unresolved,
            'unverified': unverified, 'relocation_errors': errors,
            'undefined_symbols': sorted(undefined), 'own_literal_or_data_sections': [],
            'relocation_counts': {name: relocation_types.count(name) for name in sorted(set(relocation_types))},
            'protected_manifest_sha256': sha((ROOT / 'asm/us/blob/SHA256SUMS').read_bytes()),
            'protected_manifest_entries_verified': len(manifest),
            'target_sha256': sha(struct.pack('>' + str(len(target)) + 'I', *target)),
            'linked_body_sha256': sha(body)}, target, linked, symbols


def oracle(arguments, state):
    """Independent scalar/packet contract, explicitly modulo-32-bit arithmetic."""
    signed = machine.signed
    x, y, right, bottom, s, t = map(signed, arguments)
    flags, left, clip_right, top, clip_bottom, mode = map(signed, state)
    if x < left:
        if not flags & 4:
            s = signed(s + signed(left - x))
        x = left
    if y < top:
        if not flags & 8:
            t = signed(t + signed(top - y))
        y = top
    if right > clip_right:
        if flags & 4:
            s = signed(signed(s - clip_right) + right)
        right = clip_right
    if bottom > clip_bottom:
        if flags & 8:
            t = signed(signed(t - clip_bottom) + bottom)
        bottom = clip_bottom
    if right < x or bottom < y:
        return []
    height = signed(bottom - y)
    if flags & 4:
        s = signed(signed(s + right) - x)
    if flags & 8:
        t = signed(t + height)
    dx = 4096 if mode == 0 else 1024
    dy = 512 if mode != 0 and flags & 0x8000 else 1024
    if mode != 0:
        if flags & 0x8000:
            bottom = signed(bottom + signed(height + 1))
        right, bottom = signed(right + 1), signed(bottom + 1)
    s_fixed, t_fixed = (s << 5) & 0xffff, (t << 5) & 0xffff
    if mode != 0 and flags & 0x8000 and flags & 8:
        t_fixed = (t_fixed + 16) & 0xffff
    if flags & 4:
        dx = -dx
    if flags & 8:
        dy = -dy
    return [0xe4000000 | ((right << 2) & 0xfff) << 12 | ((bottom << 2) & 0xfff),
            ((x << 2) & 0xfff) << 12 | ((y << 2) & 0xfff),
            0xe1000000, (s_fixed << 16) | t_fixed,
            0xf1000000, ((dx & 0xffff) << 16) | (dy & 0xffff)]


def host_candidate(source, build):
    # The complete source remains unchanged. Define only its external objects
    # and append a wrapper. UBSan rejects arithmetic outside the host-C domain.
    content = source.read_text() + '''
Gfx *D_80149438;
int D_8012E608,D_8012E60C,D_8012E610,D_8012E668,D_8012E674,D_8014A248;
void rect_host(const int *a, const int *g, unsigned *out) {
    Gfx buffer[4];
    unsigned i;
    for(i=0;i<4;i++) {
        buffer[i].words.w0=0xa55a0000U+i*2;
        buffer[i].words.w1=0xa55a0001U+i*2;
    }
    D_80149438=buffer;
    D_8012E608=g[0]; D_8012E60C=g[1]; D_8012E610=g[2];
    D_8012E668=g[3]; D_8012E674=g[4]; D_8014A248=g[5];
    func_80087110(a[0],a[1],a[2],a[3],a[4],a[5]);
    out[0]=(unsigned)(D_80149438-buffer)*8;
    for(i=0;i<4;i++) {
        out[1+i*2]=buffer[i].words.w0;
        out[2+i*2]=buffer[i].words.w1;
    }
}
'''
    host = build / 'host.c'
    host.write_text(content)
    run(['cc', '-std=c99', '-O2', '-fPIC', '-shared', '-fsanitize=undefined',
         '-fno-sanitize-recover=undefined', host, '-o', build / 'host.so'])
    library = ctypes.CDLL(str(build / 'host.so'))
    library.rect_host.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_uint)]
    def call(arguments, state):
        out = (ctypes.c_uint * 9)()
        library.rect_host((ctypes.c_int * 6)(*arguments), (ctypes.c_int * 6)(*state), out)
        assert out[0] in (0, 24), 'host pointer advance'
        assert list(out[7:]) == [0xa55a0006, 0xa55a0007], 'host command-buffer guard'
        if not out[0]:
            assert list(out[1:]) == [0xa55a0000 + i for i in range(8)], 'host reject touched output'
        return list(out[1:1 + out[0] // 4]), out[0]
    return call


def sanitizer_controls(build):
    """Verify the host controls reject known undefined C99 arithmetic cases.

    Run in child processes so an expected UBSan exit cannot terminate replay.
    C99 mode is deliberate: GCC's shift checks differ in C89 mode.
    """
    controls = [
        ('negative_left_shift', [-1, 0, 1, 1, 0, 0], [0, -10, 100, -10, 100, 0],
         'left shift of negative value'),
        ('signed_overflow', [-2147483648, 0, 1, 1, 0, 0], [0, 0, 100, 0, 100, 0],
         'signed integer overflow')]
    results = {}
    for name, arguments, state, diagnostic in controls:
        program = "import ctypes\nlib=ctypes.CDLL(" + repr(str(build / 'host.so')) + ")\n"
        program += "a=(ctypes.c_int*6)(*" + repr(arguments) + ")\n"
        program += "g=(ctypes.c_int*6)(*" + repr(state) + ")\n"
        program += "o=(ctypes.c_uint*9)()\nlib.rect_host(a,g,o)\n"
        result = subprocess.run([sys.executable, '-c', program], capture_output=True, text=True)
        assert result.returncode != 0 and diagnostic in result.stderr, 'host sanitizer control did not fail'
        results[name] = {'expected_failure_observed': True, 'exit_code': result.returncode}
    return results


def cases():
    rectangles = [(-1, 10, 20, 30), (10, -1, 20, 30), (10, 20, 321, 40),
                  (10, 20, 30, 241), (-10, -20, 350, 260), (10, 20, 30, 40),
                  (10, 20, 10, 20), (-10, 20, -1, 30), (10, -20, 20, -1),
                  (320, 20, 340, 30), (10, 240, 20, 260), (30, 10, 20, 40),
                  (10, 30, 20, 20), (0, 0, 319, 239), (319, 239, 320, 240),
                  (-1, -1, 0, 0), (0, 0, 0, 0)]
    for mode, flip, stretch, irrelevant in itertools.product([0, 1, -7], [0, 4, 8, 12], [0, 0x8000], [0, 0x200003]):
        for rect in rectangles:
            yield 'clip_flip_stretch', (*rect, 9, 17), (flip | stretch | irrelevant, 0, 319, 0, 239, mode), True
    for arguments in [(65536, 65536, 65537, 65537, 65536, 65536),
                      (-65536, -65536, 70000, 70000, 0, 0),
                      (0, 0, 1024, 1024, 2048, 2048)]:
        for flip in [0, 4, 8, 12]:
            yield 'word_arguments_and_packet_masks', arguments, (flip, 0, 100000, 0, 100000, 1), True
    rng = random.Random(0x80087110)
    for _ in range(3000):
        left, top = rng.randrange(1024), rng.randrange(1024)
        state = (rng.choice([0, 4, 8, 12, 0x8000, 0x8004, 0x8008, 0x800c]),
                 left, left + rng.randrange(2048), top, top + rng.randrange(2048), rng.choice([0, 1, -3]))
        arguments = (*[rng.randrange(-1024, 4096) for _ in range(4)], rng.randrange(4096), rng.randrange(4096))
        yield 'defined_C_random', arguments, state, True
    for _ in range(3000):
        arguments = [rng.getrandbits(32) for _ in range(6)]
        state = [rng.getrandbits(32) for _ in range(6)]
        state[0] = rng.choice([0, 4, 8, 12, 0x8000, 0x8004, 0x8008, 0x800c])
        state[5] = rng.choice([0, 1, 0xffffffff])
        yield 'native_modular_stress', arguments, state, False


def semantics(source, build, native, linked, symbols):
    host = host_candidate(source, build)
    tally, paths, native_offsets, linked_offsets = {}, {}, set(), set()
    host_count, reject_count, emit_count = 0, 0, 0
    for category, arguments, state, check_host in cases():
        expected = oracle(arguments, state)
        nwords, nadvance, nevents, nv = machine.execute(native, symbols[NAME], symbols, arguments, state)
        cwords, cadvance, cevents, cv = machine.execute(linked, symbols[NAME], symbols, arguments, state)
        assert nwords == cwords == expected, ('packet mismatch', category, arguments, state)
        assert nadvance == cadvance == 4 * len(expected), 'pointer mismatch'
        # The only residual is pure scheduling. External accesses retain order.
        assert nevents == cevents, 'external access ordering differs'
        native_offsets |= nv
        linked_offsets |= cv
        if check_host:
            hwords, hadvance = host(arguments, state)
            assert hwords == expected and hadvance == nadvance, ('host mismatch', category, arguments, state)
            host_count += 1
        if expected:
            emit_count += 1
            key = ('mode0' if state[5] == 0 else 'mode_nonzero') + '_flip' + str(state[0] & 12) + '_stretch' + str(bool(state[0] & 0x8000))
            paths[key] = paths.get(key, 0) + 1
        else:
            reject_count += 1
        tally[category] = tally.get(category, 0) + 1
    assert len(paths) == 16, 'not all flip/stretch/mode combinations reached'
    return {'result': 'PASS', 'cases': sum(tally.values()), 'categories': tally,
            'host_C_UBSan_cases': host_count, 'host_dialect': 'C99 for strict signed-shift UBSan coverage',
            'sanitizer_negative_controls': sanitizer_controls(build), 'emitting_cases': emit_count, 'rejected_cases': reject_count,
            'output_paths': paths, 'native_instruction_offsets_executed': len(native_offsets),
            'candidate_instruction_offsets_executed': len(linked_offsets),
            'native_unexecuted_offsets': [hex(i) for i in range(0, len(native) * 4, 4) if i not in native_offsets],
            'models': ['complete protected native stream', 'complete GNU-linked IDO stream',
                       'unchanged C source compiled with host UBSan on defined-domain cases',
                       'independent modulo-32-bit clipping and packet oracle'],
            'memory_bounds_callee_saves_and_external_order': 'PASS',
            'limitations': ['instruction-model replay, not N64 hardware or graphical validation',
                            'disjoint valid globals and aligned command storage, no racing mutation',
                            'full-word stress tests validate native modulo arithmetic only; signed C overflow and negative left shifts are excluded from the host domain',
                            'parameterized clip/mode tests do not establish all values occur in gameplay']}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--source', type=Path, default=DEFAULT)
    parser.add_argument('--build', type=Path, default=ROOT / 'build/87110_verification')
    args = parser.parse_args()
    source, build = args.source.resolve(), args.build.resolve()
    build.mkdir(parents=True, exist_ok=True)
    obj = build / 'candidate.o'
    score.compile_single(source, FLAGS, obj)
    proof, native, linked, symbols = inspect(obj, build)
    proof.update({'function': NAME, 'source_sha256': sha(source.read_bytes()), 'flags': FLAGS,
                  'object_sha256': sha(obj.read_bytes()),
                  'gnu_linker': run(['mips-linux-gnu-ld', '--version']).splitlines()[0],
                  'host_compiler': run(['cc', '--version']).splitlines()[0],
                  'claims': [], 'status': 'VERIFIED_BYTES_ONLY' if proof['accepted_exact_match'] else 'RESEARCH_ONLY',
                  'compiler_sha256': {name: sha((score.IDO / name).read_bytes()) for name in ['cc', 'cfe', 'uopt', 'ugen', 'as1']}})
    proof['semantics'] = semantics(source, build, native, linked, symbols)
    (build / 'verification.json').write_text(json.dumps(proof, indent=2) + '\n')
    print(json.dumps(proof, indent=2))


if __name__ == '__main__':
    main()
