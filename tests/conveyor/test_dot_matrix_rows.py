"""Behavior and fail-closed complete-object evidence for two row rotations."""
import ctypes
import importlib.util
import json
import math
from pathlib import Path
import random
import shutil
import struct
import subprocess

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / "cloud/work/frontier/dot_matrix_rows"
SPEC = importlib.util.spec_from_file_location("dot_matrix_rows_verify", PACKET / "verify.py")
VERIFY = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(VERIFY)


def f32(value):
    return struct.unpack("f", struct.pack("f", value))[0]


@pytest.fixture(scope="module")
def host_library(tmp_path_factory):
    compiler = shutil.which("cc")
    if not compiler:
        pytest.skip("host C compiler unavailable")
    out = tmp_path_factory.mktemp("matrix-rows") / "rows.so"
    subprocess.run([compiler, "-std=c89", "-O0", "-shared", "-fPIC",
                    "-fno-fast-math", "-ffp-contract=off",
                    *[str(ROOT / "cloud/matches" / (n + ".c")) for n in VERIFY.NAMES],
                    "-o", str(out)], check=True, capture_output=True)
    library = ctypes.CDLL(str(out))
    for name in VERIFY.NAMES:
        function = getattr(library, name)
        function.argtypes = [ctypes.c_float, ctypes.c_float,
                             ctypes.POINTER(ctypes.c_float)]
        function.restype = None
    return library


@pytest.mark.parametrize("name,row", [("func_800ACFF8", 1), ("func_800AD090", 0)])
def test_row_rotation_matches_independent_float_reference(host_library, name, row):
    random_values = random.Random(2049)
    cases = [(0.0, 1.0), (1.0, 0.0), (0.0, -1.0), (-1.0, 0.0)]
    cases += [(math.sin(i / 13.0), math.cos(i / 13.0)) for i in range(40)]
    # The functions consume coefficients directly; they need not lie on a unit circle.
    cases += [(random_values.uniform(-3, 3), random_values.uniform(-3, 3))
              for _ in range(200)]
    function = getattr(host_library, name)
    for sine, cosine in cases:
        sine, cosine = f32(sine), f32(cosine)
        original = [f32(random_values.uniform(-1000, 1000)) for _ in range(9)]
        expected = original[:]
        for column in range(3):
            a, b = original[3 * row + column], original[3 * (row + 1) + column]
            expected[3 * row + column] = f32(f32(a * cosine) - f32(b * sine))
            expected[3 * (row + 1) + column] = f32(f32(b * cosine) + f32(a * sine))
        # A guard on either side checks that only the intended 3x3 storage is touched.
        storage = (ctypes.c_float * 11)(123456.0, *original, -654321.0)
        matrix = ctypes.cast(ctypes.byref(storage, ctypes.sizeof(ctypes.c_float)),
                             ctypes.POINTER(ctypes.c_float))
        function(sine, cosine, matrix)
        assert list(storage)[1:10] == expected
        assert storage[0] == 123456.0 and storage[10] == -654321.0


def test_saved_evidence_authenticates_full_targets_and_sources():
    receipt = json.loads((PACKET / "verification.json").read_text())
    assert len(receipt["results"]) == 8
    expected_cases = {(n, "single", o) for n in VERIFY.NAMES for o in (2, 3)}
    expected_cases |= {(n, "real_pair_group", 3) for n in VERIFY.NAMES}
    expected_cases |= {(n, "real_caller_group", 3) for n in VERIFY.NAMES}
    actual_cases = set()
    targets = VERIFY.score.targets()
    for result in receipt["results"]:
        name = result["function"]
        actual_cases.add((name, result["mode"], 3 if "-O3" in result["flags"] else 2))
        source = ROOT / "cloud/matches" / (name + ".c")
        assert VERIFY.digest(source.read_bytes()) == result["source_sha256"]
        native_bytes = struct.pack(">%dI" % len(targets[name]), *targets[name])
        assert VERIFY.digest(native_bytes) == result["target_bytes_sha256"]
        assert result["target_bytes_sha256"] == result["relocated_function_bytes_sha256"]
        assert result["full_relocated_bytes_equal"]
        assert result["elf_function_size_bytes"] == result["target_extent_bytes"] == 152
        assert result["comparison"] == {
            "differing": 0, "total": 38, "unresolved": [], "unverified": [],
            "errors": [], "extra_words": 0,
        }
    assert actual_cases == expected_cases
    context = receipt["real_caller_probe"]
    assert not context["context_source_changed"]
    assert not context["caller_matches_retail"]
    for result in context["context_results"]:
        assert result["complete_relocated_bytes_unchanged"]
        assert result["baseline_relocated_bytes_sha256"] == result["extended_relocated_bytes_sha256"]
        assert result["baseline_comparison"] == result["extended_comparison"]


@pytest.fixture
def ido_object(tmp_path):
    if not (VERIFY.score.IDO / "cc").is_file():
        pytest.skip("set IDO_DIR to replay compiled-object rejection tests")
    name = VERIFY.NAMES[0]
    obj = tmp_path / "candidate.o"
    VERIFY.score.compile_single(ROOT / "cloud/matches" / (name + ".c"), VERIFY.FLAGS, obj)
    return obj, name


def test_elf_extent_check_rejects_claimed_zero_padding(ido_object):
    obj, name = ido_object
    data, sections = VERIFY.score._elf(obj)
    mutated = bytearray(data)
    changed = False
    for index, section in enumerate(sections):
        if section["type"] != 2:
            continue
        for ordinal, symbol in enumerate(VERIFY.score._symbol_table(data, sections, index)):
            if symbol["name"] == name and symbol["type"] == 2:
                struct.pack_into(">I", mutated, section["off"] + 16 * ordinal + 8,
                                 symbol["size"] + 4)
                changed = True
    assert changed
    obj.write_bytes(mutated)
    # The historic scorer tolerates zero section alignment padding. The explicit
    # ELF-symbol extent check must still reject claiming that padding as code.
    assert VERIFY.score.compare(obj, name, show=0).accepted()
    with pytest.raises(AssertionError, match="wrong complete extent"):
        VERIFY.verify_object(obj, name)


def test_full_byte_check_rejects_changed_instruction(ido_object):
    obj, name = ido_object
    data, sections = VERIFY.score._elf(obj)
    mutated = bytearray(data)
    text = sections[VERIFY.score._text_index(sections)]
    mutated[text["off"] + 3] ^= 4
    obj.write_bytes(mutated)
    with pytest.raises(AssertionError):
        VERIFY.verify_object(obj, name)
