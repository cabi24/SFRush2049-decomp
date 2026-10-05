"""Heap maximum: full ELF/source contracts and deliberately bounded semantics."""
import importlib.util
import json
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_heap_max_20261005'
SPEC = importlib.util.spec_from_file_location('heap_max_verify', HERE / 'verify.py')
v = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(v)


def receipt():
    return json.loads((HERE / 'verification.json').read_text())


def test_frozen_scope_and_complete_extent():
    saved = receipt()
    assert saved['status'] == 'MATCH'
    assert saved['new_candidate_bytes'] == 160 and saved['accepted_byte_gain'] == 0
    assert saved['candidate']['elf_function_bytes'] == 160
    assert saved['candidate']['comparison'] == dict(differing=0, total=40, unresolved=[], unverified=[], errors=[], extra_words=0)
    assert saved['existing_stub']['bytes'] == 8
    assert saved['gnu_link']['excluded_prefix_bytes'] == 8
    assert saved['gnu_link']['own_data_bytes'] == 0


def test_every_frozen_source_is_bound():
    for name, digest in receipt()['sources'].items():
        assert v.sha((HERE / name).read_bytes()) == digest


def test_all_context_claims_and_nonmatch_are_explicit():
    saved = receipt()['context']
    assert len(saved['frontier_heap_free_total']['members']) == 2
    assert len(saved['audio_heap']['members']) == 5
    for context in saved.values():
        assert context['accepted_prefix_unchanged']
        assert context['candidate']['comparison']['differing'] == 0
        for fn in context['members'].values():
            assert fn['comparison']['differing'] == fn['comparison']['extra_words'] == 0
    old = saved['audio_heap']['preexisting_nonmatches']['func_800E7D0C']
    assert old['complete_body_unchanged']
    assert old['baseline']['relocated_sha256'] == old['combined']['relocated_sha256']
    assert old['combined']['comparison']['differing'] == 15


def test_native_boundaries_and_all_instructions():
    words = v.score.targets()[v.FN]
    visited = set()
    # Fixed boundary prefix plus empty heaps. Full 4,588-case corpus is replayed below.
    for case in v.behavior.cases()[:492]:
        _, offsets = v.behavior.execute(words, case)
        visited.update(offsets)
    assert visited == set(range(0, 160, 4))


@pytest.mark.parametrize('size', [156, 164])
def test_wrong_elf_extent_is_rejected(monkeypatch, size):
    monkeypatch.setattr(v, 'symbols', lambda _: {v.FN: {'value': 0, 'size': size}})
    with pytest.raises(AssertionError, match='ELF extent differs'):
        v.exact('unused.o', v.FN)


@pytest.mark.parametrize('index', range(4))
def test_relocation_uncertainty_is_rejected(monkeypatch, index):
    monkeypatch.setattr(v, 'symbols', lambda _: {v.FN: {'value': 0, 'size': 160}})
    monkeypatch.setattr(v.score, 'text_words', lambda _: [0] * 40)
    uncertainty = [{}, [], [], []]
    uncertainty[index] = {0: 0xFFFF0000} if index == 0 else ['unproved']
    monkeypatch.setattr(v.score, 'relocate', lambda *args: ([0] * 40, *uncertainty))
    with pytest.raises(AssertionError, match='incomplete relocation'):
        v.exact('unused.o', v.FN)


def test_fresh_complete_compiler_gnu_host_native_proof(tmp_path):
    if not (v.score.IDO / 'cc').is_file():
        pytest.skip('Pinned IDO unavailable')
    if not all(shutil.which(n) for n in ['cc', 'mips-linux-gnu-ld', 'mips-linux-gnu-objcopy']):
        pytest.skip('Host/MIPS tools unavailable')
    result = v.verify(tmp_path)
    v.compare_receipt(result, receipt())
    assert result['native_behavior']['cases'] == 4588
    assert result['native_behavior']['native_executions'] == 13764
    assert all(row['discriminating_cases'] > 0 for row in result['host_behavior']['mutations_rejected'].values())


def test_receipt_tolerates_only_unrelated_manifest_annotation():
    saved = receipt()
    current = dict(saved, target_manifest_sha256='fresh-validated-manifest')
    v.compare_receipt(current, saved)
    changed = json.loads(json.dumps(current))
    changed['candidate']['target_sha256'] = 'changed-body'
    with pytest.raises(AssertionError, match='receipt differs'):
        v.compare_receipt(changed, saved)
