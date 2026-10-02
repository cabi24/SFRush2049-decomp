"""Coordinator proof protocol; full ELF/relocation cases live in test_cloud_score."""
import hashlib
import json

import pytest

from tools.cloud import review_single, score


@pytest.fixture
def replay(tmp_path, monkeypatch):
    source = tmp_path / "f.c"
    source.write_text("/* flags: -g0 -O3 -mips2 -G 0 -non_shared */\nvoid f(void) {}\n")
    (tmp_path / "SHA256SUMS").write_text("frozen target manifest\n")
    monkeypatch.setattr(score, "ASM_DIR", tmp_path)
    monkeypatch.setattr(score, "targets", lambda: {"f": [0, 0]})
    monkeypatch.setattr(score, "compile_single", lambda s, f, o: o.write_bytes(b"fresh object"))
    monkeypatch.setattr(score, "compare", lambda o, n, show: score.Comparison(0, 2, [], [], []))
    return source


def test_proof_binds_literal_flags_source_and_target_manifest(replay):
    proof = review_single.review(replay, "f", 8)
    assert proof["object_accepted"] and proof["canonical_verdict"] == "MATCH"
    assert proof["flags"] == "-g0 -O3 -mips2 -G 0 -non_shared"
    assert proof["source_sha256"] == hashlib.sha256(replay.read_bytes()).hexdigest()
    assert proof["target_manifest_sha256"] == hashlib.sha256(
        (replay.parent / "SHA256SUMS").read_bytes()).hexdigest()


@pytest.mark.parametrize("comparison", [
    score.Comparison(1, 2, [], [], []),
    score.Comparison(0, 2, ["callee"], [], []),
    score.Comparison(0, 2, [], ["local data"], []),
    score.Comparison(0, 2, [], [], ["bad relocation"]),
    score.Comparison(0, 2, [], [], [], 1),
])
def test_incomplete_object_proof_is_rejected(replay, monkeypatch, comparison):
    monkeypatch.setattr(score, "compare", lambda o, n, show: comparison)
    assert not review_single.review(replay, "f", 8)["object_accepted"]


def test_wrong_extent_is_rejected_before_compiling(replay, monkeypatch):
    def unexpected_compile(*args):
        pytest.fail("wrong extent reached compiler")
    monkeypatch.setattr(score, "compile_single", unexpected_compile)
    with pytest.raises(ValueError, match="extent"):
        review_single.review(replay, "f", 12)


@pytest.mark.parametrize("changed", ["source", "manifest"])
def test_inputs_changing_during_compile_are_rejected(replay, monkeypatch, changed):
    def compile_and_change(source, flags, obj):
        obj.write_bytes(b"fresh object")
        path = source if changed == "source" else source.parent / "SHA256SUMS"
        path.write_text(path.read_text() + "changed\n")
    monkeypatch.setattr(score, "compile_single", compile_and_change)
    with pytest.raises(ValueError, match="changed during"):
        review_single.review(replay, "f", 8)


def test_failed_compile_replaces_stale_success_proof(replay, monkeypatch):
    output = replay.parent / "proof.json"
    output.write_text('{"object_accepted": true}\n')
    def failed_compile(*args):
        raise SystemExit("IDO compile failed")
    monkeypatch.setattr(score, "compile_single", failed_compile)
    assert review_single.main([str(replay), "f", "--expected-bytes", "8",
                               "--output", str(output)]) == 1
    proof = json.loads(output.read_text())
    assert not proof["object_accepted"] and proof["error"] == "IDO compile failed"


def test_publication_header_cannot_include_trailing_annotation(replay):
    replay.write_text("/* flags: -O3 */ extra text\nvoid f(void) {}\n")
    with pytest.raises(ValueError, match="exact"):
        review_single.review(replay, "f", 8)
