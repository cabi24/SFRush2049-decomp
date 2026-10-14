"""Recomp Phase 12 (lane w32r): spill-aware clobber sets, exact fault PCs, the boot-read
pak crash, draft_check of overlay drafts.

Fast unit tests (no ROM, no recomp host): regmap_verify.Interproc counts a store of a
register the body already wrote as a spill / stack argument, not a prologue save (retail
func_800DD0C0 writes s0, s1, s3 unsaved: the control_settings pak_nopak "fault" was this
harness gap), the corrected ipa_callees.json clobbers and draft_check's widening of table
entries, the fault report's line-0 fallback and the host build's no-tail-merge flags, the
refusal of `pak fail read 1 ERR` on a `pak from` pak (a retail crash), and overlay names
in draft_check (`ovl_a:NAME`, the overlay census, the overlay lock). End to end on the
recomp host only (rocky; skipped elsewhere): w30d's control_settings is EQUIVALENT on
pak_nopak, and a matched image-A function checked against its own locked source is
EQUIVALENT while an injected bug DIVERGES.
"""
import importlib.util
import json
import re
import subprocess
import sys
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]
RECOMP = REPO / "tools" / "recomp"


def _load(name, rel):
    spec = importlib.util.spec_from_file_location(name, REPO / rel)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


dc = _load("recomp_p12_draft_check", "tools/recomp/draft_check.py")
rp = dc.rp
oracle = dc.oracle
rv = _load("recomp_p12_regmap_verify", "tools/recomp/regmap_verify.py")


# ------------------------------------------------------------------ spill vs prologue save

def _image(words, base=0x80100000):
    data = b"".join(w.to_bytes(4, "big") for w in words)
    return (base, data), [{"name": "f", "vram": base, "size": len(data)}]


ADDIU_SP_M40, ADDIU_SP_P40 = 0x27BDFFD8, 0x27BD0028
SW_RA_36, LW_RA_36 = 0xAFBF0024, 0x8FBF0024
SW_S0_16, LW_S0_16 = 0xAFB00010, 0x8FB00010
MOVE_S0_V0 = 0x00408025           # or s0,v0,zero
LI_S0_5 = 0x24100005
JAL_OUT = 0x0C000100              # jal 0x80000400 (outside the image: default effects)
NOP, JR_RA = 0, 0x03E00008


def test_spill_of_a_written_register_is_not_a_save():
    image, funcs = _image([ADDIU_SP_M40, SW_RA_36, JAL_OUT, NOP, MOVE_S0_V0, SW_S0_16, JAL_OUT, NOP,
                           LW_S0_16, LW_RA_36, JR_RA, ADDIU_SP_P40])
    assert "s0" in rv.Interproc(image, funcs).unsaved(0x80100000)


def test_prologue_save_is_still_a_save():
    image, funcs = _image([ADDIU_SP_M40, SW_RA_36, SW_S0_16, LI_S0_5, JAL_OUT, NOP,
                           LW_S0_16, LW_RA_36, JR_RA, ADDIU_SP_P40])
    assert "s0" not in rv.Interproc(image, funcs).unsaved(0x80100000)


SW_S8_24 = 0xAFBE0018            # sw s8,24(sp): an outgoing stack argument
MOVE_S8_V0 = 0x0040F025          # or s8,v0,zero


def test_stack_argument_before_the_first_write_is_not_a_save():
    # control_cell_text2: passes its s8 parameter on at 0x18(sp), never reloads it, then writes s8 unsaved
    image, funcs = _image([ADDIU_SP_M40, SW_RA_36, JAL_OUT, SW_S8_24, MOVE_S8_V0, JAL_OUT, NOP,
                           LW_RA_36, JR_RA, ADDIU_SP_P40])
    assert "s8" in rv.Interproc(image, funcs).unsaved(0x80100000)


def _retail_words(name):
    """The committed .word body of a game function (asm/us/blob)."""
    for asm in sorted((REPO / "asm" / "us" / "blob").glob("blob_*.s")):
        text = asm.read_text()
        m = re.search(r"^%s:\n(.*?)(?:\n\.section|\Z)" % re.escape(name), text, re.S | re.M)
        if m:
            return [int(x, 16) for x in re.findall(r"\.word 0x([0-9A-Fa-f]{8})", m.group(1))]
    raise AssertionError(name)


def test_retail_func_800DD0C0_clobbers_s0_to_s8():
    words = _retail_words("func_800DD0C0")
    assert len(words) == 908 // 4
    image, funcs = _image(words, 0x800DD0C0)
    assert rv.Interproc(image, funcs).unsaved(0x800DD0C0) == ["s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8"]


def test_ipa_callee_clobbers_are_the_retail_sets():
    t = json.loads((RECOMP / "ipa_callees.json").read_text())["callees"]
    assert t["func_800DD0C0"]["clobbers"] == ["s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8"]
    assert t["func_80097164"]["clobbers"] == ["s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7"]
    assert "s5" in t["DrawName"]["clobbers"]
    words = rp.ipa_shim(0x80800000, t["func_800DD0C0"]["prototype"], t["func_800DD0C0"]["params"],
                        t["func_800DD0C0"]["clobbers"], "out")
    saved = {(w >> 16) & 31 for w in words if w >> 26 == 0x2B and (w >> 21) & 31 == 29}     # sw REG,off(sp)
    assert {16, 17, 19} <= saved                                                           # s0, s1, s3


def test_widen_clobbers_keeps_members_and_order():
    table = {"f": {"params": ["sp+0"], "clobbers": ["s2"]}, "g": {"params": ["a0"]}}
    got = dc.widen_clobbers(table, {"f": ["s0", "s2"], "g": ["s1"], "h": ["s3"]}, members=("g",))
    assert got == [("f", ["s0"])]
    assert table["f"]["clobbers"] == ["s2", "s0"] and "clobbers" not in table["g"] and "h" not in table


def test_cache_epoch_bumped():
    assert dc.CACHE_EPOCH == "w32r-2"


# ------------------------------------------------------------------ fault report

def test_fault_lines_without_a_line_name_the_function():
    fault = {"signal_name": "SIGSEGV", "access": "read", "n64": "0x00000008", "frame": "721", "thread": "game7",
             "n64_function": "repl__draft__control_settings", "n64_pc": None,
             "n64_chain": [{"n64_function": "repl__draft__control_settings", "n64_pc": None}]}
    text = "\n".join(oracle.fault_lines(fault))
    assert "N64 PC unknown in control_settings (draft)" in text and "RUSH_EXACT_FAULT_LINES" in text


def test_host_build_keeps_one_code_copy_per_n64_site():
    cm = (RECOMP / "runtime" / "CMakeLists.txt").read_text()
    assert "RUSH_EXACT_FAULT_LINES" in cm and "-enable-tail-merge=false" in cm and "-fno-crossjumping" in cm


# ------------------------------------------------------------------ boot-read pak failure (retail crash)

def test_boot_read_failure_is_refused(monkeypatch):
    monkeypatch.delenv("RUSH_PAK_FAIL_ALLOW_CRASH", raising=False)
    text = (RECOMP / "scripts" / "player_reload.press").read_text()
    with pytest.raises(oracle.OracleError, match="crashes when its error dialog is closed"):
        oracle.parse_script(text + "\npak fail read 1 3\n", "x")
    with pytest.raises(oracle.OracleError):
        oracle.parse_script(text + "\npak fail read 1\n", "x")                  # default ERR 4 crashes too
    for err in oracle.BOOT_READ_FAIL_SURVIVES:
        assert oracle.parse_script(text + "\npak fail read 1 %d\n" % err, "x")["pak_fail"] == ["read:1:%d" % err]
    assert oracle.parse_script(text + "\npak fail read 2 3\n", "x")["pak_fail"] == ["read:2:3"]
    fresh = "frames 100\n4 START\npak fail read 1 3\n"                         # no `pak from`: no boot read
    assert oracle.parse_script(fresh, "x")["pak_fail"] == ["read:1:3"]
    monkeypatch.setenv("RUSH_PAK_FAIL_ALLOW_CRASH", "1")
    assert oracle.parse_script(text + "\npak fail read 1 3\n", "x")["pak_fail"] == ["read:1:3"]


def test_committed_scripts_still_parse(monkeypatch):
    monkeypatch.delenv("RUSH_PAK_FAIL_ALLOW_CRASH", raising=False)
    for name in dc.all_scripts():
        oracle.load_script(name)


# ------------------------------------------------------------------ overlay drafts

def test_overlay_names():
    assert dc.split_overlay_name("ovl_a:func_80396F44") == ("ovl_a", "func_80396F44")
    assert dc.split_overlay_name("ovl_b_func_8038A8CC") == ("ovl_b", "func_8038A8CC")
    assert dc.split_overlay_name("control_settings") == (None, "control_settings")
    assert dc.find_population("func_80396F44") == "ovl_a"
    assert dc.find_population("func_8038A8CC", image="ovl_b") == "ovl_b"
    with pytest.raises(dc.DCError, match="both overlay images"):
        dc.find_population("func_8038A400")
    assert dc.find_population("func_8038A400", image="ovl_b") == "ovl_b"
    with pytest.raises(dc.DCError):
        dc.find_population("control_settings", image="ovl_a")


def test_overlay_census_scripts_and_matched_lock():
    scripts, covered = dc.overlay_census_scripts("ovl_a", "func_80396F44")
    assert covered and scripts and "controls_frontend" in scripts
    assert dc.overlay_census_scripts("ovl_a", "func_00000000") == (None, False)
    m = dc.matched_functions()
    assert "ovl_a:func_80396F44" in m and "ovl_a_func_80396F44" in m and "control_settings" not in m


# ------------------------------------------------------------------ end to end (recomp host only)

def _on_recomp_host():
    p = dc.paths()
    return all(Path(p[k]).exists() for k in ("rom", "n64recomp", "rsprecomp", "runtime", "ido53"))


rocky = pytest.mark.skipif(not _on_recomp_host(), reason="recomp host only (ROM, N64Recomp, IDO toolkits)")


def _draft_check(*args):
    env = dict(__import__("os").environ, RUSH_DC_BUILD_JOBS="4")
    return subprocess.run([sys.executable, "-m", "tools.recomp.draft_check"] + list(args), cwd=str(REPO),
                          capture_output=True, text=True, env=env)


@rocky
def test_e2e_control_settings_pak_nopak_equivalent():
    p = _draft_check("control_settings", "cloud/work/frontier/w30d/control_settings/best.c",
                     "--scripts", "pak_nopak", "--jobs", "2")
    assert p.returncode == 0 and "VERDICT: EQUIVALENT" in p.stdout, p.stdout + p.stderr


@rocky
def test_e2e_overlay_draft_own_source_equivalent_and_bug_diverges(tmp_path):
    src = REPO / "src" / "ovl_a" / "func_80396F44.c"
    p = _draft_check("ovl_a:func_80396F44", str(src), "--scripts", "controls_frontend", "--jobs", "2")
    assert p.returncode == 0 and "VERDICT: EQUIVALENT" in p.stdout and "[ovl_a]" in p.stdout, p.stdout + p.stderr
    bug = tmp_path / "func_80396F44.c"
    bug.write_text(src.read_text().replace("return found;", "return found + 1;"))
    p = _draft_check("ovl_a:func_80396F44", str(bug), "--scripts", "controls_frontend", "--jobs", "2")
    assert p.returncode == 1 and "return value differs" in p.stdout, p.stdout + p.stderr
