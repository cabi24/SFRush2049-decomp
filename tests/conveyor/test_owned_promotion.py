"""Owned-data promotion through the batch gates (w19c).

`owned_data prepare` registers a passthrough slot (registry row + companion +
TU block); `co_owners` ties functions that emit into one slot;
context_check.py admits data only inside a slot the function owns or co-owns;
promote_batch plans, splices (removing the companion) and gates all members
of a slot as one unit; `promote run` refuses a shared slot.
"""
import hashlib
import importlib.util
import json
from pathlib import Path

import pytest

from tools.conveyor.pipeline import owned_data as O, promote as P

REPO = Path(__file__).resolve().parents[2]
FLAGS = "-g0 -O1 -mips3 -32 -G 0 -non_shared"
DATA = bytes(range(64)) + bytes(32)


def _row(**kw):
    row = {"owner": "fa", "co_owners": ["fb"], "tu": "src/rom/lib_1000.c",
           "source": "assets/us/data.bin", "container_section": ".data",
           "owner_section": ".rodata", "offset": "0x20", "size": "0x10",
           "container_vram": "0x80000000",
           "sha256": hashlib.sha256(DATA[0x20:0x30]).hexdigest(),
           "passthrough_asm": "asm/us/nonmatchings/rom/lib_1000/fa.rodata.s"}
    row.update(kw)
    return row


def _pragma(fn):
    return f'#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1000/{fn}.s")\n'


@pytest.fixture
def repo(tmp_path):
    (tmp_path / "assets/us").mkdir(parents=True)
    (tmp_path / "assets/us/data.bin").write_bytes(DATA)
    (tmp_path / "src/rom").mkdir(parents=True)
    (tmp_path / "src/rom/lib_1000.c").write_text(
        '#include "rom_tu.h"\n\n' + _pragma("fa") + _pragma("fb") + _pragma("fc"))
    (tmp_path / O.REGISTRY).write_text(json.dumps(
        {"schema": 1, "rom_slots": [], "storage_blocks": []}, indent=2) + "\n")
    return tmp_path


# --- registry ----------------------------------------------------------------------

@pytest.mark.parametrize("co", ["fb", [1], ["fb", "fb"], ["fa"], ["bad name"]])
def test_invalid_co_owners_refuse(repo, co):
    (repo / O.REGISTRY).write_text(json.dumps(
        {"schema": 1, "rom_slots": [_row(co_owners=co)], "storage_blocks": []}))
    with pytest.raises(O.OwnershipError, match="co_owners"):
        O.rom_slots(repo)


def test_prepare_registers_slot_companion_and_block(repo):
    row = O.prepare(repo, _row())
    assert row["co_owners"] == ["fb"] and O.slot_members(row) == ["fa", "fb"]
    companion = (repo / row["passthrough_asm"]).read_text()
    assert companion == '.section .rodata\n.balign 4\n.incbin "assets/us/data.bin", 0x20, 0x10\n'
    text = (repo / "src/rom/lib_1000.c").read_text()
    assert text.index(_pragma("fa")) < text.index(O.companion_block(row)) < text.index(_pragma("fb"))
    assert [r["owner"] for r in O.slots_for_function(repo, "fb", "src/rom/lib_1000.c")] == ["fa"]
    assert O.slots_for_function(repo, "fc") == []
    with pytest.raises(O.OwnershipError, match="already registered"):
        O.prepare(repo, _row())
    assert O.strip_companion(text, "fa", repo) == (
        '#include "rom_tu.h"\n\n' + _pragma("fa") + _pragma("fb") + _pragma("fc"))


@pytest.mark.parametrize("change,match", [
    ({"co_owners": ["fz"]}, "fz has no passthrough slot"),
    ({"sha256": "0" * 64}, "original-byte identity"),
    ({"offset": "0x24"}, "aligned"),
])
def test_prepare_refuses_without_writing(repo, change, match):
    before = {p: p.read_bytes() for p in repo.rglob("*") if p.is_file()}
    with pytest.raises(O.OwnershipError, match=match):
        O.prepare(repo, _row(**change))
    after = {p: p.read_bytes() for p in repo.rglob("*") if p.is_file()}
    assert after == before


def test_prepare_refuses_a_promoted_member(repo):
    tu = repo / "src/rom/lib_1000.c"
    tu.write_text(tu.read_text().replace(_pragma("fb"), "void fb(void) {}\n"))
    with pytest.raises(O.OwnershipError, match="fb has no passthrough slot"):
        O.prepare(repo, _row())


def test_promote_run_refuses_a_shared_slot(repo, monkeypatch):
    O.prepare(repo, _row())
    monkeypatch.setattr(P, "REPO", repo)
    seg = {"rom_tu": "rom/lib_1000"}
    for fn in ("fa", "fb"):
        with pytest.raises(P.Refusal, match="promote them together"):
            P._splice(repo / "src/rom/lib_1000.c", fn, seg, f"void {fn}(void) {{}}\n", "/* h */")


# --- context_check -------------------------------------------------------------------

def _context_check(monkeypatch, root):
    path = REPO / "cloud/work/boot_tail_promotion/context_check.py"
    spec = importlib.util.spec_from_file_location("context_check_under_test", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    monkeypatch.setattr(mod, "ROOT", root)
    return mod


def test_context_check_flagset_cflags(monkeypatch):
    cc = _context_check(monkeypatch, REPO)
    assert cc.cflags_for(cc.DEFAULT_FLAGSET) == cc.CFLAGS
    got = cc.cflags_for(FLAGS)
    assert "-O1" in got and "-O2" not in got
    assert got[got.index("-mips3") + 1] == "-32" and "-mips2" not in got
    for bad in ("-g0 -O2 -mips3 -G 0", "-g0 -mips2", "-g0 -O2 -mips1 -G 0"):
        with pytest.raises(SystemExit):
            cc.cflags_for(bad)


def test_context_check_admits_only_owned_sections(repo, monkeypatch):
    O.prepare(repo, _row())
    cc = _context_check(monkeypatch, repo)
    owned, why = cc.owned_sections("fb", [".rodata:16"])
    assert why is None and owned == [{"slot": "fa", "section": ".rodata", "size": 16,
                                      "members": ["fa", "fb"], "tu": "src/rom/lib_1000.c"}]
    assert "outside an owned slot" in cc.owned_sections("fb", [".data:16"])[1]
    assert "outside an owned slot" in cc.owned_sections("fc", [".rodata:16"])[1]
    assert "exceeds" in cc.owned_sections("fa", [".rodata:32"])[1]


# --- promote_batch -------------------------------------------------------------------

def _promote_batch(monkeypatch, repo):
    path = REPO / "cloud/work/boot_tail_promotion/promote_batch.py"
    spec = importlib.util.spec_from_file_location("promote_batch_owned_test", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    refusals = []
    monkeypatch.setattr(mod, "refuse", lambda fn, why: refusals.append((fn, why)))
    monkeypatch.setattr(mod, "REPO", repo)
    seg = {"yaml_name": "0x1000", "rom_tu": "rom/lib_1000", "converted": True,
           "flagset": FLAGS, "compiler": "ido5.3",
           "functions": [{"name": f, "vaddr": "0x0", "size": 8, "state": "passthrough"}
                         for f in ("fa", "fb", "fc")]}
    monkeypatch.setattr(mod.layoutmod, "derive", lambda: {"segments": [seg]})
    (repo / "lane").mkdir(exist_ok=True)
    for f in ("fa", "fb", "fc"):
        (repo / f"lane/{f}.c").write_text(f"/* flags */\nint {f}(void) {{\n    return 1;\n}}\n")
    monkeypatch.setattr(mod.lockmod, "load_lock", lambda *a: {
        f"lane/{f}.c:{f}": {"verified": "score0", "flagset": FLAGS, "target_id": f}
        for f in ("fa", "fb", "fc")})
    return mod, refusals


def test_context_check_section_dump_ignores_object_names(monkeypatch):
    cc = _context_check(monkeypatch, REPO)

    class Done:
        def __init__(self, out):
            self.stdout = out

    def fake(cmd, **kw):
        head = f"\n{cmd[-1]}:     file format elf32-tradbigmips\n\n"
        if "-s" in cmd:
            return Done(head + "Contents of section .rodata:\n 0000 80000000 00000000  ........\n")
        return Done(head)
    monkeypatch.setattr(cc.subprocess, "run", fake)
    assert cc.section_dump("a/orig.o", ".rodata") == cc.section_dump("b/ctx.o", ".rodata") == [
        "Contents of section .rodata:", " 0000 80000000 00000000  ........"]


def _ctx(fn, owned=True):
    row = {"status": "ok", "preamble": [], "flagset": FLAGS}
    if owned:
        row["owned"] = [{"slot": "fa", "section": ".rodata", "size": 8,
                         "members": ["fa", "fb"], "tu": "src/rom/lib_1000.c"}]
    return row


def test_batch_plans_and_splices_a_slot_as_one_unit(repo, monkeypatch, tmp_path):
    O.prepare(repo, _row())
    mod, refusals = _promote_batch(monkeypatch, repo)
    ctx = {"fa": _ctx("fa"), "fb": _ctx("fb"), "fc": _ctx("fc", owned=False)}
    cands = mod.plan(["0x1000"], ctx)
    assert not refusals and [c["fn"] for c in cands] == ["fa", "fb", "fc"]
    assert [[c["fn"] for c in u] for u in mod.units(cands)] == [["fa", "fb"], ["fc"]]
    out = tmp_path / "spliced"
    out.mkdir()
    mod.splice(cands, out)
    text = (out / "lib_1000.c").read_text()
    assert "GLOBAL_ASM" not in text and "ROM_OWNED_RODATA" not in text
    assert all(f"int {f}(void)" in text for f in ("fa", "fb", "fc"))
    # The real TU is untouched by an --splice-out plan.
    assert (repo / "src/rom/lib_1000.c").read_text().count("ROM_OWNED_RODATA_BEGIN fa") == 1


@pytest.mark.parametrize("ctx,refused", [
    ({"fa": _ctx("fa")}, {"fa"}),                                    # co-owner missing
    ({"fb": _ctx("fb")}, {"fb"}),                                    # owner missing
    ({"fa": _ctx("fa"), "fb": {"status": "refused", "reason": "x"}}, {"fa", "fb"}),
    ({"fa": dict(_ctx("fa"), flagset="-g0 -O2 -mips2 -G 0 -non_shared"), "fb": _ctx("fb")},
     {"fa", "fb"}),                                                  # wrong flagset
])
def test_batch_refuses_an_incomplete_slot(repo, monkeypatch, ctx, refused):
    O.prepare(repo, _row())
    mod, refusals = _promote_batch(monkeypatch, repo)
    monkeypatch.setattr(mod, "ONLY", set(ctx))
    cands = mod.plan(["0x1000"], ctx)
    assert not cands and {fn for fn, _ in refusals} == refused


def test_batch_refuses_owned_rows_without_a_companion(repo, monkeypatch):
    mod, refusals = _promote_batch(monkeypatch, repo)
    monkeypatch.setattr(mod, "ONLY", {"fc"})
    assert mod.plan(["0x1000"], {"fc": _ctx("fc")}) == []
    assert "no companion" in refusals[0][1]
    mod2, refusals2 = _promote_batch(monkeypatch, repo)
    monkeypatch.setattr(mod2, "ONLY", {"fc"})
    assert [c["fn"] for c in mod2.plan(["0x1000"], {"fc": _ctx("fc", owned=False)})] == ["fc"]
