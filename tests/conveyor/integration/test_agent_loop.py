"""Full fabric loop without IDO: coordinator + real node agent + real runner.

The compile step fails by design (no IDO on this machine) — what's under test
is the fabric itself: lease, toolkit fetch+cache+sha-verify, bundle unpack,
runner dispatch, error-envelope result, upload, DONE state.
"""
import json
import subprocess
import sys
import tarfile
import threading
import urllib.request
from pathlib import Path

import pytest

from tools.conveyor.bundles.build_job import build_job_bundle
from tools.conveyor.bundles.build_toolkit import build_toolkit
from tools.conveyor.coordinator.server import make_server

REPO = Path(__file__).resolve().parents[3]
API = "/api/v1"


@pytest.fixture
def coord(tmp_path):
    httpd = make_server(tmp_path / "data", port=0)
    threading.Thread(target=httpd.serve_forever, daemon=True).start()
    yield httpd
    httpd.shutdown()


def _call(base, token, method, path, body=None, raw=None):
    headers = {"Authorization": f"Bearer {token}"}
    data = None
    if body is not None:
        data = json.dumps(body).encode()
    elif raw is not None:
        data = raw
        headers["Content-Type"] = "application/gzip"
    req = urllib.request.Request(base + path, data=data, method=method, headers=headers)
    with urllib.request.urlopen(req) as resp:
        payload = resp.read()
        return resp.status, json.loads(payload) if payload else None


def test_agent_executes_job_end_to_end(coord, tmp_path):
    base = f"http://127.0.0.1:{coord.server_address[1]}"
    token = coord.coordinator.token

    # Minimal toolkit: real jobs/ + permuter, dummy ido/objdump/shim dirs.
    (tmp_path / "empty").mkdir()
    (tmp_path / "empty" / ".keep").write_text("")
    toolkit_path = build_toolkit(
        ido_dir=tmp_path / "empty",
        objdump_path="/bin/true",
        permuter_dir=REPO / "tools" / "decomp-permuter",
        shim_dir=tmp_path / "empty",
        jobs_dir=REPO / "tools" / "conveyor" / "jobs",
        out_path=tmp_path / "toolkit.tar.gz",
    )
    _, out = _call(base, token, "POST", f"{API}/blobs?kind=toolkit",
                   raw=toolkit_path.read_bytes())
    toolkit_sha = out["sha256"]

    manifest = {
        "job_type": "compile_score",
        "toolkit_sha": toolkit_sha,
        "cells": [{
            "candidate_id": "x.c:f", "source": "f.c", "flagset": "-O2",
            "targets": [{"target_id": "f", "file": "t.o"}],
        }],
    }
    bundle, manifest_sha = build_job_bundle(
        manifest, {"f.c": b"int f(void){return 0;}\n", "t.o": b"\x7fELF"},
        tmp_path / "job.tar.gz",
    )
    _, out = _call(base, token, "POST", f"{API}/blobs", raw=bundle.read_bytes())
    _, submitted = _call(base, token, "POST", f"{API}/work", body=[{
        "job_type": "compile_score", "manifest_sha": manifest_sha,
        "bundle_sha": out["sha256"], "toolkit_sha": toolkit_sha,
    }])
    job_id = submitted[0]["job_id"]

    # Run the real agent for exactly one job.
    proc = subprocess.run(
        [sys.executable, str(REPO / "tools/conveyor/agent/node_agent.py"),
         "--coordinator", base, "--token", token, "--cores", "1",
         "--cache", str(tmp_path / "cache"), "--once"],
        capture_output=True, text=True, timeout=120,
    )
    assert "result accepted" in proc.stdout, proc.stdout + proc.stderr

    _, job = _call(base, token, "GET", f"{API}/work/{job_id}")
    assert job["state"] == "DONE"

    # Toolkit was cached by sha; result envelope reports the compile failure.
    assert (tmp_path / "cache" / "toolkits" / toolkit_sha).is_dir()
    req = urllib.request.Request(
        f"{base}{API}/blobs/{job['result_sha']}",
        headers={"Authorization": f"Bearer {token}"},
    )
    result_file = tmp_path / "results.tar.gz"
    with urllib.request.urlopen(req) as resp:
        result_file.write_bytes(resp.read())
    with tarfile.open(result_file) as tar:
        result = json.loads(tar.extractfile("result.json").read())
    assert result["job_id"] == job_id
    # No IDO in this stub toolkit -> the runner reports a job-level error in
    # the contract envelope rather than crashing the agent or losing the job.
    assert result["exit"] == "error"
    assert "IDO" in result["error"]
