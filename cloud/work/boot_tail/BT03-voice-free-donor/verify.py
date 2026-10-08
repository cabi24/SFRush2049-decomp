#!/usr/bin/env python3
"""Source-only, pinned full-body research replay. No candidate is a match."""
import argparse
import ast
from dataclasses import asdict
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import struct
import subprocess
import sys
import tempfile

if not __debug__:
    raise SystemExit('Verification requires Python assertions; do not use -O or PYTHONOPTIMIZE.')

BASE = 'cd22879d40b3de443cfde047b86e75e159b6cec6'
PACKET = Path(__file__).resolve().parent
DEFAULT_ROOT = PACKET.parents[3]
CANDIDATE = 'cloud/work/boot_tail/BT03-voice-free-donor/nonmatch/func_8001F6EC.c'
BASELINE = 'cloud/work/boot_tail/BT03-high-larger/nonmatch/func_8001F6EC.c'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
FINAL_SOURCE_SHA = '14f88221464159614d611fbd55fa01219ac87018b12d92207ac7449757f2bc03'
ACCEPTED = {
    'func_8001EE9C': ('cloud/work/boot_tail_promotion/sources/func_8001EE9C.c', 'src/rom/lib_1f5b0.c'),
    'func_8001F9D0': ('cloud/work/boot_tail_promotion/voice_lists/sources/func_8001F9D0.c', 'src/rom/lib_1f5b0.c'),
    'func_80021BC0': ('cloud/matches/boot_tail/func_80021BC0.c', 'src/rom/lib_22300.c'),
}
NATIVE_PATHS = {name: 'asm/us/nonmatchings/rom/' + tu + '/' + name + '.s' for name, tu in (
    ('func_8001F6EC', 'lib_1f5b0'), ('func_8001EE9C', 'lib_1f5b0'),
    ('func_8001F9D0', 'lib_1f5b0'), ('func_80021BC0', 'lib_22300'),
    ('func_8001F954', 'lib_1f5b0'), ('func_80023E9C', 'lib_22300'))}



def portable_receipt(receipt):
    """Compare packet proof; base-context and whole-tree digests are provenance."""
    result = json.loads(json.dumps(receipt))
    for path in (
        'tools/cloud/score.py',
        'tools/cloud/owndata.py',
        'tools/conveyor/seeds/extract_candidates.py',
        'asm/us/boot_tail/SHA256SUMS',
        'asm/us/boot_tail/boot_tail_8000f3a4.s',
        'asm/us/boot_tail/extents.json',
        'asm/us/boot_tail/symbols.json',
        'tools/conveyor/pipeline/lock.py',
        'matched.lock.json',
        'asm/us/nonmatchings/rom/lib_1f5b0/func_8001F6EC.s',
        'asm/us/nonmatchings/rom/lib_1f5b0/func_8001EE9C.s',
        'asm/us/nonmatchings/rom/lib_1f5b0/func_8001F9D0.s',
        'asm/us/nonmatchings/rom/lib_22300/func_80021BC0.s',
        'asm/us/nonmatchings/rom/lib_1f5b0/func_8001F954.s',
        'asm/us/nonmatchings/rom/lib_22300/func_80023E9C.s',
        'cloud/work/boot_tail/BT03-high-larger/nonmatch/func_8001F6EC.c',
        'cloud/work/boot_tail_promotion/sources/func_8001EE9C.c',
        'src/rom/lib_1f5b0.c',
        'cloud/work/boot_tail_promotion/voice_lists/sources/func_8001F9D0.c',
        'cloud/matches/boot_tail/func_80021BC0.c',
        'src/rom/lib_22300.c',
    ):
        result.get('input_sha256', {}).pop(path, None)
    for row in result.get('accepted_context', []):
        row.pop('source_sha256', None)
        row.pop('current_normalized_lock_body_sha256', None)
    return result


def sha(data):
    return hashlib.sha256(data).hexdigest()


def load_module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def canonical_words(data):
    return [int(value, 16) for value in
            re.findall(rb'/\* [0-9A-F]+ [0-9A-F]+ ([0-9A-F]{8}) \*/', data)]


def donor_forms(baseline):
    indexed = baseline.replace(
        '    index = state->identifier60 & 255;\n    link = &D_80050440[index];\n'
        '    state->command00 = 0;\n    state->channel2E = 0;',
        '    state->command00 = 0;\n    state->channel2E = 0;\n'
        '    link = &D_80050440[(index = state->identifier60 & 255)];')
    def queue(source):
        return source.replace('            link->previous = D_800504C1;\n            link->next = 255;',
                              '            link->next = 255;\n            link->previous = D_800504C1;').replace(
            '            D_800504C0 = index;\n            link->next = 255;\n            link->previous = 255;',
            '            link->next = 255;\n            link->previous = 255;\n            D_800504C0 = index;')
    return indexed, queue(baseline), queue(indexed)


def verify(root, scratch):
    pins = {}
    def git(path):
        data = subprocess.check_output(['git', 'show', BASE + ':' + path], cwd=root)
        return data
    def materialize(path):
        destination = scratch / path
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(git(path))
        return destination
    def command(arguments, **options):
        return subprocess.check_output([str(x) for x in arguments], cwd=scratch,
                                       stderr=subprocess.PIPE, **options)

    for path in ('tools/cloud/score.py', 'tools/cloud/owndata.py',
                 'tools/conveyor/seeds/extract_candidates.py'):
        materialize(path)
    manifest = materialize('asm/us/boot_tail/SHA256SUMS').read_text()
    for line in manifest.splitlines():
        digest, filename = line.split('  ')
        assert re.fullmatch(r'[a-zA-Z0-9_.-]+', filename)
        assert sha(materialize('asm/us/boot_tail/' + filename).read_bytes()) == digest
    sys.path.insert(0, str(scratch / 'tools/cloud'))
    score = load_module('voice_free_score', scratch / 'tools/cloud/score.py')
    score.REPO = root
    score.IDO = Path(os.environ.get('IDO_DIR', root / 'tools/cloud/ido'))
    score.ASM_DIR = scratch / 'asm/us/boot_tail'
    extractor = load_module('voice_free_extract', scratch / 'tools/conveyor/seeds/extract_candidates.py')
    lock_tree = ast.parse(git('tools/conveyor/pipeline/lock.py'))
    normalizer = next(node for node in lock_tree.body
                      if isinstance(node, ast.FunctionDef) and node.name == 'normalize_body')
    namespace = {'re': re}
    exec(compile(ast.Module(body=[normalizer], type_ignores=[]), '<pinned normalize_body>', 'exec'), namespace)
    normalize = namespace['normalize_body']
    locks = json.loads(git('matched.lock.json'))
    addresses, targets = score.image_symbols(), score.targets()
    native = {}
    for name, path in NATIVE_PATHS.items():
        native[name] = canonical_words(git(path))
        assert native[name] == targets[name]
        assert addresses[name] == int(name[5:], 16)
    assert len(native['func_8001F6EC']) == 64
    assert 'src/rom/lib_1f5b0.c:func_8001F6EC' not in locks
    callers = {name: [i * 4 for i, word in enumerate(words)
                     if word >> 26 == 3 and ((word & 0x3FFFFFF) << 2 | 0x80000000) == 0x8001F6EC]
               for name, words in targets.items()}
    callers = {name: offsets for name, offsets in callers.items() if offsets}
    assert callers == {'func_8001F954': [92], 'func_8001F9D0': [48],
                       'func_80021BC0': [20], 'func_80023E9C': [512]}

    def compile_proof(label, source_text, name, expected_difference, bind_source=True):
        source, obj = scratch / (label + '.c'), scratch / 'candidate.o'
        source.write_text(source_text)
        score.compile_single(source, FLAGS, obj)
        comparison = score.compare(obj, name, show=0)
        assert comparison.differing == expected_difference
        assert comparison.accepted() == (expected_difference == 0)
        data, sections = score._elf(obj)
        symbols = [s for i, section in enumerate(sections) if section['type'] == 2
                   for s in score._symbol_table(data, sections, i)]
        own = [s for s in symbols if s['name'] == name and s['type'] == 2]
        assert len(own) == 1 and own[0]['value'] == 0
        extent = own[0]['size']
        assert extent == len(targets[name]) * 4
        words = score.text_words(obj)
        resolved, masks, unresolved, unverified, errors = score.relocate(obj, words, 0, len(words)*4, addresses)
        assert not (masks or unresolved or unverified or errors)
        assert all(section['size'] == 0 for section in sections if section['name'] in
                   ('.data', '.rodata', '.bss', '.sdata', '.sbss', '.lit4', '.lit8', '.rdata'))
        bindings = {s['name']: addresses.get(s['name'], score.address_named(s['name']))
                    for s in symbols if s['section'] == 0 and s['name']}
        assert all(value == int(symbol.rsplit('_', 1)[1], 16) for symbol, value in bindings.items())
        script = ('SECTIONS { .text ' + hex(addresses[name]) +
                  ' : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) '
                  '*(.pdr) *(.mdebug) *(.comment) *(.gnu.attributes) } }\n')
        script += ''.join(symbol + ' = ' + hex(value) + ';\n' for symbol, value in bindings.items())
        ld, elf, binary = scratch / 'candidate.ld', scratch / 'candidate.elf', scratch / 'candidate.bin'
        ld.write_text(script)
        command(['mips-linux-gnu-ld', '-T', ld, '-o', elf, obj])
        command(['mips-linux-gnu-objcopy', '-O', 'binary', '--only-section=.text', elf, binary])
        linked = binary.read_bytes()
        symbols_text = command(['mips-linux-gnu-readelf', '-sW', elf]).decode()
        match = re.search(r'\d+:\s+([0-9a-fA-F]+)\s+(\d+)\s+FUNC\s+GLOBAL\s+DEFAULT\s+\d+\s+' + name + r'\s', symbols_text)
        assert match and int(match[1], 16) == addresses[name] and int(match[2]) == extent
        assert linked == b''.join(struct.pack('>I', word) for word in resolved)
        assert not any(linked[extent:])
        differences = [i * 4 for i in range(max(extent // 4, len(targets[name])))
                       if i >= extent // 4 or i >= len(targets[name]) or resolved[i] != targets[name][i]]
        assert len(differences) == expected_difference
        row = dict(label=label, function=name,
                    elf_entry=hex(addresses[name]), elf_size=extent, text_size=len(linked),
                    zero_alignment_bytes=len(linked)-extent, bindings=bindings,
                    native_sha256=sha(b''.join(struct.pack('>I', word) for word in targets[name])),
                    gnu_linked_body_sha256=sha(linked[:extent]),
                    complete_body_difference_offsets=differences, comparison=asdict(comparison))
        if bind_source:
            row['source_sha256'] = sha(source_text.encode())
        return row, resolved[:extent//4]

    final = (root / CANDIDATE).read_text()
    assert sha(final.encode()) == FINAL_SOURCE_SHA
    baseline = git(BASELINE).decode()
    index_only, queue_only, donor = donor_forms(baseline)
    def body(text):
        return normalize(next(text[start:end] for name, start, end in extractor.extract_functions(text)
                              if name == 'func_8001F6EC'))
    assert body(final) == body(donor)
    candidate_row, linked = compile_proof('func_8001F6EC', final, 'func_8001F6EC', 28)
    rows = []
    for label, source, difference in (
        ('baseline', baseline, 30), ('donor_index_lifetime_only', index_only, 30),
        ('donor_queue_order_only', queue_only, 28),
        ('donor_pointer_field', final.replace('    u32 command00;', '    void *command00;'), 28)):
        row, _ = compile_proof(label, source, 'func_8001F6EC', difference)
        rows.append(row)
    accepted_rows = []
    for name, (path, tu_path) in ACCEPTED.items():
        source_text = git(path).decode()
        production = git(tu_path).decode()
        source_bodies = {fn: source_text[start:end] for fn, start, end in extractor.extract_functions(source_text)}
        production_bodies = {fn: production[start:end] for fn, start, end in extractor.extract_functions(production)}
        assert normalize(source_bodies[name]) == normalize(production_bodies[name])
        normalized_hash = sha(normalize(production_bodies[name]).encode())
        assert normalized_hash == locks[tu_path + ':' + name]['body_sha256']
        row, _ = compile_proof(name, source_text, name, 0, bind_source=False)
        row.update(source_path=path, production_path=tu_path)
        accepted_rows.append(row)

    behavior = load_module('voice_free_behavior', PACKET / 'native_behavior.py')
    native_result = behavior.verify(native['func_8001F6EC'], linked)
    expected = b''.join(bytes(image) for case in behavior.fixtures() for image in behavior.oracle(case))
    harness = scratch / 'test_host.c'
    harness.write_bytes((PACKET / 'test_host.c').read_bytes())
    (scratch / 'func_8001F6EC.c').write_text(final)
    executable = scratch / 'test_host'
    host_flags = ['-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-fstrict-aliasing',
                  '-O2', '-fsanitize=address,undefined,bounds', '-fno-omit-frame-pointer', '-no-pie']
    command(['cc'] + host_flags + [harness, '-o', executable])
    environment = dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1')
    actual = command([executable], env=environment)
    assert actual == expected, 'actual C89 source disagrees with canonical native-memory oracle'
    negative = []
    stale = final.replace('    func_8001EE9C(state);', '    index = state->identifier60 & 255;\n    func_8001EE9C(state);')
    stale = stale.replace('[(index = state->identifier60 & 255)]', '[index]')
    for label, source in (
        ('stale_pre_helper_identifier', stale),
        ('wrong_command_clear', final.replace('state->command00 = 0;', 'state->command00 = 1;')),
        ('wrong_user_guard', final.replace('link->active == 0', 'link->active != 0')),
        ('wrong_counter_direction', final.replace('--D_800504C2', '++D_800504C2'))):
        assert source != final
        (scratch / 'negative.c').write_text(source)
        command(['cc'] + host_flags + ['-DCANDIDATE="negative.c"', harness, '-o', executable])
        result = subprocess.run([str(executable)], cwd=scratch, env=environment,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        assert result.returncode != 0 or result.stdout != expected, label + ' survived'
        negative.append(label)
    for name in ('native_behavior.py', 'test_host.c', 'verify.py'):
        pins['packet:' + name] = sha((PACKET / name).read_bytes())
    native_target_bindings = {name: {'address': hex(addresses[name]), 'bytes': len(body) * 4,
                                      'sha256': sha(b''.join(struct.pack('>I', word) for word in body))}
                              for name, body in native.items()}
    return dict(native_target_bindings=native_target_bindings, status='RESEARCH-ONLY / COMPLETE-NONMATCH', base=BASE, recipe=FLAGS + ' -Wab,-r4300_mul',
                new_matching_bytes=0, accepted_byte_gain=0, candidate=candidate_row,
                controls=rows, accepted_context=accepted_rows, direct_boot_tail_callers=callers,
                native_behavior=native_result,
                actual_c89=dict(cases=native_result['cases'], canonical_output_bytes=len(actual),
                                canonical_output_sha256=sha(actual), sanitizer='ASan+UBSan+bounds',
                                negative_controls_rejected=negative),
                input_sha256=pins, ido_sha256={p.name:sha(p.read_bytes()) for p in score.IDO.iterdir() if p.is_file()},
                limits=['Only one-helper voiceFree boundary executed; real helper/caller bodies separately byte-verified',
                        'Four packed alignments, valid slots 0..31, bounded lists and ordinary sequential execution',
                        'No original N64 typedef/TU recovery, compiler instrumentation, hardware or gameplay proof',
                        'No integration, image, compression, full-ROM or hosted-CI claim; T050 remains in force'])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=DEFAULT_ROOT)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--check', action='store_true')
    options = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='voice-free-') as temporary:
        result = verify(options.repo.resolve(), Path(temporary))
    encoded = json.dumps(result, indent=2) + '\n'
    if options.check:
        assert portable_receipt(result) == portable_receipt(json.loads((PACKET / 'evidence.json').read_text())), 'frozen receipt drift'
    if options.output:
        options.output.write_text(encoded)
    print(encoded, end='')


if __name__ == '__main__':
    main()
