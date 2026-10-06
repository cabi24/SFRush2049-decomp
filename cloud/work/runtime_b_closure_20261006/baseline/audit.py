#!/usr/bin/env python3
"""Read-only closure inventory. Emits metadata/hashes, never native bytes."""
import argparse
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import subprocess
import sys
import zlib

sys.dont_write_bytecode = True
BASE = '6b2e9e506fe3d2267a710e41c85af5364ccd00c7'
SOURCES = [
    ('runtime-b-fce0-root', 'runtime_b_fce0_root_20261006', 'root.c', ['FCE0']),
    ('runtime-b-e114-update', 'runtime_b_e114_update_20261006', 'update.c', ['E114']),
    ('runtime-b-f938-setup', 'runtime_b_f938_setup_20261006', 'setup.c', ['F938']),
    ('runtime-b-da78-position', 'runtime_b_da78_position_20261006', 'position.c', ['DA78']),
    ('runtime-b-d498-visual', 'runtime_b_d498_visual_20261006', 'visual.c', ['D498']),
    ('runtime-b-e114-parent', 'runtime_b_e114_parent_20261006', 'children.c', ['D200','D328','E088']),
]
SIZES = {'FCE0':3056,'E114':5196,'F938':928,'DA78':868,'D498':768,
         'D200':296,'D328':124,'E088':140}

def sha(data):
    return hashlib.sha256(data).hexdigest()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--reference-root', type=Path, required=True)
    parser.add_argument('--workspace', type=Path, default=Path(__file__).resolve().parent.parent)
    parser.add_argument('--output', type=Path, default=Path(__file__).with_name('inventory.json'))
    args = parser.parse_args()
    root = args.reference_root.resolve()
    sys.path.insert(0, str(root/'tools/cloud'))
    spec = importlib.util.spec_from_file_location('closure_map_score', root/'tools/cloud/score.py')
    score = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = score
    spec.loader.exec_module(score)
    asset = subprocess.check_output(['git','-C',str(root),'show',BASE+':assets/us/data.bin'])
    dec = zlib.decompressobj(-15)
    image = dec.decompress(asset[0xB6FEC4-0x283D0:])
    assert dec.eof and len(image) == 43888
    assert sha(image) == 'b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd'
    wanted = {0x80380000+int(k,16):'func_8038'+k for k in SIZES}
    identity, census = {}, {}
    for population in ('ovl_b','blob'):
        score.ASM_DIR = root/'asm/us'/population
        targets, symbols = score.targets(), score.image_symbols()
        edges = {n:[] for n in wanted.values()}
        literals = {n:[] for n in wanted.values()}
        for function, words in targets.items():
            if function not in symbols:
                continue
            start = symbols[function]
            for i, word in enumerate(words):
                site = start+4*i
                if word >> 26 in (2,3):
                    destination = ((site+4)&0xF0000000)|((word&0x3FFFFFF)<<2)
                    if destination in wanted:
                        edges[wanted[destination]].append(dict(caller=function,site=hex(site),
                            kind='jal' if word>>26 == 3 else 'j'))
                if word in wanted:
                    literals[wanted[word]].append(dict(function=function,offset=hex(4*i)))
        census[population] = dict(direct_edges=edges,literal_pointer_words_in_code=literals)
        if population == 'ovl_b':
            for key, size in SIZES.items():
                name = 'func_8038'+key
                start = 0x80380000+int(key,16)
                raw = struct.pack('>'+str(len(targets[name]))+'I',*targets[name])
                assert len(raw) == size and raw == image[start-0x8038A400:start-0x8038A400+size]
                identity[name] = dict(address=hex(start),end=hex(start+size),size=size,sha256=sha(raw))
    sources = []
    for directory, packet, filename, members in SOURCES:
        p = args.workspace/directory/'cloud/work'/packet/filename
        source_hash = sha(p.read_bytes())
        frozen = args.workspace/directory/'local/frozen_manifest.json'
        freeze_status = 'no frozen_manifest.json checked by this inventory'
        if frozen.is_file():
            manifest = json.loads(frozen.read_text())
            manifest = manifest.get('files',manifest)
            expected = manifest.get('cloud/work/'+packet+'/'+filename)
            assert expected == source_hash, ('frozen source drift',str(p))
            freeze_status = 'source SHA-256 matches author frozen manifest; review status is separate'
        sources.append(dict(path=str(p.resolve()),members=members,sha256=source_hash,
                            freeze_status=freeze_status))
    result = dict(status='READ-ONLY MAP; no compile, merge, publication or matching claim',
                  base=BASE,minimum_native_function_bytes=sum(SIZES.values()),
                  targets=identity,sources=sources,census=census,
                  census_limits='Direct J/JAL census in authenticated main-game/B function targets only. Literal code-word scan is weak negative evidence; data pointers, HI/LO-built addresses, indirect calls and other runtime contexts are not excluded.',
                  schema_sha256=sha(Path(__file__).with_name('shared_schema.proposed.h').read_bytes()),
                  compiler_invocations=0)
    args.output.write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(dict(status=result['status'],native_bytes=sum(SIZES.values()),
                         source_files=len(sources),output=str(args.output)),indent=2))

if __name__ == '__main__':
    main()
