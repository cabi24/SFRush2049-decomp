"""A150C behavior and honest NONMATCH metadata, not cartridge acceptance."""
import ctypes
import importlib.util
import json
import os
from pathlib import Path
import random
import shutil
import subprocess

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/encoded_string_mapping'
spec = importlib.util.spec_from_file_location('encoded_mapping_proof', PACKET / 'verify.py')
proof = importlib.util.module_from_spec(spec)
spec.loader.exec_module(proof)


@pytest.fixture(scope='module')
def mapper(tmp_path_factory):
    if not shutil.which('gcc'):
        pytest.skip('requires host C compiler')
    work = tmp_path_factory.mktemp('encoded-mapping-host')
    table = work / 'table.c'
    table.write_text('unsigned short D_8011EAEC[256];\n')
    library = work / 'mapping.so'
    subprocess.run(['gcc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                    '-O2', '-shared', '-fPIC', str(PACKET / 'func_800A150C.c'),
                    str(table), '-o', str(library)], check=True)
    loaded = ctypes.CDLL(str(library))
    function = loaded.func_800A150C
    function.argtypes = [ctypes.POINTER(ctypes.c_ubyte), ctypes.POINTER(ctypes.c_ubyte),
                         ctypes.c_ubyte]
    function.restype = None
    character_map = (ctypes.c_ushort * 256).in_dll(loaded, 'D_8011EAEC')

    def run(encoded, mapping, limit):
        character_map[:] = mapping
        source = (ctypes.c_ubyte * len(encoded))(*encoded)
        output = (ctypes.c_ubyte * 512)(*([0xA5] * 512))
        function(output, source, limit)
        return bytes(output)
    return run


def reference(encoded, mapping, limit):
    """Independent first-index dictionary model preserving the equality stop."""
    first = {}
    for index, code in enumerate(mapping):
        first.setdefault(code, index)
    width = 2 if encoded[0] == 255 else 1
    cursor = 1 if width == 2 else 0
    produced = []
    limit &= 255
    while True:
        code = int.from_bytes(encoded[cursor:cursor + width], 'big')
        if code == 0:
            break
        if code in first:
            produced.append(first[code])
        if len(produced) == limit:
            break
        cursor += width
    produced += [0] * max(0, limit - len(produced))
    return bytes(produced) + bytes([0xA5] * (512 - len(produced)))


def test_directed_mapping_edges(mapper):
    identity = list(range(256))
    duplicates = identity.copy()
    duplicates[0] = 65
    pairs = [0x5000 + index for index in range(256)]
    pairs[3], pairs[7], pairs[255] = 0x0100, 1, 0xFFFF
    cases = [
        (b'\0', identity, 0), (b'\0', identity, 255),
        (b'ABC\0', identity, 2), (b'ABC\0', identity, 5),
        (b'ABC\0', identity, 0), (b'ABC\0', identity, 256),
        (b'ABC\0', duplicates, 3), (b'A\xffB\0', identity, 4),
        (b'AB\0', [0] * 256, 3), (b'AB\0', [0] * 256, 0),
        (b'\xff\0\0', pairs, 4),
        (b'\xff\x01\0\0\x01\xff\xff\0\0', pairs, 5),
        (b'\xff\x01\0\0\x01\xff\xff\0\0', pairs, 0),
        (b'\xff\x77\x77\x01\0\0\0', pairs, 1),
        (b'\xff\x77\x77\x01\0\0\0', pairs, 0),
    ]
    for encoded, mapping, limit in cases:
        assert mapper(encoded, mapping, limit) == reference(encoded, mapping, limit)
    assert mapper(b'ABC\0', identity, 0)[:4] == b'ABC\xa5'
    assert mapper(b'ABC\0', duplicates, 3)[:4] == b'\0BC\xa5'


def test_deterministic_generated_mapping_cases(mapper):
    rng = random.Random(0xA150C)
    for case in range(1000):
        wide = case % 2 == 0
        mapping = [rng.randrange(65536 if wide else 384) for _ in range(256)]
        mapping[0] = mapping[23]  # Preserve first-match selection with duplicates.
        codes = [rng.choice(mapping) if rng.randrange(3) else rng.randrange(1, 65536 if wide else 256)
                 for _ in range(rng.randrange(1, 41))]
        codes = [code for code in codes if code != 0 and (wide or code < 256)]
        if not wide and codes and codes[0] == 255:
            codes[0] = 254
        encoded = (b'\xff' if wide else b'') + b''.join(
            code.to_bytes(2 if wide else 1, 'big') for code in codes) + bytes(2 if wide else 1)
        limit = rng.choice([0, 1, 4, 16, 255, rng.randrange(256)])
        assert mapper(encoded, mapping, limit) == reference(encoded, mapping, limit)


def test_frozen_receipt_is_explicit_nonmatch():
    receipt = json.loads((PACKET / 'verification.json').read_text())
    assert receipt['status'] == 'NONMATCH'
    assert receipt['claims'] == [] and receipt['new_coverage_bytes'] == 0
    assert not receipt['image_gate_run'] and not receipt['full_rom_gate_run']
    assert receipt['source_sha256'] == proof.sha256(PACKET / 'func_800A150C.c')
    assert receipt['comparison']['differing'] == 1
    assert receipt['differing_offsets'] == [0xD4]
    assert receipt['candidate_elf_size_bytes'] == receipt['target_bytes'] == 312


def test_fresh_complete_native_replay(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido'))
    if not (ido / 'cc').is_file():
        pytest.skip('requires pinned IDO')
    fresh = proof.verify(tmp_path)
    saved = json.loads((PACKET / 'verification.json').read_text())
    # The dated whole-catalog fingerprint may change for unrelated accepted work.
    # Keep this function's source, native extent/hash, and complete replay pinned.
    fresh.pop("target_manifest_sha256")
    saved.pop("target_manifest_sha256")
    assert fresh == saved
