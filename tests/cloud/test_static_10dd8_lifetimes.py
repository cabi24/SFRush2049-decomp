"""One bounded packet replay; ~1 s with IDO, no private inputs."""
import importlib.util
import json
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_static_10dd8_lifetimes_20261013'


def test_static_10dd8_packet(monkeypatch):
    for tool in ('gcc', 'mips-linux-gnu-as', 'mips-linux-gnu-ld',
                 'mips-linux-gnu-objdump', 'mips-linux-gnu-readelf'):
        if not shutil.which(tool):
            pytest.skip('C compiler or MIPS binutils missing: ' + tool)
    spec = importlib.util.spec_from_file_location('static_10dd8_verify', PACKET / 'verify.py')
    verify = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(verify)
    saved = json.loads((PACKET / 'verification.json').read_text())
    previous = verify.score.ASM_DIR, verify.score.IDO
    assert verify.inputs() == saved['inputs'], 'source, target, or verification inputs changed'
    assert (verify.score.ASM_DIR, verify.score.IDO) == previous
    result = verify.run()
    assert (verify.score.ASM_DIR, verify.score.IDO) == previous
    assert result == saved, 'receipt evidence changed'
    assert result['result'] == 'MATCH'
    assert result['gnu_link_equal'] and result['current_whole_tu_equal']
    assert result['semantics']['native_and_host_equal']

    def fail_command(*args):
        raise RuntimeError('forced replay failure')
    with monkeypatch.context() as patch:
        patch.setattr(verify, 'command', fail_command)
        with pytest.raises(RuntimeError, match='forced replay failure'):
            verify.run()
    assert (verify.score.ASM_DIR, verify.score.IDO) == previous
