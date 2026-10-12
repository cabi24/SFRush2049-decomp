#!/usr/bin/env python3
"""Input-bound complete image-B nearest-steering match and owned-data proof."""
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
NAME, ADDRESS, SIZE = 'func_8038DDDC', 0x8038DDDC, 684
RODATA, FLAGS = 0x80394DE4, '-g0 -O3 -mips2 -G 0 -non_shared'
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
        next_extent = min(int(e['address'], 16) for e in extents['functions'] if int(e['address'], 16) > ADDRESS)
        require(next_extent == ADDRESS + SIZE == 0x8038E088, 'native next boundary changed')
        native = packed(score.targets()[NAME])
        require(len(native) == SIZE and native == image[ADDRESS - 0x8038A400:ADDRESS - 0x8038A400 + SIZE], 'native target/image binding changed')
        literal = image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 16]
        require(literal == struct.pack('>4f', -0.04, 0.04, 0.03, -0.03), 'owned steering literals changed')
        return {'sources_sha256': {name: sha((HERE / name).read_bytes()) for name in ('group.c', 'group.json', 'verify.py')},
                'population': 'ovl_b', 'target': NAME, 'extent': extent,
                'target_words_sha256': sha(native), 'baseline_source_sha256': sha((ROOT / 'cloud/work/lean/runtime_b_nearest_steering_20261006/candidate.c').read_bytes()), 'image_sha256': sha(image),
                'literal_address': hex(RODATA), 'literal_sha256': sha(literal),
                'flags': FLAGS, 'compiler': 'ido5.3'}


def inspect(path, linked=False):
    raw, secs = score._elf(path)
    h = struct.unpack_from('>16sHHIIIIIHHHHHH', raw)
    require(h[0][:8] == b'\x7fELF\x01\x02\x01\x00' and h[0][8] == 0 and h[1] == (2 if linked else 1) and h[2] == 8 and h[7] == 0x10000000, 'ELF type/ABI changed: ' + repr((h[0][:9], h[1], h[2], hex(h[7]))))
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
                require((sh[3], sec['size']) == (ADDRESS if linked else 0, SIZE + 4), 'text section placement changed')
            elif sec['name'] == '.rodata':
                require((sh[3], sec['size']) == (RODATA if linked else 0, 16), 'rodata section placement changed')
            else:
                require(not linked and sec['name'] in ('.reginfo', '.options'), 'unexpected allocated section: ' + sec['name'])
            if sec['name'] in ('.text', '.rodata'):
                owned[sec['name']] = raw[sec['off']:sec['off'] + sec['size']]
    require(set(owned) == {'.text', '.rodata'}, 'missing text/rodata')
    require(len(owned['.text']) == SIZE + 4 and owned['.text'][SIZE:] == b'\0' * 4, 'nonzero/excess function tail')
    require(len(owned['.rodata']) == 16, 'owned-literal extent changed')
    return raw, secs, symbols, owned


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'requires recognized stock IDO 5.3')
    ld = shutil.which('mips-linux-gnu-ld')
    require(ld is not None, 'GNU MIPS linker missing')
    image = image_bytes()
    with bound_targets(image), tempfile.TemporaryDirectory(prefix='dot-b-steering-') as temp:
        tmp = Path(temp)
        obj = tmp / 'steering.o'
        spec = score.compile_group(HERE, obj)
        require(spec['members'] == spec['claims'] == spec['keep'] == [NAME] and spec['context'] == [], 'group contract changed')
        result = score.compare(obj, NAME, show=0)
        require(result.accepted() and result.total == 171, result.summary())
        raw, sections, symbols, owned = inspect(obj)
        native = packed(score.targets()[NAME])
        literal = image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 16]
        require(owned['.rodata'] == literal, 'complete owned literal differs')
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
        require(len(relocs) == 23, 'relocation count changed')
        script = tmp / 'native.ld'
        script.write_text('\n'.join('%s = 0x%08x;' % x for x in sorted(undefined.items())) +
            '\nSECTIONS { .text 0x8038DDDC : SUBALIGN(4) { *(.text) } .rodata 0x80394DE4 : SUBALIGN(4) { *(.rodata) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n')
        linked = tmp / 'linked.elf'
        subprocess.run([ld, '-EB', '--hash-style=sysv', '-T', str(script), '-o', str(linked), str(obj)], check=True, capture_output=True)
        _, _, ls, lo = inspect(linked, True)
        differences = [i for i in range(0, SIZE, 4) if lo['.text'][i:i+4] != native[i:i+4]]
        require(differences == [] and lo['.rodata'] == literal, 'GNU full body/owned-data mismatch')
        require(not [n for n, s in ls.items() if s['section'] == 0], 'GNU unresolved symbol')
        require(all(ls[n]['value'] == a and ls[n]['section'] == 0xfff1 for n, a in undefined.items()), 'GNU external binding changed')
        with bound_targets(None):
            absent = score.compare(obj, NAME, show=0)
            require(not absent.accepted() and absent.unverified, 'missing own-data accepted')
        corruption_controls = []
        for offset in range(0, 16, 4):
            corrupt = bytearray(image)
            corrupt[RODATA - 0x8038A400 + offset] ^= 1
            with bound_targets(bytes(corrupt)):
                bad_data = score.compare(obj, NAME, show=0)
                require(not bad_data.accepted() and bad_data.errors, 'corrupt literal accepted')
                corruption_controls.append({'offset': offset, 'rejected': True})
        baseline = ROOT / 'cloud/work/lean/runtime_b_nearest_steering_20261006/candidate.c'
        control_obj = tmp / 'baseline.o'
        with cwd(tmp):
            score.compile_single(baseline, FLAGS, control_obj)
        prior = score.compare(control_obj, NAME, show=0)
        require(prior.differing == 14 and not any((prior.extra_words, prior.errors, prior.unresolved, prior.unverified)), 'baseline changed: ' + prior.summary())
        _, _, _, control_sections = inspect(control_obj)
        control_linked = tmp / 'baseline.elf'
        subprocess.run([ld, '-EB', '--hash-style=sysv', '-T', str(script), '-o', str(control_linked), str(control_obj)], check=True, capture_output=True)
        _, _, _, baseline_owned = inspect(control_linked, True)
        expected_offsets = [0x8C,0xF4,0x100,0x110,0x124,0x128,0x12C,0x158,0x1B4,0x1B8,0x1BC,0x1EC,0x22C,0x238]
        baseline_diff = [at for at in range(0, SIZE, 4) if baseline_owned['.text'][at:at+4] != native[at:at+4]]
        require(baseline_diff == expected_offsets and baseline_owned['.rodata'] == literal, 'GNU baseline residual changed')
        for at in baseline_diff:
            original, changed = struct.unpack_from('>I', native, at)[0], struct.unpack_from('>I', baseline_owned['.text'], at)[0]
            require(original >> 16 == changed >> 16 and (changed & 65535) == (original & 65535) + 4, 'baseline is not a four-byte home residual')
        # Stock trace is a diagnostic switch, not a compiler modification.
        stock_obj, traced_obj = tmp / 'stock.o', tmp / 'traced.o'
        with cwd(tmp):
            score.compile_single(HERE / 'group.c', FLAGS, stock_obj)
            score.compile_single(HERE / 'group.c', FLAGS + ' -Wa,-R', traced_obj)
        require(stock_obj.read_bytes() == traced_obj.read_bytes(), 'stock trace changes object')
        single = score.compare(stock_obj, NAME, show=0)
        require(single.accepted(), 'standalone control differs')
        return {'inputs': binding, 'comparison': asdict(result), 'notes': list(result.notes),
                'function_bytes': SIZE, 'text_section_bytes': SIZE + 4, 'owned_data_bytes': 16,
                'rodata_section_bytes': 16, 'text_zero_padding': 4, 'rodata_zero_padding': 0,
                'gnu_text_vma': hex(ADDRESS), 'gnu_rodata_vma': hex(RODATA),
                'gnu_function_sha256': sha(lo['.text'][:SIZE]), 'gnu_literal_sha256': sha(lo['.rodata']),
                'gnu_differing_offsets': differences, 'baseline_gnu_differing_offsets': baseline_diff,
                'external_bindings': undefined, 'relocations': relocs,
                'controls': {'inherited_baseline': asdict(prior), 'baseline_home_delta_bytes': 4, 'missing_data': 'rejected', 'corrupt_literals': corruption_controls, 'stock_as1_trace': 'whole-object identical', 'standalone': asdict(single)},
                'claims': [NAME], 'new_strict_verified_bytes': SIZE, 'preserved_or_integrated_bytes': 0,
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
        print('Recorded full 684-byte MATCH and 16-byte owned-data receipt')
    elif args.inputs_only:
        require(inputs() == json.loads((HERE / 'verification.json').read_text())['inputs'], 'input binding changed')
        print('PASS: source and selected input bindings')
    elif args.score:
        with bound_targets(image_bytes()):
            sys.argv = ['score.py', 'group', str(HERE), '--targets', str(ROOT / 'asm/us/ovl_b')]
            raise SystemExit(score.main())
    else:
        verify()
        print('PASS: 171/171 words, complete GNU function/data extents, 23 relocations, 14 baseline home controls, four corrupt literals rejected')
