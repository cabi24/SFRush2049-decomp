"""Portable packet identity and bounded compiler falsification checks."""
import importlib.util
import json
from pathlib import Path
import shutil
import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_hidden_dust_20261006'
spec = importlib.util.spec_from_file_location('hidden_dust_probe', HERE / 'verify.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)

def receipt():
    return json.loads((HERE / 'verification.json').read_text())

def test_packet_identity():
    assert probe.check_identity(receipt())

def test_control_claims_and_full_extents():
    saved = receipt()
    assert saved['claims'] == []
    for name, row in saved['replay'].items():
        assert row['strict_match'] is False
        assert row['native_bytes'] == 600
        assert row['elf_function_bytes'] < 600
        assert row['missing_words'] == (600 - row['elf_function_bytes']) // 4
        assert row['all_body_relocations_resolved']
        assert row['owned_storage_sections'] == {}
    for route in ('single', 'group'):
        for suffix in ('.c', '_selector.c'):
            assert saved['replay'][route + '/baseline' + suffix] == saved['replay'][route + '/candidate' + suffix]

def test_own_source_mutation_rejected(tmp_path):
    for name in (*probe.SOURCES, 'verify.py'):
        shutil.copyfile(HERE / name, tmp_path / name)
    path = tmp_path / 'candidate.c'
    path.write_text(path.read_text().replace('return pad->disabled;', 'return 0;'))
    with pytest.raises(AssertionError, match='packet source or selected native body changed'):
        probe.check_identity(receipt(), tmp_path)

def test_native_mutation_rejected(monkeypatch):
    original = probe.score.targets()
    changed = dict(original)
    changed[probe.FN] = original[probe.FN].copy()
    changed[probe.FN][0] ^= 1
    monkeypatch.setattr(probe.score, 'targets', lambda: changed)
    with pytest.raises(AssertionError, match='packet source or selected native body changed'):
        probe.check_identity(receipt())

def test_unrelated_native_annotation_does_not_bind(monkeypatch):
    changed = dict(probe.score.targets())
    changed['unrelated_packet_annotation'] = [0]
    monkeypatch.setattr(probe.score, 'targets', lambda: changed)
    assert probe.check_identity(receipt())

def test_fresh_compiler_replay():
    if not (probe.score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    assert probe.compile_replay() == receipt()['replay']
