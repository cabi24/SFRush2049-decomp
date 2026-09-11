"""006 close-out hardening: the farm daemon survives a flaky coordinator
path, the coordinator retries a locked one-row blob insert, and the
permuter job names its function explicitly."""
import http.client
import os
import sqlite3
import threading
import time

import pytest

from tools.conveyor.coordinator import server as servermod
from tools.conveyor.pipeline import farm


# -- farm: transient-error retry -------------------------------------------

def test_transient_error_is_retried_once_then_succeeds():
    calls, slept, logged = [], [], []

    def step():
        calls.append(1)
        if len(calls) == 1:
            raise http.client.RemoteDisconnected("closed")
        return {"ok": len(calls)}

    out = farm.with_transient_retry(step, sleep=slept.append, log=logged.append)

    assert out == {"ok": 2}
    assert slept == [farm.TRANSIENT_RETRY_DELAY]
    assert len(logged) == 1 and "retrying" in logged[0]


def test_persistent_transient_error_skips_the_cycle_without_raising():
    slept, logged = [], []

    def step():
        raise ConnectionRefusedError("coordinator down")

    out = farm.with_transient_retry(step, sleep=slept.append, log=logged.append)

    assert out is None
    assert len(slept) == 1            # one retry, then give up
    assert "skipped this cycle" in logged[-1]


def test_non_transient_errors_still_propagate():
    def step():
        raise KeyError("real bug")

    with pytest.raises(KeyError):
        farm.with_transient_retry(step, sleep=lambda _s: None, log=lambda _m: None)


def test_run_once_keeps_going_when_one_step_is_flaky(monkeypatch):
    monkeypatch.setattr(farm, "ingest", lambda *a: {"ingested": 1})
    monkeypatch.setattr(farm, "flywheel_cycle",
                        lambda *a: (_ for _ in ()).throw(ConnectionResetError()))
    monkeypatch.setattr(farm, "top_up", lambda *a: {"topped": 2})
    monkeypatch.setattr(farm.time, "sleep", lambda _s: None)

    stats = farm.run_once(None, None, None, "tk", 8, 60)

    assert stats == {"ingested": 1, "topped": 2}


# -- coordinator: locked blob insert retry --------------------------------

def test_record_blob_retries_while_database_is_locked(tmp_path, monkeypatch):
    monkeypatch.setattr(servermod, "BLOB_RECORD_RETRY_DELAY", 0.05)
    coord = servermod.Coordinator(tmp_path)
    sha = coord.store.put_bytes(b"blob")
    # Make the coordinator's own busy wait short so the retry loop is what
    # carries it over the lock, not the 30 s connection timeout.
    coord.conn.execute("PRAGMA busy_timeout=20")

    blocker = sqlite3.connect(str(tmp_path / "conveyor.db"), timeout=0)
    blocker.execute("BEGIN IMMEDIATE")          # hold the write lock
    errors = []

    def record():
        try:
            coord._record_blob(sha, "job")
        except Exception as exc:  # pragma: no cover - reported below
            errors.append(exc)

    worker = threading.Thread(target=record)
    worker.start()
    time.sleep(0.3)                             # spans several retry rounds
    blocker.commit()                            # release; the retry lands
    worker.join(timeout=5)
    blocker.close()

    assert not errors and not worker.is_alive()
    assert coord.conn.execute(
        "SELECT kind FROM blob WHERE sha256=?", (sha,)).fetchone()[0] == "job"


def test_record_blob_gives_up_after_retry_budget(tmp_path, monkeypatch):
    monkeypatch.setattr(servermod, "BLOB_RECORD_RETRY_DELAY", 0.01)
    monkeypatch.setattr(servermod, "BLOB_RECORD_RETRIES", 2)
    coord = servermod.Coordinator(tmp_path)
    sha = coord.store.put_bytes(b"blob")
    blocker = sqlite3.connect(str(tmp_path / "conveyor.db"), timeout=0)
    blocker.execute("BEGIN IMMEDIATE")
    coord.conn.execute("PRAGMA busy_timeout=10")
    try:
        with pytest.raises(sqlite3.OperationalError):
            coord._record_blob(sha, "job")
    finally:
        blocker.rollback()
        blocker.close()


# -- permuter_search: explicit function name --------------------------------

def test_permuter_job_writes_function_txt_from_manifest(tmp_path, monkeypatch):
    import subprocess
    import tools.conveyor.jobs.permuter_search as job

    inputs = tmp_path / "job" / "inputs"
    inputs.mkdir(parents=True)
    (inputs / "base.c").write_text("void f(void (*cb)(int)) { cb(1); }\n")
    (inputs / "target.o").write_bytes(b"")
    perm_dir = tmp_path / "perm"
    perm_dir.mkdir()
    monkeypatch.setattr(job.tempfile, "mkdtemp", lambda prefix="": str(perm_dir))
    monkeypatch.setattr(job, "_write_compile_sh", lambda d, flags: None)
    monkeypatch.setattr(job, "_toolkit", lambda: tmp_path)
    seen = {}

    def fake_run(cmd, **kw):                    # baseline compile "fails"
        seen["function.txt"] = (perm_dir / "function.txt").read_text()
        return subprocess.CompletedProcess(cmd, 1, "", "")

    monkeypatch.setattr(job.subprocess, "run", fake_run)
    monkeypatch.setattr(job.shutil, "rmtree", lambda *a, **k: None)

    out, artifacts = job.run(tmp_path / "job", {
        "target_id": "display_list_traverse", "seed_file": "base.c",
        "target_file": "target.o", "compile_flags": "", "budget": {},
    }, progress=type("P", (), {"update": lambda self, **k: None})())

    assert seen["function.txt"] == "display_list_traverse\n"
    assert out["error"] == "seed does not compile" and artifacts == {}


# -- toolkit portability: glibc never bundled, bundled objdump health-checked --

def test_toolkit_ldd_libs_exclude_glibc(monkeypatch, tmp_path):
    import subprocess
    from tools.conveyor.bundles import build_toolkit as bt

    ldd = (
        "\tlinux-vdso.so.1 (0x1)\n"
        "\tlibopcodes-2.38-mips.so => /usr/lib/x86_64-linux-gnu/libopcodes-2.38-mips.so (0x2)\n"
        "\tlibbfd-2.38-mips.so => /usr/lib/x86_64-linux-gnu/libbfd-2.38-mips.so (0x3)\n"
        "\tlibctf-mips.so.0 => /usr/lib/x86_64-linux-gnu/libctf-mips.so.0 (0x4)\n"
        "\tlibz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x5)\n"
        "\tlibc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x6)\n"
        "\tlibm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x7)\n"
        "\tlibpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x8)\n"
        "\t/lib64/ld-linux-x86-64.so.2 (0x9)\n"
    )
    monkeypatch.setattr(bt.subprocess, "run",
                        lambda *a, **k: subprocess.CompletedProcess(a, 0, ldd, ""))

    names = sorted(p.name for p in bt._ldd_libs(tmp_path / "objdump"))

    assert names == ["libbfd-2.38-mips.so", "libctf-mips.so.0",
                     "libopcodes-2.38-mips.so", "libz.so.1"]


def test_scoring_falls_back_to_system_objdump_when_bundled_aborts(
        monkeypatch, tmp_path):
    import tools.conveyor.jobs.scoring as scoring

    toolkit = tmp_path / "tk"
    (toolkit / "bin").mkdir(parents=True)
    (toolkit / "lib").mkdir()
    bundled = toolkit / "bin" / "objdump"
    bundled.write_text("#!/bin/sh\nexit 134\n")          # SIGABRT-shaped
    bundled.chmod(0o755)
    system = tmp_path / "mips-linux-gnu-objdump"
    system.write_text("#!/bin/sh\necho 'GNU objdump 2.45'\n")
    system.chmod(0o755)
    monkeypatch.setenv("CONVEYOR_TOOLKIT", str(toolkit))
    monkeypatch.delenv("LD_LIBRARY_PATH", raising=False)
    monkeypatch.setenv("PATH", str(tmp_path))
    monkeypatch.setattr(scoring, "_objdump_path_cache", {})

    assert scoring._objdump_path() == str(system)
    assert "LD_LIBRARY_PATH" not in os.environ      # restored after fallback
    assert scoring.objdump_command().startswith(str(system) + " ")


def test_scoring_prefers_bundled_objdump_when_it_runs(monkeypatch, tmp_path):
    import tools.conveyor.jobs.scoring as scoring

    toolkit = tmp_path / "tk"
    (toolkit / "bin").mkdir(parents=True)
    (toolkit / "lib").mkdir()
    bundled = toolkit / "bin" / "objdump"
    bundled.write_text("#!/bin/sh\necho 'GNU objdump 2.38'\n")
    bundled.chmod(0o755)
    monkeypatch.setenv("CONVEYOR_TOOLKIT", str(toolkit))
    monkeypatch.delenv("LD_LIBRARY_PATH", raising=False)
    monkeypatch.setattr(scoring, "_objdump_path_cache", {})

    assert scoring._objdump_path() == str(bundled)
    assert os.environ["LD_LIBRARY_PATH"] == str(toolkit / "lib")
