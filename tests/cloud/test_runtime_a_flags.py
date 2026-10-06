"""Image-qualified contract, source binding and bounded native regression."""
import copy
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import struct
import shutil
import subprocess
import sys

import pytest
from tools.cloud import score

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_runtime_a_flags_20261006'
spec = importlib.util.spec_from_file_location('runtime_a_flags_native', PACKET / 'native.py')
native = importlib.util.module_from_spec(spec)
spec.loader.exec_module(native)
proof_spec = importlib.util.spec_from_file_location('runtime_a_flags_proof', PACKET / 'verify.py')
v = importlib.util.module_from_spec(proof_spec); proof_spec.loader.exec_module(v)


def target(monkeypatch):
    monkeypatch.setattr(score, 'ASM_DIR', ROOT / 'asm/us/ovl_a')
    return score.targets()['func_80393004']


def test_receipt_is_bound_to_current_sources_and_target(monkeypatch):
    receipt = json.loads((PACKET / 'verification.json').read_text())
    for name, digest in v.portable(receipt)['input_sha256'].items():
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


def test_portability_keeps_packet_and_native_proof_strict():
    saved = json.loads((PACKET / 'verification.json').read_text())
    changed = copy.deepcopy(saved)
    for name in v.PROVENANCE_INPUTS:
        changed['input_sha256'][name] = 'different integration metadata revision'
    assert v.portable(changed) == v.portable(saved)
    assert saved == json.loads((PACKET / 'verification.json').read_text())
    for field in ['function', 'producer', 'direct_callers', 'source_sha256', 'recipe', 'assembler_addition', 'tool_sha256', 'behavior', 'rejected_semantic_mutants', 'rejected_elf_controls']:
        mutant = copy.deepcopy(changed)
        mutant[field] = 'proof drift'
        assert v.portable(mutant) != v.portable(saved), field


@pytest.mark.parametrize('field', ['gnu_ld', 'host_cc'])
def test_portability_ignores_host_tool_version_provenance(field):
    saved = json.loads((PACKET / 'verification.json').read_text())
    changed = copy.deepcopy(saved)
    changed[field] = 'another valid toolchain version'
    assert changed[field] != saved[field]
    assert v.portable(changed) == v.portable(saved)
    assert field not in v.portable(changed)
    assert saved == json.loads((PACKET / 'verification.json').read_text())
    assert changed[field] == 'another valid toolchain version'


@pytest.mark.parametrize(('section', 'field'), [
    ('function', 'target_sha256'), ('function', 'gnu_linked_sha256'),
    ('function', 'elf_function_size'), ('function', 'section_size'),
    ('function', 'alignment_zero_bytes'), ('function', 'relocations'),
    ('function', 'owned_data_bytes'), ('behavior', 'trace_sha256'),
    ('tool_sha256', 'cc'), ('tool_sha256', 'as1'),
])
def test_host_tool_portability_does_not_hide_proof_drift(section, field):
    saved = json.loads((PACKET / 'verification.json').read_text())
    changed = copy.deepcopy(saved)
    changed['gnu_ld'] = 'another linker version'
    changed['host_cc'] = 'another host compiler version'
    changed[section][field] = 'proof drift'
    assert v.portable(changed) != v.portable(saved)


def require_replay_toolchain():
    missing = ['IDO ' + name for name in ('cc', 'cfe', 'uopt', 'ugen', 'as1')
               if not (v.score.IDO / name).is_file()]
    missing += [name for name in ('mips-linux-gnu-ld', 'gcc')
                if not shutil.which(name)]
    if missing:
        reason = ('pinned IDO, MIPS GNU linker and host C compiler required; '
                  'missing: ' + ', '.join(missing))
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1':
            pytest.fail('REQUIRE_TOOLCHAIN=1: ' + reason)
        pytest.skip(reason)


@pytest.mark.parametrize('strict', [False, True])
@pytest.mark.parametrize('missing', ['cc', 'cfe', 'uopt', 'ugen', 'as1',
                                     'mips-linux-gnu-ld', 'gcc'])
def test_fresh_replay_missing_tool_guard(monkeypatch, tmp_path, strict, missing):
    ido = tmp_path / 'ido'
    ido.mkdir()
    for name in ('cc', 'cfe', 'uopt', 'ugen', 'as1'):
        if name != missing:
            (ido / name).touch()
    monkeypatch.setattr(v.score, 'IDO', ido)
    monkeypatch.setattr(shutil, 'which',
                        lambda name: None if name == missing else '/tool/' + name)
    monkeypatch.setenv('REQUIRE_TOOLCHAIN', '1' if strict else '0')
    expected = pytest.fail.Exception if strict else pytest.skip.Exception
    with pytest.raises(expected, match=missing):
        require_replay_toolchain()


def test_fresh_o3_replay(tmp_path):
    require_replay_toolchain()
    out = tmp_path / 'verification.json'
    subprocess.run([sys.executable, str(PACKET / 'verify.py'), '--output', str(out)], check=True, capture_output=True, text=True)
    saved = json.loads((PACKET / 'verification.json').read_text())
    assert v.portable(json.loads(out.read_text())) == v.portable(saved)


def test_verified_submission_recipe_is_exact_o3():
    assert v.SOURCE.read_text().splitlines()[0] == '/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
    assert v.FLAGS == '-g0 -O3 -mips2 -G 0 -non_shared'
