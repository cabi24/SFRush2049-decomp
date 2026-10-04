#!/usr/bin/env python3
"""Metadata-only external-address/ABI evidence; never exports object or table bytes."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile
ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from tools.cloud import score


def run():
    score.ASM_DIR = ROOT / 'asm/us/boot_tail'
    image = score.image_symbols()
    rows = []
    for address in ('80015A0C', '80016998'):
        name = 'func_' + address
        source = ROOT / 'cloud/matches/boot_tail' / (name + '.c')
        if not source.exists():
            source = WORK / (name + '_NONMATCH.c')
        with tempfile.TemporaryDirectory(prefix='numeric-address-') as t:
            obj = Path(t) / 'candidate.o'
            score.compile_single(source, '-g0 -O2 -mips2 -G 0 -non_shared', obj)
            data, sections = score._elf(obj)
            text_index = score._text_index(sections)
            relocations = []
            for sec in sections:
                if sec['type'] != 9 or sec['info'] != text_index:
                    continue
                symbols = score._symbol_table(data, sections, sec['link'])
                for off in range(0, sec['size'], 8):
                    site, info = struct.unpack_from('>II', data, sec['off'] + off)
                    symbol = symbols[info >> 8]
                    name_ = symbol['name']
                    resolved = image.get(name_, score.address_named(name_))
                    relocations.append({'text_offset': site, 'type': info & 255,
                        'symbol': name_, 'symbol_section': symbol['section'],
                        'address': ('0x%08X' % resolved) if resolved is not None else None,
                        'address_source': 'canonical symbol map' if name_ in image else 'canonical address-named external'})
            rows.append({'name': name, 'source_sha256': hashlib.sha256(source.read_bytes()).hexdigest(),
                'data_section_bytes': {s['name']: s['size'] for s in sections if s['name'] in ('.data', '.rodata', '.rdata', '.sdata', '.bss', '.sbss')},
                'text_relocations': relocations})
    return {'schema_version': 1,
        'scope': 'Native field/stride/call ABI and address proof; external contents are not published.',
        'native_abi': {'func_80015A0C': 'int(u16 key, void *payload, u16 parameter); actual15058 wrapper supplies key, payload+12 and halfword at+10',
            'func_80016998': 'int(u16 key), explicitly always returns0; reviewed150C8 wrapper supplies the single u16'},
        'actual_calls': 'Only no-input synchronization wrappers14594 and145DC in retained bodies; complete canonical helpers were inspected and are unchanged.',
        'native_layouts': {'insertion_record': '12 bytes: payload pointer0, id4, parameter6, references8, two unmodified tail bytes10..11',
            'insertion_globals': 'signed count8003CE18;256 records at8003CE20',
            'removal_record': '8 bytes: payload pointer0, id4, references6',
            'range': '4 bytes: unsigned-short count0, first2;512 ranges at8003DA28',
            'removal_globals': 'signed count8003DA20;2048 entries at8003E228'},
        'host_layout_limit': 'Host packed pointer-containing records are16 and12 bytes, respectively. Host field-model checks are separate from native IDO width assertions and whole-object instruction replay.',
        'rows': rows}



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = json.dumps(run(), indent=2) + '\n'
    if args.check:
        if result != (WORK / 'abi_proof.json').read_text():
            raise SystemExit('ABI/address metadata drift')
        print('BT03 final-resource ABI/address metadata reproduced')
    else:
        print(result, end='')
