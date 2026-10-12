"""cloud/work/frontier/tools/trace71: the pure-Python patch scripts and the sum71.sh decoder.

The traced IDO 7.1 passes themselves are built and gated on the builder (install71.sh, fidelity71.sh); these tests
check that the source patches touch exactly their anchors, stay inert without their switches, and that the trace
summary decodes webs the way the README says."""
import importlib.util
import shutil
import subprocess
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]
TK = REPO / "cloud/work/frontier/tools/trace71"


def load(name):
    spec = importlib.util.spec_from_file_location(name, TK / "patches" / f"{name}.py")
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


UOPT_ANCHORS = [
    "static void f_compute_save(uint8_t *mem, uint32_t sp, uint32_t a0) {\n",
    "uint32_t a3 = 0;\nL469280:\n//globalcolor:",
    "L4693ac:\na1 = 0x1001c7a0;\n",
    "L4698f4:\nf0.w[0] = MEM_U32(s5 + 48);\n",
    "L469a84:\ncf = f2.f[0] < f20.f[0];",
    "L469c54:\n",
    "cf = f6.f[0] <= f20.f[0];\n//nop;\nif (!cf) {",
    "L469e84:\nt5 = MEM_U32(sp + 220);",
    "L46a450:\ncf = f2.f[0] < f20.f[0];",
    "L46a640:\n",
    "cf = f20.f[0] < f10.f[0];\n//nop;\nif (!cf) {",
    "L46a82c:\nt8 = MEM_U32(sp + 220);",
    "L468a84:\nif (t6 != zero) {",
    "L468f00:\n",
    "L4690cc:\nt6 = 0x1001c358;\n",
    "L469160:\nf0.f[0] = f20.f[0];\n",
]


def fake_uopt():
    return "\n/* filler */\n".join(["#include \"header.h\"\n"] + UOPT_ANCHORS) + "\n"


def test_uopt_profile_hooks_every_anchor_once():
    mod = load("instrument71_uopt")
    out = mod.instrument(fake_uopt())
    assert mod.MARK in out
    for rec in ("p1cand", "p1dec", "p1color", "p2dec", "p2color", "calleecost", "procname", "[CUP] unit",
                "[CUP] transit", "[CUP] total"):
        assert rec in out, rec
    # the 5.3 globals are retargeted, the 7.1 ones used
    assert "0x10021c74" not in out and "0x1001eb70" not in out
    assert "MEM_U32(0x1001c530)" in out and "MEM_U8(0x1001c4d0)" in out
    # CDX_FN may carry a force without CDX_PROC
    assert 'dkwb_cdx_proc < 0 && !getenv("CDX_FN")' in out
    with pytest.raises(SystemExit):
        mod.instrument(out)


def test_uopt_profile_refuses_a_missing_or_duplicated_anchor():
    mod = load("instrument71_uopt")
    src = fake_uopt()
    with pytest.raises(SystemExit):
        mod.instrument(src.replace("L46a640:\n", "L46a641:\n"))
    with pytest.raises(SystemExit):
        mod.instrument(src + "L469c54:\n")


def test_spill_hooks_are_gated_and_anchored():
    mod = load("spill71")
    area = "".join("t0 = 0x1001c31c;\nMEM_U32(t0 + 0) = v1;\n" for _ in range(5))
    src = ("#include \"header.h\"\nstatic void f_spilltemps(uint8_t *mem, uint32_t sp) {\n" + area
           + "L465f30:\nif (v0 == 0) {\nL466320:\na2 = MEM_U32(sp + 160);\nL466148:\nif (v0 == 0) {\nL466438:\n}\n")
    out = mod.hooks(mod.area(src))
    assert out.count("if (SPON)") == 5 + 2 and out.count("if (SPON && v0 != 0)") == 2
    assert 'AREA %s:%d proc=%s area=%d' in out and '"f_spilltemps", ' in out
    for rec in ("SPILLTEMP proc=", "HOME proc=", "CONFL proc=", "SPILLWEB proc="):
        assert rec in out, rec
    with pytest.raises(SystemExit):
        mod.hooks(src.replace("L466438:\n", ""))


def test_ugen_globals_retargeted(tmp_path):
    f = tmp_path / "ugen.c"
    f.write_text("#define DKWB_IBUFFER_FORWARD_CURSOR 0x10018e70u\n#define DKWB_CURRENT_SOURCE_LINE 0x10018e00u\n"
                 "#define DKWB_IBUFFER_BACKWARD_CURSOR 0x10018e78u\n")
    subprocess.run(["python3", str(TK / "patches/ugen71_addrs.py"), str(f)], check=True, capture_output=True)
    text = f.read_text()
    assert "0x10021238u" in text and "0x10021240u" in text and "0x10021220u" in text and "0x10018e" not in text


TRACE = """\
[CDX] p1dec phase=p1 proc=0 web=3 sym=3 class=1 save=3.000000 nocs=2 totalsave=6.000000 bestcost=4.000000 bestcolor=15 bestreg=s1 forbidden0=0x5b820000 forbidden1=0x00000000 regsleft=13 numintf=8 available0=0 available1=0 allcallersave=0 taken1=-1 taken2=-1 decision=color forced=-2
[CDX] webdetail phase=p1 proc=0 role=target web=3 sym=3 type=3 dtype=8 table=1 chain=0 exprtable=1 exprchain=1 bb=1 line=2 raw10=0xfffffff8 raw14=0x00001902 raw18=0x04010000 raw20=0x00000000
[CDX] p1color phase=p1 proc=0 web=3 sym=3 color=15 reg=s1 forced=-2
[CDX] p1dec phase=p1 proc=0 web=7 sym=7 class=1 save=1.0 nocs=1 totalsave=1.0 bestcost=0.0 bestcolor=1 bestreg=v0 forbidden0=0 forbidden1=0 regsleft=1 numintf=1 available0=0 available1=0 allcallersave=0 taken1=-1 taken2=-1 decision=color forced=-2
[CDX] webdetail phase=p1 proc=0 role=target web=7 sym=7 type=2 dtype=6 table=0 chain=0 exprtable=-1 exprchain=-1 bb=-1 line=-1 raw10=0x00002000 raw14=0 raw18=0 raw20=0
[CDX] p1color phase=p1 proc=0 web=7 sym=7 color=2 reg=v1 forced=2
"""


@pytest.mark.skipif(shutil.which("awk") is None, reason="needs awk")
def test_sum71_decodes_variables_constants_and_forces():
    out = subprocess.run(["sh", str(TK / "sum71.sh"), "-"], input=TRACE, capture_output=True, text=True,
                         check=True).stdout.splitlines()
    assert len(out) == 2
    assert "-> s1" in out[0] and "var@-8/4" in out[0] and "tot=6.000000" in out[0]
    assert "-> v1(forced)" in out[1] and "const 8192" in out[1]


@pytest.mark.skipif(shutil.which("awk") is None, reason="needs awk")
def test_sum71_names_variable_webs(tmp_path):
    names = tmp_path / "vars"
    names.write_text("-4 state Local\n-8 count Local\n0 configuration Param\n")
    out = subprocess.run(["sh", str(TK / "sum71.sh"), "-", str(names)], input=TRACE, capture_output=True,
                         text=True, check=True).stdout.splitlines()
    assert "var@-8/4 count" in out[0]


def test_mdvars_reads_one_procedure_scope():
    spec = importlib.util.spec_from_file_location("mdvars", TK / "lib" / "mdvars.py")
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    syms = [("Proc", 1, 0, "other"), ("Local", 5, -4, "x"), ("End", 1, 0, "other"),
            ("Proc", 1, 0, "fn"), ("Param", 5, 0, "a"), ("Block", 1, 0, ""), ("Local", 5, -4, "i"),
            ("Local", 5, -6, "id"), ("End", 1, 0, ""), ("End", 1, 0, "fn"), ("Local", 5, -4, "after")]
    assert mod.proc_vars(syms, "fn") == [(0, "a", "Param"), (-4, "i", "Local"), (-6, "id", "Local")]
