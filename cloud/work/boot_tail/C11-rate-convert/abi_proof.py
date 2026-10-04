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
    for address in ('8001E50C',):
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
        'scope': 'Native reference addresses and actual ABI only; original external float-array lengths/values and rate contents remain unknown.',
        'native_abi': 'u32(u8 note,u32 encoded_value); sole canonical caller1A5D8 supplies a masked byte from its second word and the packed voice word at+92. Call-free body returns the full unsigned conversion word.',
        'external_views': {'D_8002C640': 'extern const float[] at8002C640; positive note-reference difference indexes this view',
            'D_8002C840': 'extern const float[] at8002C840; positive reference-note difference indexes this view; base is128 float slots afterC640',
            'D_8004F800': 'extern signed int rate at8004F800; native signed-word-to-float conversion, not unsigned'},
        'original_lengths_and_values_known': False,
        'rows': rows}



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = json.dumps(run(), indent=2) + '\n'
    if args.check:
        if result != (WORK / 'abi_proof.json').read_text():
            raise SystemExit('ABI/address metadata drift')
        print('C11 rate-conversion ABI/address metadata reproduced')
    else:
        print(result, end='')
