"""F207C full-body, source-provenance and host-behavior regression checks."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_billboard_helpers_20261005'
spec = importlib.util.spec_from_file_location('billboard_name_entry_verify', HERE / 'verify.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_receipt_binds_complete_matching_source():
    receipt = json.loads((HERE / 'verification.json').read_text())
    assert receipt['claims'] == ['func_800F207C']
    assert receipt['accepted_byte_gain'] == 0
    for path, expected in receipt['source_sha256'].items():
        assert hashlib.sha256((HERE / path).read_bytes()).hexdigest() == expected
    body = receipt['comparison']
    assert body['elf_function_bytes'] == body['native_bytes'] == 1684
    assert body['differing'] == body['extra_words'] == 0
    assert body['full_extent_relocated_equal']
    assert body['resolved_body_sha256'] == body['native_sha256']
    assert receipt['independent_gnu_link_equal']
    for key in ['unresolved', 'unverified', 'errors']:
        assert body[key] == []


def test_real_caller_preserved_and_not_claimed():
    original = (ROOT / 'cloud/work/ipa-groups/billboard_render/br.c').read_text()
    canonical = original.replace('D_8014A118', 'input_rec0').replace('D_8014A108', 'active_player_count')
    canonical = canonical.replace('   /* stand-in in ctx.c */', '')
    candidate = (HERE / 'group/billboard_render.c').read_text()
    assert candidate[candidate.index('typedef signed int s32;'):] == canonical
    group = json.loads((HERE / 'group/group.json').read_text())
    assert group['members'] == group['claims'] == ['func_800F207C']
    assert group['context'] == group['keep'] == ['billboard_render']
    assert group['files'] == ['func_800F207C.c', 'billboard_render.c']
    assert '__standin' not in ''.join((HERE / 'group' / f).read_text() for f in group['files'])


def test_context_conditions_are_reported_without_new_blockers():
    receipt = json.loads((HERE / 'verification.json').read_text())
    controls = receipt['controls']
    assert not receipt['caller_nonmatch']['full_extent_relocated_equal']
    assert controls['single_O2']['differing'] > 0
    assert controls['single_O3']['differing'] > 0
    assert controls['expanded_unchanged']['func_800F207C']['differing'] > 0
    for name in packet.CALLEES:
        assert controls['expanded_unchanged'][name]['full_extent_relocated_equal']
    assert all(row['full_extent_relocated_equal'] for row in controls['expanded_existing_policy'].values())
    overrides = json.loads((ROOT / 'src/blob/unit_overrides.json').read_text())
    assert packet.EXISTING_BLOCKERS <= {entry['name'] for entry in overrides['inline_blockers']}
    assert receipt['host_test'] == '240 behavior cases passed'


def test_current_target_whole_extent_is_bound():
    import struct
    native = packet.score.targets()['func_800F207C']
    receipt = json.loads((HERE / 'verification.json').read_text())
    assert len(native) == 421
    assert hashlib.sha256(struct.pack('>421I', *native)).hexdigest() == receipt['comparison']['native_sha256']


def test_rebuild_whole_extent_and_independent_gnu_link(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido'))
    if not (ido / 'cc').exists() or not all(shutil.which(tool) for tool in
                                          ['cc', 'mips-linux-gnu-ld', 'mips-linux-gnu-objcopy']):
        pytest.skip('pinned compiler or GNU MIPS toolchain unavailable')
    result = packet.verify(tmp_path)
    assert result['comparison']['full_extent_relocated_equal']
    assert result['independent_gnu_link_equal']
    assert result['host_test'] == '240 behavior cases passed'
