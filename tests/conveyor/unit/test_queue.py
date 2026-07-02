import datetime

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator import queue


@pytest.fixture
def conn(tmp_path):
    c = dbmod.connect(tmp_path / "conveyor.db")
    yield c
    c.close()


def _job(manifest_sha="m1", **kw):
    base = {
        "job_type": "compile_score",
        "manifest_sha": manifest_sha,
        "bundle_sha": "b" * 64,
        "toolkit_sha": "t" * 64,
        "priority": 100,
    }
    base.update(kw)
    return base


def _lease(conn, node="node-a", caps=("x86_64",), cached=()):
    return queue.lease(conn, node, node, list(caps), 4, cached_toolkits=cached)


def _force_expire(conn, job_id):
    past = (
        datetime.datetime.now(datetime.timezone.utc) - datetime.timedelta(seconds=1)
    ).strftime("%Y-%m-%dT%H:%M:%S.%f+00:00")
    with dbmod.tx(conn):
        conn.execute(
            "UPDATE work_unit SET lease_expires=? WHERE job_id=?", (past, job_id)
        )


def test_submit_lease_result_roundtrip(conn):
    [res] = queue.submit(conn, [_job()])
    job_id = res["job_id"]
    leased = _lease(conn)
    assert leased["job_id"] == job_id
    assert leased["heartbeat_seconds"] == queue.HEARTBEAT_SECONDS
    assert queue.submit_result(conn, job_id, "node-a", "r" * 64) == "accepted"
    assert queue.get_job(conn, job_id)["state"] == "DONE"


def test_duplicate_result_discarded(conn):
    [res] = queue.submit(conn, [_job()])
    job_id = res["job_id"]
    _lease(conn)
    assert queue.submit_result(conn, job_id, "node-a", "r" * 64) == "accepted"
    assert queue.submit_result(conn, job_id, "node-b", "x" * 64) == "duplicate"
    assert queue.get_job(conn, job_id)["result_sha"] == "r" * 64  # first wins


def test_cache_short_circuit(conn):
    [res] = queue.submit(conn, [_job()])
    _lease(conn)
    queue.submit_result(conn, res["job_id"], "node-a", "r" * 64)
    [again] = queue.submit(conn, [_job()])  # identical manifest_sha
    assert again == {"cached_result": "r" * 64}


def test_search_jobs_never_cache(conn):
    job = _job(job_type="permuter_search", batch=False, max_attempts=None)
    [res] = queue.submit(conn, [job])
    _lease(conn)
    queue.submit_result(conn, res["job_id"], "node-a", "r" * 64)
    [again] = queue.submit(conn, [dict(job)])
    assert "job_id" in again  # a fresh search job, not a cached result


def test_expiry_reissues_then_fails(conn):
    [res] = queue.submit(conn, [_job(max_attempts=2)])
    job_id = res["job_id"]
    for expected_attempt in (1, 2):
        leased = _lease(conn)
        assert leased["job_id"] == job_id
        assert queue.get_job(conn, job_id)["attempt"] == expected_attempt
        _force_expire(conn, job_id)
        queue.expire_stale(conn)
        state = queue.get_job(conn, job_id)["state"]
        assert state == ("PENDING" if expected_attempt < 2 else "FAILED")
    assert _lease(conn) is None  # FAILED jobs are not leasable


def test_heartbeat_extends_and_persists_progress(conn):
    [res] = queue.submit(
        conn, [_job(job_type="permuter_search", batch=False, max_attempts=None)]
    )
    job_id = res["job_id"]
    _lease(conn)
    hb = queue.heartbeat(
        conn, job_id, "node-a", progress={"best_score": 42, "best_source_sha": "s" * 64}
    )
    assert hb["action"] == "continue"
    job = queue.get_job(conn, job_id)
    assert job["best_score"] == 42 and job["best_source_sha"] == "s" * 64
    # A worse score never regresses the recorded best.
    queue.heartbeat(conn, job_id, "node-a", progress={"best_score": 99})
    assert queue.get_job(conn, job_id)["best_score"] == 42


def test_heartbeat_lease_conflict_409(conn):
    [res] = queue.submit(conn, [_job()])
    _lease(conn, node="node-a")
    assert queue.heartbeat(conn, res["job_id"], "node-b") is None


def test_cancel_reported_at_heartbeat(conn):
    [res] = queue.submit(conn, [_job()])
    job_id = res["job_id"]
    _lease(conn)
    assert queue.cancel(conn, job_id)
    hb = queue.heartbeat(conn, job_id, "node-a")
    assert hb["action"] == "cancel"


def test_capability_pinning(conn):
    queue.submit(conn, [_job(required_capability="builder")])
    assert _lease(conn, caps=("x86_64",)) is None
    leased = _lease(conn, node="watchman", caps=("x86_64", "builder"))
    assert leased is not None


def test_priority_and_toolkit_affinity(conn):
    [low] = queue.submit(conn, [_job(manifest_sha="m-low", priority=10)])
    queue.submit(conn, [_job(manifest_sha="m-high", priority=90)])
    leased = _lease(conn)
    assert leased["job_id"] == low["job_id"]  # lower number leases first

    # Among equal priorities, prefer the job whose toolkit the node caches.
    queue.submit(conn, [_job(manifest_sha="m-a", priority=50, toolkit_sha="a" * 64)])
    [cached_job] = queue.submit(
        conn, [_job(manifest_sha="m-b", priority=50, toolkit_sha="c" * 64)]
    )
    leased = _lease(conn, node="node-c", cached=("c" * 64,))
    assert leased["job_id"] == cached_job["job_id"]
