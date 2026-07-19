"""006 flywheel selection, input, and scheduling-contract tests."""
import json

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.pipeline import autodecomp, farm, status


def _database(path):
    conn = dbmod.connect(path)
    with dbmod.tx(conn):
        for index, target_id in enumerate(("fresh", "searched", "scored")):
            conn.execute(
                "INSERT INTO n64_target"
                " (target_id,address,population,insn_count,target_o_sha,tier)"
                " VALUES (?,?, 'extracted',2,?,'raw_word')",
                (target_id, 0x80000000 + index * 8, target_id * 8),
            )
            conn.execute(
                "INSERT INTO function_status (target_id,status,updated_at)"
                " VALUES (?,'unmatched','now')", (target_id,),
            )
        conn.execute(
            "INSERT INTO work_unit"
            " (job_id,job_type,target_id,manifest_sha,priority,state,created_at,updated_at)"
            " VALUES ('search','permuter_search','searched','manifest',60,'DONE','now','now')"
        )
        conn.execute(
            "INSERT INTO matrix_entry"
            " (target_id,candidate_id,flagset,toolkit_sha,score)"
            " VALUES ('scored','candidate','flags','toolkit',123)"
        )
    return conn


def _histogram(path, *, complete=True):
    path.write_text(json.dumps({
        "run": {"population_complete": complete},
        "targets": {
            "fresh": {"bucket": "compiled"},
            "searched": {"bucket": "compiled"},
            "scored": {"bucket": "compiled"},
            "blocked": {"bucket": "blocked"},
        },
    }))


def test_flywheel_selects_only_compiled_targets_without_any_score_evidence(
        tmp_path):
    conn = _database(tmp_path / "conveyor.db")
    histogram = tmp_path / "m2c_histogram.json"
    _histogram(histogram)

    selection = farm.flywheel_selection(conn, histogram)

    assert [row["target_id"] for row in selection.targets] == ["fresh"]
    assert selection.compiled == 3
    assert selection.scored == 2
    assert selection.in_search == 0
    assert status.extracted_report_line(conn, histogram) == (
        "extracted: compiled 3, scored 2, in_search 0"
    )


def test_flywheel_refuses_histogram_without_population_complete(tmp_path):
    conn = _database(tmp_path / "conveyor.db")
    histogram = tmp_path / "m2c_probe.json"
    _histogram(histogram, complete=False)

    with pytest.raises(ValueError, match="population_complete"):
        farm.flywheel_selection(conn, histogram)


def test_flywheel_priority_remains_below_every_static_path():
    assert farm.FLYWHEEL_PRIORITY > farm.VERIFY_PRIORITY
    assert farm.FLYWHEEL_PRIORITY > farm.PROMOTE_PRIORITY
    assert farm.FLYWHEEL_PRIORITY > autodecomp.STATIC_SEED_PRIORITY


def test_flywheel_cycle_uses_submit_one_priority_and_standard_budget(
        tmp_path, monkeypatch):
    conn = _database(tmp_path / "conveyor.db")
    histogram = tmp_path / "m2c_histogram.json"
    _histogram(histogram)
    submitted = []

    monkeypatch.setattr(
        autodecomp, "_asm_for_rows", lambda *_args: {"fresh": "fresh.s"}
    )
    monkeypatch.setattr(
        autodecomp, "submit_one",
        lambda *args, **kwargs: submitted.append((args, kwargs)) or "seeded",
    )

    stats = farm.flywheel_cycle(
        conn, object(), object(), "toolkit", histogram_path=histogram
    )

    assert stats == {"flywheel_started": 1, "compiled": 3,
                     "scored": 2, "in_search": 0}
    assert submitted[0][0][5] == 0x80000000
    assert submitted[0][1] == {
        "budget_seconds": farm.STANDARD_SEARCH_BUDGET_SECONDS,
        "priority": farm.FLYWHEEL_PRIORITY,
    }
