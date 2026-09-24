"""Seed sweep: compile-and-score every m2c seed, no permuter.

Measured 2026-09-24: 12 of the first 17 game-code matches were already
byte-identical at base, each after a wasted four-hour search.
"""
import io
import json
import tarfile

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator.store import BlobStore
from tools.conveyor.pipeline import sweep


class StubHttp:
    def __init__(self):
        self.jobs = []

    def call(self, method, path, body=None, raw=None):
        if path.endswith("/blobs"):
            return 201, {"sha256": "b" * 64}
        if path.endswith("/work"):
            self.jobs.extend(body)
            return 201, [{"job_id": "j"} for _ in body]
        return 200, {}


def _target(conn, target_id, insn_count, sha):
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO n64_target (target_id,address,population,insn_count,"
            " target_o_sha,tier) VALUES (?,?, 'extracted',?,?,'reloc_aware')",
            (target_id, 0x80090000 + insn_count, insn_count, sha))


def _histogram(path, buckets):
    path.write_text(json.dumps({
        "run": {"population_complete": True},
        "targets": {name: {"bucket": bucket} for name, bucket in buckets.items()}}))


def test_only_compiled_targets_are_swept_smallest_first(tmp_path, monkeypatch):
    conn = dbmod.connect(tmp_path / "db.sqlite")
    store = BlobStore(tmp_path / "blobs")
    sha = store.put_bytes(b"\x7fELF")
    _target(conn, "big", 300, sha)
    _target(conn, "small", 9, sha)
    _target(conn, "blocked_one", 5, sha)
    histogram = tmp_path / "h.json"
    _histogram(histogram, {"big": "compiled", "small": "compiled",
                           "blocked_one": "blocked"})
    monkeypatch.setattr(sweep, "_seed", lambda *a: "int f(void){return 0;}\n")
    monkeypatch.setattr(sweep.autodecomp, "_context_sha", lambda *a, **k: "ctx")
    http = StubHttp()

    counts = sweep.submit(conn, store, http, "tk", flagsets=("-O2",),
                          histogram_path=histogram)

    assert counts["targets"] == 2 and counts["cells"] == 2
    assert [r["target_id"] for r in sweep.compiled_targets(conn, histogram)] == [
        "small", "big"]
    assert http.jobs and http.jobs[0]["priority"] == sweep.SWEEP_PRIORITY
    assert http.jobs[0]["batch"] is True          # deterministic: cache it


def test_submit_refuses_an_incomplete_population(tmp_path):
    conn = dbmod.connect(tmp_path / "db.sqlite")
    histogram = tmp_path / "h.json"
    histogram.write_text(json.dumps({"run": {"population_complete": False},
                                     "targets": {}}))
    with pytest.raises(ValueError, match="population is not complete"):
        sweep.compiled_targets(conn, histogram)


def _result_blob(store, cells, toolkit="tk"):
    payload = {"job_id": "j", "job_type": "compile_score", "exit": "ok",
               "toolkit_sha": toolkit, "payload": {"cells": cells}}
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w:gz") as tar:
        data = json.dumps(payload).encode()
        info = tarfile.TarInfo("result.json")
        info.size = len(data)
        tar.addfile(info, io.BytesIO(data))
    return store.put_bytes(buf.getvalue())


def test_ingest_records_scores_and_names_the_matches(tmp_path):
    conn = dbmod.connect(tmp_path / "db.sqlite")
    store = BlobStore(tmp_path / "blobs")
    _target(conn, "hit", 8, "osha")
    _target(conn, "miss", 12, "osha")
    sha = _result_blob(store, [
        {"candidate_id": "m2c:hit", "flagset": "-O2", "target_id": "hit",
         "score": 0, "compile": "ok", "target_o_sha": "osha"},
        {"candidate_id": "m2c:miss", "flagset": "-O2", "target_id": "miss",
         "score": 45, "compile": "ok", "target_o_sha": "osha"},
        {"candidate_id": "arcade:other", "flagset": "-O2", "target_id": "miss",
         "score": 3, "compile": "ok"},          # not ours: left alone
    ])
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO work_unit (job_id,job_type,manifest_sha,state,result_sha,"
            " created_at,updated_at) VALUES ('j','compile_score','m','DONE',?,'n','n')",
            (sha,))

    counts, zeros = sweep.ingest(conn, store)

    assert counts["cells"] == 2 and counts["zero_score_cells"] == 1
    assert zeros == [("hit", "-O2")]
    rows = dict(conn.execute(
        "SELECT target_id,score FROM matrix_entry WHERE candidate_id LIKE 'm2c:%'"))
    assert rows == {"hit": 0, "miss": 45}
    # attribution rides along so a target rebuild supersedes these scores
    assert conn.execute("SELECT target_o_sha FROM matrix_entry WHERE target_id='hit'"
                        ).fetchone()[0] == "osha"
    assert conn.execute("SELECT ingested_at IS NOT NULL FROM work_unit").fetchone()[0]

    again, _ = sweep.ingest(conn, store)
    assert again.get("cells", 0) == 0            # ingested once


def test_failed_compiles_are_counted_not_recorded_as_a_score(tmp_path):
    conn = dbmod.connect(tmp_path / "db.sqlite")
    store = BlobStore(tmp_path / "blobs")
    _target(conn, "broken", 20, "osha")
    sha = _result_blob(store, [{"candidate_id": "m2c:broken", "flagset": "-O2",
                                "target_id": "broken", "score": None,
                                "compile": "fail:syntax"}])
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO work_unit (job_id,job_type,manifest_sha,state,result_sha,"
            " created_at,updated_at) VALUES ('j','compile_score','m','DONE',?,'n','n')",
            (sha,))

    counts, zeros = sweep.ingest(conn, store)

    assert counts["no_score"] == 1 and zeros == []
    assert conn.execute("SELECT count(*) FROM matrix_entry").fetchone()[0] == 0
