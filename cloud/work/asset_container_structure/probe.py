#!/usr/bin/env python3
"""Bounded, metadata-only trailing-directory research probe (Python 3.9+).

Never extracts payloads. A valid directory is not a proven compressed boundary.
Quantities are opaque except in the explicitly empirical profile check.
"""
import argparse
import collections
import hashlib
import json
import struct
import zlib
from pathlib import Path

BASE = 0x10000
GAME_START = 0xB0CB10
MAX_OUTPUT = 8 * 1024 * 1024
GAME_SHA256 = 'bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d'
# Empirical strides: agreement is a format hypothesis, not consumer proof.
STRIDES = dict(IMAG=1, TXLD=1, OBHD=88, PLHD=24, TXHD=36, OBJS=1,
               PATH=1, PTHD=36, WHDR=1, WOBJ=104, GTLD=4, GDAT=28)
FAMILIES = {
    ('IMAG', 'TXLD', 'OBHD', 'PLHD', 'TXHD', 'OBJS'): 'six_tag',
    ('IMAG', 'TXLD', 'OBHD', 'PLHD', 'TXHD', 'OBJS', 'PATH', 'PTHD'): 'eight_tag',
    ('WHDR', 'WOBJ', 'GTLD', 'GDAT'): 'four_tag',
}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def inflate_exact(source, expected, max_output=MAX_OUTPUT):
    if not isinstance(expected, int) or isinstance(expected, bool) or not 0 <= expected <= max_output:
        raise ValueError('invalid or excessive expected output length')
    decoder = zlib.decompressobj(-15)
    try:
        output = decoder.decompress(source, expected + 1)
    except zlib.error as exc:
        raise ValueError('invalid raw DEFLATE') from exc
    if len(output) != expected or not decoder.eof or decoder.unused_data or decoder.unconsumed_tail:
        raise ValueError('output size, exact EOF, or trailing input mismatch')
    return output


def parse_directory(payload):
    """Validate a conservative structural profile; reject unsupported layouts.

    The final word of each entry is an opaque quantity, NOT always byte size.
    interval_end is inferred from the next entry, not from that quantity.
    """
    if len(payload) < 8:
        raise ValueError('short header')
    directory, count = struct.unpack_from('>II', payload)
    if directory < 8 or directory % 8 or count == 0 or count > (len(payload) - 8) // 12:
        raise ValueError('invalid directory offset or count')
    if directory + 12 * count != len(payload):
        raise ValueError('directory does not end exactly at output end')
    entries = []
    seen = set()
    for index in range(count):
        pos = directory + 12 * index
        tag_bytes, offset, quantity = struct.unpack_from('>4sII', payload, pos)
        if any(c < 32 or c > 126 for c in tag_bytes):
            raise ValueError('non-printable tag')
        tag = tag_bytes.decode('ascii')
        if tag in seen:
            raise ValueError('duplicate tag unsupported')
        seen.add(tag)
        if offset < 8 or offset > directory or offset % 8:
            raise ValueError('entry offset outside aligned payload region')
        if entries and offset < entries[-1]['offset']:
            raise ValueError('unordered entry offsets unsupported')
        entries.append(dict(tag=tag, offset=offset, quantity=quantity))
    if entries[0]['offset'] != 8:
        raise ValueError('first entry does not start immediately after header')
    for index, entry in enumerate(entries):
        end = entries[index + 1]['offset'] if index + 1 < count else directory
        entry['interval_end'] = end
        entry['interval_bytes'] = end - entry['offset']
        if entry['quantity'] and end == entry['offset']:
            raise ValueError('nonzero quantity in empty interval')
    family = FAMILIES.get(tuple(e['tag'] for e in entries), 'unknown_tags')
    profile = family != 'unknown_tags'
    for entry in entries:
        stride = STRIDES.get(entry['tag'])
        if stride is not None:
            size = stride * entry['quantity']
            aligned = (size + 7) // 8 * 8
            agrees = aligned == entry['interval_bytes']
            entry['empirical_stride'] = stride
            entry['empirical_extent_agrees'] = agrees
            if agrees:
                entry['inferred_padding_bytes'] = aligned - size
            profile = profile and agrees
        else:
            profile = False
    return dict(directory_offset=directory, entry_count=count, family=family,
                empirical_profile_agrees=profile, entries=entries)


def interval_ledger(rows, start, end):
    """Partition union and gaps without double-counting alternative starts."""
    if start >= end:
        raise ValueError('empty ledger scope')
    ranges = []
    for row in rows:
        left, right = int(row['rom'], 16), int(row['rom'], 16) + row['compressed']
        if left < start or right > end or left >= right:
            raise ValueError('range outside ledger scope')
        ranges.append((left, right, row['rom']))
    cuts = sorted({start, end} | {p for left, right, _ in ranges for p in (left, right)})
    result = []
    for left, right in zip(cuts, cuts[1:]):
        owners = sorted(name for a, b, name in ranges if a <= left and right <= b)
        result.append(dict(start=hex(left), end=hex(right), bytes=right-left,
                           candidates=owners, status='candidate_covered' if owners else 'unexplained_gap'))
    return result


def probe(data, rows, base=BASE):
    records = []
    seen = set()
    for row in rows:
        off = int(row['rom'], 16)
        size = row['compressed']
        if off in seen or not isinstance(size, int) or isinstance(size, bool) or size <= 0:
            raise ValueError('duplicate start or invalid compressed length')
        seen.add(off)
        local = off - base
        if local < 0 or local + size > len(data):
            raise ValueError('stream outside input')
        source = data[local:local+size]
        output = inflate_exact(source, row['decompressed'])
        record = dict(rom=hex(off), compressed=size, decompressed=len(output),
                      end=hex(off+size), compressed_sha256=sha(source), decompressed_sha256=sha(output))
        if off < GAME_START:
            try:
                record['directory'] = parse_directory(output)
            except ValueError as exc:
                record['directory_rejection'] = str(exc)
        records.append(record)
    pregame = [r for r in records if int(r['rom'], 16) < GAME_START]
    if not pregame:
        raise ValueError('no pre-game rows')
    ledger = interval_ledger(pregame, min(int(r['rom'], 16) for r in pregame), GAME_START)
    groups = collections.defaultdict(list)
    for row in pregame:
        groups[row['decompressed_sha256']].append(row['rom'])
    counts = collections.Counter(r['directory']['family'] for r in pregame if 'directory' in r)
    return dict(schema_version=1, input_sha256=sha(data), input_bytes=len(data), input_rom_base=hex(base),
                scope='supplied candidates only; no completeness or matching claim',
                summary=dict(rows=len(records), pregame_rows=len(pregame), directory_families=dict(sorted(counts.items())),
                             union_candidate_bytes=sum(i['bytes'] for i in ledger if i['candidates']),
                             unexplained_gap_bytes=sum(i['bytes'] for i in ledger if not i['candidates']),
                             duplicate_payloads=[v for v in groups.values() if len(v)>1]),
                rows=records, interval_ledger=ledger)


def boundary_evidence(data, base=BASE):
    """Authenticate the known ambiguous starts and sparse loader-table evidence.

    The input table field is read from the decompressed game, never exported.
    This is not a general table-extent claim or a scanner.
    """
    end = 0x399363
    lengths = {0x36BCA5: 390231, 0x36BCAB: 390083, 0x36BCB0: 390064}
    candidates = []
    payloads = {}
    for off, size in lengths.items():
        source = data[off-base:end-base]
        output = inflate_exact(source, size)
        payloads[off] = output
        record = dict(rom=hex(off), compressed=end-off, decompressed=len(output),
                      end=hex(end), compressed_sha256=sha(source), decompressed_sha256=sha(output))
        try:
            record['directory'] = parse_directory(output)
        except ValueError as exc:
            record['directory_rejection'] = str(exc)
        candidates.append(record)
    aligned = payloads[0x36BCB0]
    for record in candidates:
        output = payloads[int(record['rom'], 16)]
        extra = len(output) - len(aligned)
        record['prefix_bytes_before_aligned_output'] = extra
        record['suffix_equals_aligned_output'] = output[extra:] == aligned
    # Exact census bounds for the game stream; changes fail closed.
    game_rows = json.loads((Path(__file__).with_name('streams.json')).read_text())
    row = next(r for r in game_rows if int(r['rom'], 16) == GAME_START)
    source = data[GAME_START-base:GAME_START-base+row['compressed']]
    game = inflate_exact(source, row['decompressed'])
    if sha(game) != GAME_SHA256:
        raise ValueError('game identity mismatch; refusing native table interpretation')
    field = 0x8011B5BC + 4 * 60
    index = field - 0x80086A50
    if index < 0 or index + 4 > len(game):
        raise ValueError('loader-table field outside game image')
    pointer = struct.unpack_from('>I', game, index)[0]
    codec_field = 0x80123564 + 4 * 60
    codec = struct.unpack_from('>I', game, codec_field - 0x80086A50)[0]
    return dict(candidates=candidates,
                candidate_union=interval_ledger(candidates, min(lengths), end),
                game_sha256=sha(game), loader_table_field=dict(table_vram='0x8011b5bc', slot=60,
                    field_vram=hex(field), game_offset=hex(index), rom_pointer=hex(pointer)),
                codec_field=dict(field_vram=hex(codec_field), value=codec),
                table_selects_aligned_candidate=pointer == 0x36BCB0 and codec == 2)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('data', type=Path)
    parser.add_argument('streams', type=Path)
    parser.add_argument('--out', type=Path, required=True)
    args = parser.parse_args()
    data = args.data.read_bytes()
    result = probe(data, json.loads(args.streams.read_text()))
    result['boundary_evidence'] = boundary_evidence(data)
    args.out.write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result['summary'], indent=2))


if __name__ == '__main__':
    main()
