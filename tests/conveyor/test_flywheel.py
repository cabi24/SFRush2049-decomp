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


def test_flywheel_cycle_gives_new_seeds_the_triage_budget(
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

    assert stats == {"flywheel_started": 1, "flywheel_promoted": 0,
                     "compiled": 3, "scored": 2, "in_search": 0}
    assert submitted[0][0][5] == 0x80000000
    assert submitted[0][1] == {
        "budget_seconds": farm.TRIAGE_BUDGET_SECONDS,
        "priority": farm.FLYWHEEL_PRIORITY,
    }


def test_queued_flywheel_jobs_do_not_consume_the_static_search_budget(tmp_path):
    conn = _database(tmp_path / "conveyor.db")
    with dbmod.tx(conn):
        for index in range(20):                       # a full flywheel queue
            conn.execute(
                "INSERT INTO work_unit (job_id,job_type,target_id,manifest_sha,"
                "priority,state,created_at,updated_at)"
                " VALUES (?,'permuter_search','fresh','m',60,'PENDING','now','now')",
                (f"fw{index}",))
        conn.execute(
            "INSERT INTO work_unit (job_id,job_type,target_id,manifest_sha,"
            "priority,state,created_at,updated_at)"
            " VALUES ('st','permuter_search','scored','m',10,'LEASED','now','now')")

    class NoHttp:
        def call(self, *a, **k):
            raise AssertionError("no prospects -> no submissions")

    stats = farm.top_up(conn, None, NoHttp(), "tk", max_inflight=8,
                        budget_seconds=60)

    assert stats["inflight"] == 1                    # only the static search counts


# --- triage: short first pass, smallest first, survivors get the full budget --

def _sized(conn, target_id, insn_count, status="unmatched"):
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO n64_target (target_id,address,population,insn_count,"
            " target_o_sha,tier) VALUES (?,?,'extracted',?,?,'raw_word')",
            (target_id, 0x80100000 + insn_count, insn_count, target_id * 8))
        conn.execute(
            "INSERT INTO function_status (target_id,status,updated_at)"
            " VALUES (?,?,'now')", (target_id, status))


def _search(conn, job_id, target_id, seconds, state="DONE", score=None):
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO work_unit (job_id,job_type,target_id,manifest_sha,"
            " priority,state,budget,best_score,created_at,updated_at)"
            " VALUES (?,'permuter_search',?,'m',60,?,?,?,'now','now')",
            (job_id, target_id, state,
             None if seconds is None else json.dumps({"wall_seconds": seconds}),
             score))


def test_selection_orders_smallest_first(tmp_path):
    conn = _database(tmp_path / "conveyor.db")
    _sized(conn, "big", 400)
    _sized(conn, "small", 12)
    _sized(conn, "middle", 90)
    histogram = tmp_path / "m2c_histogram.json"
    histogram.write_text(json.dumps({
        "run": {"population_complete": True},
        "targets": {name: {"bucket": "compiled"}
                    for name in ("big", "small", "middle")}}))

    selection = farm.flywheel_selection(conn, histogram)

    assert [row["target_id"] for row in selection.targets] == [
        "small", "middle", "big"]


def test_triage_survivor_is_a_close_score_that_never_had_a_full_search(tmp_path):
    conn = _database(tmp_path / "conveyor.db")
    for name, size in (("close", 14), ("far", 20), ("already_full", 16),
                       ("matched_already", 10), ("tiny_close", 8)):
        _sized(conn, name, size,
               status="matched" if name == "matched_already" else "unmatched")
    _search(conn, "j1", "close", farm.TRIAGE_BUDGET_SECONDS, score=45)
    _search(conn, "j2", "far", farm.TRIAGE_BUDGET_SECONDS, score=5000)
    _search(conn, "j3", "already_full", farm.TRIAGE_BUDGET_SECONDS, score=30)
    _search(conn, "j4", "already_full", farm.STANDARD_SEARCH_BUDGET_SECONDS,
            state="LEASED")
    _search(conn, "j5", "matched_already", farm.TRIAGE_BUDGET_SECONDS, score=0)
    _search(conn, "j6", "tiny_close", farm.TRIAGE_BUDGET_SECONDS, score=200)

    survivors = [row["target_id"] for row in farm.triage_survivors(conn)]

    assert survivors == ["tiny_close", "close"]      # smallest first


def test_an_unrecorded_budget_counts_as_a_full_search_already_spent(tmp_path):
    """Pre-triage searches stored no budget; re-running them at full length
    would redo work, so absence must not read as 'only had a short pass'."""
    conn = _database(tmp_path / "conveyor.db")
    _sized(conn, "legacy", 12)
    _search(conn, "old", "legacy", None, score=40)

    assert farm.triage_survivors(conn) == ()


def test_cycle_promotes_survivors_at_full_budget_and_triages_the_rest(
        tmp_path, monkeypatch):
    conn = _database(tmp_path / "conveyor.db")
    _sized(conn, "survivor", 14)
    _search(conn, "j1", "survivor", farm.TRIAGE_BUDGET_SECONDS, score=45)
    histogram = tmp_path / "m2c_histogram.json"
    _histogram(histogram)
    calls = []
    monkeypatch.setattr(autodecomp, "_asm_for_rows", lambda *a: {})
    monkeypatch.setattr(
        autodecomp, "submit_one",
        lambda *args, **kwargs: calls.append((args[4], kwargs["budget_seconds"]))
        or "seeded")

    stats = farm.flywheel_cycle(conn, object(), object(), "toolkit",
                                histogram_path=histogram)

    assert stats["flywheel_promoted"] == 1 and stats["flywheel_started"] == 1
    assert calls == [("survivor", farm.STANDARD_SEARCH_BUDGET_SECONDS),
                     ("fresh", farm.TRIAGE_BUDGET_SECONDS)]
