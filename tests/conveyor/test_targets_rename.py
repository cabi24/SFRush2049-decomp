"""targets_rename: re-register static inventory rows under current layout names."""
import json

import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.coordinator.store import BlobStore
from tools.conveyor.pipeline import layout as layoutmod
from tools.conveyor.pipeline import targets as targetsmod
from tools.conveyor.pipeline import targets_rename as tr

A, B, C, FRAG = 0x8000FE00, 0x8000FF60, 0x80010110, 0x8000FE40


def _mapping():
    return {"segments": [
        {"yaml_name": "0x10a00", "refusal": None, "functions": [
            {"name": "__osEPiRawWriteIo", "vaddr": hex(A), "size": 352}]},
        {"yaml_name": "0x10b60", "refusal": None, "functions": [
            {"name": "__osEPiRawReadIo", "vaddr": hex(B), "size": 352}]},
        {"yaml_name": "0x10d10", "refusal": None, "functions": [
            {"name": "osYieldThread", "vaddr": hex(C), "size": 80}]},
        {"yaml_name": "dynamic", "refusal": "dynamic-population", "functions": []},
    ]}


def _regions():
    return {
        A: targetsmod.Region("osEPiRawStartDma", A, [], ["27BDFFE8"] * 86),
        B: targetsmod.Region("osEPiRawReadIo", B, [], ["27BDFFE0"] * 87),
        C: targetsmod.Region("osYieldThread", C, [], ["03E00008"] * 19),
    }


def _words_of(address, nbytes):
    return list(_regions()[address].words[: nbytes // 4])


def _builder(region, name, out_o):
    out_o.write_bytes(("obj:" + name + ":" + "".join(region.words)).encode())
    return out_o


def _insert(conn, tid, addr, n, o_sha, population="static"):
    conn.execute("INSERT INTO n64_target (target_id, address, population, insn_count,"
                 " target_o_sha, tier) VALUES (?,?,?,?,?, 'reloc_aware')",
                 (tid, addr, population, n, o_sha))
    conn.execute("INSERT INTO function_status (target_id, status, best_score, updated_at)"
                 " VALUES (?, 'unmatched', 99, 'x')", (tid,))


def _matrix(conn, tid, n):
    for i in range(n):
        conn.execute("INSERT INTO matrix_entry (target_id, candidate_id, flagset, toolkit_sha,"
                     " score) VALUES (?,?,?,?,?)", (tid, f"c{i}", "-O2", "tk", 5))


@pytest.fixture
def conn(tmp_path):
    c = dbmod.connect(tmp_path / "conveyor.db")
    _insert(c, "osEPiRawStartDma", A, 88, "old_a")            # stale name at A
    _insert(c, "__osEPiRawReadIo", FRAG, 29, "frag")          # current name, wrong address
    _insert(c, "osYieldThread", C, 4072, "keep_c")            # stale insn_count
    _insert(c, "read_controllers", A + 0x40, 117742, "huge")  # mid-function label
    _insert(c, "unrelated", 0x80020000, 7, "u")
    _matrix(c, "osEPiRawStartDma", 2)
    _matrix(c, "__osEPiRawReadIo", 3)
    _matrix(c, "unrelated", 4)
    c.commit()
    return c


def _snapshot(conn, exclude):
    rows = {}
    for t in ("n64_target", "function_status", "matrix_entry"):
        rows[t] = sorted(tuple(r) for r in conn.execute(f"SELECT * FROM {t}")
                         if r["target_id"] not in exclude)
    return rows


def _plan(conn, names, retire=(), locks=None):
    return tr.plan(conn, tr.layout_functions(_mapping()), names, retire, locks or {})


def test_plan_renames_readdresses_refreshes_and_retires(conn):
    actions = _plan(conn, ["__osEPiRawWriteIo", "__osEPiRawReadIo", "osYieldThread"],
                    ["read_controllers"])
    ops = [(a["op"], a["target_id"]) for a in actions]
    assert ops == [("register", "__osEPiRawWriteIo"), ("readdress", "__osEPiRawReadIo"),
                   ("refresh", "osYieldThread"), ("retire", "osEPiRawStartDma"),
                   ("retire", "read_controllers")]
    assert actions[1]["address"] == B and actions[0]["size"] == 352


def test_apply_writes_only_planned_rows(conn, tmp_path):
    touched = {"__osEPiRawWriteIo", "__osEPiRawReadIo", "osYieldThread",
               "osEPiRawStartDma", "read_controllers"}
    before = _snapshot(conn, touched)
    actions = _plan(conn, ["__osEPiRawWriteIo", "__osEPiRawReadIo", "osYieldThread"],
                    ["read_controllers"])
    tr.build_objects(actions, _regions(), BlobStore(tmp_path / "blobs"),
                     builder=_builder, words_of=_words_of)
    purged = tr.apply(conn, actions)
    assert _snapshot(conn, touched) == before            # every other row preserved
    assert purged == 2 + 3                                # retired + re-addressed evidence
    get = lambda t: conn.execute("SELECT * FROM n64_target WHERE target_id=?", (t,)).fetchone()
    assert get("osEPiRawStartDma") is None and get("read_controllers") is None
    assert conn.execute("SELECT COUNT(*) FROM function_status WHERE target_id IN"
                        " ('osEPiRawStartDma','read_controllers')").fetchone()[0] == 0
    new = get("__osEPiRawWriteIo")
    assert (new["address"], new["insn_count"], new["tier"]) == (A, 86, "reloc_aware")
    assert (tmp_path / "blobs" / new["target_o_sha"]).is_file()
    moved = get("__osEPiRawReadIo")
    assert (moved["address"], moved["insn_count"]) == (B, 87)
    assert get("osYieldThread")["insn_count"] == 19
    assert conn.execute("SELECT status FROM function_status WHERE target_id='__osEPiRawWriteIo'"
                        ).fetchone()[0] == "unmatched"


def test_refuses_when_a_lock_targets_a_retired_row(conn):
    with pytest.raises(tr.RenameRefusal, match="referenced by lock"):
        _plan(conn, ["__osEPiRawWriteIo"],
              locks={"osEPiRawStartDma": ["src/rom/lib_10a00.c:__osEPiRawWriteIo"]})
    with pytest.raises(tr.RenameRefusal, match="referenced by lock"):
        _plan(conn, [], ["read_controllers"], locks={"read_controllers": ["x.c:read_controllers"]})


def test_lock_targets_reads_explicit_and_implicit_target_ids(tmp_path):
    lock = tmp_path / "matched.lock.json"
    lock.write_text(json.dumps({"a.c:f": {"target_id": "old_f"}, "b.c:g": {}}))
    assert tr.lock_targets(lock) == {"old_f": ["a.c:f"], "g": ["b.c:g"]}


def test_refuses_retiring_a_layout_function_start(conn):
    with pytest.raises(tr.RenameRefusal, match="starts layout function"):
        _plan(conn, [], ["osEPiRawStartDma"])


def test_refuses_retiring_a_current_layout_name(conn):
    conn.execute("UPDATE n64_target SET address=? WHERE target_id='osYieldThread'", (A,))
    with pytest.raises(tr.RenameRefusal, match="current layout function name"):
        _plan(conn, ["__osEPiRawWriteIo"])


def test_refuses_unknown_or_extracted_rows(conn):
    with pytest.raises(tr.RenameRefusal, match="not a function in the derived layout"):
        _plan(conn, ["nope"])
    with pytest.raises(tr.RenameRefusal, match="no such inventory row"):
        _plan(conn, [], ["nope"])
    _insert(conn, "game_fn", A, 3, "g", population="extracted")
    with pytest.raises(tr.RenameRefusal, match="only static rows"):
        _plan(conn, ["__osEPiRawWriteIo"])


def test_refuses_a_promoted_or_queued_row(conn):
    conn.execute("INSERT INTO promotion_record (promotion_id, target_id, source_sha, outcome,"
                 " created_at) VALUES ('p', 'osEPiRawStartDma', 's', 'promoted', 'x')")
    with pytest.raises(tr.RenameRefusal, match="promotion_record"):
        _plan(conn, ["__osEPiRawWriteIo"])
    conn.execute("INSERT INTO work_unit (job_id, job_type, target_id, manifest_sha, state,"
                 " created_at, updated_at) VALUES ('j','search','read_controllers','m','PENDING','x','x')")
    with pytest.raises(tr.RenameRefusal, match="open work units"):
        _plan(conn, [], ["read_controllers"])


def test_gate_failure_refuses_without_writing(conn, tmp_path):
    def failing(region, name, out_o):
        raise tr.RenameRefusal(f"{name}: round-trip gate failed: word_mismatch@0")

    actions = _plan(conn, ["__osEPiRawWriteIo"])
    before = _snapshot(conn, ())
    with pytest.raises(tr.RenameRefusal, match="gate failed"):
        tr.build_objects(actions, _regions(), BlobStore(tmp_path / "b"), builder=failing,
                         words_of=_words_of)
    with pytest.raises(tr.RenameRefusal, match="no asm region"):
        tr.build_objects(actions, {}, BlobStore(tmp_path / "b"), builder=_builder,
                         words_of=_words_of)
    assert _snapshot(conn, ()) == before


def test_audit_flags_names_extents_and_fragments(conn):
    problems = {t: p for t, _a, p in tr.audit(conn, tr.layout_functions(_mapping()), _regions())}
    assert problems["osEPiRawStartDma"] == "layout name is __osEPiRawWriteIo"
    assert problems["osYieldThread"].startswith("insn_count 4072")
    assert "inside __osEPiRawWriteIo" in problems["read_controllers"]
    assert "unrelated" in problems  # no layout function at its address


def _cli(monkeypatch, tmp_path, argv):
    monkeypatch.setattr(layoutmod, "derive", _mapping)
    monkeypatch.setattr(layoutmod, "collect_regions", _regions)
    monkeypatch.setattr(tr, "build_object", _builder)
    monkeypatch.setattr(targetsmod, "function_words", _words_of)
    lock = tmp_path / "lock.json"
    lock.write_text("{}")
    tr.main(["--data", str(tmp_path), "--lockfile", str(lock)] + argv)


def test_dry_run_prints_the_plan_and_writes_nothing(conn, tmp_path, monkeypatch, capsys):
    before = _snapshot(conn, ())
    conn.close()
    _cli(monkeypatch, tmp_path, ["apply", "__osEPiRawWriteIo", "--retire", "read_controllers",
                                 "--dry-run"])
    out = capsys.readouterr().out
    assert "register  __osEPiRawWriteIo" in out and "retire    osEPiRawStartDma" in out
    assert "retire    read_controllers" in out and "dry run: nothing written" in out
    assert not (tmp_path / "blobs").exists() or not any((tmp_path / "blobs").iterdir())
    assert _snapshot(dbmod.connect(tmp_path / "conveyor.db"), ()) == before


def test_cli_apply_then_audit_is_clean_for_the_renamed_rows(conn, tmp_path, monkeypatch, capsys):
    conn.close()
    _cli(monkeypatch, tmp_path, ["apply", "__osEPiRawWriteIo", "__osEPiRawReadIo",
                                 "osYieldThread", "--retire", "read_controllers"])
    assert "applied 5 actions" in capsys.readouterr().out
    _cli(monkeypatch, tmp_path, ["audit"])
    out = capsys.readouterr().out
    assert out.split() [0] == "unrelated" and len(out.strip().splitlines()) == 1


def test_build_object_assembles_and_gates_a_real_region(tmp_path):
    import shutil
    if shutil.which("mips-linux-gnu-as") is None:
        pytest.skip("mips-linux-gnu-as not installed")
    try:
        from tools.conveyor.jobs import scoring  # noqa: F401  (needs decomp-permuter)
    except Exception as exc:  # pragma: no cover - environment dependent
        pytest.skip(f"scorer unavailable: {exc}")
    region = targetsmod.Region("old", C, ["jr $ra", "nop"], ["03E00008", "00000000"])
    out = tr.build_object(region, "osYieldThread", tmp_path / "t.o")
    assert out.is_file()
    bad = targetsmod.Region("old", C, ["jr $ra", "nop"], ["03E00008", "24020001"])
    with pytest.raises(tr.RenameRefusal, match="gate failed"):
        tr.build_object(bad, "osYieldThread", tmp_path / "u.o")


# --- w24s: --absorbed (a promoted slot folded into its neighbour) -------------

STUB = A + 0x20  # inside __osEPiRawWriteIo (A .. A+352)


def _absorbable(conn):
    _insert(conn, "viStub", STUB, 5, "stub")
    _matrix(conn, "viStub", 2)
    for pid, outcome in (("p1", "promoted"), ("p0", "failed")):
        conn.execute("INSERT INTO promotion_record (promotion_id, target_id, source_sha, outcome,"
                     " created_at) VALUES (?, 'viStub', 's', ?, 'x')", (pid, outcome))
    conn.commit()


def test_absorbed_retires_a_promoted_slot_and_keeps_its_history(conn, tmp_path):
    _absorbable(conn)
    before = _snapshot(conn, {"__osEPiRawWriteIo", "osEPiRawStartDma", "viStub"})
    actions = tr.plan(conn, tr.layout_functions(_mapping()), ["__osEPiRawWriteIo"], (), {},
                      absorbed=["viStub"])
    stub = [a for a in actions if a["target_id"] == "viStub"][0]
    assert stub["op"] == "retire" and stub["absorbed_by"] == "__osEPiRawWriteIo"
    assert "absorbed into __osEPiRawWriteIo" in stub["note"]
    assert "promotion_record 1 promoted -> retired" in "\n".join(tr.format_plan(conn, actions))
    tr.build_objects(actions, _regions(), BlobStore(tmp_path / "blobs"),
                     builder=_builder, words_of=_words_of)
    tr.apply(conn, actions)
    assert _snapshot(conn, {"__osEPiRawWriteIo", "osEPiRawStartDma", "viStub"}) == before
    assert conn.execute("SELECT 1 FROM n64_target WHERE target_id='viStub'").fetchone() is None
    assert conn.execute("SELECT COUNT(*) FROM matrix_entry WHERE target_id='viStub'").fetchone()[0] == 0
    assert sorted(tuple(r) for r in conn.execute(
        "SELECT promotion_id, outcome FROM promotion_record WHERE target_id='viStub'")) == [
        ("p0", "failed"), ("p1", "retired")]
    assert conn.execute("SELECT insn_count FROM n64_target WHERE target_id='__osEPiRawWriteIo'"
                        ).fetchone()[0] == 86


def test_plain_retire_still_refuses_a_promoted_slot(conn):
    _absorbable(conn)
    with pytest.raises(tr.RenameRefusal, match="promotion_record"):
        _plan(conn, ["__osEPiRawWriteIo"], ["viStub"])


def test_absorbed_refusals(conn):
    _absorbable(conn)
    fns = tr.layout_functions(_mapping())
    with pytest.raises(tr.RenameRefusal, match="not inside a layout function this plan"):
        tr.plan(conn, fns, ["osYieldThread"], (), {}, absorbed=["viStub"])
    with pytest.raises(tr.RenameRefusal, match="referenced by lock"):
        tr.plan(conn, fns, ["__osEPiRawWriteIo"], (), {"viStub": ["src/rom/lib_1050.c:viStub"]},
                absorbed=["viStub"])
    with pytest.raises(tr.RenameRefusal, match="not absorbed"):
        tr.plan(conn, fns, ["__osEPiRawWriteIo"], (), {}, absorbed=["osYieldThread"])
    with pytest.raises(tr.RenameRefusal, match="both retired and absorbed"):
        tr.plan(conn, fns, ["__osEPiRawWriteIo"], ["viStub"], {}, absorbed=["viStub"])
    with pytest.raises(tr.RenameRefusal, match="no such inventory row to absorb"):
        tr.plan(conn, fns, ["__osEPiRawWriteIo"], (), {}, absorbed=["nope"])
    conn.execute("INSERT INTO work_unit (job_id, job_type, target_id, manifest_sha, state,"
                 " created_at, updated_at) VALUES ('j','search','viStub','m','PENDING','x','x')")
    with pytest.raises(tr.RenameRefusal, match="open work units"):
        tr.plan(conn, fns, ["__osEPiRawWriteIo"], (), {}, absorbed=["viStub"])


def test_cli_absorbed_dry_run(conn, tmp_path, monkeypatch, capsys):
    _absorbable(conn)
    before = _snapshot(conn, ())
    _cli(monkeypatch, tmp_path, ["apply", "__osEPiRawWriteIo", "--absorbed", "viStub", "--dry-run"])
    out = capsys.readouterr().out
    assert "retire    viStub" in out and "absorbed into __osEPiRawWriteIo" in out
    assert _snapshot(conn, ()) == before


# --- rename: a pre-existing gate failure is carried, never hidden or invented --

def _failing_gate(region, name, out_o):
    raise tr.RenameRefusal(f"{name}: round-trip gate failed: word_mismatch@3")


def test_rename_carries_a_gate_failure_the_row_already_had(conn, tmp_path):
    conn.execute("UPDATE n64_target SET gate_reason='length_mismatch 90 != 88'"
                 " WHERE target_id='osEPiRawStartDma'")
    conn.commit()
    actions = tr.rename_plan(conn, tr.layout_functions(_mapping()),
                             [(A, "osEPiRawStartDma", "__osEPiRawWriteIo")])
    tr.build_rename_objects(conn, actions, _regions(), BlobStore(tmp_path / "b"),
                            builder=_failing_gate, words_of=_words_of)
    assert actions[0]["carry"] == "length_mismatch 90 != 88"
    assert "gate failure carried: length_mismatch 90 != 88" in tr.format_rename(conn, actions)[0]
    tr.apply_rename(conn, actions)
    row = conn.execute("SELECT insn_count, target_o_sha, gate_reason FROM n64_target"
                       " WHERE target_id='__osEPiRawWriteIo'").fetchone()
    assert tuple(row) == (88, "old_a", "length_mismatch 90 != 88")
    assert conn.execute("SELECT 1 FROM n64_target WHERE target_id='osEPiRawStartDma'").fetchone() is None
    assert conn.execute("SELECT COUNT(*) FROM matrix_entry WHERE target_id='__osEPiRawWriteIo'"
                        ).fetchone()[0] == 2


def test_rename_still_refuses_a_gate_failure_of_a_passing_row(conn, tmp_path):
    actions = tr.rename_plan(conn, tr.layout_functions(_mapping()),
                             [(A, "osEPiRawStartDma", "__osEPiRawWriteIo")])
    with pytest.raises(tr.RenameRefusal, match="round-trip gate failed"):
        tr.build_rename_objects(conn, actions, _regions(), BlobStore(tmp_path / "b"),
                                builder=_failing_gate, words_of=_words_of)
    tr.build_rename_objects(conn, actions, _regions(), BlobStore(tmp_path / "b2"),
                            builder=_builder, words_of=_words_of)
    assert "carry" not in actions[0] and actions[0]["insn_count"] == 86
