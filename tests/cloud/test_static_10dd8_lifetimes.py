"""One bounded packet replay; ~1 s with IDO, no private inputs."""
import importlib.util
import json
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_static_10dd8_lifetimes_20261013'


def test_static_10dd8_packet():
    for tool in ('gcc', 'mips-linux-gnu-as', 'mips-linux-gnu-ld',
                 'mips-linux-gnu-objdump', 'mips-linux-gnu-readelf'):
        if not shutil.which(tool):
            pytest.skip('C compiler or MIPS binutils missing: ' + tool)
    spec = importlib.util.spec_from_file_location('static_10dd8_verify', PACKET / 'verify.py')
    verify = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(verify)
    saved = json.loads((PACKET / 'verification.json').read_text())
    assert verify.inputs() == saved['inputs'], 'source, target, or verification inputs changed'
    result = verify.run()
    assert result == saved, 'receipt evidence changed'
    assert result['result'] == 'MATCH'
    assert result['gnu_link_equal'] and result['current_whole_tu_equal']
    assert result['semantics']['native_and_host_equal']
