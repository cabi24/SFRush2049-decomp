#!/usr/bin/env python3
"""Input-bound complete image-B trail-callback match and owned-data proof."""
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
NAME, ADDRESS, SIZE = 'func_8038CB20', 0x8038CB20, 1332
RODATA, FLAGS = 0x80394D90, '-g0 -O3 -mips2 -G 0 -non_shared'
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
        require(next_extent == ADDRESS + SIZE == 0x8038D054, 'native next boundary changed')
        native = packed(score.targets()[NAME])
        require(len(native) == SIZE and native == image[ADDRESS - 0x8038A400:ADDRESS - 0x8038A400 + SIZE], 'native target/image binding changed')
        literal = image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 28]
        require(literal == struct.pack('>7f', 0.0333333, 0.2, 0.0333333, 0.025, 0.35, 0.1, 0.16), 'owned trail literals changed')
        old_dir = score.ASM_DIR
        try:
            score.ASM_DIR = ROOT / 'asm/us/blob'
            helpers = {name: {'size': len(score.targets()[name]) * 4,
                             'words_sha256': sha(packed(score.targets()[name]))}
                       for name in ('func_8008D0C0', 'func_800AFA84', 'func_8008E3C0',
                                    'func_800A78BC', 'func_8008C074')}
        finally:
            score.ASM_DIR = old_dir
        return {'helper_targets': helpers,
                'test_sha256': sha((ROOT / 'tests/cloud/test_dot_runtime_b_trail_20261012.py').read_bytes()),
                'sources_sha256': {name: sha((HERE / name).read_bytes()) for name in ('group.c', 'group.json', 'verify.py')},
                'population': 'ovl_b', 'target': NAME, 'extent': extent,
                'target_words_sha256': sha(native), 'baseline_source_sha256': sha((ROOT / 'cloud/work/lean/runtime_b_trail_update_20261006/candidate.c').read_bytes()), 'image_sha256': sha(image),
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
                require((sh[3], sec['size']) == (ADDRESS if linked else 0, SIZE + 12), 'text section placement changed')
            elif sec['name'] == '.rodata':
                require((sh[3], sec['size']) == (RODATA if linked else 0, 32), 'rodata section placement changed')
            else:
                require(not linked and sec['name'] in ('.reginfo', '.options'), 'unexpected allocated section: ' + sec['name'])
            if sec['name'] in ('.text', '.rodata'):
                owned[sec['name']] = raw[sec['off']:sec['off'] + sec['size']]
    require(set(owned) == {'.text', '.rodata'}, 'missing text/rodata')
    require(len(owned['.text']) == SIZE + 12 and owned['.text'][SIZE:] == b'\0' * 12, 'nonzero/excess function tail')
    require(len(owned['.rodata']) == 32 and owned['.rodata'][28:] == b'\0' * 4, 'owned-literal extent changed')
    return raw, secs, symbols, owned


def frame_diagnostic(tmp, source, tag):
    work = tmp / tag; work.mkdir()
    (work / 'group.c').write_text(source)
    (work / 'keep.txt').write_text(NAME + '\n')
    common = ['-mips2', '-EB', '-g0', '-O3']
    def run(tool, *args):
        return subprocess.run([score.ido(tool), *args], cwd=work, text=True,
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True)
    run('cc', '-j', *FLAGS.split(), 'group.c')
    run('uld', '-L/usr/lib/mips2/nonshared', '-_SYSTYPE_SVR4', '-mips2', '-non_shared',
        '-g0', '-no_AutoGnum', '-kp', 'keep.txt', 'group.u', '-ko', 'linked')
    run('usplit', '-mips2', '-o', 'split', '-t', 'st', 'linked')
    run('umerge', '-Olimit', '5000', *common, 'split', '-o', 'merged', '-t', 'st')
    run('uopt', '-G', '0', '-Olimit', '5000', *common, 'merged', 'opt', '-t', 'st', 'optlog')
    before = (work / 'st').read_bytes()
    accounting = None
    for diagnostic in (False, True):
        (work / 'st').write_bytes(before)
        result = run('ugen', '-G', '0', *common, 'opt', '-o', 'gen', '-t', 'st',
                     '-temp', 'ugtmp', *(['-d'] if diagnostic else []))
        run('as1', '-elf', '-G', '0', '-p0', *common, score.R4300_AS1,
            '-Olimit', '5000', 'gen', '-o', 'on.o' if diagnostic else 'off.o', '-t', 'st')
        if diagnostic:
            accounting = dict((kind, int(size)) for kind, size in re.findall(
                r'def mtype=([MTP]) length=(\d+)', result.stderr))
    require((work / 'on.o').read_bytes() == (work / 'off.o').read_bytes(), 'UGEN diagnostic changes object')
    return accounting


def layout_check(tmp):
    source = '#include "' + str(HERE / 'group.c') + '"\n'
    source += '#define OFF(T, F) ((unsigned int)&((T *)0)->F)\n'
    facts = [('pointer_width', 'sizeof(void *) == 4'), ('scalar_width', 'sizeof(s32) == 4 && sizeof(f32) == 4'),
             ('trail_size', 'sizeof(Trail) == 64'), ('fade_size', 'sizeof(Fade) == 28'),
             ('trail_origin', 'OFF(Trail, origin) == 16'), ('trail_previous', 'OFF(Trail, previous) == 28'),
             ('trail_lower', 'OFF(Trail, lower) == 40'), ('trail_upper', 'OFF(Trail, upper) == 52'),
             ('fade_object', 'OFF(Fade, object) == 8'), ('fade_interval', 'OFF(Fade, interval) == 16'),
             ('fade_color', 'OFF(Fade, color) == 20'), ('fade_source', 'OFF(Fade, source) == 24'),
             ('fade_pool_active', 'OFF(FadePool, active) == 16'), ('trail_pool_active', 'OFF(TrailPool, active) == 16')]
    source += ''.join('typedef char check_%s[(%s) ? 1 : -1];\n' % fact for fact in facts)
    p = tmp / 'layout.c'; p.write_text(source)
    with cwd(tmp):
        score.compile_single(p, FLAGS, tmp / 'layout.o')
    return dict((name, expression) for name, expression in facts)


def replay():
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'requires recognized stock IDO 5.3')
    ld = shutil.which('mips-linux-gnu-ld')
    require(ld is not None, 'GNU MIPS linker missing')
    image = image_bytes()
    with bound_targets(image), tempfile.TemporaryDirectory(prefix='dot-b-trail-') as temp:
        tmp = Path(temp)
        obj = tmp / 'trail.o'
        spec = score.compile_group(HERE, obj)
        require(spec['members'] == spec['claims'] == spec['keep'] == [NAME] and spec['context'] == [], 'group contract changed')
        result = score.compare(obj, NAME, show=0)
        require(result.accepted() and result.total == 333, result.summary())
        raw, sections, symbols, owned = inspect(obj)
        native = packed(score.targets()[NAME])
        literal = image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 28]
        require(owned['.rodata'][:28] == literal, 'complete owned literal differs')
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
        require(len(relocs) == 35, 'relocation count changed: ' + str(len(relocs)))
        script = tmp / 'native.ld'
        script.write_text('\n'.join('%s = 0x%08x;' % x for x in sorted(undefined.items())) +
            '\nSECTIONS { .text 0x8038CB20 : SUBALIGN(4) { *(.text) } .rodata 0x80394D90 : SUBALIGN(4) { *(.rodata) } /DISCARD/ : { *(.reginfo) *(.options) *(.MIPS.abiflags) } }\n')
        linked = tmp / 'linked.elf'
        subprocess.run([ld, '-EB', '--hash-style=sysv', '-T', str(script), '-o', str(linked), str(obj)], check=True, capture_output=True)
        _, _, ls, lo = inspect(linked, True)
        differences = [i for i in range(0, SIZE, 4) if lo['.text'][i:i+4] != native[i:i+4]]
        require(differences == [] and lo['.rodata'][:28] == literal, 'GNU full body/owned-data mismatch')
        require(not [n for n, s in ls.items() if s['section'] == 0], 'GNU unresolved symbol')
        require(all(ls[n]['value'] == a and ls[n]['section'] == 0xfff1 for n, a in undefined.items()), 'GNU external binding changed')
        with bound_targets(None):
            absent = score.compare(obj, NAME, show=0)
            require(not absent.accepted() and absent.unverified, 'missing own-data accepted')
        corruption_controls = []
        for offset in range(0, 28, 4):
            corrupt = bytearray(image)
            corrupt[RODATA - 0x8038A400 + offset] ^= 1
            with bound_targets(bytes(corrupt)):
                bad_data = score.compare(obj, NAME, show=0)
                require(not bad_data.accepted() and bad_data.errors, 'corrupt literal accepted')
                corruption_controls.append({'offset': offset, 'rejected': True})
        baseline = ROOT / 'cloud/work/lean/runtime_b_trail_update_20261006/candidate.c'
        control_obj = tmp / 'baseline.o'
        with cwd(tmp):
            score.compile_single(baseline, FLAGS, control_obj)
        prior = score.compare(control_obj, NAME, show=0)
        require(prior.differing == 54 and not any((prior.extra_words, prior.errors, prior.unresolved, prior.unverified)), 'baseline changed: ' + prior.summary())
        _, _, _, control_sections = inspect(control_obj)
        control_linked = tmp / 'baseline.elf'
        subprocess.run([ld, '-EB', '--hash-style=sysv', '-T', str(script), '-o', str(control_linked), str(control_obj)], check=True, capture_output=True)
        _, _, _, baseline_owned = inspect(control_linked, True)
        baseline_diff = [at for at in range(0, SIZE, 4) if baseline_owned['.text'][at:at+4] != native[at:at+4]]
        require(len(baseline_diff) == 54 and baseline_owned['.rodata'][:28] == literal, 'GNU baseline residual changed')
        controls = {}
        final_source = (HERE / 'group.c').read_text()
        forms = {
            'no_unused_locals': final_source.replace('    s32 unused_local_0, unused_local_1;\n', ''),
            'no_pool_read': final_source.replace('        if (D_80396B80.active) {}\n', ''),
        }
        for label, source in forms.items():
            p = tmp / (label + '.c'); p.write_text(source)
            o = tmp / (label + '.o')
            with cwd(tmp):
                score.compile_single(p, FLAGS, o)
            r = score.compare(o, NAME, show=0)
            linked_control = tmp / (label + '.elf')
            subprocess.run([ld, '-EB', '--hash-style=sysv', '-T', str(script), '-o', str(linked_control), str(o)], check=True, capture_output=True)
            _, _, _, control_sections = inspect(linked_control, True)
            difference_offsets = [at for at in range(0, SIZE, 4) if control_sections['.text'][at:at+4] != native[at:at+4]]
            require(not any((r.errors, r.unresolved, r.unverified, r.extra_words)), label + ': unverified control')
            if label == 'no_unused_locals':
                require(r.differing == 2 and difference_offsets == [0, 0x530], 'unused-local residual is not frame-only')
                require(struct.unpack_from('>I', control_sections['.text'], 0)[0] & 65535 == 65536-216, 'control frame changed')
            else:
                require(r.differing == 20 and len(difference_offsets) == 20, 'pool-read residual changed')
            controls[label] = {'comparison': asdict(r), 'gnu_differing_offsets': difference_offsets}
        native_layout = layout_check(tmp)
        accounting = frame_diagnostic(tmp, final_source, 'native_frame')
        short_accounting = frame_diagnostic(tmp, forms['no_unused_locals'], 'short_frame')
        require(accounting == {'M': 100, 'T': 28, 'P': 4}, 'native frame accounting changed')
        require(short_accounting == {'M': 92, 'T': 28, 'P': 4}, 'short frame accounting changed')
        # Stock trace is a diagnostic switch, not a compiler modification.
        stock_obj, traced_obj = tmp / 'stock.o', tmp / 'traced.o'
        with cwd(tmp):
            score.compile_single(HERE / 'group.c', FLAGS, stock_obj)
            score.compile_single(HERE / 'group.c', FLAGS + ' -Wa,-R', traced_obj)
        require(stock_obj.read_bytes() == traced_obj.read_bytes(), 'stock trace changes object')
        single = score.compare(stock_obj, NAME, show=0)
        require(single.accepted(), 'standalone control differs')
        return {'inputs': binding, 'comparison': asdict(result), 'notes': list(result.notes),
                'function_bytes': SIZE, 'text_section_bytes': SIZE + 12, 'owned_data_bytes': 28,
                'rodata_section_bytes': 32, 'text_zero_padding': 12, 'rodata_zero_padding': 4,
                'gnu_text_vma': hex(ADDRESS), 'gnu_rodata_vma': hex(RODATA),
                'gnu_function_sha256': sha(lo['.text'][:SIZE]), 'gnu_literal_sha256': sha(lo['.rodata']),
                'gnu_differing_offsets': differences, 'baseline_gnu_differing_offsets': baseline_diff,
                'external_bindings': undefined, 'relocations': relocs,
                'native_layout': native_layout, 'frame_accounting': {'native224': accounting, 'without_two_locals216': short_accounting},
                'controls': {'inherited_baseline': asdict(prior), 'shaping_controls': controls, 'missing_data': 'rejected', 'corrupt_literals': corruption_controls, 'stock_as1_trace': 'whole-object identical', 'standalone': asdict(single)},
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
        print('Recorded full 1,332-byte MATCH and 28-byte owned-data receipt')
    elif args.inputs_only:
        require(inputs() == json.loads((HERE / 'verification.json').read_text())['inputs'], 'input binding changed')
        print('PASS: source and selected input bindings')
    elif args.score:
        with bound_targets(image_bytes()):
            sys.argv = ['score.py', 'group', str(HERE), '--targets', str(ROOT / 'asm/us/ovl_b')]
            raise SystemExit(score.main())
    else:
        verify()
        print('PASS: 333/333 words, complete GNU function/data extents, 35 relocations, baseline/frame/priority controls, seven corrupt literals rejected')
