"""Pure-Python bounded native/source-contract replay; no IDO dependency."""
import importlib.util
import json
from pathlib import Path

PACKET = Path(__file__).resolve().parents[2] / 'cloud/work/runtime_b_aa8c_entry_20261006'


def test_runtime_b_aa8c_entry_packet():
    spec = importlib.util.spec_from_file_location('aa8c_entry_verify', PACKET / 'verify.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    assert module.replay(PACKET.parents[2]) == json.loads((PACKET / 'verification.json').read_text())


def test_independent_source_semantics_review(tmp_path):
    import subprocess
    import sys
    import pytest
    parser = pytest.importorskip('pycparser.c_parser', reason='pycparser 3.00 parser API required')
    if not hasattr(parser.CParser, '_parse_compound_statement'):
        pytest.skip('pycparser 3.00 parser API required')
    review = PACKET / 'independent-review'
    output = tmp_path / 'independent-source-review.json'
    result = subprocess.run([sys.executable, str(review / 'verify_review.py'),
                             '--packet', str(PACKET),
                             '--reference-root', str(PACKET.parents[2]),
                             '--output', str(output)],
                            text=True, capture_output=True, cwd=tmp_path)
    assert result.returncode == 0, result.stdout + result.stderr
    assert json.loads(output.read_text()) == json.loads((review / 'review.json').read_text())
