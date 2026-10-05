"""Explicit CI coverage for adapted sources omitted by check_submissions."""
import importlib.util
import json
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

from tools.cloud import score


ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / "cloud/work/boot_tail_promotion/macro_wrapper_contracts"
SPEC = importlib.util.spec_from_file_location(
    "macro_wrapper_contract_fast_tests", PACKET / "test_verify.py")
fast_tests = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(fast_tests)
# Collect source/extent and post-promotion lifecycle regressions in normal CI.
WrapperContractTests = fast_tests.WrapperContractTests
WrapperLifecycleTests = fast_tests.WrapperLifecycleTests


def test_actual_tu_macro_wrapper_contracts():
    if not (score.IDO / "cc").is_file():
        pytest.skip("IDO unavailable")
    for tool in ("mips-linux-gnu-as", "mips-linux-gnu-objdump", "cc"):
        if not shutil.which(tool):
            pytest.skip("C compiler unavailable" if tool == "cc" else tool + " unavailable")
    # Isolate the verifier's boot-tail target selection from the other scorers.
    completed = subprocess.run(
        [sys.executable, str(PACKET / "verify.py")], cwd=ROOT,
        capture_output=True, text=True, timeout=180)
    assert completed.returncode == 0, completed.stdout + completed.stderr
    report = json.loads(completed.stdout)
    assert report["result"] == "PASS"
    assert report["candidate_functions"] == 8
    assert report["candidate_body_bytes"] == 352
    assert report["baseline_existing_locked_functions"] == 15
    assert report["tu_function_symbols"] == 66
    assert report["all_tu_function_offsets_unchanged"]
    assert report["unverified_passthrough_and_padding_bytes_unchanged"]
    assert all(row["all_full_relocated_bytes_equal"] for row in report["results"])
    assert report["host_semantics"] == {"wrapper_count": 9, "cases": 36, "passed": True}
