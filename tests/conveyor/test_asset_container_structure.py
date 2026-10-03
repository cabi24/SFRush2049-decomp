"""Synthetic, ROM-free tests for the conservative directory research probe."""
import importlib.util
import json
import struct
import zlib
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/asset_container_structure'
spec = importlib.util.spec_from_file_location('asset_directory_probe', PACKET / 'probe.py')
probe = importlib.util.module_from_spec(spec)
spec.loader.exec_module(probe)


def fixture(entries=((b'TEST', 8, 8),), directory=16):
    return struct.pack('>II', directory, len(entries)) + bytes(directory-8) + b''.join(
        struct.pack('>4sII', *e) for e in entries)


def test_generic_unknown_tags_are_structural_not_known_family():
    parsed = probe.parse_directory(fixture())
    assert parsed['family'] == 'unknown_tags'
    assert not parsed['empirical_profile_agrees']
    assert parsed['entries'][0]['interval_bytes'] == 8


def test_known_family_zero_quantity_and_alignment_padding():
    entries = [(b'WHDR', 8, 1), (b'WOBJ', 16, 0), (b'GTLD', 16, 1), (b'GDAT', 24, 1)]
    parsed = probe.parse_directory(fixture(entries, 56))
    assert parsed['empirical_profile_agrees']
    assert [e['inferred_padding_bytes'] for e in parsed['entries']] == [7, 0, 4, 4]


def test_quantity_is_not_a_generic_byte_length():
    parsed = probe.parse_directory(fixture(((b'TEST', 8, 0xffffffff),)))
    assert parsed['entries'][0]['quantity'] == 0xffffffff
    assert not parsed['empirical_profile_agrees']


def test_stride_disagreement_is_reported_not_silently_accepted():
    p = probe.parse_directory(fixture(((b'OBHD', 8, 0xffffffff),)))
    assert not p['entries'][0]['empirical_extent_agrees']


@pytest.mark.parametrize('payload', [b'', b'1234567',
    struct.pack('>II', 16, 0xffffffff) + bytes(20),
    struct.pack('>II', 0x80000010, 1) + bytes(20),
    fixture()[:-1], fixture()+b'x',
    fixture(((b'TEST', 0, 1),)), fixture(((b'TEST', 24, 1),)),
    fixture(((b'TEST', 9, 1),)), fixture(((b'TEST', 16, 1),)),
    fixture(((b'BAD\0', 8, 1),)),
    fixture(((b'TEST', 8, 0), (b'TEST', 8, 0))),
    fixture(((b'ONE ', 8, 0), (b'TWO ', 16, 0), (b'THRE', 8, 0))),
])
def test_rejects_unsupported_or_malformed_directory(payload):
    with pytest.raises(ValueError):
        probe.parse_directory(payload)


def test_exact_inflate_rejects_trailing_truncated_wrong_size_and_bomb():
    output = bytes(4096)
    compressor = zlib.compressobj(wbits=-15)
    source = compressor.compress(output) + compressor.flush()
    assert probe.inflate_exact(source, 4096) == output
    for compressed, size in [(source+b'extra',4096), (source[:-1],4096), (source,4095),
                             (source,4097), (source,-1), (source,True), (b'broken',4096)]:
        with pytest.raises(ValueError):
            probe.inflate_exact(compressed, size)
    with pytest.raises(ValueError):
        probe.inflate_exact(source, 4096, max_output=1024)


def test_union_preserves_accepted_overlap_and_unknown_gaps():
    rows = [dict(rom='0x10', compressed=16), dict(rom='0x15', compressed=11)]
    ledger = probe.interval_ledger(rows, 0x10, 0x25)
    assert sum(r['bytes'] for r in ledger) == 21
    assert [len(r['candidates']) for r in ledger] == [1,2,0]
    assert ledger[-1]['status'] == 'unexplained_gap'
    with pytest.raises(ValueError):
        probe.interval_ledger(rows, 0x11, 0x25)


def test_manifest_consistency_and_nonempty_profile_evidence():
    manifest = json.loads((PACKET / 'manifest.json').read_text())
    assert manifest['summary']['directory_families'] == {'eight_tag':7,'four_tag':19,'six_tag':38}
    rows = [r for r in manifest['rows'] if 'directory' in r]
    assert len(rows) == 64
    assert all(r['directory']['empirical_profile_agrees'] for r in rows)
    boundary = manifest['boundary_evidence']
    assert boundary['table_selects_aligned_candidate']
    assert 'directory' in boundary['candidates'][-1]
    assert all(r['suffix_equals_aligned_output'] for r in boundary['candidates'])


def test_probe_rejects_duplicate_out_of_bounds_and_invalid_lengths():
    output = fixture()
    compressor = zlib.compressobj(wbits=-15)
    data = compressor.compress(output) + compressor.flush()
    row = dict(rom=hex(probe.BASE), compressed=len(data), decompressed=len(output))
    assert probe.probe(data, [row])['summary']['rows'] == 1
    with pytest.raises(ValueError):
        probe.probe(data, [row, row])
    for changes in [dict(rom=hex(probe.BASE-1)), dict(compressed=len(data)+1),
                    dict(compressed=0), dict(compressed=True), dict(decompressed=probe.MAX_OUTPUT+1)]:
        with pytest.raises(ValueError):
            probe.probe(data, [dict(row, **changes)])


def test_rejected_stored_hit_cannot_skip_accepted_nested_candidate():
    # Synthetic model of the earlier census filter-order defect. This neither
    # imports nor changes the upstream scanner, and is not a fix claim.
    import random
    rng = random.Random(2049)
    payload = bytes(rng.randrange(16) for _ in range(4096))
    encoder = zlib.compressobj(wbits=-15)
    inner = encoder.compress(payload) + encoder.flush()
    size = len(inner)
    data = bytes([1]) + struct.pack('<HH', size, size ^ 0xffff) + inner

    def scan(skip_rejected):
        off = 0
        accepted = []
        while off < len(data):
            decoder = zlib.decompressobj(-15)
            try:
                decoded = decoder.decompress(data[off:])
            except zlib.error:
                off += 1
                continue
            if decoder.eof and len(decoded) >= 512:
                used = len(data) - off - len(decoder.unused_data)
                ok = used >= 256 and len(decoded) > 1.15 * used
                if ok:
                    accepted.append(off)
                if skip_rejected or ok:
                    off += used
                    continue
            off += 1
        return accepted

    assert size >= 256
    assert scan(True) == []
    assert 5 in scan(False)
