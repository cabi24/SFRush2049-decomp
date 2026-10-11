"""Per-ROM-TU compiler selection (rom_compilers.json -> opt_overrides.mk CC :=),
the lock/segment compiler checks, and promote_batch's compiler preconditions."""
import importlib.util
import json
import shutil
import subprocess
from pathlib import Path

import pytest

from tools.conveyor import compilers
from tools.conveyor.pipeline import layout

REPO = Path(__file__).resolve().parents[2]
BASE_CFLAGS = "-G 0 -mips2 -O2 -non_shared -Iinclude -Wab,-r4300_mul -Xcpluscomm"
needs_make = pytest.mark.skipif(not shutil.which("make"), reason="needs make")


def _seg(name, compiler=None, flagset="-g0 -O2 -mips2 -G 0 -non_shared", converted=True,
         functions=()):
    seg = {"yaml_name": name, "rom_tu": f"rom/lib_{int(name, 16):x}", "converted": converted,
           "flagset": flagset, "functions": [{"name": f, "vaddr": "0x0", "size": 4,
                                              "state": "passthrough"} for f in functions]}
    if compiler is not None:
        seg["compiler"] = compiler
    return seg


def _overrides(tmp_path, monkeypatch, segments):
    fragment = tmp_path / "overrides.mk"
    monkeypatch.setattr(layout, "OPT_OVERRIDES_MK", fragment)
    layout.write_opt_overrides({"segments": segments})
    return fragment


def _effective(tmp_path, fragment, tu):
    """(CC, CFLAGS) make resolves for build/src/rom/<tu>.o with the fragment."""
    makefile = tmp_path / "Makefile"
    makefile.write_text(
        "COMPILER := ido\nBUILD_DIR := build\nCC := ido53/cc\n"
        "IDO71_CC := ido71/cc\n"
        f"CFLAGS := {BASE_CFLAGS}\ninclude {fragment}\n"
        "build/src/rom/%.o:\n\t@echo $(CC) '|' $(CFLAGS)\n")
    out = subprocess.run(["make", "--no-print-directory", "-f", str(makefile),
                          f"build/src/rom/{tu}.o"], check=True, capture_output=True,
                         text=True).stdout
    cc, _, flags = out.partition("|")
    return cc.strip(), flags.split()


def test_unpinned_segments_emit_nothing(tmp_path, monkeypatch):
    frag = _overrides(tmp_path, monkeypatch, [_seg("0x11640"), _seg("0x10d60", "ido5.3")])
    assert "CC :=" not in frag.read_text()
    assert "lib_11640" not in frag.read_text()


def test_pinned_segment_gets_cc_line_only_when_converted(tmp_path, monkeypatch):
    frag = _overrides(tmp_path, monkeypatch, [
        _seg("0x11640", "ido7.1"), _seg("0x26f60", "ido7.1", converted=False)])
    lines = frag.read_text().splitlines()
    assert "$(BUILD_DIR)/src/rom/lib_11640.o: CC := $(IDO71_CC)" in lines
    assert not any("26f60" in line for line in lines)


@needs_make
def test_make_resolves_cc_per_tu_and_keeps_flags(tmp_path, monkeypatch):
    frag = _overrides(tmp_path, monkeypatch, [
        _seg("0x11640", "ido7.1"), _seg("0x10d60"),
        _seg("0x7600", "ido7.1", flagset="-g0 -O1 -mips2 -G 0 -non_shared")])
    assert _effective(tmp_path, frag, "lib_11640") == ("ido71/cc", BASE_CFLAGS.split())
    assert _effective(tmp_path, frag, "lib_10d60") == ("ido53/cc", BASE_CFLAGS.split())
    cc, flags = _effective(tmp_path, frag, "lib_7600")
    assert cc == "ido71/cc" and "-O1" in flags and "-O2" not in flags


def test_unknown_compiler_refuses(tmp_path, monkeypatch):
    with pytest.raises(SystemExit, match="unknown compiler"):
        _overrides(tmp_path, monkeypatch, [_seg("0x11640", "ido6.2")])


def test_compiler_refusal_requires_exact_compiler():
    seg71, seg53 = _seg("0x11640", "ido7.1"), _seg("0x10d60", "ido5.3")
    assert layout.compiler_refusal(seg71, {"compiler": "ido7.1"}) is None
    assert layout.compiler_refusal(seg53, {}) is None
    assert "re-lock with the ido7.1 toolkit" in layout.compiler_refusal(seg71, {})
    assert "builds with ido5.3" in layout.compiler_refusal(seg53, {"compiler": "ido7.1"})


def test_compiler_conflicts_flag_only_bound_locks_in_other_segments():
    mapping = {"segments": [_seg("0x11640", "ido7.1", functions=["a", "b"]),
                            _seg("0x10d60", "ido5.3", functions=["c", "d"])]}
    entries = {
        "src/rom/lib_11640.c:a": {"target_id": "a"},                       # legacy 5.3: fine
        "x/b.c:b": {"target_id": "b", "compiler": "ido7.1"},               # agrees
        "x/c.c:c": {"target_id": "c", "compiler": "ido7.1"},               # conflict
        "x/d.c:d": {"target_id": "d"},
    }
    out = layout.compiler_conflicts(mapping, entries)
    assert [spec for spec, _ in out] == ["x/c.c:c"]


def test_derive_refuses_pins_on_unconverted_segments(monkeypatch):
    subs = [{"off": 0x11640, "type": "c", "name": "rom/lib_11640"},
            {"off": 0x26f60, "type": "asm", "name": None}]
    monkeypatch.setattr(layout, "parse_subsegments", lambda p=None: (subs, 0x27000))
    for fn in ("parse_symbol_addrs", "collect_regions", "load_flagsets", "load_census_extents"):
        monkeypatch.setattr(layout, fn, lambda *a, **k: {})
    monkeypatch.setattr(layout, "_derive_segment", lambda sub, rs, *a, **k: {
        "yaml_name": hex(rs), "rom_tu": f"rom/lib_{rs:x}", "converted": sub["type"] == "c",
        "flagset": None, "functions": [], "refusal": None})
    mapping = layout.derive(compiler_pins={"0x11640": "ido7.1"})
    assert [s.get("compiler") for s in mapping["segments"]] == ["ido7.1", "ido5.3", None]
    with pytest.raises(SystemExit, match="not converted code segments: 0x26f60"):
        layout.derive(compiler_pins={"0x26f60": "ido7.1"})


def test_structural_map_hash_ignores_compiler():
    seg = dict(_seg("0x11640"), size=4, vram_start="0x0", refusal=None)
    before = layout.map_hash({"segments": [seg]})
    assert layout.map_hash({"segments": [dict(seg, compiler="ido7.1")]}) == before


# --- promote_batch ----------------------------------------------------------------

def _promote_batch(monkeypatch):
    path = REPO / "cloud/work/boot_tail_promotion/promote_batch.py"
    spec = importlib.util.spec_from_file_location("promote_batch_under_test", path)
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    refusals = []
    monkeypatch.setattr(mod, "refuse", lambda fn, why: refusals.append((fn, why)))
    return mod, refusals


FLAGS = "-g0 -O2 -mips2 -G 0 -non_shared"


@pytest.mark.parametrize("lock_compiler,row_compiler,refusal", [
    ("ido7.1", "ido7.1", None),
    (None, "ido7.1", "re-lock with the ido7.1 toolkit"),
    ("ido7.1", None, "context row computed with ido5.3"),
])
def test_promote_batch_plan_checks_both_compilers(monkeypatch, lock_compiler, row_compiler,
                                                  refusal):
    mod, refusals = _promote_batch(monkeypatch)
    seg = _seg("0x11640", "ido7.1", flagset=FLAGS, functions=["func_80015348"])
    monkeypatch.setattr(mod.layoutmod, "derive", lambda: {"segments": [seg]})
    entry = {"verified": "score0", "flagset": FLAGS, "target_id": "func_80015348"}
    if lock_compiler:
        entry["compiler"] = lock_compiler
    monkeypatch.setattr(mod.lockmod, "load_lock",
                        lambda *a: {"lane/func_80015348.c:func_80015348": entry})
    row = {"status": "ok", "preamble": []}
    if row_compiler:
        row["compiler"] = row_compiler
    cands = mod.plan(["0x11640"], {"func_80015348": row})
    if refusal is None:
        assert [c["fn"] for c in cands] == ["func_80015348"] and not refusals
    else:
        assert not cands and refusal in refusals[0][1]


def test_promote_batch_53_segment_unchanged(monkeypatch):
    mod, refusals = _promote_batch(monkeypatch)
    seg = _seg("0x10d60", "ido5.3", flagset=FLAGS, functions=["osFoo"])
    monkeypatch.setattr(mod.layoutmod, "derive", lambda: {"segments": [seg]})
    monkeypatch.setattr(mod.lockmod, "load_lock", lambda *a: {
        "src/x.c:osFoo": {"verified": "score0", "flagset": FLAGS, "target_id": "osFoo"}})
    cands = mod.plan(["0x10d60"], {"osFoo": {"status": "ok", "preamble": []}})
    assert [c["fn"] for c in cands] == ["osFoo"] and not refusals


def test_context_check_records_compiler():
    text = (REPO / "cloud/work/boot_tail_promotion/context_check.py").read_text()
    assert 'row["compiler"] = COMPILER' in text
    assert "--compiler" in text
