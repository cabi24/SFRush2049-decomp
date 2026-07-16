"""Population-axis and context-regression tests for autodecomp."""
import json

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.pipeline import autodecomp
from tools.conveyor.pipeline import farm


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


def test_extracted_flagset_fallback_and_static_fallback(tmp_path):
    conn = _database(tmp_path / "conveyor.db")

    assert farm._flagset_for(conn, "static_fn") == farm.DEFAULT_FLAGSET
    assert farm._flagset_for(conn, "extracted_fn") == farm.EXTRACTED_FLAGSETS[0]
    assert farm.EXTRACTED_FLAGSETS[1] == "-g0 -O1 -mips2 -G 0 -non_shared"


def test_empty_game_types_keeps_known_good_static_seed_byte_identical(
        tmp_path, monkeypatch):
    """SC-005 baseline: extending the header chain must not perturb a seed."""
    asm_idx = autodecomp._asm_index()
    assert "osViGetFramebuffer" in asm_idx

    monkeypatch.setattr(autodecomp, "GAME_TYPES", tmp_path / "absent.h")
    autodecomp._context_cache = None
    before = autodecomp.m2c_seed("osViGetFramebuffer", 0x800083D0, asm_idx)

    monkeypatch.setattr(
        autodecomp, "GAME_TYPES", autodecomp.REPO / "include" / "game_types.h"
    )
    autodecomp._context_cache = None
    after = autodecomp.m2c_seed("osViGetFramebuffer", 0x800083D0, asm_idx)
    autodecomp._context_cache = None

    assert before is not None
    assert after == before


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

    def derive(_conn, target_id):
        if target_id == "scan_overrun_fn":
            raise autodecomp.disasmmod.DisassemblyError("scan_overrun")
        return asm

    def seed(target_id, *args, diagnostics=None, **kwargs):
        if target_id == "m2c_fail_fn":
            diagnostics[target_id] = "m2c exploded\nmore detail"
            return None
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
    monkeypatch.setattr(autodecomp, "HISTOGRAM_JSON", tmp_path / "hist.json")
    monkeypatch.setattr(autodecomp, "HISTOGRAM_MD", tmp_path / "hist.md")

    rows = autodecomp._histogram_rows(conn, None, 0)
    first = autodecomp._histogram_data(conn, rows)
    autodecomp._write_histogram(rows, *first)
    one = json.loads(autodecomp.HISTOGRAM_JSON.read_text())
    second = autodecomp._histogram_data(conn, rows)
    autodecomp._write_histogram(rows, *second)
    two = json.loads(autodecomp.HISTOGRAM_JSON.read_text())

    one["run"].pop("timestamp")
    two["run"].pop("timestamp")
    assert one == two
    assert sum(two["buckets"].values()) == two["run"]["targets"] == 5
    assert set(two["targets"]) == {
        "compiled_fn", "extracted_fn", "m2c_fail_fn", "nested_fn",
        "scan_overrun_fn",
    }
    assert two["buckets"] == {
        "blocked": 1, "compiled": 1, "decompiler_failure": 1,
        "extent_conflict": 1, "no_disasm": 1,
    }
    assert two["targets"]["extracted_fn"]["bucket"] == "blocked"
    assert two["targets"]["nested_fn"]["bucket"] == "extent_conflict"
    assert two["targets"]["scan_overrun_fn"]["detail"] == "scan_overrun"
    assert two["targets"]["m2c_fail_fn"]["detail"] == "m2c exploded"
