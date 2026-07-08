"""Compile-failure clustering + result ingest (`pipeline.matrix`)."""
import argparse
import io
import json
import tarfile

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator.store import BlobStore
from tools.conveyor.pipeline import matrix as matrixmod

# Realistic cfe stderr, truncated mid-line at the front the way the old
# tail-capture stored it.
CFE_TAIL = """etup (show); break;
 -------------^
cfe: Error: /tmp/conveyor-578915e9/job/inputs/5c76af2c.c, line 49: 'ATR_JOIN' undefined; reoccurrences will not be reported.
         case ATR_JOIN: ShowJoin (show); break;
 -------------^
cfe: Error: /tmp/conveyor-578915e9/job/inputs/5c76af2c.c, line 52: 'ATR_GAMESTAT' undefined; reoccurrences will not be reported.
"""

CFE_SYNTAX = """cfe: Error: /tmp/conveyor-x/job/inputs/ab.c, line 3: Syntax Error
     struct foo {
 ----^
"""


def test_undefined_identifiers_become_signatures():
    sigs = matrixmod.failure_signatures(CFE_TAIL)
    assert "undefined: ATR_JOIN" in sigs
    assert "undefined: ATR_GAMESTAT" in sigs


def test_syntax_error_without_type_token_stays_generic():
    sigs = matrixmod.failure_signatures(CFE_SYNTAX)
    assert sigs == {"Syntax Error"}


CFE_HEAD_SYNTAX = """cfe: Error: /tmp/conveyor-x/job/inputs/56f424de.c, line 7: Syntax Error
     extern int (*gTransitionFunc) (BLIT *);
 ----------------------------------------^
cfe: Error: /tmp/conveyor-x/job/inputs/56f424de.c, line 9: Syntax Error
     return 1;
 ----^
"""


def test_head_captured_syntax_error_mines_unknown_type():
    sigs = matrixmod.failure_signatures(CFE_HEAD_SYNTAX)
    # First error yields the type token; the follow-up is cascade noise.
    assert sigs == {"unknown type? BLIT", "Syntax Error"}


def test_tail_truncated_syntax_error_stays_generic():
    sigs = matrixmod.failure_signatures("onveyor-x/ab.c, li" + CFE_HEAD_SYNTAX)
    assert "Syntax Error" in sigs
    assert not any(s.startswith("unknown type?") for s in sigs)


def test_unrecognized_message_falls_back_to_raw_text():
    assert matrixmod.failure_signatures("timeout") == {"timeout"}
    assert matrixmod.failure_signatures("") == {"empty error output"}


@pytest.fixture
def conn(tmp_path):
    c = dbmod.connect(tmp_path / "db.sqlite")
    yield c
    c.close()


def _add_candidate(conn, cand_id, compile_status):
    conn.execute(
        "INSERT INTO arcade_candidate (candidate_id, name, src_file,"
        " body_sha, compile_status) VALUES (?, ?, ?, 'x', ?)",
        (cand_id, cand_id.split(":")[1], cand_id.split(":")[0],
         json.dumps(compile_status)),
    )
    conn.commit()


def test_aggregate_counts_each_candidate_once(conn):
    fail = "fail:" + CFE_TAIL
    # Fails identically under both flagsets: signatures dedupe per candidate.
    _add_candidate(conn, "game/a.c:f1", {"-O1": fail, "-O2": fail})
    # Compiles under one flagset: counted ok, not blocked.
    _add_candidate(conn, "game/a.c:f2", {"-O1": fail, "-O2": "ok"})
    # Untried: ignored.
    _add_candidate(conn, "game/a.c:f3", {})

    blocked, ok = matrixmod.aggregate_failures(conn)
    assert ok == 1
    assert set(blocked) == {"game/a.c:f1"}
    assert blocked["game/a.c:f1"] == {
        "undefined: ATR_JOIN", "undefined: ATR_GAMESTAT",
    }


def test_aggregate_merges_signatures_across_flagsets(conn):
    _add_candidate(conn, "game/b.c:g", {
        "-O1": "fail:" + CFE_TAIL, "-O2": "fail:timeout",
    })
    blocked, _ = matrixmod.aggregate_failures(conn)
    assert blocked["game/b.c:g"] == {
        "undefined: ATR_JOIN", "undefined: ATR_GAMESTAT", "timeout",
    }


# --- result ingest: score_reloc_blind column (T006) -------------------------

TOOLKIT = "t" * 64


def _compile_score_result(cells):
    return {"job_id": "j", "job_type": "compile_score", "exit": "ok",
            "error": None, "payload": {"cells": cells}}


def _put_result(store, result):
    data = json.dumps(result).encode()
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w:gz") as tar:
        info = tarfile.TarInfo("result.json")
        info.size = len(data)
        tar.addfile(info, io.BytesIO(data))
    return store.put_bytes(buf.getvalue())


def _mk_done_compile_job(conn, store, cells):
    result_sha = _put_result(store, _compile_score_result(cells))
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO work_unit (job_id, job_type, manifest_sha, toolkit_sha,"
            " state, result_sha, created_at, updated_at)"
            " VALUES ('cs-job', 'compile_score', 'm', ?, 'DONE', ?, '2026', '2026')",
            (TOOLKIT, result_sha),
        )


def test_ingest_stores_reloc_blind_and_tolerates_missing(tmp_path):
    """A cell carrying score_reloc_blind lands in the column; an old-blob cell
    without the field ingests as NULL — not rejected (data-model.md FR-007)."""
    data = tmp_path
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")
    dbmod.set_meta(conn, "toolkit_sha", TOOLKIT)
    for tid in ("with_blind", "without_blind"):
        with dbmod.tx(conn):
            conn.execute(
                "INSERT INTO n64_target (target_id, address, population, insn_count,"
                " target_o_sha) VALUES (?, 1, 'static', 12, '0')", (tid,))
            conn.execute(
                "INSERT INTO function_status (target_id, status, updated_at)"
                " VALUES (?, 'unmatched', strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
                (tid,))
    _mk_done_compile_job(conn, store, [
        {"candidate_id": "ultralib:src/os/f.c:with_blind", "flagset": "-O1",
         "target_id": "with_blind", "score": 20, "score_reloc_blind": 0,
         "compile": "ok"},
        # Simulates an old (pre-002) result blob: no score_reloc_blind key.
        {"candidate_id": "game/a.c:without_blind", "flagset": "-O1",
         "target_id": "without_blind", "score": 7, "compile": "ok"},
    ])
    conn.commit()
    conn.close()

    args = argparse.Namespace(data=str(data), coordinator="http://x", token=None)
    matrixmod.cmd_ingest(args)

    conn = dbmod.connect(data / "conveyor.db")
    row = conn.execute(
        "SELECT score, score_reloc_blind FROM matrix_entry WHERE target_id='with_blind'"
    ).fetchone()
    assert (row["score"], row["score_reloc_blind"]) == (20, 0)
    row = conn.execute(
        "SELECT score, score_reloc_blind FROM matrix_entry WHERE target_id='without_blind'"
    ).fetchone()
    assert row["score"] == 7 and row["score_reloc_blind"] is None
    conn.close()
