"""Regression checks for conservative D02 scout metadata (no matching claims)."""
import importlib.util
from pathlib import Path
import json
ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT/'cloud/work/dot_substantial_scout'
spec = importlib.util.spec_from_file_location('substantial_scout', PACKET/'preflight.py')
scout = importlib.util.module_from_spec(spec)
spec.loader.exec_module(scout)

def test_jal_uses_pc_region_and_excludes_plain_jump():
    assert scout.direct_calls([0x0c000004, 0x08000004], 0x80001000) == [(0x80001000, 0x80000010)]
    assert scout.direct_calls([0x0c000004], 0x8ffffffc) == [(0x8ffffffc, 0x90000010)]

def test_frame_only_before_control_transfer():
    assert scout.entry_frame([0x3c010001, 0x27bdfe78]) == 392
    assert scout.entry_frame([0x03e00008, 0x27bdffd8]) is None
    assert scout.entry_frame([0x27bd0028]) is None
    for branch in (0x50000000, 0x54000000, 0x58000000, 0x5c000000, 0x45000000):
        assert scout.entry_frame([branch, 0x27bdffd8]) is None

def test_native_metadata_replays_complete_current_extents():
    result = scout.audit()
    assert result == json.loads((PACKET/'native_audit.json').read_text())
    assert result['claims'] == []
    assert all(not row['accepted_name_lock'] for row in result['targets'])
    assert sum(row['native_bytes'] for row in result['targets']) == 12788

def test_registered_caller_corrects_old_bigfish_report():
    rows = {r['target']:r for r in scout.audit()['targets']}
    assert rows['func_80100E58']['direct_callers'] == ['func_8010221C']
    assert rows['func_8010C974']['entry_frame_bytes'] == 392
    assert any(c['address'] == '0x8038D798' and c['names'] == [] for c in rows['func_8010C974']['callees'])

def test_modified_symbol_bytes_fail_closed(monkeypatch):
    original = Path.read_bytes
    def read_bytes(path):
        data = original(path)
        if path == ROOT/'asm/us/blob/symbols.json':
            return data + b' '
        return data
    monkeypatch.setattr(Path, 'read_bytes', read_bytes)
    import pytest
    with pytest.raises(SystemExit, match='SHA-256 mismatch'):
        scout.audit()
