"""FunctionStatusRecord transition engine (US5, FR-011): the single choke
point for pipeline state changes, enforcing the data-model state machine.

    unmatched -> candidate_identified -> seeded -> in_search -> matched -> verified
                   (matrix)             (seed)     (farm)      (score 0)  (promoted)

Backward moves allowed: in_search->seeded (stall), matched->seeded (verify
rollback). `verified` is terminal and only reachable off a promoted
PromotionRecord (enforced by the caller writing both in one transaction).
"""
from ..coordinator import db as dbmod

ORDER = ("unmatched", "candidate_identified", "seeded", "in_search",
         "matched", "verified")

ALLOWED = {
    ("unmatched", "candidate_identified"),
    ("unmatched", "seeded"),                  # manual seed without matrix
    ("candidate_identified", "seeded"),
    ("candidate_identified", "in_search"),    # seed+submit in one step
    ("candidate_identified", "candidate_identified"),  # re-rank refresh
    ("seeded", "in_search"),
    ("seeded", "seeded"),
    ("in_search", "matched"),
    ("in_search", "seeded"),                  # stalled / budget exhausted
    ("in_search", "in_search"),               # re-issue
    ("matched", "verified"),
    ("matched", "seeded"),                    # verify rollback (FR-010)
}


class InvalidTransition(ValueError):
    pass


def transition(conn, target_id, new_status, *, human_flag=None,
               best_score=None, best_candidate_id=None, force=False):
    """Move one function to new_status inside the caller's transaction.
    Manual overrides are respected: automated (non-force) calls never touch a
    row whose override is set, except to advance it forward (FR-015)."""
    row = conn.execute(
        "SELECT status, override FROM function_status WHERE target_id=?",
        (target_id,),
    ).fetchone()
    if row is None:
        raise KeyError(f"unknown target {target_id}")
    current = row["status"]
    if not force and (current, new_status) not in ALLOWED:
        raise InvalidTransition(f"{target_id}: {current} -> {new_status}")
    if (row["override"] and not force
            and ORDER.index(new_status) < ORDER.index(current)):
        return False  # automated backward move on an overridden row: refuse
    conn.execute(
        "UPDATE function_status SET status=?, human_flag=?,"
        " best_score=COALESCE(?, best_score),"
        " best_candidate_id=COALESCE(?, best_candidate_id),"
        " updated_at=strftime('%Y-%m-%dT%H:%M:%fZ','now') WHERE target_id=?",
        (new_status, human_flag, best_score, best_candidate_id, target_id),
    )
    return True


def counts(conn):
    rows = conn.execute(
        "SELECT status, COUNT(*) AS n FROM function_status GROUP BY status"
    ).fetchall()
    out = {s: 0 for s in ORDER}
    out.update({r["status"]: r["n"] for r in rows})
    return out


def reconcile(conn):
    """Cross-check status counts against the evidence tables; returns a list
    of discrepancies (empty = healthy). Used by tests and `cli report`."""
    problems = []
    verified = conn.execute(
        "SELECT target_id FROM function_status WHERE status='verified'"
    ).fetchall()
    for r in verified:
        ok = conn.execute(
            "SELECT 1 FROM promotion_record WHERE target_id=? AND outcome='promoted'",
            (r["target_id"],),
        ).fetchone()
        if not ok:
            problems.append(f"{r['target_id']}: verified without a promoted "
                            "PromotionRecord")
    orphans = conn.execute(
        "SELECT f.target_id FROM function_status f"
        " LEFT JOIN n64_target t USING (target_id) WHERE t.target_id IS NULL"
    ).fetchall()
    for r in orphans:
        problems.append(f"{r['target_id']}: status row without an n64_target")
    return problems
