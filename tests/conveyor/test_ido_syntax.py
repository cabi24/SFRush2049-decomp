"""The histogram's gcc check must refuse what IDO's cfe refuses.

96 extracted seeds once counted as "compiled" (gcc -std=gnu89 accepted them)
while IDO rejected every one: 84 did arithmetic on a `void *`, 12 compared
distinct pointer types. Those seeds could never be scored.
"""
import shutil
import subprocess

import pytest

from tools.conveyor.pipeline import autodecomp

pytestmark = pytest.mark.skipif(
    shutil.which("mips-linux-gnu-gcc") is None or shutil.which("cpp") is None,
    reason="needs the mips cross gcc")

_PRELUDE = "typedef unsigned char u8; typedef int s32;\n"


def _ok(body):
    ok, _ = autodecomp._seed_compile_errors(_PRELUDE + body)
    return ok


def test_void_pointer_arithmetic_is_rejected():
    assert not _ok("void *f(void *p) { return p + 8; }\n")


def test_byte_cast_arithmetic_is_accepted():
    assert _ok("void *f(void *p) { return (u8 *) p + 8; }\n")


def test_distinct_pointer_comparison_is_rejected():
    assert not _ok("struct S { s32 a; }; extern struct S d;\n"
                   "s32 f(s32 *p) { return p != &d; }\n")


def test_void_cast_comparison_is_accepted():
    assert _ok("struct S { s32 a; }; extern struct S d;\n"
               "s32 f(s32 *p) { return p != (void *) &d; }\n")


def test_pointer_integer_comparison_is_rejected():
    assert not _ok("s32 f(s32 *p, s32 i) { return p == i; }\n")


def test_m2c_patches_apply_to_the_pinned_submodule():
    root = autodecomp.M2C.parent
    patches = sorted(autodecomp.M2C_PATCHES.glob("*.patch"))
    assert patches, "tools/m2c_patches is empty"
    autodecomp.ensure_m2c_patched()
    for patch in patches:
        proc = subprocess.run(
            ["git", "-C", str(root), "apply", "--check", "--reverse", str(patch)],
            capture_output=True, text=True)
        assert proc.returncode == 0, proc.stderr


_STRIDE_ASM = """glabel f
lui $v0, %hi(D_1000)
addiu $v0, $v0, %lo(D_1000)
lui $v1, %hi(D_1010)
addiu $v1, $v1, %lo(D_1010)
.L1:
sw $zero, 0($v0)
addiu $v0, $v0, 4
bne $v0, $v1, .L1
nop
jr $ra
nop
"""


def test_m2c_scales_typed_pointer_steps(tmp_path):
    """m2c's `ptr + n` is a byte offset; C scales it by the pointee size.
    Before patch 0002 this loop came out as `var_v0 += 4` on an `s32 *`, a
    16-byte step -- the most common defect in hand-finished near misses."""
    autodecomp.ensure_m2c_patched()
    asm = tmp_path / "f.s"
    asm.write_text(_STRIDE_ASM)
    ctx = tmp_path / "ctx.c"
    ctx.write_text("typedef int s32; typedef unsigned char u8;\n"
                   "extern s32 D_1000;\nextern s32 D_1010;\n")
    out = subprocess.run(
        ["python3", str(autodecomp.M2C), str(asm), "-f", "f", "--valid-syntax",
         "--context", str(ctx)], capture_output=True, text=True, check=True).stdout
    assert "s32 *var_v0;" in out
    assert "var_v0 = var_v0 + 1;" in out
    assert "+= 4" not in out


_IPA_CALLEE = """glabel callee
sw $t0,16($a0)
jr $ra
nop
"""
_IPA_CALLER = """glabel caller
addiu $sp,$sp,-24
sw $ra,20($sp)
jal callee
li $t0,54
lw $ra,20($sp)
jr $ra
addiu $sp,$sp,24
"""


def _m2c(tmp_path, asm_text, name, env=None):
    import os
    asm = tmp_path / f"{name}.s"
    asm.write_text(asm_text)
    ctx = tmp_path / "ctx.c"
    ctx.write_text("typedef int s32; typedef unsigned char u8;\n")
    return subprocess.run(
        ["python3", str(autodecomp.M2C), str(asm), "-f", name, "--valid-syntax",
         "--context", str(ctx)], capture_output=True, text=True, check=True,
        env=dict(os.environ, **(env or {}))).stdout


def test_m2c_ipa_mode_turns_register_parameters_into_c_parameters(tmp_path):
    """Patch 0004: with M2C_IPA_REGS, a callee's $t0 is a parameter and a
    call to it passes the value its caller put in $t0."""
    import json
    autodecomp.ensure_m2c_patched()
    regs = tmp_path / "map.json"
    regs.write_text(json.dumps({"callee": ["t0"]}))
    env = {"M2C_IPA_REGS": str(regs)}
    callee = _m2c(tmp_path, _IPA_CALLEE, "callee", env)
    caller = _m2c(tmp_path, _IPA_CALLER, "caller", env)
    assert "ipa_t0" in callee and "unset register" not in callee
    assert "callee(0x36)" in caller
    plain = _m2c(tmp_path, _IPA_CALLEE, "callee")
    assert "unset register $t0" in plain          # plain seeds are unchanged


def test_m2c_casts_a_load_through_an_integer_typed_address(tmp_path):
    """Patch 0005: a load whose address m2c typed as an integer is spelled
    through M2C_FIELD instead of the uncompilable `*x`."""
    autodecomp.ensure_m2c_patched()
    asm = tmp_path / "f.s"
    asm.write_text("glabel f\nlh $v0, 0($a0)\njr $ra\nnop\n")
    ctx = tmp_path / "ctx.c"
    ctx.write_text("typedef int s32; typedef short s16;\ns32 f(s32 arg0);\n")
    out = subprocess.run(
        ["python3", str(autodecomp.M2C), str(asm), "-f", "f", "--valid-syntax",
         "--context", str(ctx)], capture_output=True, text=True, check=True).stdout
    assert "M2C_FIELD(arg0, s16 *, 0)" in out
    assert "*arg0" not in out
