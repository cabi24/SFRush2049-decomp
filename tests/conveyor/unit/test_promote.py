"""Promotion transaction (`pipeline.promote`, 004), per
contracts/promotion-transaction.md. Pi-local: a throwaway git repo + fixture
layout map; the real transaction runs (splice, real git checkout/commit, lock
migration, promotion_record) with only the matching build+SHA-1 gate mocked
(the real gate is the quickstart's job). No coordinator, no builder.
"""
import copy
import json
import subprocess

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.pipeline import promote as P

FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"

# One converted segment (rom/lib_8800) with two passthrough slots.
BASE_MAP = {"segments": [
    {"yaml_name": "0x8800", "rom_tu": "rom/lib_8800", "vram_start": "0x80007c00",
     "size": 96, "converted": True, "flagset": FLAGS, "refusal": None,
     "functions": [
         {"name": "strlen", "vaddr": "0x80007c40", "size": 40, "state": "passthrough"},
         {"name": "memcpy", "vaddr": "0x80007c68", "size": 56, "state": "passthrough"}]},
    {"yaml_name": "0x8700", "rom_tu": "rom/lib_8700", "vram_start": "0x80007b00",
     "size": 16, "converted": False, "flagset": None, "refusal": None,
     "functions": [{"name": "bzero", "vaddr": "0x80007b00", "size": 16,
                    "state": "passthrough"}]},
]}

BODIES = {
    "strlen": "int strlen(const char *s) { return 0; }",
    "memcpy": "void *memcpy(void *a, const void *b, int n) { return a; }",
}

TU_TEXT = (
    '#include "rom_tu.h"\n'
    '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8800/strlen.s")\n'
    '#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8800/memcpy.s")\n'
)


def _git(repo, *a):
    subprocess.run(["git", "-C", str(repo), *a], check=True,
                   capture_output=True, text=True)


@pytest.fixture
def repo(tmp_path, monkeypatch):
    r = tmp_path / "repo"
    (r / "src" / "rom").mkdir(parents=True)
    (r / "src" / "libc").mkdir(parents=True)
    _git(r, "init", "-q")
    _git(r, "config", "user.email", "t@t.test")
    _git(r, "config", "user.name", "t")
    (r / "src" / "rom" / "lib_8800.c").write_text(TU_TEXT)
    (r / "src" / "libc" / "string.c").write_text(
        BODIES["strlen"] + "\n" + BODIES["memcpy"] + "\n")
    lf = r / "matched.lock.json"
    lf.write_text(json.dumps({
        "src/libc/string.c:strlen": {
            "target_id": "strlen", "flagset": FLAGS, "verified": "score0",
            "body_sha256": "x", "toolkit_sha": None, "verified_at": "2026-07-01"},
    }, indent=2))
    _git(r, "add", "-A")
    _git(r, "commit", "-q", "-m", "init")
    data = tmp_path / "data"; data.mkdir()  # DB lives OUTSIDE the repo

    monkeypatch.setattr(P, "REPO", r)
    # _run defaults cwd to the real repo (import-time bound); force the fixture.
    orig_run = P._run
    monkeypatch.setattr(P, "_run", lambda cmd, cwd=None, timeout=1200:
                        orig_run(cmd, cwd=cwd or r, timeout=timeout))
    monkeypatch.setattr(P.layoutmod, "derive", lambda: copy.deepcopy(BASE_MAP))
    monkeypatch.setattr(P, "extract_named_function",
                        lambda path, func: BODIES[func])
    monkeypatch.setattr(P.lockmod, "load_lock",
                        lambda path=lf: json.loads(lf.read_text()) if lf.exists() else {})
    monkeypatch.setattr(P.lockmod, "save_lock",
                        lambda entries, path=lf: lf.write_text(
                            json.dumps(entries, indent=2, sort_keys=True) + "\n"))
    # Gate mocked; each test sets the outcome it wants.
    return r, monkeypatch, lf, data


def _mock_gate(monkeypatch, ok, detail="ROM matches!"):
    monkeypatch.setattr(P, "_build_and_gate",
                        lambda via_builder, tu_rel: (ok, detail))


def _outcomes(data_dir):
    conn = dbmod.connect(data_dir / "conveyor.db")
    return [dict(r) for r in conn.execute(
        "SELECT target_id, outcome, source, flags, rom_tu FROM promotion_record"
        " ORDER BY created_at")]


# --- precondition refusals --------------------------------------------------

def test_refuse_unknown_segment(repo):
    r, mp, _, data = repo
    with pytest.raises(P.Refusal, match="no segment"):
        P.run_promotion("0xBEEF:strlen", "src/libc/string.c", data=data)


def test_refuse_unconverted_segment(repo):
    r, mp, _, data = repo
    with pytest.raises(P.Refusal, match="not converted"):
        P.run_promotion("0x8700:bzero", "src/libc/mem.c", data=data)


def test_refuse_function_not_in_segment(repo):
    r, mp, _, data = repo
    with pytest.raises(P.Refusal, match="not in segment"):
        P.run_promotion("0x8800:nope", "src/libc/string.c", data=data)


def test_refuse_no_evidence(repo):
    r, mp, _, data = repo
    # memcpy has no lock entry and no override -> refuse with the remedy.
    with pytest.raises(P.Refusal, match="no verification evidence"):
        P.run_promotion("0x8800:memcpy", "src/libc/string.c", data=data)


def test_refuse_no_pinned_flagset(repo):
    r, mp, _, data = repo
    m = copy.deepcopy(BASE_MAP)
    m["segments"][0]["flagset"] = None
    mp.setattr(P.layoutmod, "derive", lambda: copy.deepcopy(m))
    with pytest.raises(P.Refusal, match="no pinned flagset"):
        P.run_promotion("0x8800:strlen", "src/libc/string.c", data=data)


def test_refuse_dirty_tree(repo):
    r, mp, _, data = repo
    (r / "src" / "rom" / "lib_8800.c").write_text(TU_TEXT + "// dirty\n")
    with pytest.raises(P.Refusal, match="working tree dirty"):
        P.run_promotion("0x8800:strlen", "src/libc/string.c", data=data)


# --- splice / pass / lock migration -----------------------------------------

def test_pass_splices_migrates_lock_and_records(repo):
    r, mp, lf, data = repo
    _mock_gate(mp, True)
    sha = P.run_promotion("0x8800:strlen", "src/libc/string.c", data=data)
    assert sha  # a commit hash

    tu = (r / "src" / "rom" / "lib_8800.c").read_text()
    assert "PROMOTED" in tu and BODIES["strlen"] in tu
    assert 'nonmatchings/rom/lib_8800/strlen.s' not in tu   # pragma replaced
    assert 'nonmatchings/rom/lib_8800/memcpy.s' in tu       # neighbor untouched

    lock = json.loads(lf.read_text())
    assert "src/rom/lib_8800.c:strlen" in lock               # migrated to ROM TU
    assert lock["src/rom/lib_8800.c:strlen"]["verified"] == "rom-sha1"
    assert "src/libc/string.c:strlen" not in lock            # ref entry removed

    recs = _outcomes(data)
    assert len(recs) == 1 and recs[0]["outcome"] == "promoted"
    assert recs[0]["rom_tu"] == "rom/lib_8800" and recs[0]["flags"] == FLAGS
    # tree is clean after a committed promotion
    assert subprocess.run(["git", "-C", str(r), "status", "--porcelain"],
                          capture_output=True, text=True).stdout.strip() == ""


# --- gate failure / rollback ------------------------------------------------

def test_gate_failure_rolls_back_byte_identical(repo):
    r, mp, lf, data = repo
    before_tu = (r / "src" / "rom" / "lib_8800.c").read_text()
    before_lock = lf.read_text()
    _mock_gate(mp, False, "hash mismatch aa!=bb")

    with pytest.raises(P.Refusal, match="GATE FAILED"):
        P.run_promotion("0x8800:strlen", "src/libc/string.c", data=data)

    # TU restored byte-for-byte; lock untouched.
    assert (r / "src" / "rom" / "lib_8800.c").read_text() == before_tu
    assert lf.read_text() == before_lock
    assert subprocess.run(["git", "-C", str(r), "status", "--porcelain"],
                          capture_output=True, text=True).stdout.strip() == ""
    recs = _outcomes(data)
    assert len(recs) == 1 and recs[0]["outcome"] == "failed"
    assert "hash mismatch" in json.loads(_record_evidence(data))["refusal"]


def _record_evidence(data_dir):
    conn = dbmod.connect(data_dir / "conveyor.db")
    return conn.execute(
        "SELECT evidence FROM promotion_record ORDER BY created_at DESC LIMIT 1"
    ).fetchone()[0]


# --- already promoted -------------------------------------------------------

def test_already_promoted_refuses(repo):
    r, mp, lf, data = repo
    _mock_gate(mp, True)
    P.run_promotion("0x8800:strlen", "src/libc/string.c", data=data)  # first: ok
    # Re-run with --override (evidence now lives at the ROM path); the splice
    # finds no passthrough slot and refuses.
    with pytest.raises(P.Refusal, match="no passthrough slot|already promoted"):
        P.run_promotion("0x8800:strlen", "src/libc/string.c", data=r,
                        override_reason="retry")


# --- override path ----------------------------------------------------------

def test_override_supplies_evidence(repo):
    r, mp, lf, data = repo
    _mock_gate(mp, True)
    sha = P.run_promotion("0x8800:memcpy", "src/libc/string.c", data=data,
                          override_reason="hand-checked identical")
    assert sha
    recs = _outcomes(data)
    assert recs[0]["outcome"] == "promoted"
    ev = json.loads(_record_evidence(data))["evidence"]
    assert "override" in ev and "hand-checked" in ev


# --- one implementation (CLI dispatches to the library) ---------------------

def test_cli_run_calls_library(repo, monkeypatch):
    r, mp, _, data = repo
    called = {}
    monkeypatch.setattr(P, "run_promotion",
                        lambda *a, **k: called.setdefault("hit", (a, k)))
    ns = type("NS", (), {"spec": "0x8800:strlen", "source": "src/libc/string.c",
                         "via_builder": False, "override": False, "reason": None,
                         "data": str(data)})()
    P.cmd_run(ns)
    assert called["hit"][0][0] == "0x8800:strlen"  # same transaction entrypoint
