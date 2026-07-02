"""SC-003 regression: hand-confirmed pairings must be reproduced by the matrix.

Requires a populated matrix (run extract/submit/ingest against a live pool
first), so this is node_required. Known pairings from CLAUDE.md.
"""
import os
import sqlite3
from pathlib import Path

import pytest

pytestmark = pytest.mark.node_required

DATA = Path(os.environ.get("CONVEYOR_DATA", "~/.conveyor")).expanduser()

# (n64 work-dir name, arcade candidate) — high-confidence hand matches.
KNOWN_PAIRINGS = [
    ("game_loop", "game/game.c:game"),
    ("dll_remove", "GUTS/os/dll.c:dll_remove"),
    ("dll_init", "GUTS/os/dll.c:dll_init"),
]

TOP_N = 5


@pytest.fixture
def conn():
    db = DATA / "conveyor.db"
    if not db.is_file():
        pytest.skip("no conveyor DB — run the matrix first")
    c = sqlite3.connect(db)
    c.row_factory = sqlite3.Row
    yield c
    c.close()


@pytest.mark.parametrize("target_id,expected_candidate", KNOWN_PAIRINGS)
def test_known_pairing_ranks_in_top_n(conn, target_id, expected_candidate):
    rows = conn.execute(
        "SELECT candidate_id, MIN(score) AS score FROM matrix_entry"
        " WHERE target_id = ? GROUP BY candidate_id"
        " ORDER BY score, candidate_id LIMIT ?",
        (target_id, TOP_N),
    ).fetchall()
    if not rows:
        pytest.skip(f"no matrix cells for {target_id} yet")
    ranked = [r["candidate_id"] for r in rows]
    assert expected_candidate in ranked, (
        f"{target_id}: expected {expected_candidate} in top {TOP_N}, got {ranked}"
    )
