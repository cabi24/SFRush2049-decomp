"""w24t: replace.py refuses (or, with --allow-ipa, warns about) replacement
bodies that call IDO -O3 register-parameter callees; tools/recomp/ipa_detect.py
reads those registers from the retail words. No ROM or compiler needed; the
whole-image calibration runs when build/game_code.bin is present."""
import importlib.util
import json
import struct
import sys
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(REPO / "tools" / "recomp"))


def _load(name, rel):
    spec = importlib.util.spec_from_file_location(name, REPO / rel)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


rp = _load("recomp_replace_ipa", "tools/recomp/replace.py")
d = _load("recomp_ipa_detect", "tools/recomp/ipa_detect.py")

# --- hand-assembled MIPS II words -------------------------------------------
NOP = 0


def I(op, rs, rt, imm):
    return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)


def R(rs, rt, rd, fn, sa=0):
    return (rs << 21) | (rt << 16) | (rd << 11) | (sa << 6) | fn


def COP1(fmt, ft, fs, fd, fn):
    return (0x11 << 26) | (fmt << 21) | (ft << 16) | (fs << 11) | (fd << 6) | fn


ADDIU_SP = I(9, 29, 29, -24)
JR_RA = R(31, 0, 0, 8)
SDC1_F20 = I(0x3D, 29, 20, 16)          # save of f20/f21
SW_S0 = I(0x2B, 29, 16, 8)             # save of s0
ADD_S_F4_F22_F24 = COP1(0x10, 24, 22, 4, 0)
C_EQ_S_F22_F20 = COP1(0x10, 20, 22, 0, 0x32)
ADD_S_F0_F12_F14 = COP1(0x10, 14, 12, 0, 0)
LWC1_F20 = I(0x31, 29, 20, 0)
ADDU_V0_S0_A0 = R(16, 4, 2, 0x21)
JAL = (3 << 26) | 0x24000
BNEZ_T0 = I(5, 8, 0, 2)


def test_fp_register_parameters_are_named():
    body = [ADDIU_SP, SDC1_F20, LWC1_F20, C_EQ_S_F22_F20, NOP, ADD_S_F4_F22_F24, JR_RA, NOP]
    assert d.register_params(body) == ["f22", "f24"]


def test_o32_body_and_prologue_saves_are_not_parameters():
    assert d.register_params([ADDIU_SP, SDC1_F20, SW_S0, ADD_S_F0_F12_F14, JR_RA, NOP]) == []


def test_callee_saved_gpr_parameter():
    assert d.register_params([ADDU_V0_S0_A0, JR_RA, NOP]) == ["s0"]


def test_path_sensitive_read_before_write():
    # beqz a0,L ; nop ; addu v0,a0,a0 ; L: addu v1,v0,a0 ; jr ra ; nop
    beqz = I(4, 4, 0, 2)
    body = [beqz, NOP, R(4, 4, 2, 0x21), R(2, 4, 3, 0x21), JR_RA, NOP]
    assert d.register_params(body) == ["v0"]
    # unconditional `b` skips the write on every path: still a read of v0
    b = I(4, 0, 0, 2)
    assert d.register_params([b, NOP, R(4, 4, 2, 0x21), R(2, 4, 3, 0x21), JR_RA, NOP]) == ["v0"]
    # written before the branch on every path: no parameter
    assert d.register_params([R(4, 4, 2, 0x21), beqz, NOP, R(4, 4, 2, 0x21),
                              R(2, 4, 3, 0x21), JR_RA, NOP]) == []


def test_temps_read_after_a_call_belong_to_the_callee():
    assert d.register_params([JAL, NOP, BNEZ_T0, NOP, JR_RA, NOP]) == []


def test_lwl_lwr_pair_is_a_write():
    lwl, lwr = I(0x22, 4, 1, 0), I(0x26, 4, 1, 3)
    assert d.register_params([lwl, lwr, I(0x2A, 29, 1, 0), JR_RA, NOP]) == []


# --- object side: which callees a replacement reaches -------------------------

def _elf(callees, defined=()):
    syms = [{"name": n, "shndx": 0, "type": 0, "bind": 1, "section": None, "value": 0} for n in callees] + \
           [{"name": n, "shndx": 1, "type": 2, "bind": 0, "section": ".text", "value": 0} for n in defined]
    relocs = [{"type": 4, "sym": s, "section": ".text", "offset": 0} for s in syms] + \
             [{"type": 5, "sym": {"name": "D_80100000", "shndx": 0}, "section": ".text", "offset": 4}]
    return {"symbols": syms, "relocs": relocs, "sections": [], "referenced": set(callees) | set(defined)}


BASE = 0x80090000
IPA_BODY = [ADDIU_SP, SDC1_F20, LWC1_F20, C_EQ_S_F22_F20, NOP, ADD_S_F4_F22_F24, JR_RA, NOP]
PLAIN_BODY = [ADD_S_F0_F12_F14, JR_RA, NOP, NOP]


def _image():
    words = IPA_BODY + PLAIN_BODY
    return {"game": (BASE, struct.pack(">%dI" % len(words), *words))}


FUNCTIONS = {"game": [{"name": "entity_transform_calc", "vram": BASE, "size": 4 * len(IPA_BODY)},
                      {"name": "plain_fn", "vram": BASE + 4 * len(IPA_BODY), "size": 16}]}
TABLE = {"entity_transform_calc": BASE, "plain_fn": BASE + 32}


def test_called_functions_are_jal_targets_only():
    assert d.called_functions(_elf(["entity_transform_calc", "plain_fn"], ["local"])) == \
        ["entity_transform_calc", "plain_fn"]


def test_hazards_from_retail_words_and_a_scan_table():
    elf = _elf(["entity_transform_calc", "plain_fn"])
    assert rp.ipa_hazards(elf, TABLE, FUNCTIONS, _image()) == \
        {"entity_transform_calc": "register parameters f22,f24"}
    scan = {"plain_fn": "register parameters t9", "not_called": "register parameters s0"}
    assert set(rp.ipa_hazards(elf, TABLE, FUNCTIONS, _image(), scan)) == {"entity_transform_calc", "plain_fn"}


def test_a_static_copy_of_the_callee_is_not_a_hazard():
    elf = _elf(["plain_fn"], defined=["entity_transform_calc"])
    assert rp.ipa_hazards(elf, TABLE, FUNCTIONS, _image()) == {}


# --- build(): refuse by default, warn with allow_ipa ---------------------------

def _build(tmp_path, monkeypatch, entry_extra=None, **kwargs):
    src = tmp_path / "draft.c"
    src.write_text("void f(void) {}\n")
    monkeypatch.setattr(rp, "read_elf", lambda data: _elf(["entity_transform_calc"]))

    def run(cmd, **kw):
        if "-o" in cmd and cmd[0].endswith("/cc"):
            Path(cmd[cmd.index("-o") + 1]).write_bytes(b"obj")
        return type("P", (), {"returncode": 1 if cmd[0] == "ld.lld" else 0, "stdout": "", "stderr": "ld"})()

    entry = dict({"function": "f", "population": "game", "mode": "ido",
                  "source": str(src.relative_to(tmp_path))}, **(entry_extra or {}))
    return rp.build([("draft_f", entry)], FUNCTIONS, {}, tmp_path / "w", {"ido5.3": "/tk"},
                    repo=tmp_path, run=run, images=_image(), **kwargs)


def test_build_refuses_ipa_callee_without_allow(tmp_path, monkeypatch):
    with pytest.raises(rp.ReplaceError, match="IPA register-parameter.*entity_transform_calc.*f22,f24.*--allow-ipa"):
        _build(tmp_path, monkeypatch)


@pytest.mark.parametrize("how", ["flag", "entry"])
def test_allow_ipa_warns_and_builds(tmp_path, monkeypatch, how):
    warned = []
    kwargs = {"allow_ipa": True} if how == "flag" else {}
    extra = {"allow_ipa": True} if how == "entry" else None
    # the fake ld fails: reaching it proves the IPA gate let the build through
    with pytest.raises(rp.ReplaceError, match="ld.lld failed"):
        _build(tmp_path, monkeypatch, extra, warn=warned.append, **kwargs)
    assert len(warned) == 1 and "WARNING draft_f" in warned[0] and "entity_transform_calc" in warned[0]


def test_gen_config_has_allow_ipa_flag():
    text = (REPO / "tools/recomp/gen_config.py").read_text()
    assert '"--allow-ipa"' in text and "allow_ipa=args.allow_ipa" in text


# --- calibration on the retail image (when present) --------------------------

GAME_BIN = REPO / "build" / "game_code.bin"


@pytest.mark.skipif(not GAME_BIN.is_file(), reason="needs build/game_code.bin (make extract)")
def test_calibration_on_the_game_image():
    gc = _load("recomp_gen_config_ipa", "tools/recomp/gen_config.py")
    funcs = {"game": gc.load_game_functions(REPO / "asm/us/blob")}
    found = d.scan(funcs, {"game": (0x80086A50, GAME_BIN.read_bytes())})
    assert found["entity_transform_calc"] == ["f22", "f24", "f26", "f28"]
    assert found["audio_effect_setup"] == ["s0"]
    lock = json.loads((REPO / "blob_matched.lock.json").read_text())
    standalone = [t for t, e in lock.items() if not e.get("group")]
    assert [t for t in standalone if t in found] == []
