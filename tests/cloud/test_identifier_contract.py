"""Identifier repair checks are explicit: changed-submission CI omits ROM TUs."""
import importlib.util
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location(
    "identifier_contract_verify",
    ROOT / "cloud/work/boot_tail_promotion/identifier_contract/verify.py")
verify = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(verify)


def test_overlay_preserves_every_other_part_of_real_tu():
    text = verify.checked(["git", "show", verify.BASE + ":" + verify.TU])
    text = text.replace(verify.OLD_DECL, verify.NEW_DECL)
    source = (ROOT / verify.SOURCE).read_text()
    combined = verify.overlay(text, source)
    assert combined.replace(source.rstrip(), verify.PRAGMA, 1) == text
    assert "extern int func_8001EDF4(unsigned int);" in combined
    assert verify.NEW_DECL in combined
    assert verify.OLD_DECL not in combined


@pytest.mark.parametrize("source", ["", verify.PRAGMA + "\n" + verify.PRAGMA])
def test_overlay_refuses_missing_or_duplicate_slot(source):
    with pytest.raises(verify.VerificationError, match="exactly one"):
        verify.overlay(source, "unsigned int candidate(void) { return 0; }")


@pytest.mark.parametrize("size", [44, 52])
def test_exact_extent_rejects_short_body_and_zero_padding_disguised_as_body(size):
    symbols = {verify.CANDIDATE: {"value": 0, "size": size},
               "neighbor": {"value": 48, "size": 4}}
    with pytest.raises(verify.VerificationError, match="extent differs"):
        verify.exact_extent(symbols, verify.CANDIDATE, 48)


def test_exact_extent_rejects_neighbor_overlap():
    symbols = {verify.CANDIDATE: {"value": 0, "size": 48},
               "neighbor": {"value": 44, "size": 4}}
    with pytest.raises(verify.VerificationError, match="neighbor"):
        verify.exact_extent(symbols, verify.CANDIDATE, 48)


@pytest.mark.parametrize("uncertainty", range(4))
def test_relocation_verification_refuses_every_uncertainty(monkeypatch, uncertainty):
    monkeypatch.setattr(verify, "function_symbols", lambda _: (
        {verify.CANDIDATE: {"value": 0, "size": 48}}, []))
    monkeypatch.setattr(verify.score, "text_words", lambda _: [0] * 12)
    flags = [{}, [], [], []]
    flags[uncertainty] = {0: 0xFFFF0000} if uncertainty == 0 else ["uncertain"]
    monkeypatch.setattr(verify.score, "relocate", lambda *args: ([0] * 12, *flags))
    with pytest.raises(verify.VerificationError, match="incomplete relocation"):
        verify.relocated_bytes("unused.o", verify.CANDIDATE, 48, {})


def test_actual_combined_tu_preserves_all_accepted_bodies_and_candidate(tmp_path):
    if not (verify.score.IDO / "cc").is_file():
        pytest.skip("IDO unavailable")
    if not all(shutil.which(n) for n in (
        "mips-linux-gnu-as", "mips-linux-gnu-ld", "mips-linux-gnu-objcopy")):
        pytest.skip("MIPS binutils unavailable")
    old_target_dir = verify.score.ASM_DIR
    result = verify.verify(tmp_path)
    assert verify.score.ASM_DIR == old_target_dir
    assert result["negative_control"]["rejected"]
    assert result["standalone_candidate"] == {"function_bytes": 48, "match": True}
    hashes = set()
    for name, count in (("baseline", 17), ("repaired", 17), ("candidate_overlay", 18)):
        stage = result["stages"][name]
        assert len(stage["functions"]) >= count
        assert stage["all_tu_slots_verified"] == 24
        hashes.add(stage["linked_text_sha256"])
        for row in stage["functions"]:
            assert row["function_bytes"] == row["target_bytes"]
            assert row["target_sha256"] == row["relocated_sha256"] == row["gnu_linked_sha256"]
    assert len(hashes) == 1
    assert result["rom_gate"].startswith("not run")
