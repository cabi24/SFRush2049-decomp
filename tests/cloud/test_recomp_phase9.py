"""Recomp Phase 9 (lane w29r): host faults, static helpers, argument homes.

Fast unit tests (no ROM, no recomp host): replace.code_ranges covers a static
helper's code (IDO writes no symbol for it), the host's `[rush] FAULT` lines
are parsed and mapped back to the N64 instruction, a truncated call log names
the faulting call, oracle.run_hosts tolerates a signal death only where asked,
draft_check words a fault as a divergence, the runtime pieces exist (fault.cpp,
arg-home exclusion, call log v3). End to end on the recomp host only (rocky;
skipped elsewhere): a render_helper draft that reads a wild N64 address is
reported as `draft FAULTED` with its PC, and w28c's func_8010AEAC draft with
static helpers (the -11 crash) is EQUIVALENT on options_menus.
"""
import importlib.util
import signal
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


dc = _load("recomp_p9_draft_check", "tools/recomp/draft_check.py")
rp = dc.rp
oracle = dc.oracle


# ------------------------------------------------------------------ static helpers (the AEAC crash)

def test_code_ranges_cover_static_helpers():
    # object: .text 0xD50 bytes, one named function at +0x6E0 (IDO: static helpers have no symbol)
    obj = {"sections": [{"name": ".text", "size": 0xD50}, {"name": ".rodata", "size": 0xA0}],
           "symbols": [{"name": "func_8010AEAC", "type": 2, "section": ".text", "value": 0x6E0, "size": 0x66C}]}
    linked = {"symbols": [{"name": "func_8010AEAC", "type": 2, "value": 0x808006E0, "size": 0x66C}]}
    named = [{"name": "func_8010AEAC", "vram": 0x808006E0, "size": 0x66C}]
    assert rp.code_ranges(named, obj, linked) == [(0x80800000, 0xD50)]


def test_static_helper_call_is_retargeted_to_the_shim():
    # a jal in the static helper (before the named function) must reach the shim too
    jal = (3 << 26) | ((0x800B4200 >> 2) & 0x03FFFFFF)
    raw = bytearray(0x20)
    raw[0x4:0x8] = jal.to_bytes(4, "big")       # static helper
    raw[0x14:0x18] = jal.to_bytes(4, "big")     # named function at +0x10
    named = [{"name": "f", "vram": 0x80800010, "size": 0x10}]
    obj = {"sections": [{"name": ".text", "size": 0x20}],
           "symbols": [{"name": "f", "type": 2, "section": ".text", "value": 0x10, "size": 0x10}]}
    linked = {"symbols": [{"name": "f", "type": 2, "value": 0x80800010, "size": 0x10}]}
    _, n_old = rp.retarget_calls(bytes(raw), 0x80800000, 0x800B4200, 0x80800100,
                                 [(f["vram"], f["size"]) for f in named])
    out, n_new = rp.retarget_calls(bytes(raw), 0x80800000, 0x800B4200, 0x80800100,
                                   rp.code_ranges(named, obj, linked))
    assert (n_old, n_new) == (1, 2)
    assert int.from_bytes(out[4:8], "big") & 0x03FFFFFF == (0x80800100 >> 2) & 0x03FFFFFF


# ------------------------------------------------------------------ fault report parsing

ERR = """[rush] t=0.08s replacement image loaded: 1024 B at 0x80800000
[rush] FAULT signal=11 (SIGSEGV) access=read n64=0x01010000 host=0x7ffef6210000 frame=4 thread=game7
[rush] FAULT active=draft=ido:func_800C813C fn=0 seq=3 depth=1
[rush] FAULT pc=0x662898 exe_base=0x400000 bt=0x401000,0x7ffff7a3e730,0x662898,0x5052b9
[rush] FAULT regs at=00000000 v0=FFFFFFFF s2=01010000 sp=80031C90 ra=00000000
"""


def test_parse_fault_fields():
    f = oracle.parse_fault(ERR)
    assert f["signal"] == "11" and f["signal_name"] == "SIGSEGV" and f["access"] == "read"
    assert f["n64"] == "0x01010000" and f["frame"] == "4" and f["thread"] == "game7"
    assert f["active"] == "draft=ido:func_800C813C" and f["seq"] == "3"
    assert f["regs"]["s2"] == "01010000"
    assert oracle.parse_fault("[rush] nothing\n") is None


def test_n64_line_from_recompiled_c(tmp_path):
    src = tmp_path / "RecompiledFuncs" / "funcs_40.c"
    src.parent.mkdir()
    src.write_text("\n".join([
        "RECOMP_FUNC void repl__draft__func_800C813C(uint8_t* rdram, recomp_context* ctx) {",
        "    // 0x80800000: addiu       $sp, $sp, -0x48",
        "    ctx->r29 = ADD32(ctx->r29, -0X48);",
        "    // 0x8080013C: lw          $v0, 0x0($s2)",
        "    ctx->r2 = MEM_W(ctx->r18, 0X0);",
        "}"]) + "\n")
    pc, insn, func, start = oracle._n64_at(str(src), 5)
    assert (pc, insn, func, start) == (0x8080013C, "lw $v0, 0x0($s2)", "repl__draft__func_800C813C", 0x80800000)
    fault = dict(oracle.parse_fault(ERR), n64_pc=pc, insn=insn, n64_function=func, n64_offset=pc - start,
                 n64_chain=[{"n64_function": func, "n64_pc": pc}, {"n64_function": "func_800C813C", "n64_pc": 0x800FD3F0}],
                 call={"seq": 3, "name": "func_800C813C (draft)", "fn_call": 3, "frame": 4, "inside": []})
    lines = oracle.fault_lines(fault)
    assert lines[0] == "  fault: SIGSEGV, N64 read of 0x01010000 at frame 4 (thread game7)"
    assert lines[1] == "  N64 PC 0x8080013C `lw $v0, 0x0($s2)` = func_800C813C+0x13C (draft)"
    assert "s2=01010000" in lines[2]
    assert any(ln.startswith("  faulting call: func_800C813C (draft) call #3") for ln in lines)


def test_open_call_of_a_truncated_log():
    log = ["# rush2049 call log v3: effect\n", "# fn 0 draft=ido:func_800C813C args=- ret=-\n",
           "c 1 2 0 -\n", "C 1 2 0 r=- w=2:00\n", "c 2 3 0 -\n", "C 2 3 0 r=- w=2:00\n", "c 3 4 0 -\n"]
    oc = oracle.open_call(log)
    assert (oc["seq"], oc["frame"], oc["fn_call"], oc["name"], oc["calls"]) == (3, 4, 3, "func_800C813C (draft)", 3)
    assert oracle.open_call(log[:4]) == {"calls": 1}


def test_run_hosts_tolerates_only_a_signal_death_where_asked(tmp_path):
    die = [sys.executable, "-c", "import os, signal; os.kill(os.getpid(), signal.SIGSEGV)"]
    ok = [sys.executable, "-c", "pass"]
    rcs = oracle.run_hosts([(ok, tmp_path / "a.err"), (die, tmp_path / "b.err")], tolerate=[tmp_path / "b.err"])
    assert rcs[str(tmp_path / "b.err")] == -signal.SIGSEGV and rcs[str(tmp_path / "a.err")] == 0
    with pytest.raises(oracle.OracleError):
        oracle.run_hosts([(die, tmp_path / "c.err")])
    fail = [sys.executable, "-c", "raise SystemExit(2)"]
    with pytest.raises(oracle.OracleError):      # an ordinary error exit is never tolerated
        oracle.run_hosts([(fail, tmp_path / "d.err")], tolerate=[tmp_path / "d.err"])


def test_draft_check_words_a_fault_as_divergence():
    crash = dict(oracle.parse_fault(ERR), n64_pc=0x8080013C, insn="lw $v0, 0x0($s2)",
                 n64_function="repl__draft__func_800C813C", n64_offset=0x13C, n64_chain=[])
    res = {"script": "saves", "crash": crash,
           "first": {"what": "crash", "frame": 4, "name": "func_800C813C (draft)", "fn_call": 3, "seq": 3,
                     "fields": []}}
    lines = dc.divergence_lines(res, "func_800C813C")
    assert lines[0] == ("FIRST DIVERGENCE: script saves, frame 4, func_800C813C (draft) call #3 (global seq 3): "
                        "the draft FAULTED (host signal; retail did not)")
    assert any("N64 read of 0x01010000" in ln for ln in lines)
    # an earlier differing call comes first; the fault follows
    res["first"] = {"what": "effect", "frame": 2, "name": "func_800C813C (draft)", "fn_call": 1, "seq": 1,
                    "fields": []}
    lines = dc.divergence_lines(res, "func_800C813C")
    assert "memory written differs" in lines[0] and any("later the draft FAULTED" in ln for ln in lines)


def test_runtime_has_fault_report_and_arg_homes():
    src = RECOMP / "runtime" / "src"
    assert "src/fault.cpp" in (RECOMP / "runtime" / "CMakeLists.txt").read_text()
    fault = (src / "fault.cpp").read_text()
    assert "[rush] FAULT signal=" in fault and "rush_fault_enter" in fault
    cap = (src / "capture.cpp").read_text()
    assert "off < floor + 16" in cap and "call log v3" in cap
    assert cap.count("rush_fault_enter(rdram, ctx, fn") == 2      # both RUSH_CAPTURE variants
    assert "rush::fault_setup();" in (src / "main.cpp").read_text()


def test_duplicate_scripts_are_run_once():
    assert "list(dict.fromkeys(" in (RECOMP / "draft_check.py").read_text()


# ------------------------------------------------------------------ end to end (recomp host only)

def _on_recomp_host():
    p = dc.paths()
    return all(Path(p[k]).exists() for k in ("rom", "n64recomp", "rsprecomp", "runtime", "ido53"))


rocky = pytest.mark.skipif(not _on_recomp_host(), reason="recomp host only (ROM, N64Recomp, IDO toolkits)")


@rocky
def test_e2e_wild_read_is_reported_not_an_error(tmp_path):
    text = (REPO / "src" / "blob" / "render_helper.c").read_text()
    old = "    D_80114744 = value;"
    assert old in text
    bad = tmp_path / "render_helper_wild.c"     # every call reads a wild KUSEG address (no RDRAM there)
    bad.write_text(text.replace(old, old + "\n    D_80149B88 |= *(volatile u32 *)0x01010000;", 1))
    p = subprocess.run([sys.executable, "-m", "tools.recomp.draft_check", "render_helper", str(bad),
                        "--scripts", "race", "--frames", "120"], cwd=str(REPO), capture_output=True, text=True)
    assert p.returncode == 1, p.stdout + p.stderr
    assert "draft FAULTED" in p.stdout and "N64 read of 0x01010000" in p.stdout
    assert "= render_helper+0x" in p.stdout and "call #1 (global seq 1)" in p.stdout


@rocky
def test_e2e_static_helper_draft_no_longer_crashes():
    src = REPO / "cloud" / "work" / "frontier" / "w28c" / "func_8010AEAC" / "dc_static.c"
    p = subprocess.run([sys.executable, "-m", "tools.recomp.draft_check", "func_8010AEAC", str(src),
                        "--scripts", "options_menus"], cwd=str(REPO), capture_output=True, text=True)
    assert p.returncode == 0, p.stdout + p.stderr
    assert "ipa shim slot_state_setup: 4 call site(s)" in p.stdout
    assert "VERDICT: EQUIVALENT over 960 calls" in p.stdout
