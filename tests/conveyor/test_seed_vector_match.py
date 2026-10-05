"""Host semantic controls for the independently scored collision callback."""
from pathlib import Path
import shutil
import subprocess

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / "cloud/work/frontier/dot_seed_vector"


def test_collision_callback_host_contract(tmp_path):
    compiler = shutil.which("cc")
    if compiler is None:
        pytest.skip("Host C compiler unavailable")
    executable = tmp_path / "collision_contract"
    subprocess.run(
        [compiler, "-std=c99", "-O2", "-Wall", "-Wextra",
         "-Wno-pointer-to-int-cast", "-Wno-unused-parameter",
         str(PACKET / "semantics.c"), "-lm", "-o", str(executable)],
        check=True, capture_output=True, text=True,
    )
    result = subprocess.run([str(executable)], check=True, capture_output=True, text=True)
    assert result.stdout == "1033 collision contract cases passed\n"


def test_verification_receipt_is_source_bound():
    import hashlib
    import json
    receipt = json.loads((PACKET / "verification.json").read_text())
    source = PACKET / "func_8010C02C.c"
    assert receipt["source_sha256"] == hashlib.sha256(source.read_bytes()).hexdigest()
    assert set(receipt["builds"]) == {"O2", "O3", "O3_direct_callee_group"}
    for build in receipt["builds"].values():
        assert build["strict_match"]
        assert build["actual_elf_function_bytes"] == 696
        assert build["compared_full_words"] == 174
        assert build["different_words"] == 0
        assert build["unresolved_relocations"] == build["unverified_relocations"] == 0
        assert build["relocation_masks"] == build["nonzero_extra_words"] == 0
    assert not any(receipt["promotion"].values())


@pytest.fixture
def verifier():
    import importlib.util
    spec = importlib.util.spec_from_file_location("seed_vector_verifier", PACKET / "verify.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    if not (module.score.IDO / "cc").is_file():
        pytest.skip("IDO compiler unavailable; run verify.py with the documented toolchain")
    return module


def test_wrong_relocation_target_is_rejected(verifier, tmp_path):
    # This changes only a bound symbol address, so a relocation-blind scorer
    # would miss it. The full-word verifier must reject it.
    source = (PACKET / "func_8010C02C.c").read_text()
    path = tmp_path / "wrong_symbol.c"
    path.write_text(source.replace("D_80150F38", "D_80150F3C"))
    obj = tmp_path / "wrong_symbol.o"
    verifier.score.compile_single(path, "-g0 -O3 -mips2 -G 0 -non_shared", obj)
    with pytest.raises(AssertionError):
        verifier.verify_object(obj)


def test_wrong_actual_extent_is_rejected(verifier, tmp_path):
    path = tmp_path / "short.c"
    path.write_text("int func_8010C02C(void) { return 0; }\n")
    obj = tmp_path / "short.o"
    verifier.score.compile_single(path, "-g0 -O3 -mips2 -G 0 -non_shared", obj)
    with pytest.raises(AssertionError):
        verifier.verify_object(obj)


def test_submission_is_identical_to_reviewed_candidate():
    assert (ROOT / "cloud/matches/func_8010C02C.c").read_bytes() == (
        PACKET / "func_8010C02C.c").read_bytes()
