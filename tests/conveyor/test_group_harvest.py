"""group_search harvest, tiers and the farm hook, on a tmp coordinator DB
(nothing under ~/.conveyor) with a fake blob_group.splice."""
import gzip
import io
import json
import tarfile

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator.store import BlobStore
from tools.conveyor.pipeline import blob_group, farm, group_jobs


@pytest.fixture
def env(tmp_path):
    conn = dbmod.connect(tmp_path / "db.sqlite")
    store = BlobStore(tmp_path / "blobs")
    yield conn, store, tmp_path
    conn.close()


def _bundle(store, group, seed_name="group.c"):
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w:gz") as tar:
        data = json.dumps({"group": group, "seed_name": seed_name}).encode()
        info = tarfile.TarInfo("manifest.json")
        info.size = len(data)
        tar.addfile(info, io.BytesIO(data))
    return store.put_bytes(buf.getvalue())


def _target(conn, target, status="in_search"):
    with dbmod.tx(conn):
        conn.execute("INSERT INTO function_status (target_id, status, updated_at)"
                     " VALUES (?, ?, '2026')", (target, status))


def _job(conn, store, job_id, target, group, score, state="DONE", seconds=900,
         source=b"int f(void) { return 1; }\n"):
    sha = store.put_bytes(gzip.compress(source)) if score == 0 else None
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO work_unit (job_id, job_type, target_id, manifest_sha, bundle_sha,"
            " state, best_score, best_source_sha, budget, created_at, updated_at)"
            " VALUES (?, 'group_search', ?, 'm', ?, ?, ?, ?, ?, '2026', '2026')",
            (job_id, target, _bundle(store, group), state, score, sha,
             json.dumps({"wall_seconds": seconds})))


def _groups(tmp_path, group="demo"):
    root = tmp_path / "cloud"
    (root / group).mkdir(parents=True)
    (root / group / "group.c").write_text("int f(void) { return 0; }\n")
    (root / group / "group.json").write_text(json.dumps(
        {"members": [], "context": ["f"], "files": ["group.c"], "keep": ["f"], "flags": ""}))
    return root, tmp_path / "work"


def _status(conn, target):
    return conn.execute("SELECT status, human_flag FROM function_status WHERE target_id=?",
                        (target,)).fetchone()


# --- harvest ----------------------------------------------------------------

def test_harvest_splices_marks_matched_and_cancels_pending(env, monkeypatch):
    conn, store, tmp = env
    root, dest = _groups(tmp)
    _target(conn, "f")
    _job(conn, store, "j0", "f", "demo", 0)
    _job(conn, store, "j1", "f", "demo", None, state="PENDING")
    _job(conn, store, "j2", "f", "demo", None, state="LEASED")
    spliced = []
    monkeypatch.setattr(blob_group, "splice", lambda g, d: spliced.append(g) or {"spliced": [g]})
    out = group_jobs.harvest(conn, store, lock={}, root=root, dest=dest)
    assert out["applied"] == ["f"] and spliced == ["demo"]
    assert _status(conn, "f")["status"] == "matched"
    states = {r["job_id"]: r["state"] for r in conn.execute("SELECT job_id, state FROM work_unit")}
    assert states == {"j0": "DONE", "j1": "CANCELLED", "j2": "LEASED"}
    assert "f" in json.loads((dest / "demo" / "group.json").read_text())["members"]


def test_harvest_is_idempotent_once_locked(env, monkeypatch):
    conn, store, tmp = env
    root, dest = _groups(tmp)
    _target(conn, "f")
    _job(conn, store, "j0", "f", "demo", 0)
    calls = []
    monkeypatch.setattr(blob_group, "splice", lambda g, d: calls.append(g) or {})
    lock = {}
    group_jobs.harvest(conn, store, lock=lock, root=root, dest=dest)
    lock["f"] = {}                       # what the real splice records
    out = group_jobs.harvest(conn, store, lock=lock, root=root, dest=dest)
    assert calls == ["demo"] and out["applied"] == [] and out["skipped"] == 1


def test_harvest_dry_run_changes_nothing(env, monkeypatch):
    conn, store, tmp = env
    root, dest = _groups(tmp)
    _target(conn, "f")
    _job(conn, store, "j0", "f", "demo", 0)
    monkeypatch.setattr(blob_group, "splice", lambda g, d: pytest.fail("spliced"))
    out = group_jobs.harvest(conn, store, dry_run=True, lock={}, root=root, dest=dest)
    assert out["would_apply"] == ["f"] and _status(conn, "f")["status"] == "in_search"
    assert not dest.exists()


def test_refused_splice_restores_the_tree_and_is_not_retried(env, monkeypatch):
    conn, store, tmp = env
    root, dest = _groups(tmp)
    (dest / "demo").mkdir(parents=True)
    original = {"members": ["x"], "context": ["f"], "files": ["group.c"], "keep": ["f"],
                "flags": ""}
    (dest / "demo" / "group.json").write_text(json.dumps(original))
    (dest / "demo" / "group.c").write_text("old\n")
    _target(conn, "f")
    _job(conn, store, "j0", "f", "demo", 0)
    calls = []

    def boom(group, document):
        calls.append(group)
        raise blob_group.GroupError("image gate: bytes differ")
    monkeypatch.setattr(blob_group, "splice", boom)
    out = group_jobs.harvest(conn, store, lock={}, root=root, dest=dest, log=lambda m: None)
    assert out["refused"] == ["f"]
    assert json.loads((dest / "demo" / "group.json").read_text()) == original
    assert (dest / "demo" / "group.c").read_text() == "old\n"
    row = _status(conn, "f")
    assert row["status"] == "in_search" and row["human_flag"].startswith("group_harvest_refused")
    group_jobs.harvest(conn, store, lock={}, root=root, dest=dest, log=lambda m: None)
    assert calls == ["demo"]             # the flag stops the retry


def test_refused_splice_of_a_fresh_group_leaves_no_directory(env, monkeypatch):
    conn, store, tmp = env
    root, dest = _groups(tmp)
    _target(conn, "f")
    _job(conn, store, "j0", "f", "demo", 0)
    monkeypatch.setattr(blob_group, "splice",
                        lambda g, d: (_ for _ in ()).throw(RuntimeError("nope")))
    group_jobs.harvest(conn, store, lock={}, root=root, dest=dest, log=lambda m: None)
    assert not (dest / "demo").exists()


# --- tiers ------------------------------------------------------------------

def _plan(conn, store, cands, lock=None, **kw):
    return group_jobs.plan_tiers(conn, store, cands, lock={} if lock is None else lock, **kw)


def test_near_miss_gets_the_next_tier_only(env):
    conn, store, _ = env
    _job(conn, store, "a", "f", "demo", 300)
    cands = [("demo", "f", 40)]
    assert _plan(conn, store, cands) == [("demo", "f", 2700)]
    _job(conn, store, "b", "f", "demo", 90, seconds=2700)
    assert _plan(conn, store, cands) == [("demo", "f", 7200)]
    _job(conn, store, "c", "f", "demo", 80, seconds=7200)
    assert _plan(conn, store, cands) == []          # all tiers completed


def test_tier_limits(env):
    conn, store, _ = env
    _job(conn, store, "a", "f", "demo", 600)
    assert _plan(conn, store, [("demo", "f", 1)]) == []             # over the threshold
    assert _plan(conn, store, [("demo", "f", 1)], threshold=700) == [("demo", "f", 2700)]
    _job(conn, store, "b", "g", "demo", 200, seconds=2700)
    assert _plan(conn, store, [("demo", "g", 1)]) == []             # 7200 needs <= 100


def test_open_locked_and_solved_targets_get_nothing(env):
    conn, store, _ = env
    for t in ("open", "locked", "zero"):
        _job(conn, store, "d-" + t, t, "demo", 0 if t == "zero" else 50)
    _job(conn, store, "p", "open", "demo", None, state="PENDING")
    cands = [("demo", t, 1) for t in ("open", "locked", "zero")]
    assert _plan(conn, store, cands, lock={"locked": {}}) == []


def test_function_in_two_groups_goes_to_its_lowest_scoring_group(env):
    conn, store, _ = env
    _job(conn, store, "a", "f", "alpha", 400)
    _job(conn, store, "b", "f", "beta", 120)
    cands = [("alpha", "f", 1), ("beta", "f", 1)]
    assert _plan(conn, store, cands) == [("beta", "f", 2700)]


def test_tiers_submits_the_plan_and_dry_run_does_not(env, monkeypatch):
    conn, store, _ = env
    _job(conn, store, "a", "f", "demo", 100)
    sent = []
    monkeypatch.setattr(group_jobs, "submit_jobs",
                        lambda client, tk, picks, root, doc, priority=50: sent.extend(picks))
    monkeypatch.setattr(group_jobs.blob_layout, "load", lambda: {})
    kw = dict(lock={}, cands=[("demo", "f", 1)])
    assert group_jobs.tiers(conn, store, None, "tk", dry_run=True, **kw) == [("demo", "f", 2700)]
    assert sent == []
    group_jobs.tiers(conn, store, None, "tk", **kw)
    assert sent == [("demo", "f", 2700)]


# --- farm hook --------------------------------------------------------------

def _stub_farm_steps(monkeypatch, cycle):
    for name in ("ingest", "flywheel_cycle", "top_up"):
        monkeypatch.setattr(farm, name, lambda *a: {})
    monkeypatch.setattr(farm, "group_search_cycle", cycle)


def test_farm_default_never_touches_group_search(monkeypatch):
    def cycle(*a):
        pytest.fail("group_search ran with the flag off")
    _stub_farm_steps(monkeypatch, cycle)
    monkeypatch.setattr(group_jobs, "harvest", lambda *a, **k: pytest.fail("harvest"))
    assert farm.GROUP_SEARCH is False
    assert farm.run_once(None, None, None, "tk", 8, 60) == {}


def test_farm_flag_on_runs_the_cycle_and_reports_it(monkeypatch):
    _stub_farm_steps(monkeypatch, lambda *a: {"gs_spliced": 1})
    assert farm.run_once(None, None, None, "tk", 8, 60, group_search=True) == {"gs_spliced": 1}


def test_group_search_cycle_summary(monkeypatch):
    monkeypatch.setattr(group_jobs, "harvest",
                        lambda c, s: {"applied": ["a", "b"], "refused": ["c"]})
    monkeypatch.setattr(group_jobs, "tiers", lambda c, s, h, t: [("g", "x", 2700)])
    assert farm.group_search_cycle(None, None, None, "tk") == {
        "gs_spliced": 2, "gs_refused": 1, "gs_tiers_queued": 1}
