#!/usr/bin/env python3
"""Compile the real group and emit sanitized full-extent verification evidence."""
import argparse
import contextlib
import hashlib
import io
import json
from pathlib import Path
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[3]
PACKET = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score


def sha(data):
    return hashlib.sha256(data).hexdigest()


def verify():
    accepted = ROOT / 'src/blob/groups/frontier_pad_config/group.c'
    source = PACKET / 'group.c'
    unchanged = source.read_bytes().startswith(accepted.read_bytes())
    if not unchanged:
        raise AssertionError('Accepted context is no longer the unchanged source prefix')
    spec = json.loads((PACKET / 'group.json').read_text())
    receipt = {
        'baseline': 'cf10b339',
        'status': 'COMPLETE-NONMATCH',
        'flags': spec['flags'],
        'source_sha256': sha(source.read_bytes()),
        'accepted_context_sha256': sha(accepted.read_bytes()),
        'accepted_context_unchanged': unchanged,
        'compiler_sha256': {name: sha(Path(score.ido(name)).read_bytes())
                            for name in ('cc', 'uopt', 'ugen', 'as1')},
        'bodies': {},
        'coverage_claim': False,
    }
    with tempfile.TemporaryDirectory(prefix='pad-reset-verify-') as directory:
        obj = Path(directory) / 'group.o'
        score.compile_group(PACKET, obj)
        data, sections = score._elf(obj)
        functions = {sym['name']: sym
                     for i, section in enumerate(sections) if section['type'] == 2
                     for sym in score._symbol_table(data, sections, i)
                     if sym['type'] == 2}
        words = score.text_words(obj)
        for name in ['audio_channel_reset'] + spec['context']:
            symbol = functions[name]
            begin, size = symbol['value'], symbol['size']
            target = score.targets()[name]
            resolved, masks, unresolved, unverified, errors = score.relocate(
                obj, words, begin, begin + size, score.image_symbols())
            actual = resolved[begin // 4:(begin + size) // 4]
            differing = sum(a != b for a, b in zip(actual, target))
            differing += abs(len(actual) - len(target))
            with contextlib.redirect_stdout(io.StringIO()):
                canonical = score.compare(obj, name, show=0)
            row = {
                'address': hex(score.image_symbols()[name]),
                'elf_symbol_bytes': size,
                'target_bytes': len(target) * 4,
                'exact_extent': size == len(target) * 4,
                'full_relocated_differing_words': differing,
                'canonical_differing_words': canonical.differing,
                'canonical_extra_nonzero_words': canonical.extra_words,
                'unresolved': unresolved,
                'unverified': unverified,
                'relocation_errors': errors,
                'masked_sites': len(masks),
                'relocated_body_sha256': sha(struct.pack('>' + str(len(actual)) + 'I', *actual)),
                'target_body_sha256': sha(struct.pack('>' + str(len(target)) + 'I', *target)),
            }
            row['strict_match'] = (row['exact_extent'] and differing == 0 and
                                   not masks and not unresolved and not unverified and not errors)
            receipt['bodies'][name] = row
    expected = receipt['bodies']['audio_channel_reset']
    assert expected['elf_symbol_bytes'] == expected['target_bytes'] == 164
    assert expected['full_relocated_differing_words'] == 15
    assert not expected['strict_match']
    assert not expected['unresolved'] and not expected['unverified']
    assert not expected['relocation_errors'] and not expected['masked_sites']
    assert all(receipt['bodies'][name]['strict_match'] for name in spec['context'])
    return receipt


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    encoded = json.dumps(verify(), indent=2) + '\n'
    if args.output:
        args.output.write_text(encoded)
    else:
        print(encoded, end='')
