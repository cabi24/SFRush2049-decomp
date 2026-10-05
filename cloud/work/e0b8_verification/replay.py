#!/usr/bin/env python3
"""Independent exact-extent, linked-byte and semantic E0B8 research replay.

Usage: python3 cloud/work/e0b8_verification/replay.py [--group GROUP_DIR | --source C_FILE]
The source is compiled freshly with stock IDO. GNU ld and the project relocator
must agree on every body word. Match acceptance additionally requires the exact
ELF function extent; semantic agreement is never a matching claim. Nothing is
spliced, locked, or changed in protected manifests. Only source hashes, offsets,
counts and scalar outcomes are emitted; raw target data remains untracked.
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
import math
import random
import re
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score

spec = importlib.util.spec_from_file_location('e0b8_machine', Path(__file__).with_name('native_machine.py'))
machine = importlib.util.module_from_spec(spec)
spec.loader.exec_module(machine)
NAME = 'func_8008E0B8'
SOURCE = ROOT / 'cloud/work/near_miss_B28/func_8008E0B8_vector.c'
GROUP = ROOT / 'cloud/work/ipa-groups/dot_e0b8_vector_normalize'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
TARGET_SIZE = 140


def sha(data):
    return hashlib.sha256(data).hexdigest()


def run(args):
    result = subprocess.run(list(map(str, args)), check=True, capture_output=True, text=True)
    return result.stdout


def elf_extent(obj, name=NAME):
    """Use GNU nm, independent of the project ELF implementation."""
    records = [line.split() for line in run(['mips-linux-gnu-nm', '-S', obj]).splitlines()]
    entries = [record for record in records if len(record) == 4 and record[3] == name]
    assert len(entries) == 1, 'missing or duplicate ELF function'
    assert entries[0][2].lower() == 't', 'function is not text'
    return int(entries[0][0], 16), int(entries[0][1], 16)


def exact_match(size, linked, target):
    """Fail closed even for all-zero ELF padding or a truncated prefix match."""
    return size == len(target) * 4 == TARGET_SIZE and linked == target


def independent_target(name=NAME, expected_size=TARGET_SIZE):
    directory = ROOT / 'asm/us/blob'
    manifest = dict((filename, digest) for digest, filename in
                    (line.split('  ') for line in (directory / 'SHA256SUMS').read_text().splitlines()))
    sections = set()
    target = None
    for filename, digest in manifest.items():
        content = (directory / filename).read_bytes()
        assert sha(content) == digest, 'protected target integrity failure'
        if not filename.endswith('.s'):
            continue
        current = None
        for line in content.decode().splitlines():
            match = re.match(r'\.section \.text\.(\S+?),', line)
            if match:
                current = match[1]
                assert current not in sections, 'duplicate target section'
                sections.add(current)
                if current == name:
                    target = []
            elif line.startswith('.section'):
                current = None
            match = re.match(r'\s*\.word\s+(0x[\da-fA-F]+)', line)
            if match and current == name:
                target.append(int(match[1], 16))
    assert target is not None and len(target) * 4 == expected_size, 'unexpected native extent'
    assert target == score.targets()[name], 'independent target parser disagrees'
    return target, manifest


def inspect_object(obj, build):
    target, manifest = independent_target()
    table = score.image_symbols()
    start, size = elf_extent(obj)
    assert size and size % 4 == 0, 'invalid full ELF extent'
    # Link the entire object so section or relocation handling cannot be hidden
    # by slicing first. Only named, verified retail symbols are admitted here.
    undefined = [line.split()[-1] for line in run(['mips-linux-gnu-nm', '-u', obj]).splitlines()]
    assert all(name in table for name in undefined), 'unresolved external symbol'
    base = table[NAME] - start
    script = 'OUTPUT_ARCH(mips)\nENTRY(' + NAME + ')\nSECTIONS { . = ' + hex(base)
    script += '; .text : SUBALIGN(4) { *(.text) } }\n'
    script += '\n'.join(name + ' = ' + hex(table[name]) + ';' for name in undefined)
    (build / 'link.ld').write_text(script)
    linked_object = build / 'linked.elf'
    run(['mips-linux-gnu-ld', '-T', build / 'link.ld', '-o', linked_object, obj])
    run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', linked_object, build / 'linked.bin'])
    linked_address, linked_size = elf_extent(linked_object)
    assert linked_address == table[NAME] and linked_size == size, 'link changed function extent'
    raw = (build / 'linked.bin').read_bytes()
    body = raw[start:start + size]
    assert len(body) == size, 'truncated linked function'
    linked = list(struct.unpack('>' + str(size // 4) + 'I', body))
    words = score.text_words(obj)
    relocated, masks, unresolved, unverified, errors = score.relocate(obj, words, start, start + size, table)
    assert not (masks or unresolved or unverified or errors), 'relocation proof incomplete'
    assert relocated[start // 4:(start + size) // 4] == linked, 'GNU linker and project relocation disagree'
    with contextlib.redirect_stdout(io.StringIO()):
        result = score.compare(obj, NAME, show=0)
    offsets = [hex(i * 4) for i in range(max(len(target), len(linked)))
               if i >= len(target) or i >= len(linked) or target[i] != linked[i]]
    proof = {'target_bytes': TARGET_SIZE, 'elf_function_bytes': size,
             'linked_text_bytes': len(raw), 'byte_equal': linked == target,
             'exact_extent': size == TARGET_SIZE,
             'accepted_exact_match': exact_match(size, linked, target),
             'project_scorer_differing_words': result.differing,
             'project_scorer_target_words': result.total,
             'project_scorer_accepted': result.accepted(),
             'residual_offsets': offsets,
             'gnu_linker_equals_project_relocator': True,
             'unresolved': unresolved, 'unverified': unverified, 'relocation_errors': errors,
             'protected_manifest_sha256': sha((ROOT / 'asm/us/blob/SHA256SUMS').read_bytes()),
             'protected_manifest_entries_verified': len(manifest),
             'target_sha256': sha(struct.pack('>35I', *target)),
             'linked_body_sha256': sha(body)}
    return proof, target, linked, table


def inspect_context(obj, build, names, table):
    """Check every real group context member over its complete ELF extent."""
    raw = (build / 'linked.bin').read_bytes()
    words = score.text_words(obj)
    target_start, _ = elf_extent(obj)
    linked_base = table[NAME] - target_start
    proofs = {}
    for name in names:
        if name == NAME:
            continue
        target, _ = independent_target(name, len(score.targets()[name]) * 4)
        start, size = elf_extent(obj, name)
        linked_address, linked_size = elf_extent(build / 'linked.elf', name)
        assert linked_address == table[name] and linked_size == size, 'context linked address/extent changed'
        offset = linked_address - linked_base
        body = raw[offset:offset + size]
        assert len(body) == size, 'context linked body truncated'
        linked = list(struct.unpack('>' + str(size // 4) + 'I', body))
        relocated, masks, unresolved, unverified, errors = score.relocate(obj, words, start, start + size, table)
        assert not (masks or unresolved or unverified or errors), 'context relocation incomplete'
        assert relocated[start // 4:(start + size) // 4] == linked, 'context GNU/project relocation mismatch'
        with contextlib.redirect_stdout(io.StringIO()):
            comparison = score.compare(obj, name, show=0)
        assert size == len(target) * 4 and linked == target and comparison.accepted(), 'context exact-match regression'
        proofs[name] = {'elf_function_bytes': size, 'target_bytes': len(target) * 4,
                        'linked_address': hex(linked_address), 'exact_extent': True,
                        'byte_equal': True, 'gnu_linker_equals_project_relocator': True,
                        'project_scorer_differing_words': comparison.differing,
                        'project_scorer_target_words': comparison.total,
                        'linked_body_sha256': sha(body)}
    return proofs


def host_candidate(source, build):
    """Preserve the function body; bind the external threshold to a valid object.

    The scalar extern declaration alone is replaced with a pointer binding so
    threshold/vector overlap can be tested without inventing an out-of-bounds
    array at a host scalar global. No expression or operation in the body changes.
    """
    text = source.read_text()
    text, replacements = re.subn(r'extern\s+f32\s+D_8012394C\s*;',
                                'static float *e0b8_threshold;\n#define D_8012394C (*e0b8_threshold)', text)
    assert replacements == 1, 'host harness requires one scalar threshold declaration'
    text += '''
#include <string.h>
void e0b8_host_case(const unsigned *input, int alias, unsigned *output) {
    float vector[3];
    float threshold;
    float result;
    memcpy(vector, input, sizeof(vector));
    memcpy(&threshold, input + 3, sizeof(threshold));
    e0b8_threshold = alias >= 0 ? vector + alias : &threshold;
    result = func_8008E0B8(vector);
    memcpy(output, &result, sizeof(result));
    memcpy(output + 1, vector, sizeof(vector));
}
'''
    file = build / 'host_candidate.c'
    file.write_text(text)
    run(['cc', '-std=c89', '-O2', '-fno-fast-math', '-ffp-contract=off',
         '-fexcess-precision=standard', '-Wno-unknown-pragmas', '-fPIC', '-shared',
         file, '-lm', '-o', build / 'host.so'])
    library = ctypes.CDLL(str(build / 'host.so'))
    word = ctypes.c_uint32
    library.e0b8_host_case.argtypes = [ctypes.POINTER(word), ctypes.c_int, ctypes.POINTER(word)]
    library.e0b8_host_case.restype = None
    def call(xyz, threshold, alias):
        out = (word * 4)()
        library.e0b8_host_case((word * 4)(*xyz, threshold), alias, out)
        return list(out)
    return call


def oracle(xyz, threshold):
    """Source-level binary32 contract, separate from instruction decoding."""
    b, f = machine.bits, machine.value
    x, y, z = map(f, xyz)
    xx, yy, zz = b(x * x), b(y * y), b(z * z)
    length = b(math.sqrt(f(b(f(b(f(xx) + f(yy))) + f(zz)))))
    if f(length) <= f(threshold):
        return [0] + list(xyz), False
    inverse = f(b(machine.divide(1.0, f(length))))
    return [length, b(x * inverse), b(y * inverse), b(z * inverse)], True


def cases():
    b = machine.bits
    # Boundary outcomes, exact-length equality, and signed-zero preservation.
    for xyz in [(0, 0, 0), (0x80000000, 0, 0x80000000),
                tuple(map(b, (3, 4, 0))), tuple(map(b, (-3, 0, 4))),
                (b(1), 0, 0), (b(1e-5), 0, 0), (1, 0x80000001, 0),
                (b(1e19), b(-1e19), b(1e19)), (0x7f7fffff, 0, 0)]:
        length = oracle(xyz, b(-1))[0][0]
        boundaries = [0, 0x80000000, b(-1), b(1e-5), b(1), b(5),
                      0x7f800000, 0xff800000, 0x7fc12345, length]
        if 0 < length < 0x7f800000:
            boundaries.extend([length - 1, length + 1])
        for threshold in boundaries:
            yield 'boundary', list(xyz), threshold, -1
    specials = [0, 0x80000000, 1, 0x80000001, 0x007fffff, 0x00800000,
                b(1), b(-1), b(1e-5), b(-1e-5), 0x7f7fffff,
                0xff7fffff, 0x7f800000, 0xff800000, 0x7fc12345]
    for xyz in itertools.product(specials, repeat=3):
        yield 'edge_product', list(xyz), b(1e-5), -1
    for xyz in [(b(3), b(4), b(0)), (b(1), b(0), b(0)),
                (b(-1), b(0), 0x80000000), (0x7fc12345, b(2), b(3))]:
        for alias in range(3):
            yield 'threshold_alias', list(xyz), b(99), alias
    rng = random.Random(0x8008e0b8)
    for index in range(4000):
        if index < 2000:
            xyz = [b(rng.uniform(-1e6, 1e6)) for _ in range(3)]
            threshold = b(rng.choice([0, 1e-5, rng.uniform(-1e6, 2e6)]))
        else:
            # Quiet any NaN test input; signaling-NaN/FCSR behavior is excluded.
            xyz = [rng.getrandbits(32) for _ in range(3)]
            xyz = [word | 0x400000 if (word & 0x7fffffff) > 0x7f800000 else word for word in xyz]
            threshold = rng.choice([b(1e-5), b(0), b(-1), b(1e30), 0x7fc12345])
        yield 'randomized', xyz, threshold, -1


def verify_semantics(source, build, native, linked, table):
    host = host_candidate(source, build)
    tally, early, normalize = {}, 0, 0
    threshold_address = table['D_8012394C']
    canonical = lambda words: list(map(machine.canonical, words))
    for category, xyz, threshold, alias in cases():
        vector_address = 0x10000 if alias < 0 else threshold_address - alias * 4
        actual_threshold = threshold if alias < 0 else xyz[alias]
        expected, writes_expected = oracle(xyz, actual_threshold)
        native_ret, native_xyz, native_events = machine.execute(native, table[NAME], vector_address, threshold_address, xyz, threshold)
        linked_ret, linked_xyz, linked_events = machine.execute(linked, table[NAME], vector_address, threshold_address, xyz, threshold)
        h = host(xyz, threshold, alias)
        assert canonical([native_ret] + native_xyz) == canonical([linked_ret] + linked_xyz) == canonical(h) == canonical(expected), ('semantic divergence', category, xyz, threshold, alias)
        for events in [native_events, linked_events]:
            reads = [address for kind, address in events if kind == 'read']
            writes = [address for kind, address in events if kind == 'write']
            assert sorted(reads) == sorted([vector_address, vector_address + 4, vector_address + 8, threshold_address]), 'unexpected external reads'
            assert writes == ([vector_address + i * 4 for i in range(3)] if writes_expected else []), 'unexpected external writes'
            if writes:
                assert max(i for i, event in enumerate(events) if event[0] == 'read') < min(i for i, event in enumerate(events) if event[0] == 'write'), 'output changed before input snapshot'
        if not writes_expected:
            assert native_ret == linked_ret == h[0] == 0, 'early result is not positive zero'
            assert native_xyz == linked_xyz == h[1:] == xyz, 'early exit did not preserve input bits'
            early += 1
        else:
            normalize += 1
        tally[category] = tally.get(category, 0) + 1
    return {'result': 'PASS', 'cases': sum(tally.values()), 'categories': tally,
            'early_preserve_cases': early, 'normalization_cases': normalize,
            'models': ['complete native instructions', 'complete freshly linked IDO instructions',
                       'host C candidate body', 'source-level binary32 oracle'],
            'external_access_bounds_and_snapshot_order': 'PASS',
            'limitations': ['round-to-nearest binary32 arithmetic',
                            'NaNs compared by class, not payload or signaling behavior',
                            'FCSR exceptions, traps and alternate rounding modes are not modeled',
                            'parameterized threshold tests do not claim all inputs occur in gameplay']}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    selection = parser.add_mutually_exclusive_group()
    selection.add_argument('--source', type=Path, help='compile a standalone non-matching comparison candidate')
    selection.add_argument('--group', type=Path, help='O3 group directory; defaults to the final matching packet')
    parser.add_argument('--build', type=Path, default=ROOT / 'build/e0b8_verification')
    args = parser.parse_args()
    source, build = (args.source or SOURCE).resolve(), args.build.resolve()
    if not args.source:
        args.group = (args.group or GROUP).resolve()
    build.mkdir(parents=True, exist_ok=True)
    obj = build / 'candidate.o'
    group = None
    flags = FLAGS
    if args.group:
        group = score.compile_group(args.group.resolve(), obj)
        assert len(group['files']) == 1, 'host replay requires one group source file'
        source = args.group.resolve() / group['files'][0]
        flags = group['flags']
    else:
        score.compile_single(source, FLAGS, obj)
    proof, native, linked, table = inspect_object(obj, build)
    if group:
        proof['context'] = inspect_context(obj, build, group['members'], table)
        proof['group_manifest_sha256'] = sha((args.group / 'group.json').read_bytes())
    proof.update({'function': NAME, 'source_sha256': sha(source.read_bytes()),
                  'flags': flags, 'claims': [], 'status': 'VERIFIED_BYTES_ONLY' if proof['accepted_exact_match'] else 'RESEARCH_ONLY',
                  'compiler_sha256': {name: sha((score.IDO / name).read_bytes())
                                      for name in ['cc', 'cfe', 'uopt', 'ugen', 'as1']}})
    proof['semantics'] = verify_semantics(source, build, native, linked, table)
    (build / 'verification.json').write_text(json.dumps(proof, indent=2) + '\n')
    print(json.dumps(proof, indent=2))


if __name__ == '__main__':
    main()
