"""Coordinator HTTP server (stdlib ThreadingHTTPServer + JSON).

Implements specs/001-matching-pipeline/contracts/coordinator-api.md.
One SQLite connection guarded by a lock: the coordinator is the single
writer and load is tens of requests/minute, so this is deliberately simple.
"""
import base64
import gzip
import json
import re
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

from . import auth as authmod
from . import db as dbmod
from . import queue as queuemod
from .store import BlobStore

API = "/api/v1"
AGENT_SOURCE = Path(__file__).resolve().parent.parent / "agent" / "node_agent.py"


class Coordinator:
    """Owns state; the HTTP handler delegates everything here."""

    def __init__(self, data_dir):
        self.data_dir = Path(data_dir)
        self.data_dir.mkdir(parents=True, exist_ok=True)
        self.conn = dbmod.connect(self.data_dir / "conveyor.db")
        self.store = BlobStore(self.data_dir / "blobs")
        self.token = authmod.load_or_create_token(self.data_dir)
        self.lock = threading.Lock()
        self.draining = False

    # -- node-facing ------------------------------------------------------
    def lease(self, body):
        with self.lock:
            return queuemod.lease(
                self.conn,
                body["node_id"],
                body.get("hostname", ""),
                body.get("capabilities", ["x86_64"]),
                body.get("cores", 1),
                cached_toolkits=body.get("cached_toolkits", []),
                agent_version=body.get("agent_version"),
            )

    def heartbeat(self, job_id, body):
        progress = body.get("progress")
        if progress and progress.get("best_source"):
            # Inline gz+b64 best-so-far source: persist to the blob store so
            # a dead node loses at most one heartbeat interval of progress.
            raw = gzip.decompress(base64.b64decode(progress["best_source"]))
            with self.lock:
                progress["best_source_sha"] = self.store.put_bytes(raw)
                self._record_blob(progress["best_source_sha"], "result")
        with self.lock:
            return queuemod.heartbeat(self.conn, job_id, body["node_id"], progress)

    def result(self, job_id, node_id, reader, length):
        # Stream OUTSIDE the lock: put_stream writes to a unique temp file and
        # renames atomically, so a slow multi-MB upload must never block
        # leases/heartbeats (a >120s stall would mass-expire healthy leases).
        sha = self.store.put_stream(reader, length)
        result_ok = self._result_exit_ok(sha)
        with self.lock:
            self._record_blob(sha, "result")
            status = queuemod.submit_result(
                self.conn, job_id, node_id, sha, result_ok=result_ok
            )
        return status, sha

    def _result_exit_ok(self, sha):
        """True iff the uploaded result bundle's envelope says exit == 'ok'.
        Malformed bundles count as errors — they must not complete a job."""
        import tarfile

        path = self.store.get(sha)
        try:
            with tarfile.open(path) as tar:
                envelope = json.loads(tar.extractfile("result.json").read())
            return envelope.get("exit") == "ok"
        except (tarfile.TarError, KeyError, ValueError, OSError):
            return False

    # -- operator-facing ---------------------------------------------------
    def put_blob(self, reader, length, kind="job"):
        sha = self.store.put_stream(reader, length)  # outside the lock; see result()
        with self.lock:
            self._record_blob(sha, kind)
        return sha

    def submit_work(self, jobs):
        with self.lock:
            return queuemod.submit(self.conn, jobs)

    def cancel(self, job_id):
        with self.lock:
            return queuemod.cancel(self.conn, job_id)

    def get_job(self, job_id):
        with self.lock:
            return queuemod.get_job(self.conn, job_id)

    def nodes(self):
        with self.lock:
            rows = self.conn.execute(
                "SELECT node_id, hostname, capabilities, cores, last_seen FROM node"
            ).fetchall()
            running = {
                r["leased_by"]: r["n"]
                for r in self.conn.execute(
                    "SELECT leased_by, COUNT(*) AS n FROM work_unit"
                    " WHERE state='LEASED' GROUP BY leased_by"
                )
            }
        return [
            {
                "node_id": r["node_id"],
                "hostname": r["hostname"],
                "capabilities": json.loads(r["capabilities"]),
                "cores": r["cores"],
                "last_seen": r["last_seen"],
                "running": running.get(r["node_id"], 0),
            }
            for r in rows
        ]

    def status(self):
        with self.lock:
            queuemod.expire_stale(self.conn)
            statuses = {
                r["status"]: r["n"]
                for r in self.conn.execute(
                    "SELECT status, COUNT(*) AS n FROM function_status GROUP BY status"
                )
            }
            attention = [
                {
                    "target_id": r["target_id"],
                    "reason": r["human_flag"],
                    "best_score": r["best_score"],
                }
                for r in self.conn.execute(
                    "SELECT target_id, human_flag, best_score FROM function_status"
                    " WHERE human_flag IS NOT NULL"
                    " ORDER BY best_score IS NULL, best_score LIMIT 100"
                )
            ]
            q = queuemod.counts(self.conn)
        return {
            "functions": {
                s: statuses.get(s, 0)
                for s in (
                    "unmatched",
                    "candidate_identified",
                    "seeded",
                    "in_search",
                    "matched",
                    "verified",
                )
            },
            "human_attention": attention,
            "queue": {k: q.get(k, 0) for k in ("pending", "leased", "done", "failed")},
            "nodes": self.nodes(),
        }

    def health(self):
        with self.lock:
            q = queuemod.counts(self.conn)
        return {
            "status": "draining" if self.draining else "ok",
            "pending": q.get("pending", 0),
            "leased": q.get("leased", 0),
        }

    def get_meta(self, key):
        with self.lock:
            return dbmod.get_meta(self.conn, key)

    def set_meta(self, key, value):
        with self.lock:
            dbmod.set_meta(self.conn, key, value)

    def _record_blob(self, sha, kind):
        size = self.store.size(sha) or 0
        self.conn.execute(
            "INSERT OR IGNORE INTO blob (sha256, kind, size_bytes, created_at)"
            " VALUES (?, ?, ?, strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
            (sha, kind, size),
        )
        self.conn.commit()


_ROUTES = [
    ("POST", re.compile(rf"^{API}/work/lease$"), "h_lease"),
    ("POST", re.compile(rf"^{API}/work/(?P<job_id>[0-9a-f-]+)/heartbeat$"), "h_heartbeat"),
    ("POST", re.compile(rf"^{API}/work/(?P<job_id>[0-9a-f-]+)/result$"), "h_result"),
    ("POST", re.compile(rf"^{API}/work/(?P<job_id>[0-9a-f-]+)/cancel$"), "h_cancel"),
    ("POST", re.compile(rf"^{API}/work$"), "h_submit"),
    ("GET", re.compile(rf"^{API}/work/(?P<job_id>[0-9a-f-]+)$"), "h_get_job"),
    ("POST", re.compile(rf"^{API}/blobs$"), "h_put_blob"),
    ("GET", re.compile(rf"^{API}/blobs/(?P<sha>[0-9a-f]{{64}})$"), "h_get_blob"),
    ("GET", re.compile(rf"^{API}/meta/(?P<key>[\w.-]+)$"), "h_get_meta"),
    ("PUT", re.compile(rf"^{API}/meta/(?P<key>[\w.-]+)$"), "h_put_meta"),
    ("GET", re.compile(rf"^{API}/status$"), "h_status"),
    ("GET", re.compile(rf"^{API}/nodes$"), "h_nodes"),
    ("GET", re.compile(r"^/agent/node_agent\.py$"), "h_agent"),
    ("GET", re.compile(r"^/healthz$"), "h_healthz"),
]

_NO_AUTH = {"h_healthz"}


class Handler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"
    coordinator = None  # set by make_server

    # -- plumbing ----------------------------------------------------------
    def log_message(self, fmt, *args):  # quiet by default
        pass

    def _json(self, code, obj, extra_headers=()):
        self.send_response(code)
        for k, v in extra_headers:
            self.send_header(k, v)
        if code == 204:
            self.send_header("Content-Length", "0")
            self.end_headers()
            return
        body = json.dumps(obj).encode()
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _read_json(self):
        length = int(self.headers.get("Content-Length", 0))
        if length == 0:
            return {}
        return json.loads(self.rfile.read(length))

    def _dispatch(self, method):
        path, _, query = self.path.partition("?")
        for m, pattern, name in _ROUTES:
            if m != method:
                continue
            match = pattern.match(path)
            if not match:
                continue
            if name not in _NO_AUTH and not authmod.check_bearer(
                self.headers.get("Authorization"), self.coordinator.token
            ):
                return self._json(401, {"error": "unauthorized"})
            if self.coordinator.draining and name in ("h_lease", "h_submit"):
                return self._json(503, {"error": "draining"})
            try:
                return getattr(self, name)(match.groupdict(), query)
            except (ValueError, KeyError, json.JSONDecodeError) as exc:
                return self._json(422, {"error": str(exc)})
        self._json(404, {"error": "not found"})

    def do_GET(self):
        self._dispatch("GET")

    def do_POST(self):
        self._dispatch("POST")

    def do_PUT(self):
        self._dispatch("PUT")

    # -- handlers ----------------------------------------------------------
    def h_lease(self, params, query):
        body = self._read_json()
        if "node_id" not in body:
            return self._json(422, {"error": "node_id required"})
        job = self.coordinator.lease(body)
        if job is None:
            return self._json(204, {}, extra_headers=[("Retry-After", "15")])
        self._json(200, job)

    def h_heartbeat(self, params, query):
        body = self._read_json()
        out = self.coordinator.heartbeat(params["job_id"], body)
        if out is None:
            return self._json(409, {"error": "lease not held"})
        self._json(200, out)

    def h_result(self, params, query):
        node_id = dict(
            p.split("=", 1) for p in query.split("&") if "=" in p
        ).get("node_id", "")
        length = int(self.headers.get("Content-Length", 0))
        status, sha = self.coordinator.result(
            params["job_id"], node_id, self.rfile, length
        )
        if status is None:
            return self._json(404, {"error": "unknown job"})
        self._json(200, {"accepted": status == "accepted",
                         **({"reason": status} if status != "accepted" else {})})

    def h_cancel(self, params, query):
        if not self.coordinator.cancel(params["job_id"]):
            return self._json(404, {"error": "unknown job"})
        self._json(200, {"cancelled": True})

    def h_submit(self, params, query):
        jobs = self._read_json()
        if not isinstance(jobs, list):
            return self._json(422, {"error": "expected a list of jobs"})
        for job in jobs:
            for field in ("job_type", "manifest_sha"):
                if field not in job:
                    return self._json(422, {"error": f"job missing {field}"})
        self._json(201, self.coordinator.submit_work(jobs))

    def h_get_job(self, params, query):
        job = self.coordinator.get_job(params["job_id"])
        if job is None:
            return self._json(404, {"error": "unknown job"})
        self._json(200, job)

    def h_put_blob(self, params, query):
        length = int(self.headers.get("Content-Length", 0))
        kind = dict(
            p.split("=", 1) for p in query.split("&") if "=" in p
        ).get("kind", "job")
        sha = self.coordinator.put_blob(self.rfile, length, kind)
        self._json(201, {"sha256": sha})

    def h_get_blob(self, params, query):
        path = self.coordinator.store.get(params["sha"])
        if path is None:
            return self._json(404, {"error": "unknown blob"})
        size = path.stat().st_size
        self.send_response(200)
        self.send_header("Content-Type", "application/gzip")
        self.send_header("Content-Length", str(size))
        self.end_headers()
        with open(path, "rb") as f:
            while True:
                chunk = f.read(1024 * 1024)
                if not chunk:
                    break
                self.wfile.write(chunk)

    def h_status(self, params, query):
        self._json(200, self.coordinator.status())

    def h_nodes(self, params, query):
        self._json(200, self.coordinator.nodes())

    def h_agent(self, params, query):
        body = AGENT_SOURCE.read_bytes()
        self.send_response(200)
        self.send_header("Content-Type", "text/x-python")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def h_get_meta(self, params, query):
        value = self.coordinator.get_meta(params["key"])
        if value is None:
            return self._json(404, {"error": "unknown key"})
        self._json(200, {"key": params["key"], "value": value})

    def h_put_meta(self, params, query):
        body = self._read_json()
        if "value" not in body:
            return self._json(422, {"error": "value required"})
        self.coordinator.set_meta(params["key"], str(body["value"]))
        self._json(200, {"key": params["key"], "value": str(body["value"])})

    def h_healthz(self, params, query):
        self._json(200, self.coordinator.health())


def make_server(data_dir, port=8323, host=""):
    coordinator = Coordinator(data_dir)
    handler = type("BoundHandler", (Handler,), {"coordinator": coordinator})
    httpd = ThreadingHTTPServer((host, port), handler)
    httpd.coordinator = coordinator
    return httpd


def serve(data_dir, port=8323, host=""):
    httpd = make_server(data_dir, port, host)
    print(f"conveyor coordinator on :{httpd.server_address[1]}  data={data_dir}")
    print(f"token: {httpd.coordinator.token}")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
