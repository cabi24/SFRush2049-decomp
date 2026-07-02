"""Work-unit queue and lease engine.

State machine (data-model.md):

    PENDING -> LEASED -> DONE
       ^          |-- lease expired -> PENDING (attempt+1), or FAILED past max
       |          `-- cancel -> CANCELLED
       `-- requeue

Invariants enforced here:
- at most one live lease per job (single-writer SQLite + guarded UPDATEs)
- first result wins; duplicates acknowledged but discarded
- a DONE result for the same manifest_sha short-circuits identical submissions
"""
import datetime
import json
import uuid

from . import db as dbmod

LEASE_SECONDS = 120
HEARTBEAT_SECONDS = 30
DEFAULT_MAX_ATTEMPTS_BATCH = 3


def _now():
    return datetime.datetime.now(datetime.timezone.utc)


def _iso(dt):
    return dt.strftime("%Y-%m-%dT%H:%M:%S.%f+00:00")


def _parse_iso(s):
    return datetime.datetime.strptime(s, "%Y-%m-%dT%H:%M:%S.%f+00:00").replace(
        tzinfo=datetime.timezone.utc
    )


def submit(conn, jobs):
    """Submit a list of job dicts; returns one entry per job:
    {"job_id": ...} or {"cached_result": <result_sha>} (FR-006 short-circuit).

    Job dict fields: job_type, manifest_sha, bundle_sha, toolkit_sha,
    required_capability, priority, budget, max_attempts, batch (bool).
    """
    out = []
    now = _iso(_now())
    with dbmod.tx(conn):
        for job in jobs:
            if job.get("batch", True):
                cached = conn.execute(
                    "SELECT result_sha FROM work_unit "
                    "WHERE manifest_sha = ? AND state = 'DONE' AND result_sha IS NOT NULL "
                    "ORDER BY updated_at DESC LIMIT 1",
                    (job["manifest_sha"],),
                ).fetchone()
                if cached:
                    out.append({"cached_result": cached["result_sha"]})
                    continue
            job_id = str(uuid.uuid4())
            conn.execute(
                "INSERT INTO work_unit (job_id, job_type, target_id, manifest_sha,"
                " bundle_sha, toolkit_sha, required_capability, priority, state,"
                " attempt, max_attempts, budget, created_at, updated_at)"
                " VALUES (?, ?, ?, ?, ?, ?, ?, ?, 'PENDING', 0, ?, ?, ?, ?)",
                (
                    job_id,
                    job["job_type"],
                    job.get("target_id"),
                    job["manifest_sha"],
                    job.get("bundle_sha"),
                    job.get("toolkit_sha"),
                    job.get("required_capability", "x86_64"),
                    job.get("priority", 100),
                    job.get(
                        "max_attempts",
                        DEFAULT_MAX_ATTEMPTS_BATCH if job.get("batch", True) else None,
                    ),
                    json.dumps(job.get("budget")) if job.get("budget") else None,
                    now,
                    now,
                ),
            )
            out.append({"job_id": job_id})
    return out


def lease(conn, node_id, hostname, capabilities, cores, cached_toolkits=(), agent_version=None):
    """Lease the best PENDING job this node can run, or None.

    Selection: capability match, then priority, then prefer jobs whose toolkit
    the node already caches, then FIFO.
    """
    now = _now()
    now_s = _iso(now)
    expires = _iso(now + datetime.timedelta(seconds=LEASE_SECONDS))
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO node (node_id, hostname, capabilities, cores, agent_version, last_seen)"
            " VALUES (?, ?, ?, ?, ?, ?)"
            " ON CONFLICT(node_id) DO UPDATE SET hostname=excluded.hostname,"
            " capabilities=excluded.capabilities, cores=excluded.cores,"
            " agent_version=excluded.agent_version, last_seen=excluded.last_seen",
            (node_id, hostname, json.dumps(sorted(capabilities)), cores, agent_version, now_s),
        )
        _expire_stale(conn, now)
        placeholders = ",".join("?" for _ in capabilities) or "''"
        rows = conn.execute(
            f"SELECT * FROM work_unit WHERE state = 'PENDING'"
            f" AND required_capability IN ({placeholders})"
            f" ORDER BY priority, created_at LIMIT 50",
            tuple(capabilities),
        ).fetchall()
        if not rows:
            return None
        cached = set(cached_toolkits)
        chosen = None
        best_priority = rows[0]["priority"]
        for row in rows:
            if row["priority"] != best_priority:
                break
            if row["toolkit_sha"] in cached:
                chosen = row
                break
        if chosen is None:
            chosen = rows[0]
        conn.execute(
            "UPDATE work_unit SET state='LEASED', leased_by=?, lease_expires=?,"
            " attempt=attempt+1, updated_at=? WHERE job_id=? AND state='PENDING'",
            (node_id, expires, now_s, chosen["job_id"]),
        )
    return {
        "job_id": chosen["job_id"],
        "job_type": chosen["job_type"],
        "bundle_sha": chosen["bundle_sha"],
        "toolkit_sha": chosen["toolkit_sha"],
        "lease_expires": expires,
        "heartbeat_seconds": HEARTBEAT_SECONDS,
        "budget": json.loads(chosen["budget"]) if chosen["budget"] else None,
    }


def heartbeat(conn, job_id, node_id, progress=None):
    """Extend a lease. Returns dict with action continue|cancel, or None on 409."""
    now = _now()
    expires = _iso(now + datetime.timedelta(seconds=LEASE_SECONDS))
    with dbmod.tx(conn):
        row = conn.execute(
            "SELECT state, leased_by FROM work_unit WHERE job_id = ?", (job_id,)
        ).fetchone()
        if row is None:
            return None
        if row["state"] == "CANCELLED":
            return {"lease_expires": expires, "action": "cancel"}
        if row["state"] != "LEASED" or row["leased_by"] != node_id:
            return None  # 409: not this node's lease anymore
        conn.execute(
            "UPDATE work_unit SET lease_expires=?, updated_at=? WHERE job_id=?",
            (expires, _iso(now), job_id),
        )
        if progress:
            _record_progress(conn, job_id, progress, _iso(now))
        conn.execute(
            "UPDATE node SET last_seen=? WHERE node_id=?", (_iso(now), node_id)
        )
    return {"lease_expires": expires, "action": "continue"}


def _record_progress(conn, job_id, progress, now_s):
    best_score = progress.get("best_score")
    best_source_sha = progress.get("best_source_sha")
    if best_score is None:
        return
    row = conn.execute(
        "SELECT best_score FROM work_unit WHERE job_id = ?", (job_id,)
    ).fetchone()
    if row["best_score"] is None or best_score < row["best_score"]:
        conn.execute(
            "UPDATE work_unit SET best_score=?, best_source_sha=COALESCE(?, best_source_sha),"
            " updated_at=? WHERE job_id=?",
            (best_score, best_source_sha, now_s, job_id),
        )


def submit_result(conn, job_id, node_id, result_sha):
    """First result wins. Returns 'accepted', 'duplicate', or None (unknown job)."""
    now_s = _iso(_now())
    with dbmod.tx(conn):
        row = conn.execute(
            "SELECT state, leased_by FROM work_unit WHERE job_id = ?", (job_id,)
        ).fetchone()
        if row is None:
            return None
        if row["state"] in ("DONE", "CANCELLED", "FAILED"):
            return "duplicate"
        # Accept from the lease holder, or from anyone if the job bounced back
        # to PENDING after an expiry (the work is idempotent; contract allows it).
        conn.execute(
            "UPDATE work_unit SET state='DONE', result_sha=?, leased_by=NULL,"
            " lease_expires=NULL, updated_at=? WHERE job_id=?",
            (result_sha, now_s, job_id),
        )
    return "accepted"


def cancel(conn, job_id):
    with dbmod.tx(conn):
        row = conn.execute(
            "SELECT state FROM work_unit WHERE job_id = ?", (job_id,)
        ).fetchone()
        if row is None:
            return False
        if row["state"] in ("PENDING", "LEASED"):
            conn.execute(
                "UPDATE work_unit SET state='CANCELLED', updated_at=? WHERE job_id=?",
                (_iso(_now()), job_id),
            )
    return True


def expire_stale(conn):
    """Public expiry sweep (also run inside every lease call)."""
    with dbmod.tx(conn):
        return _expire_stale(conn, _now())


def _expire_stale(conn, now):
    now_s = _iso(now)
    stale = conn.execute(
        "SELECT job_id, attempt, max_attempts FROM work_unit"
        " WHERE state='LEASED' AND lease_expires < ?",
        (now_s,),
    ).fetchall()
    expired = 0
    for row in stale:
        if row["max_attempts"] is not None and row["attempt"] >= row["max_attempts"]:
            new_state = "FAILED"
        else:
            new_state = "PENDING"
        conn.execute(
            "UPDATE work_unit SET state=?, leased_by=NULL, lease_expires=NULL,"
            " updated_at=? WHERE job_id=?",
            (new_state, now_s, row["job_id"]),
        )
        expired += 1
    return expired


def get_job(conn, job_id):
    row = conn.execute("SELECT * FROM work_unit WHERE job_id = ?", (job_id,)).fetchone()
    return dict(row) if row else None


def counts(conn):
    rows = conn.execute(
        "SELECT state, COUNT(*) AS n FROM work_unit GROUP BY state"
    ).fetchall()
    return {row["state"].lower(): row["n"] for row in rows}
