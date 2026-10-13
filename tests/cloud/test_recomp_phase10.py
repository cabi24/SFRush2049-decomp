"""Recomp Phase 10 (lane w30r): pak ghost files, unlocks, pak faults, initialised statics, renamed facts.

Fast unit tests (no ROM, no recomp host): the controller-pak fault lever (`pak fail
OP N [ERR]` script lines, host --pak-fail, the runtime's injection points), the
committed Phase 10 scripts (ghost_pak's gSaveGhostFiles poke, the `pak from
ghost_pak` chain, unlock's developer flag), static_layout (IDO writes no symbol
for a function-local static: the layout from the source, checked on rocky
against IDO 5.3 objects), static_bindings by NAME / FUNCTION:NAME through that
layout, and frontier facts keyed by today's names after a rename. End to end on
the recomp host only (rocky; skipped elsewhere): w29d's SaveServer draft with
INITIALISED function-local statics is EQUIVALENT with `--statics` and DIVERGES
(naming sInitCountdown) without it; ghost_reload reaches draw_ui_element.
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


dc = _load("recomp_p10_draft_check", "tools/recomp/draft_check.py")
rp = dc.rp
oracle = dc.oracle
sl = _load("recomp_p10_static_layout", "tools/recomp/static_layout.py")


# ------------------------------------------------------------------ pak fault lever

def test_parse_pak_fail():
    sc = oracle.parse_script("frames 10\npak fail delete 1\npak fail write 3 6\n1 A\n")
    assert sc["pak_fail"] == ["delete:1:4", "write:3:6"]
    assert oracle.lever_args(sc) == ["--pak-fail", "delete:1:4,write:3:6"]
    sc = oracle.parse_script("frames 10\npoke 1 80100000 1 1\npak fail alloc 2\n")
    assert oracle.lever_args(sc) == ["--poke", "30:80100000:1:1", "--pak-fail", "alloc:2:4"]
    for bad in ("pak fail erase 1", "pak fail delete 0", "pak fail delete x", "pak fail delete 1 0",
                "pak fail delete 1 256", "pak fail delete"):
        with pytest.raises(oracle.OracleError):
            oracle.parse_script("frames 10\n" + bad + "\n")


def test_host_pak_fail_option_and_injection_points():
    src = RECOMP / "runtime" / "src"
    main = (src / "main.cpp").read_text()
    assert '"--pak-fail"' in main and "rush::pak_add_fail(item)" in main
    assert 'v == "from" || v == "fail"' in main          # --press-file tolerates the harness lines
    pak = (src / "pak.cpp").read_text()
    for op in ("delete", "read", "write", "alloc", "state"):
        assert '"%s"' % op in pak
    assert pak.count('injected(') >= 5 and "bool rush::pak_add_fail" in pak
    assert "bool pak_add_fail(const std::string& item);" in (src / "rush_runtime.h").read_text()


def test_script_probe_and_pak_image_pass_the_levers():
    assert "oracle.lever_args(sc)" in (RECOMP / "script_probe.py").read_text()
    o = (RECOMP / "oracle.py").read_text()
    assert '"--pak-write"] + lever_args(sc)' in o and "return args + lever_args(sc)" in o


def test_committed_phase10_scripts():
    gp = oracle.load_script("ghost_pak")
    assert gp["pak_from"] == "saves"
    assert any(x.endswith(":801148E4:1:1") for x in gp["pokes"]) and any(":80152735:1:1" in x for x in gp["pokes"])
    for name in ("ghost_reload", "ghost_resave", "ghost_delete_fail"):
        assert oracle.load_script(name)["pak_from"] == "ghost_pak", name
    assert oracle.load_script("ghost_delete_fail")["pak_fail"] == ["delete:1:4"]
    ul = oracle.load_script("unlock")
    assert ul.get("pak_from") is None and any(x.endswith(":801164C0:1:2") for x in ul["pokes"])
    names = dc.all_scripts()
    for name in ("ghost_pak", "ghost_reload", "ghost_resave", "ghost_delete_fail", "unlock"):
        assert name in names


# ------------------------------------------------------------------ static_layout (IDO: no static symbols)

SRC_T2 = """extern void use(void*);
typedef signed char s8; typedef short s16;
int gInit = 7;
static short fsInit = 5;
static int fsBss;
int gCommon;
extern int gExt;
int fn(int x) {
    static char a = 1;
    static char unused = 4;     /* never referenced: IDO drops it, no gap */
    static short b = 2;
    static char c[3] = {9,9,9};
    static double d = 1.5;
    static void (*fp)(int) = 0;
    static char e;
    static s16 f[5];
    use(&a); use(&b); use(c); use(&d); use(&fp); use(&e); use(f); use(&fsInit); use(&fsBss);
    return 0;
}
int fn2(void) { static s8 z[] = { 0x55, -1 }; use(z); return gInit; }
"""

# .data as IDO 5.3 lays it out (rocky, -O3 -G 0; same rules as the probe objects):
# gInit 0, fsInit 4, a 8, b 0xC, c 0x10, d 0x18, fp 0x20, z 0x24
DATA_T2 = (struct.pack(">i", 7) + struct.pack(">h", 5) + b"\0\0" + b"\x01\0\0\0" + struct.pack(">h", 2) + b"\0\0"
           + b"\x09\x09\x09\0" + b"\0" * 4 + struct.pack(">d", 1.5) + b"\0" * 4 + b"\x55\xff\0\0")


def _elf_t2(data=DATA_T2, ginit_at=0):
    return {"sections": [{"name": ".text", "size": 0x80, "offset": 0}, {"name": ".data", "size": 0x30, "offset": 0x80},
                         {"name": ".bss", "size": 0x20, "offset": 0}],
            "symbols": [{"name": "gInit", "type": 1, "bind": 1, "section": ".data", "value": ginit_at, "size": 4}],
            "_data": bytes(0x80) + data + bytes(0x30 - len(data))}


def test_static_layout_matches_ido():
    objs, problems = sl.layout(SRC_T2, _elf_t2())
    assert problems == []
    got = {o.key(): (o.section, o.offset) for o in objs}
    assert got == {"gInit": (".data", 0), "fsInit": (".data", 4), "fsBss": (".bss", 0), "gCommon": (None, None),
                   "fn:a": (".data", 8), "fn:b": (".data", 0xC), "fn:c": (".data", 0x10), "fn:d": (".data", 0x18),
                   "fn:fp": (".data", 0x20), "fn:e": (".bss", 4), "fn:f": (".bss", 8), "fn2:z": (".data", 0x24)}


def test_static_layout_rejects_a_mismatch():
    objs, problems = sl.layout(SRC_T2, _elf_t2(ginit_at=4))         # a symbol where the layout does not put it
    assert any("gInit at 0x4, computed 0x0" in p for p in problems)
    assert all(o.offset is None for o in objs if o.section == ".data")
    bad = bytearray(DATA_T2)
    bad[0xC + 1] = 7                                               # fn:b's initialiser is 2
    objs, problems = sl.layout(SRC_T2, _elf_t2(bytes(bad)))
    assert any("fn:b+0 holds 0x7, initialiser 0x2" in p for p in problems)
    assert all(o.offset is None for o in objs if o.section == ".data")


def test_static_layout_stops_at_an_unknown_size():
    src = "struct S { int a; };\nint f(void) { static struct S s = {1}; static int k = 3; use(&s); use(&k); }\n"
    objs, problems = sl.layout(src, {"sections": [{"name": ".data", "size": 16, "offset": 0}], "symbols": [],
                                     "_data": bytes(16)})
    assert [o.offset for o in objs] == [None, None] and any("size of f:s unknown" in p for p in problems)


def _hilo_elf(offset):
    """An object whose .text loads a .data static at `offset` (lui/lb pair against the section symbol)."""
    text = struct.pack(">II", 0x3C0E0000, 0x81CE0000 | offset)          # lui t6,0 ; lb t6,offset(t6)
    data_sym = {"name": ".data", "type": 3, "bind": 0, "section": ".data", "value": 0, "size": 0x30}
    return {"sections": [{"name": ".text", "size": 8, "offset": 0}], "_data": text,
            "symbols": [data_sym], "relocs": [{"section": ".text", "offset": 0, "type": rp.R_MIPS_HI16,
                                              "sym": data_sym},
                                             {"section": ".text", "offset": 4, "type": rp.R_MIPS_LO16,
                                              "sym": data_sym}]}


def test_static_bindings_by_name_through_the_layout():
    layout = {(".data", 0xC): ("b", 2, "fn"), (".data", 0x10): ("c", 3, "fn")}
    func_obj = {"value": 0, "size": 8}
    elf = _hilo_elf(0x11)                                                # c[1]
    b, u = rp.static_bindings(elf, "fn", func_obj, None, lambda a: None, {"c": 0x80117434}, layout)
    assert b == {(".data", 0x11): (0x80117435, "manifest")} and u == []
    b, u = rp.static_bindings(elf, "fn", func_obj, None, lambda a: None, {"fn:c": 0x80117434}, layout)
    assert b[(".data", 0x11)][0] == 0x80117435
    b, u = rp.static_bindings(elf, "fn", func_obj, None, lambda a: None, {}, layout)
    assert b == {} and u == [((".data", 0x11), "c")]                     # named in the private list
    b, u = rp.static_bindings(elf, "fn", func_obj, None, lambda a: None, {"c": 0x80117434}, None)
    assert b == {}                                                       # Phase 9: no name, no binding


# ------------------------------------------------------------------ frontier facts after a rename

def test_frontier_facts_follow_renames(tmp_path):
    fr = tmp_path / "frontier.json"
    fr.write_text(json.dumps({"facts": {
        "AdjustSpeed": {"address": "0x800A2680", "register_params": ["a0", "t0"]},
        "func_80096CA8": {"address": "0x80096CA8", "register_params": ["s8", "t0"],
                          "unsaved_callee_regs": ["s0", "s1"]},
        "drawMain_old": {"address": "0x8010221C", "unsaved_callee_regs": ["s0"]},
        "kept": {"address": "0x80099999"}}}))
    syms = tmp_path / "symbols.json"
    syms.write_text(json.dumps({"symbols": {"pak_delete_file_retry": "0x800A2680", "SetupWaveModel": "0x80096CA8",
                                            "drawMain": "0x8010221C"}}))
    facts, renamed = rp.frontier_facts(fr, syms)
    assert renamed == {"AdjustSpeed": "pak_delete_file_retry", "func_80096CA8": "SetupWaveModel",
                       "drawMain_old": "drawMain"}
    assert set(facts) == {"pak_delete_file_retry", "SetupWaveModel", "drawMain", "kept"}
    assert facts["SetupWaveModel"]["register_params"] == ["s8", "t0"]


def test_fact_readers_use_the_remap():
    src = (RECOMP / "draft_check.py").read_text()
    assert src.count("rp.frontier_facts(frontier)[0]") == 2
    assert "dc.rp.frontier_facts(frontier)[0]" in (RECOMP / "regmap_verify.py").read_text()
    assert "facts, renamed = frontier_facts(frontier)" in (RECOMP / "replace.py").read_text()


# ------------------------------------------------------------------ end to end (recomp host only)

def _on_recomp_host():
    p = dc.paths()
    return all(Path(p[k]).exists() for k in ("rom", "n64recomp", "rsprecomp", "runtime", "ido53"))


rocky = pytest.mark.skipif(not _on_recomp_host(), reason="recomp host only (ROM, N64Recomp, IDO toolkits)")
SAVESERVER = REPO / "cloud" / "work" / "frontier" / "w29d" / "func_800C813C" / "best.c"


@rocky
def test_e2e_initialised_statics_bind_by_name():
    base = [sys.executable, "-m", "tools.recomp.draft_check", "SaveServer", str(SAVESERVER), "--scripts", "saves"]
    p = subprocess.run(base + ["--statics", "sInitCountdown=0x8011742C,sLastPak=0x80117430,"
                               "sReadingPak=0x80117434"], cwd=str(REPO), capture_output=True, text=True)
    assert p.returncode == 0, p.stdout + p.stderr
    assert "VERDICT: EQUIVALENT over 1568 calls" in p.stdout
    p = subprocess.run(base, cwd=str(REPO), capture_output=True, text=True)
    assert p.returncode == 1, p.stdout + p.stderr
    assert ".data+0x0 sInitCountdown" in p.stdout and "word 0x8011742C: retail 03000000->02000000" in p.stdout


@rocky
def test_e2e_ghost_reload_reaches_the_ghost_branch():
    p = subprocess.run([sys.executable, "-m", "tools.recomp.script_probe", "ghost_reload",
                        "--functions", "draw_ui_element,func_800CBF2C", "--frames", "700"],
                       cwd=str(REPO), capture_output=True, text=True)
    assert p.returncode == 0, p.stdout + p.stderr
    assert "reached draw_ui_element" in p.stdout and "reached func_800CBF2C" in p.stdout


def test_function_pointer_parameters():
    text = "void control_settings(s32 row, s8 *sel1, s32 (*poll)(void))\n{\n}\n"
    p = rp.find_prototype(text, "control_settings")
    assert p == "void control_settings(s32 row, s8 *sel1, s32 (*poll)(void))"
    assert rp.parse_prototype(p)[2] == [("s32", "row"), ("s8 *", "sel1"), ("void *", "poll")]


def test_lane_names_follow_renames():
    stale = {"func_800C813C": "SaveServer", "AdjustSpeed": "pak_delete_file_retry", "func_80096CA8": "SetupWaveModel"}
    name, extra, done = dc.current_names("func_800C813C,func_800C7EC8",
                                         ["--params", "func_800C813C:a=s0", "--callee",
                                          "void func_80096CA8(s32 slot, s32 first)=s8,t0", "--scripts", "saves"], stale)
    assert name == "SaveServer,func_800C7EC8"
    assert extra == ["--params", "SaveServer:a=s0", "--callee", "void SetupWaveModel(s32 slot, s32 first)=s8,t0",
                     "--scripts", "saves"]
    assert done == [("func_80096CA8", "SetupWaveModel"), ("func_800C813C", "SaveServer")]
