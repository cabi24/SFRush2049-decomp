"""The queue repair must coexist with every real lib_25bb0 TU slot."""
import importlib.util
from pathlib import Path
import shutil

import pytest

from tools.cloud import check_submissions, score

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location(
    "queue_wrapper_verify",
    ROOT / "cloud/work/boot_tail_promotion/queue_wrappers/verify.py")
verify = importlib.util.module_from_spec(spec)
spec.loader.exec_module(verify)


@pytest.fixture(scope="module")
def report(tmp_path_factory):
    if not (score.IDO / "cc").is_file():
        pytest.skip("IDO missing")
    for tool in ("mips-linux-gnu-as", "mips-linux-gnu-objdump", "mips-linux-gnu-readelf"):
        if not shutil.which(tool):
            pytest.skip(tool + " missing")
    return verify.verify(tmp_path_factory.mktemp("queue-wrapper-promotion"))


def test_changed_sources_remain_selected_by_normal_ci():
    paths = [str(path.relative_to(ROOT)) for path in verify.candidate_paths().values()]
    jobs = list(check_submissions.commands(ROOT, paths))
    assert len(jobs) == 3
    assert all("--targets=" + str(ROOT / "asm/us/boot_tail") in job for job in jobs)
    for source in verify.candidate_paths().values():
        text = source.read_text()
        assert "typedef struct OSMesgQueue_s OSMesgQueue;" in text
        assert "extern OSMesgQueue D_800586A8;" in text
        assert "MessageQueue" not in text


def test_exact_target_extents_and_relocations(report):
    assert report["candidate_bytes"] == 140
    assert {fn: row["target_bytes"] for fn, row in report["standalone"].items()} == {
        "func_800250F0": 48, "func_80025120": 48, "func_80025150": 44}
    for rows in (report["standalone"], report["baseline_tu"], report["current_tu"], report["combined_tu"]):
        for row in rows.values():
            assert row["strict_match"]
            assert not any(row[key] for key in
                           ("differing", "extra_words", "unresolved", "unverified", "errors"))


def test_actual_combined_tu_preserves_all_five_locked_bodies(report):
    assert report["all_tu_functions"] == 21
    assert set(report["baseline_tu"]) == set(report["combined_tu"])
    assert set(report["locked_c_regressions"]) == set(verify.LOCKED)
    assert report["allocated_data_bytes"] == 0
    assert report["full_rom_gate"].startswith("NOT RUN")


def test_failure_controls_detect_original_bug_and_changed_code(report):
    controls = report["failure_controls"]
    assert "redeclaration of 'D_800586A8'" in controls["old_declaration_full_tu"]
    assert controls["wrong_blocking_flag"]["differing"] > 0


def promoted_fixture(work, names):
    """Model a legitimate lock migration entirely in memory and temp files."""
    text, locks = verify.baseline_inputs()
    paths = {fn: path for fn, path in verify.candidate_paths().items() if fn in names}
    rows = verify.fit(paths, work)
    text = verify.splice(text, paths, rows)
    for fn, path in paths.items():
        old_key = str(path.relative_to(ROOT)) + ":" + fn
        entry = locks.pop(old_key)
        entry["verified"] = "rom-sha1"
        locks[str(verify.TU) + ":" + fn] = entry
    return text, locks


@pytest.mark.parametrize("promoted", [verify.FUNCTIONS[:1], verify.FUNCTIONS[::2], verify.FUNCTIONS])
def test_partial_and_full_promotion_survive_migrated_source_locks(report, tmp_path, promoted):
    real_tu = (ROOT / verify.TU).read_bytes()
    real_locks = (ROOT / "matched.lock.json").read_bytes()
    text, locks = promoted_fixture(tmp_path, promoted)
    result = verify.verify(tmp_path, text, locks)
    assert set(result["lifecycle"]["promoted_candidates"]) == set(promoted)
    assert set(result["lifecycle"]["pending_candidates"]) == set(verify.FUNCTIONS) - set(promoted)
    assert result["lifecycle"]["all_tu_function_offsets_unchanged"]
    assert len(result["current_tu"]) == len(result["combined_tu"]) == 21
    for fn in verify.LOCKED + verify.FUNCTIONS:
        row = result["combined_tu"][fn]
        assert row["function_symbol_bytes"] == row["target_bytes"]
    assert (ROOT / verify.TU).read_bytes() == real_tu
    assert (ROOT / "matched.lock.json").read_bytes() == real_locks


def test_promoted_candidate_without_current_tu_lock_fails(report, tmp_path):
    text, locks = promoted_fixture(tmp_path, verify.FUNCTIONS)
    del locks[str(verify.TU) + ":" + verify.FUNCTIONS[0]]
    with pytest.raises(AssertionError, match="current TU lock/body mismatch"):
        verify.verify(tmp_path, text, locks)


def test_bad_promoted_body_with_updated_lock_still_fails_native_compare(report, tmp_path):
    text, locks = promoted_fixture(tmp_path, verify.FUNCTIONS)
    text = text.replace("osRecvMesg(&D_800586A8, 0, 1);", "osRecvMesg(&D_800586A8, 0, 0);", 1)
    fn = verify.FUNCTIONS[0]
    locks[str(verify.TU) + ":" + fn]["body_sha256"] = verify.text_body_sha(verify.text_bodies(text)[fn])
    with pytest.raises(AssertionError, match="func_800250F0:.*words differ"):
        verify.verify(tmp_path, text, locks)


def test_original_five_lock_population_is_preserved(report, tmp_path):
    text, locks = verify.baseline_inputs()
    del locks[str(verify.TU) + ":" + verify.LOCKED[0]]
    with pytest.raises(AssertionError, match="original TU lock/body missing"):
        verify.verify(tmp_path, text, locks)
