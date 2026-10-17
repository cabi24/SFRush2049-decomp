"""One bounded protocol-v2 test for car-body collision and preserved closure."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil

import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_mat3_carbody_20261017/groups/func_800AD4C8'
spec=importlib.util.spec_from_file_location('carbody_packet',HERE/'replay.py')
packet=importlib.util.module_from_spec(spec)
spec.loader.exec_module(packet)


def test_complete_carbody_packet(tmp_path):
    receipt=json.loads((HERE/'proof.json').read_text())
    review=json.loads((HERE/'independent_review.json').read_text())
    assert packet.inputs()==receipt['inputs']
    packet.preserved_sources()
    for name,digest in review['source_sha256'].items():
        assert hashlib.sha256((HERE/name).read_bytes()).hexdigest()==digest
    if not shutil.which('cc'):
        pytest.skip('host C compiler unavailable')
    assert packet.semantic_check(tmp_path)==receipt['host_semantics']
    if not (packet.score.IDO/'cc').exists():
        pytest.skip('IDO missing for strict group replay')
    assert packet.verify()==receipt
