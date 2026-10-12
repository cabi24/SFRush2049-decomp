#!/usr/bin/env python3
"""Input-bound stock IDO/GNU proof for the complete image-A C93C body.

No static ROM TU is rebuilt: this is the stock IDO 5.3 overlay group route.
ROM-TU experiments must use tools.conveyor.compilers.tu_cc instead.
"""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
NAME, ADDRESS, SIZE = 'func_8038C93C', 0x8038C93C, 936
RODATA = 0x803B930C
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
IMAGE_HASH = '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers
spec = importlib.util.spec_from_file_location('c93c_elf', HERE / 'elf.py')
elf = importlib.util.module_from_spec(spec)
spec.loader.exec_module(elf)
require = elf.require


def sha(raw):
    return hashlib.sha256(raw).hexdigest()


def pack(words):
    return struct.pack('>%dI' % len(words), *words)


def image():
    raw = (ROOT / 'assets/us/data.bin').read_bytes()
    require(len(raw) == 12418096 and sha(raw) == ASSET_HASH, 'asset identity')
    require(int.from_bytes(raw[0x3850:0x3854], 'big') == 0xB5C534, 'loader pointer')
    data = zlib.decompress(raw[0xB5C534 - 0x283D0:], -15)
    require(len(data) == 194128 and sha(data) == IMAGE_HASH, 'image A identity')
    return data


@contextmanager
def target_scope():
    previous = score.ASM_DIR
    score.ASM_DIR = ROOT / 'asm/us/ovl_a'
    key = owndata.artifact_dir(score.ASM_DIR)
    absent = object()
    old = score._own_data.get(key, absent)
    try:
        data = image()
        score._own_data[key] = owndata.ImageData.from_image(data, 0x8038A400)
        yield data
    finally:
        score.ASM_DIR = previous
        if old is absent:
            score._own_data.pop(key, None)
        else:
            score._own_data[key] = old


def inputs():
    with target_scope() as data:
        manifest = score.target_manifest()
        metadata = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        recipe = json.loads((HERE / 'group.json').read_text())
        require(recipe['members'] == recipe['claims'] == [NAME], 'exact member/claim set')
        require(recipe['keep'] == ['func_8038DFEC'], 'genuine root only')
        require(not recipe.get('allow_unverified'), 'unverified claims prohibited')
        words = score.targets()
        names = recipe['members'] + recipe['context']
        native = {}
        for name in names:
            row = next(row for row in metadata['functions'] if row['name'] == name)
            require(row['size'] == len(words[name]) * 4, 'native extent')
            native[name] = dict(extent=row, words_sha256=sha(pack(words[name])))
        require(native[NAME]['extent']['size'] == SIZE, 'claim size')
        require(metadata['image'] == 'A' and metadata['image_sha256'] == IMAGE_HASH, 'wrong population')
        symbols = score.image_symbols()
        tokens = set(re.findall(r'\b[A-Za-z_]\w*\b', (HERE / 'group.c').read_text()))
        references = {name: symbols.get(name, score.address_named(name)) for name in sorted(tokens)
                      if name in symbols or score.address_named(name) is not None}
        return dict(protocol_version=2, compiler='ido5.3', population='ovl_a', recipe=recipe,
                    files={name: sha((HERE / name).read_bytes()) for name in
                           ('group.c', 'group.json', 'verify.py', 'elf.py')},
                    targets=native, references=references, asset_sha256=ASSET_HASH,
                    image_sha256=IMAGE_HASH, owned_data=dict(address=hex(RODATA), bytes=32,
                    sha256=sha(data[RODATA-0x8038A400:RODATA-0x8038A400+32])))


def shell(*args):
    result = subprocess.run(list(map(str, args)), capture_output=True, text=True)
    require(result.returncode == 0, (args, result.stdout, result.stderr))
    return result.stdout


def gnu(obj, directory, native, data):
    sections, symbols, rels = elf.read(obj)
    text_index, text_row, text = sections['.text']
    start, size, typ, section = symbols[NAME]
    require(size == SIZE and typ == 2 and section == text_index, 'full ELF function extent')
    next_start = min((value[0] for value in symbols.values()
                      if value[2] == 2 and value[3] == text_index and value[0] > start), default=len(text))
    require(not any(text[start+SIZE:next_start]), 'nonzero extra function words')
    require({name for name, (_, row, raw) in sections.items()
             if row[2] & 2 and row[5]} == {'.text', '.rodata', '.reginfo'}, 'unexpected allocated section')
    ro = sections['.rodata'][2]
    require(len(ro) == 32 and ro == data[RODATA-0x8038A400:RODATA-0x8038A400+32], 'all owned literals')
    relevant = [r for r in rels if r[3] == text_index and start <= r[0] < start+SIZE]
    require(all(r[1] in (4, 5, 6) for r in relevant), 'unknown member relocation')
    addresses = score.image_symbols()
    externs = {}
    for name, value in symbols.items():
        if value[3] != 0:
            continue
        address = addresses.get(name, score.address_named(name))
        require(address is not None, 'unresolved external ' + name)
        externs[name] = address
    script = directory / 'whole.ld'
    script.write_text('SECTIONS { .text 0x%X : SUBALIGN(4) { *(.text) }\n'
                      '.rodata 0x%X : SUBALIGN(4) { *(.rodata) }\n'
                      '/DISCARD/ : { *(.options) *(.reginfo) *(.mdebug) *(.pdr) *(.MIPS.abiflags) *(.gnu.attributes) } }\n'
                      % (ADDRESS-start, RODATA) + ''.join('%s = 0x%X;\n' % pair for pair in sorted(externs.items())))
    linked = directory / 'whole.elf'
    shell('mips-linux-gnu-ld', '-EB', '-T', script, '-o', linked, obj)
    linked_sections, linked_symbols, linked_rels = elf.read(linked)
    require(not linked_rels, 'GNU residual relocations')
    linked_index, linked_row, linked_text = linked_sections['.text']
    require(linked_row[3] == ADDRESS-start and linked_symbols[NAME] == (ADDRESS, SIZE, 2, linked_index), 'GNU address/extent')
    actual = linked_text[start:start+SIZE]
    require(actual == pack(native), 'full GNU/native words differ')
    require(linked_sections['.rodata'][1][3] == RODATA and linked_sections['.rodata'][2] == ro, 'GNU owned data')
    # Only this complete body is a placement claim. Context is linked at an
    # arbitrary diagnostic base; no original whole-TU layout is asserted.
    return dict(member_relocations=len(relevant), linked_words_sha256=sha(actual),
                full_extent=SIZE, owned_rodata_bytes=len(ro), rodata_sha256=sha(ro))


def caller_audit(obj):
    native = score.targets()['func_8038DFEC']
    direct_native = sum(word >> 26 == 3 and ((word & 0x3FFFFFF) << 2 | 0x80000000) == ADDRESS for word in native)
    sections, symbols, rels = elf.read(obj)
    start, size, typ, index = symbols['func_8038DFEC']
    words = list(struct.unpack('>%dI' % (size//4), sections['.text'][2][start:start+size]))
    all_words = score.text_words(obj)
    resolved, masks, unresolved, unverified, errors = score.relocate(obj, all_words, start, start+size, score.image_symbols())
    require(not any((masks, unresolved, unverified, errors)), 'caller callsite resolution')
    calls = sum(word >> 26 == 3 and ((word & 0x3FFFFFF) << 2 | 0x80000000) == ADDRESS for word in resolved[start//4:(start+size)//4])
    require(direct_native == calls == 1, 'genuine caller edge')
    first_call = next(i for i, word in enumerate(words) if word >> 26 == 3)
    prologue = words[:first_call]
    saved_int = sorted({(word >> 16) & 31 for word in prologue if word >> 26 == 43 and (word >> 21) & 31 == 29})
    saved_fp = sorted({(word >> 16) & 31 for word in prologue if word >> 26 == 61 and (word >> 21) & 31 == 29})
    require(set(range(16,24)) | {30,31} <= set(saved_int), 'root must preserve private integer clobbers')
    require({20,22,24,26} <= set(saved_fp), 'root must preserve private float clobbers')
    callee = score.targets()[NAME]
    require(callee[0] == 0x27BDFFB8, 'native 72-byte private frame')
    return dict(native_direct_calls=direct_native, compiled_direct_calls=calls,
                compiled_root_saved_integer=saved_int, compiled_root_saved_fp=saved_fp,
                private_callee_frame=72, source_arguments=0,
                context_matches=False)


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'stock IDO 5.3 required')
    with target_scope() as data, tempfile.TemporaryDirectory(prefix='c93c-verify-') as tmp:
        directory = Path(tmp)
        obj = directory / 'group.o'
        score.compile_group(HERE, obj)
        result = score.compare(obj, NAME, show=0)
        require(result.accepted(), result.summary())
        require(result.notes == ('own .rodata verified at 0x803B930C..0x803B932C',), 'owned literal placement')
        proof = gnu(obj, directory, score.targets()[NAME], data)
        abi = caller_audit(obj)
        controls = {}
        for label, old, new in [('wrong_literal', '75.1f;', '76.1f;'),
                                ('merged_literal_web', '55.20f;', '55.2f;')]:
            control = directory / label
            control.mkdir()
            source = (HERE / 'group.c').read_text()
            require(old in source, 'control token missing')
            (control / 'group.c').write_text(source.replace(old, new))
            shutil.copyfile(HERE / 'group.json', control / 'group.json')
            mutant = control / 'group.o'
            score.compile_group(control, mutant)
            comparison = score.compare(mutant, NAME, show=0)
            require(not comparison.accepted(), 'bad control accepted: ' + label)
            controls[label] = dict(rejected=True, differing=comparison.differing,
                                   unverified_relocations=len(comparison.unverified),
                                   owned_data_failures=len(comparison.errors),
                                   extra_words=comparison.extra_words)
        return dict(inputs=binding, comparison=asdict(result), owned_data_notes=list(result.notes),
                    gnu=proof, caller_abi=abi, negative_controls=controls,
                    new_candidate_bytes=SIZE, preserved_bytes=0, accepted_image_bytes=0)


def verify(inputs_only=False):
    receipt = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == receipt['inputs'], 'input binding changed')
    if inputs_only:
        return {'status': 'INPUTS PASS', 'compiled': False}
    actual = replay()
    require(actual == receipt, 'replay drift')
    return actual


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    args = parser.parse_args()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(), indent=2) + '\n')
    else:
        verify(args.inputs_only)
    if args.inputs_only and not args.record:
        print('INPUTS PASS: authenticated source, recipe and native inputs; compiler/GNU checks not run')
    else:
        print('PASS: C93C 234/234 words, 936-byte extent, 32 owned literal bytes, stock IDO/GNU and genuine caller ABI')
