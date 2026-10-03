"""Behavioral contract checks for the unclaimed graphics-init research."""
from pathlib import Path
import shutil
import subprocess
import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/dot_graphics_init_closure'

def test_graphics_init_and_flag_contracts(tmp_path):
    cc = shutil.which('cc')
    if cc is None:
        pytest.skip('host C compiler unavailable')
    output = tmp_path / 'host_test'
    subprocess.run([cc, '-std=c99', '-Wall', '-Wextra', '-Werror', '-O2',
                    *[str(PACKET / name) for name in ('init.c', 'flags.c', 'host_test.c')],
                    '-o', str(output)], check=True)
    result = subprocess.run([str(output)], check=True, capture_output=True, text=True)
    assert '28672 flag grid + 10000 full-width cases' in result.stdout
