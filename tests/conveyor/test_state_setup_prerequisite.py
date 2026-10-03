"""Host behavioral test of a partial source region, never a match gate."""
from pathlib import Path
import shutil
import subprocess

import pytest


def test_native_evidenced_fast_prepare(tmp_path):
    cc = shutil.which("cc")
    if cc is None:
        pytest.skip("host C compiler unavailable")
    root = Path(__file__).resolve().parents[2]
    source = Path(__file__).parent / "fixtures/state_setup_prerequisite.c"
    includes = root / "cloud/work/state_setup_prerequisite"
    executable = tmp_path / "fast_prepare"
    subprocess.run([cc, "-std=c89", "-pedantic", "-Wall", "-Wextra", "-Werror",
                    "-I", str(includes), str(source), "-o", str(executable)], check=True)
    subprocess.run([str(executable)], check=True)
