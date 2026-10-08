"""Integration provenance may drift; packet and executable proof may not."""
import ast
import copy
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import struct
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKETS = {
    '152': ('frontier/dot_runtime_a_resource_cache_20261006', 'portable',
            ('game_manifest', 'protected_manifest', 'scorer_sha256', 'object_sha256', 'mdebug_sha256')),
    '153': ('runtime_b_event_ring_20261006', 'comparable', ('inputs_sha256',)),
    '154': ('runtime_a_float_product', 'comparable', ('scorer_sha256', 'protected_manifest_sha256')),
    '155': ('frontier/dot_runtime_a_stat_bar_20261006', 'comparable',
            ('protected_targets', 'helper_protected_targets', 'scorer_sha256')),
    '156': ('runtime_b_lives_text_20261006', 'comparable', ('input_sha256', 'target_sha256', 'object_sha256')),
    '157': ('frontier/dot_runtime_a_availability_boundary_20261006', 'comparable',
            ('protected_targets', 'scorer_sha256')),
    '159': ('runtime_a_settings_bar_20261006', 'comparable',
            ('protected_targets', 'helper_protected_targets', 'scorer_sha256')),
    '160': ('runtime_b_e114_parent_20261006', 'comparable', ('target_manifest_sha256', 'tools')),
    '162': ('frontier/dot_sequence_start_979a0_20261006', 'comparable',
            ('accepted_group_spec_sha256', 'scorer_sha256', 'own_data_tool_sha256', 'native_manifest_sha256')),
}


def load(number):
    relative, name, provenance = PACKETS[number]
    packet = ROOT / 'cloud/work' / relative
    # Load only the pure normalizer, avoiding unrelated packet module namespaces.
    tree = ast.parse((packet / 'verify.py').read_text())
    definitions = [node for node in tree.body if isinstance(node, ast.FunctionDef)
                   and node.name in (name, 'require')]
    namespace = {'json': json}
    exec(compile(ast.Module(body=definitions, type_ignores=[]), str(packet / 'verify.py'), 'exec'), namespace)
    return packet, json.loads((packet / 'verification.json').read_text()), namespace[name], provenance


def excluded(number, receipt):
    paths = [(key,) for key in PACKETS[number][2]]
    if number == '152':
        paths += [('controls', label, key) for label in receipt['controls']
                  for key in ('object_sha256', 'mdebug_sha256')]
        paths += [(section, label, 'source_sha256')
                  for section in ('matched_siblings', 'helper_contracts') for label in receipt[section]]
    if number == '160':
        paths += [('object', 'sha256'), ('independent_link', 'sha256')]
    if number == '153':
        paths += [('helpers', label, 'source_sha256') for label in receipt['helpers']]
    if number in ('153', '156'):
        paths += [('tool_sha256', tool) for tool in ('gcc', 'mips-linux-gnu-ld')]
    return paths


def alter(receipt, path):
    result = copy.deepcopy(receipt)
    node = result
    for key in path[:-1]:
        node = node[key]
    node[path[-1]] = 'deliberate drift'
    return result


def leaves(value, prefix=()):
    if isinstance(value, dict):
        for key, child in value.items():
            yield from leaves(child, prefix + (key,))
    elif isinstance(value, list) and value:
        for index, child in enumerate(value):
            yield from leaves(child, prefix + (index,))
    else:
        yield prefix


@pytest.mark.parametrize('number', PACKETS)
def test_only_explicit_historical_provenance_is_nonbinding(number):
    packet, receipt, comparable, provenance = load(number)
    baseline = comparable(receipt)
    paths = excluded(number, receipt)
    for path in paths:
        assert comparable(alter(receipt, path)) == baseline, (number, path)
    assert comparable(receipt) == baseline  # normalization never changes its input
    for path in leaves(receipt):
        if any(path[:len(skip)] == skip for skip in paths):
            continue
        try:
            normalized = comparable(alter(receipt, path))
        except (AssertionError, ValueError, TypeError, KeyError, AttributeError):
            continue  # Invalid proof structure is rejected, never normalized away.
        assert normalized != baseline, (number, path)


@pytest.mark.parametrize('number', PACKETS)
def test_packet_verifier_and_sources_remain_bound(number):
    packet, receipt, comparable, provenance = load(number)
    verifier = hashlib.sha256((packet / 'verify.py').read_bytes()).hexdigest()
    bindings = receipt.get('files', receipt.get('packet_sha256', receipt.get('support_sha256', receipt.get('artifact_hashes', {}))))
    for filename, digest in bindings.items():
        assert not filename.startswith('test_'), filename
        assert hashlib.sha256((packet / filename).read_bytes()).hexdigest() == digest, filename
    assert verifier in [*bindings.values(), receipt.get('verifier_sha256'), receipt.get('verification_script_sha256')]
    if number == '162':
        for filename, digest in receipt['source_sha256'].items():
            assert hashlib.sha256((packet / 'group' / filename).read_bytes()).hexdigest() == digest
        assert hashlib.sha256((packet / 'group/group.json').read_bytes()).hexdigest() == receipt['group_spec_sha256']


@pytest.mark.parametrize('number,image,name', [
    ('155', 'ovl_a', 'func_803A4134'), ('156', 'ovl_b', 'func_80393518'),
])
def test_o3_header_and_actual_backend_policy(number, image, name):
    packet, receipt, comparable, provenance = load(number)
    source = ROOT / 'cloud/matches' / image / (name + '.c')
    assert source.read_text().splitlines()[0] == '/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
    assert receipt['flags'] == '-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
    assert receipt['source_sha256'] == hashlib.sha256(source.read_bytes()).hexdigest()
    assert receipt['comparison']['differing'] == receipt['comparison']['extra_words'] == 0


@pytest.mark.parametrize('number', ['153', '155', '156', '157', '160'])
def test_guarded_complete_packet_replay(number):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido'))
    if not (ido / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    packet, receipt, comparable, provenance = load(number)
    result = subprocess.run([sys.executable, str(packet / 'verify.py'), '--check'],
                            cwd=ROOT, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr


def packet_module(number):
    packet = ROOT / 'cloud/work' / PACKETS[number][0]
    spec = importlib.util.spec_from_file_location('late_packet_' + number, packet / 'verify.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def test_event_ring_context_is_read_at_historical_base(tmp_path, monkeypatch):
    """Later accepted locks and integrated sources cannot redefine this packet."""
    module = packet_module('153')
    paths = ['blob_matched.lock.json', 'src/blob/func_800F7E30.c',
             'src/blob/effect_cleanup.c']
    baseline = {path: module.pinned(path) for path in paths}
    (tmp_path / '.git').symlink_to((ROOT / '.git').resolve(), target_is_directory=True)
    for path in paths:
        live = tmp_path / path
        live.parent.mkdir(parents=True, exist_ok=True)
        live.write_text('different integrated source and accepted locks\n')
    monkeypatch.setattr(module, 'ROOT', tmp_path)
    assert {path: module.pinned(path) for path in paths} == baseline
    lock = json.loads(baseline['blob_matched.lock.json'])
    for name in ('func_800F7E30', 'effect_cleanup'):
        assert lock[name]['source_sha256'] == hashlib.sha256(baseline['src/blob/' + name + '.c']).hexdigest()
        assert lock[name]['verified'] == 'image_gate'


@pytest.fixture
def e114_linked_images(tmp_path):
    from tools.cloud import score
    if not (score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    module = packet_module('160')
    obj = tmp_path / 'children.o'
    score.compile_single(module.HERE / 'children.c', module.FLAGS, obj)
    script = tmp_path / 'candidate.ld'
    script.write_text('\n'.join('%s = 0x%08X;' % item for item in module.BINDINGS.items()) +
        '\nSECTIONS { .text 0x81000000 : { *(.text) } .rodata 0x81010000 : { *(.rodata) } '
        '/DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) *(.comment) } }\n')
    images = []
    for index, options in enumerate(([], ['--hash-size=1'], ['-z', 'max-page-size=0x1000'],
                                     ['-z', 'max-page-size=0x10000'])):
        output = tmp_path / ('children-%d.elf' % index)
        module.run(['mips-linux-gnu-ld', *options, '-T', script, '-o', output, obj])
        images.append(output.read_bytes())
    return module, images


def test_e114_linked_symbol_order_and_file_packing_are_provenance(e114_linked_images):
    module, images = e114_linked_images
    fingerprints = [module.portable_linked_elf(data) for data in images]
    assert all(value == fingerprints[0] for value in fingerprints)
    # A linker's default may equal either explicit layout. Require an effective
    # packing change, not an incidental distinct hash for every option.
    assert images[2] != images[3]
    assert len({hashlib.sha256(data).hexdigest() for data in images}) >= 2
    # Independently guarantee symbol-order coverage even when --hash-size is
    # a no-op on a particular GNU version. There are no remaining relocations.
    data = bytearray(images[0])
    header = struct.unpack_from('>HHIIIIIHHHHHH', data, 16)
    rows = [struct.unpack_from('>10I', data, header[5] + 40 * i)
            for i in range(header[11])]
    symtab = next(row for row in rows if row[1] == 2)
    global_entries = [at for at in range(symtab[4], symtab[4] + symtab[5], 16)
                      if data[at + 12] >> 4 == 1]
    first, second = global_entries[:2]
    data[first:first + 16], data[second:second + 16] = \
        data[second:second + 16], data[first:first + 16]
    assert data != images[0]
    assert module.portable_linked_elf(data) == fingerprints[0]


def test_e114_linked_semantic_mutations_are_rejected(e114_linked_images):
    module, images = e114_linked_images
    original = images[0]
    proof = module.portable_linked_elf(original)
    header = struct.unpack_from('>HHIIIIIHHHHHH', original, 16)
    rows = [struct.unpack_from('>10I', original, header[5] + 40 * i)
            for i in range(header[11])]
    names = rows[header[12]]
    labels = [original[names[4] + row[0]:].split(b'\0', 1)[0].decode() for row in rows]

    def rejected(changed):
        try:
            actual = module.portable_linked_elf(changed)
        except (AssertionError, ValueError, IndexError, struct.error):
            return
        assert actual != proof

    for name in ('.text', '.rodata'):
        index = labels.index(name)
        row = rows[index]
        changed = bytearray(original); changed[row[4]] ^= 1
        rejected(changed)
        for field in (2, 3, 5, 8):  # section flags, virtual extent and alignment
            changed = bytearray(original)
            struct.pack_into('>I', changed, header[5] + 40 * index + 4 * field, row[field] ^ 1)
            rejected(changed)
    symtab = rows[labels.index('.symtab')]
    for field in (4, 8, 12, 13):  # symbol value, extent, type/binding and visibility
        changed = bytearray(original)
        changed[symtab[4] + 3 * 16 + field] ^= 1
        rejected(changed)
    changed = bytearray(original); changed[39] ^= 1  # MIPS ABI flags
    rejected(changed)
    changed = bytearray(original)
    name_at = names[4] + rows[labels.index('.rodata')][0]
    changed[name_at:name_at + 7] = b'.bogus\0'
    with pytest.raises(AssertionError):
        module.portable_linked_elf(changed)
    changed = bytearray(original)
    struct.pack_into('>I', changed, header[5] + 40 * labels.index('.rodata') + 4, 9)
    with pytest.raises(AssertionError):
        module.portable_linked_elf(changed)


def test_e114_raw_link_hash_cannot_replace_linked_semantic_proof():
    _, receipt, comparable, _ = load('160')
    del receipt['independent_link']['portable_elf']
    with pytest.raises(AssertionError, match='missing linked ELF semantics'):
        comparable(receipt)
