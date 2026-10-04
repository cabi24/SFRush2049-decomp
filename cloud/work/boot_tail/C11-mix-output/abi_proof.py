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
    for address in ('8001E0E0',):
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
        'scope': 'External address and native eight-input ABI proof only; all float table/scalar contents remain unknown and are not defined in the candidate.',
        'native_abi': 'void(u16*,u16*,u32,u32,u32,u16*,u32,u16*); only canonical caller14A74 independently matches with four output pointers at real record offsets66,64,68,70',
        'declared_data_views': {'D_8002CA40': '129 float entries, base8002CA40 through last8002CC40; bounded volume/aux index0..127 plus next',
            'D_8002CC44': 'four-float view at8002CC44 through8002CC50; tested pan/span0..800000 produces index0..2 plus next',
            'D_8002D910': 'extern float scale read once and reused; unknown contents',
            'D_8002D914': 'extern float span-output multiplier; unknown contents',
            'D_8002D918': 'extern float special-pan left multiplier; unknown contents',
            'D_8002D91C': 'extern float special-pan right multiplier; unknown contents',
            'D_8002D920': 'extern float auxiliary multiplier; unknown contents'},
        'rows': rows}



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = json.dumps(run(), indent=2) + '\n'
    if args.check:
        if result != (WORK / 'abi_proof.json').read_text():
            raise SystemExit('ABI/address metadata drift')
        print('C11 mixer external-address metadata reproduced')
    else:
        print(result, end='')
