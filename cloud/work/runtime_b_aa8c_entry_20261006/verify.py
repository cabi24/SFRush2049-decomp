#!/usr/bin/env python3
"""Replay bounded native behavior. No IDO build, native dump or live-tree hash pin."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import sys
import zlib

HERE = Path(__file__).resolve().parent
ROOT_PATH = HERE.parents[2]
BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
EXPECTED = {
    'func_8038A95C': (0x8038A95C, 184, 'f6f3b782e02e565f5bf723f54839b9c7f80483f410028f6a9db7e595b580664e'),
    'func_8038AA14': (0x8038AA14, 120, '9f23d5a9e52f138ede5552e5f931250ec7df73aa654150fd157966e6ca83808d'),
    'func_8038AA8C': (0x8038AA8C, 7812, 'c8842f0b25b7c17e36345611643e4b67a7c0c17929f7be6ba93785b530b67b39'),
}
IMAGE_HASH = 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
PACKET_FILES = ('layouts.h', 'cleanup_helpers.c', 'aa8c_entry.c.fragment',
                'native.py', 'reference.py', 'verify.py', 'README.md')


def digest(raw): return hashlib.sha256(raw).hexdigest()


def git_read(reference, path):
    return subprocess.check_output(['git', '-C', str(reference), 'show', BASE + ':' + path])


def load_targets(reference):
    # Use the unchanged canonical target loader; target words, not the changing
    # manifest/scorer/accepted source hashes, bind this packet across integration.
    spec = importlib.util.spec_from_file_location('aa8c_current_score', ROOT_PATH / 'tools/cloud/score.py')
    original_path = sys.path[:]
    sys.path.insert(0, str(ROOT_PATH / 'tools/cloud'))
    try:
        score = importlib.util.module_from_spec(spec)
        sys.modules[spec.name] = score
        spec.loader.exec_module(score)
    finally:
        sys.path[:] = original_path
    score.ASM_DIR = reference / 'asm/us/ovl_b'
    targets = score.targets()
    asset = git_read(reference, 'assets/us/data.bin')
    inflater = zlib.decompressobj(-15)
    image = inflater.decompress(asset[0xB6FEC4 - 0x283D0:])
    assert inflater.eof and len(image) == 43888 and digest(image) == IMAGE_HASH
    metadata, code = {}, {}
    for name, (start, size, expected) in EXPECTED.items():
        words = targets[name]
        raw = struct.pack('>' + str(len(words)) + 'I', *words)
        assert len(raw) == size and digest(raw) == expected
        assert raw == image[start - 0x8038A400:start - 0x8038A400 + size]
        metadata[name] = {'address': hex(start), 'size': size, 'sha256': expected}
        for index, word in enumerate(words):
            address = start + index * 4
            if name != 'func_8038AA8C' or address < 0x8038AB8C or address >= 0x8038C8E8:
                code[address] = word
    # Accepted-source evidence is bound to BASE, not the future live tree.
    remove = git_read(reference, 'src/blob/sound_call_minimal.c').decode()
    assert 'entity_spawn_callback(arg0, 0, 0);' in remove
    hide = git_read(reference, 'src/blob/model_data_load.c').decode()
    assert 'mode == 0' in hide and '| 0x80000000' in hide
    return code, metadata


def behavior(code):
    def local_module(name):
        spec = importlib.util.spec_from_file_location('aa8c_entry_' + name, HERE / (name + '.py'))
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        return module
    native, reference = local_module('native'), local_module('reference')
    Fixture, Machine = native.Fixture, native.Machine
    ROOT, GROUP_CLEAN, SLOT_CLEAN = native.ROOT, native.GROUP_CLEAN, native.SLOT_CLEAN
    COLOR, PHYSICS = native.COLOR, native.PHYSICS
    cleanup, entry = reference.cleanup, reference.entry
    count, coverage, branches = 0, set(), set()
    standard = [0xFFFFFFFF, 0, 0x0000FFFF, 0x80000001, 0xFFFF8000]
    def check(start=ROOT, raw_player=None, **arguments):
        nonlocal count
        original, expected = Fixture(**arguments), Fixture(**arguments)
        machine = Machine(code, original, start, raw_player)
        actual_status = machine.run()
        if start == ROOT: expected_status = entry(expected)
        else:
            cleanup(expected, start == GROUP_CLEAN, arguments['player'])
            expected_status = 'return'
        assert actual_status == expected_status, (start, arguments, actual_status, expected_status)
        assert original.memory.snapshot() == expected.memory.snapshot(), (start, arguments, 'state')
        assert original.trace == expected.trace, (start, arguments, 'trace', original.trace, expected.trace)
        if start == ROOT:
            assert (COLOR, 4) in original.memory.reads
            if (arguments['update'] & 65535) == 0:
                assert not any(PHYSICS <= address < PHYSICS + 4 * 0x808 for address, _ in original.memory.reads)
            else:
                guard_read = (PHYSICS + arguments['player'] * 0x808 + 0x640, 1)
                assert original.memory.reads.index((COLOR, 4)) < original.memory.reads.index(guard_read)
        count += 1; coverage.update(machine.coverage); branches.update(machine.branches)

    # Every raw cached byte, every raw signed inhibit byte, and real update=0/1.
    # These are orthogonal domains, not a misleading full Cartesian-product claim.
    for player in range(4):
        for cached in range(256):
            for update, inhibit in ((0, 0), (1, 0), (1, 1), (1, 128)):
                check(player=player, cached=cached, update=update, inhibit=inhibit,
                      handles=standard, extra=0x1234FFFF)
        for inhibit in range(256):
            for cached in (0, 1):
                check(player=player, cached=cached, update=1, inhibit=inhibit,
                      handles=standard, extra=-1)
        for update in (0, 1, 2, -1, 0x8000, 0xFFFF0000, 0x10000, 0xFFFF0001):
            for cached in (0, 1, 8, 9, 255):
                for mutation in (0, 1, 2):
                    check(player=player, cached=cached, update=update, inhibit=0,
                          handles=standard, extra=0x7FFFFFFF, mutation=mutation)
        for start in (GROUP_CLEAN, SLOT_CLEAN):
            for mask in range(32):
                values = [standard[i] if mask & (1 << i) else -1 for i in range(5)]
                # Slot zero must sometimes be a live low-half-minus-one handle.
                if mask & 1: values[0] = 0x7654FFFF
                for extra in (-1, 0x0000FFFF):
                    for mutation in (0, 1, 2):
                        check(start, raw_player=0xFACE0000 | player,
                              player=player, cached=0, update=0, inhibit=0,
                              handles=values, extra=extra, mutation=mutation)
    assert coverage == set(code), ('native instruction coverage', sorted(set(code) - coverage))
    conditional = {address for address, word in code.items()
                   if word >> 26 in (4, 5, 20) and ((word >> 21) & 31) != ((word >> 16) & 31)}
    assert all((address, False) in branches and (address, True) in branches for address in conditional)

    rejected = []
    controls = ('narrow sentinel', 'early store', 'wide removal argument', 'wrong trailing byte',
                'unconditional extra clear', 'wide update test', 'inhibit exactly one',
                'mode instead of cache', 'early handle read', 'reload owner')
    for mutant in controls:
        caught = False
        for extra in (-1, 0x1234FFFF):
            for update, inhibit in ((0, 0), (0x10000, 0), (1, 128)):
                a = dict(player=2, cached=0, update=update, inhibit=inhibit,
                         handles=[0x0000FFFF, 1, -1, 0x8000, -1], extra=extra,
                         mutation=0 if mutant in ('wrong trailing byte', 'unconditional extra clear') else 2)
                original, wrong = Fixture(**a), Fixture(**a)
                native_status = Machine(code, original).run()
                wrong_status = entry(wrong, mutant)
                caught |= (native_status != wrong_status or original.trace != wrong.trace or
                           original.memory.snapshot() != wrong.memory.snapshot())
        assert caught, ('wrong contract survived', mutant)
        rejected.append(mutant)

    # Fail-closed controls, not claims about valid gameplay behavior.
    fixture_args = dict(player=0, cached=0, update=0, inhibit=0, handles=standard, extra=-1)
    for label, changed_code, change in (
        ('unsupported opcode', {**code, ROOT: 0xFFFFFFFF}, None),
        ('unmapped owner', code, lambda f: f.memory.put(f.descriptor + 8, 0x7FFF, 2)),
        ('unaligned descriptor', code, lambda f: setattr(f, 'descriptor', f.descriptor + 1)),
    ):
        bad = Fixture(**fixture_args)
        if change: change(bad)
        try: Machine(changed_code, bad).run()
        except AssertionError as exc:
            assert label.split()[0] in str(exc), (label, str(exc))
        else: raise AssertionError(('negative control succeeded', label))
        rejected.append(label)
    return {'paired_native_reference_fixtures': count,
            'native_instruction_coverage': {'A95C': 46, 'AA14': 30, 'AA8C_entry': 64, 'AA8C_exit_epilogue': 10},
            'conditional_branch_outcomes': len(conditional) * 2,
            'wrong_contracts_rejected': rejected,
            'domain': 'owners 0..3 only; orthogonal cached/inhibit byte domains; explicit update and mutation cases'}


def replay(reference_root):
    code, targets = load_targets(reference_root.resolve())
    result = {'status': 'PARTIAL-SOURCE; bounded native behavior verified; no compiled candidate or matching claim',
              'base': BASE, 'image_sha256': IMAGE_HASH, 'targets': targets,
              'packet_sha256': {name: digest((HERE / name).read_bytes()) for name in PACKET_FILES},
              'behavior': behavior(code), 'compiler_invocations': 0, 'new_matching_claims': [],
              'not_run': ['complete AA8C source', 'private O3 closure compilation', 'compiled C behavior',
                          'strict matching', 'object/relocation/owned-data proof', 'ROM/image construction']}
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--reference-root', type=Path, default=ROOT_PATH)
    parser.add_argument('--output', type=Path, default=HERE / 'verification.json')
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = replay(args.reference_root)
    if args.check:
        assert result == json.loads(args.output.read_text()), 'packet replay differs'
    else: args.output.write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result, indent=2))


if __name__ == '__main__': main()
