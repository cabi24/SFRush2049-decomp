"""`cli gc`: reclaim unreferenced job/result blobs, sparing everything live."""
import argparse

import pytest

from tools.conveyor import cli
from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator.store import BlobStore


@pytest.fixture
def env(tmp_path):
    conn = dbmod.connect(tmp_path / "conveyor.db")
    store = BlobStore(tmp_path / "blobs")
    yield tmp_path, conn, store
    conn.close()


def _mkblob(conn, store, payload, kind, age_days):
    sha = store.put_bytes(payload.encode())
    conn.execute(
        "INSERT INTO blob(sha256, kind, size_bytes, created_at)"
        " VALUES (?,?,?, strftime('%Y-%m-%dT%H:%M:%fZ','now', ?))",
        (sha, kind, store.size(sha), f"-{age_days} days"))
    conn.commit()
    return sha


def _gc(tmp_path, **kw):
    args = argparse.Namespace(data=str(tmp_path), coordinator="http://x",
                              token=None, days=7, apply=False, verbose=False)
    for k, v in kw.items():
        setattr(args, k, v)
    cli.cmd_gc(args)


def test_gc_deletes_only_old_unreferenced_job_and_result_blobs(env):
    tmp_path, conn, store = env
    old_job = _mkblob(conn, store, "A", "job", 30)
    old_res = _mkblob(conn, store, "B", "result", 30)
    new_job = _mkblob(conn, store, "C", "job", 1)
    referenced = _mkblob(conn, store, "D", "result", 30)
    toolkit = _mkblob(conn, store, "E", "toolkit", 30)
    conn.execute(
        "INSERT INTO work_unit(job_id, job_type, manifest_sha, result_sha,"
        " created_at, updated_at) VALUES ('j1','x','m1',?, 'now','now')",
        (referenced,))
    conn.commit()

    _gc(tmp_path, apply=True)

    present = lambda s: store.get(s) is not None
    assert not present(old_job) and not present(old_res)     # collected
    assert present(new_job)                                  # too new
    assert present(referenced)                               # still referenced
    assert present(toolkit)                                  # never touched
    remaining = {r["sha256"] for r in conn.execute("SELECT sha256 FROM blob")}
    assert remaining == {new_job, referenced, toolkit}


def test_gc_dry_run_deletes_nothing(env):
    tmp_path, conn, store = env
    sha = _mkblob(conn, store, "A", "job", 30)

    _gc(tmp_path)  # apply defaults to False

    assert store.get(sha) is not None
    assert conn.execute("SELECT COUNT(*) c FROM blob").fetchone()["c"] == 1


def test_gc_respects_object_shas_references(env):
    tmp_path, conn, store = env
    nested = _mkblob(conn, store, "Z", "result", 30)
    conn.execute(
        "INSERT INTO arcade_candidate(candidate_id, name, src_file, body_sha,"
        " object_shas) VALUES ('c1','fn','f.c','deadbeef', ?)",
        (f'{{"-O2": "{nested}"}}',))
    conn.commit()

    _gc(tmp_path, apply=True)

    assert store.get(nested) is not None
