"""Image-qualified contract, source binding and bounded native regression."""
import hashlib
import importlib.util
import json
from pathlib import Path
import struct

import pytest
from tools.cloud import score

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_runtime_a_flags_20261006'
spec = importlib.util.spec_from_file_location('runtime_a_flags_native', PACKET / 'native.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)


def target(monkeypatch):
    monkeypatch.setattr(score, 'ASM_DIR', ROOT / 'asm/us/ovl_a')
    return score.targets()['func_80393004']


def test_receipt_is_bound_to_current_sources_and_target(monkeypatch):
    receipt = json.loads((PACKET / 'verification.json').read_text())
    for name, digest in receipt['input_sha256'].items():
        assert hashlib.sha256((ROOT / name).read_bytes()).hexdigest() == digest, name
    words = target(monkeypatch)
    assert hashlib.sha256(struct.pack('>45I', *words)).hexdigest() == receipt['function']['target_sha256']
    assert receipt['image']['name'] == 'A'
    assert receipt['function']['address'] == '0x80393004'
    assert receipt['function']['size'] == 180
    assert receipt['function']['owned_data_bytes'] == 0
    assert receipt['score'] == 'MATCH'


@pytest.mark.parametrize('index', range(4))
@pytest.mark.parametrize('count', [-2147483645, -4, -3, -2, -1, 0, 1, 7, 8, 9, 2147483647])
def test_directed_contract(monkeypatch, index, count):
    words = target(monkeypatch)
    counts = [0, 4, -4, 8]
    counts[index] = count
    initial = native.fixture(index, counts, 72)
    result = native.run(words, initial)
    assert result['memory'] == native.oracle(initial, index, count)
    assert result['coverage'] == set(range(0, 180, 4))
    assert result['branches'] == {(164, True), (164, False)}


@pytest.mark.parametrize('count', [-2147483648, -2147483647, -2147483646])
def test_underflow_limits_are_discrepancies(monkeypatch, count):
    initial = native.fixture(0, [count, 0, 0, 0], 91)
    result = native.run(target(monkeypatch), initial)
    expected = native.oracle(initial, 0, count)
    assert result['memory'] != expected
    assert result['memory'][native.ROW0 + 6] == 1
    assert expected[native.ROW0 + 6] == 0


@pytest.mark.parametrize('index', [-128, -1, 4, 127])
def test_unestablished_slots_are_rejected(monkeypatch, index):
    initial = native.fixture(index, [0, 0, 0, 0], 20)
    with pytest.raises(AssertionError, match='invalid count slot'):
        native.run(target(monkeypatch), initial)


def test_native_extent_is_not_truncated(monkeypatch):
    words = target(monkeypatch)
    initial = native.fixture(0, [0, 0, 0, 0], 20)
    for body in (words[:-1], words + [0]):
        with pytest.raises(AssertionError, match='extent'):
            native.run(body, initial)


def test_image_b_is_not_the_target(monkeypatch):
    monkeypatch.setattr(score, 'ASM_DIR', ROOT / 'asm/us/ovl_b')
    manifest = score.target_manifest()
    data = json.loads(score.verified_bytes(score.ASM_DIR / 'extents.json', manifest))
    matches = [f for f in data['functions']
               if int(f['address'], 16) <= native.ENTRY < int(f['address'], 16) + f['size']]
    assert len(matches) == 1
    assert matches[0]['name'] == 'func_80392FE4'
    assert matches[0]['size'] == 1332
