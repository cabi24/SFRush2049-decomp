#!/usr/bin/env python3
"""Input-bound image-B callback research; full extents and owned-data proof."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
NAME, ADDRESS, SIZE = 'func_80392894', 0x80392894, 1872
RODATA, FLAGS = 0x80394F50, '-g0 -O3 -mips2 -G 0 -non_shared'
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers


def require(condition, message):
    if not condition:
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
def bound_targets(image):
    old_dir = score.ASM_DIR
    target_dir = ROOT / 'asm/us/ovl_b'
    data_dir = owndata.artifact_dir(target_dir)
    present, previous = data_dir in score._own_data, score._own_data.get(data_dir)
    score.ASM_DIR = target_dir
    score._own_data[data_dir] = None if image is None else owndata.ImageData.from_image(image, 0x8038A400)
    try:
        yield
    finally:
        score.ASM_DIR = old_dir
        if present:
            score._own_data[data_dir] = previous
        else:
            score._own_data.pop(data_dir, None)


@contextmanager
def cwd(path):
    old = Path.cwd()
    os.chdir(path)
    try:
        yield
    finally:
        os.chdir(old)


def inputs():
    image = image_bytes()
    with bound_targets(image):
        manifest = score.target_manifest()
        extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        extent = next(e for e in extents['functions'] if e['name'] == NAME)
        require(int(extent['address'], 16) == ADDRESS and extent['size'] == SIZE, 'native extent changed')
        native = packed(score.targets()[NAME])
        require(len(native) == SIZE and native == image[ADDRESS - 0x8038A400:ADDRESS - 0x8038A400 + SIZE], 'native target/image binding changed')
        literal = image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 4]
        require(literal == struct.pack('>f', 2.69), 'owned 2.69 literal changed')
        return {'sources_sha256': {name: sha((HERE / name).read_bytes()) for name in ('candidate.c', 'verify.py')},
                'population': 'ovl_b', 'target': NAME, 'extent': extent,
                'target_words_sha256': sha(native), 'baseline_source_sha256': sha((ROOT / 'cloud/work/lean/runtime_b_stunt_hud_20261006/candidate.c').read_bytes()), 'image_sha256': sha(image),
                'literal_address': hex(RODATA), 'literal_sha256': sha(literal),
                'flags': FLAGS, 'compiler': 'ido5.3'}


def inspect(path, linked=False):
    raw, secs = score._elf(path)
    h = struct.unpack_from('>16sHHIIIIIHHHHHH', raw)
    require(h[0][:6] == b'\x7fELF\x01\x02' and h[1] == (2 if linked else 1) and h[2] == 8, 'ELF type/ABI changed')
    symbols = {s['name']: s for i, sec in enumerate(secs) if sec['type'] == 2
               for s in score._symbol_table(raw, secs, i) if s['name']}
    ti = score._text_index(secs)
    functions = {n for n, s in symbols.items() if s['type'] == 2 and s['section'] not in (0, 0xfff1)}
    require(functions == {NAME}, 'defined function set changed')
    fn = symbols[NAME]
    require((fn['value'], fn['size'], fn['section']) == (ADDRESS if linked else 0, SIZE, ti), 'complete function placement changed')
    owned = {}
    for i, sec in enumerate(secs):
        sh = struct.unpack_from('>IIIIIIIIII', raw, h[6] + i * h[11])
        if linked:
            require(sec['type'] not in (4, 9) or not sec['size'], 'linked relocations remain')
        if sh[2] & 2 and sec['size']:
            if sec['name'] == '.text':
                require((sh[3], sec['size']) == (ADDRESS if linked else 0, SIZE), 'text section placement changed')
            elif sec['name'] == '.rodata':
                require((sh[3], sec['size']) == (RODATA if linked else 0, 16), 'rodata section placement changed')
            else:
                require(not linked and sec['name'] in ('.reginfo', '.options'), 'unexpected allocated section: ' + sec['name'])
            if sec['name'] in ('.text', '.rodata'):
                owned[sec['name']] = raw[sec['off']:sec['off'] + sec['size']]
    require(set(owned) == {'.text', '.rodata'}, 'missing text/rodata')
    require(len(owned['.text']) == SIZE, 'nonzero/excess function tail')
    require(owned['.rodata'][4:] == b'\0' * 12, 'nonzero/excess owned-literal tail')
    return raw, secs, symbols, owned


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'requires recognized stock IDO 5.3')
    ld = shutil.which('mips-linux-gnu-ld')
    require(ld is not None, 'GNU MIPS linker missing')
    image = image_bytes()
    with bound_targets(image), tempfile.TemporaryDirectory(prefix='dot-b-stunt-') as temp:
        tmp = Path(temp)
        obj = tmp / 'callback.o'
        with cwd(tmp):
            score.compile_single(HERE / 'candidate.c', FLAGS, obj)
        result = score.compare(obj, NAME, show=0)
        require(result.differing == 2 and result.total == 468 and not result.accepted() and not any((result.extra_words, result.unresolved, result.unverified, result.errors)), result.summary())
        raw, sections, symbols, owned = inspect(obj)
        native = packed(score.targets()[NAME])
        literal = image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 4]
        require(owned['.rodata'][:4] == literal, 'complete owned literal differs')
        addresses = score.image_symbols()
        undefined = {n: addresses.get(n, score.address_named(n)) for n, s in symbols.items() if s['section'] == 0}
        require(all(v is not None for v in undefined.values()), 'external binding changed')
        relocs = []
        for sec in sections:
            if sec['type'] != 9:
                continue
            require(sec['info'] == score._text_index(sections), 'unexpected relocated section')
            names = score._symbol_table(raw, sections, sec['link'])
            for at in range(sec['off'], sec['off'] + sec['size'], 8):
                offset, info = struct.unpack_from('>II', raw, at)
                target = names[info >> 8]
                name = target['name'] or sections[target['section']]['name']
                require(offset % 4 == 0 and 0 <= offset < SIZE and info & 255 in (4, 5, 6), 'invalid relocation')
                relocs.append({'offset': offset, 'kind': info & 255, 'symbol': name})
        require(len(relocs) == 86, 'relocation count changed')
        script = tmp / 'native.ld'
        script.write_text('\n'.join('%s = 0x%08x;' % x for x in sorted(undefined.items())) +
            '\nSECTIONS { .text 0x80392894 : SUBALIGN(4) { *(.text) } .rodata 0x80394F50 : SUBALIGN(4) { *(.rodata) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n')
        linked = tmp / 'linked.elf'
        subprocess.run([ld, '-EB', '-T', str(script), '-o', str(linked), str(obj)], check=True, capture_output=True)
        _, _, ls, lo = inspect(linked, True)
        differences = [i for i in range(0, SIZE, 4) if lo['.text'][i:i+4] != native[i:i+4]]
        require(differences == [0x398, 0x39C] and lo['.rodata'][:4] == literal, 'GNU full body/owned-data residual changed')
        require(lo['.text'][0x398:0x39C] == native[0x39C:0x3A0] and lo['.text'][0x39C:0x3A0] == native[0x398:0x39C], 'residual is not the exact two-word swap')
        require(not [n for n, s in ls.items() if s['section'] == 0], 'GNU unresolved symbol')
        require(all(ls[n]['value'] == a and ls[n]['section'] == 0xfff1 for n, a in undefined.items()), 'GNU external binding changed')
        with bound_targets(None):
            absent = score.compare(obj, NAME, show=0)
            require(not absent.accepted() and absent.unverified, 'missing own-data accepted')
        corrupt = bytearray(image)
        corrupt[RODATA - 0x8038A400] ^= 1
        with bound_targets(bytes(corrupt)):
            bad_data = score.compare(obj, NAME, show=0)
            require(not bad_data.accepted() and bad_data.errors, 'corrupt literal accepted')
        controls = {}
        for label, source, expected in (
            ('inherited_baseline', (ROOT / 'cloud/work/lean/runtime_b_stunt_hud_20261006/candidate.c').read_text(), 28),
            ('without_unrecovered_locals', (HERE / 'candidate.c').read_text().replace('    s32 unrecovered_local_0, unrecovered_local_1;\n', ''), 10)):
            control = tmp / (label + '.c')
            control.write_text(source)
            control_obj = tmp / (label + '.o')
            with cwd(tmp):
                score.compile_single(control, FLAGS, control_obj)
            prior = score.compare(control_obj, NAME, show=0)
            require(prior.differing == expected and not any((prior.extra_words, prior.errors, prior.unresolved, prior.unverified)), label + ' changed: ' + prior.summary())
            controls[label] = asdict(prior)
        return {'inputs': binding, 'comparison': asdict(result), 'notes': list(result.notes),
                'function_bytes': SIZE, 'text_section_bytes': SIZE, 'owned_data_bytes': 4,
                'rodata_section_bytes': 16, 'text_zero_padding': 0, 'rodata_zero_padding': 12,
                'gnu_text_vma': hex(ADDRESS), 'gnu_rodata_vma': hex(RODATA),
                'gnu_function_sha256': sha(lo['.text']), 'gnu_literal_sha256': sha(lo['.rodata'][:4]),
                'gnu_differing_offsets': differences, 'residual': 'two-word instruction-order swap',
                'external_bindings': undefined, 'relocations': relocs,
                'controls': dict(controls, missing_data='rejected', corrupt_literal='rejected'),
                'claims': [], 'new_strict_verified_bytes': 0, 'preserved_or_integrated_bytes': 0,
                'new_image_landed_bytes': 0, 'new_rom_verified_bytes': 0}


def verify():
    expected = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == expected['inputs'], 'input binding changed')
    actual = replay()
    require(actual == expected, 'receipt differs')
    return actual


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    parser.add_argument('--score', action='store_true')
    args = parser.parse_args()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(), indent=2) + '\n')
        print('Recorded full 1872-byte NONMATCH and four-byte owned-data receipt')
    elif args.inputs_only:
        require(inputs() == json.loads((HERE / 'verification.json').read_text())['inputs'], 'input binding changed')
        print('PASS: source and selected input bindings')
    elif args.score:
        with bound_targets(image_bytes()), tempfile.TemporaryDirectory() as tmp:
            obj = Path(tmp) / 'callback.o'
            with cwd(tmp):
                score.compile_single(HERE / 'candidate.c', FLAGS, obj)
            result = score.compare(obj, NAME, show=0)
            print(NAME + ': ' + result.summary())
            for note in result.notes:
                print(note)
    else:
        verify()
        print('PASS: 2/468-word NONMATCH; full GNU extents; 86 relocations; owned data; baseline and frame controls')
