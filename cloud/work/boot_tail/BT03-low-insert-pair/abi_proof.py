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
    for address in ('80015D68', '8001671C'):
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
        'scope': 'Native field/stride/call ABI and address proof; no external state contents are published.',
        'native_abi': 'Both int(u16 key, void *payload); actual no-input lock/unlock14594/145DC are the only calls in each retained body. Reviewed wrappers14F80/14F14 supply the same key/payload slots.',
        'native_layouts': {'ResourceEntry': '8 bytes: payload pointer+0, unsigned-short id+4, unsigned-short references+6; packed field views and reverse whole-record shifts',
            'ResourceRange': '4 bytes: unsigned-short count+0, unsigned-short first+2;512 entries at8003DA28 end at8003E228',
            'flat_globals': 'signed count80038608,2048 native8-byte entries at80038610',
            'bucket_globals': 'signed count8003DA20,512 native4-byte ranges8003DA28,2048 native8-byte entries8003E228'},
        'host_layout_limit': 'Pointer-containing packed entries are12 bytes on the64-bit host; host tests compare field semantics using real host pointers, not native object-byte geometry. Native IDO assertions and native/relocated instruction replay establish the8-byte N64 layout.',
        'rows': rows}



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = json.dumps(run(), indent=2) + '\n'
    if args.check:
        if result != (WORK / 'abi_proof.json').read_text():
            raise SystemExit('ABI/address metadata drift')
        print('BT03 insertion ABI/address metadata reproduced')
    else:
        print(result, end='')
