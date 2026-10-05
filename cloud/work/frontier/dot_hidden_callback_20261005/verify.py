#!/usr/bin/env python3
"""Source-bound complete-body, independent GNU link and bounded behavior proof."""
import argparse
import ctypes
from dataclasses import asdict
import hashlib
import importlib.util
import itertools
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT))
from tools.cloud import score
spec = importlib.util.spec_from_file_location('hidden_native', HERE / 'native.py')
native = importlib.util.module_from_spec(spec); spec.loader.exec_module(native)
FN = 'state_update_global'
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
SOURCE = ROOT / 'cloud/matches/state_update_global.c'
CONTEXT = ROOT / 'src/blob/groups/frontier_pad_config/group.c'
MUTANTS = {
    'stale_hide_return': ('return blt->Hide;', 'return hide;'),
    'unsigned_hide': ('s8 Hide;', 'u8 Hide;'),
    'clear_before_update': ('    Input_ApplyPadConfig(blt);\n    blt->AnimFunc = 0;',
                            '    blt->AnimFunc = 0;\n    Input_ApplyPadConfig(blt);'),
    'skip_visible_update': ('    Input_ApplyPadConfig(blt);\n    blt->AnimFunc = 0;',
                           '    blt->AnimFunc = 0;'),
    'positive_only_global': ('D_80149D98 != 0', 'D_80149D98 > 0'),
}


def sha(data): return hashlib.sha256(data).hexdigest()


def symbol(obj, name):
    data, sections = score._elf(obj)
    matches = [s for i, section in enumerate(sections) if section['type'] == 2
               for s in score._symbol_table(data, sections, i)
               if s['name'] == name and s['type'] == 2 and s['section'] == score._text_index(sections)]
    assert len(matches) == 1, (name, matches)
    return matches[0]


def inspect(obj, name):
    comparison = score.compare(obj, name, show=0)
    result = dict(asdict(comparison), verdict=comparison.summary(), symbol_bytes=symbol(obj, name)['size'])
    result['native_bytes'] = len(score.targets()[name]) * 4
    return result


def complete(result):
    return (result['symbol_bytes'] == result['native_bytes'] and result['verdict'] == 'MATCH'
            and not any(result[k] for k in ('differing', 'unresolved', 'unverified', 'errors', 'extra_words')))


def code_proof(work):
    obj = work / 'candidate.o'; score.compile_single(SOURCE, FLAGS, obj)
    result = inspect(obj, FN); assert complete(result)
    fn = symbol(obj, FN); start, end = fn['value'], fn['value'] + fn['size']
    assert fn['size'] == native.SIZE == 112
    want = score.targets()[FN]
    resolved, masks, unresolved, unverified, errors = score.relocate(obj, score.text_words(obj), start, end, score.image_symbols())
    assert resolved[start // 4:end // 4] == want
    assert not any((masks, unresolved, unverified, errors))
    data, sections = score._elf(obj); ti = score._text_index(sections)
    assert not any(s['size'] for s in sections if s['name'] in ('.data', '.rodata', '.rdata', '.bss'))
    relocs = []
    for section in sections:
        if section['type'] == 9 and section['info'] == ti:
            symbols = score._symbol_table(data, sections, section['link'])
            for p in range(section['off'], section['off'] + section['size'], 8):
                offset, info = struct.unpack_from('>II', data, p)
                if start <= offset < end:
                    relocs.append({'offset': offset - start, 'type': info & 255, 'symbol': symbols[info >> 8]['name']})
    assert len(relocs) == 6 and sum(r['type'] == 4 for r in relocs) == 2
    undefined = [s for i, section in enumerate(sections) if section['type'] == 2
                 for s in score._symbol_table(data, sections, i) if s['section'] == 0 and s['name']]
    names = score.image_symbols()
    script = work / 'proof.ld'
    script.write_text('SECTIONS { .text 0x%x : SUBALIGN(4) { *(.text) } }\n' % (native.BASE - start)
                      + ''.join('%s = 0x%x;\n' % (s['name'], names[s['name']]) for s in undefined))
    elf, binary = work / 'proof.elf', work / 'proof.bin'
    subprocess.run(['mips-linux-gnu-ld', '-EB', '-T', str(script), '-o', str(elf), str(obj)], check=True, capture_output=True)
    subprocess.run(['mips-linux-gnu-objcopy', '-O', 'binary', '-j', '.text', str(elf), str(binary)], check=True, capture_output=True)
    linked = binary.read_bytes(); body = linked[start:end]
    assert symbol(elf, FN)['value'] == native.BASE and symbol(elf, FN)['size'] == native.SIZE
    assert body == struct.pack('>28I', *want) and not any(linked[end:])
    result.update(start=hex(native.BASE), end_exclusive=hex(native.BASE + native.SIZE),
                  full_symbol_equality=True, independent_gnu_link=True, body_sha256=sha(body),
                  relocations=relocs, own_data_bytes=0, excluded_preceding_helper_bytes=start,
                  excluded_zero_alignment_bytes=len(linked) - end)
    return result, list(struct.unpack('>28I', body))


def cases():
    for g, hide, first, mutate in itertools.product([0, 1, -1, 2, 2147483647, -2147483648], range(-128, 128), [-1, 0, 1, 256], [0, 3]):
        yield g, hide, first, 256, mutate
    for args in itertools.product([0, -1, 1, -2147483648], [-128, -1, 0, 1, 2, 127], [-128, -1, 0, 1, 127, 256], [-128, -1, 0, 1, 127, 256], range(4)):
        yield args


def host_library(work, name='ordinary'):
    source = SOURCE
    if name != 'ordinary':
        before, after = MUTANTS[name]
        text = SOURCE.read_text(); assert text.count(before) == 1
        source = work / (name + '.c'); source.write_text(text.replace(before, after))
    library = work / (name + '.so')
    command = ['cc', '-std=c99', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
               '-fsanitize=undefined', '-fno-sanitize-recover=all', '-DCANDIDATE_SOURCE="%s"' % source,
               str(HERE / 'host.c'), '-o', str(library)]
    subprocess.run(command, check=True, capture_output=True)
    dll = ctypes.CDLL(str(library)); dll.run_case.argtypes = [ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)]
    return dll


def host_result(dll, args):
    output = (ctypes.c_int * 14)(); dll.run_case((ctypes.c_int * 5)(*args), output)
    return list(output)


def behavior(work, linked_words):
    selected = list(cases()); host = host_library(work)
    native_words = score.targets()[FN]; visited = set(); digest = hashlib.sha256()
    for args in selected:
        expected = native.reference(args)
        assert host_result(host, args) == expected, ('host', args)
        for label, words in [('native', native_words), ('GNU-linked', linked_words)]:
            machine = native.Machine(words, args)
            assert machine.run() == expected, (label, args)
            visited.update(machine.visited)
        digest.update(struct.pack('>14i', *expected))
    assert visited == set(range(0, native.SIZE, 4))
    controls = {}
    for mutant in MUTANTS:
        bad = host_library(work, mutant)
        for args in selected:
            if host_result(bad, args) != native.reference(args):
                controls[mutant] = {'rejected': True, 'witness': list(args)}; break
        assert mutant in controls, mutant
    return {'cases': len(selected), 'native_runs': len(selected) * 2, 'host_ubsan': 'passed',
            'target_instructions_covered': len(visited), 'target_instructions': 28,
            'ordered_callback_snapshots': 'equal', 'full_untouched_blit_and_stack_canaries': 'preserved',
            'o32_saved_registers': 'preserved', 'global_loads_per_native_run': 1,
            'output_sha256': digest.hexdigest(), 'negative_controls': controls}


def context_proof(work):
    group = work / 'context'; group.mkdir()
    shutil.copyfile(SOURCE, group / 'candidate.c'); shutil.copyfile(CONTEXT, group / 'pad.c')
    old = json.loads((CONTEXT.parent / 'group.json').read_text())
    names = old['members'] + old['context']
    spec = {'files': ['candidate.c', 'pad.c'], 'members': [FN], 'context': names,
            'keep': [FN] + old['keep'], 'flags': FLAGS}
    (group / 'group.json').write_text(json.dumps(spec))
    obj = group / 'group.o'; score.compile_group(group, obj)
    result = {name: inspect(obj, name) for name in [FN] + names}
    assert all(complete(row) for row in result.values())
    # Also check exact ELF boundaries instead of depending on scorer section attribution.
    for name in result:
        fn = symbol(obj, name); start, end = fn['value'], fn['value'] + fn['size']
        words, masks, unknown, unverified, errors = score.relocate(obj, score.text_words(obj), start, end, score.image_symbols())
        assert words[start // 4:end // 4] == score.targets()[name]
        assert not any((masks, unknown, unverified, errors))
    return {'context_source': str(CONTEXT.relative_to(ROOT)), 'context_sha256': sha(CONTEXT.read_bytes()),
            'context_unchanged': (group / 'pad.c').read_bytes() == CONTEXT.read_bytes(),
            'existing_roots_preserved': True, 'bodies': result,
            'limit': 'This is the genuine three-body callback/callee group, not the full-game shadow unit.'}


def controls(work):
    result = {}
    for name, source, flags in [
        ('archived_direct_o3', ROOT / 'cloud/work/near_miss_B/state_update_global_best.c', FLAGS),
        ('helper_o2', SOURCE, FLAGS.replace('-O3', '-O2'))]:
        obj = work / (name + '.o'); score.compile_single(source, flags, obj)
        result[name] = dict(inspect(obj, FN), source=str(source.relative_to(ROOT)), source_sha256=sha(source.read_bytes()), flags=flags)
    assert result['archived_direct_o3']['differing'] == 3
    assert not complete(result['helper_o2'])
    return result


def verify(work, with_behavior=True):
    code, words = code_proof(work)
    result = {'status': 'STRICT_MATCH_PENDING_INDEPENDENT_REVIEW', 'accepted_byte_gain': 0,
              'base_revision': 'cc4d5fdd0bbc42dbf6be49f00d9454af8cb9c4f5', 'function': FN,
              'flags': FLAGS, 'source': str(SOURCE.relative_to(ROOT)), 'source_sha256': sha(SOURCE.read_bytes()),
              'code': code, 'controls': controls(work), 'genuine_context': context_proof(work)}
    if with_behavior: result['behavior'] = behavior(work, words)
    result['packet_sha256'] = {name: sha((HERE / name).read_bytes()) for name in ['verify.py', 'native.py', 'host.c']}
    result['compiler_sha256'] = {name: sha(Path(score.ido(name)).read_bytes()) for name in ['cc', 'cfe', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1']}
    result['target_manifest_sha256'] = sha((score.ASM_DIR / 'SHA256SUMS').read_bytes())
    result['limits'] = ['No full-game shadow unit, splice, linked image, compressed stream or ROM gate.',
                        'Bounded native interpreter and adversarial UpdateBlit mock, not actual callee or gameplay execution.',
                        'The host compiles unchanged C using its native pointer layout; ELF/native checks independently establish the O32 offsets.',
                        'Only the 112-byte callback is claimed; the preceding standalone helper and section alignment are excluded.',
                        'Claim checks cannot reveal unpublished worker activity.']
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--write', action='store_true'); args = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix='hidden-callback-proof-') as directory:
        result = verify(Path(directory))
    if args.write: (HERE / 'verification.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps({key: result[key] for key in ['status', 'code', 'genuine_context', 'behavior']}, indent=2))
