#!/usr/bin/env python3
"""Replay one input-bound image-B match; emit metadata, never native bytes."""
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
SOURCE = ROOT / 'cloud/matches/ovl_b/func_803936A8.c'
NAME = 'func_803936A8'
ADDRESS, SIZE = 0x803936A8, 1164
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
OLD_LOCALS = '    s32 i, x, y;\n    Event *event;\n    char text[32];'
NEW_LOCALS = '    char text[32];\n    s32 i, x, y;\n    Event *event;'
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers


def require(condition, message):
    if not condition:
        raise ValueError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def word_bytes(words):
    return struct.pack('>%dI' % len(words), *words)


@contextmanager
def target_context():
    old = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_b'
    try:
        yield
    finally:
        score.ASM_DIR = old


@contextmanager
def in_directory(path):
    old = Path.cwd()
    os.chdir(path)
    try:
        yield
    finally:
        os.chdir(old)


def callback_evidence():
    # Read the already-tracked current-tree asset in memory only. No historical
    # non-ancestor Git checkout, extracted image file or network is required.
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(digest(asset) == 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8',
            'source asset identity changed')
    require(struct.unpack_from('>I', asset, 0x3854)[0] == 0xB6FEC4, 'image B loader pointer changed')
    image = zlib.decompress(asset[0xB6FEC4 - 0x283D0:], -15)
    require(len(image) == 43888 and digest(image) ==
            'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd', 'image B identity changed')
    start = 0x80393DE8 + 9 * 36
    descriptor = image[start - 0x8038A400:start - 0x8038A400 + 36]
    require(struct.unpack_from('>I', descriptor, 0)[0] == 0xffffffff, 'callback texture sentinel changed')
    require(struct.unpack_from('>I', descriptor, 28)[0] == ADDRESS, 'callback registration changed')
    native = score.targets()
    require(image[ADDRESS - 0x8038A400:ADDRESS - 0x8038A400 + SIZE] == word_bytes(native[NAME]),
            'target does not describe the authenticated image')
    setup = native['func_803925D0']
    require(((setup[0x58 // 4] & 0x3ffffff) << 2 | 0x80000000) == 0x800B37E8,
            'setup no longer calls NewMultiBlit')
    require(setup[0x54 // 4] & 65535 == 0x3de8 and setup[0x5c // 4] & 65535 == 10,
            'setup descriptor/count evidence changed')
    return {'asset_sha256': digest(asset), 'image_sha256': digest(image),
            'descriptor_address': hex(start), 'callback_slot': hex(start + 28),
            'descriptor_sha256': digest(descriptor),
            'setup_words_sha256': digest(word_bytes(setup)),
            'producer_words_sha256': digest(word_bytes(native['func_803914B4']))}


def inputs():
    with target_context():
        manifest = score.target_manifest()
        words = score.targets()[NAME]
        extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        extent = next(e for e in extents['functions'] if e['name'] == NAME)
        require(extent['address'] == hex(ADDRESS).upper().replace('0X', '0x') and extent['size'] == SIZE,
                'target extent changed')
        require(extent['evidence'] == ['data_ref', 'prologue'], 'target evidence changed')
        require(len(words) * 4 == SIZE, 'target words do not fill full extent')
        require((words[0] & 65535) == 65536 - 144, 'native frame changed')
        require((words[0x34 // 4] >> 26, (words[0x34 // 4] >> 16) & 31,
                 words[0x34 // 4] & 65535) == (43, 4, 144), 'callback argument home changed')
        require(SOURCE.read_text().count(NEW_LOCALS) == 1, 'declared local layout changed')
        return {'sources_sha256': {
                    str(SOURCE.relative_to(ROOT)): digest(SOURCE.read_bytes()),
                    str(Path(__file__).resolve().relative_to(ROOT)): digest(Path(__file__).read_bytes())},
                'flags': FLAGS, 'image': 'B', 'target': NAME, 'address': hex(ADDRESS), 'size': SIZE,
                'target_words_sha256': digest(word_bytes(words)),
                'target_metadata_sha256': {n: manifest[n] for n in ('extents.json', 'symbols.json')},
                'callback': callback_evidence()}


def elf_details(path, linked=False):
    raw, sections = score._elf(path)
    header = struct.unpack_from('>16sHHIIIIIHHHHHH', raw)
    require(header[0][:6] == b'\x7fELF\x01\x02' and header[2] == 8, 'not big-endian ELF32 MIPS')
    require(header[1] == (2 if linked else 1), 'ELF type changed')
    ti = score._text_index(sections)
    symbols = {s['name']: s for i, sec in enumerate(sections) if sec['type'] == 2
               for s in score._symbol_table(raw, sections, i) if s['name']}
    defined = {n for n, s in symbols.items() if s['type'] == 2 and s['section'] not in (0, 0xfff1)}
    require(defined == {NAME}, 'unexpected defined function set')
    sym = symbols[NAME]
    require(sym['value'] == (ADDRESS if linked else 0) and sym['size'] == SIZE and sym['section'] == ti,
            'full STT_FUNC extent/address changed')
    for i, sec in enumerate(sections):
        sh = struct.unpack_from('>IIIIIIIIII', raw, header[6] + i * header[11])
        if sh[2] & 2 and sec['size']:
            require(sec['name'] == '.text' or
                    (not linked and sec['name'] == '.reginfo' and sec['size'] == 24),
                    'unexpected allocated/owned data: ' + sec['name'])
        if linked:
            require(sec['type'] not in (4, 9) or not sec['size'], 'linked relocations remain')
        if i == ti:
            require(sh[3] == (ADDRESS if linked else 0), 'text VMA changed')
    text = sections[ti]
    data = raw[text['off']:text['off'] + text['size']]
    require(len(data) == 1168 and data[SIZE:] == b'\0' * 4, 'nonzero/excess alignment bytes')
    return raw, sections, symbols, data


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'requires canonical IDO 5.3')
    linker = shutil.which('mips-linux-gnu-ld')
    require(linker is not None, 'mips-linux-gnu binutils linker missing')
    with target_context(), tempfile.TemporaryDirectory(prefix='dot-b-consumer-') as tmp:
        tmp = Path(tmp)
        obj = tmp / 'consumer.o'
        # Isolate O3 driver's basename-derived temporary .u files.
        with in_directory(tmp):
            score.compile_single(SOURCE, FLAGS, obj)
        comparison = score.compare(obj, NAME, show=0)
        require(comparison.accepted(), comparison.summary())
        raw, sections, symbols, text = elf_details(obj)
        addresses = score.image_symbols()
        undefined = {n: addresses.get(n, score.address_named(n)) for n, s in symbols.items()
                     if s['section'] == 0}
        require(undefined and all(v is not None for v in undefined.values()), 'unresolved external symbol')
        resolved, masks, unresolved, unverified, errors = score.relocate(
            obj, score.text_words(obj), 0, SIZE, addresses)
        require(not any((masks, unresolved, unverified, errors)), 'relocation uncertainty or masked word')
        target = score.targets()[NAME]
        require(resolved[:SIZE // 4] == target, 'full relocation-resolved words differ')
        relocations = []
        for sec in sections:
            if sec['type'] != 9:
                continue
            require(sec['info'] == score._text_index(sections), 'relocation outside text')
            names = score._symbol_table(raw, sections, sec['link'])
            for at in range(sec['off'], sec['off'] + sec['size'], 8):
                offset, info = struct.unpack_from('>II', raw, at)
                require(0 <= offset < SIZE and offset % 4 == 0 and info & 255 in (4, 5, 6),
                        'unexpected relocation kind or extent')
                relocations.append({'offset': offset, 'kind': info & 255, 'symbol': names[info >> 8]['name']})
        require(len(relocations) == 62 and {r['symbol'] for r in relocations} == set(undefined),
                'relocation coverage changed')
        script = tmp / 'native.ld'
        script.write_text('\n'.join('%s = 0x%08x;' % item for item in sorted(undefined.items())) +
            '\nSECTIONS { .text 0x803936A8 : SUBALIGN(4) { *(.text) } '
            '/DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n')
        linked = tmp / 'linked.elf'
        subprocess.run([linker, '-EB', '-T', str(script), '-o', str(linked), str(obj)],
                       check=True, capture_output=True)
        _, _, linked_symbols, linked_text = elf_details(linked, True)
        require(linked_text[:SIZE] == word_bytes(target), 'GNU-linked full text differs')
        require(not [n for n, s in linked_symbols.items() if s['section'] == 0], 'GNU undefined symbols remain')
        for name, address in undefined.items():
            require(linked_symbols[name]['value'] == address and linked_symbols[name]['section'] == 0xfff1,
                    'GNU external address binding changed: ' + name)
        control_source = tmp / 'dot_event_consumer_control.c'
        control_source.write_text(SOURCE.read_text().replace(NEW_LOCALS, OLD_LOCALS, 1))
        control_obj = tmp / 'control.o'
        with in_directory(tmp):
            score.compile_single(control_source, FLAGS, control_obj)
        control = score.compare(control_obj, NAME, show=0)
        require(control.differing == 2 and control.total == 291 and not any((control.extra_words,
                control.unresolved, control.unverified, control.errors)), 'negative control changed')
        control_words, control_masks, u, v, e = score.relocate(
            control_obj, score.text_words(control_obj), 0, SIZE, addresses)
        require(not any((control_masks, u, v, e)), 'negative-control relocation uncertainty')
        indices = [i for i, word in enumerate(target) if word != control_words[i]]
        require(indices == [0x74 // 4, 0x130 // 4], 'negative control differs outside buffer homes')
        require(all((target[i] & 65535, control_words[i] & 65535) == (112, 96) for i in indices),
                'buffer-home hypothesis changed')
        require(control_words[0] == target[0], 'negative control frame changed')
        return {'inputs': binding, 'comparison': asdict(comparison), 'compiler': 'stock IDO 5.3',
                'complete_function_bytes': SIZE, 'text_bytes': len(text), 'owned_data_bytes': 0,
                'gnu_exact_vma': hex(ADDRESS), 'gnu_linked_text_sha256': digest(linked_text[:SIZE]),
                'relocations': relocations,
                'negative_control': {'differing': 2, 'byte_offsets': [0x74, 0x130],
                                     'frame_bytes': 144, 'native_buffer_offset': 112, 'old_buffer_offset': 96},
                'new_strict_verified_bytes': SIZE, 'new_image_landed_bytes': 0, 'new_rom_verified_bytes': 0}


def verify():
    expected = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == expected['inputs'], 'input binding changed')
    actual = replay()
    require(actual == expected, 'verification receipt differs')
    return actual


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    args = parser.parse_args()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(), indent=2) + '\n')
        print('Recorded input-bound MATCH receipt')
    elif args.inputs_only:
        require(inputs() == json.loads((HERE / 'verification.json').read_text())['inputs'], 'input binding changed')
        print('PASS: source, verifier, target and callback evidence bindings')
    else:
        verify()
        print('PASS: 291/291 words; 1164-byte exact-VMA GNU proof; 62 relocations; 2-word control')
