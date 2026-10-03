"""Authenticated native bounded replay with explicitly stubbed callees."""
import importlib.util
from pathlib import Path
import pytest

ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location('callback_lifetime', ROOT / 'cloud/work/overlay_callback_lifetime/replay.py')
M = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(M)

@pytest.fixture(scope='module')
def game():
    return M.load_game(ROOT)


def setup(game, order, hooks=None):
    vm = M.Replay(game, hooks)
    # Complete caller-save/callee-save spills may read untouched saved registers.
    for address in range(vm.r[29] - 128, vm.r[29] + 128):
        vm.memory[address] = 0
    vm.write(M.HEAD, order[0] if order else 0, 4)
    vm.write(M.FREE, 0, 4)
    vm.write(M.COUNT, len(order), 2)
    vm.write(M.PEAK, len(order), 2)
    for i, node in enumerate(order):
        vm.write(node, order[i+1] if i+1 < len(order) else 0, 4)
        vm.write(node+6, -1, 2)
        vm.write(node+20, 0x8010C974, 4)
    return vm


@pytest.mark.parametrize('index', [0, 1, 2])
def test_native_unlink_head_middle_tail(game, index):
    nodes = [0x90020000, 0x90020100, 0x90020200]
    vm = setup(game, nodes)
    vm.r[4], vm.r[5] = nodes[index], 1
    vm.run(0x8009079C)
    remaining = [n for i, n in enumerate(nodes) if i != index]
    assert vm.read(M.HEAD, 4) == remaining[0]
    assert vm.read(remaining[0], 4) == remaining[1]
    assert vm.read(remaining[1], 4) == 0
    assert vm.read(nodes[index]+20, 4) == 0
    assert vm.read(M.FREE, 4) == nodes[index]
    assert vm.read(M.COUNT, 2) == 2
    assert vm.read(M.PEAK, 2) == 3
    assert not vm.calls


def test_native_release_callee_is_explicit_assumption(game):
    observed = []
    def release(vm):
        observed.append((vm.r[4], vm.r[5], vm.r[6], vm.read(0x90020014, 4)))
    vm = setup(game, [0x90020000], {0x8008AE8C: release})
    vm.write(0x90020006, 7, 2)
    vm.r[4], vm.r[5] = 0x90020000, 1
    vm.run(0x8009079C)
    assert observed == [(7, 1, 15, 0x8010C974)]
    assert vm.read(M.HEAD, 4) == 0
    assert vm.read(0x90020014, 4) == 0


@pytest.mark.parametrize('count', [0, 1, 3])
def test_native_drain_then_updater_has_no_callbacks(game, count):
    nodes = [0x90020000 + 256*i for i in range(count)]
    vm = setup(game, nodes, {0x800B0580: lambda vm: None, 0x800B066C: lambda vm: None})
    vm.run(0x800B0618)
    assert vm.read(M.HEAD, 4) == 0
    assert vm.read(M.COUNT, 2) == 0
    assert all(vm.read(n+20, 4) == 0 for n in nodes)
    vm.r[31] = M.STOP
    vm.run(0x800B0868)
    assert [call[0] for call in vm.calls] == [0x800B0580, 0x800B066C]


def test_native_updater_dispatches_live_callback_without_loader_state(game):
    observed = []
    vm = setup(game, [0x90020000], {0x8010C974: lambda vm: observed.append((vm.r[4],vm.r[5])), 0x800B066C: lambda vm: None})
    vm.run(0x800B0868)
    assert observed == [(0x90020000, 1)]
    # Loader flag addresses were never initialized: a read would fail closed.


def test_record_120_does_not_call_table_plus_8_during_setup(game):
    offset = 0x80117530 + 120*48 - M.BASE
    flags = int.from_bytes(game[offset+18:offset+20], 'big')
    assert flags == 0x022A
    assert flags & 2
    assert not flags & (4 | 0x4000)


def test_execution_bound_is_fail_closed(game):
    vm = setup(game, [])
    with pytest.raises(ValueError, match='outside bounded'):
        vm.run(0x80090798)


def test_native_updater_survives_current_node_retirement(game):
    visited = []
    def retire_current(vm):
        visited.append(vm.r[4])
        continuation = vm.r[31]
        vm.r[31] = M.STOP
        vm.run(0x8009079C)
        vm.r[31] = continuation
    nodes = [0x90020000, 0x90020100, 0x90020200]
    vm = setup(game, nodes, {0x8010C974: retire_current, 0x800B066C: lambda vm: None})
    vm.run(0x800B0868)
    assert visited == nodes
    assert vm.read(M.HEAD, 4) == 0
    assert vm.read(M.COUNT, 2) == 0


def test_uninitialized_data_read_fails_closed(game):
    vm = M.Replay(game)
    with pytest.raises(KeyError):
        vm.run(0x800B0868)


def test_manifest_hashes_bind_reviewed_regions(game):
    import hashlib
    import json
    manifest = json.loads((ROOT / 'cloud/work/overlay_callback_lifetime/evidence.json').read_text())
    assert manifest['game_sha256'] == M.GAME_SHA256
    assert manifest['runtime_residency_proven'] is False
    for region in manifest['ranges']:
        start = int(region['start'], 16) - M.BASE
        end = int(region['end_exclusive'], 16) - M.BASE
        assert hashlib.sha256(game[start:end]).hexdigest() == region['sha256']


def test_unsupported_opcode_fails_closed(game, monkeypatch):
    vm = setup(game, [])
    monkeypatch.setattr(vm, 'instruction', lambda pc: 63 << 26)
    with pytest.raises(ValueError, match='unsupported instruction'):
        vm.run(0x800B0868)


def test_unhooked_external_target_fails_closed(game):
    vm = setup(game, [0x90020000])
    vm.write(0x90020006, 7, 2)
    vm.r[4], vm.r[5] = 0x90020000, 1
    with pytest.raises(ValueError, match='outside bounded'):
        vm.run(0x8009079C)


def test_cyclic_list_exceeds_step_bound(game):
    vm = setup(game, [0x90020000], {0x8010C974: lambda vm: None})
    vm.write(0x90020000, 0x90020000, 4)
    with pytest.raises(ValueError, match='step bound exceeded'):
        vm.run(0x800B0868)
