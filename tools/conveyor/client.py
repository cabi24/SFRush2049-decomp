"""Shared coordinator HTTP client + token loading for all Pi-side tools.

One implementation replaces the four divergent copies that lived in cli.py,
pipeline/matrix.py, pipeline/farm.py, and pipeline/sweep.py. Non-2xx responses
are returned as (status, body) — they never raise — so long-running daemons
degrade gracefully instead of dying on a 404/503 (agent/node_agent.py keeps
its own copy by design: it must stay a single curl-able file).
"""
import json
import sys
import urllib.error
import urllib.request
from pathlib import Path

DEFAULT_DATA = Path("~/.conveyor").expanduser()


def load_token(token=None, data_dir=None):
    """Explicit token, else <data>/token file, else a friendly exit."""
    if token:
        return token
    token_file = Path(data_dir or DEFAULT_DATA) / "token"
    if token_file.is_file():
        return token_file.read_text().strip()
    sys.exit(f"no --token given and {token_file} not found")


class Http:
    def __init__(self, base, token):
        self.base = base.rstrip("/")
        self.token = token

    def call(self, method, path, body=None, raw=None, timeout=300):
        """Returns (status, parsed_json_or_None). Never raises on HTTP errors."""
        headers = {"Authorization": f"Bearer {self.token}"}
        data = None
        if body is not None:
            data = json.dumps(body).encode()
            headers["Content-Type"] = "application/json"
        elif raw is not None:
            data = raw
            headers["Content-Type"] = "application/gzip"
        req = urllib.request.Request(self.base + path, data=data, method=method,
                                     headers=headers)
        try:
            with urllib.request.urlopen(req, timeout=timeout) as resp:
                payload = resp.read()
                return resp.status, json.loads(payload) if payload else None
        except urllib.error.HTTPError as e:
            payload = e.read()
            try:
                return e.code, json.loads(payload) if payload else None
            except json.JSONDecodeError:
                return e.code, None

    def download(self, sha, dest, timeout=300):
        req = urllib.request.Request(
            f"{self.base}/api/v1/blobs/{sha}",
            headers={"Authorization": f"Bearer {self.token}"},
        )
        with urllib.request.urlopen(req, timeout=timeout) as resp:
            Path(dest).write_bytes(resp.read())

    def pinned_toolkit(self):
        """Current pinned toolkit sha, or a friendly exit if none."""
        status, meta = self.call("GET", "/api/v1/meta/toolkit_sha")
        if status != 200:
            sys.exit("no toolkit pinned — run publish-toolkit first")
        return meta["value"]
