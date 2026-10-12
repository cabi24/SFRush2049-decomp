#!/usr/bin/env python3
"""Authenticate existing image A, then run unmodified stock scoring/data gates."""
import argparse
from contextlib import contextmanager
from dataclasses import asdict
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile
import zlib

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
GROUP_NAME = 'dot_runtime_a_menu_renderers_20261011'
GROUP = HERE / 'groups' / GROUP_NAME
MEMBERS = ['func_803A6F2C', 'func_803A7620', 'func_803A7918']
CONTEXT = ['func_803A7180']
ASSET_SHA256 = 'f06d4ad0bb7dc7aff494acddc736f1b56bc271a2292c0286879cc9189bec31c8'
TABLE_ADDRESS = 0x803B9834
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
    require(spec['context'] == CONTEXT and spec['keep'] == ['func_803A7918'], 'genuine IPA context changed')
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
        paths = ['groups/' + GROUP_NAME + '/group.c', 'groups/' + GROUP_NAME + '/group.json', 'verify.py', 'trace_probes.py']
        return {'sources_sha256': {p: digest((HERE / p).read_bytes()) for p in paths},
                'target_metadata_sha256': {p: manifest[p] for p in ('extents.json', 'symbols.json')},
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
    root_start = score.symbols(obj)['func_803A7918']
    resolved = []
    for offset, _kind, symbol in rels:
        addend = struct.unpack_from('>I', raw, offset)[0] + symbol['value']
        relative = addend - root_start
        require(relative % 4 == 0 and 0 <= relative < 576, 'table target leaves complete root body')
        resolved.append(0x803A7918 + relative)
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
        value = score.compare(obj, CONTEXT[0], show=0)
        require(value.differing == 2 and not any((value.unverified, value.unresolved,
                value.errors, value.extra_words)), '7180 context residual changed')
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
            bad = score.compare(obj, 'func_803A7918', show=0)
            require(not bad.accepted() and bad.errors, 'canonical scorer accepted mutated table entry')
        score._own_data[key] = owndata.ImageData([(TABLE_ADDRESS, table[:-1])])
        missing = score.compare(obj, 'func_803A7918', show=0)
        require(not missing.accepted(), 'canonical scorer accepted a truncated table')
        score._own_data[key] = None
        absent = score.compare(obj, 'func_803A7918', show=0)
        require(not absent.accepted() and len(absent.unverified) == 2,
                'canonical scorer no longer fails closed without table data')
        score._own_data[key] = image
        output = {'inputs': binding, 'results': results, 'owned_table': own,
                  'controls': {'all_five_table_entry_mutations_rejected': True,
                               'truncated_table_rejected': True, 'absent_table_unverified_references': 2},
                  'new_verified_candidate_bytes': 576, 'accepted_image_bytes': 0,
                  'preserved_existing_member_bytes': 1356, 'context_differing_words': 2}
        if deep:
            output['installer'] = installer_check(obj, data, base, binding)
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
        print('Recorded strict authenticated-image proof: 576 newly verified candidate bytes')
    else:
        actual = verify(args.deep)
        print('PASS: all three complete members; full owned table; seven fail-closed controls')
        if args.deep:
            print(json.dumps(actual['installer'], indent=2))
    return 0


if __name__ == '__main__':
    sys.exit(main())
