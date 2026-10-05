"""Fail-closed checks for the non-matching vector-blend research packet."""
import importlib.util
import json
from pathlib import Path
import hashlib

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / "cloud/work/frontier/dot_vector_blend"
spec = importlib.util.spec_from_file_location("dot_vector_blend_replay", PACKET / "replay.py")
replay = importlib.util.module_from_spec(spec)
spec.loader.exec_module(replay)


def test_packet_is_explicit_nonmatching_research():
    receipt = json.loads((PACKET / "verification.json").read_text())
    assert receipt["status"] == "NON_MATCHING_RESEARCH"
    assert receipt["claims"] == []
    assert receipt["wrong_literal_negative_control_refused"] is True
    assert len(receipt["cases"]) == 6
    for case in receipt["cases"]:
        assert case["elf_function_size_bytes"] == case["target_extent_bytes"] == 448
        assert case["full_relocated_words_equal"] is False
        assert case["comparison"]["total"] == 112
        assert not any(case["comparison"][k] for k in
                       ("unresolved", "errors", "extra_words"))
        best = case["case"].startswith("best")
        assert len(case["comparison"]["unverified"]) == (2 if best else 0)
        assert case["own_data_verified_sites"] == (["0x078", "0x07C"] if best else [])
        assert case["remaining_unverified_after_own_data_proof"] == 0
        expected = ["0x160"] if case["case"].startswith("best") else ["0x114", "0x118", "0x160"]
        assert case["residual_offsets"] == expected
        assert case["comparison"]["differing"] == len(expected)


def test_receipt_is_bound_to_unchanged_source_inputs():
    receipt = json.loads((PACKET / "verification.json").read_text())
    for filename, expected in receipt["source_inputs"].items():
        assert hashlib.sha256((ROOT / filename).read_bytes()).hexdigest() == expected
    assert hashlib.sha256((ROOT / "asm/us/blob_data/SHA256SUMS").read_bytes()).hexdigest() == receipt["own_data_manifest_sha256"]
    assert hashlib.sha256((ROOT / "tools/cloud/score.py").read_bytes()).hexdigest() == receipt["scorer_sha256"]
    assert hashlib.sha256((ROOT / "asm/us/blob/SHA256SUMS").read_bytes()).hexdigest() == receipt["target_manifest_sha256"]


@pytest.mark.parametrize("size", [444, 452, 464])
def test_replay_refuses_truncated_or_padded_elf_extent(monkeypatch, size):
    monkeypatch.setattr(replay.score, "targets", lambda: {replay.NAME: [0] * 112})
    monkeypatch.setattr(replay, "extent", lambda obj, name: (0, size))
    with pytest.raises(AssertionError, match="wrong full ELF extent"):
        replay.inspect_object("not-opened.o", [0x160])


def test_reused_helper_has_only_real_accepted_body():
    source = replay.helper_source()
    accepted = replay.HELPER.read_text()
    body = source[source.index("void func_800E8CB8("):].rstrip()
    assert body in accepted
    assert "__standin" not in source
    assert "M2C_ERROR" not in source
    assert source.count("\nvoid ") == 1
