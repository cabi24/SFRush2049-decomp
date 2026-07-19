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
    ("seeded", "verified"),                   # promotion landed after a manual
                                              # rollback raced it; the commit
                                              # is real, so record it
}


class InvalidTransition(ValueError):
    pass


def transition(conn, target_id, new_status, *, human_flag=None,
               best_score=None, best_candidate_id=None, force=False):
    """Move one function to new_status inside the caller's transaction.

    FR-015 (overrides preserved) applies to the *pairing/flag fields*, which
    automated writers must guard individually (see matrix.update_rankings) —
    status movement itself is never blocked by an override: a manually seeded
    function whose search stalls must still travel in_search -> seeded, or it
    deadlocks invisibly."""
    row = conn.execute(
        "SELECT status FROM function_status WHERE target_id=?", (target_id,)
    ).fetchone()
    if row is None:
        raise KeyError(f"unknown target {target_id}")
    current = row["status"]
    if not force and (current, new_status) not in ALLOWED:
        raise InvalidTransition(f"{target_id}: {current} -> {new_status}")
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


def extracted_counts(conn, histogram_path=None):
    """Flywheel coverage for the standard report."""
    from . import farm

    selection = farm.flywheel_selection(
        conn, histogram_path or farm.HISTOGRAM_JSON
    )
    return selection.compiled, selection.scored, selection.in_search


def extracted_report_line(conn, histogram_path=None):
    compiled, scored, in_search = extracted_counts(conn, histogram_path)
    return (f"extracted: compiled {compiled}, scored {scored}, "
            f"in_search {in_search}")


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
    unrecorded = conn.execute(
        "SELECT p.target_id FROM promotion_record p"
        " JOIN function_status f USING (target_id)"
        " WHERE p.outcome='promoted' AND f.status != 'verified'"
    ).fetchall()
    for r in unrecorded:
        problems.append(
            f"{r['target_id']}: promoted to the repo but status is not verified"
        )
    return problems
