#!/usr/bin/env python3
"""Replay pinned preflight and two complete NONMATCH bodies plus bounded controls."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import sys
import tempfile

ROOT = Path(__file__).resolve().parents[4]
WORK = Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT))
from tools.cloud import score


def digest(path): return hashlib.sha256(path.read_bytes()).hexdigest()


def function_size(obj,name):
    data,secs = score._elf(obj)
    syms = [sym for i,sec in enumerate(secs) if sec['type'] == 2
            for sym in score._symbol_table(data,secs,i)
            if sym['name'] == name and sym['type'] == 2]
    if len(syms) != 1 or syms[0]['value'] != 0:
        raise ValueError('unexpected ELF function symbol')
    return syms[0]['size']


def preflight():
    score.ASM_DIR = ROOT/'asm/us/boot_tail'
    old = json.loads((ROOT/'cloud/work/boot_tail/packet2/preflight.json').read_text())
    compiler = {p.name:digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()}
    if compiler != old['compiler_files_sha256']:
        raise ValueError('pinned IDO toolchain drift')
    manifest = score.target_manifest()
    for name in manifest: score.verified_bytes(score.ASM_DIR/name,manifest)
    inventory = json.loads((ROOT/'specs/015-boot-tail-runtime/inventory.json').read_text())['functions']
    extents = json.loads((score.ASM_DIR/'extents.json').read_text())['functions']
    if {x['name']:x['size'] for x in inventory} != {x['name']:x['size'] for x in extents}:
        raise ValueError('inventory/extent mismatch')
    if len(score.targets()) != 439 or len(extents) != 439:
        raise ValueError('target census drift')
    with tempfile.TemporaryDirectory(prefix='c13-controller-getter-') as tmp:
        obj = Path(tmp)/'getter.o'
        score.compile_single(ROOT/'cloud/matches/boot_tail/func_80010A00.c',score.DEFAULT_FLAGS,obj)
        result = score.compare(obj,'func_80010A00',show=0)
        if not result.accepted() or function_size(obj,'func_80010A00') != 12:
            raise ValueError('existing getter regression')
    return {'manifest':'PASS','extent_count':439,'extent_bytes':sum(x['size'] for x in extents),
            'inventory_equal':True,'getter_strict_match':True,'getter_function_bytes':12,
            'compiler_files_sha256':compiler}


def run():
    check = preflight()
    results = []
    for category in ('nonmatch','variants'):
        for source in sorted((WORK/category).glob('*.c')):
            name = re.search(r'func_[0-9A-F]{8}',source.name)[0]
            want = score.targets()[name]
            for level in (2,1):
                flags = '-g0 -O%d -mips2 -G 0 -non_shared'%level
                with tempfile.TemporaryDirectory(prefix='c13-controller-score-') as tmp:
                    obj = Path(tmp)/'candidate.o'
                    score.compile_single(source,flags,obj)
                    result = score.compare(obj,name,show=0)
                    words = score.text_words(obj)
                    size = function_size(obj,name)
                    got,masks,unresolved,unverified,errors = score.relocate(obj,words,0,len(words)*4,score.image_symbols())
                    full = (size == len(want)*4 and got[:len(want)] == want
                            and not any(got[len(want):]) and not masks
                            and not unresolved and not unverified and not errors)
                    if result.accepted() or full:
                        raise ValueError('Unexpected matching control requires reviewed promotion: '+str(source))
                    frame = next((-(w&65535) if (w&32768)==0 else 65536-(w&65535)
                                  for w in words if w>>16 == 0x27BD),0)
                    results.append({'name':name,'source_path':str(source.relative_to(ROOT)),
                        'purpose':category,'source_sha256':digest(source),'flags':flags,
                        'effective_flags':flags+' -Wab,-r4300_mul',
                        'target_bytes':len(want)*4,'elf_function_bytes':size,
                        'elf_function_extent_equal':size == len(want)*4,
                        'object_text_bytes':len(words)*4,'frame_bytes':frame,
                        'target_body_sha256':hashlib.sha256(struct.pack('>%dI'%len(want),*want)).hexdigest(),
                        'differing_words':result.differing,'total_words':result.total,
                        'nonzero_excess_words':result.extra_words,
                        'unresolved':result.unresolved,'unverified':result.unverified,'errors':result.errors,
                        'full_object_unresolved':unresolved,'full_object_unverified':unverified,
                        'full_object_relocation_errors':errors,'masked_relocations':len(masks),
                        'scorer_accepted':result.accepted(),'strict_exact_function_match':full})
    paths = ['asm/us/boot_tail/SHA256SUMS','asm/us/boot_tail/boot_tail_8000f3a4.s',
             'asm/us/boot_tail/extents.json','asm/us/boot_tail/symbols.json',
             'specs/015-boot-tail-runtime/inventory.json','tools/cloud/score.py','tools/cloud/setup.sh']
    return {'schema_version':1,'packet':'C13-controller-remainder',
            'source_base':'96b9dd1979f7c797f97a1dd2317fc0852092c0dc',
            'claim_acknowledgment':'c3b07951','preflight':check,
            'protected_input_hashes':{p:digest(ROOT/p) for p in paths},'results':results}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check',action='store_true')
    args = parser.parse_args()
    output = json.dumps(run(),indent=2)+'\n'
    if args.check:
        if output != (WORK/'scores.json').read_text(): raise SystemExit('receipt drift')
        print('C13-controller-remainder: preflight and all NONMATCH receipts reproduced')
    else: print(output,end='')
