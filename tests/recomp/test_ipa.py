"""Phase 4 (w24r): IPA-aware replacement build (tools/recomp/replace.py ipa_*).
The shims are executed on a small MIPS interpreter: the callee must see the
o32 arguments in its register convention, and the o32 caller must get every
callee-saved register (and ra/sp) back."""
import importlib.util
import json
import struct
import sys
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]


def _load(name, rel):
    spec = importlib.util.spec_from_file_location(name, REPO / rel)
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


rp = _load("recomp_replace_ipa", "tools/recomp/replace.py")


class Cpu:
    def __init__(self):
        self.r = [0] * 32
        self.f = [0] * 32
        self.mem = {}

    def step(self, w):
        op = w >> 26
        rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        imm = w & 0xFFFF
        simm = imm - 0x10000 if imm & 0x8000 else imm
        if w == 0:
            return None
        if op == 9:
            if rt:
                self.r[rt] = (self.r[rs] + simm) & 0xFFFFFFFF
        elif op == 0x2B:
            self.mem[(self.r[rs] + simm) & 0xFFFFFFFF] = self.r[rt]
        elif op == 0x23:
            self.r[rt] = self.mem[(self.r[rs] + simm) & 0xFFFFFFFF]
        elif op == 0x39:
            self.mem[(self.r[rs] + simm) & 0xFFFFFFFF] = self.f[rt]
        elif op == 0x31:
            self.f[rt] = self.mem[(self.r[rs] + simm) & 0xFFFFFFFF]
        elif op == 0x11 and rs == 4:
            self.f[rd] = self.r[rt]
        elif op == 0x11 and rs == 0:
            self.r[rt] = self.f[rd]
        elif op == 0x11 and rs == 16 and (w & 63) == 6:
            self.f[(w >> 6) & 31] = self.f[rd]
        elif op == 0 and (w & 63) == 0x25:
            self.r[rd] = self.r[rs] | self.r[rt]
        elif op == 3:
            return ("jal", (w & 0x03FFFFFF) << 2)
        elif op == 0 and (w & 63) == 8:
            return ("jr", rs)
        else:
            raise AssertionError("unexpected word %08X" % w)
        return None


def run_shim(words, cpu, callee):
    """Run until the shim returns; callee(cpu) runs at the jal (after its delay slot)."""
    pc, pending = 0, None
    for _ in range(1000):
        ev = cpu.step(words[pc])
        pc += 1
        if pending:
            kind, val = pending
            pending = None
            if kind == "jal":
                callee(cpu)
            else:
                return
        if ev:
            pending = ev
    raise AssertionError("shim did not return")


def _fresh():
    cpu = Cpu()
    for i in range(1, 32):
        cpu.r[i] = 0x1000 + i
        cpu.f[i] = 0x2000 + i
    cpu.r[29] = 0x80300000
    cpu.r[31] = 0x8009ABCD
    return cpu


def test_outbound_shim_moves_o32_floats_into_f22_f28_and_restores():
    words = rp.ipa_shim(0x80091CA4, "void entity_transform_calc(s32 h, f32 x, f32 y, f32 z, f32 w)",
                        ["a0", "f22", "f24", "f26", "f28"], ["f20"], "out")
    cpu = _fresh()
    cpu.r[4], cpu.r[5], cpu.r[6], cpu.r[7] = 7, 0x3F800000, 0, 0xC0000000
    cpu.mem[0x80300000 + 16] = 0x3F000000                    # 5th argument at the caller's sp+16
    before = (list(cpu.r), list(cpu.f))
    seen = {}

    def callee(c):
        seen.update(h=c.r[4], x=c.f[22], y=c.f[24], z=c.f[26], w=c.f[28])
        for k in (20, 21, 22, 23, 24, 26, 28):               # the callee may clobber its parameters and f20
            c.f[k] = 0xDEAD
        c.r[2] = 0x55
    run_shim(words, cpu, callee)
    assert seen == {"h": 7, "x": 0x3F800000, "y": 0, "z": 0xC0000000, "w": 0x3F000000}
    assert cpu.r[29] == before[0][29] and cpu.r[31] == before[0][31] and cpu.r[2] == 0x55
    for k in range(20, 32):
        assert cpu.f[k] == before[1][k], "f%d not restored" % k
    for k in range(16, 24):
        assert cpu.r[k] == before[0][k]
    assert words[-2] == 0x03E00008 and (words[-1] >> 26) == 9   # jr ra; addiu sp (delay slot)


def test_outbound_shim_s0_parameter_and_unsaved_clobbers():
    words = rp.ipa_shim(0x80095800, "void audio_effect_setup(void *effect)", ["s0"], ["s1", "s2"], "out")
    cpu = _fresh()
    cpu.r[4] = 0x80123456
    before = list(cpu.r)
    seen = {}

    def callee(c):
        seen["s0"] = c.r[16]
        c.r[16], c.r[17], c.r[18] = 1, 2, 3
    run_shim(words, cpu, callee)
    assert seen["s0"] == 0x80123456
    assert cpu.r[16:19] == before[16:19]


def test_inbound_shim_gives_an_o32_body_its_arguments():
    words = rp.ipa_shim(0x80800000, "void f(s32 h, f32 x, f32 y, f32 z, f32 w)",
                        ["a0", "f22", "f24", "f26", "f28"], (), "in")
    cpu = _fresh()
    cpu.r[4] = 9
    cpu.f[22], cpu.f[24], cpu.f[26], cpu.f[28] = 1, 2, 3, 4
    before = list(cpu.r)
    seen = {}

    def body(c):
        seen.update(a0=c.r[4], a1=c.r[5], a2=c.r[6], a3=c.r[7], s16=c.mem[(c.r[29] + 16) & 0xFFFFFFFF])
        for k in list(range(4, 16)) + [24, 25]:              # o32 body clobbers caller-save registers
            c.r[k] = 0xBAD
    run_shim(words, cpu, body)
    assert seen == {"a0": 9, "a1": 1, "a2": 2, "a3": 3, "s16": 4}
    assert cpu.r[4:16] == before[4:16] and cpu.r[24:26] == before[24:26]   # a retail preserver's values kept


def test_cyclic_moves_refused():
    with pytest.raises(rp.ReplaceError):
        rp.ipa_shim(0x80000000, "void f(s32 a, s32 b)", ["a1", "a0"], (), "out")


def test_retarget_calls_only_hits_the_target():
    jal = lambda t: (3 << 26) | ((t >> 2) & 0x03FFFFFF)
    raw = b"".join(struct.pack(">I", w) for w in (jal(0x80091CA4), 0, jal(0x80091BA8), 0))
    out, n = rp.retarget_calls(raw, 0x80800000, 0x80091CA4, 0x80800040, [(0x80800000, 16)])
    assert n == 1
    assert struct.unpack(">4I", out)[0] == jal(0x80800040)
    assert struct.unpack(">4I", out)[2] == jal(0x80091BA8)


def test_ipa_plan_refuses_undescribed_detected_callees():
    table = {"entity_transform_calc": {"params": ["a0"], "prototype": "void entity_transform_calc(s32 h)"}}
    detected = {"entity_transform_calc": "register parameters f22", "audio_effect_setup": "register parameters s0"}
    e = {"function": "engine_sound_sync"}
    with pytest.raises(rp.ReplaceError, match="audio_effect_setup"):
        rp.ipa_plan("x", e, {"entity_transform_calc", "audio_effect_setup", "D_80142728"}, table, detected)
    use, notes = rp.ipa_plan("x", e, {"entity_transform_calc", "audio_effect_setup"}, table, detected,
                             allow_ipa=True)
    assert list(use) == ["entity_transform_calc"] and "audio_effect_setup" in notes[0]
    with pytest.raises(rp.ReplaceError, match="itself"):
        rp.ipa_plan("x", {"function": "audio_effect_setup"}, set(), {}, detected)


def test_committed_ipa_table_is_consistent():
    table = rp.load_ipa_table()
    assert {"entity_transform_calc", "audio_effect_setup", "func_80091AF0", "func_80091AF8"} <= set(table)
    for name, e in table.items():
        if "body" in e:
            assert (REPO / e["body"]).is_file(), e["body"]
        else:
            rp.ipa_shim(0x80000000, e["prototype"], e["params"], e.get("clobbers", ()), "out")


def test_detection_reads_members_and_frontier(tmp_path):
    (tmp_path / "m.json").write_text(json.dumps({"callees": {"a": ["f22", "a0"], "b": ["a1"]}}))
    (tmp_path / "f.json").write_text(json.dumps({"facts": {"c": {"register_params": [], "unsaved_callee_regs": ["s1"]},
                                                           "a": {"unsaved_callee_regs": ["s2"]}}}))
    d = rp.load_ipa_detection(tmp_path / "m.json", tmp_path / "f.json")
    assert set(d) == {"a", "c"} and "f22" in d["a"] and "s2" in d["a"] and "b" not in d
    assert rp.load_ipa_detection(tmp_path / "none", tmp_path / "none") == {}
