"""D08 metadata/audit regressions, not game behavioral or match acceptance tests."""
import importlib.util
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/dot_d08_scout'
spec = importlib.util.spec_from_file_location('dot_d08_audit', PACKET / 'audit.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)

def test_jal_decode_excludes_tail_and_indirect_calls():
    # Synthetic opcodes: direct JAL, ordinary J, JALR, zero.
    assert audit.direct_calls([0x0c03e4b2, 0x0803e4b2, 0x0320f809, 0]) == {0x800f92c8: 1}

def test_current_native_identity_and_closed_direct_topology():
    fresh = audit.audit()
    saved = json.loads((PACKET / 'verification.json').read_text())
    # The receipt is a dated record (2026-10-03). Since then func_800DE860 and
    # the root itself were matched and spliced (2026-10-04, frontier waves 0/2),
    # and the group source the packet used as accepted context was superseded,
    # so live lock state and context identity are no longer compared.
    live = lambda targets: {name: {k: v for k, v in facts.items() if k != 'accepted_lock_present'}
                            for name, facts in targets.items()}
    assert live(fresh['targets']) == live(saved['targets'])
    assert fresh['baseline_source_sha256'] == saved['baseline_source_sha256']
    assert fresh['claims'] == []
    root = fresh['targets']['render_large_objects']
    assert root['native_words'] == 1413
    assert root['entry_frame_bytes'] == 472
    assert root['direct_callers'] == {'render_viewport_init': 1}
    assert {k:v['count'] for k,v in root['direct_callees'].items()} == {'0x800DE860':1, '0x800F92C8':12}
    assert root['indirect_jalr_count'] == 0

def test_research_receipt_does_not_claim_root_or_new_credit():
    receipt = json.loads((PACKET / 'verification.json').read_text())
    assert receipt['claims'] == []
    root = receipt['replay']['render_large_objects']
    assert not root['accepted_object_match']
    assert (root['differing'], root['total']) == (1361,1413)
    assert (root['candidate_elf_size_bytes'],root['trailing_padding_bytes']) == (5272,12)
    assert receipt['targets']['func_800F92C8']['accepted_lock_present']
    assert not receipt['targets']['render_large_objects']['accepted_lock_present']
