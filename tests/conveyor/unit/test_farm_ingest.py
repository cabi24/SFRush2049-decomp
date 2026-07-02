"""Farm harvest/promotion state machine, tested with fabricated results —
no nodes, no HTTP calls that matter (captured by a stub).
"""
import io
import json
import tarfile

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator.store import BlobStore
from tools.conveyor.pipeline import farm


class StubHttp:
    """Captures submissions instead of talking to a coordinator."""

    def __init__(self):
        self.submitted = []

    def call(self, method, path, body=None, raw=None):
        if path.endswith("/blobs"):
            return 201, {"sha256": "f" * 64}
        if path.endswith("/work"):
            self.submitted.extend(body)
            return 201, [{"job_id": "stub"} for _ in body]
        return 200, {}


@pytest.fixture
def env(tmp_path):
    conn = dbmod.connect(tmp_path / "db.sqlite")
    store = BlobStore(tmp_path / "blobs")
    yield conn, store
    conn.close()


def _result_bundle(store, payload, artifacts=None):
    result = {"job_id": "j", "job_type": "permuter_search", "exit": "ok",
              "error": None, "payload": payload}
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w:gz") as tar:
        data = json.dumps(result).encode()
        info = tarfile.TarInfo("result.json")
        info.size = len(data)
        tar.addfile(info, io.BytesIO(data))
        for name, content in (artifacts or {}).items():
            info = tarfile.TarInfo(name)
            info.size = len(content)
            tar.addfile(info, io.BytesIO(content))
    return store.put_bytes(buf.getvalue())


def _mk_target(conn, target_id, status="in_search"):
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO n64_target (target_id, address, population, insn_count,"
            " target_o_sha) VALUES (?, 1, 'extracted', 10, ?)",
            (target_id, "0" * 64),
        )
        conn.execute(
            "INSERT INTO function_status (target_id, status, best_candidate_id,"
            " updated_at) VALUES (?, ?, 'game/game.c:game',"
            " strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
            (target_id, status),
        )


def _mk_done_job(conn, store, job_type, target_id, payload, artifacts=None):
    result_sha = _result_bundle(store, payload, artifacts)
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO work_unit (job_id, job_type, target_id, manifest_sha,"
            " state, result_sha, created_at, updated_at)"
            " VALUES (?, ?, ?, 'm', 'DONE', ?, '2026', '2026')",
            (f"job-{target_id}-{job_type}", job_type, target_id, result_sha),
        )


def test_win_becomes_matched_and_promotion_submitted(env):
    conn, store = env
    _mk_target(conn, "func_a")
    # target.o blob must exist for the promotion bundle
    o_sha = store.put_bytes(b"\x7fELFtarget")
    with dbmod.tx(conn):
        conn.execute("UPDATE n64_target SET target_o_sha=? WHERE target_id='func_a'",
                     (o_sha,))
    _mk_done_job(conn, store, "permuter_search", "func_a",
                 {"target_id": "func_a", "final_best_score": 0, "base_score": 42},
                 {"best.c": b"int f(void){return 0;}\n"})
    http = StubHttp()
    stats = farm.ingest(conn, store, http, "t" * 64)
    assert stats["harvested"] == 1
    row = conn.execute(
        "SELECT status FROM function_status WHERE target_id='func_a'").fetchone()
    assert row["status"] == "matched"
    assert http.submitted and http.submitted[0]["job_type"] == "verify_promote"
    assert http.submitted[0]["required_capability"] == "builder"


def test_stall_flags_for_attention(env):
    conn, store = env
    _mk_target(conn, "func_b")
    _mk_done_job(conn, store, "permuter_search", "func_b",
                 {"target_id": "func_b", "final_best_score": 77, "base_score": 90},
                 {"best.c": b"..."})
    stats = farm.ingest(conn, store, StubHttp(), "t" * 64)
    assert stats["stalled"] == 1
    row = conn.execute(
        "SELECT status, human_flag, best_score FROM function_status"
        " WHERE target_id='func_b'").fetchone()
    assert (row["status"], row["human_flag"], row["best_score"]) == \
        ("seeded", "stalled", 77)


def test_promotion_outcomes(env):
    conn, store = env
    for tid, outcome in (("func_c", "promoted"), ("func_d", "rolled_back:score")):
        _mk_target(conn, tid, status="matched")
        _mk_done_job(conn, store, "verify_promote", tid,
                     {"target_id": tid, "outcome": outcome,
                      "build_ok": outcome == "promoted",
                      "sha1_ok": outcome == "promoted",
                      "commit_hash": "abc" if outcome == "promoted" else None,
                      "doc_header_injected": True})
    stats = farm.ingest(conn, store, StubHttp(), "t" * 64)
    assert stats["promoted"] == 1 and stats["rolled_back"] == 1
    assert conn.execute("SELECT status FROM function_status WHERE target_id='func_c'"
                        ).fetchone()["status"] == "verified"
    row = conn.execute("SELECT status, human_flag FROM function_status"
                       " WHERE target_id='func_d'").fetchone()
    assert row["status"] == "seeded" and row["human_flag"].startswith("verify_failed")
    n = conn.execute("SELECT COUNT(*) AS n FROM promotion_record").fetchone()["n"]
    assert n == 2


def test_ingest_is_idempotent(env):
    conn, store = env
    _mk_target(conn, "func_e")
    _mk_done_job(conn, store, "permuter_search", "func_e",
                 {"target_id": "func_e", "final_best_score": 5, "base_score": 9},
                 {"best.c": b"..."})
    farm.ingest(conn, store, StubHttp(), "t" * 64)
    stats = farm.ingest(conn, store, StubHttp(), "t" * 64)  # second run: no-op
    assert stats == {"harvested": 0, "promoted": 0, "stalled": 0, "rolled_back": 0}
