"""Honest research status, pinned full extents and executable donor regressions."""
import importlib.util
import json
from pathlib import Path
import shutil
import struct
import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_quadtree_donor_20261005'
spec=importlib.util.spec_from_file_location('quadtree_verify',HERE/'verify.py')
proof=importlib.util.module_from_spec(spec);spec.loader.exec_module(proof)


def receipt():
    row=json.loads((HERE/'verification.json').read_text())
    assert row['source_sha256']==proof.sha(proof.SOURCE.read_bytes())
    assert row['accepted_context_sha256']==proof.sha(proof.ACCEPTED.read_bytes())
    for name,digest in row['source_files'].items(): assert proof.sha((HERE/name).read_bytes())==digest
    for name,digest in row['verifier_sha256'].items(): assert proof.sha((HERE/name).read_bytes())==digest
    native=b''.join(struct.pack('>I',w) for w in proof.score.targets()[proof.FN])
    assert proof.sha(native)==row['native_target_sha256']
    return row


def test_source_bound_nonmatch_and_extent():
    row=receipt()
    assert row['status']=='NONMATCH' and row['claims']==[]
    result=row['controls']['candidate']
    assert result['symbol_bytes']==result['native_bytes']==224
    assert result['differing']==result['full_extent_differing']==19
    assert result['extra_words']==result['full_extent_extra_words']==0
    assert not(result['unverified'] or result['unresolved'] or result['errors'])
    assert result['gnu_full_body_equals_project_relocation'] and result['own_data_bytes']==0
    assert len(result['relocations'])==2


def test_donor_boundary_and_real_wrapper_regression():
    row=receipt();groups=row['genuine_wrapper_context']
    assert groups['archive'][proof.FN]['differing']==52
    assert groups['candidate'][proof.FN]['differing']==19
    for result in groups.values():
        assert result[proof.WRAPPER]['verdict']=='MATCH'
        assert result[proof.WRAPPER]['symbol_bytes']==216
    # Canonical comparison ignores this zero word beyond the target, but full ELF proof must retain it.
    row=row['controls']['donor_iterative']
    assert row['extra_words']==0 and row['full_extent_extra_words']==1 and row['symbol_bytes']==228


def test_semantic_edges_and_fail_closed_execution():
    words=proof.score.targets()[proof.FN]
    for case in list(proof.cases())[:484]:
        assert proof.native.Machine(words,**case).run()==proof.oracle(case)
    sample=next(proof.cases());bad=list(words);bad[0]=0xFFFFFFFF
    with pytest.raises(AssertionError,match='unsupported'): proof.native.Machine(bad,**sample).run()


def test_fresh_compiler_and_complete_behavior(tmp_path):
    if not (proof.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    rows,groups,words,_=proof.compiler_proofs(tmp_path)
    expected=receipt()
    assert rows==expected['controls']
    assert groups==expected['genuine_wrapper_context']
    assert proof.behavior(tmp_path,words)==expected['behavior']


def test_wrong_source_binding_rejected(monkeypatch):
    actual=proof.sha
    monkeypatch.setattr(proof,'sha',lambda b:'0'*64 if b==proof.SOURCE.read_bytes() else actual(b))
    with pytest.raises(AssertionError):receipt()
