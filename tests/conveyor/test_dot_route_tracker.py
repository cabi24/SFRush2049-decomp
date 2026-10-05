"""Bounded route-tracker research: source binding, truthful layout, replay."""
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / "cloud/work/frontier/dot_route_tracker"


def read(name):
    return json.loads((PACKET / name).read_text())


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def test_research_claims_and_native_extents():
    claim = read("claim.json")
    assert claim["claims"] == read("verification.json")["claims"] == []
    extents = {r["name"]: int(r["end"], 16) - int(r["start"], 16)
               for r in claim["targets"]}
    assert extents == {"audio_mixer_main": 732, "audio_priority_find": 432}


def test_truthful_header_record_capacity():
    layout = read("layout.json")
    assert layout["header_size"] + layout["record_capacity"] * layout["record_size"] == 812
    assert int(layout["address"], 16) + 12 == int(layout["record_address"], 16)
    assert int(layout["address"], 16) + 812 == int(layout["end_address"], 16)
    assert layout["record"]["crossing_indices_s16_20"] + 20 * 2 == 74
    assert layout["record"]["segment_length_f32"] + 4 == layout["record_size"]
    assert {p["function"] for p in layout["capacity_proof"]} == {
        "audio_dsp_process", "audio_effect_apply", "func_800F8EC8"}


def test_source_and_context_hash_binding():
    receipt = read("verification.json")
    lock = json.loads((ROOT / "blob_matched.lock.json").read_text())
    for name, expected in receipt["sources"].items():
        assert digest(ROOT / name) == expected
    for name in receipt["recipe"]["context"]:
        assert receipt["sources"][lock[name]["source"]] == lock[name]["source_sha256"]
    # Scorer/owndata hashes are a frozen provenance snapshot, not locks on
    # future work (owndata gained .bss placement on 2026-10-05).
    for name, expected in read("host_verification.json")["source_sha256"].items():
        assert digest(PACKET / name) == expected


def test_evidence_levels_remain_separate():
    proof = read("verification.json")
    rows = proof["results"]
    mixer = rows["audio_mixer_main"]
    assert mixer["comparison"]["differing"] == 64
    assert mixer["comparison"]["extra_words"] == 1
    assert mixer["symbol_slice_bytes"] == 736
    assert mixer["entry_stack_adjust"] == -72
    assert not mixer["canonical_strict_match"]
    assert rows["audio_priority_find"]["canonical_strict_match"]
    assert "audio_priority_find" not in proof["claims"]
    for name in ["func_800BA2B8", "func_800BA61C"]:
        assert rows[name]["comparison"]["differing"] == 0
        assert rows[name]["comparison"]["unverified"] == []
        assert rows[name]["canonical_strict_match"]
        assert rows[name]["own_data"]["ok"]
    assert proof["diagnosis"]["causal_next_variant"] is None


def test_paired_existing_suite_receipt():
    receipt = read("baseline_tests.json")
    assert receipt["results_identical"]
    assert receipt["results"]["base"] == receipt["results"]["current"]
    assert receipt["results"]["base"]["tests"] == 669
    assert receipt["results"]["base"]["passed"] == 669
    assert receipt["results"]["base"]["failed"] == 0
    history = read("baseline_tests_historical.json")
    assert history["status"] == "historical superseded baseline"
    assert history["results"]["base"]["failed"] == 21


def test_fresh_pinned_compiler_replay():
    ido = Path(os.environ.get("IDO_DIR", ROOT / "tools/cloud/ido"))
    if not (ido / "cc").is_file() or not shutil.which("mips-linux-gnu-as"):
        pytest.skip("IDO or GNU MIPS tools unavailable; source-bound tests still run")
    subprocess.run([sys.executable, str(PACKET / "verify.py")], cwd=ROOT, check=True)


def test_sanitized_host_semantics():
    if not shutil.which("gcc"):
        pytest.skip("GCC unavailable; host sanitizer test not run")
    subprocess.run([sys.executable, str(PACKET / "host_tests.py")], cwd=ROOT, check=True)
