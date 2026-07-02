"""End-to-end smoke: requires a live coordinator with a pinned toolkit and at
least one attached x86-64 node (see quickstart.md). Run with:

    pytest tests/conveyor/integration -m node_required --coordinator http://<pi>:8323
"""
import os
import subprocess
import sys

import pytest

pytestmark = pytest.mark.node_required

COORDINATOR = os.environ.get("CONVEYOR_COORDINATOR", "http://127.0.0.1:8323")


def test_strlen_smoke_end_to_end():
    proc = subprocess.run(
        [sys.executable, "-m", "tools.conveyor.cli",
         "--coordinator", COORDINATOR, "smoke", "--function", "strlen"],
        capture_output=True, text=True, timeout=700,
    )
    assert proc.returncode == 0, proc.stdout + proc.stderr
    assert "SMOKE PASS" in proc.stdout
