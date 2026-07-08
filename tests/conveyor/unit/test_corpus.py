"""Corpus candidates pipeline (`pipeline.corpus`): register / ingest / submit.

Pi-local: builds throwaway git repos in tmp_path and drives the commands with
fabricated DB state. No coordinator (submit's Http is stubbed).
"""
import argparse
import json
import subprocess

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator.store import BlobStore
from tools.conveyor.pipeline import corpus as corpusmod

TOOLKIT = "t" * 64


# --- helpers ----------------------------------------------------------------

def _git(path, *a):
    subprocess.run(["git", "-C", str(path), *a], check=True,
                   capture_output=True, text=True)


def _make_clone(tmp_path, body=None):
    repo = tmp_path / "clone"
    (repo / "src").mkdir(parents=True)
    _git_init(repo)
    (repo / "src" / "os.c").write_text(
        body or "int foo(int x) { return x + 1; }\n"
                "int bar(void) { return 0; }\n"
    )
    _git(repo, "add", "-A")
    _git(repo, "commit", "-q", "-m", "init")
    return repo


def _git_init(repo):
    _git(repo, "init", "-q")
    _git(repo, "config", "user.email", "t@t.test")
    _git(repo, "config", "user.name", "tester")


def _args(data, **kw):
    base = dict(data=str(data), coordinator="http://x", token=None)
    base.update(kw)
    return argparse.Namespace(**base)


class StubHttp:
    def __init__(self, *a, **k):
        self.posted = []
        self.blobs = 0

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


def test_strip_comments_preserves_literals_and_newlines():
    from tools.conveyor.seeds import context as ctx
    src = ('#include "PR/os.h"\n'
           '// a line comment with an apostrophe: don\'t choke\n'
           'int f(void) { /* block */ char *u = "http://x//y"; return 0; }\n')
    out = ctx.strip_comments(src)
    assert "//" not in out.replace('"http://x//y"', "")  # comments gone
    assert '"http://x//y"' in out                        # string literal intact
    assert '#include "PR/os.h"' in out                   # directive line intact
    assert out.count("\n") == src.count("\n")             # newlines preserved


# --- register + ingest (T008) -----------------------------------------------

def test_register_ingest_grammar_provenance_idempotence(tmp_path, capsys):
    repo = _make_clone(tmp_path)
    data = tmp_path / "data"
    head = subprocess.run(["git", "-C", str(repo), "rev-parse", "HEAD"],
                          capture_output=True, text=True).stdout.strip()

    # A pre-existing arcade candidate must survive untouched (FR-003).
    conn = dbmod.connect(data / "conveyor.db")
    with dbmod.tx(conn):
        conn.execute(
            "INSERT INTO arcade_candidate (candidate_id, name, src_file, body_sha)"
            " VALUES ('game/a.c:foo', 'foo', 'game/a.c', 'deadbeef')")
    n_arcade_before = conn.execute(
        "SELECT COUNT(*) AS n FROM arcade_candidate WHERE origin='arcade'"
    ).fetchone()["n"]
    conn.close()

    corpusmod.cmd_register(_args(data, origin="ultralib", path=str(repo),
                                 repo_url="https://example/u",
                                 include_dirs="include"))
    corpusmod.cmd_ingest(_args(data, origin=None, allow_dirty=False))
    out = capsys.readouterr().out
    assert "2 functions from 1 files (2 new, 0 updated)" in out
    assert head[:8] in out

    conn = dbmod.connect(data / "conveyor.db")
    rows = {r["candidate_id"]: r for r in conn.execute(
        "SELECT candidate_id, name, src_file, origin, provenance"
        " FROM arcade_candidate WHERE origin='ultralib'")}
    # id grammar <origin>:<rel>:<name>
    assert set(rows) == {"ultralib:src/os.c:foo", "ultralib:src/os.c:bar"}
    foo = rows["ultralib:src/os.c:foo"]
    assert foo["name"] == "foo" and foo["src_file"] == "src/os.c"
    prov = json.loads(foo["provenance"])
    assert prov == {"repo_url": "https://example/u", "rel": "src/os.c",
                    "commit": head}
    # arcade rows untouched
    assert conn.execute(
        "SELECT COUNT(*) AS n FROM arcade_candidate WHERE origin='arcade'"
    ).fetchone()["n"] == n_arcade_before
    arcade = conn.execute(
        "SELECT provenance, origin FROM arcade_candidate WHERE candidate_id='game/a.c:foo'"
    ).fetchone()
    assert arcade["origin"] == "arcade" and arcade["provenance"] == "{}"
    conn.close()

    # Idempotent re-ingest.
    corpusmod.cmd_ingest(_args(data, origin=None, allow_dirty=False))
    out = capsys.readouterr().out
    assert "(0 new, 0 updated)" in out


def test_ingest_dedupes_duplicate_names_first_wins(tmp_path, capsys):
    # Two #ifdef-style definitions of one function in one file (as several
    # ultralib files carry): keep the first, and stay idempotent.
    body = ("int dup(void) { return 1; }\n"
            "int dup(void) { return 2; }\n"
            "int other(void) { return 0; }\n")
    repo = _make_clone(tmp_path, body=body)
    data = tmp_path / "data"
    corpusmod.cmd_register(_args(data, origin="ultralib", path=str(repo),
                                 repo_url="u", include_dirs="include"))
    corpusmod.cmd_ingest(_args(data, origin=None, allow_dirty=False))
    out = capsys.readouterr().out
    assert "2 functions from 1 files (2 new, 0 updated)" in out  # dup counted once
    corpusmod.cmd_ingest(_args(data, origin=None, allow_dirty=False))
    assert "(0 new, 0 updated)" in capsys.readouterr().out
    conn = dbmod.connect(data / "conveyor.db")
    body_sha = conn.execute(
        "SELECT body_sha FROM arcade_candidate WHERE candidate_id='ultralib:src/os.c:dup'"
    ).fetchone()["body_sha"]
    import hashlib
    assert body_sha == hashlib.sha256(
        b"int dup(void) { return 1; }").hexdigest()  # first body wins
    conn.close()


def test_ingest_refuses_dirty_clone(tmp_path):
    repo = _make_clone(tmp_path)
    data = tmp_path / "data"
    corpusmod.cmd_register(_args(data, origin="ultralib", path=str(repo),
                                 repo_url=None, include_dirs="include"))
    (repo / "DIRTY").write_text("x")  # untracked -> status --porcelain nonempty
    with pytest.raises(SystemExit) as e:
        corpusmod.cmd_ingest(_args(data, origin=None, allow_dirty=False))
    assert e.value.code != 0
    # Nothing recorded.
    conn = dbmod.connect(data / "conveyor.db")
    assert conn.execute(
        "SELECT COUNT(*) AS n FROM arcade_candidate WHERE origin='ultralib'"
    ).fetchone()["n"] == 0
    conn.close()


def test_register_rejects_bad_origin(tmp_path):
    repo = _make_clone(tmp_path)
    data = tmp_path / "data"
    for bad in ("arcade", "a/b", "libc.c"):
        with pytest.raises(SystemExit):
            corpusmod.cmd_register(_args(data, origin=bad, path=str(repo),
                                         repo_url=None, include_dirs=None))


def test_register_rejects_non_clone(tmp_path):
    plain = tmp_path / "plain"
    plain.mkdir()
    with pytest.raises(SystemExit):
        corpusmod.cmd_register(_args(tmp_path / "data", origin="x",
                                     path=str(plain), repo_url=None,
                                     include_dirs=None))


# --- submit (T010) ----------------------------------------------------------

def _setup_corpus_and_targets(tmp_path, monkeypatch):
    repo = _make_clone(tmp_path)
    data = tmp_path / "data"
    corpusmod.cmd_register(_args(data, origin="ultralib", path=str(repo),
                                 repo_url="u", include_dirs="include"))
    corpusmod.cmd_ingest(_args(data, origin=None, allow_dirty=False))
    conn = dbmod.connect(data / "conveyor.db")
    store = BlobStore(data / "blobs")
    o_sha = store.put_bytes(b"\x7fELFfoo-target")
    with dbmod.tx(conn):
        # foo: has a target .o; bar: no .o (skip); baz: no candidate (no pairing)
        conn.execute("INSERT INTO n64_target (target_id, address, population,"
                     " insn_count, target_o_sha) VALUES ('foo', 1, 'static', 3, ?)",
                     (o_sha,))
        conn.execute("INSERT INTO n64_target (target_id, address, population,"
                     " insn_count, target_o_sha) VALUES ('bar', 2, 'static', 3, NULL)")
        conn.execute("INSERT INTO n64_target (target_id, address, population,"
                     " insn_count, target_o_sha) VALUES ('baz', 3, 'static', 3, ?)",
                     (o_sha,))
    conn.commit()
    conn.close()
    stub = StubHttp()
    monkeypatch.setattr(corpusmod, "Http", lambda *a, **k: stub)
    monkeypatch.setattr(corpusmod, "load_token", lambda *a, **k: "x")
    return data, repo, stub


def test_submit_pairs_by_name_skips_and_dry_run_uploads_nothing(
        tmp_path, monkeypatch, capsys):
    data, repo, stub = _setup_corpus_and_targets(tmp_path, monkeypatch)
    corpusmod.cmd_submit(_args(data, origin=None, flagsets=["-O1", "-O2"],
                               dry_run=True))
    out = capsys.readouterr().out
    # foo + bar name-match targets (baz has no candidate) => 2 pairings;
    # bar has no target .o => 1 no_target_o skip; foo × 2 flagsets => 2 cells.
    assert "2 name pairings" in out
    assert "no_target_o=1 unextractable=0" in out
    assert "dry run: 1 jobs (2 cells)" in out
    # Dry run uploads nothing.
    assert stub.blobs == 0 and stub.posted == []


def test_submit_dedupes_against_existing_cells(tmp_path, monkeypatch, capsys):
    data, repo, stub = _setup_corpus_and_targets(tmp_path, monkeypatch)
    conn = dbmod.connect(data / "conveyor.db")
    with dbmod.tx(conn):
        for fs in ("-O1", "-O2"):
            conn.execute(
                "INSERT INTO matrix_entry (target_id, candidate_id, flagset,"
                " toolkit_sha, score) VALUES ('foo', 'ultralib:src/os.c:foo', ?, ?, 5)",
                (fs, TOOLKIT))
    conn.commit()
    conn.close()
    corpusmod.cmd_submit(_args(data, origin=None, flagsets=["-O1", "-O2"],
                               dry_run=False))
    out = capsys.readouterr().out
    # foo fully scored already => no cells => 0 jobs submitted; bar still skips.
    assert "2 name pairings; 0 jobs submitted (0 cells)" in out
    assert stub.posted == []


def test_submit_counts_unextractable(tmp_path, monkeypatch, capsys):
    data, repo, stub = _setup_corpus_and_targets(tmp_path, monkeypatch)
    # Remove foo's body from the source so it can no longer be extracted.
    (repo / "src" / "os.c").write_text("int bar(void) { return 0; }\n")
    corpusmod.cmd_submit(_args(data, origin=None, flagsets=["-O1"],
                               dry_run=True))
    out = capsys.readouterr().out
    assert "unextractable=1" in out


# --- reloc_only_diff flags + artifacts (T015) -------------------------------

def _corpus_env(tmp_path):
    """A registered+ingested corpus and n64_target/function_status rows for
    foo (unmatched) and bar (unmatched)."""
    repo = _make_clone(tmp_path)
    data = tmp_path / "data"
    corpusmod.cmd_register(_args(data, origin="ultralib", path=str(repo),
                                 repo_url="https://example/u", include_dirs="include"))
    corpusmod.cmd_ingest(_args(data, origin=None, allow_dirty=False))
    conn = dbmod.connect(data / "conveyor.db")
    dbmod.set_meta(conn, "toolkit_sha", TOOLKIT)
    for tid, insns in (("foo", 12), ("bar", 12)):
        with dbmod.tx(conn):
            conn.execute("INSERT INTO n64_target (target_id, address, population,"
                         " insn_count, target_o_sha) VALUES (?,1,'static',?,'o')",
                         (tid, insns))
            conn.execute("INSERT INTO function_status (target_id, status, updated_at)"
                         " VALUES (?, 'unmatched', strftime('%Y-%m-%dT%H:%M:%fZ','now'))",
                         (tid,))
    return data, repo, conn


def _cell(conn, target, cand, flagset, score, rb):
    with dbmod.tx(conn):
        conn.execute(
            "INSERT OR REPLACE INTO matrix_entry (target_id, candidate_id, flagset,"
            " toolkit_sha, score, score_reloc_blind) VALUES (?,?,?,?,?,?)",
            (target, cand, flagset, TOOLKIT, score, rb))


def test_reloc_flag_set_and_artifact_written(tmp_path):
    data, repo, conn = _corpus_env(tmp_path)
    _cell(conn, "foo", "ultralib:src/os.c:foo", "-g0 -O1 -mips2 -G 0 -non_shared", 20, 0)
    work = tmp_path / "work"
    flagged, newly, artifacts = corpusmod._recompute_reloc_flags(
        conn, TOOLKIT, work_root=work)
    assert (flagged, newly, artifacts) == (1, 1, 1)
    assert conn.execute("SELECT human_flag FROM function_status WHERE target_id='foo'"
                        ).fetchone()["human_flag"] == "reloc_only_diff"
    art = (work / "corpus" / "foo" / "corpus_match.c").read_text()
    assert "CORPUS MATCH (reloc_only_diff) — NOT byte-verified" in art
    assert "Origin:      ultralib (https://example/u)" in art
    assert "Source:      src/os.c @" in art
    assert "Scores:      true=20  reloc_blind=0" in art
    assert "int foo(int x)" in art  # candidate body appended


def test_true_zero_is_not_flagged(tmp_path):
    data, repo, conn = _corpus_env(tmp_path)
    _cell(conn, "foo", "ultralib:src/os.c:foo", "-O1", 0, 0)  # true 0 => promotion path
    flagged, newly, artifacts = corpusmod._recompute_reloc_flags(
        conn, TOOLKIT, work_root=tmp_path / "work")
    assert flagged == 0
    assert conn.execute("SELECT human_flag FROM function_status WHERE target_id='foo'"
                        ).fetchone()["human_flag"] != "reloc_only_diff"


def test_flag_clears_when_evidence_changes(tmp_path):
    data, repo, conn = _corpus_env(tmp_path)
    _cell(conn, "foo", "ultralib:src/os.c:foo", "-O1", 20, 0)
    corpusmod._recompute_reloc_flags(conn, TOOLKIT, work_root=tmp_path / "work")
    # New evidence: reloc_blind now nonzero -> flag must clear (FR-011).
    _cell(conn, "foo", "ultralib:src/os.c:foo", "-O1", 20, 7)
    flagged, _, _ = corpusmod._recompute_reloc_flags(
        conn, TOOLKIT, work_root=tmp_path / "work")
    assert flagged == 0
    assert conn.execute("SELECT human_flag FROM function_status WHERE target_id='foo'"
                        ).fetchone()["human_flag"] is None


def test_overridden_and_matched_targets_never_flagged(tmp_path):
    data, repo, conn = _corpus_env(tmp_path)
    with dbmod.tx(conn):
        conn.execute("UPDATE function_status SET override='manual' WHERE target_id='foo'")
        conn.execute("UPDATE function_status SET status='matched' WHERE target_id='bar'")
    _cell(conn, "foo", "ultralib:src/os.c:foo", "-O1", 20, 0)
    _cell(conn, "bar", "ultralib:src/os.c:bar", "-O1", 20, 0)
    flagged, _, _ = corpusmod._recompute_reloc_flags(
        conn, TOOLKIT, work_root=tmp_path / "work")
    assert flagged == 0
    rows = {r["target_id"]: r["human_flag"] for r in conn.execute(
        "SELECT target_id, human_flag FROM function_status")}
    assert rows["foo"] != "reloc_only_diff" and rows["bar"] != "reloc_only_diff"


def test_artifact_regenerates_on_reingest(tmp_path):
    data, repo, conn = _corpus_env(tmp_path)
    _cell(conn, "foo", "ultralib:src/os.c:foo", "-O1", 20, 0)
    work = tmp_path / "work"
    corpusmod._recompute_reloc_flags(conn, TOOLKIT, work_root=work)
    art = work / "corpus" / "foo" / "corpus_match.c"
    art.write_text("STALE HAND EDIT")
    corpusmod._recompute_reloc_flags(conn, TOOLKIT, work_root=work)
    assert "STALE HAND EDIT" not in art.read_text()
    assert "CORPUS MATCH" in art.read_text()
