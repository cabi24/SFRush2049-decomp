"""Source-bound regression checks for the bounded, unclaimed research packet."""
import importlib.util
import json
from pathlib import Path

import pytest


ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / "cloud/work/frontier/dot_source_boundary_scout_20261005"
spec = importlib.util.spec_from_file_location("source_boundary_scout", HERE / "verify.py")
scout = importlib.util.module_from_spec(spec)
spec.loader.exec_module(scout)
RECEIPT = json.loads((HERE / "verification.json").read_text())


def test_bound_sources_and_no_claims():
    scout.check_inputs(RECEIPT)
    assert RECEIPT["claims"] == []
    for family in ("scene_accessors", "cleanup_inline"):
        for record in RECEIPT["compiler"][family].values():
            assert record["claims"] == []


def test_source_mutation_fails_closed():
    changed = dict(RECEIPT, source_inputs=dict(RECEIPT["source_inputs"]))
    path = scout.WALKER_INPUTS["model_data_load"]
    changed["source_inputs"][path] = "0" * 64
    with pytest.raises(ValueError, match="source snapshot changed"):
        scout.check_inputs(changed)


def test_native_metadata_and_allocator_gate():
    actual = scout.native_summary()
    assert scout.stable_native(actual) == scout.stable_native(RECEIPT["native"])
    assert actual["function_count"] == 1216
    assert actual["allocator_highwater_direct_candidates"] == ["func_8008E26C"]
    for name in scout.ACCESSORS:
        assert actual["functions"][name]["direct_j_or_jal_callers"] == []


def test_unrelated_manifest_comments_do_not_invalidate_native_evidence():
    historical = RECEIPT["native"]
    changed = dict(historical, target_manifest={"annotation-only-change": "0" * 64})
    assert scout.stable_native(changed) == scout.stable_native(historical)
    changed = dict(historical, word_count=historical["word_count"] + 1)
    assert scout.stable_native(changed) != scout.stable_native(historical)


def test_native_release_homes_are_independent_witnesses():
    records = RECEIPT["native"]["functions"]
    expected = {
        "func_800C885C": (188, 64, [56, 32]),
        "wheel_params_set": (232, 64, [56, 32]),
        "func_800C8918": (628, 208, [192, 168, 80, 56, 32]),
    }
    for name, (size, frame, homes) in expected.items():
        record = records[name]
        assert record["native_bytes"] == size
        assert record["frame_bytes"] == frame
        assert [site["immediate_address_home"] for site in record["release_homes"]] == homes


def test_accessor_substitution_is_bounded_and_keeps_body_operations():
    original = (ROOT / scout.WALKER_INPUTS["model_data_load"]).read_text()
    flag = scout.accessor_calls(original)
    full = scout.accessor_calls(original, True)
    assert flag.count("t = func_8008AE64(a);") == 4
    assert flag.count("func_8008AE48(a, t |") == 4
    assert "D_8012E700[a].child" in flag
    assert "D_8012E700[a].child" not in full
    assert "D_8012E700[a].sibling" not in full
    for text in (flag, full):
        assert "    s32 t;\n    s32 c;" in text
        assert text.count("model_data_load(") == original.count("model_data_load(")
        assert "__inline" not in text
        assert "volatile" not in text
    records = RECEIPT["compiler"]["scene_accessors"]
    for label in ("baseline", "flag_accessors", "all_accessors"):
        assert records[label]["keep"] == scout.WALKERS + scout.ACCESSORS
        assert records[label]["flags"] == scout.FLAGS
        for name in scout.ACCESSORS:
            assert records[label]["functions"][name]["strict_match"]
            assert records[label]["source_sha256"][name + ".c"] == RECEIPT["source_inputs"][
                "src/blob/" + name + ".c"]
        for name in scout.WALKERS:
            assert not any(records[label]["functions"][name]["out_of_line_accessor_calls"].values())


def test_canonical_table_failure_stops_before_callers():
    record = RECEIPT["compiler"]["scene_accessors"]["canonical_helpers"]
    assert not record["accessor_gate_passed"]
    assert not record["caller_rerun_performed"]
    assert set(record["functions"]) == set(scout.ACCESSORS)
    assert record["functions"]["func_8008AE64"]["differing"] == 2
    assert record["functions"]["func_8008AE64"]["elf_function_bytes"] == 40
    assert all(record["functions"][name]["strict_match"] for name in scout.ACCESSORS[1:])


def test_cleanup_changes_only_one_keyword_and_preserves_recipe():
    records = RECEIPT["compiler"]["cleanup_inline"]
    for label, path in scout.CLEANUP_INPUTS.items():
        original = (ROOT / path / "group.c").read_text()
        changed = scout.annotate_free(original)
        assert changed.replace("__inline void audio_effect_process(u32 address) {",
                               "void audio_effect_process(u32 address) {") == original
        assert len(changed) - len(original) == len("__inline ")
        before, after = records[label + "_baseline"], records[label + "_inline_keyword"]
        assert before["flags"] == after["flags"]
        assert before["keep"] == after["keep"]
        matching = [name for name, result in before["functions"].items() if result["strict_match"]]
        assert len(matching) == 10
        assert all(after["functions"][name]["strict_match"] for name in matching)
        assert not before["functions"]["audio_effect_process"]["strict_match"]
        assert not after["functions"]["audio_effect_process"]["strict_match"]


def test_inline_cause_is_separate_from_matching_success():
    records = RECEIPT["compiler"]["cleanup_inline"]
    wheel = records["wheel_inline_keyword"]["functions"]["wheel_params_set"]
    assert (wheel["differing"], wheel["elf_function_bytes"], wheel["frame_bytes"]) == (4, 232, 48)
    assert wheel["out_of_line_release_wrapper_calls"] == 0
    assert [site["immediate_address_home"] for site in wheel["release_homes"]] == [40, 32]
    shutdown = records["shutdown_inline_keyword"]["functions"]["func_800C8918"]
    assert (shutdown["differing"], shutdown["elf_function_bytes"], shutdown["frame_bytes"]) == (86, 632, 88)
    assert shutdown["extra_words"] == 1
    assert shutdown["out_of_line_release_wrapper_calls"] == 0
    for family in ("scene_accessors", "cleanup_inline"):
        for record in RECEIPT["compiler"][family].values():
            for name, result in record["functions"].items():
                assert result["unresolved"] == []
                assert result["unverified"] == []
                assert result["errors"] == []
                if name in scout.WALKERS + scout.CLEANUPS:
                    assert not result["strict_match"]
