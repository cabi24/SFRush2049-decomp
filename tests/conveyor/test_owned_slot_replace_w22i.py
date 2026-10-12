"""Growing a promoted owner's slot into a co-owned one (w22i).

func_80023BDC (C) owned the first 0x14 bytes of lib_22300's .rodata as a
4-byte logical table; func_80023E9C's table follows. As C both are one 0x1E0
section, and there is no hash-neutral passthrough state for the grown slot
(asm-processor's GLOBAL_ASM .rodata is a file-scope array that IDO places
before every jump table). `owned_data.replace_slot` therefore joins the new
members' promotion transaction (promote_batch --replace-slot), and a function
may be a member of several slots of its TU (func_80023E9C also owns the TU's
.bss block).
"""
import hashlib
import importlib.util
import json
from pathlib import Path

import pytest

from tools.conveyor.pipeline import owned_data as O

REPO = Path(__file__).resolve().parents[2]
FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"
DATA = bytes(range(64)) + bytes(32)
TU = "src/rom/lib_1000.c"
LD = """SECTIONS
{
    .main_data : SUBALIGN(16)
    {
        build/us/assets/us/data.o(.data);
        build/us/src/rom/lib_1000.o(.rodata);
    }
    data_VRAM_END = .;
    .main_bss (NOLOAD) : SUBALIGN(16)
    {
        build/us/src/rom/lib_1000.o(.bss);
    }
}
"""


def _row(**kw):
    row = {"owner": "fa", "tu": TU, "source": "assets/us/data.bin", "container_section": ".data",
           "owner_section": ".rodata", "offset": "0x20", "size": "0x10",
           "container_vram": "0x80000000", "sha256": hashlib.sha256(DATA[0x20:0x30]).hexdigest(),
           "passthrough_asm": "asm/us/nonmatchings/rom/lib_1000/fa.rodata.s"}
    row.update(kw)
    return row


def _grown(**kw):
    row = {"co_owners": ["fb"], "size": "0x20", "sha256": hashlib.sha256(DATA[0x20:0x40]).hexdigest()}
    row.update(kw)
    return _row(**row)


BSS = {"owner": "fb", "co_owners": ["fc"], "tu": TU, "owner_section": ".bss",
       "container_section": ".bss", "container_vram": "0x80100000", "offset": "0x0", "size": "0x30",
       "passthrough_asm": "asm/us/nonmatchings/rom/lib_1000/fb.bss.s"}


def _pragma(fn):
    return f'#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1000/{fn}.s")\n'


PROMOTED_FA = "/* PROMOTED 2026-10-11 — fa\n */\nint fa(void) {\n    return 1;\n}\n"


@pytest.fixture
def repo(tmp_path):
    (tmp_path / "assets/us").mkdir(parents=True)
    (tmp_path / "assets/us/data.bin").write_bytes(DATA)
    (tmp_path / "src/rom").mkdir(parents=True)
    (tmp_path / TU).write_text('#include "rom_tu.h"\n\n' + PROMOTED_FA + _pragma("fb") + _pragma("fc"))
    (tmp_path / O.REGISTRY).write_text(json.dumps(
        {"schema": 1, "rom_slots": [_row(), BSS], "storage_blocks": []}, indent=2) + "\n")
    asm = tmp_path / "asm/us/nonmatchings/rom/lib_1000"
    asm.mkdir(parents=True)
    (asm / "fa.rodata.s").write_text(O.companion_source(next(r for r in O.rom_slots(tmp_path) if not O.is_bss(r)), tmp_path))
    # fb owns the .bss block with fc (both passthrough): its companion is in the TU.
    bss = next(r for r in O.rom_slots(tmp_path) if O.is_bss(r))
    (asm / "fb.bss.s").write_text(O.companion_source(bss))
    text = (tmp_path / TU).read_text()
    (tmp_path / TU).write_text(text.replace(_pragma("fb"), _pragma("fb") + O.companion_block(bss)))
    (tmp_path / "rush2049.us.ld").write_text(O.rewrite_linker(LD, tmp_path))
    return tmp_path


def _state(repo):
    return {p: (repo / p).read_text() for p in (O.REGISTRY, "rush2049.us.ld", TU,
                                                "asm/us/nonmatchings/rom/lib_1000/fa.rodata.s")}


# --- the committed lib_22300 slots ------------------------------------------------

def test_committed_lib_22300_slots():
    rows = {(r["owner"], r["owner_section"]): r for r in O.rom_slots(REPO, check_source=False)
            if r["tu"] == "src/rom/lib_22300.c"}
    ro = rows[("func_80023BDC", ".rodata")]
    assert O.slot_members(ro) == ["func_80023BDC", "func_80023E9C"]
    assert (ro["container_vram"] + ro["offset"], ro["size"]) == (0x8002D950, 0x1E0)
    assert not ro.get("logical_extent") and ro.get("alignment", 16) == 16
    assert ro["sha256"] == "8840692c9c1a1a9fe3fbd7f3f1f218599e8f10bffc717d046a662cc508f808cd"
    bss = rows[("func_80023E9C", ".bss")]
    assert O.slot_members(bss) == ["func_80023E9C", "func_80022874"]
    assert (bss["container_vram"], bss["size"]) == (0x80056200, 0x30)
    text = (REPO / ro["tu"]).read_text()
    # Both slots are fully promoted: no member passthrough, no companion block.
    for r in (ro, bss):
        assert O.pending_members(r, text) == [] and O.companion_block(r) not in text
    if (REPO / ro["source"]).is_file():
        assert (REPO / ro["passthrough_asm"]).read_text() == O.companion_source(ro, REPO)
    ld = (REPO / "rush2049.us.ld").read_text()
    assert 'ASSERT(rom_owned_func_80023BDC_END - rom_owned_func_80023BDC_START == 0x1e0' in ld
    assert ".rom_owned_bss_func_80023E9C 0x80056200 (NOLOAD)" in ld


# --- replace_slot -------------------------------------------------------------------

def test_pending_members_follow_the_tu(repo):
    text = (repo / TU).read_text()
    rows = {r["owner_section"]: r for r in O.rom_slots(repo)}
    old, bss = rows[".rodata"], rows[".bss"]
    assert O.pending_members(old, text) == [] and O.pending_members(bss, text) == ["fb", "fc"]


def test_replace_slot_writes_row_companion_linker_and_restores(repo):
    before = _state(repo)
    row, originals = O.replace_slot(repo, _grown())
    assert O.slot_members(row) == ["fa", "fb"] and row["size"] == 0x20
    reg = json.loads((repo / O.REGISTRY).read_text())["rom_slots"]
    assert [r["owner"] for r in reg] == ["fa", "fb"] and reg[0]["size"] == "0x20"  # replaced in place
    assert (repo / row["passthrough_asm"]).read_text() == O.companion_source(row, repo)
    ld = (repo / "rush2049.us.ld").read_text()
    assert "rom_owned_fa_END - rom_owned_fa_START == 0x20" in ld
    assert (repo / TU).read_text() == before[TU]  # no companion block: the TU is the promotion's
    assert O.pending_members(row, (repo / TU).read_text()) == ["fb"]
    O.restore(originals)
    assert _state(repo) == before


@pytest.mark.parametrize("raw,match", [
    (_grown(offset="0x10", sha256=hashlib.sha256(DATA[0x10:0x30]).hexdigest()), "grow the same slot"),
    (_grown(tu="src/rom/lib_2000.c"), "grow the same slot"),
    (_row(), "grow the same slot"),                                     # not larger
    (_grown(owner="fz", co_owners=["fb"]), "exactly one registered row"),
    (_grown(co_owners=["fz"]), "still be passthrough"),                 # fz has no passthrough line
    (_grown(co_owners=["fb", "fz"]), "still be passthrough"),
    (_grown(sha256="0" * 64), "original-byte identity"),
    (dict(BSS, size="0x40"), "not fully promoted"),                  # w23i: .bss rows may grow too
    (dict(BSS, owner="fa", size="0x40"), "keep the slot's section"),
])
def test_replace_slot_refusals_write_nothing(repo, raw, match):
    before = _state(repo)
    with pytest.raises(O.OwnershipError, match=match):
        O.replace_slot(repo, raw)
    assert _state(repo) == before


def test_replace_slot_refuses_an_unpromoted_old_slot(repo):
    tu = repo / TU
    tu.write_text(tu.read_text().replace(PROMOTED_FA, _pragma("fa")))
    before = _state(repo)
    with pytest.raises(O.OwnershipError, match="not fully promoted"):
        O.replace_slot(repo, _grown())
    assert _state(repo) == before


def test_replace_slot_refuses_a_logical_growth(repo):
    with pytest.raises(O.OwnershipError, match="complete section"):
        O.replace_slot(repo, _grown(size="0x1c", alignment=4, logical_extent=True,
                                    sha256=hashlib.sha256(DATA[0x20:0x3c]).hexdigest()))


# --- promote_batch: several slots per function, --replace-slot ---------------------

def _promote_batch(monkeypatch, repo):
    path = REPO / "cloud/work/boot_tail_promotion/promote_batch.py"
    spec = importlib.util.spec_from_file_location("promote_batch_replace_test", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    refusals = []
    monkeypatch.setattr(mod, "refuse", lambda fn, why: refusals.append((fn, why)))
    monkeypatch.setattr(mod, "REPO", repo)
    seg = {"yaml_name": "0x1000", "rom_tu": "rom/lib_1000", "converted": True,
           "flagset": FLAGS, "compiler": "ido5.3",
           "functions": [{"name": "fa", "vaddr": "0x0", "size": 8, "state": "promoted"}]
                        + [{"name": f, "vaddr": "0x0", "size": 8, "state": "passthrough"} for f in ("fb", "fc")]}
    monkeypatch.setattr(mod.layoutmod, "derive", lambda: {"segments": [seg]})
    (repo / "lane").mkdir(exist_ok=True)
    for f in ("fb", "fc"):
        (repo / f"lane/{f}.c").write_text(f"/* flags */\nint {f}(void) {{\n    return 1;\n}}\n")
    monkeypatch.setattr(mod.lockmod, "load_lock", lambda *a: {
        f"lane/{f}.c:{f}": {"verified": "score0", "flagset": FLAGS, "target_id": f} for f in ("fb", "fc")})
    return mod, refusals


def _owned(slot, section, members):
    return {"slot": slot, "section": section, "size": 16, "members": members, "tu": TU}


CTX = {"fb": {"status": "ok", "preamble": [], "flagset": FLAGS,
              "owned": [_owned("fa", ".rodata", ["fa", "fb"]), _owned("fb", ".bss", ["fb", "fc"])]},
       "fc": {"status": "ok", "preamble": [], "flagset": FLAGS, "owned": [_owned("fb", ".bss", ["fb", "fc"])]}}


def test_batch_plans_two_slots_and_the_replacement_as_one_unit(repo, monkeypatch, tmp_path):
    mod, refusals = _promote_batch(monkeypatch, repo)
    monkeypatch.setattr(mod, "REPLACED", O.replace_slot(repo, _grown()))
    cands = mod.plan(["0x1000"], CTX)
    assert not refusals and [c["fn"] for c in cands] == ["fb", "fc"]
    fb = cands[0]["slot"]
    assert fb["members"] == ["fb", "fc"]
    assert {p["owner"]: p["members"] for p in fb["slots"]} == {"fa": ["fb"], "fb": ["fb", "fc"]}
    assert [[c["fn"] for c in u] for u in mod.units(cands)] == [["fb", "fc"]]
    out = tmp_path / "spliced"
    out.mkdir()
    mod.splice(cands, out)
    text = (out / "lib_1000.c").read_text()
    assert "GLOBAL_ASM" not in text and "ROM_OWNED_RODATA" not in text
    assert all(f"int {f}(void)" in text for f in ("fa", "fb", "fc"))


def test_batch_without_the_replacement_refuses_the_rodata_owner(repo, monkeypatch):
    # Without --replace-slot fb's .rodata is not a slot of its TU state (the
    # old row is fa's alone): its context row's owned .rodata is refused, and
    # fc goes with it (they share the .bss block).
    mod, refusals = _promote_batch(monkeypatch, repo)
    assert mod.plan(["0x1000"], CTX) == []
    assert {fn for fn, _ in refusals} == {"fb", "fc"}
    assert "is not a slot of fb" in dict(refusals)["fb"]


def test_batch_refuses_a_replacement_member_left_out(repo, monkeypatch):
    mod, refusals = _promote_batch(monkeypatch, repo)
    monkeypatch.setattr(mod, "REPLACED", O.replace_slot(repo, _grown()))
    monkeypatch.setattr(mod, "ONLY", {"fc"})
    assert mod.plan(["0x1000"], CTX) == [] and refusals[0][0] == "fc" and "needs fb" in refusals[0][1]


def test_main_dry_run_restores_the_replacement(repo, monkeypatch, capsys):
    mod, refusals = _promote_batch(monkeypatch, repo)
    before = _state(repo)
    (repo / "ctx.jsonl").write_text("".join(json.dumps(dict(v, function=k)) + "\n" for k, v in CTX.items()))
    (repo / "grown.json").write_text(json.dumps(_grown()))

    class Done:
        stdout, stderr, returncode = "", "", 0
    monkeypatch.setattr(mod, "run", lambda *a, **k: Done())
    monkeypatch.setattr(mod.sys, "argv", ["promote_batch.py", str(repo / "ctx.jsonl"), "0x1000",
                                          "--replace-slot", str(repo / "grown.json"), "--dry-run"])
    mod.main()
    out = capsys.readouterr().out
    assert "PLAN fb" in out and "PLAN fc" in out and "replace-slot fa: restored" in out
    assert _state(repo) == before
