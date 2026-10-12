#!/usr/bin/env python3
"""Input-bound, stock-compiler proof for the existing option-eligibility group."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
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
sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers
from tools.conveyor.pipeline import blob_group, ovl_splice

MEMBERS = {'func_803ADDA8': 1048, 'func_803AE1C0': 860, 'func_8038F744': 88}
IMAGE_HASH = '0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667'
ASSET_HASH = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
RODATA = 0x803B99C0
FLAGS = '-g0 -O3 -mips2 -G 0 -non_shared'


def require(value, message):
    if not value:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>%dI' % len(words), *words)


@contextmanager
def target_context():
    old = score.ASM_DIR
    directory = ROOT / 'asm/us/ovl_a'
    key = owndata.artifact_dir(directory)
    present, prior = key in score._own_data, score._own_data.get(key)
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(sha(asset) == ASSET_HASH, 'current-tree asset identity changed')
    require(struct.unpack_from('>I', asset, 0x3850)[0] == 0xB5C534, 'loader pointer changed')
    image = zlib.decompress(asset[0xB5C534 - 0x283D0:], -15)
    require(len(image) == 194128 and sha(image) == IMAGE_HASH, 'image-A identity changed')
    bound_image = owndata.ImageData.from_image(image, 0x8038A400)
    try:
        score.ASM_DIR = directory
        score._own_data[key] = bound_image
        yield image
    finally:
        score.ASM_DIR = old
        if present:
            score._own_data[key] = prior
        else:
            score._own_data.pop(key, None)


def stack_census(words):
    frame = -(words[0] & 0xFFFF) + 65536
    accesses, addresses = set(), set()
    for word in words:
        op, base, reg, low = word >> 26, (word >> 21) & 31, (word >> 16) & 31, word & 65535
        off = low if low < 32768 else low - 65536
        if base == 29 and op in (0x21, 0x23, 0x29, 0x2B, 0x31, 0x39, 0x35, 0x3D):
            accesses.add((op, reg, off))
        if base == 29 and op == 9 and reg != 29:
            addresses.add(off)
    require(frame == 112, 'native frame changed')
    require(addresses == {56, 60}, 'stack-address escapes changed')
    require(accesses == {(0x2B, 31, 28), (0x23, 31, 28), (0x2B, 14, 16),
                         (0x2B, 25, 16), (0x21, 15, 58), (0x21, 18, 56),
                         (0x21, 14, 58), (0x21, 24, 58), (0x2B, 0, 108),
                         (0x23, 4, 108), (0x23, 2, 108), (0x23, 15, 108),
                         (0x2B, 2, 108), (0x2B, 4, 0), (0x2B, 5, 4)}, 'native stack access census changed')
    return {'frame_bytes': frame, 'return_address': 28, 'point_range': [56, 60],
            'text_range': [60, 92], 'position_home': 92, 'y_home': 96,
            'x_home': 100, 'j_home': 104, 'i_home': 108,
            'address_taken_offsets': sorted(addresses), 'scalar_homes_are_nonoverlapping': True}


def inputs():
    source = (HERE / 'group.c').read_text()
    group = json.loads((HERE / 'group.json').read_text())
    require(group['members'] == group['claims'] == list(MEMBERS), 'every exact body must be claimed')
    require(group.get('context', []) == [] and group['keep'] == ['func_803AE1C0'], 'group ABI roots changed')
    require(group['files'] == ['group.c'] and group['flags'] == FLAGS, 'recipe changed')
    require(source.count('char text[32];') == 1 and source.count('char text[48];') == 1, 'buffer reconstruction changed')
    dda8 = source.split('void func_803ADDA8(void)\n{', 1)[1].split('\nvoid func_803ADDA8(void);', 1)[0]
    require(dda8.count('fcvt_wrapper(text,D_803B8CDC,j+1);') == 1 and 'for(j=0;j<4;j++)' in dda8, 'bounded writer changed')
    require(len(re.findall(r'(?<!\.)\btext\b', dda8)) == 4, 'text escape/use set changed')
    with target_context() as image:
        manifest = score.target_manifest()
        extents = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
        require((extents['image'], extents['base']) == ('A', '0x8038A400'), 'wrong population')
        rows = {row['name']: row for row in extents['functions']}
        targets = {}
        for name, size in MEMBERS.items():
            body = packed(score.targets()[name])
            address = int(name[-8:], 16)
            require(rows[name]['size'] == size == len(body), 'target extent changed')
            require(image[address - 0x8038A400:address - 0x8038A400 + size] == body, 'target/image mismatch')
            targets[name] = {'image': 'A', 'address': hex(address), 'size': size, 'words_sha256': sha(body)}
        fmt = image[0x803B8CDC - 0x8038A400:].split(b'\0', 1)[0].decode('ascii')
        require(re.fullmatch('%d', fmt) is not None, 'native decimal format changed')
        return {'sources_sha256': {name: sha((HERE / name).read_bytes()) for name in ('group.c', 'group.json', 'verify.py', 'host.c', 'trace.py')},
                'targets': targets, 'flags': FLAGS, 'asset_sha256': ASSET_HASH, 'image_sha256': IMAGE_HASH,
                'format_bound': {'address': '0x803B8CDC', 'format_sha256': sha(fmt.encode() + b'\0'),
                                 'argument_range': [1, 4], 'output_bytes_including_nul': 2},
                'stack': stack_census(score.targets()['func_803ADDA8'])}


def elf(path):
    """Independent ELF32 BE reader for GNU and complete symbol/relocation checks."""
    data = path.read_bytes()
    require(data[:6] == b'\x7fELF\x01\x02' and struct.unpack_from('>H', data, 18)[0] == 8, 'not BE ELF32 MIPS')
    base = struct.unpack_from('>I', data, 32)[0]
    stride, count, names_index = struct.unpack_from('>HHH', data, 46)
    require(stride == 40, 'section stride changed')
    rows = [struct.unpack_from('>10I', data, base + i * stride) for i in range(count)]
    names_row = rows[names_index]
    names = data[names_row[4]:names_row[4] + names_row[5]]
    sections, symbols, tables, relocs = {}, {}, {}, []
    for index, row in enumerate(rows):
        name = names[row[0]:].split(b'\0')[0].decode()
        raw = data[row[4]:row[4] + row[5]]
        sections[name] = (index, row, raw)
        if row[1] == 2:
            string_row = rows[row[6]]
            strings = data[string_row[4]:string_row[4] + string_row[5]]
            table = []
            for offset in range(0, len(raw), 16):
                no, value, size, info, other, section = struct.unpack_from('>IIIBBH', raw, offset)
                name = strings[no:].split(b'\0')[0].decode()
                table.append(name)
                if name:
                    symbols[name] = (value, size, info & 15, section)
            tables[index] = table
    for row in rows:
        if row[1] in (4, 9):
            require(row[1] == 9, 'unexpected RELA')
            for offset in range(row[4], row[4] + row[5], 8):
                at, info = struct.unpack_from('>II', data, offset)
                relocs.append((at, info & 255, tables[row[6]][info >> 8], row[7]))
    return sections, symbols, relocs


def gnu_verify(obj, work, image):
    sections, symbols, relocs = elf(obj)
    ti, _, raw = sections['.text']
    lines = ['.set noreorder', '.set noat']
    externs, counts = set(), {}
    for name, size in MEMBERS.items():
        start, actual, kind, section = symbols[name]
        require((actual, kind, section) == (size, 2, ti), 'incomplete ELF body: ' + name)
        following = min((v[0] for v in symbols.values() if v[2] == 2 and v[3] == ti and v[0] > start), default=len(raw))
        require(following >= start + size and not any(raw[start + size:following]), 'extra or overlapping function: ' + name)
        words = list(struct.unpack('>%dI' % (size // 4), raw[start:start + size]))
        entries = []
        for loc, typ, symbol, dest in relocs:
            if dest != ti or not start <= loc < start + size:
                continue
            require(typ in (4, 5, 6) and loc % 4 == 0, 'unsupported relocation')
            off = loc - start
            if symbol == '.text':
                require(typ == 4, 'unexpected text address relocation')
                addend = (words[off // 4] & 0x3FFFFFF) * 4
                owners = [(n, v) for n, v in symbols.items()
                          if v[2] == 2 and v[3] == ti and v[0] <= addend < v[0] + v[1]]
                require(len(owners) == 1, 'ambiguous call target')
                symbol, owner = owners[0]
                words[off // 4] = (words[off // 4] & 0xFC000000) | ((addend - owner[0]) // 4)
            if symbol == '.rodata':
                symbol = 'packet_owned_rodata'
            require(not symbol.startswith('.'), 'unowned data relocation')
            entries.append((off, typ, symbol))
            externs.add(symbol)
        lines += ['.section .review_' + name + ',"ax",@progbits', '.balign 4', '.globl ' + name,
                  '.type ' + name + ',@function', name + ':']
        lines += ['.word 0x%08x' % word for word in words]
        lines += ['.size %s,.-%s' % (name, name)]
        lines += ['.reloc %s+%d,%s,%s' % (name, off, {4: 'R_MIPS_26', 5: 'R_MIPS_HI16', 6: 'R_MIPS_LO16'}[typ], symbol)
                  for off, typ, symbol in entries]
        counts[name] = len(entries)
    owned = sections['.rodata'][2]
    require(len(owned) == 32 and owned[20:] == b'\0' * 12, 'owned-data extent/padding changed')
    require(owned[:20] == image[RODATA - 0x8038A400:RODATA - 0x8038A400 + 20], 'all owned floats differ')
    lines += ['.section .review_owned,"a",@progbits', '.globl packet_owned_rodata', 'packet_owned_rodata:']
    lines += ['.word 0x%08x' % word for word in struct.unpack('>5I', owned[:20])]
    asm, reloc, script, linked = [work / name for name in ('review.s', 'review.o', 'review.ld', 'review.elf')]
    asm.write_text('\n'.join(lines) + '\n')
    addresses = score.image_symbols()
    assignments = []
    for name in sorted(externs - set(MEMBERS) - {'packet_owned_rodata'}):
        address = addresses.get(name, score.address_named(name))
        require(address is not None, 'unresolved GNU symbol: ' + name)
        assignments.append('%s = 0x%X;' % (name, address))
    script.write_text('SECTIONS {\n' + ''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n' % (n, n[-8:], n) for n in MEMBERS)
                      + '.review_owned 0x803B99C0 : SUBALIGN(4) { *(.review_owned) }\n'
                      + '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n' + '\n'.join(assignments))
    for command in (['mips-linux-gnu-as', '-EB', '-mips2', '-o', str(reloc), str(asm)],
                    ['mips-linux-gnu-ld', '-EB', '-T', str(script), '-o', str(linked), str(reloc)]):
        require(shutil.which(command[0]), 'GNU MIPS binutils missing: ' + command[0])
        subprocess.run(command, check=True, capture_output=True)
    ss, sy, rr = elf(linked)
    require(not rr, 'GNU relocation left unresolved')
    for name, size in MEMBERS.items():
        index, row, body = ss['.review_' + name]
        address = int(name[-8:], 16)
        require(row[3] == address and len(body) == size and sy[name] == (address, size, 2, index), 'GNU extent changed')
        require(body == packed(score.targets()[name]), 'GNU full words differ: ' + name)
    own_index, own_row, own_bytes = ss['.review_owned']
    require(own_row[3] == RODATA and sy['packet_owned_rodata'] == (RODATA, 0, 0, own_index), 'GNU owned address/anchor differs')
    require(own_bytes == owned[:20], 'GNU owned section differs')
    return {'relocations': counts, 'owned_bytes': 20, 'owned_start': hex(RODATA),
            'owned_sha256': sha(owned[:20]), 'full_extent_words_exact': True}

def controls(source):
    old_order = source.replace('    s32 i;\n    s32 j,x;\n    f32 y;\n    f32 *position;\n    char text[32];',
                              '    s32 j,x;\n    f32 y;\n    f32 *position;\n    s32 i;\n    char text[48];', 1)
    baseline = old_order.replace('    f32 *position;', '    Slot60 *slot;', 1)
    baseline = baseline.replace('position=D_803B6610[i].position;', 'slot=&D_803B6610[i];', 1)
    baseline = baseline.replace('D_803B6610[i].angle', 'slot->angle').replace('brake_light_update(0,position,', 'brake_light_update(0,slot->position,', 1)
    return {'baseline': (baseline, 6), 'position_text48': (old_order, 2)}


def replay(deep=False):
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'requires stock IDO 5.3')
    with target_context() as image, tempfile.TemporaryDirectory(prefix='dda8-packet-') as temp:
        work = Path(temp)
        obj = work / 'group.o'
        score.compile_group(HERE, obj)
        sections, symbols, relocs = elf(obj)
        defined = {name for name, value in symbols.items() if value[2] == 2 and value[3] == sections['.text'][0]}
        require(defined == set(MEMBERS), 'defined function set changed')
        results = {}
        for name, size in MEMBERS.items():
            result = score.compare(obj, name, show=0)
            require(result.accepted() and symbols[name][1:3] == (size, 2), name + ': ' + result.summary())
            results[name] = {'comparison': asdict(result), 'notes': list(result.notes)}
        outcome = {'inputs': binding, 'compiler': 'stock IDO 5.3', 'members': results,
                   'new_exact_code_bytes': 1048, 'preserved_exact_code_bytes': 948,
                   'new_landed_image_bytes': 0, 'new_rom_verified_bytes': 0}
        if deep:
            outcome['gnu'] = gnu_verify(obj, work, image)
            installed = ovl_splice.packet_spec(HERE)
            require(installed['members'] == list(MEMBERS) and installed['keep'] == ['func_803AE1C0'], 'installer contract changed')
            native = {name: packed(score.targets()[name]) for name in MEMBERS}
            slices, text_index = blob_group.member_slices(obj, list(MEMBERS),
                {name: {'vaddr': int(name[-8:], 16), 'size': size} for name, size in MEMBERS.items()})
            relocated = blob_group.relocate(obj, slices, text_index, score.image_symbols(),
                members=list(MEMBERS), image=(image, 0x8038A400))
            require(relocated == native, 'installer relocation changed complete bodies')
            key = owndata.artifact_dir(score.ASM_DIR)
            saved = score._own_data[key]
            score._own_data[key] = None
            try:
                missing = score.compare(obj, 'func_803ADDA8', show=0)
                require(not missing.accepted() and missing.unverified, 'missing own-data accepted')
            finally:
                score._own_data[key] = saved
            for offset in range(0, 20, 4):
                corrupt = bytearray(image)
                corrupt[RODATA - 0x8038A400 + offset] ^= 1
                score._own_data[key] = owndata.ImageData.from_image(corrupt, 0x8038A400)
                try:
                    altered = score.compare(obj, 'func_803ADDA8', show=0)
                    require(not altered.accepted() and altered.errors, 'corrupt literal accepted')
                    try:
                        blob_group.relocate(obj, slices, text_index, score.image_symbols(),
                            members=list(MEMBERS), image=(bytes(corrupt), 0x8038A400))
                    except blob_group.GroupError:
                        pass
                    else:
                        raise ValueError('installer accepted corrupt owned literal')
                finally:
                    score._own_data[key] = saved
            outcome['installer'] = 'all three complete bodies exact; missing data and five corrupt literals rejected'
            outcome['controls'] = {}
            for name, (source, count) in controls((HERE / 'group.c').read_text()).items():
                folder = work / name
                folder.mkdir()
                (folder / 'group.c').write_text(source)
                (folder / 'group.json').write_bytes((HERE / 'group.json').read_bytes())
                score.compile_group(folder, folder / 'group.o')
                result = score.compare(folder / 'group.o', 'func_803ADDA8', show=0)
                require(result.differing == count and not result.extra_words and not result.unverified, 'control changed')
                for sibling in ('func_803AE1C0', 'func_8038F744'):
                    require(score.compare(folder / 'group.o', sibling, show=0).accepted(), 'control changed installed member')
                outcome['controls'][name] = asdict(result)
            cc = shutil.which('cc') or shutil.which('gcc')
            require(cc, 'C compiler missing')
            subprocess.run([cc, '-std=c99', '-O2', '-Wall', '-Wextra', '-Werror', '-Wno-unused-parameter',
                            str(HERE / 'host.c'), '-o', str(work / 'host')], check=True, capture_output=True)
            subprocess.run([str(work / 'host')], check=True, capture_output=True)
            outcome['host'] = '640 projected/flat cases; all decimal writes two bytes including NUL'
            # IDO ABI/layout assertions; compile only our source, never a static ROM TU.
            abi = work / 'abi.c'
            abi.write_text('#include "' + str(HERE / 'group.c') + '"\n'
                           '#define CHECK(n,e) typedef char n[(e)?1:-1]\n'
                           'CHECK(word,sizeof(s32)==4);CHECK(pointer,sizeof(void*)==4);\n'
                           'CHECK(color,sizeof(Color4)==4);CHECK(slot,sizeof(Slot60)==60);\n'
                           'CHECK(camera,sizeof(Camera)==152);CHECK(language,sizeof(Language)==20);\n'
                           'CHECK(position_offset, __builtin_offsetof(Slot60,position)==48);\n')
            # IDO predates __builtin_offsetof; an unevaluated member-address constant is supported.
            abi.write_text(abi.read_text().replace('__builtin_offsetof(Slot60,position)', '(u32)&((Slot60*)0)->position'))
            score.compile_single(abi, FLAGS.replace('-O3', '-O2'), work / 'abi.o')
            outcome['abi'] = 'IDO32 scalar, pointer, by-value Color4, Slot60, Camera, Language and position offset checked'
        return outcome


def verify(deep=False):
    expected = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == expected['inputs'], 'input binding changed')
    actual = replay(deep)
    require(all(expected[key] == value for key, value in actual.items()), 'verification receipt differs')
    return actual


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--deep', action='store_true')
    parser.add_argument('--inputs-only', action='store_true')
    parser.add_argument('--score', action='store_true')
    args = parser.parse_args()
    if args.score:
        with target_context():
            sys.argv = [str(ROOT / 'tools/cloud/score.py'), 'group', str(HERE), '--targets', str(ROOT / 'asm/us/ovl_a')]
            raise SystemExit(score.main())
    if args.record:
        result = replay(args.deep)
        (HERE / 'verification.json').write_text(json.dumps(result, indent=2, sort_keys=True) + '\n')
    elif args.inputs_only:
        result = inputs()
        require(result == json.loads((HERE / 'verification.json').read_text())['inputs'], 'input binding changed')
    else:
        result = verify(args.deep)
    for name in MEMBERS:
        if 'members' in result:
            print(name + ': MATCH (' + str(MEMBERS[name]) + ' bytes)')
    print('Image A: 1048 fresh exact bytes; 948 preserved; 0 newly ROM-verified bytes')
