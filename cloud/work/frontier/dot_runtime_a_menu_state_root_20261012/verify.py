#!/usr/bin/env python3
"""Authenticate existing image A, then run unmodified stock scoring/data gates."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
import re
import shutil
import subprocess
from pathlib import Path
import struct
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP_NAME = 'dot_runtime_a_menu_state_20261006'
GROUP = HERE / 'groups' / GROUP_NAME
MEMBERS = ['func_80398B40', 'func_80398C3C', 'func_80398E70', 'func_803990D0']
CONTEXT = []
ASSET_SHA256 = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
TABLE_ADDRESS = 0x803B9508
TABLE_SIZE = 20
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.cloud import score, owndata
from tools.conveyor import compilers


def require(value, message):
    if not value:
        raise ValueError(message)


def digest(data):
    return hashlib.sha256(data).hexdigest()


@contextmanager
def target_scope(image=None):
    previous = score.ASM_DIR
    directory = ROOT / 'asm/us/ovl_a'
    key = owndata.artifact_dir(directory)
    missing = object()
    saved = score._own_data.get(key, missing)
    score.ASM_DIR = directory
    try:
        if image is not None:
            score._own_data[key] = image
        yield
    finally:
        score.ASM_DIR = previous
        if saved is missing:
            score._own_data.pop(key, None)
        else:
            score._own_data[key] = saved


def metadata():
    with target_scope():
        manifest = score.target_manifest()
        return json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))


def authenticate():
    """No git-history access or binary output; both asset and full image are pinned."""
    meta = metadata()
    asset = (ROOT / 'assets/us/data.bin').read_bytes()
    require(len(asset) == 12418096 and digest(asset) == ASSET_SHA256, 'asset identity mismatch')
    offset = int(meta['rom_offset'], 16)
    require(int.from_bytes(asset[0x3850:0x3854], 'big') == offset == 0xB5C534,
            'image A loader pointer mismatch')
    decompressor = zlib.decompressobj(-15)
    data = decompressor.decompress(asset[offset - 0x283D0:]) + decompressor.flush()
    require(decompressor.eof, 'image A stream is incomplete')
    require(len(data) == meta['size'] == 194128 and digest(data) == meta['image_sha256'],
            'image A identity mismatch')
    base = int(meta['base'], 16)
    require(base == 0x8038A400, 'unexpected image base')
    table = data[TABLE_ADDRESS - base:TABLE_ADDRESS - base + TABLE_SIZE]
    require(len(table) == TABLE_SIZE, 'table outside image')
    return data, base, table


def inputs():
    spec = json.loads((GROUP / 'group.json').read_text())
    require(spec['members'] == spec['claims'] == MEMBERS, 'superseding member/claim set changed')
    require(spec['context'] == CONTEXT and spec['keep'] == ['func_803990D0'], 'genuine IPA context changed')
    require(not spec.get('allow_unverified'), 'an uncertainty waiver is not permitted')
    with target_scope():
        manifest = score.target_manifest()
        meta = metadata()
        extents = {e['name']: e for e in meta['functions']}
        targets = {}
        for name in MEMBERS + CONTEXT:
            words = score.targets()[name]
            extent = extents[name]
            require(len(words) * 4 == extent['size'], 'native extent mismatch')
            targets[name] = {'address': extent['address'], 'size': extent['size'],
                             'words_sha256': digest(struct.pack('>%dI' % len(words), *words))}
        paths = ['groups/' + GROUP_NAME + '/group.c', 'groups/' + GROUP_NAME + '/group.json', 'verify.py']
        return {'sources_sha256': {p: digest((HERE / p).read_bytes()) for p in paths},
                'target_population': 'A',
                'targets': targets, 'asset_sha256': ASSET_SHA256,
                'image_sha256': meta['image_sha256'], 'image_size': meta['size'],
                'members': MEMBERS, 'context': CONTEXT, 'flags': spec['flags']}


def table_structure(obj, data, base, table):
    elf = owndata._Object(obj)
    rodata = [i for i, sec in enumerate(elf.sections) if sec['name'] == '.rodata']
    text = [i for i, sec in enumerate(elf.sections) if sec['name'] == '.text']
    require(len(rodata) == len(text) == 1, 'unexpected section set')
    rels = elf.relocations(rodata[0])
    require([r[0] for r in rels] == list(range(0, TABLE_SIZE, 4)), 'table relocation coverage changed')
    require(all(kind == owndata.R_MIPS_32 and symbol['section'] == text[0]
                for _offset, kind, symbol in rels), 'non-text switch-table entry')
    raw = elf.raw(rodata[0])
    require(len(raw) == 32 and not any(raw[TABLE_SIZE:]), 'nonzero data outside owned table')
    root_start = score.symbols(obj)['func_803990D0']
    resolved = []
    for offset, _kind, symbol in rels:
        addend = struct.unpack_from('>I', raw, offset)[0] + symbol['value']
        relative = addend - root_start
        require(relative % 4 == 0 and 0 <= relative < 692, 'table target leaves complete root body')
        resolved.append(0x803990D0 + relative)
    actual = struct.pack('>5I', *resolved)
    require(actual == table, 'complete table differs after independent entry mapping')
    return {'address': hex(TABLE_ADDRESS), 'size': TABLE_SIZE, 'entries': 5,
            'relocations': 'five R_MIPS_32 to in-root .text offsets',
            'sha256': digest(table), 'zero_section_alignment_bytes': len(raw) - TABLE_SIZE}


def installer_check(obj, data, base, binding):
    """Read-only actual installer relocation path; never calls splice or writes locks."""
    from tools.conveyor.pipeline import blob_group, ovl_splice
    installed_spec = ovl_splice.packet_spec(GROUP)
    require(installed_spec['members'] == MEMBERS and installed_spec['context'] == CONTEXT,
            'packet-to-installer conversion changed members/context')
    extents = {name: {'vaddr': int(e['address'], 16), 'size': e['size']}
               for name, e in binding['targets'].items()}
    slices, text_ndx = blob_group.member_slices(obj, MEMBERS + CONTEXT, extents)
    bodies = blob_group.relocate(obj, slices, text_ndx, score.image_symbols(), members=MEMBERS,
                                image=(data, base))
    result = {}
    for name in MEMBERS:
        extent = extents[name]
        expected = data[extent['vaddr'] - base:extent['vaddr'] - base + extent['size']]
        require(bodies[name] == expected, 'installer relocated body mismatch: ' + name)
        result[name] = {'bytes': len(bodies[name]), 'sha256': digest(bodies[name])}
    # The same strict data gate must reject a changed table rather than accept code alone.
    bad = bytearray(data)
    bad[TABLE_ADDRESS - base] ^= 1
    try:
        blob_group.relocate(obj, slices, text_ndx, score.image_symbols(), members=MEMBERS,
                            image=(bytes(bad), base))
    except blob_group.GroupError:
        pass
    else:
        raise ValueError('installer accepted a corrupted owned table')
    return {'members': result, 'corrupt_table_rejected': True, 'writes_production_state': False}


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


def gnu_verify(obj, work, table):
    sections, symbols, relocs = elf(obj)
    ti, _, raw = sections['.text']
    lines = ['.set noreorder', '.set noat']
    externs, counts = set(), {}
    for name in MEMBERS:
        size = len(score.targets()[name]) * 4
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
    require(len(owned) == 32 and not any(owned[20:]), 'owned-data extent/padding changed')
    root_start = symbols['func_803990D0'][0]
    data_relocs = [(loc, typ, symbol) for loc, typ, symbol, dest in relocs if dest == sections['.rodata'][0]]
    require(data_relocs == [(i, 2, '.text') for i in range(0, 20, 4)], 'switch-table relocations changed')
    lines += ['.section .review_owned,"a",@progbits', '.globl packet_owned_rodata', 'packet_owned_rodata:']
    for offset in range(0, 20, 4):
        addend = struct.unpack_from('>I', owned, offset)[0] - root_start
        require(0 <= addend < 692 and addend % 4 == 0, 'table branch outside root')
        lines += ['.word func_803990D0 + %d' % addend]
    asm, reloc, script, linked = [work / name for name in ('review.s', 'review.o', 'review.ld', 'review.elf')]
    asm.write_text('\n'.join(lines) + '\n')
    addresses = score.image_symbols()
    assignments = []
    for name in sorted(externs - set(MEMBERS) - {'packet_owned_rodata'}):
        address = addresses.get(name, score.address_named(name))
        require(address is not None, 'unresolved GNU symbol: ' + name)
        assignments.append('%s = 0x%X;' % (name, address))
    script.write_text('SECTIONS {\n' + ''.join('.review_%s 0x%s : SUBALIGN(4) { *(.review_%s) }\n' % (n, n[-8:], n) for n in MEMBERS)
                      + '.review_owned 0x803B9508 : SUBALIGN(4) { *(.review_owned) }\n'
                      + '/DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) *(.pdr) *(.gnu.attributes) }\n}\n' + '\n'.join(assignments))
    for command in (['mips-linux-gnu-as', '-EB', '-mips2', '-o', str(reloc), str(asm)],
                    ['mips-linux-gnu-ld', '-EB', '-T', str(script), '-o', str(linked), str(reloc)]):
        require(shutil.which(command[0]), 'GNU MIPS binutils missing: ' + command[0])
        subprocess.run(command, check=True, capture_output=True)
    ss, sy, rr = elf(linked)
    require(not rr, 'GNU relocation left unresolved')
    for name in MEMBERS:
        size = len(score.targets()[name]) * 4
        index, row, body = ss['.review_' + name]
        address = int(name[-8:], 16)
        require(row[3] == address and len(body) == size and sy[name] == (address, size, 2, index), 'GNU extent changed')
        require(body == struct.pack('>%dI' % (size // 4), *score.targets()[name]), 'GNU full words differ: ' + name)
    require(ss['.review_owned'][2] == table, 'GNU relocated owned table differs')
    return {'relocations': counts, 'owned_bytes': 20, 'owned_start': hex(TABLE_ADDRESS),
            'owned_sha256': digest(table), 'full_extent_words_exact': True}



def frame_controls(temp):
    source = (GROUP / 'group.c').read_text()
    original = source.replace('    s32 frame_slot3;\n', '').replace(
        '    s32 frame_slot0, frame_slot1, frame_slot2;\n', '')
    three = source.replace('    s32 frame_slot3;\n', '')
    four_below = source.replace('    s32 frame_slot3;\n', '').replace(
        'frame_slot0, frame_slot1, frame_slot2;', 'frame_slot0, frame_slot1, frame_slot2, frame_slot3;')
    expected = {'original': (8, 96, 56), 'three_below': (8, 104, 64), 'four_below': (6, 112, 72)}
    results = {}
    for label, code in [('original', original), ('three_below', three), ('four_below', four_below)]:
        directory = temp / label
        directory.mkdir()
        (directory / 'group.c').write_text(code)
        (directory / 'group.json').write_bytes((GROUP / 'group.json').read_bytes())
        obj = directory / 'group.o'
        score.compile_group(directory, obj)
        value = score.compare(obj, 'func_803990D0', show=0)
        start = score.symbols(obj)['func_803990D0'] // 4
        words = score.text_words(obj)[start:]
        frame = 65536 - (words[2] & 65535)
        home = words[10] & 65535
        require((value.differing, frame, home) == expected[label], 'frame control changed: ' + label)
        require(not any((value.unresolved, value.unverified, value.errors, value.extra_words)), 'bad frame control')
        for name in MEMBERS[:-1]:
            require(score.compare(obj, name, show=0).accepted(), 'old member changed in frame control')
        results[label] = dict(differing_words=value.differing, frame_bytes=frame, descriptor_home=home)
    return results


def replay(deep=False):
    binding = inputs()
    score.ido('cc')
    require(compilers.identify_dir(score.IDO) == 'ido5.3', 'stock IDO 5.3 required')
    data, base, table = authenticate()
    image = owndata.ImageData.from_image(data, base)
    with tempfile.TemporaryDirectory(prefix='renderer-owned-data-') as temp, target_scope(image):
        obj = Path(temp) / 'group.o'
        score.compile_group(GROUP, obj)
        results = {}
        for name in MEMBERS:
            value = score.compare(obj, name, show=0)
            require(value.accepted(), name + ': ' + value.summary())
            results[name] = asdict(value)
        raw, sections = score._elf(obj)
        symbols = [s for i, sec in enumerate(sections) if sec['type'] == 2
                   for s in score._symbol_table(raw, sections, i)]
        for name in MEMBERS + CONTEXT:
            symbol = next(s for s in symbols if s['name'] == name)
            require(symbol['size'] == binding['targets'][name]['size'], 'compiled extent mismatch: ' + name)
        own = table_structure(obj, data, base, table)
        key = owndata.artifact_dir(score.ASM_DIR)
        # Each of the five entries is independently load-bearing, including the last.
        for entry in range(5):
            mutated = bytearray(table)
            mutated[entry * 4] ^= 1
            score._own_data[key] = owndata.ImageData([(TABLE_ADDRESS, bytes(mutated))])
            bad = score.compare(obj, 'func_803990D0', show=0)
            require(not bad.accepted() and bad.errors, 'canonical scorer accepted mutated table entry')
        score._own_data[key] = owndata.ImageData([(TABLE_ADDRESS, table[:-1])])
        missing = score.compare(obj, 'func_803990D0', show=0)
        require(not missing.accepted(), 'canonical scorer accepted a truncated table')
        score._own_data[key] = None
        absent = score.compare(obj, 'func_803990D0', show=0)
        require(not absent.accepted() and len(absent.unverified) == 2,
                'canonical scorer no longer fails closed without table data')
        score._own_data[key] = image
        frames = frame_controls(Path(temp))
        output = {'frame_controls': frames, 'inputs': binding, 'results': results, 'owned_table': own,
                  'controls': {'all_five_table_entry_mutations_rejected': True,
                               'truncated_table_rejected': True, 'absent_table_unverified_references': 2},
                  'new_verified_candidate_bytes': 692, 'accepted_image_bytes': 0,
                  'preserved_existing_member_bytes': 1332}
        if deep:
            output['installer'] = installer_check(obj, data, base, binding)
            output['gnu'] = gnu_verify(obj, Path(temp), table)
        return output


def verify(deep=False):
    expected = json.loads((HERE / 'verification.json').read_text())
    require(inputs() == expected['inputs'], 'receipt input bindings changed')
    actual = replay(deep)
    # Normalize tuples from scorer dataclasses to the JSON receipt representation.
    normalized = json.loads(json.dumps(actual))
    require({k: normalized[k] for k in expected} == expected, 'receipt proof changed')
    return actual


def canonical_score():
    inputs()
    data, base, _table = authenticate()
    old = sys.argv
    try:
        sys.argv = ['score.py', 'group', str(GROUP), '--targets', str(ROOT / 'asm/us/ovl_a')]
        with target_scope(owndata.ImageData.from_image(data, base)):
            print('Authenticated current-tree asset and full image A; canonical group scorer:')
            return score.main()
    finally:
        sys.argv = old


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--record', action='store_true')
    parser.add_argument('--score', action='store_true')
    parser.add_argument('--deep', action='store_true')
    args = parser.parse_args()
    if args.score:
        return canonical_score()
    if args.record:
        (HERE / 'verification.json').write_text(json.dumps(replay(), indent=2) + '\n')
        print('Recorded strict authenticated-image proof: 692 newly verified candidate bytes')
    else:
        actual = verify(args.deep)
        print('PASS: all four complete members; full owned table; frame and data controls')
        if args.deep:
            print(json.dumps({key: actual[key] for key in ('installer', 'gnu')}, indent=2))
    return 0


if __name__ == '__main__':
    sys.exit(main())
