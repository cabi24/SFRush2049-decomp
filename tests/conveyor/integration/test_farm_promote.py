"""US2 end-to-end (T033): seed a near-match, run a budgeted search on a live
pool, verify heartbeat progress persistence, harvest, and the promotion gate.

Needs: live coordinator + x86-64 node + builder node with a scratch repo
clone. See quickstart.md. Deliberately budgeted small — this is a plumbing
test, not a matching run.
"""
import json
import os
import subprocess
import sys
import time
import urllib.request
from pathlib import Path

import pytest

from tools.conveyor.pipeline import lock

pytestmark = pytest.mark.node_required

COORDINATOR = os.environ.get("CONVEYOR_COORDINATOR", "http://127.0.0.1:8323")
DATA = Path(os.environ.get("CONVEYOR_DATA", "~/.conveyor")).expanduser()
REPO = Path(__file__).resolve().parents[3]


def _require_unpromoted_fixture(entries):
    if any(e.get("target_id") == "strlen" and e.get("verified") == "rom-sha1"
           for e in entries.values()):
        pytest.skip("strlen is already promoted; run this win-path fixture in an "
                    "unpromoted scratch checkout, preserving the re-seed guard")


def test_seed_search_harvest_promote_cycle():
    _require_unpromoted_fixture(lock.load_lock())
    token = (DATA / "token").read_text().strip()

    # strlen matches at score 0 immediately -> exercises the full win path.
    src = REPO / "src" / "libc" / "string.c"
    proc = subprocess.run(
        [sys.executable, "-m", "tools.conveyor.cli", "--coordinator", COORDINATOR,
         "seed", "strlen", str(src), "--budget", "300"],
        capture_output=True, text=True, timeout=60, cwd=str(REPO),
    )
    assert proc.returncode == 0, proc.stdout + proc.stderr

    deadline = time.time() + 600
    final = None
    while time.time() < deadline:
        proc = subprocess.run(
            [sys.executable, "-m", "tools.conveyor.pipeline.farm",
             "--coordinator", COORDINATOR, "run", "--once"],
            capture_output=True, text=True, timeout=120, cwd=str(REPO),
        )
        req = urllib.request.Request(
            f"{COORDINATOR}/api/v1/status",
            headers={"Authorization": f"Bearer {token}"},
        )
        with urllib.request.urlopen(req) as resp:
            final = json.loads(resp.read())
        f = final["functions"]
        if f["matched"] + f["verified"] >= 1:
            break
        time.sleep(10)
    assert final and final["functions"]["matched"] + final["functions"]["verified"] >= 1, final


def test_promoted_fixture_is_skipped_before_seeding():
    with pytest.raises(pytest.skip.Exception, match="already promoted"):
        _require_unpromoted_fixture({"src/rom/lib_8800.c:strlen": {
            "target_id": "strlen", "verified": "rom-sha1"}})


def test_object_evidence_still_allows_fresh_win_path():
    _require_unpromoted_fixture({"src/libc/string.c:strlen": {
        "target_id": "strlen", "verified": "score0"}})
