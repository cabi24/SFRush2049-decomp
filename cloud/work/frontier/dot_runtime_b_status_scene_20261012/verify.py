#!/usr/bin/env python3
"""Input-bound whole-group proof for image B's 532-byte status-scene helper."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
NAME = 'func_80391650'
ADDRESS, SIZE, RODATA = 0x80391650, 532, 0x80394F08
CONTEXT = ['func_80391864', 'func_80391B00']
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
BASELINE = ROOT / 'cloud/work/lean/runtime_b_hud_scenes_20261006'
TEST = ROOT / 'tests/cloud/test_dot_runtime_b_status_scene.py'
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers


def require(ok, message):
    if not ok:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


def image_bytes():
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(sha(asset) == 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8', 'asset identity changed')
    require(struct.unpack_from('>I', asset, 0x3854)[0] == 0xB6FEC4, 'loader pointer changed')
    image = zlib.decompress(asset[0xB6FEC4 - 0x283D0:], -15)
    require(len(image) == 43888 and sha(image) == 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd', 'image identity changed')
    return image


@contextmanager
def targets(image):
    old = score.ASM_DIR
    directory = ROOT / 'asm/us/ovl_b'
    key = owndata.artifact_dir(directory)
    present, previous = key in score._own_data, score._own_data.get(key)
    score.ASM_DIR = directory
    score._own_data[key] = None if image is None else owndata.ImageData.from_image(image, 0x8038A400)
    try:
        yield
    finally:
        score.ASM_DIR = old
        if present:
            score._own_data[key] = previous
        else:
            score._own_data.pop(key, None)


def native_calls(words, address):
    return [i * 4 for i, word in enumerate(words)
            if word >> 26 == 3 and ((word & 0x3ffffff) << 2 | 0x80000000) == address]


def stack_zero(word):
    return (word >> 26, word >> 21 & 31, word >> 16 & 31, word & 65535) == (43, 29, 0, 0)


def inputs():
    image = image_bytes()
    with targets(image):
        table = score.targets()
        manifest = score.target_manifest()
        extent_rows = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))['functions']
        selected = {name: next(e for e in extent_rows if e['name'] == name) for name in [NAME] + CONTEXT}
        require(selected[NAME]['address'] == hex(ADDRESS).upper().replace('0X', '0x') and selected[NAME]['size'] == SIZE, 'member extent changed')
        for name, row in selected.items():
            address = int(row['address'], 16)
            require(len(table[name]) * 4 == row['size'], 'selected extent/target disagreement')
            require(packed(table[name]) == image[address - 0x8038A400:address - 0x8038A400 + row['size']], 'selected target/image disagreement')
        callers = {name: native_calls(words, ADDRESS) for name, words in table.items() if native_calls(words, ADDRESS)}
        require(callers == {'func_80391B00': [0x5c]}, 'native caller census changed')
        require(stack_zero(table['func_80391B00'][0x60 // 4]), 'native stack argument changed')
        literal = image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 4]
        require(literal == struct.pack('>f', 0.025), 'owned literal changed')
        addresses = score.image_symbols()
        helper_names = ['sound_call_minimal', 'string_copy_format', 'func_8008E398', 'model_data_load', 'model_transform_setup', 'func_8008B32C', 'func_80090E9C', 'car_lights_render']
        helper_addresses = {name: addresses.get(name, score.address_named(name)) for name in helper_names}
        globals_used = ['D_80151AD0', 'D_80152818', 'D_80395CD0', 'D_80150B70', 'D_80393B68', 'D_80140BDC', 'D_80393B74', 'D_8012E700', 'D_80116178', 'D_80394210']
        binding = {name: addresses.get(name, score.address_named(name)) for name in helper_names + globals_used}
        require(all(isinstance(a, int) for a in binding.values()), 'unresolved member binding')
    old = score.ASM_DIR
    try:
        score.ASM_DIR = ROOT / 'asm/us/blob'
        game = score.targets()
        game_addresses = score.image_symbols()
        helper_words = {}
        for n in helper_names:
            matches = [key for key in game if game_addresses.get(key, score.address_named(key)) == helper_addresses[n]]
            require(len(matches) == 1, 'ambiguous native helper target')
            helper_words[n] = {'target': matches[0], 'sha256': sha(packed(game[matches[0]]))}
    finally:
        score.ASM_DIR = old
    source_files = [HERE / 'group.c', HERE / 'group.json', HERE / 'verify.py', HERE / 'layout_check.c', TEST, BASELINE / 'group.c', BASELINE / 'group.json']
    return {'sources_sha256': {str(p.relative_to(ROOT)): sha(p.read_bytes()) for p in source_files},
            'population': 'ovl_b', 'selected_extents': selected,
            'selected_words_sha256': {n: sha(packed(table[n])) for n in selected},
            'image_sha256': sha(image), 'owned_literal': {'address': RODATA, 'bytes': 4, 'sha256': sha(literal)},
            'member_external_bindings': binding, 'helper_words_sha256': helper_words,
            'native_callers': callers, 'native_stack_argument': {'offset': 0, 'value': 0},
            'flags': FLAGS, 'compiler': 'ido5.3'}


def inspect(path, linked=False):
    raw, sections = score._elf(path)
    header = struct.unpack_from('>16sHHIIIIIHHHHHH', raw)
    require(header[0][:9] == b'\x7fELF\x01\x02\x01\0\0' and header[1] == (2 if linked else 1) and header[2] == 8 and header[7] in (0x10000000, 0x10000001), 'ELF ABI/MIPS2 identity changed')
    symbols = {s['name']: s for i, sec in enumerate(sections) if sec['type'] == 2 for s in score._symbol_table(raw, sections, i) if s['name']}
    ti = score._text_index(sections)
    functions = {n: s for n, s in symbols.items() if s['type'] == 2 and s['section'] not in (0, 0xfff1)}
    require(set(functions) == {NAME, *CONTEXT}, 'defined function set changed')
    origin = ADDRESS if linked else 0
    require((functions[NAME]['value'], functions[NAME]['size'], functions[NAME]['section']) == (origin, SIZE, ti), 'member full extent changed')
    ordered = sorted(functions.items(), key=lambda pair: pair[1]['value'])
    cursor = origin
    for name, fn in ordered:
        require(fn['value'] == cursor and fn['section'] == ti and fn['size'] > 0, 'gap, overlap or section in complete function intervals')
        cursor += fn['size']
    require(functions[CONTEXT[0]]['value'] == origin + SIZE, 'next function boundary changed')
    allocated = {}
    for i, sec in enumerate(sections):
        sh = struct.unpack_from('>IIIIIIIIII', raw, header[6] + i * header[11])
        if linked:
            require(sec['type'] not in (4, 9) or not sec['size'], 'GNU relocations remain')
        if sh[2] & 2 and sec['size']:
            if sec['name'] in ('.text', '.rodata'):
                require(sh[3] == ((ADDRESS if sec['name'] == '.text' else RODATA) if linked else 0), 'section placement changed')
                allocated[sec['name']] = raw[sec['off']:sec['off'] + sec['size']]
            else:
                require(not linked and sec['name'] in ('.reginfo', '.options'), 'unexpected allocated section')
    require(set(allocated) == {'.text', '.rodata'}, 'missing owned sections')
    suffix = allocated['.text'][cursor - origin:]
    require(len(suffix) < 16 and suffix == b'\0' * len(suffix), 'nonzero or excessive final text suffix')
    return raw, sections, symbols, functions, allocated


def link_complete(obj, temp, tag):
    raw, sections, symbols, functions, allocated = inspect(obj)
    addresses = score.image_symbols()
    undefined = {n: addresses.get(n, score.address_named(n)) for n, s in symbols.items() if s['section'] == 0}
    require(all(v is not None for v in undefined.values()), 'unknown external binding')
    member_relocs, counts = [], {'.text': 0, '.rodata': 0}
    for sec in sections:
        if sec['type'] != 9:
            continue
        relocated = sections[sec['info']]['name']
        require(relocated in counts, 'unexpected relocated section')
        table = score._symbol_table(raw, sections, sec['link'])
        for pos in range(sec['off'], sec['off'] + sec['size'], 8):
            offset, info = struct.unpack_from('>II', raw, pos)
            require(offset % 4 == 0 and offset + 4 <= sections[sec['info']]['size'], 'relocation outside section')
            kind = info & 255
            require(kind in ((4, 5, 6) if relocated == '.text' else (2,)), 'unexpected relocation kind')
            counts[relocated] += 1
            if relocated == '.text' and offset < SIZE:
                symbol = table[info >> 8]
                name = symbol['name'] or sections[symbol['section']]['name']
                if symbol['section'] != 0:
                    require(name == '.rodata', 'unexpected local member reference')
                member_relocs.append({'offset': offset, 'kind': kind, 'symbol': name})
    require(len(member_relocs) == 38, 'member relocation census changed')
    require([r['offset'] for r in member_relocs if r['symbol'] == '.rodata'] == [0x20, 0x24], 'member owned-data references changed')
    script = temp / (tag + '.ld')
    script.write_text('\n'.join('%s = 0x%08x;' % x for x in sorted(undefined.items())) + '\nSECTIONS { .text 0x80391650 : SUBALIGN(4) { *(.text) } .rodata 0x80394F08 : SUBALIGN(4) { *(.rodata) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n')
    out = temp / (tag + '.elf')
    subprocess.run(['mips-linux-gnu-ld', '-EB', '--hash-style=sysv', '-T', str(script), '-o', str(out), str(obj)], check=True, capture_output=True)
    _, _, bound, linked_functions, linked = inspect(out, True)
    require(not [n for n, s in bound.items() if s['section'] == 0], 'GNU unresolved symbol')
    require(all(bound[n]['value'] == a and bound[n]['section'] == 0xfff1 for n, a in undefined.items()), 'GNU external binding changed')
    return linked, {'member_text_relocations': member_relocs, 'whole_object_relocation_counts': counts,
                    'whole_object_bytes': {k: len(v) for k, v in allocated.items()},
                    'complete_function_extents': {n: {'offset': s['value'], 'size': s['size']} for n, s in functions.items()},
                    'external_bindings': undefined}


def replay(deep=False):
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'requires recognized stock IDO5.3')
    compiler = {name: sha(Path(score.ido(name)).read_bytes()) for name in ['cc', 'cfe', 'uld', 'usplit', 'umerge', 'uopt', 'ugen', 'as1']}
    require(shutil.which('mips-linux-gnu-ld') is not None, 'GNU MIPS linker missing')
    image = image_bytes()
    with targets(image), tempfile.TemporaryDirectory(prefix='dot-b-status-') as name:
        temp = Path(name)
        obj = temp / 'group.o'
        spec = score.compile_group(HERE, obj)
        require(spec['members'] == spec['claims'] == [NAME] and spec['context'] == CONTEXT and spec['keep'] == ['func_80391B00'], 'member/context/keep contract changed')
        result = score.compare(obj, NAME, show=0)
        require(result.accepted(), result.summary())
        require(all(not score.compare(obj, n, show=0).accepted() for n in CONTEXT), 'a verified context must be promoted to members and claims')
        linked, facts = link_complete(obj, temp, 'candidate')
        native = packed(score.targets()[NAME])
        literal = image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 4]
        require(linked['.text'][:SIZE] == native and linked['.rodata'][:4] == literal, 'complete GNU member/literal mismatch')
        raw, secs, symbols, functions, sections = inspect(obj)
        parent = functions['func_80391B00']
        parent_words = struct.unpack('>%dI' % (parent['size'] // 4), sections['.text'][parent['value']:parent['value'] + parent['size']])
        call_sites = []
        for sec in secs:
            if sec['type'] != 9 or secs[sec['info']]['name'] != '.text':
                continue
            table = score._symbol_table(raw, secs, sec['link'])
            for at in range(sec['off'], sec['off'] + sec['size'], 8):
                offset, info = struct.unpack_from('>II', raw, at)
                symbol = table[info >> 8]
                word = struct.unpack_from('>I', sections['.text'], offset)[0]
                calls_member = (symbol['name'] == NAME or
                                (symbol['section'] == score._text_index(secs) and
                                 symbol['value'] + ((word & 0x3ffffff) << 2) == functions[NAME]['value']))
                if info & 255 == 4 and calls_member:
                    relative = offset - parent['value']
                    require(0 <= relative < parent['size'] - 4 and stack_zero(parent_words[relative // 4 + 1]), 'compiled caller stack argument changed')
                    call_sites.append(relative)
        require(len(call_sites) == 1, 'compiled caller census changed')
        with targets(None):
            absent = score.compare(obj, NAME, show=0)
            require(not absent.accepted() and absent.unverified, 'missing owned data accepted')
        bad_image = bytearray(image)
        bad_image[RODATA - 0x8038A400] ^= 1
        with targets(bytes(bad_image)):
            bad = score.compare(obj, NAME, show=0)
            require(not bad.accepted() and bad.errors, 'corrupt owned data accepted')
        layout = temp / 'layout.o'
        subprocess.run([score.ido('cc'), '-g0', '-O2', '-mips2', '-G', '0', '-non_shared', '-I', str(HERE), '-c', str(HERE / 'layout_check.c'), '-o', str(layout)], check=True, capture_output=True, cwd=temp)
        record = {'inputs': binding, 'compiler_sha256': compiler, 'comparison': asdict(result),
                  'proof': facts, 'compiled_private_call_sites': call_sites,
                  'member_sha256': sha(linked['.text'][:SIZE]), 'owned_literal_sha256': sha(linked['.rodata'][:4]),
                  'controls': {'absent_owned_data': 'rejected', 'corrupt_owned_data': 'rejected', 'native_c89_layout': 'passed'},
                  'new_strict_verified_bytes': SIZE, 'preserved_candidate_bytes': 0,
                  'new_image_landed_bytes': 0, 'new_rom_verified_bytes': 0}
        if deep:
            baseline = temp / 'baseline'; baseline.mkdir()
            for file in ('group.c', 'group.json'):
                (baseline / file).write_bytes((BASELINE / file).read_bytes())
            old_obj = temp / 'baseline.o'
            score.compile_group(baseline, old_obj)
            old = score.compare(old_obj, NAME, show=0)
            require(old.differing == 19 and not any((old.extra_words, old.unverified, old.unresolved, old.errors)), 'baseline residual changed')
            old_linked, _ = link_complete(old_obj, temp, 'baseline')
            differences = [i for i in range(0, SIZE, 4) if old_linked['.text'][i:i + 4] != native[i:i + 4]]
            require(len(differences) == 19, 'independent GNU baseline differs')
            source = (HERE / 'group.c').read_text()
            first, rest = source.split('s32 func_80391864(void *unused)', 1)
            variants = {
                'declarations': (first.replace('    Point point;\n    s32 kind, i;', '    s32 i, kind;\n    Point point;'), 3),
                'status_order': (first.replace('        status = &D_80395CD0[i];\n', '').replace('    for (i = 0; i < D_80151AD0; i++) {', '    for (i = 0; i < D_80151AD0; i++) {\n        status = &D_80395CD0[i];'), 6),
                'uv_induction': (first.replace('D_80395CD0[i].uv', 'status->uv'), 10),
                'direct_byte': (first.replace('*(u8 *)(u32)&D_80140BDC', 'D_80140BDC'), 80),
            }
            for tag, (body, expected) in variants.items():
                directory = temp / tag; directory.mkdir()
                (directory / 'group.c').write_text(body + 's32 func_80391864(void *unused)' + rest)
                (directory / 'group.json').write_bytes((HERE / 'group.json').read_bytes())
                control_obj = temp / (tag + '.o')
                score.compile_group(directory, control_obj)
                control = score.compare(control_obj, NAME, show=0)
                require(control.differing == expected and not any((control.unresolved, control.unverified, control.errors, control.extra_words)), 'causal rollback changed: ' + tag)
            print('PASS: full GNU baseline 19; causal rollbacks declarations=3, status_order=6, uv_induction=10, direct_byte=80')
        return record


def verify(deep=False):
    expected = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == expected['inputs'], 'input receipt changed')
    actual = replay(deep)
    require(actual == expected, 'verification receipt changed')
    return actual


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    parser.add_argument('--score', action='store_true')
    parser.add_argument('--deep', action='store_true')
    args = parser.parse_args()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(args.deep), indent=2) + '\n')
        print('Recorded complete 532-byte member and four-byte owned literal')
    elif args.inputs_only:
        require(inputs() == json.loads((HERE / 'verification.json').read_text())['inputs'], 'input receipt changed')
        print('PASS: current source and selected native bindings')
    elif args.score:
        with targets(image_bytes()):
            sys.argv = ['score.py', 'group', str(HERE), '--targets', str(ROOT / 'asm/us/ovl_b')]
            raise SystemExit(score.main())
    else:
        verify(args.deep)
        print('PASS: full member/owned data; GNU relocations/extents; genuine caller; negative controls; native layout')
