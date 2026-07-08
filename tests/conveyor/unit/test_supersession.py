"""Evidence attribution + supersession (003), per
contracts/evidence-supersession.md. Pi-local: fabricated DB/store state, no
coordinator. The populate-driven purge tests need mips-linux-gnu-as (skipif).
"""
import argparse
import io
import json
import shutil
import subprocess
import tarfile

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator.store import BlobStore
from tools.conveyor.pipeline import corpus as corpusmod
from tools.conveyor.pipeline import matrix as matrixmod
from tools.conveyor.pipeline import targets as T

HAS_AS = shutil.which("mips-linux-gnu-as") is not None
TOOLKIT = "t" * 64
T1, T2 = "1" * 64, "2" * 64


def _args(data, **kw):
    base = dict(data=str(data), coordinator="http://x", token=None)
    base.update(kw)
    return argparse.Namespace(**base)


class StubHttp:
    def __init__(self, *a, **k):
        self.posted, self.blobs = [], 0

    def pinned_toolkit(self):
        return TOOLKIT

    def call(self, method, path, body=None, raw=None):
        if path.endswith("/blobs"):
            self.blobs += 1
            return 201, {"sha256": "f" * 64}
        if path.endswith("/work"):
            self.posted.extend(body)
            return 201, [{"job_id": f"j{i}"} for i in range(len(body))]
        return 200, {}


# --- purge on target-object change (drives populate) ------------------------

def _fake_words(monkeypatch, words_by_name):
    addr2name = {0x80006A00: "foo", 0x80006B00: "bar"}
    inv = [{"name": "foo", "address": 0x80006A00, "category": "", "flags": "",
            "size": len(words_by_name["foo"]) * 4},
           {"name": "bar", "address": 0x80006B00, "category": "", "flags": "",
            "size": len(words_by_name["bar"]) * 4}]
    monkeypatch.setattr(T, "load_work_inventory", lambda work_dir=None: inv)
    monkeypatch.setattr(T, "index_asm_regions", lambda asm_dir=None: {})
    monkeypatch.setattr(T, "function_words",
                        lambda addr, size: words_by_name[addr2name[addr]])


@pytest.mark.skipif(not HAS_AS, reason="mips-linux-gnu-as not available")
def test_purge_deletes_only_changed_target_all_toolkits_never_workunit(
        tmp_path, monkeypatch):
    words = {"foo": ["24020001", "03E00008"], "bar": ["24030002", "03E00008"]}
    _fake_words(monkeypatch, words)
    data = tmp_path / "d"
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")

    s1 = T.populate(conn, store)
    # Both static, no region -> raw_word_static; first build purges 0 rows.
    assert s1["tiers"]["raw_word_static"] == 2
    assert s1["superseded_targets"] == 2 and s1["purged_rows"] == 0

    # Fabricate evidence for foo (two toolkits) and bar (one), plus a work_unit
    # that must survive the purge (audit trail).
    with dbmod.tx(conn):
        for tk in (T1, T2):
            conn.execute(
                "INSERT INTO matrix_entry (target_id, candidate_id, flagset,"
                " toolkit_sha, score) VALUES ('foo','c',?,?,5)", ("-O1", tk))
        conn.execute(
            "INSERT INTO matrix_entry (target_id, candidate_id, flagset,"
            " toolkit_sha, score) VALUES ('bar','c',?,?,5)", ("-O1", T1))
        conn.execute(
            "INSERT INTO work_unit (job_id, job_type, target_id, manifest_sha,"
            " state, created_at, updated_at) VALUES"
            " ('j','compile_score','foo','m','DONE','2026','2026')")

    # foo's bytes change; bar's do not.
    words["foo"] = ["2402AAAA", "03E00008"]
    s2 = T.populate(conn, store)
    assert s2["superseded_targets"] == 1        # only foo changed
    assert s2["purged_rows"] == 2               # both toolkits' foo rows

    surviving = {(r["target_id"], r["toolkit_sha"]) for r in conn.execute(
        "SELECT target_id, toolkit_sha FROM matrix_entry")}
    assert surviving == {("bar", T1)}           # bar untouched, foo gone
    # work_unit is never touched by supersession (FR-006).
    assert conn.execute("SELECT COUNT(*) AS n FROM work_unit").fetchone()["n"] == 1


@pytest.mark.skipif(not HAS_AS, reason="mips-linux-gnu-as not available")
def test_unchanged_rerun_supersedes_nothing(tmp_path, monkeypatch):
    words = {"foo": ["24020001", "03E00008"], "bar": ["24030002", "03E00008"]}
    _fake_words(monkeypatch, words)
    data = tmp_path / "d"
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")
    T.populate(conn, store)
    s2 = T.populate(conn, store)             # identical inputs
    assert s2["superseded_targets"] == 0 and s2["purged_rows"] == 0


# --- attribution echo on ingest ---------------------------------------------

def _compile_result_bundle(store, cells):
    result = {"job_id": "j", "job_type": "compile_score", "exit": "ok",
              "error": None, "payload": {"cells": cells}}
    buf = io.BytesIO()
    with tarfile.open(fileobj=buf, mode="w:gz") as tar:
        data = json.dumps(result).encode()
        info = tarfile.TarInfo("result.json")
        info.size = len(data)
        tar.addfile(info, io.BytesIO(data))
    return store.put_bytes(buf.getvalue())


def test_ingest_stores_echoed_sha_and_null_for_legacy(tmp_path):
    data = tmp_path / "d"
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")
    dbmod.set_meta(conn, "toolkit_sha", TOOLKIT)

    cells = [
        {"candidate_id": "c1", "flagset": "-O1", "target_id": "foo",
         "score": 0, "score_reloc_blind": 0, "compile": "ok",
         "target_o_sha": "a" * 64},                       # echoed
        {"candidate_id": "c2", "flagset": "-O1", "target_id": "bar",
         "score": 3, "score_reloc_blind": 1, "compile": "ok"},  # legacy, no key
    ]
    result_sha = _compile_result_bundle(store, cells)
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO work_unit (job_id, job_type, target_id, manifest_sha,"
            " toolkit_sha, state, result_sha, created_at, updated_at) VALUES"
            " ('j','compile_score',NULL,'m',?, 'DONE', ?, '2026','2026')",
            (TOOLKIT, result_sha))

    matrixmod.cmd_ingest(_args(data))
    rows = {r["target_id"]: r["target_o_sha"] for r in conn.execute(
        "SELECT target_id, target_o_sha FROM matrix_entry")}
    assert rows == {"foo": "a" * 64, "bar": None}


# --- reschedule: purged target reschedules, unchanged stays cached ----------

def _git(path, *a):
    subprocess.run(["git", "-C", str(path), *a], check=True,
                   capture_output=True, text=True)


def _make_corpus_env(tmp_path, monkeypatch):
    repo = tmp_path / "clone"
    (repo / "src").mkdir(parents=True)
    _git(repo, "init", "-q")
    _git(repo, "config", "user.email", "t@t.test")
    _git(repo, "config", "user.name", "t")
    (repo / "src" / "os.c").write_text(
        "int foo(int x) { return x + 1; }\n"
        "int bar(void) { return 0; }\n")
    _git(repo, "add", "-A")
    _git(repo, "commit", "-q", "-m", "init")

    data = tmp_path / "d"
    corpusmod.cmd_register(_args(data, origin="ultralib", path=str(repo),
                                 repo_url="u", include_dirs="include"))
    corpusmod.cmd_ingest(_args(data, origin=None, allow_dirty=False))
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")
    o_sha = store.put_bytes(b"\x7fELFfoo-target")
    with dbmod.tx(conn):
        for tid in ("foo", "bar"):
            conn.execute("INSERT INTO n64_target (target_id, address, population,"
                         " insn_count, target_o_sha) VALUES (?,1,'static',3,?)",
                         (tid, o_sha))
    conn.commit()
    stub = StubHttp()
    monkeypatch.setattr(corpusmod, "Http", lambda *a, **k: stub)
    monkeypatch.setattr(corpusmod, "load_token", lambda *a, **k: "x")
    return data, conn


def test_purged_target_reschedules_only_its_pairings(tmp_path, monkeypatch, capsys):
    data, conn = _make_corpus_env(tmp_path, monkeypatch)
    # Both foo and bar fully scored under the pinned toolkit.
    with dbmod.tx(conn):
        for tid, cid in (("foo", "ultralib:src/os.c:foo"),
                         ("bar", "ultralib:src/os.c:bar")):
            conn.execute(
                "INSERT INTO matrix_entry (target_id, candidate_id, flagset,"
                " toolkit_sha, score) VALUES (?,?,?,?,0)", (tid, cid, "-O1", TOOLKIT))
    corpusmod.cmd_submit(_args(data, origin=None, flagsets=["-O1"], dry_run=True))
    assert "dry run: 0 jobs (0 cells)" in capsys.readouterr().out  # all cached

    # Supersede foo (as populate's purge would): its rows vanish.
    with dbmod.tx(conn):
        conn.execute("DELETE FROM matrix_entry WHERE target_id='foo'")
    corpusmod.cmd_submit(_args(data, origin=None, flagsets=["-O1"], dry_run=True))
    out = capsys.readouterr().out
    # foo reschedules (1 cell); bar still cached.
    assert "dry run: 1 jobs (1 cells)" in out


# --- flag interplay: clears after purge, upgrades on true-0 re-ingest --------

def _corpus_root_and_target(conn, tid, o_sha="o"):
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO corpus_root (origin, path, repo_url, commit_sha,"
            " include_dirs) VALUES ('ultralib','p','u','deadbeef','[]')")
        conn.execute("INSERT INTO arcade_candidate (candidate_id, name, src_file,"
                     " body_sha, origin) VALUES (?,?,?,?,'ultralib')",
                     (f"ultralib:os.c:{tid}", tid, "os.c", "b"))
        conn.execute("INSERT INTO n64_target (target_id, address, population,"
                     " insn_count, target_o_sha) VALUES (?,1,'static',12,?)",
                     (tid, o_sha))
        conn.execute("INSERT INTO function_status (target_id, status, updated_at)"
                     " VALUES (?, 'unmatched', strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
                     (tid,))


def test_reloc_flag_clears_after_purge_then_upgrades_on_true0(tmp_path):
    data = tmp_path / "d"
    conn = dbmod.connect(data / "conveyor.db")
    dbmod.set_meta(conn, "toolkit_sha", TOOLKIT)
    _corpus_root_and_target(conn, "foo")
    work = tmp_path / "work"

    # reloc_only_diff evidence (true>0, reloc_blind=0) -> flag set.
    with dbmod.tx(conn):
        conn.execute("INSERT INTO matrix_entry (target_id, candidate_id, flagset,"
                     " toolkit_sha, score, score_reloc_blind, target_o_sha)"
                     " VALUES ('foo','ultralib:os.c:foo','-O1',?,20,0,'o')", (TOOLKIT,))
    flagged, _, _ = corpusmod._recompute_reloc_flags(conn, TOOLKIT, work_root=work)
    assert flagged == 1
    assert conn.execute("SELECT human_flag FROM function_status WHERE target_id='foo'"
                        ).fetchone()["human_flag"] == "reloc_only_diff"

    # Supersession purge removes foo's evidence -> flag must clear (no evidence).
    with dbmod.tx(conn):
        conn.execute("DELETE FROM matrix_entry WHERE target_id='foo'")
    flagged, _, _ = corpusmod._recompute_reloc_flags(conn, TOOLKIT, work_root=work)
    assert flagged == 0
    assert conn.execute("SELECT human_flag FROM function_status WHERE target_id='foo'"
                        ).fetchone()["human_flag"] is None

    # Re-ingest a true-0 cell (against the new reloc-aware object) -> normal
    # candidate_identified path, no reloc_only_diff.
    with dbmod.tx(conn):
        conn.execute("INSERT INTO matrix_entry (target_id, candidate_id, flagset,"
                     " toolkit_sha, score, score_reloc_blind, target_o_sha)"
                     " VALUES ('foo','ultralib:os.c:foo','-O1',?,0,0,'o')", (TOOLKIT,))
    matrixmod.update_rankings(conn, TOOLKIT)
    flagged, _, _ = corpusmod._recompute_reloc_flags(conn, TOOLKIT, work_root=work)
    assert flagged == 0
    row = conn.execute("SELECT status, human_flag FROM function_status"
                       " WHERE target_id='foo'").fetchone()
    assert row["status"] == "candidate_identified" and row["human_flag"] is None


# --- SC-006 attribution report line -----------------------------------------

def test_report_attribution_line_counts_mismatch_and_legacy(tmp_path, capsys):
    data = tmp_path / "d"
    conn = dbmod.connect(data / "conveyor.db")
    dbmod.set_meta(conn, "toolkit_sha", TOOLKIT)
    _corpus_root_and_target(conn, "foo", o_sha="cur")
    with dbmod.tx(conn):
        # matching, legacy (NULL), and a stale mismatch (should never happen
        # post-purge, but the check must catch it if it does).
        conn.execute("INSERT INTO matrix_entry (target_id, candidate_id, flagset,"
                     " toolkit_sha, score, target_o_sha)"
                     " VALUES ('foo','ultralib:os.c:foo','-O1',?,5,'cur')", (TOOLKIT,))
        conn.execute("INSERT INTO matrix_entry (target_id, candidate_id, flagset,"
                     " toolkit_sha, score, target_o_sha)"
                     " VALUES ('foo','ultralib:os.c:foo','-O2',?,5,NULL)", (TOOLKIT,))
        conn.execute("INSERT INTO matrix_entry (target_id, candidate_id, flagset,"
                     " toolkit_sha, score, target_o_sha)"
                     " VALUES ('foo','c2','-O1',?,5,'STALE')", (TOOLKIT,))
    corpusmod.cmd_report(_args(data, target=None))
    out = capsys.readouterr().out
    assert "attribution: 3 cells checked, 1 mismatched (expect 0), legacy=1" in out
