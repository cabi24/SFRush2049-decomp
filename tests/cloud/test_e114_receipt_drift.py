"""E114 receipt mismatch diagnostics preserve every source-bound proof field."""
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import struct

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/runtime_b_e114_parent_20261006'


def module():
    spec = importlib.util.spec_from_file_location('e114_receipt_drift', PACKET / 'verify.py')
    verifier = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(verifier)
    return verifier


def test_e114_diagnostics_name_changed_added_missing_and_type_leaves():
    verifier = module()
    expected = {'a': [{'value': 3}], 'missing': {}, 'typed': True}
    actual = {'a': [{'value': 4}, {}], 'extra': 1, 'typed': 1}
    assert verifier.receipt_differences(expected, actual) == [
        '$["a"]: length 1 != 2',
        '$["a"][0]["value"]: expected 3; actual 4',
        '$["extra"]: unexpected field',
        '$["missing"]: missing field',
        '$["typed"]: type bool != int',
    ]


def test_e114_diagnostics_do_not_modify_or_discard_unknown_proof():
    verifier = module()
    expected = json.loads((PACKET / 'verification.json').read_text())
    untouched = copy.deepcopy(expected)
    actual = copy.deepcopy(expected)
    actual['object']['sha256'] = 'historical file-layout provenance'
    actual['independent_link']['sha256'] = 'historical file-layout provenance'
    verifier.check_receipt(expected, actual)
    actual['new_unknown_proof'] = {'value': 1}
    with pytest.raises(AssertionError, match=r'\$\["new_unknown_proof"\]: unexpected field'):
        verifier.check_receipt(expected, actual)
    assert expected == untouched


@pytest.mark.parametrize('path', [
    ('link_flags', 0),
    ('independent_link', 'portable_elf', 'ident_sha256'),
    ('independent_link', 'portable_elf', 'abi', 'flags'),
    ('independent_link', 'portable_elf', 'allocated_sections', 0, 'sha256'),
    ('independent_link', 'portable_elf', 'symbols', 0, 'size'),
    ('object', 'portable_elf', 'sections', 8, 'sha256'),
    ('native_behavior', 'paired_fixtures'),
    ('source_sha256',),
])
def test_e114_diagnostics_locate_semantic_mutation(path):
    verifier = module()
    expected = json.loads((PACKET / 'verification.json').read_text())
    actual = copy.deepcopy(expected)
    node = actual
    for key in path[:-1]:
        node = node[key]
    node[path[-1]] = 'changed proof'
    with pytest.raises(AssertionError) as error:
        verifier.check_receipt(expected, actual)
    assert '$' + ''.join('[' + json.dumps(key) + ']' for key in path) in str(error.value)


def test_e114_source_and_verifier_receipt_bindings():
    receipt = json.loads((PACKET / 'verification.json').read_text())
    for filename, digest in receipt['artifact_hashes'].items():
        assert hashlib.sha256((PACKET / filename).read_bytes()).hexdigest() == digest, filename


@pytest.fixture
def linked_images(tmp_path):
    from tools.cloud import score
    if not (score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    verifier = module()
    obj = tmp_path / 'children.o'
    score.compile_single(PACKET / 'children.c', verifier.FLAGS, obj)
    script = tmp_path / 'candidate.ld'
    script.write_text('\n'.join('%s = 0x%08X;' % item for item in verifier.BINDINGS.items()) +
        '\nSECTIONS { .text 0x81000000 : { *(.text) } .rodata 0x81010000 : { *(.rodata) } '
        '/DISCARD/ : { *(.reginfo) *(.options) *(.mdebug) *(.comment) } }\n')
    images = {}
    configurations = {
        'pinned': verifier.LINK_FLAGS,
        'gnu': ('--hash-style=gnu',),
        'sysv': ('--hash-style=sysv',),
        'both': ('--hash-style=both',),
        'override-gnu': ('--hash-style=gnu',) + verifier.LINK_FLAGS,
    }
    for label, options in configurations.items():
        output = tmp_path / (label + '.elf')
        verifier.run(['mips-linux-gnu-ld', *options, '-T', script, '-o', output, obj])
        images[label] = output.read_bytes()
    return verifier, images


def test_e114_hash_style_is_explicit_without_masking_abi(linked_images):
    verifier, images = linked_images
    assert verifier.LINK_FLAGS == ('--hash-style=sysv',)
    receipt = json.loads((PACKET / 'verification.json').read_text())
    assert receipt['link_flags'] == list(verifier.LINK_FLAGS)
    proof = verifier.portable_linked_elf(images['pinned'])
    assert proof == receipt['independent_link']['portable_elf']
    for label in ('sysv', 'both', 'override-gnu'):
        assert images[label][8] == 0
        assert verifier.portable_linked_elf(images[label]) == proof
    # The actual alternate linker mode reproduces the historical ABI drift.
    # It is still rejected, even when every code/data/symbol fact is equal.
    assert images['gnu'][8] == 5 and images['pinned'][8] == 0
    gnu = verifier.portable_linked_elf(images['gnu'])
    assert gnu != proof
    assert verifier.receipt_differences(proof, gnu) == [
        '$["ident_sha256"]: expected %s; actual %s' %
        (json.dumps(proof['ident_sha256']), json.dumps(gnu['ident_sha256']))]


def test_e114_entire_elf_identity_remains_bound(linked_images):
    verifier, images = linked_images
    original = images['pinned']
    proof = verifier.portable_linked_elf(original)
    for offset in range(16):
        changed = bytearray(original)
        changed[offset] ^= 1
        try:
            actual = verifier.portable_linked_elf(changed)
        except (AssertionError, ValueError, IndexError, struct.error):
            continue
        assert actual != proof, offset
