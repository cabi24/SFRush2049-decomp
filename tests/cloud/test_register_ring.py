"""Validate diagnostic scratch selection without compiler or retail bytes."""
import importlib.util
from pathlib import Path
import struct

import pytest

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location('register_ring_replay',
                                            ROOT / 'cloud/work/register-ring/replay.py')
replay = importlib.util.module_from_spec(spec)
spec.loader.exec_module(replay)


def test_reserved_record_requires_one_aligned_expected_record():
    record = struct.pack('>4I', 0x68000000, 1, 6, 2)
    assert replay.reserved_record(b'\0' * 8 + record + b'\0' * 4) == 8
    for invalid in (b'', record[:-1], b'\0' + record, record * 2,
                    struct.pack('>4I', 0x68000000, 1, 12, 2)):
        with pytest.raises(ValueError):
            replay.reserved_record(invalid)
