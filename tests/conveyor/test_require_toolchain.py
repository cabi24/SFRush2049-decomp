"""V02: a missing toolchain is a failure where the run requires it."""
import importlib.util
from pathlib import Path

_spec = importlib.util.spec_from_file_location(
    "shared_conftest", Path(__file__).resolve().parents[1] / "conftest.py")
shared = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(shared)


def test_toolchain_skip_reasons_are_recognised():
    for reason in ("IDO missing (tools/cloud/setup.sh)", "IDO not installed",
                   "mips-linux-gnu-as absent", "MIPS binutils absent",
                   "needs IDO, tools/mips_to_c and mips-linux-gnu-objdump",
                   "host C compiler unavailable", "gcc absent"):
        assert shared.is_toolchain_skip(reason), reason


def test_missing_private_inputs_stay_skips():
    for reason in ("baserom.us.z64 not present", "corpus data missing",
                   "no conveyor DB — run the matrix first", "derived asm not present",
                   "canonical SDK checkout absent", "corpus unavailable"):
        assert not shared.is_toolchain_skip(reason), reason
