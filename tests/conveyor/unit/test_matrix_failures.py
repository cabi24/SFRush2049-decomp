"""Compile-failure clustering (`pipeline.matrix failures`)."""
import json

import pytest

from tools.conveyor.coordinator import db as dbmod
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
