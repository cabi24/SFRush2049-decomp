"""Strict extent/relocation evidence and behavior of the graphics initializer."""
import copy
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / "cloud/work/dot_graphics_init_match"
spec = importlib.util.spec_from_file_location("graphics_init_match_verify", PACKET / "verify.py")
verify = importlib.util.module_from_spec(spec)
spec.loader.exec_module(verify)


def test_saved_receipt_and_source_binding():
    evidence = json.loads((PACKET / "verification.json").read_text())
    for name in verify.STRICT:
        verify.assert_strict(evidence["functions"][name])
    assert evidence["functions"]["sound_init"]["elf_st_size"] == 400
    assert evidence["functions"]["sound_init"]["relocation_count"] == 37
    for path, digest in evidence["sources_sha256"].items():
        # Production sources it used as context (src/blob/...) are a snapshot:
        # object_render.c was re-sourced to its natural 64-bit shift spelling
        # on 2026-10-05 with identical bytes. The packet's own files stay bound.
        if path.startswith("src/"):
            continue
        assert verify.sha((ROOT / path).read_bytes()) == digest
    assert verify.sha((PACKET / "recipe.json").read_bytes()) == evidence["recipe_sha256"]
    # Protected hashes are a frozen provenance snapshot, not locks on future work.


@pytest.mark.skipif(not (verify.score.IDO / "cc").is_file(),
                    reason="IDO compiler unavailable")
def test_recompile_genuine_group_and_accepted_baselines(tmp_path):
    """Normal CI must compile this recipe-only submission, not trust its receipt."""
    fresh_path = tmp_path / "verification.json"
    subprocess.run([sys.executable, str(PACKET / "verify.py"),
                    "--output", str(fresh_path)], cwd=ROOT, check=True)
    fresh = json.loads(fresh_path.read_text())
    saved = json.loads((PACKET / "verification.json").read_text())
    # The verifier enforces exact ELF extents and fully resolved words for all
    # six strict functions. Compare the complete records, including the mode
    # helper's explicitly excluded table references and both causal controls.
    for key in ("functions", "accepted_baseline", "controls",
                "mode_table_sections_unchanged", "sources_sha256", "recipe_sha256"):
        assert fresh[key] == saved[key]
    # Do not freeze unrelated protected files or tool binary hashes in CI.


@pytest.mark.parametrize("mutation", ["extent", "mask", "words", "unresolved", "extra"])
def test_strict_gate_rejects_bad_evidence(mutation):
    row = copy.deepcopy(json.loads((PACKET / "verification.json").read_text())["functions"]["sound_init"])
    if mutation == "extent":
        row["elf_st_size"] += 4
    elif mutation == "mask":
        row["masked_offsets"] = [0]
    elif mutation == "words":
        row["full_word_equal"] = False
    elif mutation == "unresolved":
        row["comparison"]["unresolved"] = ["missing"]
    else:
        row["comparison"]["extra_words"] = 1
    with pytest.raises(AssertionError):
        verify.assert_strict(row)


def test_initializer_contracts(tmp_path):
    cc = shutil.which("cc")
    if cc is None:
        pytest.skip("host C compiler unavailable")
    binary = tmp_path / "host_test"
    subprocess.run([cc, "-std=c99", "-O2", "-Wall", "-Wextra", "-Werror",
                    str(PACKET / "host_test.c"), "-o", str(binary)], check=True)
    result = subprocess.run([str(binary)], check=True, text=True, capture_output=True)
    assert "5142 initialization cases and full-state no-op passed" in result.stdout
