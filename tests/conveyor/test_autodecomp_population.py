"""Population-axis and context-regression tests for autodecomp."""
import json

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.pipeline import autodecomp
from tools.conveyor.pipeline import farm
from tools.conveyor.pipeline import lock


def _database(path):
    conn = dbmod.connect(path)
    with dbmod.tx(conn):
        for target_id, population, gate in (
                ("static_fn", "static", None),
                ("extracted_fn", "extracted", None),
                ("nested_fn", "extracted", "extent_conflict:outer")):
            conn.execute(
                "INSERT INTO n64_target"
                " (target_id,address,population,insn_count,target_o_sha,tier,gate_reason)"
                " VALUES (?,?,?,2,?,'raw_word',?)",
                (target_id, 0x80086A50, population, target_id * 8, gate),
            )
            conn.execute(
                "INSERT INTO function_status (target_id,status,updated_at)"
                " VALUES (?,'unmatched','now')", (target_id,),
            )
    return conn


def test_default_static_selection_keeps_original_query_and_seed(tmp_path,
                                                                 monkeypatch):
    conn = _database(tmp_path / "conveyor.db")
    traced = []
    conn.set_trace_callback(traced.append)
    rows = autodecomp._population_rows(conn, "static", None, 200)
    select = next(sql for sql in traced if "FROM n64_target t" in sql)

    assert "t.population='static'" in select
    assert "f.status IN ('unmatched','seeded')" in select
    assert "ORDER BY t.insn_count LIMIT 200" in select
    assert [row["target_id"] for row in rows] == ["static_fn"]

    monkeypatch.setattr(autodecomp, "_context", lambda: (None, ""))
    monkeypatch.setattr(autodecomp.subprocess, "run", lambda *a, **k: type(
        "Result", (), {"returncode": 0, "stdout": "s32 static_fn(void) {}",
                       "stderr": ""})())
    asm = tmp_path / "static.s"
    asm.write_text("glabel static_fn\n")
    before = autodecomp.m2c_seed("static_fn", 0x80086A50,
                                 {"static_fn": asm})
    after = autodecomp.m2c_seed("static_fn", 0x80086A50,
                                {"static_fn": asm}, diagnostics={})
    assert after == before


def test_at_file_resolution_aborts_on_unknown_target(tmp_path):
    conn = _database(tmp_path / "conveyor.db")
    names = tmp_path / "targets.txt"
    names.write_text("extracted_fn\nmissing_fn\n")

    with pytest.raises(SystemExit, match="missing_fn"):
        autodecomp._resolve_targets(conn, "extracted", f"@{names}")


def test_at_file_resolution_strips_comments(tmp_path):
    conn = _database(tmp_path / "conveyor.db")
    names = tmp_path / "targets.txt"
    names.write_text(
        "# extracted cluster\n"
        "extracted_fn  # 0x80086A50\n"
        "\n"
        "# another comment\n"
    )

    assert autodecomp._resolve_targets(
        conn, "extracted", f"@{names}"
    ) == ["extracted_fn"]


def test_extent_conflict_is_refused(tmp_path):
    conn = _database(tmp_path / "conveyor.db")

    with pytest.raises(SystemExit, match="extent_conflict:outer"):
        autodecomp._resolve_targets(conn, "extracted", "nested_fn")


def test_promotion_firewall_rejects_extracted_population(tmp_path):
    conn = _database(tmp_path / "conveyor.db")
    conn.close()

    with pytest.raises(
            SystemExit,
            match=("^error: extracted_fn is extracted-population — "
                   "evidence-only \\(005/FR-010\\)$")):
        lock.require_promotable_population("extracted_fn", tmp_path)

    # Static inventory targets and names absent from inventory remain eligible.
    lock.require_promotable_population("static_fn", tmp_path)
    lock.require_promotable_population("not_in_inventory", tmp_path)


def test_extracted_flagset_fallback_and_static_fallback(tmp_path):
    conn = _database(tmp_path / "conveyor.db")

    assert farm._flagset_for(conn, "static_fn") == farm.DEFAULT_FLAGSET
    assert farm._flagset_for(conn, "extracted_fn") == farm.EXTRACTED_FLAGSETS[0]
    assert farm.EXTRACTED_FLAGSETS[1] == "-g0 -O1 -mips2 -G 0 -non_shared"


def test_clean_m2c_retypes_unknown_function_pointer_casts():
    body = "callback = (? (*)(s32)) value;\nfn(? arg);"

    assert autodecomp._clean_m2c(body) == (
        "callback = (s32 (*)(s32)) value;\nfn(s32 arg);"
    )


def test_clean_m2c_injects_only_missing_saved_register_locals():
    body = ("s32 f(void) {\n"
            "    s32 saved_reg_s1;\n"
            "    saved_reg_s0 = 1;\n"
            "    saved_reg_s1 = 2;\n"
            "    saved_reg_s0x = 3;\n"
            "    return saved_reg_s0;\n"
            "}\n")

    out = autodecomp._clean_m2c(body)

    assert out.count("s32 saved_reg_s0;") == 1
    assert out.count("s32 saved_reg_s1;") == 1
    assert "s32 saved_reg_s0x;" not in out


def test_game_types_keeps_known_good_static_function_body_byte_identical(
        tmp_path, monkeypatch):
    """SC-005 (as amended 2026-07-17): game-context additions must not change
    the m2c-emitted function *body* for a static target. The seed's shared
    context prelude grows with game_types.h by construction, so whole-seed
    text identity is the wrong guard (research/t019-stall.md root cause 2);
    emit_src returns just the emitted function, which is what must not move."""
    asm_idx = autodecomp._asm_index()
    assert "osViGetFramebuffer" in asm_idx

    monkeypatch.setattr(autodecomp, "GAME_TYPES", tmp_path / "absent.h")
    monkeypatch.setattr(autodecomp, "M2C_PROTOS", tmp_path / "absent-protos.h")
    autodecomp._context_cache = None
    before = autodecomp.emit_src("osViGetFramebuffer", 0x800083D0, asm_idx)

    monkeypatch.setattr(
        autodecomp, "GAME_TYPES", autodecomp.REPO / "include" / "game_types.h"
    )
    autodecomp._context_cache = None
    after = autodecomp.emit_src("osViGetFramebuffer", 0x800083D0, asm_idx)

    layer = tmp_path / "m2c_protos.h"
    layer.write_text("s32 unrelated_generated_game_fn(s32 arg0);\n")
    monkeypatch.setattr(autodecomp, "M2C_PROTOS", layer)
    autodecomp._context_cache = None
    with_layer = autodecomp.emit_src(
        "osViGetFramebuffer", 0x800083D0, asm_idx
    )
    autodecomp._context_cache = None

    assert before is not None
    assert after == before
    assert with_layer == before


def test_generated_layer_is_last_and_its_content_changes_context_sha(
        tmp_path, monkeypatch):
    layer = tmp_path / "m2c_protos.h"
    layer.write_text("s32 generated_a(void);\n")
    monkeypatch.setattr(autodecomp, "M2C_PROTOS", layer)
    autodecomp._context_cache = None

    first = autodecomp._context()
    first_sha = autodecomp._context_sha(first)
    assert first[1].rstrip().endswith("s32 generated_a(void);")

    layer.write_text("s32 generated_b(void);\n")
    second = autodecomp._context()
    second_sha = autodecomp._context_sha(second)
    autodecomp._context_cache = None

    assert second[1].rstrip().endswith("s32 generated_b(void);")
    assert second_sha != first_sha


def test_histogram_is_exclusive_complete_and_deterministic(tmp_path, monkeypatch):
    conn = _database(tmp_path / "conveyor.db")
    with dbmod.tx(conn):
        for target_id in ("compiled_fn", "m2c_fail_fn", "scan_overrun_fn"):
            conn.execute(
                "INSERT INTO n64_target"
                " (target_id,address,population,insn_count,target_o_sha,tier)"
                " VALUES (?,0x80086A50,'extracted',2,?,'raw_word')",
                (target_id, target_id * 8),
            )
    asm = tmp_path / "derived.s"
    asm.write_text("glabel extracted_fn\n")

    def derive(_conn, target_id, **_kwargs):
        if target_id == "scan_overrun_fn":
            raise autodecomp.disasmmod.DisassemblyError("scan_overrun")
        return asm

    def seed(target_id, *args, diagnostics=None, **kwargs):
        if target_id == "m2c_fail_fn":
            diagnostics[target_id] = "m2c exploded\nmore detail"
            return None
        if target_id == "extracted_fn":
            diagnostics[(target_id, "raw")] = "M2C_ERROR(something unknown)"
        return target_id

    monkeypatch.setattr(autodecomp.disasmmod, "derive", derive)
    monkeypatch.setattr(autodecomp, "m2c_seed", seed)
    monkeypatch.setattr(
        autodecomp, "_seed_compile_errors",
        lambda value: ((True, []) if value == "compiled_fn" else
                       (False, [("Player", "p->speed")])),
    )
    monkeypatch.setattr(autodecomp, "_arcade_hint", lambda token: None)
    monkeypatch.setattr(autodecomp, "_context", lambda: (None, "context"))
    image = tmp_path / "game_code.bin"
    image.write_bytes(b"image")
    monkeypatch.setattr(autodecomp.disasmmod, "GAME_CODE_BIN", image)
    histogram_json = tmp_path / "hist.json"
    histogram_md = tmp_path / "hist.md"

    rows = autodecomp._histogram_rows(conn, None, 0)
    first = autodecomp._histogram_data(conn, rows)
    autodecomp._write_histogram(rows, *first, population_complete=True,
                                json_path=histogram_json, md_path=histogram_md)
    one = json.loads(histogram_json.read_text())
    second = autodecomp._histogram_data(conn, rows)
    autodecomp._write_histogram(rows, *second, population_complete=True,
                                json_path=histogram_json, md_path=histogram_md)
    two = json.loads(histogram_json.read_text())

    one["run"].pop("timestamp")
    two["run"].pop("timestamp")
    assert one == two
    assert sum(two["buckets"].values()) == two["run"]["targets"] == 5
    assert set(two["targets"]) == {
        "compiled_fn", "extracted_fn", "m2c_fail_fn", "nested_fn",
        "scan_overrun_fn",
    }
    assert two["buckets"] == {
        "blocked": 0, "compiled": 1, "partial_decomp": 1,
        "decompiler_failure": 1, "extent_conflict": 1, "no_disasm": 1,
    }
    assert two["run"]["population_complete"] is True
    assert two["targets"]["extracted_fn"]["bucket"] == "partial_decomp"
    assert two["targets"]["nested_fn"]["bucket"] == "extent_conflict"
    assert two["targets"]["scan_overrun_fn"]["detail"] == "scan_overrun"
    assert two["targets"]["m2c_fail_fn"]["detail"] == "m2c exploded"


def test_clean_m2c_rewrites_member_access_on_byte_cursor_locals():
    body = ("void f(void) {\n"
            "    u8 *spB4;\n"
            "    if (a < spB4->unk1C) {}\n"
            "    if (b < spB4->unk-12F4) {}\n"
            "    other->unk1C = 1;\n"
            "}\n")
    out = autodecomp._clean_m2c(body)
    assert "(*(s32 *) (spB4 + 0x1C))" in out
    assert "(*(s32 *) (spB4 - 0x12F4))" in out
    assert "other->unk1C" in out  # only declared u8* locals rewrite


def test_clean_m2c_rewrites_scalar_cursor_without_struct_pointer_overreach():
    body = ("void f(void) {\n"
            "    s32 cursor;\n"
            "    u16 *halfwords;\n"
            "    Player *player;\n"
            "    cursor->unk4 = halfwords->unk-8;\n"
            "    player->unk4 = global_cursor->unk4;\n"
            "}\n")

    out = autodecomp._clean_m2c(body)

    assert "(*(s32 *) (cursor + 0x4))" in out
    assert "(*(s32 *) (halfwords - 0x8))" in out
    assert "player->unk4" in out
    assert "global_cursor->unk4" in out


def test_partial_decomp_uses_raw_m2c_output_before_hygiene(tmp_path, monkeypatch):
    conn = _database(tmp_path / "conveyor.db")
    rows = autodecomp._histogram_rows(conn, ["extracted_fn"], 0)
    asm = tmp_path / "derived.s"
    asm.write_text("glabel extracted_fn\n")
    raw = "s32 extracted_fn(void) { M2C_ERROR(x); return 1; }"
    monkeypatch.setattr(
        autodecomp.disasmmod, "derive", lambda *_, **_kwargs: asm
    )
    monkeypatch.setattr(autodecomp.subprocess, "run", lambda *a, **k: type(
        "Result", (), {"returncode": 0, "stdout": raw, "stderr": ""})())
    monkeypatch.setattr(autodecomp, "_context", lambda: (None, ""))
    monkeypatch.setattr(autodecomp, "_clean_m2c", lambda text: text.replace(
        "M2C_ERROR(x);", ""))
    monkeypatch.setattr(autodecomp, "_seed_compile_errors",
                        lambda _seed: (True, []))

    buckets, targets, _, _ = autodecomp._histogram_data(conn, rows)

    assert buckets["partial_decomp"] == 1
    assert buckets["compiled"] == buckets["blocked"] == 0
    assert targets["extracted_fn"]["bucket"] == "partial_decomp"


def test_scoped_cluster_routes_to_probe_without_touching_population(
        tmp_path, monkeypatch):
    population_json = tmp_path / "m2c_histogram.json"
    population_md = tmp_path / "m2c_histogram.md"
    probe_json = tmp_path / "m2c_probe.json"
    probe_md = tmp_path / "m2c_probe.md"
    population_json.write_text("population sentinel\n")
    population_md.write_text("population sentinel\n")
    conn = _database(tmp_path / "conveyor.db")
    monkeypatch.setattr(autodecomp, "_conn", lambda _data: (conn, None))
    monkeypatch.setattr(autodecomp, "HISTOGRAM_JSON", population_json)
    monkeypatch.setattr(autodecomp, "HISTOGRAM_MD", population_md)
    monkeypatch.setattr(autodecomp, "PROBE_JSON", probe_json)
    monkeypatch.setattr(autodecomp, "PROBE_MD", probe_md)
    monkeypatch.setattr(autodecomp, "_histogram_data", lambda *_: (
        {"compiled": 1, "blocked": 0, "partial_decomp": 0,
         "decompiler_failure": 0, "no_disasm": 0, "extent_conflict": 0},
        {"extracted_fn": {"bucket": "compiled", "detail": ""}}, [], {}))
    monkeypatch.setattr(autodecomp, "_context", lambda: (None, "context"))
    image = tmp_path / "game_code.bin"
    image.write_bytes(b"image")
    monkeypatch.setattr(autodecomp.disasmmod, "GAME_CODE_BIN", image)
    args = type("Args", (), {"data": tmp_path, "population": "extracted",
                              "targets": "extracted_fn", "limit": 0,
                              "top": 25})()

    autodecomp.cmd_clusters(args)

    assert population_json.read_text() == "population sentinel\n"
    assert population_md.read_text() == "population sentinel\n"
    probe = json.loads(probe_json.read_text())
    assert probe["run"]["population_complete"] is False


def test_histogram_diff_reports_sorted_movements_and_deltas(tmp_path, capsys):
    old = {
        "buckets": {"blocked": 2, "compiled": 0, "partial_decomp": 0},
        "targets": {
            "z_fn": {"bucket": "blocked"},
            "a_fn": {"bucket": "blocked"},
        },
        "blockers": [{"symbol": "ZType", "count": 1,
                      "functions": ["z_fn"]},
                     {"symbol": "AType", "count": 2,
                      "functions": ["a_fn", "z_fn"]}],
    }
    new = {
        "buckets": {"blocked": 0, "compiled": 1, "partial_decomp": 1},
        "targets": {
            "z_fn": {"bucket": "partial_decomp"},
            "a_fn": {"bucket": "compiled"},
        },
        "blockers": [{"symbol": "NewType", "count": 1,
                      "functions": ["a_fn"]}],
    }
    old_path = tmp_path / "old.json"
    new_path = tmp_path / "new.json"
    old_path.write_text(json.dumps(old))
    new_path.write_text(json.dumps(new))

    autodecomp.cmd_clusters_diff(old_path, new_path)

    assert capsys.readouterr().out == (
        "bucket deltas:\n"
        "  blocked: 2 -> 0 (-2)\n"
        "  compiled: 0 -> 1 (+1)\n"
        "  partial_decomp: 0 -> 1 (+1)\n"
        "target movements:\n"
        "  a_fn: blocked -> compiled\n"
        "  z_fn: blocked -> partial_decomp\n"
        "blocker deltas:\n"
        "  AType: 2 -> 0 (-2); functions -[a_fn,z_fn] +[]\n"
        "  NewType: 0 -> 1 (+1); functions -[] +[a_fn]\n"
        "  ZType: 1 -> 0 (-1); functions -[z_fn] +[]\n"
    )
