#!/usr/bin/env python3
"""Strict stock-IDO/GNU proof for the complete image-A settings-values callback."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
import re
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP = HERE / 'groups/dot_runtime_a_settings_values_20261012'
BASELINE = ROOT / 'cloud/work/lean/runtime_a_settings_values_20261006/candidate.c'
NAME = 'func_803AECE4'
ENTRY, SIZE, DATA, DATA_SIZE = 0x803AECE4, 2656, 0x803B99E0, 108
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'
ASSET_SHA = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers


def require(ok, message):
    if not ok:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def pack(words):
    return struct.pack('>%dI' % len(words), *words)


@contextmanager
def population(name='ovl_a', image=None):
    previous = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us' / name
    key = owndata.artifact_dir(score.ASM_DIR)
    sentinel = object()
    saved = score._own_data.get(key, sentinel)
    score._own_data[key] = image
    try:
        yield
    finally:
        score.ASM_DIR = previous
        if saved is sentinel:
            score._own_data.pop(key, None)
        else:
            score._own_data[key] = saved


def authenticate():
    with population():
        manifest = score.target_manifest()
        meta = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(len(asset) == 12418096 and sha(asset) == ASSET_SHA, 'asset identity')
    offset = int(meta['rom_offset'], 16)
    require(int.from_bytes(asset[0x3850:0x3854], 'big') == offset == 0xB5C534,
            'loader pointer')
    decoder = zlib.decompressobj(-15)
    image = decoder.decompress(asset[offset - 0x283D0:]) + decoder.flush()
    require(decoder.eof and len(image) == meta['size'] and sha(image) == meta['image_sha256'],
            'image identity')
    return image, int(meta['base'], 16)


def inputs():
    image, image_base = authenticate()
    callback_slot = 0x803B7CC8
    require(struct.unpack_from('>I', image, callback_slot-image_base)[0] == ENTRY,
            'native callback slot changed')
    paths = [GROUP / 'settings_values.c', GROUP / 'group.json', BASELINE,
             HERE / 'verify.py', HERE / 'trace.json']
    bindings = {}
    for pop, names in [('ovl_a', [NAME]), ('blob', [
            'render_helper', 'object_create', 'slot_state_setup', 'dispatch_handler', 'func_800B669C',
            'brake_light_update', 'sound_pitch_diff_halved', 'state_utility',
            'fcvt_wrapper', 'func_800ED66C', 'func_800BEA3C',
            'sound_control', 'UpdateActiveObjects'])]:
        with population(pop):
            manifest = score.target_manifest()
            words, symbols = score.targets(), score.image_symbols()
            for name in names:
                body = pack(words[name])
                bindings[pop + ':' + name] = {'address': hex(symbols[name]),
                    'bytes': len(body), 'sha256': sha(body)}
            if pop == 'ovl_a':
                meta = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
                row = next(r for r in meta['functions'] if r['name'] == NAME)
                require(int(row['address'], 16) == ENTRY and row['size'] == SIZE,
                        'complete target extent')
                require(min(int(r['address'],16) for r in meta['functions']
                        if int(r['address'],16)>ENTRY) == ENTRY + SIZE, 'next native boundary')
                table = score.image_symbols()
                tokens = set(re.findall(r'\b[A-Za-z_]\w*\b', (GROUP/'settings_values.c').read_text()))
                anchors = {n:hex(table.get(n,score.address_named(n))) for n in sorted(tokens)
                           if table.get(n,score.address_named(n)) is not None}
    return {'sources': {str(p.relative_to(ROOT)): sha(p.read_bytes()) for p in paths},
            'targets': bindings, 'asset_sha256': ASSET_SHA,
            'image_sha256': meta['image_sha256'], 'symbols': anchors,
            'native_next_boundary': hex(ENTRY+SIZE),
            'callback_slot': {'address':hex(callback_slot),'target':hex(ENTRY)}}


def symbols(obj):
    return [s for i, sec in enumerate(obj.sections) if sec['type'] == 2
            for s in obj.symbols(i)]


def section_header(obj, index):
    offset = struct.unpack_from('>I', obj.data, 32)[0]
    stride = struct.unpack_from('>H', obj.data, 46)[0]
    return struct.unpack_from('>10I', obj.data, offset + stride * index)


def run(*args):
    p = subprocess.run(list(map(str, args)), text=True, capture_output=True)
    require(p.returncode == 0, p.stdout + p.stderr)


def gnu_proof(path, temp, image, base):
    obj = owndata._Object(path)
    ti = next(i for i, s in enumerate(obj.sections) if s['name'] == '.text')
    ri = next(i for i, s in enumerate(obj.sections) if s['name'] == '.rodata')
    funcs = [s for s in symbols(obj) if s['type'] == 2 and s['section'] != 0]
    require(len(funcs) == 1 and (funcs[0]['name'], funcs[0]['value'], funcs[0]['size'],
            funcs[0]['section']) == (NAME, 0, SIZE, ti), 'complete object function')
    require(len(obj.raw(ti)) == SIZE, 'text extent/padding')
    literals = image[DATA-base:DATA-base+DATA_SIZE]
    require(len(obj.raw(ri)) == 112 and obj.raw(ri)[108:] == bytes(4), 'owned extent/padding')
    relocations = obj.relocations(ti)
    require(len(relocations) == 145 and all(0 <= o < SIZE and o % 4 == 0 and t in (4,5,6)
            for o,t,s in relocations), 'complete relocation extent/type')
    data_relocations = obj.relocations(ri)
    require(len(data_relocations) == 21 and all(24 <= o < 108 and o % 4 == 0 and t == 2
            for o,t,s in data_relocations), 'complete table relocation extent/type')
    for i, sec in enumerate(obj.sections):
        require(not (section_header(obj, i)[2] & 2) or not sec['size'] or
                sec['name'] in ('.text', '.rodata', '.reginfo', '.options'),
                'unexpected allocated section')
    table = score.image_symbols()
    anchors = {s['name']: table.get(s['name'], score.address_named(s['name']))
               for s in symbols(obj) if s['section'] == 0 and s['name']}
    require(all(v is not None for v in anchors.values()), 'unresolved external symbol')
    script = temp / 'link.ld'
    script.write_text('SECTIONS {\n.text 0x%X : SUBALIGN(4) { *(.text) }\n'
        '.rodata 0x%X : SUBALIGN(4) { *(.rodata) }\n'
        '/DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.gnu.attributes) }\n}\n'
        % (ENTRY, DATA) + ''.join('%s = 0x%X;\n' % pair for pair in sorted(anchors.items())))
    linked = temp / 'linked.elf'
    run('mips-linux-gnu-ld', '-EB', '-T', script, '-o', linked, path)
    elf = owndata._Object(linked)
    require(not any(s['type'] in (4,9) for s in elf.sections), 'remaining GNU relocations')
    expected = {'.text': (ENTRY, pack(score.targets()[NAME])),
                '.rodata': (DATA, literals + bytes(4))}
    found = set()
    for i, sec in enumerate(elf.sections):
        if section_header(elf, i)[2] & 2 and sec['size']:
            require(sec['name'] in expected, 'unexpected linked section')
            addr, content = expected[sec['name']]
            require(section_header(elf, i)[3] == addr and elf.raw(i) == content,
                    'GNU full bytes/placement: ' + sec['name'])
            found.add(sec['name'])
    require(found == set(expected), 'missing linked section')
    f = next(s for s in symbols(elf) if s['name'] == NAME)
    require((f['value'], f['size'], f['type']) == (ENTRY, SIZE, 2), 'linked function extent')
    return {'function_bytes': SIZE, 'text_padding_bytes': 0, 'text_relocations': 145, 'data_relocations': 21,
            'native_sha256': sha(expected['.text'][1][:SIZE]),
            'owned_data': {'address': hex(DATA), 'bytes': DATA_SIZE, 'padding_bytes': 4,
                           'sha256': sha(literals)},
            'anchors': {k:hex(v) for k,v in sorted(anchors.items())}}


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'stock IDO 5.3 required')
    spec = json.loads((GROUP / 'group.json').read_text())
    require(spec['members'] == spec['claims'] == spec['keep'] == [NAME]
            and spec['context'] == [] and spec['files'] == ['settings_values.c'], 'claims/ABI recipe')
    image, base = authenticate()
    with tempfile.TemporaryDirectory(prefix='settings-values-') as work, population(
            image=owndata.ImageData.from_image(image, base)):
        temp = Path(work)
        obj, old = temp / 'group.o', temp / 'baseline.o'
        score.compile_group(GROUP, obj)
        actual = score.compare(obj, NAME, show=0)
        require(actual.accepted(), actual.summary())
        baseline_source = temp / 'settings_values_home_baseline.c'
        baseline_source.write_text(BASELINE.read_text())
        score.compile_single(baseline_source, FLAGS, old)
        baseline = score.compare(old, NAME, show=0)
        require(baseline.differing == 270 and not any((baseline.errors, baseline.unresolved,
                baseline.unverified, baseline.extra_words)), 'baseline changed')
        gnu = gnu_proof(obj, temp, image, base)
        compiler = {n:sha(Path(score.ido(n)).read_bytes()) for n in
                    ['cc','cfe','uld','usplit','umerge','uopt','ugen','as1']}
        key = owndata.artifact_dir(score.ASM_DIR)
        score._own_data[key] = None
        missing = score.compare(obj, NAME, show=0)
        require(len(missing.unverified) == 14 and not missing.accepted(), 'missing-data gate')
        corrupt = bytearray(image)
        corrupt[DATA-base] ^= 1
        score._own_data[key] = owndata.ImageData.from_image(corrupt, base)
        bad = score.compare(obj, NAME, show=0)
        require(bad.errors and not bad.accepted(), 'corrupt-literal gate')
        corrupt = bytearray(image)
        corrupt[DATA-base+24] ^= 1
        score._own_data[key] = owndata.ImageData.from_image(corrupt, base)
        bad_table = score.compare(obj, NAME, show=0)
        require(bad_table.errors and not bad_table.accepted(), 'corrupt-table gate')
        return {'inputs': binding, 'comparison': asdict(actual), 'notes': list(actual.notes),
                'baseline': asdict(baseline), 'gnu': gnu, 'compiler_binary_sha256':compiler,
                'new_match_bytes': SIZE, 'preserved_match_bytes': 0,
                'accepted_image_bytes': 0, 'rom_bytes': 0,
                'controls': {'missing_data_unverified': 14, 'corrupt_literal_rejected': True, 'corrupt_table_rejected': True}}


def verify(inputs_only=False):
    expected = json.loads((HERE / 'verification.json').read_text())
    actual = inputs() if inputs_only else replay()
    require(actual == (expected['inputs'] if inputs_only else expected), 'input-bound receipt differs')
    return actual


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--record', action='store_true')
    p.add_argument('--inputs-only', action='store_true')
    p.add_argument('--score', action='store_true')
    args = p.parse_args()
    if args.score:
        data, base = authenticate()
        old = sys.argv
        try:
            sys.argv = ['score.py','group',str(GROUP),'--targets',str(ROOT/'asm/us/ovl_a')]
            with population(image=owndata.ImageData.from_image(data, base)):
                return score.main()
        finally:
            sys.argv = old
    if args.record:
        (HERE/'verification.json').write_text(json.dumps(replay(), indent=2) + '\n')
    else:
        verify(args.inputs_only)
    print('PASS: settings-values input proof' if args.inputs_only else
          'PASS: A:803AECE4 MATCH, 664 words / 2656 bytes; complete GNU/data/extent proof')
    return 0


if __name__ == '__main__':
    sys.exit(main())
