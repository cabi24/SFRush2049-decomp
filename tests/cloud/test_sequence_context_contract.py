"""Explicit source-adapter checks; the changed-submission selector omits these files."""
import importlib.util
import os
from pathlib import Path
import shutil
import subprocess

import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / "cloud/work/boot_tail_promotion/sequence_context_contract"
SPEC = importlib.util.spec_from_file_location("sequence_context_verify", HERE / "verify.py")
verify = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(verify)


def test_adapter_preserves_function_order_and_passthrough_slots():
    original = verify.checked(["git", "show", verify.BASE + ":" + verify.TU])
    adapted = verify.adapt_context(original)
    assert list(verify.bodies(adapted)) == list(verify.bodies(original))
    assert [s for s in original.splitlines() if s.startswith("#pragma")] == [
        s for s in adapted.splitlines() if s.startswith("#pragma")]
    assert "extern SequenceContext *D_8004BE80;" in adapted
    assert "extern SequenceContext D_80043EB8[8];" in adapted
    assert "void func_8001734C(SequenceContext *context)" in adapted
    assert "context->active" in adapted and "context->pending" in adapted
    assert verify.adapt_context(adapted) == adapted


def test_overlay_supports_later_legitimate_promotion():
    original = verify.checked(["git", "show", verify.BASE + ":" + verify.TU])
    adapted = verify.adapt_context(original)
    for name in verify.CANDIDATES:
        source = (HERE / "sources" / (name + ".c")).read_text()
        adapted = verify.overlay(adapted, name, source)
        assert verify.overlay(adapted, name, source) == adapted
    assert set(verify.CANDIDATES) <= set(verify.bodies(adapted))


@pytest.mark.parametrize("copies", [0, 2])
def test_overlay_refuses_missing_or_duplicate_slot(copies):
    name = verify.CANDIDATES[0]
    pragma = '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_17dc0/' + name + '.s")\n'
    with pytest.raises(verify.VerificationError, match="exactly one"):
        verify.overlay(pragma * copies, name, (HERE / "sources" / (name + ".c")).read_text())


@pytest.mark.parametrize("size", [100, 108])
def test_exact_extent_rejects_short_and_padded_bodies(size):
    name = verify.CANDIDATES[0]
    with pytest.raises(verify.VerificationError, match="extent differs"):
        verify.exact_extent({name: {"size": size, "value": 0}}, name, 104)


def test_exact_extent_rejects_neighbor_overlap():
    name = verify.CANDIDATES[0]
    symbols = {name: {"size": 104, "value": 0}, "next": {"size": 4, "value": 100}}
    with pytest.raises(verify.VerificationError, match="neighbor"):
        verify.exact_extent(symbols, name, 104)


@pytest.mark.parametrize("uncertainty", range(4))
def test_relocation_check_rejects_masks_and_unverified_references(monkeypatch, uncertainty):
    name = verify.CANDIDATES[0]
    monkeypatch.setattr(verify, "function_symbols", lambda _: ({name: {"size": 104, "value": 0}}, []))
    monkeypatch.setattr(verify.score, "text_words", lambda _: [0] * 26)
    flags = [{}, [], [], []]
    flags[uncertainty] = {0: 0xFFFF0000} if uncertainty == 0 else ["uncertain"]
    monkeypatch.setattr(verify.score, "relocate", lambda *args: ([0] * 26, *flags))
    with pytest.raises(verify.VerificationError, match="incomplete relocation"):
        verify.relocated_bytes("unused.o", name, 104, {})


def test_actual_tu_and_every_adapted_standalone_source(tmp_path):
    if not (verify.score.IDO / "cc").is_file():
        pytest.skip("IDO unavailable")
    if not all(shutil.which(n) for n in (
        "mips-linux-gnu-as", "mips-linux-gnu-ld", "mips-linux-gnu-objcopy")):
        pytest.skip("MIPS binutils unavailable")
    old = verify.score.ASM_DIR
    result = verify.verify(tmp_path)
    assert verify.score.ASM_DIR == old
    assert result["candidate_bytes"] == 1596
    assert len(result["standalone_candidates"]) == 7
    assert all(row["match"] for row in result["standalone_candidates"].values())
    assert len(result["negative_controls"]) == 9
    assert all(row["rejected"] for row in result["negative_controls"].values())
    assert len(result["stages"]["baseline"]["functions"]) == 16
    assert len(result["stages"]["candidate_overlay"]["functions"]) >= 23
    for stage in result["stages"].values():
        assert stage["all_tu_slots_verified"] == 49
        for row in stage["functions"]:
            assert row["function_bytes"] == row["target_bytes"]
            assert row["target_sha256"] == row["relocated_sha256"] == row["gnu_linked_sha256"]
    assert len({s["linked_text_sha256"] for s in result["stages"].values()}) == 1
    assert not result["production_source_written"] and result["matching_credit_added"] == 0
    assert result["rom_gate"].startswith("not run")


def compile_and_run_host(tmp_path, mutation=None):
    cc = shutil.which("cc")
    if not cc:
        pytest.skip("C compiler unavailable")
    destination = tmp_path / "contract"
    shutil.copytree(HERE, destination, ignore=shutil.ignore_patterns("__pycache__"))
    if mutation:
        name, old, new = mutation
        path = destination / "sources" / (name + ".c")
        text = path.read_text()
        assert old in text
        path.write_text(text.replace(old, new, 1))
    binary = tmp_path / "host_test"
    command = [cc, "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
               "-fsanitize=address,undefined", "-fno-omit-frame-pointer",
               str(destination / "host_test.c"), "-o", str(binary)]
    built = subprocess.run(command, capture_output=True, text=True)
    assert built.returncode == 0, built.stdout + built.stderr
    return subprocess.run([str(binary)], capture_output=True, text=True,
                          env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"})


def test_all_seven_adapted_sources_semantics(tmp_path):
    result = compile_and_run_host(tmp_path)
    assert result.returncode == 0, result.stdout + result.stderr


@pytest.mark.parametrize("mutation", [
    ("func_80017540", "next = entry->next;", "next = 0;"),
    ("func_80018A30", "highF74 + D_8004BE80->half120 <", "highF74 + D_8004BE80->half120 <="),
    ("func_80018B3C", "lowF70 = 0;", "lowF70 = 1;"),
    ("func_80018C2C", "flagsFEE |= 8;", "flagsFEE |= 4;"),
    ("func_80018D40", "pendingFF0 = 0;", "pendingFF0 = 1;"),
    ("func_80019194", "i<64", "i<63"),
    ("func_800198C8", "inactiveFC1 = 1;", "inactiveFC1 = 0;"),
])
def test_semantics_rejects_one_wrong_behavior_per_candidate(tmp_path, mutation):
    result = compile_and_run_host(tmp_path, mutation)
    assert result.returncode != 0
    assert "Assertion" in result.stderr, result.stderr
