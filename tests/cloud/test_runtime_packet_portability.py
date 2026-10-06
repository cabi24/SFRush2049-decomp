"""Integration portability retains every packet, native, ELF and behavior proof."""
import copy
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKETS = {
    142: 'runtime_b_reset_20261006',
    143: 'frontier/dot_runtime_a_callback_20261006',
    144: 'runtime_b_texture_ring_20261006',
    145: 'boot_tail/BT03-voice-unblock-topology',
    146: 'frontier/dot_runtime_a_formatted_20261006',
    147: 'runtime_b_object_phases_20261006',
    148: 'boot_tail/BT03-voice-free-donor',
    149: 'runtime_b_teardown_20261006',
    150: 'frontier/dot_runtime_a_menu_loop_20261006',
    151: 'runtime_b_initializer_20261006',
}


def load(number):
    packet = ROOT / 'cloud/work' / PACKETS[number]
    spec = importlib.util.spec_from_file_location('portable_runtime_' + str(number), packet / 'verify.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    receipt = json.loads((packet / ('evidence.json' if number in (145, 148) else 'verification.json')).read_text())
    return packet, module, receipt


def leaves(value, path=()):
    if isinstance(value, dict):
        for key, child in value.items():
            yield from leaves(child, path + (key,))
    elif isinstance(value, list):
        for key, child in enumerate(value):
            yield from leaves(child, path + (key,))
    else:
        yield path


def mutate(value, path):
    result = copy.deepcopy(value)
    parent = result
    for part in path[:-1]:
        parent = parent[part]
    parent[path[-1]] = 'deliberate proof drift'
    return result


@pytest.mark.parametrize('number', PACKETS)
def test_packet_verifier_and_harness_remain_bound(number):
    packet, module, receipt = load(number)
    proof = module.portable_receipt(receipt)
    verifier_sha = hashlib.sha256((packet / 'verify.py').read_bytes()).hexdigest()
    if 'verifier_sha256' in proof:
        assert proof['verifier_sha256'] == verifier_sha
    elif 'packet_sha256' in proof:
        assert proof['packet_sha256']['verify.py'] == verifier_sha
    elif 'input_sha256' in proof:
        assert proof['input_sha256']['packet:verify.py'] == verifier_sha
    else:
        assert proof['inputs_sha256'][str((packet / 'verify.py').relative_to(ROOT))] == verifier_sha
    assert 'test_packet.py' not in receipt.get('packet_sha256', {})
    for field in ('inputs_sha256', 'input_sha256'):
        for path, digest in proof.get(field, {}).items():
            actual = packet / path[7:] if path.startswith('packet:') else ROOT / path
            assert hashlib.sha256(actual.read_bytes()).hexdigest() == digest
    for path, digest in proof.get('packet_sha256', {}).items():
        assert hashlib.sha256((packet / path).read_bytes()).hexdigest() == digest


@pytest.mark.parametrize('number', PACKETS)
def test_only_historical_provenance_can_drift(number):
    _, module, receipt = load(number)
    proof = module.portable_receipt(receipt)
    # All retained scalar facts remain equality-bound, including full ELF,
    # relocations, selected native words/addresses, own data, and compiler IDs.
    for path in leaves(proof):
        assert module.portable_receipt(mutate(receipt, path)) != proof, path
    for field in ('inputs_sha256', 'input_sha256'):
        for path in receipt.get(field, {}):
            if path not in proof.get(field, {}):
                assert module.portable_receipt(mutate(receipt, (field, path))) == proof
    for field in ('protected_targets', 'helper_protected_targets', 'scorer_sha256', 'owndata_sha256'):
        if field in receipt:
            assert module.portable_receipt(mutate(receipt, (field,))) == proof
    changed = copy.deepcopy(receipt)
    changed['unknown_proof_field'] = 'must remain bound'
    assert module.portable_receipt(changed) != proof
    assert receipt == json.loads(json.dumps(receipt))


@pytest.mark.parametrize('number', [143, 146])
def test_full_canonical_o3_replay(number, tmp_path):
    from tools.cloud import score
    if not (score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    packet, module, receipt = load(number)
    if number == 142:
        replay = module.prove()
    else:
        output = tmp_path / 'receipt.json'
        result = subprocess.run([sys.executable, str(packet / 'verify.py'), '--out', str(output)],
                                cwd=tmp_path, text=True, capture_output=True)
        assert result.returncode == 0, result.stdout + result.stderr
        replay = json.loads(output.read_text())
    assert module.portable_receipt(replay) == module.portable_receipt(receipt)
    source = module.SOURCE
    assert source.read_text().splitlines()[0] == '/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
