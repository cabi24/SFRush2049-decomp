"""Contract tests for the generated m2c declaration layer."""
import json

from tools.conveyor.pipeline import protos


def test_extract_signature_uses_callees_own_definition_line():
    output = (
        "extern s32 guessed(s32);\n\n"
        "void *math_utility(s32 arg0, struct Vec3 *arg1) {\n"
        "    return arg1;\n"
        "}\n"
    )

    assert protos.extract_signature(output, "math_utility") == (
        "void *math_utility(s32 arg0, struct Vec3 *arg1);"
    )


def test_extract_signature_cleans_nested_unknown_callback_return_type():
    output = (
        "void camera_update_a(s32 arg0, ? (*callback)(void *, s32),\n"
        "                     ? *opaque, ?32 word) {\n"
        "    callback(0, arg0);\n"
        "}\n"
    )

    assert protos.extract_signature(output, "camera_update_a") == (
        "void camera_update_a(s32 arg0, s32 (*callback)(void *, s32), "
        "s32 *opaque, s32 word);"
    )


def test_layer_falls_back_for_underivable_known_target():
    artifact = protos.build_layer(
        {"missing_body"}, {}, hand_names=set(), static_names=set(), pass_number=2
    )

    assert artifact["declarations"]["missing_body"] == {
        "signature": "s32 missing_body();",
        "source": "fallback",
        "pass": 2,
    }
    assert artifact["omitted"] == {}


def test_precedence_omits_hand_context_and_static_targets_with_reasons():
    artifact = protos.build_layer(
        {"memcpy", "osStatic", "game_fn"},
        {"memcpy": "void *memcpy(void *, const void *, u32);",
         "osStatic": "s32 osStatic(s32 arg0);",
         "game_fn": "s32 game_fn(void);"},
        hand_names={"memcpy", "osStatic"},
        static_names={"osStatic"},
        pass_number=2,
    )

    assert artifact["omitted"] == {
        "memcpy": "hand_context",
        "osStatic": "static_target",
    }
    assert list(artifact["declarations"]) == ["game_fn"]


def test_rendered_header_is_sorted_and_content_stable():
    artifact = protos.build_layer(
        {"z_fn", "a_fn"},
        {"z_fn": "void z_fn(void);", "a_fn": "s32 a_fn(s32 arg0);"},
        hand_names=set(), static_names=set(), pass_number=2,
    )
    stamp = {
        "image_sha": "image", "symbol_table_sha": "symbols",
        "context_sha": "context", "derivation_version": 2, "passes": 2,
    }

    first = protos.render_header(artifact, stamp)
    second = protos.render_header(artifact, {**stamp, "timestamp": "later"})

    assert first == second
    assert first.index("s32 a_fn") < first.index("void z_fn")


def test_write_artifacts_has_exclusive_complete_coverage(tmp_path):
    candidates = {"callee_b", "callee_a", "already_real"}
    artifact = protos.build_layer(
        candidates,
        {"callee_a": "s32 callee_a(void);"},
        hand_names={"already_real"}, static_names=set(), pass_number=2,
    )
    stamp = {
        "timestamp": "2026-07-19T00:00:00+00:00", "image_sha": "image",
        "symbol_table_sha": "symbols", "context_sha": "context",
        "derivation_version": 2, "passes": 2,
    }
    header = tmp_path / "m2c_protos.h"
    evidence = tmp_path / "m2c_protos.json"

    protos.write_artifacts(artifact, stamp, header, evidence)
    saved = json.loads(evidence.read_text())

    declared = set(saved["declarations"])
    omitted = set(saved["omitted"])
    assert declared | omitted == candidates
    assert not declared & omitted
    assert header.read_text() == protos.render_header(artifact, stamp)
