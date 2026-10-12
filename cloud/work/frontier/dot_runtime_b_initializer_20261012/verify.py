#!/usr/bin/env python3
"""Input-bound whole-function/GNU proof for the image-B initializer."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
SOURCE = ROOT / 'cloud/matches/ovl_b/func_803908D0.c'
NAME, ADDRESS, SIZE = 'func_803908D0', 0x803908D0, 560
FLAGS = '-g0 -O2 -mips2 -G 0 -non_shared'
sys.path.insert(0, str(ROOT))
from tools.cloud import score
from tools.conveyor import compilers


def require(condition, message):
    if not condition:
        raise ValueError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


@contextmanager
def targets():
    previous = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_b'
    try:
        yield
    finally:
        score.ASM_DIR = previous


@contextmanager
def in_directory(path):
    previous = Path.cwd()
    os.chdir(path)
    try:
        yield
    finally:
        os.chdir(previous)


def inputs():
    with targets():
        manifest = score.target_manifest()
        extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        extent = next(e for e in extents['functions'] if e['name'] == NAME)
        require(extent['address'] == '0x803908D0' and extent['size'] == SIZE, 'target extent changed')
        words = score.targets()[NAME]
        require(len(words) * 4 == SIZE and words[0] & 65535 == 65536 - 48, 'target extent/frame changed')
        return {'source_sha256': digest(SOURCE.read_bytes()),
                'verifier_sha256': digest(Path(__file__).read_bytes()),
                'target_words_sha256': digest(packed(words)),
                'extent': extent, 'image': extents['image'],
                'image_sha256': extents['image_sha256'], 'flags': FLAGS,
                'compiler': 'ido5.3', 'population': 'ovl_b'}


def inspect(path, linked=False):
    raw, sections = score._elf(path)
    h = struct.unpack_from('>16sHHIIIIIHHHHHH', raw)
    require(h[0][:6] == b'\x7fELF\x01\x02' and h[2] == 8, 'expected ELF32 big-endian MIPS')
    require(h[1] == (2 if linked else 1), 'unexpected ELF type')
    ti = score._text_index(sections)
    symbols = {s['name']: s for i, sec in enumerate(sections) if sec['type'] == 2
               for s in score._symbol_table(raw, sections, i) if s['name']}
    defined = {n for n, s in symbols.items() if s['type'] == 2 and s['section'] not in (0, 0xfff1)}
    require(defined == {NAME}, 'unexpected function set')
    fn = symbols[NAME]
    require((fn['value'], fn['size'], fn['section']) == (ADDRESS if linked else 0, SIZE, ti),
            'full function extent/address changed')
    for i, sec in enumerate(sections):
        sh = struct.unpack_from('>IIIIIIIIII', raw, h[6] + i * h[11])
        if sh[2] & 2 and sec['size']:
            require(sec['name'] == '.text' or
                    (not linked and sec['name'] == '.reginfo' and sec['size'] == 24),
                    'unexpected allocated/owned data: ' + sec['name'])
        if linked:
            require(sec['type'] not in (4, 9) or not sec['size'], 'linked relocations remain')
        if i == ti:
            require(sh[3] == (ADDRESS if linked else 0), 'text VMA changed')
    sec = sections[ti]
    text = raw[sec['off']:sec['off'] + sec['size']]
    require(len(text) == SIZE, 'unexpected text tail, padding, or missing words')
    return raw, sections, symbols, text


def pointer_control():
    source = SOURCE.read_text()
    source = source.replace('    char **name;\n', '    char **name;\n    char **texture_name;\n    s16 *texture;\n', 1)
    for names, handles in [('D_80394330', 'D_80399AF8'), ('D_80394344', 'D_80399B08')]:
        old = 'for (i = 0; i < 5; i++) {\n        func_800B24EC(%s[i], &%s[i], 0, D_80140BDC - 1, 1);' % (names, handles)
        new = ('for (texture_name = %s, texture = %s; texture < %s + 5; texture++, texture_name++) {\n'
               '        func_800B24EC(*texture_name, texture, 0, D_80140BDC - 1, 1);') % (names, handles, handles)
        require(source.count(old) == 1, 'indexed loop control binding changed')
        source = source.replace(old, new, 1)
    return source


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'requires recognized stock IDO 5.3')
    linker = shutil.which('mips-linux-gnu-ld')
    require(linker is not None, 'GNU MIPS linker missing')
    with targets(), tempfile.TemporaryDirectory(prefix='b-initializer-') as directory:
        temporary = Path(directory)
        obj = temporary / 'initializer.o'
        with in_directory(temporary):
            score.compile_single(SOURCE, FLAGS, obj)
        result = score.compare(obj, NAME, show=0)
        require(result.accepted(), result.summary())
        raw, sections, symbols, text = inspect(obj)
        addresses = score.image_symbols()
        undefined = {n: addresses.get(n, score.address_named(n)) for n, s in symbols.items() if s['section'] == 0}
        require(len(undefined) == 23 and all(v is not None for v in undefined.values()), 'external bindings changed')
        resolved, masks, u, v, e = score.relocate(obj, score.text_words(obj), 0, SIZE, addresses)
        require(not any((masks, u, v, e)), 'masked/unresolved/unverified relocation')
        native = score.targets()[NAME]
        require(resolved == native, 'full relocated text mismatch')
        relocs = []
        for sec in sections:
            if sec['type'] != 9:
                continue
            require(sec['info'] == score._text_index(sections), 'relocations outside text')
            names = score._symbol_table(raw, sections, sec['link'])
            for at in range(sec['off'], sec['off'] + sec['size'], 8):
                offset, info = struct.unpack_from('>II', raw, at)
                require(offset % 4 == 0 and 0 <= offset < SIZE and info & 255 in (4, 5, 6), 'invalid relocation')
                relocs.append({'offset': offset, 'kind': info & 255, 'symbol': names[info >> 8]['name']})
        require(len(relocs) == 61 and {r['symbol'] for r in relocs} == set(undefined), 'relocation coverage changed')
        script = temporary / 'native.ld'
        script.write_text('\n'.join('%s = 0x%08x;' % item for item in sorted(undefined.items())) +
                          '\nSECTIONS { .text 0x803908D0 : SUBALIGN(4) { *(.text) } '
                          '/DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n')
        linked = temporary / 'linked.elf'
        subprocess.run([linker, '-EB', '-T', str(script), '-o', str(linked), str(obj)],
                       check=True, capture_output=True)
        _, _, linked_symbols, linked_text = inspect(linked, True)
        require(linked_text == packed(native), 'GNU exact-VMA full text mismatch')
        require(not [n for n, s in linked_symbols.items() if s['section'] == 0], 'GNU undefined symbols remain')
        for name, address in undefined.items():
            require(linked_symbols[name]['value'] == address and linked_symbols[name]['section'] == 0xfff1,
                    'GNU binding changed: ' + name)
        control = temporary / 'pointer_cursor.c'
        control.write_text(pointer_control())
        control_obj = temporary / 'control.o'
        score.compile_single(control, FLAGS, control_obj)
        inspect(control_obj)
        c = score.compare(control_obj, NAME, show=0)
        require(c.differing == 6 and c.total == 140 and not any((c.extra_words, c.unresolved, c.unverified, c.errors)),
                'six-word control changed')
        cw, cm, cu, cv, ce = score.relocate(control_obj, score.text_words(control_obj), 0, SIZE, addresses)
        require(not any((cm, cu, cv, ce)), 'control relocation uncertainty')
        differences = [i * 4 for i, word in enumerate(native) if cw[i] != word]
        require(differences == [0x15c, 0x160, 0x170, 0x1ac, 0x1b0, 0x1c0], 'control mismatch sites changed')
        # Inherited count qualification is documented as noncausal: verify it.
        plain = temporary / 'ordinary_count.c'
        plain.write_text(SOURCE.read_text().replace('extern volatile u8 D_80140BDC;', 'extern u8 D_80140BDC;', 1))
        plain_obj = temporary / 'plain.o'
        score.compile_single(plain, FLAGS, plain_obj)
        require(score.compare(plain_obj, NAME, show=0).accepted(), 'count qualifier became causal')
        return {'inputs': binding, 'comparison': asdict(result), 'complete_function_bytes': SIZE,
                'text_bytes': len(text), 'owned_data_bytes': 0, 'gnu_exact_vma': hex(ADDRESS),
                'gnu_linked_text_sha256': digest(linked_text), 'external_bindings': undefined,
                'relocations': relocs, 'pointer_control': {'differing_words': 6, 'byte_offsets': differences},
                'ordinary_count_control': 'MATCH', 'new_strict_verified_bytes': SIZE,
                'preserved_or_integrated_bytes': 0, 'new_image_landed_bytes': 0, 'new_rom_verified_bytes': 0}


def trace():
    metadata = {}
    with targets(), tempfile.TemporaryDirectory(prefix='b-initializer-trace-') as directory:
        temporary = Path(directory)
        control = temporary / 'pointer_cursor.c'
        control.write_text(pointer_control())
        for name, source in [('indexed', SOURCE), ('pointer', control)]:
            folder = temporary / name
            folder.mkdir()
            with in_directory(folder):
                ordinary, diagnostic = folder / 'ordinary.o', folder / 'diagnostic.o'
                score.compile_single(source, FLAGS, ordinary)
                score.compile_single(source, FLAGS + ' -Wab,-R -Wc,-e,trees,-l,listing', diagnostic)
                require(ordinary.read_bytes() == diagnostic.read_bytes(), 'stock diagnostics changed whole object')
                listing = (folder / 'listing').read_text()
                count = len(re.findall(r'^\s*\.noalias\s+\$17,\$sp\s*$', listing, re.M))
                metadata[name] = {'whole_object_identical': True, 'name_cursor_noalias_regions': count,
                                  'differing_words': score.compare(ordinary, NAME, show=0).differing}
        require(metadata['indexed']['name_cursor_noalias_regions'] == 2 and
                metadata['pointer']['name_cursor_noalias_regions'] == 0, 'alias trace changed')
    return metadata


def verify():
    expected = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == expected['inputs'], 'input binding changed')
    actual = replay()
    require(actual == expected, 'verification receipt differs')
    return actual


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--trace', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    args = parser.parse_args()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(), indent=2) + '\n')
        print('Recorded input-bound 560-byte MATCH receipt')
    elif args.trace:
        print(json.dumps(trace(), indent=2))
    elif args.inputs_only:
        require(inputs() == json.loads((HERE / 'verification.json').read_text())['inputs'], 'input binding changed')
        print('PASS: source, verifier and selected native-input binding')
    else:
        verify()
        print('PASS: 140/140 words; 560-byte GNU exact-VMA proof; 61 relocations; six-word control')
