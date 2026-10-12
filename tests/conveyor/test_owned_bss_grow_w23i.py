"""Growing a promoted owner's .bss block at its front (w23i).

lib_16320's .bss block (owner func_80016E68 with two co-owners, w20b/w21d) was
0x30 B at 0x80042670. Once func_80016C20/80016CF0 (statics below the block)
and func_80017040 (statics above it) are C, the TU's whole .bss is 0x80 B at
0x80042630: IDO 7.1 lays a TU's .bss out in definition order, so there is no
hash-neutral passthrough state for the grown block and `replace_slot` joins
the new members' promotion (promote_batch --replace-slot), as w22i's .rodata
replacement does. The old range must lie inside the new one; the linker
ASSERTs and the promoted set's full-ROM SHA-1 prove the layout.
"""
import importlib.util
import json
from pathlib import Path

import pytest

from tools.conveyor.pipeline import owned_data as O

REPO = Path(__file__).resolve().parents[2]
FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"
TU = "src/rom/lib_1000.c"
COMPANION = "asm/us/nonmatchings/rom/lib_1000/fb.bss.s"
LD = """SECTIONS
{
    data_VRAM_END = .;
    .main_bss (NOLOAD) : SUBALIGN(16)
    {
        build/us/src/rom/lib_1000.o(.bss);
        build/us/src/rom/lib_2000.o(.bss);
    }
}
"""


def _bss(**kw):
    row = {"owner": "fb", "tu": TU, "owner_section": ".bss", "container_section": ".bss",
           "container_vram": "0x80100040", "offset": "0x0", "size": "0x30", "passthrough_asm": COMPANION}
    row.update(kw)
    return row


def _grown(**kw):
    row = {"co_owners": ["fa", "fc"], "container_vram": "0x80100000", "size": "0x80"}
    row.update(kw)
    return _bss(**row)


def _pragma(fn):
    return f'#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_1000/{fn}.s")\n'


PROMOTED_FB = "/* PROMOTED 2026-10-11 — fb\n */\nint fb(void) {\n    static int k;\n    return k;\n}\n"


@pytest.fixture
def repo(tmp_path):
    (tmp_path / "src/rom").mkdir(parents=True)
    (tmp_path / TU).write_text('#include "rom_tu.h"\n\n' + _pragma("fa") + PROMOTED_FB + _pragma("fc"))
    (tmp_path / O.REGISTRY).write_text(json.dumps(
        {"schema": 1, "rom_slots": [_bss()], "storage_blocks": []}, indent=2) + "\n")
    asm = tmp_path / "asm/us/nonmatchings/rom/lib_1000"
    asm.mkdir(parents=True)
    (tmp_path / COMPANION).write_text(O.companion_source(O.rom_slots(tmp_path)[0]))
    (tmp_path / "rush2049.us.ld").write_text(O.rewrite_linker(LD, tmp_path))
    return tmp_path


def _state(repo):
    return {p: (repo / p).read_text() for p in (O.REGISTRY, "rush2049.us.ld", TU, COMPANION)}


# --- the committed lib_16320 block ---------------------------------------------------

def test_committed_lib_16320_block_is_grown_and_fully_promoted():
    r, = [r for r in O.rom_slots(REPO, check_source=False) if r["owner"] == "func_80016E68"]
    assert O.is_bss(r) and (r["container_vram"], r["size"]) == (0x80042630, 0x80)
    assert O.slot_members(r) == ["func_80016E68", "func_80016C20", "func_80016CF0", "func_80016EE0",
                                 "func_80016F80", "func_80017040"]
    text = (REPO / r["tu"]).read_text()
    assert O.pending_members(r, text) == [] and O.companion_block(r) not in text
    assert "#pragma GLOBAL_ASM" not in text  # lib_16320 is all C
    assert (REPO / r["passthrough_asm"]).read_text() == O.companion_source(r)


# --- replace_slot on a .bss block ----------------------------------------------------

def test_bss_block_grows_at_its_front_and_restores(repo):
    before = _state(repo)
    row, originals = O.replace_slot(repo, _grown())
    assert (row["container_vram"], row["size"]) == (0x80100000, 0x80)
    assert O.slot_members(row) == ["fb", "fa", "fc"]
    reg = json.loads((repo / O.REGISTRY).read_text())["rom_slots"]
    assert len(reg) == 1 and reg[0]["container_vram"] == "0x80100000"
    assert (repo / COMPANION).read_text() == ".section .bss\n.balign 4\n.space 0x80\n"
    ld = (repo / "rush2049.us.ld").read_text()
    assert ".rom_owned_bss_fb 0x80100000 (NOLOAD)" in ld
    assert "ASSERT(rom_owned_fb_START == 0x80100000" in ld
    assert "rom_owned_fb_END - rom_owned_fb_START == 0x80" in ld
    assert (repo / TU).read_text() == before[TU]  # no companion block: the TU is the promotion's
    assert O.pending_members(row, (repo / TU).read_text()) == ["fa", "fc"]
    O.restore(originals)
    assert _state(repo) == before


def test_bss_block_may_grow_at_its_end_only(repo):
    row, originals = O.replace_slot(repo, _grown(container_vram="0x80100040", size="0x40", co_owners=["fc"]))
    assert (row["container_vram"], row["size"]) == (0x80100040, 0x40)
    O.restore(originals)


@pytest.mark.parametrize("raw,match", [
    (_grown(size="0x40"), "grow the same slot"),                     # misses the old end
    (_grown(container_vram="0x80100050"), "grow the same slot"),     # starts inside the old block
    (_bss(), "grow the same slot"),                                  # not larger
    (_grown(tu="src/rom/lib_2000.c"), "grow the same slot"),
    (_grown(passthrough_asm="asm/us/nonmatchings/rom/lib_1000/fa.bss.s"), "grow the same slot"),
    (_grown(container_vram="0x80100008"), "aligned sections"),
    (_grown(co_owners=["fa", "fz"]), "still be passthrough"),
    ({"owner": "fb", "tu": TU, "source": "assets/us/data.bin", "owner_section": ".rodata",
      "offset": "0x0", "size": "0x10", "container_vram": "0x80000000", "sha256": "0" * 64,
      "passthrough_asm": COMPANION}, "keep the slot's section"),
])
def test_bss_replacement_refusals_write_nothing(repo, raw, match):
    before = _state(repo)
    with pytest.raises(O.OwnershipError, match=match):
        O.replace_slot(repo, raw)
    assert _state(repo) == before


def test_bss_growth_into_another_block_is_refused(repo):
    reg = json.loads((repo / O.REGISTRY).read_text())
    reg["rom_slots"].append(_bss(owner="fo", tu="src/rom/lib_2000.c", container_vram="0x80100000",
                                 size="0x10", passthrough_asm="asm/us/nonmatchings/rom/lib_2000/fo.bss.s"))
    (repo / O.REGISTRY).write_text(json.dumps(reg, indent=2) + "\n")
    before = _state(repo)
    with pytest.raises(O.OwnershipError, match="overlaps another storage block"):
        O.replace_slot(repo, _grown())
    assert _state(repo) == before


def test_bss_growth_refuses_an_unpromoted_old_block(repo):
    tu = repo / TU
    tu.write_text(tu.read_text().replace(PROMOTED_FB, _pragma("fb")))
    before = _state(repo)
    with pytest.raises(O.OwnershipError, match="not fully promoted"):
        O.replace_slot(repo, _grown())
    assert _state(repo) == before


# --- promote_batch --replace-slot with a .bss block -----------------------------------

def _promote_batch(monkeypatch, repo):
    path = REPO / "cloud/work/boot_tail_promotion/promote_batch.py"
    spec = importlib.util.spec_from_file_location("promote_batch_bss_grow_test", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    refusals = []
    monkeypatch.setattr(mod, "refuse", lambda fn, why: refusals.append((fn, why)))
    monkeypatch.setattr(mod, "REPO", repo)
    seg = {"yaml_name": "0x1000", "rom_tu": "rom/lib_1000", "converted": True,
           "flagset": FLAGS, "compiler": "ido5.3",
           "functions": [{"name": "fa", "vaddr": "0x0", "size": 8, "state": "passthrough"},
                         {"name": "fb", "vaddr": "0x0", "size": 8, "state": "promoted"},
                         {"name": "fc", "vaddr": "0x0", "size": 8, "state": "passthrough"}]}
    monkeypatch.setattr(mod.layoutmod, "derive", lambda: {"segments": [seg]})
    (repo / "lane").mkdir(exist_ok=True)
    for f in ("fa", "fc"):
        (repo / f"lane/{f}.c").write_text(f"/* flags */\nint {f}(void) {{\n    static int k;\n    return k;\n}}\n")
    monkeypatch.setattr(mod.lockmod, "load_lock", lambda *a: {
        f"lane/{f}.c:{f}": {"verified": "score0", "flagset": FLAGS, "target_id": f} for f in ("fa", "fc")})
    return mod, refusals


def _ctx(members):
    owned = [{"slot": "fb", "section": ".bss", "size": 16, "members": members, "tu": TU}]
    return {f: {"status": "ok", "preamble": [], "flagset": FLAGS, "owned": owned} for f in ("fa", "fc")}


def test_batch_plans_the_grown_block_as_one_unit(repo, monkeypatch, tmp_path):
    mod, refusals = _promote_batch(monkeypatch, repo)
    monkeypatch.setattr(mod, "REPLACED", O.replace_slot(repo, _grown()))
    cands = mod.plan(["0x1000"], _ctx(["fb", "fa", "fc"]))
    assert not refusals and [c["fn"] for c in cands] == ["fa", "fc"]
    assert cands[0]["slot"]["members"] == ["fa", "fc"]  # the pending members; fb is already C
    assert [[c["fn"] for c in u] for u in mod.units(cands)] == [["fa", "fc"]]
    out = tmp_path / "spliced"
    out.mkdir()
    mod.splice(cands, out)
    text = (out / "lib_1000.c").read_text()
    assert "GLOBAL_ASM" not in text and "ROM_OWNED" not in text
    assert text.index("int fa(void)") < text.index("int fb(void)") < text.index("int fc(void)")


def test_batch_without_the_replacement_refuses_the_new_members(repo, monkeypatch):
    mod, refusals = _promote_batch(monkeypatch, repo)
    assert mod.plan(["0x1000"], _ctx(["fb", "fa", "fc"])) == []
    assert {fn for fn, _ in refusals} == {"fa", "fc"}


def test_batch_refuses_a_new_member_left_out(repo, monkeypatch):
    mod, refusals = _promote_batch(monkeypatch, repo)
    monkeypatch.setattr(mod, "REPLACED", O.replace_slot(repo, _grown()))
    monkeypatch.setattr(mod, "ONLY", {"fa"})
    assert mod.plan(["0x1000"], _ctx(["fb", "fa", "fc"])) == []
    assert refusals[0][0] == "fa" and "needs fc" in refusals[0][1]
