#!/usr/bin/env python3
"""Portable, source-only, full-body verification of the voiceUnblock topology."""
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
CANDIDATE = 'cloud/matches/boot_tail/func_8001F954.c'
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
FINAL_SOURCE_SHA = '9681a183355a574946d7b680d39f0801bf0eb5e99a04fe81fb4e587933a80333'
SELECTED = {
    'func_8001F954': (CANDIDATE, None),
    'func_80014AF0': ('cloud/work/boot_tail_promotion/audio_record_contracts/sources/func_80014AF0.c', 'src/rom/lib_11640.c'),
    'func_8001C7F4': ('cloud/work/boot_tail_promotion/sources/func_8001C7F4.c', 'src/rom/lib_1cf90.c'),
}
HEADERS = ('include/boot_tail_audio_record.h', 'include/boot_tail_sample_contract.h')
NATIVE_PATHS = {
    'func_8001F954': 'asm/us/nonmatchings/rom/lib_1f5b0/func_8001F954.s',
    'func_80014AF0': 'asm/us/nonmatchings/rom/lib_11640/func_80014AF0.s',
    'func_8001C7F4': 'asm/us/nonmatchings/rom/lib_1cf90/func_8001C7F4.s',
    'func_8001467C': 'asm/us/nonmatchings/rom/lib_11640/func_8001467C.s',
    'func_8001F6EC': 'asm/us/nonmatchings/rom/lib_1f5b0/func_8001F6EC.s',
}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def load_module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


def canonical_words(data):
    values = re.findall(rb'/\* [0-9A-F]+ [0-9A-F]+ ([0-9A-F]{8}) \*/', data)
    return [int(value, 16) for value in values]


def verify(root, scratch):
    pins = {}

    def git(path):
        data = subprocess.check_output(['git', 'show', BASE + ':' + path], cwd=root)
        pins[path] = sha(data)
        return data

    def materialize(path):
        destination = scratch / path
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_bytes(git(path))
        return destination

    def command(arguments):
        return subprocess.check_output([str(x) for x in arguments], cwd=scratch,
                                       stderr=subprocess.STDOUT)

    # Reuse immutable Git objects even in sparse worktrees. No repository source
    # or tool is edited, and temporary native bytes are never part of the packet.
    for path in ['tools/cloud/score.py', 'tools/cloud/owndata.py',
                 'tools/conveyor/seeds/extract_candidates.py']:
        materialize(path)
    for path in HEADERS:
        materialize(path)
    manifest = materialize('asm/us/boot_tail/SHA256SUMS').read_text()
    for line in manifest.splitlines():
        digest, filename = line.split('  ')
        assert re.fullmatch(r'[a-zA-Z0-9_.-]+', filename)
        assert sha(materialize('asm/us/boot_tail/' + filename).read_bytes()) == digest
    sys.path.insert(0, str(scratch / 'tools/cloud'))
    score = load_module('voice_unblock_score', scratch / 'tools/cloud/score.py')
    score.REPO = root
    score.IDO = Path(os.environ.get('IDO_DIR', root / 'tools/cloud/ido'))
    score.ASM_DIR = scratch / 'asm/us/boot_tail'
    extractor = load_module('voice_unblock_extract', scratch / 'tools/conveyor/seeds/extract_candidates.py')
    # Use the exact repository normalization implementation without importing
    # unrelated coordinator dependencies or materializing their files.
    lock_tree = ast.parse(git('tools/conveyor/pipeline/lock.py'))
    normalizer = next(node for node in lock_tree.body
                      if isinstance(node, ast.FunctionDef) and node.name == 'normalize_body')
    namespace = {'re': re}
    exec(compile(ast.Module(body=[normalizer], type_ignores=[]), '<pinned normalize_body>', 'exec'), namespace)
    normalize = namespace['normalize_body']
    locks = json.loads(git('matched.lock.json'))
    addresses = score.image_symbols()
    targets = score.targets()
    native = {}
    for name, path in NATIVE_PATHS.items():
        body = canonical_words(git(path))
        assert body == targets[name]
        assert addresses[name] == int(name[5:], 16)
        native[name] = body

    behavior = load_module('voice_unblock_native_behavior', PACKET / 'native_behavior.py')
    rows = []
    for name, (path, tu_path) in SELECTED.items():
        if name == 'func_8001F954':
            source = scratch / 'func_8001F954.c'
            source.write_bytes((root / path).read_bytes())
            assert sha(source.read_bytes()) == FINAL_SOURCE_SHA
        else:
            source = materialize(path)
            production = git(tu_path).decode()
            source_bodies = {fn: source.read_text()[start:end]
                             for fn, start, end in extractor.extract_functions(source.read_text())}
            production_bodies = {fn: production[start:end]
                                 for fn, start, end in extractor.extract_functions(production)}
            assert normalize(source_bodies[name]) == normalize(production_bodies[name])
            assert sha(normalize(production_bodies[name]).encode()) == locks[tu_path + ':' + name]['body_sha256']
        obj = scratch / (name + '.o')
        score.compile_single(source, FLAGS + ' -I' + str(scratch / 'include'), obj)
        comparison = score.compare(obj, name, show=0)
        assert comparison.accepted(), comparison
        data, sections = score._elf(obj)
        symbols = [s for i, section in enumerate(sections) if section['type'] == 2
                   for s in score._symbol_table(data, sections, i)]
        own = [s for s in symbols if s['name'] == name and s['type'] == 2]
        assert len(own) == 1 and own[0]['value'] == 0
        extent = own[0]['size']
        assert extent == len(targets[name]) * 4
        words = score.text_words(obj)
        resolved, masks, unresolved, unverified, errors = score.relocate(
            obj, words, 0, len(words) * 4, addresses)
        assert not (masks or unresolved or unverified or errors)
        assert all(section['size'] == 0 for section in sections if section['name'] in
                   ('.data', '.rodata', '.bss', '.sdata', '.sbss', '.lit4', '.lit8', '.rdata'))
        bindings = {s['name']: addresses.get(s['name'], score.address_named(s['name']))
                    for s in symbols if s['section'] == 0 and s['name']}
        assert all(value == int(symbol.rsplit('_', 1)[1], 16)
                   for symbol, value in bindings.items())
        script = ('SECTIONS { .text ' + hex(addresses[name]) +
                  ' : SUBALIGN(4) { *(.text) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) '
                  '*(.pdr) *(.mdebug) *(.comment) *(.gnu.attributes) } }\n')
        script += ''.join(symbol + ' = ' + hex(value) + ';\n' for symbol, value in bindings.items())
        ld = scratch / (name + '.ld')
        ld.write_text(script)
        elf, binary = scratch / (name + '.elf'), scratch / (name + '.bin')
        command(['mips-linux-gnu-ld', '-T', ld, '-o', elf, obj])
        command(['mips-linux-gnu-objcopy', '-O', 'binary', '--only-section=.text', elf, binary])
        linked = binary.read_bytes()
        symbols_text = command(['mips-linux-gnu-readelf', '-sW', elf]).decode()
        match = re.search(r'\d+:\s+([0-9a-fA-F]+)\s+(\d+)\s+FUNC\s+GLOBAL\s+DEFAULT\s+\d+\s+' + name + r'\s', symbols_text)
        assert match and int(match[1], 16) == addresses[name] and int(match[2]) == extent
        assert linked == b''.join(struct.pack('>I', word) for word in resolved)
        assert linked[:extent] == b''.join(struct.pack('>I', word) for word in targets[name])
        assert not any(linked[extent:])
        row = dict(function=name, source_path=path, source_sha256=sha(source.read_bytes()),
                   exact_current_lock_body=(tu_path is not None), elf_size=extent,
                   text_size=len(linked), alignment_zero_bytes=len(linked) - extent,
                   complete_native_body_sha256=sha(linked[:extent]), bindings=bindings,
                   comparison=asdict(comparison))
        if name == 'func_8001F954':
            row['behavior'] = behavior.verify(native[name], list(struct.unpack('>31I', linked[:extent])))
        rows.append(row)

    final = (root / CANDIDATE).read_text()
    controls = []
    baseline = git('cloud/work/boot_tail/BT03-high-init/nonmatch/func_8001F954.c').decode()
    signed = final.replace('func_8001F954(u32 index)', 'func_8001F954(int index)').replace('index == 0xFFFFFFFFU', 'index == -1')
    named_pointer = final.replace('u32 index)\n{', 'u32 index)\n{\n    VoiceState *state;')
    named_pointer = named_pointer.replace('    D_8004BEB8[index].identifier = index;',
        '    state = &D_8004BEB8[index];\n    state->identifier = index;')
    named_pointer = named_pointer.replace('func_8001F6EC(&D_8004BEB8[index]);', 'func_8001F6EC(state);')
    named_pointer = named_pointer.replace('D_8004BEB8[index].valueBD = 0;', 'state->valueBD = 0;')
    nested_guard = final.replace('    if (index == 0xFFFFFFFFU) {\n        return;\n    }',
                                '    if (index != 0xFFFFFFFFU) {')
    nested_guard = nested_guard.rstrip()[:-1] + '    }\n}\n'
    for label, text, flags, differences in [('baseline', baseline, FLAGS, 2),
                                          ('signed_boundary', signed, FLAGS, 0),
                                          ('named_pointer_restored', named_pointer, FLAGS, 2),
                                          ('nested_guard', nested_guard, FLAGS, 0),
                                          ('final_O1', final, FLAGS.replace('-O2', '-O1'), None)]:
        source, obj = scratch / 'control.c', scratch / 'control.o'
        source.write_text(text)
        score.compile_single(source, flags, obj)
        result = score.compare(obj, 'func_8001F954', show=0)
        assert result.differing == differences if differences is not None else not result.accepted()
        data, sections = score._elf(obj)
        function = [s for i, section in enumerate(sections) if section['type'] == 2
                    for s in score._symbol_table(data, sections, i)
                    if s['name'] == 'func_8001F954' and s['type'] == 2]
        assert len(function) == 1 and function[0]['value'] == 0
        size = function[0]['size']
        words = score.text_words(obj)
        resolved, masks, unresolved, unverified, errors = score.relocate(obj, words, 0, len(words)*4, addresses)
        assert not (masks or unresolved or unverified or errors)
        assert not any(words[size // 4:])
        whole_diff = [i * 4 for i in range(max(size // 4, 31))
                      if i >= size // 4 or i >= 31 or resolved[i] != targets['func_8001F954'][i]]
        if differences is not None:
            assert size == 124 and len(whole_diff) == differences
        controls.append(dict(source=label, source_sha256=sha(text.encode()), elf_size=size,
                             complete_body_diff_offsets=whole_diff,
                             zero_alignment_bytes=len(words)*4-size, comparison=asdict(result)))

    harness = scratch / 'test_host.c'
    harness.write_bytes((PACKET / 'test_host.c').read_bytes())
    executable = scratch / 'test_host'
    command(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
             '-fstrict-aliasing', '-O2', '-fsanitize=address,undefined,bounds',
             '-fno-omit-frame-pointer', '-no-pie', harness, '-o', executable])
    environment = dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1')
    host = subprocess.check_output([str(executable)], cwd=scratch, env=environment, text=True).strip()
    assert host == '2112 actual-source cases passed'
    negative = []
    for label, before, after in [('wrong_clear_field', '.valueBD = 0;', '.unknownBE[0] = 0;'),
                                 ('wrong_identifier_value', '.identifier = index;', '.identifier = index + 1;'),
                                 ('wrong_sentinel', 'index == 0xFFFFFFFFU', 'index == 0xFFFFFFFEU')]:
        assert final.count(before) == 1
        (scratch / 'negative.c').write_text(final.replace(before, after))
        mutant = scratch / 'test_negative'
        command(['cc', '-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror',
                 '-fsanitize=undefined,bounds', '-no-pie', '-DCANDIDATE="negative.c"', harness, '-o', mutant])
        result = subprocess.run([str(mutant)], cwd=scratch, env=environment,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        assert result.returncode != 0, label + ' survived'
        negative.append(label)
    for name in ['native_behavior.py', 'test_host.c', 'verify.py']:
        pins['packet:' + name] = sha((PACKET / name).read_bytes())
    tools = {p.name: sha(p.read_bytes()) for p in score.IDO.iterdir() if p.is_file()}
    return dict(status='PASS', base=BASE, recipe=FLAGS + ' -Wab,-r4300_mul',
                sole_new_match='func_8001F954', new_matching_bytes=124, accepted_byte_gain=0,
                rows=rows, controls=controls, host=host, actual_c_negative_controls_rejected=negative,
                input_sha256=pins, ido_sha256=tools,
                limits=['Boundary hooks do not execute the two nonmatching native helper bodies',
                        'Valid slots 0..31 and FFFFFFFF only; no malformed-state or hardware claim',
                        'No full TU, source-image, compression, full-ROM or CI gate is claimed',
                        'T050 remains in force; no 1F13C compiler attempt occurred'])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--repo', type=Path, default=DEFAULT_ROOT)
    parser.add_argument('--output', type=Path)
    parser.add_argument('--check', action='store_true')
    options = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='voice-unblock-') as name:
        result = verify(options.repo.resolve(), Path(name))
    encoded = json.dumps(result, indent=2) + '\n'
    if options.check:
        assert result == json.loads((PACKET / 'evidence.json').read_text()), 'frozen receipt drift'
    if options.output:
        options.output.write_text(encoded)
    print(encoded, end='')


if __name__ == '__main__':
    main()
