"""Read-only source-bound regressions for selector call-site classifications."""
import copy
import hashlib
import importlib.util
import json
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_slot_contract_audit_20261006'
SPEC = importlib.util.spec_from_file_location('slot_contract_audit', PACKET / 'audit.py')
audit = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(audit)


def receipt():
    return json.loads((PACKET / 'evidence.json').read_text())


def test_fresh_source_bound_census():
    assert audit.verify() == receipt()


def test_counts_are_bounded_dependency_surface_not_matches():
    result = receipt()['summary']
    assert (result['callers'], result['sites'], result['adjacent_call_label_pairs']) == (65, 170, 167)
    assert sum(result['captures'].values()) == 161
    assert result['accepted_callers'] == ['credits_scroll']
    assert result['unaccepted_caller_bytes'] == 74428
    assert len(result['only_selector_unaccepted_direct_game_dependency']) == 8
    assert result['bounded_dependency_bytes'] == 5368


@pytest.mark.parametrize('name,site', [
    ('particle_render', 0x800B8670),
    ('func_800D9058', 0x800D9098),
    ('game_results_render', 0x800FF434),
    ('func_80106B3C', 0x80106C48),
    ('func_8010EA08', 0x8010EB00),
    ('func_8010F218', 0x8010F954),
])
def test_six_sites_are_real_selector_calls_without_capture(name, site):
    words, symbols = audit.score.targets()[name], audit.score.image_symbols()
    assert audit.inspect_site(words, symbols[name], site, symbols['slot_state_setup'],
                              symbols['osRecvMesg'], symbols['osJamMesg'])


@pytest.mark.parametrize('delta', [-1, 2, 4, 8, 0x1000])
def test_wrong_native_site_is_rejected(delta):
    symbols = audit.score.image_symbols()
    with pytest.raises(audit.AuditError, match='wrong site'):
        audit.inspect_site(audit.score.targets()['func_800D9058'], symbols['func_800D9058'],
                           0x800D9098 + delta, symbols['slot_state_setup'],
                           symbols['osRecvMesg'], symbols['osJamMesg'])


def test_known_captured_site_cannot_be_labeled_no_copy():
    symbols = audit.score.image_symbols()
    with pytest.raises(audit.AuditError, match='captures selector return'):
        audit.inspect_site(audit.score.targets()['credits_scroll'], symbols['credits_scroll'],
                           0x800D67E0, symbols['slot_state_setup'],
                           symbols['osRecvMesg'], symbols['osJamMesg'])


def test_unpaired_selector_call_is_not_an_adjacent_wrapper():
    symbols = audit.score.image_symbols()
    with pytest.raises(audit.AuditError, match='lacks adjacent'):
        audit.inspect_site(audit.score.targets()['world_trigger_check'], symbols['world_trigger_check'],
                           0x800EDD54, symbols['slot_state_setup'],
                           symbols['osRecvMesg'], symbols['osJamMesg'])


def test_wrong_saved_site_fails_replay():
    saved = receipt()
    saved['no_copy_sites'][1]['address'] = '0x800D909C'
    with pytest.raises(audit.AuditError, match='no_copy_sites'):
        audit.verify(saved)


def test_actual_source_edit_fails_digest_binding(tmp_path, monkeypatch):
    for name in audit.SOURCES:
        target = tmp_path / name
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes((ROOT / name).read_bytes())
    changed = tmp_path / 'cloud/work/s20261004/D/notes.md'
    changed.write_bytes(changed.read_bytes() + b'\nChanged historical claim.\n')
    original = audit.source_hashes
    monkeypatch.setattr(audit, 'source_hashes', lambda: original(tmp_path))
    with pytest.raises(audit.AuditError, match='source_sha256'):
        audit.verify()


def test_selected_native_change_fails_body_binding(monkeypatch):
    native = copy.deepcopy(audit.score.targets())
    # Change a selection immediate, leaving the JAL site classification unchanged.
    index = (0x800D9098 - 0x800D9058) // 4
    native['func_800D9058'][index + 1] ^= 1
    monkeypatch.setattr(audit.score, 'targets', lambda: native)
    with pytest.raises(audit.AuditError, match='callers'):
        audit.verify()


def test_selected_helper_change_fails_body_binding(monkeypatch):
    native = copy.deepcopy(audit.score.targets())
    native['slot_state_setup'][-1] ^= 1
    monkeypatch.setattr(audit.score, 'targets', lambda: native)
    with pytest.raises(audit.AuditError, match='helpers'):
        audit.verify()


def test_changed_acceptance_status_changes_bound(monkeypatch):
    old_read = Path.read_text
    def changed(path, *args, **kwargs):
        text = old_read(path, *args, **kwargs)
        if path == ROOT / 'blob_matched.lock.json':
            locks = json.loads(text)
            locks['func_800D9058'] = {'synthetic_test_only': True}
            return json.dumps(locks)
        return text
    monkeypatch.setattr(Path, 'read_text', changed)
    with pytest.raises(audit.AuditError, match='summary'):
        audit.verify()


def test_reader_rechecks_manifest_after_warm_cache(tmp_path, monkeypatch):
    audit.score.targets()
    region = b'.section .text.synthetic,"ax"\n.word 0x00000000\n'
    symbols = b'{"symbols":{"synthetic":"0x80000000"}}\n'
    (tmp_path / 'synthetic.s').write_bytes(region)
    (tmp_path / 'symbols.json').write_bytes(symbols)
    manifest = '%s  synthetic.s\n%s  symbols.json\n' % (
        hashlib.sha256(region).hexdigest(), hashlib.sha256(symbols).hexdigest())
    (tmp_path / 'SHA256SUMS').write_text(manifest)
    monkeypatch.setattr(audit.score, 'ASM_DIR', tmp_path)
    assert audit.score.targets() == {'synthetic': [0]}
    (tmp_path / 'synthetic.s').write_bytes(region + b'\n')
    with pytest.raises(SystemExit, match='SHA-256 mismatch'):
        audit.collect()


def test_scanner_stops_after_v0_is_redefined():
    # Synthetic lui v0 followed by move s0,v0 must not count as a selector result.
    words = [0, 0, (15 << 26) | (2 << 16), (2 << 21) | (16 << 11) | 37, 3 << 26, 0]
    assert audit.return_capture(words, 0, 4) is None
