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
    for address in ('8001E440', '8001E688', '8001E7BC', '8001E864', '8001E940'):
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
    return {'schema_version': 1, 'scope': 'External address and native ABI proof only; original scalar/table contents are unavailable and are neither inferred nor replaced.',
        'native_abi': {'func_8001E440': 'u16 input in a0; u16 result in v0; single-precision external scalar multiply then unsigned-word conversion',
            'func_8001E688': 'double input in f12/f13, double result in f0/f1; unsigned-word truncation followed by double conversion',
            'func_8001E7BC': 'u16 phase input; signed-short result; phase modulo4096 and four reflected/sign quadrants',
            'func_8001E864': 'key pointer, base pointer, int count, int stride, genuine fifth two-pointer comparator returning int; pointer result; native frame56',
            'func_8001E940': 'u32 output pointer plus byte-state pointer; actual helper80019AA8 returns u32 after reading state[75] and tableD8004FA20; output load follows helper call'},
        'native_external_data': {'D_8002D924': {'address': '0x8002D924', 'type': 'extern const float', 'contents_known': False},
            'D_8002CC54': {'address': '0x8002CC54', 'type': 'extern const short[1024]', 'contents_known': False, 'reflected_last_element_address': '0x8002D452', 'last_element_addend': 2046}},
        'rows': rows}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = json.dumps(run(), indent=2) + '\n'
    if args.check:
        if result != (WORK / 'abi_proof.json').read_text():
            raise SystemExit('ABI/address metadata drift')
        print('C11 numeric external-address metadata reproduced')
    else:
        print(result, end='')
