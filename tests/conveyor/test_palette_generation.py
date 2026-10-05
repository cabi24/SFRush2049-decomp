"""Complete palette behavior and overflow-safe RGB5551 interpolation."""
from pathlib import Path
import shutil
import subprocess
import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/ipa-groups/dot_palette_generation_20261005'


def test_palette_generation_contracts(tmp_path):
    cc = shutil.which('cc')
    if cc is None:
        pytest.skip('host C compiler unavailable')
    binary = tmp_path / 'palette-contracts'
    subprocess.run([cc, '-std=c99', '-Wall', '-Wextra', '-Werror',
                    '-Wno-pointer-to-int-cast', '-O2',
                    str(PACKET / 'host_test.c'), '-o', str(binary)], check=True)
    result = subprocess.run([str(binary)], check=True, capture_output=True, text=True)
    assert '983040 interpolation cases; 4098 palette cases' in result.stdout
