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
    for address in ('8001DDE0',):
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
    callees = {'func_8001D928': 'void(void): reset three actual byte counters',
        'func_8001D084': 'void(Emitter*): unlink next/previous and mask flags; actual helper unchanged',
        'func_8001C860': 'void(Emitter*,float* volume,float* pitch,float* x,float* y,float* z); always writes volume/pitch, last three can remain untouched when listener count is zero',
        'func_8001DA74': 'int(Emitter*,float volume,float x,float y,float z,float pitch); real checked group/large-pool capacity result',
        'func_8001B1D0': 'u32(u16 sound_id,u8 volume,u8 pan)',
        'func_800201D0': 'u32(u32 identifier): preserve valid input or return all-ones',
        'func_8001D944': 'void(Emitter*,float volume); unchecked small-pool/group insertion requires valid capacities',
        'func_8001B8C4': 'int(u32 identifier), result ignored; verified existing source returns status',
        'func_8001CCDC': 'void(Emitter*,float volume,float x,float y,float z,float pitch); y is genuinely unused by the callee but occupies its real caller slot',
        'func_8001DC08': 'void(void): final opaque caller-observation boundary'}
    calls = {r['symbol'] for row in rows for r in row['text_relocations'] if r['type'] == 4}
    if calls != set(callees):
        raise ValueError('ten-callee inventory drift')
    return {'schema_version': 1,
        'scope': 'Exact caller and ten real external interfaces; gated CalcEmitter and all other callees remain unchanged.',
        'native_abi': 'void(void); saved-next traversal; five genuine float output locals with no invented initialization',
        'native_layout': 'Emitter68: next0,previous4,flags8,untouched40-byte range12..51,identifier52,group56,sound_id60,counter62,fade64. Pointer-containing host layout is not claimed native geometry.',
        'external_state': {'D_8004FD50': 'actual emitter-list root pointer', 'D_8002D90C': 'extern const float fade increment at8002D90C; original value unknown, not inferred from public0.3f lead'},
        'callees': callees,
        'definedness_limit': 'A consumed output must have been written in this invocation, possibly in an earlier loop iteration. Native source/runtime does not prove this for all paths. Empty-list CalcEmitter may leave x/y/z unwritten; no defaults or producer invariant are invented.',
        'rows': rows}



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    result = json.dumps(run(), indent=2) + '\n'
    if args.check:
        if result != (WORK / 'abi_proof.json').read_text():
            raise SystemExit('ABI/address metadata drift')
        print('BT03 emitter caller ABI/address metadata reproduced')
    else:
        print(result, end='')
