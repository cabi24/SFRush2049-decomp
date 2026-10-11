#!/usr/bin/env python3
"""Input-bound IDO 7.1 replay of the PR163 voice-unblock extraction.

Only scratch objects/TUs are written. This is not lock/promotion/ROM authority.
"""
import argparse
from dataclasses import asdict
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

if not __debug__:
    raise SystemExit('Verification requires assertions; do not use optimized Python.')

ROOT = Path(__file__).resolve().parents[4]
PACKET = Path(__file__).resolve().parent
GROUP = ROOT / 'cloud/matches/voice_unblock_production'
SOURCE = GROUP / 'func_8001F954.c'
LEGACY = ROOT / 'cloud/matches/boot_tail/func_8001F954.c'
TU = ROOT / 'src/rom/lib_1f5b0.c'
FN = 'func_8001F954'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
MARKER = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1f5b0/func_8001F954.s")'
DATA = {'.data', '.rodata', '.rdata', '.bss', '.sdata', '.sbss', '.lit4', '.lit8'}
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers
from tools.conveyor.seeds.extract_candidates import extract_functions


def sha(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>' + str(len(words)) + 'I', *words)


def norm(text):
    return ' '.join(text.split())


def bodies(text):
    return {name: text[start:end] for name, start, end in extract_functions(text)}


def inputs():
    paths = [SOURCE, GROUP / 'group.json', LEGACY, PACKET / 'verify.py', PACKET / 'host_test.c']
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    manifest = score.target_manifest()
    target = score.targets()[FN]  # normal protected-target validation remains active
    assert len(target) == 31
    extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
    own = [row for row in extents['functions'] if row['name'] == FN]
    assert len(own) == 1 and own[0]['address'] == '0x8001F954' and own[0]['size'] == 124
    return {
        'input_sha256': {str(p.relative_to(ROOT)): sha(p.read_bytes()) for p in paths},
        'target_rows': {name: manifest[name] for name in ('extents.json', 'symbols.json')},
        'target': {'population': 'static boot tail', 'function': FN,
                   'address': '0x8001F954', 'bytes': 124, 'words_sha256': sha(packed(target))},
    }


def source_contract():
    source, baseline, legacy = SOURCE.read_text(), TU.read_text(), LEGACY.read_text()
    assert source.splitlines()[0] == '/* flags: ' + FLAGS + ' */'
    assert set(bodies(source)) == {FN}
    expected = bodies(legacy)[FN].replace('.identifier =', '.identifier60 =').replace('.valueBD =', '.activeBD =')
    assert norm(bodies(source)[FN]) == norm(expected), 'body differs from established match'
    for name in ('SequenceNode', 'VoiceState'):
        pattern = r'typedef struct ' + name + r' \{.*?\} ' + name + ';'
        a, b = re.findall(pattern, source, re.S), re.findall(pattern, baseline, re.S)
        assert len(a) == len(b) == 1 and norm(a[0]) == norm(b[0]), name + ' contract drift'
    assert source.count('extern void func_8001F6EC(VoiceState *state);') == 1
    assert json.loads((GROUP / 'group.json').read_text()) == {
        'files': ['func_8001F954.c'], 'flags': FLAGS, 'keep': [FN], 'members': [FN],
        'claims': [FN], 'context': [], 'allow_unverified': False}
    return source, baseline


def command(args, expected_error=None):
    result = subprocess.run([str(arg) for arg in args], cwd=ROOT, capture_output=True, text=True,
        env=dict(os.environ, ASAN_OPTIONS='detect_leaks=0:halt_on_error=1', UBSAN_OPTIONS='halt_on_error=1'))
    if expected_error:
        assert result.returncode and expected_error in result.stderr, result.stdout + result.stderr
    else:
        assert result.returncode == 0, result.stdout + result.stderr
    return result


def native_row(obj, name):
    result = score.compare(obj, name, show=0)
    assert result.accepted(False), name + ': ' + result.summary()
    raw, sections = score._elf(obj)
    symbols = [s for i, sec in enumerate(sections) if sec['type'] == 2
               for s in score._symbol_table(raw, sections, i)]
    own = [s for s in symbols if s['name'] == name and s['type'] == 2 and s['section'] != 0]
    target = score.targets()[name]
    assert len(own) == 1 and own[0]['size'] == len(target) * 4, 'function extent'
    start = own[0]['value']
    resolved, masks, unresolved, unverified, errors = score.relocate(
        obj, score.text_words(obj), start, start + len(target) * 4, score.image_symbols())
    assert not (masks or unresolved or unverified or errors), 'relocation uncertainty'
    actual = resolved[start // 4:start // 4 + len(target)]
    assert actual == target, 'relocated words'
    assert not any(s['size'] for s in sections if s['name'] in DATA), 'owned data'
    return {'comparison': asdict(result), 'function_bytes': own[0]['size'],
            'body_sha256': sha(packed(actual)), 'owned_data_bytes': 0}


def independent_link(obj, work):
    """GNU resolves the entire standalone object independently of score.relocate."""
    raw, sections = score._elf(obj)
    symbols = [s for i, sec in enumerate(sections) if sec['type'] == 2
               for s in score._symbol_table(raw, sections, i)]
    addresses = score.image_symbols()
    bindings = {s['name']: addresses.get(s['name'], score.address_named(s['name']))
                for s in symbols if s['section'] == 0 and s['name']}
    assert all(v is not None for v in bindings.values())
    script = 'SECTIONS { .text 0x8001F954 : SUBALIGN(4) { *(.text) }\n'
    script += '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.mdebug) *(.comment) *(.gnu.attributes) } }\n'
    script += ''.join(k + ' = ' + hex(v) + ';\n' for k, v in sorted(bindings.items()))
    ld, elf, binary = work / 'body.ld', work / 'body.elf', work / 'body.bin'
    ld.write_text(script)
    command(['mips-linux-gnu-ld', '--hash-style=sysv', '-T', ld, '-o', elf, obj])
    command(['mips-linux-gnu-objcopy', '-O', 'binary', '--only-section=.text', elf, binary])
    data, target = binary.read_bytes(), packed(score.targets()[FN])
    assert data[:124] == target and data[124:] == b'\0' * 4, 'independent body/padding'
    listing = command(['mips-linux-gnu-readelf', '-sW', elf]).stdout
    match = re.search(r'\d+:\s+([0-9a-fA-F]+)\s+(\d+)\s+FUNC\s+GLOBAL\s+DEFAULT\s+\d+\s+' + FN + r'\s', listing)
    assert match and int(match[1], 16) == 0x8001F954 and int(match[2]) == 124
    header = elf.read_bytes()
    assert header[:16] == b'\x7fELF\x01\x02\x01' + b'\0' * 9  # ELF32, BE, SysV ABI 0
    assert struct.unpack_from('>HH', header, 16) == (2, 8)  # executable, EM_MIPS
    assert struct.unpack_from('>I', header, 36)[0] == 0x10000000  # IDO MIPS2 ELF flags
    return {'body_sha256': sha(data[:124]), 'address': '0x8001F954', 'function_bytes': 124,
            'zero_alignment_bytes': 4, 'elf': 'ELF32 big-endian MIPS2, SysV ABI 0', 'bindings': bindings}


def replay(work):
    source, baseline = source_contract()
    # Select the actual production compiler before looking for any scorer default.
    cc = compilers.tu_cc(TU.stem, score.IDO / 'cc')
    assert compilers.identify_dir(cc.parent) == 'ido7.1', 'expected pinned IDO7.1'
    for tool in ('mips-linux-gnu-as', 'mips-linux-gnu-ld', 'mips-linux-gnu-objcopy',
                 'mips-linux-gnu-objdump', 'mips-linux-gnu-readelf', 'cc'):
        assert shutil.which(tool), tool + ' absent; pinned IDO and MIPS binutils/C compiler required'
    score.IDO = cc.parent
    group_obj = work / 'group.o'
    score.compile_group(GROUP, group_obj)
    assert set(score.symbols(group_obj)) == {FN}
    proof = {'compiler': 'ido7.1', 'O3_group': native_row(group_obj, FN),
             'independent_GNU_link': independent_link(group_obj, work)}

    spec = importlib.util.spec_from_file_location('voice_v2_context', ROOT / 'cloud/work/boot_tail_promotion/context_check.py')
    context = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(context)
    context.ROOT, context.CC, context.COMPILER = ROOT, cc, 'ido7.1'
    context.CFLAGS = ['-G', '0', '-mips2', '-O2', '-non_shared', '-I' + str(ROOT / 'include'),
                     '-I' + str(ROOT / 'include/PR'), '-D_LANGUAGE_C', '-Wab,-r4300_mul',
                     '-Xcpluscomm', '-I' + str(ROOT / 'src/rom')]
    row = context.check(FN, work, str(SOURCE))
    assert row['status'] == 'ok', row
    proof['production_O2'] = native_row(work / (FN + '.orig.o'), FN)
    proof['rom_tu_header_O2'] = native_row(work / (FN + '.ctx.o'), FN)

    # Compile-time N64 ABI/layout constraints, never host pointer-size assumptions.
    offsets = {'command00': 0, 'next_identifier': 16, 'parent_identifier': 20, 'entry18': 24,
               'flags24': 36, 'value28': 40, 'external4C': 76, 'identifier60': 96, 'activeBD': 189}
    constraints = ['sizeof(void *) == 4', 'sizeof(SequenceNode) == 16', 'sizeof(VoiceState) == 416']
    constraints += ['OFF(VoiceState, ' + k + ') == ' + str(v) for k, v in offsets.items()]
    layout = source + '\n#define OFF(T, f) ((unsigned int)&((T *)0)->f)\n'
    layout += 'typedef char native_layout[(' + ' && '.join(constraints) + ') ? 1 : -1];\n'
    ok, err = context.compile_c(layout, work / 'layout.o')
    assert ok, err
    proof['native_layout'] = {'pointer_bytes': 4, 'SequenceNode_bytes': 16, 'VoiceState_bytes': 416, 'offsets': offsets}

    old = bodies(baseline)
    if MARKER in baseline:
        assert baseline.count(MARKER) == 1 and FN not in old
        have = {norm(line) for line in baseline.splitlines() if line.strip()}
        declarations = [s for s in row['preamble'] if s.startswith('#') or norm(s) not in have]
        combined = baseline.replace(MARKER, '\n'.join(declarations) + '\n' + bodies(source)[FN], 1)
        assert bodies(combined) == dict(old, **{FN: bodies(source)[FN]})
        mode = 'scratch insertion into current passthrough'
    else:
        assert FN in old and norm(old[FN]) == norm(bodies(source)[FN]), 'different live C implementation'
        combined, mode = baseline, 'already-promoted identical C body'
    slots = set(old) | set(re.findall(r'GLOBAL_ASM\("[^\"]+/(func_[0-9A-F]+)\.s"\)', baseline))
    all_rows = None
    for label, text in (('current', baseline), ('adapted', combined)):
        path, obj = work / (label + '.c'), work / (label + '.o')
        path.write_text(text)
        command([sys.executable, ROOT / 'tools/asm-processor/build.py', cc,
                 '--', 'mips-linux-gnu-as', '-march=vr4300', '-mabi=32', '-I' + str(ROOT / 'include'),
                 '--', '-c', *context.CFLAGS, '-o', obj, path])
        assert set(score.symbols(obj)) == slots, 'whole-TU function membership'
        fresh = {name: native_row(obj, name) for name in sorted(slots)}
        if all_rows is not None:
            assert fresh == all_rows, 'whole-TU native rows changed'
        all_rows = fresh
    # Context is freshly checked, not receipt-pinned to mutable TU/lock/tool files.
    live = {'mode': mode, 'compiler': 'ido7.1', 'whole_TU_functions': len(slots),
            'summed_function_bytes': sum(r['function_bytes'] for r in all_rows.values()),
            'preexisting_C_bodies_unchanged': True, 'whole_TU_strict_native_equality': True}

    (work / 'candidate.c').write_text(source)
    host = work / 'host'
    host_flags = ['-std=c89', '-pedantic-errors', '-Wall', '-Wextra', '-Werror', '-fstrict-aliasing',
                  '-O2', '-fsanitize=address,undefined,bounds', '-fno-omit-frame-pointer', '-no-pie']
    command(['cc', *host_flags, '-I' + str(work), PACKET / 'host_test.c', '-o', host])
    assert command([host]).stdout.strip() == '2112 actual-source cases passed'
    proof['bounded_host_cases'] = 2112
    rejected = []
    for label, mutant in (
            ('wrong_identifier', source.replace('.identifier60 = index;', '.identifier60 = index + 1;')),
            ('owned_data', source + '\nunsigned int voice_v2_owned = 1;\n')):
        obj = work / (label + '.o')
        ok, err = context.compile_c(mutant, obj)
        assert ok, err
        try:
            native_row(obj, FN)
        except AssertionError:
            rejected.append(label)
        else:
            raise AssertionError(label + ' incorrectly accepted')
    # Alter the function's ELF size without changing its bytes; it must fail.
    raw, sections = score._elf(group_obj)
    altered = bytearray(raw)
    for i, sec in enumerate(sections):
        if sec['type'] == 2:
            for j, symbol in enumerate(score._symbol_table(raw, sections, i)):
                if symbol['name'] == FN:
                    struct.pack_into('>I', altered, sec['off'] + j * 16 + 8, 120)
    wrong_size = work / 'wrong_size.o'
    wrong_size.write_bytes(altered)
    try:
        native_row(wrong_size, FN)
    except AssertionError:
        rejected.append('wrong_ELF_extent')
    else:
        raise AssertionError('wrong ELF extent incorrectly accepted')
    proof['negative_controls_rejected'] = rejected
    assert rejected == ['wrong_identifier', 'owned_data', 'wrong_ELF_extent']
    return proof, live


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    bound = inputs()
    source_contract()
    receipt = json.loads((PACKET / 'evidence.json').read_text()) if args.check else None
    if receipt:
        assert bound == receipt['inputs'], 'packet input binding drift'
    if args.inputs_only:
        print('Input bindings and source contract PASS')
        return
    with tempfile.TemporaryDirectory(prefix='voice-unblock-v2-') as directory:
        proof, live = replay(Path(directory))
    if receipt:
        assert proof == receipt['proof'], 'strict proof invariant drift'
    result = {'schema': 'voice-unblock-v2', 'inputs': bound, 'proof': proof,
              'live_context': live, 'new_matching_bytes': 0, 'promotion_candidate_bytes': 124 if live['mode'].startswith('scratch') else 0,
              'accepted_byte_gain': 0, 'full_ROM_gate': 'not run; coordinator owned'}
    encoded = json.dumps(result, indent=2) + '\n'
    if args.output:
        args.output.write_text(encoded)
    print(encoded, end='')


if __name__ == '__main__':
    main()
