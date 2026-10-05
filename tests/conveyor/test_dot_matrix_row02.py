"""Focused proof and finite native semantics for the ordinary row-0/2 rotation."""
import ctypes
import hashlib
import importlib.util
import json
from pathlib import Path
import random
import struct
import subprocess

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / "cloud/work/frontier/dot_matrix_row02"
SOURCE = ROOT / "cloud/matches/func_800C40E8.c"


def verifier():
    spec = importlib.util.spec_from_file_location("dot_matrix_row02_verify", PACKET / "verify.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def test_receipt_is_bound_to_final_source():
    receipt = json.loads((PACKET / "verification.json").read_text())
    sha = hashlib.sha256(SOURCE.read_bytes()).hexdigest()
    assert len(receipt["results"]) == 2
    for row in receipt["results"]:
        assert row["source_sha256"] == sha
        assert row["target_extent_bytes"] == row["elf_function_size_bytes"] == 152
        assert row["target_bytes_sha256"] == row["relocated_function_bytes_sha256"]
        assert row["full_relocated_bytes_equal"]
    assert receipt["negative_control"]["strict_verifier_rejects"]


def test_fresh_complete_replay_and_negative_control(tmp_path):
    module = verifier()
    receipt = module.replay(tmp_path)
    assert all(row["comparison"]["differing"] == 0 for row in receipt["results"])
    assert receipt["negative_control"]["comparison"]["differing"] > 0


def test_extent_gate_is_independent_of_padded_text(tmp_path, monkeypatch):
    module = verifier()
    obj = tmp_path / "candidate.o"
    module.score.compile_single(SOURCE, module.FLAGS, obj)
    start, size = module.function_extent(obj, "func_800C40E8")
    assert size == 152
    monkeypatch.setattr(module, "function_extent", lambda *_: (start, 148))
    with pytest.raises(AssertionError, match="wrong complete extent"):
        module.verify_object(obj, "func_800C40E8")


def test_native_finite_semantics_and_unchanged_middle_row(tmp_path):
    library = tmp_path / "rotation.so"
    subprocess.run(["cc", "-std=c89", "-Wall", "-Wextra", "-Werror", "-O2",
                    "-ffp-contract=off", "-fno-fast-math", "-shared", "-fPIC",
                    str(SOURCE), "-o", str(library)], check=True)
    function = ctypes.CDLL(str(library)).func_800C40E8
    function.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.POINTER(ctypes.c_float)]
    function.restype = None
    f32 = lambda x: ctypes.c_float(x).value
    bits = lambda x: struct.pack("=f", x)
    rng = random.Random(0xC40E8)
    cases = [(s, c, [float(i - 4) for i in range(9)])
             for s, c in [(0., 1.), (-0., 1.), (1., 0.), (-1., 0.), (0., -1.), (0., 0.)]]
    cases += [(f32(rng.uniform(-3, 3)), f32(rng.uniform(-3, 3)),
               [f32(rng.uniform(-100, 100)) for _ in range(9)]) for _ in range(4096)]
    for sine, cosine, values in cases:
        matrix = (ctypes.c_float * 9)(*values)
        expected = list(values)
        for column in range(3):
            first, last = values[column], values[6 + column]
            expected[column] = f32(f32(last * sine) + f32(first * cosine))
            expected[6 + column] = f32(f32(last * cosine) - f32(first * sine))
        function(sine, cosine, matrix)
        assert [bits(x) for x in matrix] == [bits(x) for x in expected]
        assert [bits(x) for x in matrix[3:6]] == [bits(x) for x in values[3:6]]
