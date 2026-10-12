"""ROM-free o32 planner, generated C++ execution, and fail-closed fixtures."""
import importlib.util
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

REPO = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location("recomp_gen_wrapper", REPO / "tools/recomp/gen_wrapper.py")
gw = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = gw
SPEC.loader.exec_module(gw)


@pytest.mark.parametrize("parameters,expected", [
    ("void", []),
    ("s32, u32, s8, u16, s32", [(0, None), (1, None), (2, None), (3, None), (4, None)]),
    ("s32, s64, u32", [(0, None), (2, None), (4, None)]),
    ("s32, s32, s32, u64, u32", [(0, None), (1, None), (2, None), (4, None), (6, None)]),
    ("f32, f32, f32, f32, f32", [(0, 12), (1, 14), (2, None), (3, None), (4, None)]),
    ("f32, f64, f32", [(0, 12), (2, 14), (4, None)]),
    ("f64, f32, f32", [(0, 12), (2, 14), (3, None)]),
    ("f64, f64, f64", [(0, 12), (2, 14), (4, None)]),
    ("s32, f32, f64", [(0, None), (1, None), (2, None)]),
    ("f32, s32, f32, f64", [(0, 12), (1, None), (2, None), (4, None)]),
])
def test_hard_float_o32_word_slots(parameters, expected):
    proto = gw.parse_prototype(f"void example({parameters});")
    assert [(loc.slot, loc.fpr) for loc in gw.argument_locations(proto)] == expected


@pytest.mark.parametrize("prototype,hosts", [
    ("extern s32 sample(const u8 count, unsigned long flags, long long value);", ["uint8_t", "uint32_t", "int64_t"]),
    ("double sample(int32_t, uint64_t, float);", ["int32_t", "uint64_t", "float"]),
    ("void sample(signed char, unsigned short int, signed long int);", ["int8_t", "uint16_t", "int32_t"]),
    ("extern\nvoid sample(void);", []),
])
def test_supported_type_spellings(prototype, hosts):
    assert [arg.host for arg in gw.parse_prototype(prototype).args] == hosts


@pytest.mark.parametrize("prototype", [
    "void old_style();", "void variadic(int n, ...);", "struct View value(void);",
    "void aggregate(struct View v);", "void array(int a[4]);", "void callback(void (*fn)(int));",
    "void unknown(size_t count);", "char ambiguous_char(void);", "long double extended(void);",
    "void booleans(_Bool x);", "void complex(_Complex double x);", "int attributes(void) __attribute__((weak));",
    "int first(void); int second(void);", "int body(void) {}", "#define X 1\nint fn(void);",
    "void bad(void, int);", "void bad(int,);", "void class(int);", "void bad(int class);",
    "static int hidden(void);", "typedef int alias(int);", "void comments(int /* count */);",
])
def test_reject_unsupported_prototypes(prototype):
    with pytest.raises(gw.PrototypeError):
        gw.generate(prototype)


def test_pointer_contract_requires_explicit_adapter():
    with pytest.raises(gw.PrototypeError, match="guest-pointers"):
        gw.generate("InputRecord* advance(InputRecord* item);")
    output = gw.generate("InputRecord* advance(const InputRecord* item, char** names);", guest_pointers=True)
    assert 'extern "C" rush_addr rush_host_advance(const rush_rdram* memory, rush_addr arg0, rush_addr arg1);' in output
    assert "rush_o32_pointer(ctx->r4)" in output
    assert "rush_host_advance(&memory, arg0, arg1)" in output
    assert "reinterpret_cast" not in output


def test_cli_deterministic_and_failure_does_not_clobber(tmp_path, capsys):
    source, out = tmp_path / "prototype.txt", tmp_path / "generated.cpp"
    source.write_text("u32 sample(s32 count, f64 scale);")
    args = ["--prototype-file", str(source), "--out", str(out), "--target", "my_adapter", "--wrapper", "ovl_a_sample", "--with-rdram"]
    assert gw.main(args) == 0
    first = out.read_bytes()
    assert gw.main(args) == 0
    assert out.read_bytes() == first
    assert b"ovl_a_sample(uint8_t* rdram, recomp_context* ctx)" in first
    source.write_text("void unsupported(...);")
    assert gw.main(args) == 2
    assert out.read_bytes() == first
    assert "plain function prototype" in capsys.readouterr().err
    assert gw.main(["--prototype", "void ok(void);"]) == 0
    assert 'extern "C" void ok' in capsys.readouterr().out


@pytest.mark.parametrize("options", [
    {"target": "same", "wrapper": "same"}, {"target": "bad-name"}, {"wrapper": "namespace"},
    {"rdram_size": 0}, {"rdram_size": 6}, {"rdram_size": 0x20000004},
])
def test_reject_invalid_generation_options(options):
    with pytest.raises(gw.PrototypeError):
        gw.generate("void sample(void);", **options)


@pytest.fixture(scope="module")
def compiler():
    compiler = shutil.which(os.environ.get("CXX", "g++"))
    if compiler is None:
        pytest.skip("C compiler g++ absent")
    return compiler


def compile_fixture(tmp_path, compiler, prototypes, body, *, sanitize=True):
    source = tmp_path / "fixture.cpp"
    chunks = [gw.generate(proto, rdram_size=256, **options) for proto, options in prototypes]
    source.write_text("\n".join(chunks) + '\n#include <cassert>\n' + body)
    executable = tmp_path / "fixture"
    command = [compiler, "-std=c++17", "-O2", "-Wall", "-Wextra", "-Werror", "-fstrict-aliasing",
               "-I", os.environ.get("RUSH_RECOMP_INCLUDE", str(REPO / "tests/recomp/fixtures/wrapper_runtime")),
               "-I", str(REPO / "tools/recomp"), str(source), "-o", str(executable)]
    if sanitize:
        command += ["-fsanitize=undefined", "-fno-sanitize-recover=all"]
    subprocess.run(command, check=True, text=True, capture_output=True)
    return executable


def run_fixture(executable):
    result = subprocess.run([str(executable)], text=True, capture_output=True)
    assert result.returncode == 0, result.stdout + result.stderr


def test_execute_integer_registers_stack_and_sign_extension(tmp_path, compiler):
    exe = compile_fixture(tmp_path, compiler, [
        ("u32 narrow(s8 a, u8 b, s16 c, u16 d, s32 e, u32 f);", {}),
        ("s64 wide(s32 a, u64 b, s32 c, s64 d);", {}),
        ("u64 spill(s32 a, s32 b, s32 c, u64 d, u32 e);", {}),
        ("void untouched(void);", {}),
    ], r'''
static unsigned calls;
extern "C" uint32_t rush_host_narrow(int8_t a, uint8_t b, int16_t c, uint16_t d, int32_t e, uint32_t f) {
    ++calls;
    assert(a == -128 && b == 255 && c == -32768 && d == 65535);
    assert(e == INT32_MIN && f == UINT32_MAX);
    return UINT32_C(0x80000001);
}
extern "C" int64_t rush_host_wide(int32_t a, uint64_t b, int32_t c, int64_t d) {
    ++calls;
    assert(a == -7 && b == UINT64_C(0x89abcdef01234567) && c == -9);
    assert(d == INT64_MIN + 7);
    return rush_o32_bits<int64_t>(UINT64_C(0xfedcba9887654321));
}
extern "C" uint64_t rush_host_spill(int32_t a, int32_t b, int32_t c, uint64_t d, uint32_t e) {
    ++calls;
    assert(a == 1 && b == 2 && c == 3 && d == UINT64_C(0x112233448899aabb) && e == 9);
    return UINT64_C(0x0123456789abcdef);
}
extern "C" void rush_host_untouched(void) { ++calls; }
int main() {
    uint8_t data[256] = {};
    rush_rdram memory{data, sizeof(data)};
    recomp_context ctx{};
    ctx.r29 = UINT64_C(0xffffffff80000020);
    ctx.r4 = UINT64_C(0x1234567800000080); // scalar upper GPR bits ignored
    ctx.r5 = 255; ctx.r6 = 0x8000; ctx.r7 = 0xffff;
    assert(rush_rdram_write_u32(&memory, 0x80000030, 0x80000000));
    assert(rush_rdram_write_u32(&memory, 0x80000034, 0xffffffff));
    ctx.r3 = 0x1234; ctx.r16 = 0x5678; ctx.f0.u64 = 0x11223344;
    narrow(data, &ctx);
    assert(ctx.r2 == UINT64_C(0xffffffff80000001) && ctx.r3 == 0x1234);
    assert(ctx.r16 == 0x5678 && ctx.f0.u64 == 0x11223344);
    ctx.r4 = static_cast<gpr>(-7); ctx.r5 = 0xdeadbeef; // alignment hole
    ctx.r6 = 0x89abcdef; ctx.r7 = 0x01234567;
    assert(rush_rdram_write_s32(&memory, 0x80000030, -9));
    assert(rush_rdram_write_u32(&memory, 0x80000034, 0xdeadbeef)); // hole
    assert(rush_rdram_write_s64(&memory, 0x80000038, INT64_MIN + 7));
    wide(data, &ctx);
    assert(ctx.r2 == UINT64_C(0xfffffffffedcba98) && ctx.r3 == UINT64_C(0xffffffff87654321));
    ctx.r29 = 0x80000020; // zero-extended stack is accepted too
    ctx.r4 = 1; ctx.r5 = 2; ctx.r6 = 3; ctx.r7 = 0xdeadbeef; // skipped for aligned pair
    assert(rush_rdram_write_u64(&memory, 0x80000030, UINT64_C(0x112233448899aabb)));
    assert(rush_rdram_write_u32(&memory, 0x80000038, 9));
    spill(data, &ctx);
    assert(ctx.r2 == 0x01234567 && ctx.r3 == UINT64_C(0xffffffff89abcdef));
    recomp_context before = ctx;
    untouched(nullptr, &ctx);
    assert(std::memcmp(&before, &ctx, sizeof(ctx)) == 0 && calls == 4);
}
''')
    run_fixture(exe)


def test_execute_fpr_gpr_stack_and_float_returns(tmp_path, compiler):
    exe = compile_fixture(tmp_path, compiler, [
        ("f32 floats(f32 a, f32 b, f32 c, f32 d, f32 e);", {}),
        ("f64 float_double(f32 a, f64 b, f64 c);", {}),
        ("f64 double_float(f64 a, f32 b, f32 c);", {}),
        ("f64 doubles(f64 a, f64 b, f64 c);", {}),
        ("f32 mixed(s32 a, f32 b, f64 c, f32 d);", {}),
        ("f64 interrupted(f32 a, s32 b, f32 c, f64 d);", {}),
    ], r'''
extern "C" float rush_host_floats(float a, float b, float c, float d, float e) {
    assert(a == 1.25f && b == -2.5f && c == 3.75f && d == -4.5f && e == 5.25f);
    return rush_o32_bits<float>(UINT32_C(0x80000000));
}
extern "C" double rush_host_float_double(float a, double b, double c) {
    assert(a == 1.25f && b == -2.5 && c == 3.75); return -9.5;
}
extern "C" double rush_host_double_float(double a, float b, float c) {
    assert(a == -1.5 && b == 2.25f && c == 3.5f); return 8.75;
}
extern "C" double rush_host_doubles(double a, double b, double c) {
    assert(a == -1.5 && b == 2.25 && c == 3.5); return 8.75;
}
extern "C" float rush_host_mixed(int32_t a, float b, double c, float d) {
    assert(a == 7 && b == -1.25f && c == -2.5 && d == 3.75f);
    return rush_o32_bits<float>(UINT32_C(0x7fc01234));
}
extern "C" double rush_host_interrupted(float a, int32_t b, float c, double d) {
    assert(a == 1.25f && b == -7 && c == -3.5f && d == 8.5);
    return rush_o32_bits<double>(UINT64_C(0x7ff8123456789abc));
}
int main() {
    uint8_t data[256] = {};
    rush_rdram memory{data, sizeof(data)};
    recomp_context ctx{};
    ctx.r29 = 0x80000020;
    ctx.f12.fl = 1.25f; ctx.f14.fl = -2.5f;
    ctx.r4 = 0xdeadbeef; ctx.r5 = 0xcafebabe; // shadow slots are not authoritative
    ctx.r6 = rush_o32_bits<uint32_t>(3.75f); ctx.r7 = rush_o32_bits<uint32_t>(-4.5f);
    assert(rush_rdram_write_f32(&memory, 0x80000030, 5.25f));
    ctx.r2 = 0x1234; ctx.r3 = 0x5678;
    floats(data, &ctx);
    assert(rush_o32_bits<uint32_t>(ctx.f0.fl) == 0x80000000);
    assert(ctx.r2 == 0x1234 && ctx.r3 == 0x5678);
    ctx.f14.d = -2.5;
    assert(rush_rdram_write_f64(&memory, 0x80000030, 3.75));
    float_double(data, &ctx); assert(ctx.f0.d == -9.5);
    ctx.f12.d = -1.5; ctx.f14.fl = 2.25f; ctx.r7 = rush_o32_bits<uint32_t>(3.5f);
    double_float(data, &ctx); assert(ctx.f0.d == 8.75);
    ctx.f14.d = 2.25;
    assert(rush_rdram_write_f64(&memory, 0x80000030, 3.5));
    doubles(data, &ctx); assert(ctx.f0.d == 8.75);
    ctx.r4 = 7; ctx.r5 = rush_o32_bits<uint32_t>(-1.25f);
    uint64_t bits = rush_o32_bits<uint64_t>(-2.5);
    ctx.r6 = bits >> 32; ctx.r7 = static_cast<uint32_t>(bits);
    assert(rush_rdram_write_f32(&memory, 0x80000030, 3.75f));
    mixed(data, &ctx); assert(rush_o32_bits<uint32_t>(ctx.f0.fl) == 0x7fc01234);
    ctx.f12.fl = 1.25f; ctx.f14.d = 9999; // integer breaks the FPR prefix
    ctx.r5 = static_cast<gpr>(-7); ctx.r6 = rush_o32_bits<uint32_t>(-3.5f);
    assert(rush_rdram_write_f64(&memory, 0x80000030, 8.5));
    interrupted(data, &ctx);
    assert(rush_o32_bits<uint64_t>(ctx.f0.d) == UINT64_C(0x7ff8123456789abc));
}
''')
    run_fixture(exe)


def test_execute_guest_addresses_and_memory_adapter(tmp_path, compiler):
    exe = compile_fixture(tmp_path, compiler, [
        ("struct InputRecord* advance(struct InputRecord* p, void* q, u32 a, u32 b, char* r);", {"guest_pointers": True}),
        ("u32 global_value(void);", {"with_rdram": True}),
    ], r'''
#include "game_rdram.h"
extern "C" rush_addr rush_host_advance(const rush_rdram* memory, rush_addr p, rush_addr q, uint32_t a, uint32_t b, rush_addr r) {
    assert(p == 0x80000080 && q == 0 && a == 3 && b == 4 && r == 0x80000090);
    rush_rdram_view view{memory, p};
    assert(rush_InputRecord_write_active(view, 0xa5));
    return p + 4;
}
extern "C" uint32_t rush_host_global_value(const rush_rdram* memory) {
    uint8_t value = 0;
    rush_rdram_view view{memory, 0x80000080};
    assert(rush_InputRecord_read_active(view, &value));
    return value;
}
int main() {
    uint8_t data[256] = {};
    rush_rdram memory{data, sizeof(data)};
    recomp_context ctx{};
    ctx.r29 = 0x80000020; ctx.r4 = UINT64_C(0xffffffff80000080); ctx.r5 = 0; ctx.r6 = 3; ctx.r7 = 4;
    assert(rush_rdram_write_addr(&memory, 0x80000030, 0x80000090));
    advance(data, &ctx); assert(ctx.r2 == UINT64_C(0xffffffff80000084));
    assert(data[0x81 ^ 3] == 0xa5);
    global_value(data, &ctx); assert(ctx.r2 == 0xa5);
}
''')
    run_fixture(exe)


@pytest.mark.parametrize("ctype,value,expected", [
    ("s8", "INT8_MIN", "UINT64_C(0xffffffffffffff80)"),
    ("u8", "UINT8_MAX", "255"), ("s16", "INT16_MIN", "UINT64_C(0xffffffffffff8000)"),
    ("u16", "UINT16_MAX", "65535"), ("s32", "INT32_MIN", "UINT64_C(0xffffffff80000000)"),
    ("u32", "UINT32_MAX", "UINT64_MAX"),
])
def test_execute_narrow_returns(tmp_path, compiler, ctype, value, expected):
    exe = compile_fixture(tmp_path, compiler, [(f"{ctype} returned(void);", {})], f'''
extern "C" {gw.TYPES[ctype].host} rush_host_returned(void) {{ return {value}; }}
int main() {{ recomp_context ctx{{}}; returned(nullptr, &ctx); assert(ctx.r2 == {expected}); }}
''')
    run_fixture(exe)


@pytest.mark.parametrize("sp", [
    "0x7ffffff0",  # invalid base must not cross into KSEG0 at sp + 16
    "0x800000f0",  # sp + 16 lands just outside supplied allocation
    "0x80000021",  # unaligned stack
    "0xa0000020",  # KSEG1 must not alias KSEG0
    "0xfffffff8",  # address addition would wrap
    "UINT64_C(0x1234567880000020)",  # not a 32-bit zero/sign extension
])
def test_bad_stack_aborts_before_target(tmp_path, compiler, sp):
    exe = compile_fixture(tmp_path, compiler, [("void bad(u32 a, u32 b, u32 c, u32 d, u32 e);", {})], f'''
extern "C" void rush_host_bad(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) {{ std::exit(77); }}
int main() {{ uint8_t data[256] = {{}}; recomp_context ctx{{}}; ctx.r29 = {sp}; bad(data, &ctx); }}
''', sanitize=False)
    result = subprocess.run([str(exe)], capture_output=True)
    assert result.returncode in (-6, 134), result.stderr


def test_native_pointer_shape_is_not_silently_truncated(tmp_path, compiler):
    exe = compile_fixture(tmp_path, compiler, [("void bad(void* p);", {"guest_pointers": True})], r'''
extern "C" void rush_host_bad(const rush_rdram*, rush_addr) { std::exit(77); }
int main() { uint8_t data[256] = {}; recomp_context ctx{}; ctx.r4 = UINT64_C(0x1234567880000080); bad(data, &ctx); }
''', sanitize=False)
    result = subprocess.run([str(exe)], capture_output=True)
    assert result.returncode in (-6, 134), result.stderr
