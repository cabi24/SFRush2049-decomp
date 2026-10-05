#!/usr/bin/env python3
"""Verify a fresh semantic formulation before invoking the stock compiler.

Requires previously downloaded, pinned SDK headers. They and binary artifacts
remain in ignored build/. Does not modify any existing proof or acceptance gate.
"""
from pathlib import Path
import argparse
import hashlib
import importlib.util
import json
import re
import shutil
import sys

ROOT = Path(__file__).resolve().parents[3]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score

HEADERS = {
    'libreultra_gbi.h': 'b418e0321f48acd86187a066a873211f0e0ff9cb',
    'mbi.h': '9956ef20eeb3090533be9d0f513ada78e208041c',
}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def git_blob_sha(data):
    return hashlib.sha1(b'blob ' + str(len(data)).encode() + b'\0' + data).hexdigest()


def sdk_context(directory):
    for name, digest in HEADERS.items():
        data = (directory / name).read_bytes()
        # Files fetched through the connector may have an extra final newline.
        if git_blob_sha(data) != digest and data.endswith(b'\n'):
            data = data[:-1]
        assert git_blob_sha(data) == digest, 'SDK source blob changed: ' + name
    mbi = (directory / 'mbi.h').read_text()
    shift = re.search(r'#define _SHIFTL\([^\n]*\\\n[^\n]+', mbi).group(0)
    # The complete unmodified SDK Gfx union is used by both compilers. On LP64
    # the host union is larger, so the wrapper measures Gfx elements, not bytes.
    (directory / 'sdk_context.h').write_text('#define _LANGUAGE_C\n#define F3DEX_GBI_2\n'
        + shift + '\n#include "libreultra_gbi.h"\n')


def extra_cases():
    state = (0, 0, 319, 0, 239, 0)
    for mode in [0, 1, -7]:
        for flip in [0, 4, 8, 12]:
            for stretch in [0, 0x8000]:
                g = (flip | stretch, *state[1:5], mode)
                for a in [(319, 239, 319, 239, 9, 17),
                          (10, 20, 30, 300, 9, 17),
                          (10, 20, 30, 40, 9, 17),
                          (10, -10, 30, 300, -17, -9)]:
                    yield 'fresh_boundary_and_phase', a, g, True
    yield 'y_word_not_short', (0, 65536, 20, 100, 0, 0), (0, 0, 100000, 0, 100000, 1), True
    # Actual caller-like coordinates start in signed halfwords, then acquire
    # positive row/height increments before the call, with no intermediate cast.
    yield 'caller_style_y_sum', (0, 32768, 20, 32770, 0, 0), (0, 0, 100000, 32700, 100000, 1), True


def falsification_controls(replay, source, build):
    content = source.read_text()
    variants = [
        ('reject_zero_area', 'if (right < x || bottom < y)',
         'if (right <= x || bottom <= y)'),
        ('narrow_y_to_short', '    if (x < D_8012E60C)',
         '    y = (short)y;\n    if (x < D_8012E60C)'),
        ('stretch_in_mode_zero', 'if (D_8014A248 != 0)',
         'if (D_8014A248 != 0 || (flags & 0x8000))'),
        ('post_stretch_texture_height', 'tex_t += height;',
         'tex_t += (D_8014A248 != 0 && (flags & 0x8000)) ? 2U * height + 1U : height;'),
        ('phase_without_vertical_flip', 'if (flags & 8)\n                t_phase = 16;',
         't_phase = 16;'),
        ('reclip_command_bottom', '    gSPTextureRectangle(',
         '    if ((int)command_bottom > D_8012E674)\n        command_bottom = D_8012E674;\n    gSPTextureRectangle('),
    ]
    controls = {}
    for name, before, after in variants:
        assert content.count(before) == 1, ('mutation target', name)
        directory = build / name
        directory.mkdir(exist_ok=True)
        for header in ['sdk_context.h', 'libreultra_gbi.h']:
            shutil.copyfile(build / header, directory / header)
        mutant = directory / 'rectangle.c'
        mutant.write_text(content.replace(before, after))
        host = replay.host_candidate(mutant, directory)
        failures = []
        for category, arguments, state, unused in extra_cases():
            if host(arguments, state)[0] != replay.oracle(arguments, state):
                failures.append({'category': category, 'arguments': arguments, 'state': state})
        assert failures, ('new tests did not reject incorrect assumption', name)
        prior = sum(host(args, state)[0] != replay.oracle(args, state)
                    for category, args, state, check_host in replay.cases() if check_host)
        controls[name] = {'rejected_by_new_cases': len(failures),
                          'rejected_by_prior_host_domain_cases': prior, 'first_failure': failures[0]}
    return controls


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--verifier-dir', type=Path, default=HERE.parent / 'texture_rect_verification')
    p.add_argument('--build', type=Path, default=ROOT / 'build/87110_fresh_behavior')
    p.add_argument('--compile', action='store_true')
    a = p.parse_args()
    build = a.build.resolve()
    build.mkdir(parents=True, exist_ok=True)
    sdk_context(build)
    spec = importlib.util.spec_from_file_location('rect_verifier', a.verifier_dir / 'replay.py')
    replay = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(replay)
    native, manifest = replay.independent_target()
    symbols = score.image_symbols()
    source = build / 'rectangle.c'
    shutil.copyfile(HERE / 'rectangle.c', source)
    host = replay.host_candidate(source, build)
    all_cases = list(replay.cases()) + list(extra_cases())
    tally, visited, paths = {}, set(), set()
    for category, arguments, state, unused in all_cases:
        expected = replay.oracle(arguments, state)
        actual, advance = host(arguments, state)
        words, nadvance, events, offsets = replay.machine.execute(native, symbols[replay.NAME], symbols, arguments, state)
        assert actual == words == expected, (category, arguments, state)
        assert advance == nadvance == 4 * len(expected)
        visited |= offsets
        tally[category] = tally.get(category, 0) + 1
        if expected:
            paths.add((state[5] != 0, state[0] & 12, bool(state[0] & 0x8000)))
    assert len(paths) == 16
    result = {'status': 'SEMANTIC_RESEARCH_ONLY', 'source_sha256': sha(source.read_bytes()),
        'design_sha256': sha((HERE / 'DESIGN.md').read_bytes()),
        'SDK_repository': 'n64decomp/libreultra', 'SDK_path': 'include/2.0I/PR',
        'SDK_git_blob_ids': HEADERS, 'cases': len(all_cases), 'categories': tally,
        'host_UBSan_cases': len(all_cases), 'native_instruction_offsets_executed': len(visited),
        'native_unexecuted_offsets': [hex(i) for i in range(0, 1780, 4) if i not in visited],
        'all_mode_flip_stretch_paths': len(paths),
        'observations': ['host C and independent packet oracle equal protected native execution',
            '32-bit unsigned arithmetic makes the full-word stress domain defined in fresh C',
            'no hardware-rendering or matching claim; source is intentionally factored',
            'read-event order is not required under the disjoint, nonracing domain'],
        'protected_manifest_entries_verified': len(manifest),
        'falsification_controls': falsification_controls(replay, source, build)}
    # Record successful source-level behavior before producing any IDO object.
    (build / 'host_verification.json').write_text(json.dumps(result, indent=2) + '\n')
    print('Host and native behavior PASS:', len(all_cases), 'cases', flush=True)
    if a.compile:
        flags = '-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
        obj = build / 'candidate.o'
        score.compile_single(source, flags, obj)
        proof, ignored, linked, ignored_symbols = replay.inspect(obj, build)
        differences = 0
        for category, arguments, state, unused in all_cases:
            expected = replay.oracle(arguments, state)
            words, advance, events, offsets = replay.machine.execute(linked, symbols[replay.NAME], symbols, arguments, state)
            assert words == expected and advance == 4 * len(expected), ('compiled behavior', category, arguments, state)
            native_events = replay.machine.execute(native, symbols[replay.NAME], symbols, arguments, state)[2]
            differences += events != native_events
        result['stock_compile'] = {'flags': flags, 'full_link_proof': proof,
            'compiled_behavior_cases': len(all_cases), 'read_event_order_differs_in_cases': differences,
            'object_sha256': sha(obj.read_bytes()),
            'compiler_sha256': {name: sha((score.IDO / name).read_bytes())
                                for name in ['cc', 'cfe', 'uopt', 'ugen', 'as1']}}
    (build / 'verification.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
