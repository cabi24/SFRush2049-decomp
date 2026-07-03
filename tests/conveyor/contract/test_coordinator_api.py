"""Contract tests: a real coordinator on a random port, exercised over HTTP
against contracts/coordinator-api.md.
"""
import gzip
import io
import json
import tarfile
import threading
import urllib.error
import urllib.request

import pytest

from tools.conveyor.coordinator.server import make_server

API = "/api/v1"


def _result_bundle(job_id, exit_status="ok"):
    """A minimally valid results.tar.gz (the server inspects the envelope)."""
    envelope = json.dumps({"job_id": job_id, "exit": exit_status,
                           "payload": {}}).encode()
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w:gz") as tar:
        info = tarfile.TarInfo("result.json")
        info.size = len(envelope)
        tar.addfile(info, io.BytesIO(envelope))
    return buf.getvalue()


@pytest.fixture
def coord(tmp_path):
    httpd = make_server(tmp_path, port=0)  # random free port
    thread = threading.Thread(target=httpd.serve_forever, daemon=True)
    thread.start()
    base = f"http://127.0.0.1:{httpd.server_address[1]}"
    yield base, httpd.coordinator.token
    httpd.shutdown()


def _call(base, token, method, path, body=None, raw=None, headers=None):
    url = base + path
    data = None
    hdrs = dict(headers or {})
    if token is not None:
        hdrs["Authorization"] = f"Bearer {token}"
    if body is not None:
        data = json.dumps(body).encode()
        hdrs["Content-Type"] = "application/json"
    elif raw is not None:
        data = raw
        hdrs["Content-Type"] = "application/gzip"
    req = urllib.request.Request(url, data=data, method=method, headers=hdrs)
    try:
        with urllib.request.urlopen(req) as resp:
            payload = resp.read()
            return resp.status, json.loads(payload) if payload else None
    except urllib.error.HTTPError as e:
        payload = e.read()
        return e.code, json.loads(payload) if payload else None


def _submit_one(base, token, **overrides):
    job = {
        "job_type": "compile_score",
        "manifest_sha": "m" * 64,
        "bundle_sha": "b" * 64,
        "toolkit_sha": "t" * 64,
    }
    job.update(overrides)
    status, out = _call(base, token, "POST", f"{API}/work", body=[job])
    assert status == 201
    return out[0]


def test_healthz_needs_no_auth(coord):
    base, _ = coord
    status, out = _call(base, None, "GET", "/healthz")
    assert status == 200 and out["status"] == "ok"


def test_auth_required_elsewhere(coord):
    base, _ = coord
    status, out = _call(base, None, "GET", f"{API}/nodes")
    assert status == 401
    status, _ = _call(base, "wrong-token", "GET", f"{API}/nodes")
    assert status == 401


def test_lease_204_when_empty(coord):
    base, token = coord
    status, _ = _call(
        base, token, "POST", f"{API}/work/lease",
        body={"node_id": "n1", "capabilities": ["x86_64"], "cores": 4},
    )
    assert status == 204


def test_submit_lease_heartbeat_result_flow(coord):
    base, token = coord
    submitted = _submit_one(base, token)
    job_id = submitted["job_id"]

    status, job = _call(
        base, token, "POST", f"{API}/work/lease",
        body={"node_id": "n1", "capabilities": ["x86_64"], "cores": 4},
    )
    assert status == 200 and job["job_id"] == job_id
    assert job["toolkit_sha"] == "t" * 64 and job["heartbeat_seconds"] == 30

    status, hb = _call(
        base, token, "POST", f"{API}/work/{job_id}/heartbeat", body={"node_id": "n1"}
    )
    assert status == 200 and hb["action"] == "continue"

    # lease conflict from another node -> 409
    status, _ = _call(
        base, token, "POST", f"{API}/work/{job_id}/heartbeat", body={"node_id": "n2"}
    )
    assert status == 409

    result = _result_bundle(job_id)
    status, out = _call(
        base, token, "POST", f"{API}/work/{job_id}/result?node_id=n1", raw=result
    )
    assert status == 200 and out["accepted"] is True

    # duplicate result discarded
    status, out = _call(
        base, token, "POST", f"{API}/work/{job_id}/result?node_id=n2", raw=result
    )
    assert status == 200 and out["accepted"] is False and out["reason"] == "duplicate"

    status, job = _call(base, token, "GET", f"{API}/work/{job_id}")
    assert status == 200 and job["state"] == "DONE"


def test_stale_node_result_rejected_over_live_lease(coord):
    base, token = coord
    submitted = _submit_one(base, token, manifest_sha="s" * 64)
    job_id = submitted["job_id"]
    _call(base, token, "POST", f"{API}/work/lease",
          body={"node_id": "live", "capabilities": ["x86_64"], "cores": 1})
    # A node that does NOT hold the lease may not complete the job.
    status, out = _call(
        base, token, "POST", f"{API}/work/{job_id}/result?node_id=stale",
        raw=_result_bundle(job_id),
    )
    assert status == 200 and out["accepted"] is False
    assert out["reason"] == "lease_mismatch"
    status, job = _call(base, token, "GET", f"{API}/work/{job_id}")
    assert job["state"] == "LEASED" and job["leased_by"] == "live"


def test_error_result_reissues_and_never_caches(coord):
    base, token = coord
    submitted = _submit_one(base, token, manifest_sha="e" * 64)
    job_id = submitted["job_id"]
    _call(base, token, "POST", f"{API}/work/lease",
          body={"node_id": "n1", "capabilities": ["x86_64"], "cores": 1})
    status, out = _call(
        base, token, "POST", f"{API}/work/{job_id}/result?node_id=n1",
        raw=_result_bundle(job_id, exit_status="error"),
    )
    assert status == 200 and out["accepted"] is True
    status, job = _call(base, token, "GET", f"{API}/work/{job_id}")
    assert job["state"] == "PENDING"  # re-issued, not completed
    # And an identical submission is NOT served the error result from cache.
    again = _submit_one(base, token, manifest_sha="e" * 64)
    assert "cached_result" not in again


def test_cached_result_short_circuit(coord):
    base, token = coord
    first = _submit_one(base, token, manifest_sha="c" * 64)
    _call(base, token, "POST", f"{API}/work/lease",
          body={"node_id": "n1", "capabilities": ["x86_64"], "cores": 1})
    _call(base, token, "POST", f"{API}/work/{first['job_id']}/result?node_id=n1",
          raw=_result_bundle(first["job_id"]))
    again = _submit_one(base, token, manifest_sha="c" * 64)
    assert "cached_result" in again


def test_blob_roundtrip(coord):
    base, token = coord
    payload = gzip.compress(b"hello bundle")
    status, out = _call(base, token, "POST", f"{API}/blobs", raw=payload)
    assert status == 201
    sha = out["sha256"]
    req = urllib.request.Request(
        f"{base}{API}/blobs/{sha}", headers={"Authorization": f"Bearer {token}"}
    )
    with urllib.request.urlopen(req) as resp:
        assert resp.status == 200
        assert gzip.decompress(resp.read()) == b"hello bundle"
    status, _ = _call(base, token, "GET", f"{API}/blobs/{'0' * 64}")
    assert status == 404


def test_cancel_flows_to_heartbeat(coord):
    base, token = coord
    submitted = _submit_one(base, token, manifest_sha="d" * 64)
    job_id = submitted["job_id"]
    _call(base, token, "POST", f"{API}/work/lease",
          body={"node_id": "n1", "capabilities": ["x86_64"], "cores": 1})
    status, _ = _call(base, token, "POST", f"{API}/work/{job_id}/cancel")
    assert status == 200
    status, hb = _call(
        base, token, "POST", f"{API}/work/{job_id}/heartbeat", body={"node_id": "n1"}
    )
    assert status == 200 and hb["action"] == "cancel"


def test_status_and_nodes_shapes(coord):
    base, token = coord
    _call(base, token, "POST", f"{API}/work/lease",
          body={"node_id": "n1", "hostname": "box", "capabilities": ["x86_64"], "cores": 8})
    status, out = _call(base, token, "GET", f"{API}/status")
    assert status == 200
    assert set(out) == {"functions", "human_attention", "queue", "nodes"}
    assert set(out["queue"]) == {"pending", "leased", "done", "failed"}
    status, nodes = _call(base, token, "GET", f"{API}/nodes")
    assert status == 200 and nodes[0]["hostname"] == "box"


def test_malformed_submit_422(coord):
    base, token = coord
    status, _ = _call(base, token, "POST", f"{API}/work", body=[{"job_type": "x"}])
    assert status == 422
    status, _ = _call(base, token, "POST", f"{API}/work", body={"not": "a list"})
    assert status == 422


def test_unknown_job_404(coord):
    base, token = coord
    status, _ = _call(base, token, "GET", f"{API}/work/{'0' * 8}")
    assert status == 404
