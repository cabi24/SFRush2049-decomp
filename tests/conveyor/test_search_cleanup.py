"""A cancelled or crashed search must not leave permuter workers running."""
import os
import signal
import subprocess
import sys
import textwrap
import time
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]

# A stand-in permuter: forks workers that sleep, records every pid, then waits.
FAKE_PERMUTER = textwrap.dedent('''
    import os, sys, time
    pids = [str(os.getpid())]
    for _ in range(3):
        child = os.fork()
        if child == 0:
            time.sleep(600)
            os._exit(0)
        pids.append(str(child))
    open(os.environ["PID_FILE"], "w").write(" ".join(pids))
    time.sleep(600)
''')

# What the node runs: install the runner's SIGTERM handler, then drive().
RUNNER = textwrap.dedent('''
    import sys
    from pathlib import Path
    import os
    sys.path.insert(0, %(jobs)r)
    import runner, permuter_search
    os.environ["CONVEYOR_TOOLKIT"] = sys.argv[2]      # after the imports: they use the real permuter

    class Progress:
        def update(self, **kw): pass

    runner.install_term_handler()
    permuter_search.drive(Path(sys.argv[1]), {"target_id": "f"}, Progress(), 100, 600, 2)
''')


def _alive(pid):
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False
    try:
        return "Z" not in Path(f"/proc/{pid}/stat").read_text().split(")")[-1].split()[0]
    except OSError:
        return False


@pytest.mark.skipif(not sys.platform.startswith("linux"), reason="needs /proc")
@pytest.mark.parametrize("how", ["sigterm", "wall_budget"])
def test_no_worker_survives_the_job(tmp_path, how):
    toolkit = tmp_path / "toolkit"
    (toolkit / "decomp-permuter").mkdir(parents=True)
    (toolkit / "decomp-permuter" / "permuter.py").write_text(FAKE_PERMUTER)
    perm_dir = tmp_path / "perm"
    perm_dir.mkdir()
    pid_file = tmp_path / "pids"
    budget = 600 if how == "sigterm" else 2
    script = RUNNER % {"jobs": str(REPO / "tools" / "conveyor" / "jobs")}
    script = script.replace("100, 600, 2)", f"100, {budget}, 2)")
    env = dict(os.environ, PID_FILE=str(pid_file))
    env.pop("CONVEYOR_TOOLKIT", None)
    runner = subprocess.Popen([sys.executable, "-c", script, str(perm_dir), str(toolkit)],
                              env=env)
    try:
        deadline = time.time() + 20
        while not pid_file.exists() and time.time() < deadline:
            time.sleep(0.1)
        assert pid_file.exists(), "the stand-in permuter never started"
        pids = [int(p) for p in pid_file.read_text().split()]
        assert all(_alive(p) for p in pids)
        if how == "sigterm":
            runner.send_signal(signal.SIGTERM)
        runner.wait(timeout=30)
        deadline = time.time() + 10
        while any(_alive(p) for p in pids) and time.time() < deadline:
            time.sleep(0.1)
        assert not [p for p in pids if _alive(p)], "permuter processes were left running"
    finally:
        if runner.poll() is None:
            runner.kill()
