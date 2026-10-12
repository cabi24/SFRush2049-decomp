#!/usr/bin/env python3
"""Full image-B position initializer and owned-literal proof, with no native dumps."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
import os
import re
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
NAME, ADDRESS, SIZE = 'func_8038D054', 0x8038D054, 340
RODATA, FLAGS = 0x80394DAC, '-g0 -O3 -mips2 -G 0 -non_shared'
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers
from tools.conveyor.pipeline import blob_group, ovl_splice


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
        literal = image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 12]
        require(literal == struct.pack('>3f', 0.1, 0.35, 0.025), 'owned literal pool changed')
        inherited_path = ROOT / 'cloud/work/lean/runtime_b_position_init_20261006/candidate.c'
        inherited = inherited_path.read_text()
        clean = lambda source: re.sub(r'/\*.*?\*/', '', source, flags=re.S).strip()
        require(clean((HERE / 'group.c').read_text()) == clean(inherited), 'inherited executable source changed')
        caller = score.targets()['func_8038FCE0']
        require(caller[0x8e4 // 4] >> 26 == 9 and ((caller[0x8e4 // 4] >> 21) & 31) == 17 and
                ((caller[0x8e4 // 4] >> 16) & 31) == 5 and (caller[0x8e4 // 4] & 0xffff) == 32,
                'real caller position pointer setup changed')
        move = caller[0x8e8 // 4]
        require(move >> 26 == 0 and ((move >> 21) & 31) == 5 and ((move >> 16) & 31) == 0 and
                ((move >> 11) & 31) == 6 and (move & 63) == 37, 'real caller second position copy changed')
        call = caller[0xb00 // 4]
        require(call >> 26 == 3 and ((call & 0x3ffffff) << 2 | 0x80000000) == ADDRESS, 'real caller target changed')
        home = int.from_bytes(native[4:8], 'big')
        require(home >> 26 == 43 and ((home >> 21) & 31) == 29 and ((home >> 16) & 31) == 6 and
                (home & 0xffff) == 40, 'genuine unused third parameter home changed')
        return {'inherited_source_sha256': sha(inherited.encode()),
                'native_caller_sha256': sha(packed(caller)),
                'caller_argument_setup_offset': '0x8e4', 'caller_call_offset': '0xb00',
                'sources_sha256': {name: sha((HERE / name).read_bytes()) for name in ('group.c', 'group.json', 'verify.py', 'host_test.c')},
                'population': 'ovl_b', 'target': NAME, 'extent': extent,
                'target_words_sha256': sha(native), 'image_sha256': sha(image),
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
                require((sh[3], sec['size']) == (ADDRESS if linked else 0, 352), 'text section placement changed')
            elif sec['name'] == '.rodata':
                require((sh[3], sec['size']) == (RODATA if linked else 0, 16), 'rodata section placement changed')
            else:
                require(not linked and sec['name'] in ('.reginfo', '.options'), 'unexpected allocated section: ' + sec['name'])
            if sec['name'] in ('.text', '.rodata'):
                owned[sec['name']] = raw[sec['off']:sec['off'] + sec['size']]
    require(set(owned) == {'.text', '.rodata'}, 'missing text/rodata')
    require(owned['.text'][SIZE:] == b'\0' * 12, 'nonzero/excess function tail')
    require(owned['.rodata'][12:] == b'\0' * 4, 'nonzero/excess owned-literal tail')
    return raw, secs, symbols, owned


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'requires recognized stock IDO 5.3')
    ld, host = shutil.which('mips-linux-gnu-ld'), shutil.which('cc')
    require(ld is not None and host is not None, 'GNU MIPS linker or host C compiler missing')
    image = image_bytes()
    with bound_targets(image), tempfile.TemporaryDirectory(prefix='dot-b-position-init-') as temp:
        tmp = Path(temp)
        obj = tmp / 'position.o'
        spec = score.compile_group(HERE, obj)
        require(spec['members'] == spec['claims'] == spec['keep'] == [NAME] and spec['context'] == [], 'group contract changed')
        result = score.compare(obj, NAME, show=0)
        require(result.accepted(), result.summary())
        raw, sections, symbols, owned = inspect(obj)
        native = packed(score.targets()[NAME])
        literal = image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 12]
        require(owned['.rodata'][:12] == literal, 'complete owned literal differs')
        addresses = score.image_symbols()
        undefined = {n: addresses.get(n, score.address_named(n)) for n, s in symbols.items() if s['section'] == 0}
        require(set(undefined) == {'D_80395EE8', 'func_8008E3C0'} and all(v is not None for v in undefined.values()), 'external binding changed')
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
        require(len(relocs) == 9, 'relocation count changed')
        script = tmp / 'native.ld'
        script.write_text('\n'.join('%s = 0x%08x;' % x for x in sorted(undefined.items())) +
            '\nSECTIONS { .text 0x8038D054 : SUBALIGN(4) { *(.text) } .rodata 0x80394DAC : SUBALIGN(4) { *(.rodata) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n')
        linked = tmp / 'linked.elf'
        subprocess.run([ld, '-EB', '-T', str(script), '-o', str(linked), str(obj)], check=True, capture_output=True)
        _, _, ls, lo = inspect(linked, True)
        require(lo['.text'][:SIZE] == native and lo['.rodata'][:12] == literal, 'GNU full body/owned-data mismatch')
        require(not [n for n, s in ls.items() if s['section'] == 0], 'GNU unresolved symbol')
        require(all(ls[n]['value'] == a and ls[n]['section'] == 0xfff1 for n, a in undefined.items()), 'GNU external binding changed')
        with bound_targets(None):
            absent = score.compare(obj, NAME, show=0)
            require(not absent.accepted() and absent.unverified, 'missing own-data accepted')
        installed = ovl_splice.packet_spec(HERE)
        require(installed['members'] == installed['keep'] == [NAME] and installed['context'] == [], 'installer member contract changed')
        slices, text_ndx = blob_group.member_slices(obj, [NAME], {NAME: {'vaddr': ADDRESS, 'size': SIZE}})
        require(slices == {NAME: (0, ADDRESS, SIZE)}, 'installer slice changed')
        bodies = blob_group.relocate(obj, slices, text_ndx, addresses,
                                     members=[NAME], image=(image, 0x8038A400))
        require(bodies == {NAME: native}, 'installer full-body/data relocation differs')
        for literal_offset in (0, 4, 8):
            corrupt = bytearray(image)
            corrupt[RODATA - 0x8038A400 + literal_offset] ^= 1
            with bound_targets(bytes(corrupt)):
                bad_data = score.compare(obj, NAME, show=0)
                require(not bad_data.accepted() and bad_data.errors, 'corrupt literal accepted')
            try:
                blob_group.relocate(obj, slices, text_ndx, addresses,
                                    members=[NAME], image=(bytes(corrupt), 0x8038A400))
            except blob_group.GroupError:
                pass
            else:
                raise ValueError('installer accepted corrupt owned literal')
        control = tmp / 'wrong_kind.c'
        control.write_text((HERE / 'group.c').read_text().replace('effect->kind = 4;', 'effect->kind = 5;'))
        control_obj = tmp / 'control.o'
        with cwd(tmp):
            score.compile_single(control, FLAGS, control_obj)
        prior = score.compare(control_obj, NAME, show=0)
        require(prior.differing > 0 and not prior.accepted(), 'wrong mode flag accepted')
        layout = tmp / 'layout.c'
        layout.write_text('#include <stddef.h>\n#include "group.c"\n' +
            '\n'.join('typedef char check_%s[(%s) ? 1 : -1];' % (n, e) for n, e in {
                'size': 'sizeof(Effect) == 64', 'kind': 'offsetof(Effect, kind) == 8',
                'lifetime': 'offsetof(Effect, lifetime) == 12', 'origin': 'offsetof(Effect, origin) == 16',
                'previous': 'offsetof(Effect, previous) == 28', 'lower': 'offsetof(Effect, lower) == 40',
                'upper': 'offsetof(Effect, upper) == 52'}.items()))
        subprocess.run([host, '-m32', '-std=c89', '-fsyntax-only', '-I', str(HERE), str(layout)], check=True, capture_output=True)
        exe = tmp / 'host_test'
        subprocess.run([host, '-std=c89', '-O2', '-fstrict-aliasing', '-Wall', '-Wextra', '-Werror', '-Wno-unused-parameter', '-Wno-maybe-uninitialized', '-fsanitize=undefined,bounds', '-I', str(HERE), str(HERE / 'host_test.c'), '-o', str(exe)], check=True, capture_output=True)
        host_result = subprocess.check_output([str(exe)], text=True).strip()
        require(host_result == 'PASS: 240 source fixtures including null allocation and overlapping positions', 'host fixture result changed')
        return {'inputs': binding, 'comparison': asdict(result), 'notes': list(result.notes),
                'function_bytes': SIZE, 'text_section_bytes': 352, 'owned_data_bytes': 12,
                'rodata_section_bytes': 16, 'text_zero_padding': 12, 'rodata_zero_padding': 4,
                'gnu_text_vma': hex(ADDRESS), 'gnu_rodata_vma': hex(RODATA),
                'gnu_function_sha256': sha(lo['.text'][:SIZE]), 'gnu_literal_sha256': sha(lo['.rodata'][:12]),
                'external_bindings': undefined, 'relocations': relocs,
                'controls': {'wrong_kind_differing_words': prior.differing, 'installer': 'exact', 'installer_corrupt_literal': 'all three rejected', 'target_layout': '32-bit offsets exact', 'missing_data': 'rejected', 'corrupt_literal': 'all three rejected', 'host': host_result},
                'new_strict_verified_bytes': 0, 'inherited_strict_verified_bytes': SIZE, 'preserved_or_integrated_bytes': 0,
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
        print('Recorded inherited 340-byte strict MATCH and 12-byte owned-data receipt')
    elif args.inputs_only:
        require(inputs() == json.loads((HERE / 'verification.json').read_text())['inputs'], 'input binding changed')
        print('PASS: source and selected input bindings')
    elif args.score:
        with bound_targets(image_bytes()):
            sys.argv = ['score.py', 'group', str(HERE), '--targets', str(ROOT / 'asm/us/ovl_b')]
            raise SystemExit(score.main())
    else:
        verify()
        print('PASS: inherited 85/85 words; GNU and installer extents/data; nine relocations; negative controls; 240 host fixtures')
