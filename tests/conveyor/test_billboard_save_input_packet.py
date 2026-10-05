"""Independent receipts and replay for the genuine F1930 context packet."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_billboard_context_20261005'
spec = importlib.util.spec_from_file_location('save_input_proof', HERE / 'verify.py')
packet = importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_source_receipt_and_claim_scope():
    result = json.loads((HERE / 'verification.json').read_text())
    assert result['claims'] == ['func_800F1930']
    assert result['new_matching_candidate_bytes'] == 980
    assert result['accepted_byte_gain'] == 0
    for path, digest in result['source_sha256'].items():
        assert hashlib.sha256((HERE / path).read_bytes()).hexdigest() == digest
    group = json.loads((HERE / 'group/group.json').read_text())
    assert group['members'] == group['claims'] == ['func_800F1930']
    assert group['keep'] == ['billboard_render']
    assert all('__standin' not in (HERE / 'group' / filename).read_text() for filename in group['files'])


def test_full_extent_and_context_caveats():
    result = json.loads((HERE / 'verification.json').read_text())
    for name, size in [('func_800F1930',980), ('func_800F207C',1684), ('func_800F2718',368)]:
        row = result['comparison'][name]
        assert row['elf_function_bytes'] == row['native_bytes'] == size
        assert row['differing'] == row['extra_words'] == 0
        assert row['full_extent_relocated_equal']
        assert result['independent_group_relocation_equal'][name]
    for name in ['func_800F0F44','func_800F1210','billboard_render']:
        assert not result['comparison'][name]['full_extent_relocated_equal']
        assert result['comparison'][name]['differing'] > 0
    assert result['comparison']['func_800F1210']['errors']
    assert result['controls']['signed_mode_literal']['func_800F1930']['differing'] == 1
    assert result['host_test'] == '103 save-input scenarios passed'


def test_existing_exact_sources_not_rewritten():
    prior = ROOT / 'cloud/work/frontier/dot_billboard_helpers_20261005/group/func_800F207C.c'
    assert prior.read_bytes() == (HERE / 'group/func_800F207C.c').read_bytes()
    from tools.conveyor.pipeline import blob_unit
    original = (ROOT / 'src/blob/groups/func_800F2718_grp/func_800F2718.c').read_text()
    copied = (HERE / 'group/func_800F2718.c').read_text()
    def body(source, name):
        entry = next(e for e in blob_unit.scan_defs(source) if e['name'] == name)
        return source[entry['open']:entry['close'] + 1]
    assert body(copied,'func_800F2718') == body(original,'func_800F2718')
    assert {e['name'] for e in blob_unit.scan_defs(copied)} == {'func_800F2718'}


def test_native_callers_and_literal_preserve_actual_contract():
    from tools.cloud import score
    native, symbols = score.targets(), score.image_symbols()
    call = 0x0C000000 | ((symbols['func_800F1930'] >> 2) & 0x03FFFFFF)
    assert [n for n, words in native.items() if call in words] == ['billboard_render']
    source = (HERE / 'group/func_800F0F44.c').read_text()
    assert 'extern s32 D_8014A110;' in source
    assert source.count('D_8014A110 == 2U') == 1
    assert 'Ref *r = *(Ref **)D_8014A160;' in source
    assert 'struct Obj **next;' in source
    assert 'n = o->next' in source
    assert '(*o->q)->id' in source


def test_rebuild_complete_bodies_and_sanitized_scenarios(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido'))
    if not (ido / 'cc').exists() or not all(shutil.which(t) for t in
                                          ['cc','mips-linux-gnu-readelf','mips-linux-gnu-objcopy']):
        pytest.skip('pinned compiler or GNU MIPS tools unavailable')
    result = packet.verify(tmp_path)
    assert result['comparison']['func_800F1930']['full_extent_relocated_equal']
    assert all(result['independent_group_relocation_equal'].values())
    assert result['host_test'] == '103 save-input scenarios passed'


def test_new_pointer_interfaces_normalized_but_production_still_blocked():
    render = (HERE / 'group/billboard_render.c').read_text()
    walker = (HERE / 'group/func_800F0F44.c').read_text()
    transition = (HERE / 'group/func_800F1210.c').read_text()
    for source in [render, walker]:
        assert 'extern void *D_8014A160;' in source
    assert 'extern void *D_801461A8;' in render
    assert 'extern s32 func_800CC040(s32, void *, void *, s32);' in render
    assert 'extern void *func_800CC50C(s32, char *);' in render
    for source in [render, transition]:
        assert 'extern s8 *D_80149418[]' in source
        assert 'extern void sound_handles_array_clear(s8 *);' in source
    assert walker.endswith('}\n') and not walker.endswith('}\n\n')
    receipt = json.loads((HERE / 'verification.json').read_text())
    assert receipt['production_integration_status'] == 'BLOCKED_ON_INHERITED_DECLARATIONS_AND_OPEN_CONTEXT'
    assert receipt['remaining_type_contracts']
