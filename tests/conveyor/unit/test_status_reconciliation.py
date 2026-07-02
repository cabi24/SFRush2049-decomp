"""US5: transition engine legality + reconciliation invariants (T044)."""
import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.pipeline import status as statusmod


@pytest.fixture
def conn(tmp_path):
    c = dbmod.connect(tmp_path / "db.sqlite")
    with dbmod.tx(c):
        for tid in ("f1", "f2"):
            c.execute(
                "INSERT INTO n64_target (target_id, address, population)"
                " VALUES (?, 1, 'extracted')", (tid,))
            c.execute(
                "INSERT INTO function_status (target_id, status, updated_at)"
                " VALUES (?, 'unmatched', '2026')", (tid,))
    yield c
    c.close()


def test_full_forward_walk(conn):
    with dbmod.tx(conn):
        for step in ("candidate_identified", "seeded", "in_search", "matched"):
            assert statusmod.transition(conn, "f1", step)
        conn.execute(
            "INSERT INTO promotion_record (promotion_id, target_id, source_sha,"
            " outcome, created_at) VALUES ('p1', 'f1', 's', 'promoted', '2026')")
        assert statusmod.transition(conn, "f1", "verified")
    assert statusmod.counts(conn)["verified"] == 1
    assert statusmod.reconcile(conn) == []


def test_illegal_jump_rejected(conn):
    with dbmod.tx(conn):
        with pytest.raises(statusmod.InvalidTransition):
            statusmod.transition(conn, "f1", "verified")  # unmatched -> verified
        with pytest.raises(statusmod.InvalidTransition):
            statusmod.transition(conn, "f1", "matched")


def test_rollback_paths(conn):
    with dbmod.tx(conn):
        for step in ("candidate_identified", "in_search"):
            statusmod.transition(conn, "f1", step)
        assert statusmod.transition(conn, "f1", "seeded", human_flag="stalled")
        statusmod.transition(conn, "f1", "in_search")
        statusmod.transition(conn, "f1", "matched")
        assert statusmod.transition(conn, "f1", "seeded",
                                    human_flag="verify_failed:x")


def test_override_blocks_automated_backward_move(conn):
    with dbmod.tx(conn):
        statusmod.transition(conn, "f2", "candidate_identified")
        statusmod.transition(conn, "f2", "in_search")
        conn.execute(
            "UPDATE function_status SET override='{\"manual_seed\":\"x.c\"}'"
            " WHERE target_id='f2'")
        assert statusmod.transition(conn, "f2", "seeded") is False  # refused
        assert statusmod.transition(conn, "f2", "seeded", force=True) is True


def test_reconcile_catches_unbacked_verified(conn):
    with dbmod.tx(conn):
        conn.execute(
            "UPDATE function_status SET status='verified' WHERE target_id='f1'")
    problems = statusmod.reconcile(conn)
    assert problems and "without a promoted PromotionRecord" in problems[0]
