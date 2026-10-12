"""ROM-free RDRAM layout/accessor fixtures: no runtime, ROM or generated game C."""
import os
from pathlib import Path
import re
import shutil
import subprocess

import pytest
from pycparser import c_ast, c_parser

REPO = Path(__file__).resolve().parents[2]
FIXTURE = REPO / "tests/recomp/fixtures/rdram_accessors.c"
HEADER = REPO / "tools/recomp/game_rdram.h"


def compiler(name):
    path = shutil.which(name)
    if path is None:
        pytest.skip(f"C compiler {name} unavailable for host fixture")
    return path


@pytest.mark.parametrize("cc,standard", [("cc", "c11"), ("c++", "c++17")])
def test_host_accessors(tmp_path, cc, standard):
    binary = tmp_path / "rdram-fixture"
    subprocess.run([compiler(cc), f"-std={standard}", "-O2", "-Wall", "-Wextra", "-Werror",
                    "-pedantic", "-fstrict-aliasing", "-I", str(REPO), str(FIXTURE),
                    "-o", str(binary)], check=True, capture_output=True, text=True)
    result = subprocess.run([str(binary)], check=True, capture_output=True, text=True)
    assert "fixtures passed" in result.stdout


def test_host_accessors_ubsan(tmp_path):
    binary = tmp_path / "rdram-ubsan"
    subprocess.run([compiler("cc"), "-std=c11", "-O1", "-g", "-Wall", "-Wextra", "-Werror",
                    "-fsanitize=undefined", "-fno-sanitize-recover=all", "-I", str(REPO),
                    str(FIXTURE), "-o", str(binary)], check=True, capture_output=True, text=True)
    subprocess.run([str(binary)], check=True, capture_output=True, text=True,
                   env={**os.environ, "UBSAN_OPTIONS": "halt_on_error=1:print_stacktrace=1"})


# Only primitive widths, pointers and ordinary structs occur in this header.
# This test computes guest offsets from the actual source declarations, not
# host sizeof/offsetof. o32 pointers/ints are 4 bytes, short 2, byte 1; the
# known OSMesgQueue dependency is six o32 words. No source/ROM extraction.
PRELUDE = """
typedef unsigned char u8; typedef signed char s8;
typedef unsigned short u16; typedef short s16;
typedef unsigned int u32; typedef int s32;
typedef float f32; typedef float F32; typedef short S16;
typedef struct {
    void *mtqueue; void *fullqueue;
    s32 validCount; s32 first; s32 msgCount; void *msg;
} OSMesgQueue;
"""
KINDS = {"u8": (1, "u8"), "s8": (1, "s8"), "u16": (2, "u16"), "s16": (2, "s16"),
         "u32": (4, "u32"), "s32": (4, "s32"), "f32": (4, "f32"),
         "F32": (4, "f32"), "S16": (2, "s16")}


def source_types():
    source = (REPO / "include/game_types.h").read_text()
    source = re.sub(r"/\*.*?\*/", "", source, flags=re.S)
    source = re.sub(r"^\s*#.*$", "", source, flags=re.M)
    tree = c_parser.CParser().parse(PRELUDE + source)
    return {n.name: n.type.type for n in tree.ext if isinstance(n, c_ast.Typedef)
            and isinstance(n.type, c_ast.TypeDecl) and isinstance(n.type.type, c_ast.Struct)}


def constant(node):
    if isinstance(node, c_ast.Constant):
        return int(node.value, 0)
    assert isinstance(node, c_ast.BinaryOp) and node.op in ("+", "-")
    left, right = constant(node.left), constant(node.right)
    return left + right if node.op == "+" else left - right


def scalar(node):
    if isinstance(node, c_ast.PtrDecl):
        return 4, "addr"
    assert isinstance(node, c_ast.TypeDecl) and isinstance(node.type, c_ast.IdentifierType)
    name = " ".join(node.type.names)
    if name == "OSMesgQueue":
        return 24, "view"
    return KINDS[name]


def guest_layouts():
    layouts = {}
    for name, struct in source_types().items():
        if name == "OSMesgQueue":
            continue
        fields, position = {}, 0
        for field in struct.decls:
            array = isinstance(field.type, c_ast.ArrayDecl)
            size, kind = scalar(field.type.type if array else field.type)
            align = min(size, 4)
            position = (position + align - 1) // align * align
            count = constant(field.type.dim) if array else 1
            if not field.name.lower().lstrip("_").startswith("pad"):
                fields[field.name] = (position, kind, count, size)
            position += size * count
        layouts[name] = fields
    return layouts


def schema_fields():
    text = HEADER.read_text()
    scalars = re.findall(r" X\((\w+), (\w+), (0x[0-9a-f]+), (\w+), (\w+)\)", text)
    arrays = re.findall(r" X\((\w+), (\w+), (0x[0-9a-f]+), (\d+), (\w+), (\w+)\)", text)
    values = {(s, f): (int(o, 16), k, 1) for s, f, o, k, _ in scalars}
    values.update({(s, f): (int(o, 16), k, int(n)) for s, f, o, n, k, _ in arrays})
    return values


def test_every_named_game_field_has_its_guest_layout():
    expected = {(s, f): v[:3] for s, fields in guest_layouts().items() for f, v in fields.items()}
    actual = schema_fields()
    # The embedded OSMesgQueue is intentionally a subview, not a native copy.
    text = HEADER.read_text()
    offset = int(re.search(r"rush_SegmentHeader_unk9CC0_offset = (0x[0-9a-f]+)", text)[1], 16)
    actual[("SegmentHeader", "unk9CC0")] = (offset, "view", 1)
    assert actual == expected


def test_original_header_offsets_with_32bit_compiler(tmp_path):
    """Extra independent compiler check where an o32 or ILP32 cc is available.

    These structs contain no 64-bit-aligned values, so ILP32 with 4-byte
    pointers has the same field layout as o32. It is not an ABI execution test.
    """
    mips = shutil.which("mips-linux-gnu-gcc")
    cc = mips or compiler("cc")
    flags = ["-mabi=32"] if mips else ["-m32"]
    probe = subprocess.run([cc, *flags, "-x", "c", "-fsyntax-only", "-"],
                           input="_Static_assert(sizeof(void*) == 4, \"ILP32\");",
                           capture_output=True, text=True)
    if probe.returncode:
        pytest.skip("optional independent 32-bit layout compiler unavailable")
    lines = [PRELUDE, '#include "include/game_types.h"']
    for (name, field), (offset, kind, count) in schema_fields().items():
        lines.append(f'_Static_assert(__builtin_offsetof({name}, {field}) == {offset}, "{name}.{field}");')
        width = {"addr": 4, "f32": 4, "u8": 1, "s8": 1, "u16": 2, "s16": 2, "s32": 4}[kind]
        lines.append(f'_Static_assert(sizeof((({name}*)0)->{field}) == {width * count}, "field width");')
    lines.append('_Static_assert(__builtin_offsetof(SegmentHeader, unk9CC0) == 0x9cc0, "queue offset");')
    lines.append('_Static_assert(sizeof(OSMesgQueue) == 24, "queue size");')
    source = tmp_path / "layouts.c"
    source.write_text("\n".join(lines))
    subprocess.run([cc, *flags, "-std=c11", "-ffreestanding", "-fno-builtin", "-Wall", "-Werror", "-Wno-comment",
                    "-I", str(REPO), "-fsyntax-only", str(source)], check=True,
                   capture_output=True, text=True)


def test_actual_runtime_header_if_supplied(tmp_path):
    """Opt-in upstream comparison: fixtures stay offline and self-contained."""
    include = os.environ.get("RUSH_N64RECOMP_INCLUDE")
    if not include:
        pytest.skip("optional pinned runtime headers not supplied (RUSH_N64RECOMP_INCLUDE)")
    assert (Path(include) / "recomp.h").is_file(), "explicit runtime include path is invalid"
    binary = tmp_path / "runtime-contract"
    subprocess.run([compiler("cc"), "-std=c11", "-O2", "-Wall", "-Wextra", "-Werror",
                    "-I", str(REPO), "-I", include,
                    str(REPO / "tests/recomp/fixtures/rdram_runtime_contract.c"),
                    "-o", str(binary), "-lm"], check=True, capture_output=True, text=True)
    subprocess.run([str(binary)], check=True, capture_output=True, text=True)
