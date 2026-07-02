"""FR-005/FR-006 determinism audit (T046) — runs fully locally.

Identical inputs must produce identical bundle bytes and manifest hashes, and
an identical batch resubmission must short-circuit to the cached result.
"""
import threading

from tools.conveyor.bundles.build_job import build_job_bundle
from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator import queue


def test_bundle_and_manifest_bit_identical(tmp_path):
    manifest = {
        "job_type": "compile_score", "toolkit_sha": "t" * 64,
        "cells": [{"candidate_id": "a", "source": "a.c", "flagset": "-O2",
                   "targets": [{"target_id": "x", "file": "x.o"}]}],
    }
    files = {"a.c": b"int a;", "x.o": b"\x7fELF"}
    p1, s1 = build_job_bundle(manifest, files, tmp_path / "1.tar.gz")
    p2, s2 = build_job_bundle(dict(reversed(list(manifest.items()))), files,
                              tmp_path / "2.tar.gz")
    assert s1 == s2
    assert p1.read_bytes() == p2.read_bytes()


def test_identical_resubmission_short_circuits(tmp_path):
    conn = dbmod.connect(tmp_path / "db.sqlite")
    job = {"job_type": "compile_score", "manifest_sha": "m" * 64,
           "bundle_sha": "b" * 64, "toolkit_sha": "t" * 64}
    [first] = queue.submit(conn, [dict(job)])
    queue.lease(conn, "n", "n", ["x86_64"], 1)
    queue.submit_result(conn, first["job_id"], "n", "r" * 64)
    for _ in range(3):  # stays cached however many times it's re-asked
        [again] = queue.submit(conn, [dict(job)])
        assert again == {"cached_result": "r" * 64}
    conn.close()
