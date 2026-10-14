"""Recomp Phase 11 (lane w31r): battle mines, overlay probes, pak dialogs, stack-argument reads, all-lanes summary.

Fast unit tests (no ROM, no recomp host): the committed Phase 11 scripts (battle_mine's
weapon-slot poke + Z, the three pak-dialog scripts' fault codes), script_probe's overlay
function lists (`ovl_a`, `ovl_a:NAME`) and the census probes they become, ipa_detect's
save detection (a callee-saved register stored to the stack is a prologue save only when
the body reloads it from that slot: retail control_cell_text passes its s7 parameter as a
stack argument), the w28d dc.args (register mappings of control_cell_text/2/3 and
func_800E56F8), and draft_check --all-lanes (lane globs, option forwarding, matched
functions not run, cached rows reused). End to end on the recomp host only (rocky; skipped
elsewhere): battle_mine reaches func_8010C974, pak_repair reaches control_cell_text3, and an
image-A function is probed.
"""
import importlib.util
import json
import struct
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


dc = _load("recomp_p11_draft_check", "tools/recomp/draft_check.py")
rp = dc.rp
oracle = dc.oracle
ipa = _load("recomp_p11_ipa_detect", "tools/recomp/ipa_detect.py")
sp = _load("recomp_p11_script_probe", "tools/recomp/script_probe.py")
census = sys.modules.get("census") or _load("census", "tools/recomp/census.py")


# ------------------------------------------------------------------ scripts

def test_battle_mine_script():
    sc = oracle.load_script("battle_mine")
    assert sc["pak"] is True and sc["frames"] == 1500
    assert sc["pokes"] == ["780:80152B9C:309:2"]          # 26 s: weapon type 3 (mine), 9 rounds
    assert "battle_mine" in dc.all_scripts()
    text = (RECOMP / "scripts" / "battle_mine.press").read_text()
    assert "\n27 Z\n" in text and "func_8010C974" in text


def test_battle_weapons_script():
    sc = oracle.load_script("battle_weapons")
    assert [x.split(":")[2] for x in sc["pokes"]] == ["9", "109", "709"]       # types 0, 1, 7 with 9 rounds
    assert all(":80152B9C:" in x for x in sc["pokes"]) and "battle_weapons" in dc.all_scripts()


@pytest.mark.parametrize("name,err,fn", [("pak_nopak", 1, "func_800DD0C0"), ("pak_repair", 10, "control_cell_text3"),
                                         ("pak_corrupt", 3, "control_cell_text")])
def test_pak_dialog_scripts(name, err, fn):
    sc = oracle.load_script(name)
    assert sc["pak_fail"] == ["write:1:%d" % err]
    assert sc.get("pak_from") is None and sc["frames"] == 1100
    assert fn in (RECOMP / "scripts" / (name + ".press")).read_text()
    assert name in dc.all_scripts()


# ------------------------------------------------------------------ overlay probes

def test_script_probe_overlay_items():
    game = [{"name": "func_8010C974", "vram": 0x8010C974, "size": 2636}]
    got = sp.choose(game, "ovl_a:func_803990D0,func_8010C974")
    assert [(f["name"], f.get("population", "game")) for f in got] == [("func_8010C974", "game"),
                                                                      ("func_803990D0", "ovl_a")]
    assert [sp.label(f) for f in got] == ["func_8010C974", "ovl_a_func_803990D0"]
    whole = sp.choose(game, "ovl_b")
    assert len(whole) == 49 and all(f["population"] == "ovl_b" for f in whole)
    with pytest.raises(sp.dc.DCError):
        sp.choose(game, "ovl_a:func_80000000")
    with pytest.raises(sp.dc.DCError):
        sp.choose(game, "no_such_function")


def test_census_probe_entries_for_overlays():
    doc = census.probe_entries([{"name": "func_8038A400", "vram": 0x8038A400, "size": 556, "population": "ovl_a"},
                                {"name": "func_8038A400", "vram": 0x8038A400, "size": 1, "population": "ovl_b"}])
    e = doc["entries"]
    assert set(e) == {"c_ovl_a_func_8038A400", "c_ovl_b_func_8038A400"}
    assert e["c_ovl_a_func_8038A400"]["population"] == "ovl_a"
    assert e["c_ovl_a_func_8038A400"]["capture_args"] == ["a0", "a1", "a2", "a3", "ra"]
    ovl = census.overlay_functions()
    assert len([f for f in ovl if f["population"] == "ovl_a"]) == 193
    assert len([f for f in ovl if f["population"] == "ovl_b"]) == 49
    assert all(f["label"].startswith(f["population"] + "_") for f in ovl)


def test_overlay_census_files_are_committed():
    doc = json.loads((RECOMP / "census" / "overlays.json").read_text())
    f = doc["functions"]
    assert len(f) == 242
    assert f["ovl_a_func_8039842C"]["scripts"]                  # the PLAYER screen (updatePlayerScreen)
    assert "## ovl_a:" in (RECOMP / "census" / "OVERLAYS.md").read_text()


# ------------------------------------------------------------------ stack-argument reads (save detection)

def _w(*words):
    return list(words)


ADDIU_SP_M40 = 0x27BDFFD8
SW_RA_36 = 0xAFBF0024
LW_RA_36 = 0x8FBF0024
JAL = 0x0C02F81E                 # jal 0x800BE078
SW_S7_24 = 0xAFB70018            # sw s7,24(sp): an outgoing stack argument (control_cell_text)
SW_S0_16 = 0xAFB00010            # sw s0,16(sp)
LW_S0_16 = 0x8FB00010            # lw s0,16(sp)
LI_S0_5 = 0x24100005             # li s0,5 (s0 written, not read)
JR_RA = 0x03E00008
ADDIU_SP_P40 = 0x27BD0028


def test_stack_argument_of_a_callee_saved_register_is_a_read():
    body = _w(ADDIU_SP_M40, SW_RA_36, JAL, SW_S7_24, LW_RA_36, JR_RA, ADDIU_SP_P40)
    assert "s7" in ipa.register_params(body)
    restores = ipa._restores(body)
    assert not ipa._is_save(SW_S7_24, restores) and ipa._is_save(SW_S7_24)      # old answer without restores


def test_prologue_save_with_restore_is_not_a_read():
    body = _w(ADDIU_SP_M40, SW_RA_36, SW_S0_16, LI_S0_5, LW_S0_16, LW_RA_36, JR_RA, ADDIU_SP_P40)
    assert ipa._is_save(SW_S0_16, ipa._restores(body))
    assert "s0" not in ipa.register_params(body)


# ------------------------------------------------------------------ dc.args for the w28d drafts

W28D = REPO / "cloud" / "work" / "frontier" / "w28d"


@pytest.mark.parametrize("dirname,function", [("attract_mode_handler", "control_cell_text"),
                                              ("attract_video_handler", "control_cell_text2"),
                                              ("attract_demo_handler", "control_cell_text3"),
                                              ("func_800E56F8", "func_800E56F8")])
def test_w28d_dc_args_map_every_parameter(dirname, function):
    drafts = {best.parent.name: (name, extra) for name, best, extra in dc.lane_drafts(str(W28D))}
    name, extra = drafts[dirname]
    name, extra, _done = dc.current_names(name, extra)
    assert name == function
    spec = extra[extra.index("--params") + 1]
    proto = rp.find_prototype((W28D / dirname / "best.c").read_text(), function)
    capture, regs = dc.parse_params(spec, proto)          # raises unless every prototype parameter is mapped
    assert len(regs) == len(rp.parse_prototype(proto)[2])


def test_control_cell_text3_stack_parameters():
    extra = dc.lane_drafts(str(W28D))
    spec = [e for n, b, e in extra if b.parent.name == "attract_demo_handler"][0]
    spec = spec[spec.index("--params") + 1]
    assert "text=sp+8" in spec and "s3=sp+20" in spec and "sel2=s8" in spec


# ------------------------------------------------------------------ --all-lanes

def test_forwarded_options():
    assert dc.forwarded_options(["--all-lanes", "--scripts", "race", "--jobs", "4", "--refresh"]) == \
        ["--scripts", "race", "--jobs", "4"]
    assert dc.forwarded_options(["--all-lanes", "w28*", "--json", "x.json", "--remote", "rocky"]) == \
        ["--remote", "rocky"]
    assert dc.forwarded_options(["--lane", "w28d", "--remote", "rocky", "--jobs", "6"]) == \
        ["--remote", "rocky", "--jobs", "6"]


def test_all_lane_dirs_default_globs():
    names = [d.name for d in dc.all_lane_dirs(dc.DEFAULT_LANE_GLOBS)]
    assert "w28a" in names and "w30d" in names and "w27r" not in names


def test_check_drafts_skips_matched_and_reuses_cache(tmp_path, monkeypatch, capsys):
    d = tmp_path / "wX" / "func_80000001"
    d.mkdir(parents=True)
    best = d / "best.c"
    best.write_text("void func_80000001(void) {}\n")
    m = tmp_path / "wX" / "func_80000002"
    m.mkdir()
    (m / "best.c").write_text("void func_80000002(void) {}\n")
    monkeypatch.setattr(dc, "matched_functions", lambda lock=None: {"func_80000002"})
    key = dc.draft_key("func_80000001", best, [], [])
    cache = tmp_path / "cache.json"
    cache.write_text(json.dumps({key: {"verdict": "EQUIVALENT", "detail": "over 3 calls / 1 scripts (race 3)",
                                       "date": "2026-10-14"}}))

    def no_run(*a, **k):
        raise AssertionError("a cached or matched draft must not run")
    monkeypatch.setattr(dc.subprocess, "run", no_run)
    monkeypatch.setattr(dc, "_save_cache", lambda *a: (_ for _ in ()).throw(AssertionError("no new rows")))
    rows = dc.check_drafts([("wX", "func_80000001", best, []), ("wX", "func_80000002", m / "best.c", [])], [],
                           "TEST", cache_path=cache)
    assert [r["verdict"] for r in rows] == ["EQUIVALENT", "MATCHED"]
    assert rows[0]["cached"] == "2026-10-14"
    out = capsys.readouterr().out
    assert "TEST: 2 drafts: 1 EQUIVALENT, 1 MATCHED" in out and "(cached)" in out
    assert dc.draft_key("func_80000001", best, [], ["--scripts", "race"]) != key
    assert dc.draft_key("func_80000001", best, [], ["--jobs", "3"]) == key       # parallelism is not in the key
    (tmp_path / "wX" / "func_80000001" / "best.c").write_text("void func_80000001(void) { }\n")
    assert dc.draft_key("func_80000001", best, [], []) != key                    # the draft is


# ------------------------------------------------------------------ end to end (recomp host only)

def _on_recomp_host():
    p = dc.paths()
    return all(Path(p[k]).exists() for k in ("rom", "n64recomp", "rsprecomp", "runtime", "ido53"))


rocky = pytest.mark.skipif(not _on_recomp_host(), reason="recomp host only (ROM, N64Recomp, IDO toolkits)")


@rocky
def test_e2e_battle_mine_and_pak_repair_reach_their_functions():
    p = subprocess.run([sys.executable, "-m", "tools.recomp.script_probe", "battle_mine", "pak_repair",
                        "--functions", "func_8010C974,control_cell_text3,ovl_a:func_8039842C", "--frames", "1000"],
                       cwd=str(REPO), capture_output=True, text=True)
    assert p.returncode == 0, p.stdout + p.stderr
    mine, repair = p.stdout.split("\npak_repair (")           # results print in argument order
    assert "reached func_8010C974" in mine and "control_cell_text3" not in mine
    assert "reached control_cell_text3" in repair and "func_8010C974" not in repair
    assert p.stdout.count("reached ovl_a_func_8039842C") == 2          # the front end is image A


# ------------------------------------------------------------------ inbound shim: stack parameter at the same offset

def test_inbound_shim_copies_a_stack_parameter_at_the_same_offset():
    proto = "void control_cell_text3(s32 sel1, s32 row, u8 *text, char *s1, char *s2, char *s3, s32 col, s32 sel2)"
    params = ["s5", "s6", "sp+8", "sp+12", "sp+16", "sp+20", "s7", "s8"]
    words = rp.ipa_shim(0x80800000, proto, params, (), "in")
    frame = (-(words[0] & 0xFFFF)) & 0xFFFF                 # addiu sp,sp,-frame
    lw_t9 = [(w & 0xFFFF) for w in words if w >> 16 == 0x8FB9]          # lw t9,off(sp)
    sw_t9 = [(w & 0xFFFF) for w in words if w >> 16 == 0xAFB9]          # sw t9,off(sp)
    # s2 (o32 sp+16) and s3 (sp+20) come from the retail caller's sp+16/+20, i.e. frame+16/+20 inside the shim
    assert frame + 16 in lw_t9 and frame + 20 in lw_t9 and 16 in sw_t9 and 20 in sw_t9
