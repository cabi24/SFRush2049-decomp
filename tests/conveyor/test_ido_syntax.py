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
