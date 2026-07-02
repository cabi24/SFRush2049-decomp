#!/usr/bin/env python3
"""Conveyor node agent — single file, stdlib only.

Bootstrap on any x86-64 Linux box:

    curl -H "Authorization: Bearer <token>" http://<pi>:8323/agent/node_agent.py -o node_agent.py
    python3 node_agent.py --coordinator http://<pi>:8323 --token <token> --cores $(nproc)

The agent holds no project state: it pulls self-contained job bundles, caches
immutable toolkit bundles by sha256, executes in a scratch dir, posts results,
and deletes the scratch. Killing it (or the whole box) at any moment is safe —
leases expire and jobs re-issue.
"""
import argparse
import hashlib
import json
import os
import shutil
import signal
import socket
import subprocess
import sys
import tarfile
import tempfile
import threading
import time
import urllib.error
import urllib.request
import uuid
from pathlib import Path

AGENT_VERSION = "1"
API = "/api/v1"


class Api:
    def __init__(self, base, token):
        self.base = base.rstrip("/")
        self.token = token

    def _request(self, method, path, body=None, raw=None, timeout=60):
        headers = {"Authorization": f"Bearer {self.token}"}
        data = None
        if body is not None:
            data = json.dumps(body).encode()
            headers["Content-Type"] = "application/json"
        elif raw is not None:
            data = raw
            headers["Content-Type"] = "application/gzip"
        req = urllib.request.Request(
            self.base + path, data=data, method=method, headers=headers
        )
        try:
            with urllib.request.urlopen(req, timeout=timeout) as resp:
                payload = resp.read()
                return resp.status, json.loads(payload) if payload else None, dict(resp.headers)
        except urllib.error.HTTPError as e:
            payload = e.read()
            try:
                parsed = json.loads(payload) if payload else None
            except json.JSONDecodeError:
                parsed = None
            return e.code, parsed, dict(e.headers)

    def download(self, sha, dest):
        req = urllib.request.Request(
            f"{self.base}{API}/blobs/{sha}",
            headers={"Authorization": f"Bearer {self.token}"},
        )
        h = hashlib.sha256()
        with urllib.request.urlopen(req, timeout=300) as resp, open(dest, "wb") as out:
            for chunk in iter(lambda: resp.read(1024 * 1024), b""):
                h.update(chunk)
                out.write(chunk)
        if h.hexdigest() != sha:
            dest.unlink()
            raise RuntimeError(f"blob {sha[:12]} failed sha256 verification")


def _safe_extract(tar_path, dest):
    with tarfile.open(tar_path, "r:gz") as tar:
        for member in tar.getmembers():
            target = (dest / member.name).resolve()
            if not str(target).startswith(str(dest.resolve())):
                raise RuntimeError(f"unsafe path in bundle: {member.name}")
        tar.extractall(dest)


class Agent:
    def __init__(self, args):
        self.api = Api(args.coordinator, args.token)
        self.cores = args.cores
        self.capabilities = [c.strip() for c in args.capabilities.split(",") if c.strip()]
        self.cache = Path(args.cache).expanduser()
        self.cache.mkdir(parents=True, exist_ok=True)
        self.repo = args.repo
        self.node_id = self._load_node_id()
        self.stop = threading.Event()

    def _load_node_id(self):
        id_file = self.cache / "node_id"
        if id_file.is_file():
            return id_file.read_text().strip()
        node_id = str(uuid.uuid4())
        id_file.write_text(node_id + "\n")
        return node_id

    def _cached_toolkits(self):
        return [p.name for p in (self.cache / "toolkits").glob("*") if p.is_dir()]

    def toolkit_dir(self, sha):
        tk_root = self.cache / "toolkits"
        tk_root.mkdir(exist_ok=True)
        final = tk_root / sha
        if final.is_dir():
            return final
        print(f"[agent] fetching toolkit {sha[:12]}…")
        with tempfile.TemporaryDirectory(dir=str(tk_root)) as tmp:
            tar_path = Path(tmp) / "toolkit.tar.gz"
            self.api.download(sha, tar_path)
            unpack = Path(tmp) / "unpack"
            unpack.mkdir()
            _safe_extract(tar_path, unpack)
            os.replace(unpack, final)
        return final

    def run(self, once=False):
        print(f"[agent] node {self.node_id} caps={self.capabilities} cores={self.cores}")
        while not self.stop.is_set():
            status, job, headers = self.api._request(
                "POST",
                f"{API}/work/lease",
                body={
                    "node_id": self.node_id,
                    "hostname": socket.gethostname(),
                    "cores": self.cores,
                    "capabilities": self.capabilities,
                    "cached_toolkits": self._cached_toolkits(),
                    "agent_version": AGENT_VERSION,
                },
            )
            if status == 204 or status == 503:
                if once:
                    return False
                self.stop.wait(float(headers.get("Retry-After", 15)))
                continue
            if status == 426:
                print("[agent] coordinator requires a newer agent; re-download and restart")
                return False
            if status != 200:
                print(f"[agent] lease error {status}; backing off")
                self.stop.wait(30)
                continue
            self.execute(job)
            if once:
                return True
        return True

    def execute(self, job):
        job_id = job["job_id"]
        print(f"[agent] job {job_id[:8]} type={job['job_type']}")
        scratch = Path(tempfile.mkdtemp(prefix=f"conveyor-{job_id[:8]}-"))
        cancelled = threading.Event()
        proc_holder = {}
        try:
            toolkit = self.toolkit_dir(job["toolkit_sha"]) if job.get("toolkit_sha") else None
            bundle = scratch / "job.tar.gz"
            self.api.download(job["bundle_sha"], bundle)
            job_dir = scratch / "job"
            job_dir.mkdir()
            _safe_extract(bundle, job_dir)

            progress_file = job_dir / "progress.json"
            out_path = scratch / "results.tar.gz"

            hb = threading.Thread(
                target=self._heartbeat_loop,
                args=(job_id, job.get("heartbeat_seconds", 30), progress_file,
                      cancelled, proc_holder),
                daemon=True,
            )
            hb.start()

            env = dict(os.environ)
            if toolkit:
                env["CONVEYOR_TOOLKIT"] = str(toolkit)
            if self.repo:
                env["CONVEYOR_REPO"] = str(self.repo)
            env["CONVEYOR_CORES"] = str(self.cores)
            env["CONVEYOR_JOB_ID"] = job_id

            runner = (toolkit / "jobs" / "runner.py") if toolkit else None
            if runner is None or not runner.is_file():
                raise RuntimeError("toolkit has no jobs/runner.py")
            proc = subprocess.Popen(
                [sys.executable, str(runner), "--job", str(job_dir), "--out", str(out_path)],
                env=env, cwd=str(scratch),
                stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
            )
            proc_holder["proc"] = proc
            output, _ = proc.communicate()
            cancelled.set()  # stops the heartbeat loop

            if proc.returncode != 0 and not out_path.is_file():
                # Build a minimal error result so the coordinator learns why.
                self._pack_error(out_path, job, output.decode(errors="replace")[-4000:])
            self._upload_result(job_id, out_path)
        except Exception as exc:  # noqa: BLE001 — agent must survive anything
            print(f"[agent] job {job_id[:8]} failed locally: {exc}")
            try:
                out_path = scratch / "results.tar.gz"
                self._pack_error(out_path, job, str(exc))
                self._upload_result(job_id, out_path)
            except Exception:
                pass  # lease will expire; job re-issues
        finally:
            cancelled.set()
            shutil.rmtree(scratch, ignore_errors=True)

    def _heartbeat_loop(self, job_id, interval, progress_file, cancelled, proc_holder):
        while not cancelled.wait(interval):
            progress = None
            try:
                if progress_file.is_file():
                    progress = json.loads(progress_file.read_text())
            except (json.JSONDecodeError, OSError):
                progress = None
            body = {"node_id": self.node_id}
            if progress:
                body["progress"] = progress
            status, out, _ = self.api._request(
                "POST", f"{API}/work/{job_id}/heartbeat", body=body
            )
            if status == 409 or (status == 200 and out and out.get("action") == "cancel"):
                print(f"[agent] job {job_id[:8]} cancelled/lost; aborting")
                proc = proc_holder.get("proc")
                if proc and proc.poll() is None:
                    proc.send_signal(signal.SIGTERM)
                cancelled.set()
                return

    def _pack_error(self, out_path, job, message):
        result = {
            "job_id": job["job_id"],
            "job_type": job["job_type"],
            "toolkit_sha": job.get("toolkit_sha"),
            "exit": "error",
            "error": message,
            "payload": {},
        }
        with tempfile.TemporaryDirectory() as tmp:
            rj = Path(tmp) / "result.json"
            rj.write_text(json.dumps(result))
            with tarfile.open(out_path, "w:gz") as tar:
                tar.add(rj, arcname="result.json")

    def _upload_result(self, job_id, out_path):
        raw = out_path.read_bytes()
        status, out, _ = self.api._request(
            "POST", f"{API}/work/{job_id}/result?node_id={self.node_id}", raw=raw,
            timeout=300,
        )
        if status == 200:
            accepted = out.get("accepted")
            print(f"[agent] job {job_id[:8]} result {'accepted' if accepted else 'duplicate'}")
        else:
            print(f"[agent] job {job_id[:8]} result upload got {status}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--coordinator", required=True)
    parser.add_argument("--token", required=True)
    parser.add_argument("--cores", type=int, default=os.cpu_count() or 1)
    parser.add_argument("--capabilities", default="x86_64")
    parser.add_argument("--cache", default="~/.conveyor-cache")
    parser.add_argument("--repo", default=None,
                        help="canonical repo clone (builder nodes only)")
    parser.add_argument("--once", action="store_true",
                        help="run at most one job, then exit (for testing)")
    args = parser.parse_args()

    agent = Agent(args)
    signal.signal(signal.SIGTERM, lambda *a: agent.stop.set())
    agent.run(once=args.once)


if __name__ == "__main__":
    main()
