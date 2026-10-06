"""Portable regression checks for the revived real-caller route helper."""
import importlib.util
import json
import os
from pathlib import Path
import shutil
import struct

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_route_crossing_20261006'
spec = importlib.util.spec_from_file_location('dot_route_crossing_proof', PACKET / 'verify.py')
proof = importlib.util.module_from_spec(spec)
spec.loader.exec_module(proof)


def test_recipe_native_and_owned_source_binding():
    receipt = json.loads((PACKET / 'verification.json').read_text())
    assert receipt['result'] == 'MATCH' and receipt['native_bytes'] == 432
    assert receipt['elf_function_bytes'][proof.FN] == 432
    assert receipt['target_relocations'] == 4 and receipt['owned_data_bytes'] == 0
    assert receipt['context']['base_commit'] == proof.BASE
    native = proof.score.targets()[proof.FN]
    assert proof.digest(struct.pack('>' + str(len(native)) + 'I', *native)) == receipt['native_sha256']
    assert receipt['native_sha256'] == receipt['compiled_sha256']
    for name, expected in receipt['own_files'].items():
        assert proof.digest((ROOT / name).read_bytes()) == expected
    assert proof.GROUP.joinpath('group.c').read_text().splitlines()[0] == '/* flags: ' + proof.FLAGS + ' */'


def test_historical_complete_real_caller_and_helper():
    history = Path(os.environ.get('RUSH_ROUTE_HISTORY_REPO', ROOT))
    assert proof.historical_source(history)['complete_source_unchanged_except_header_comment']


def test_native_crossing_boundary_and_narrowing():
    words = proof.score.targets()[proof.FN]
    case = next(proof.fixtures())
    for changes, expected in [({}, 1), ({'range':3}, -1), ({'type':2}, 0),
                              ({'type':2,'who':9}, -1), ({'route':1}, -1),
                              ({'who':0x12340000,'route':0xffff0000},1),
                              ({'points':[(-1,0,0),(0,0,0)]},1)]:
        current = dict(case, **changes)
        assert proof.oracle(current) == expected
        assert proof.execute(words, current) == expected


def test_unknown_and_truncated_native_fail_closed():
    words = list(proof.score.targets()[proof.FN])
    case = next(proof.fixtures())
    words[0] = 0xffffffff
    with pytest.raises(AssertionError, match='unsupported'):
        proof.execute(words, case)
    with pytest.raises(AssertionError, match='instruction fetch'):
        proof.execute(proof.score.targets()[proof.FN][:-1], case)


def test_fresh_complete_compiler_and_behavior_replay():
    if not (proof.score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    if not shutil.which('gcc'):
        pytest.skip('host GCC required for unchanged-source behavioral proof')
    history = Path(os.environ.get('RUSH_ROUTE_HISTORY_REPO', ROOT))
    assert proof.proof(history) == json.loads((PACKET / 'verification.json').read_text())
