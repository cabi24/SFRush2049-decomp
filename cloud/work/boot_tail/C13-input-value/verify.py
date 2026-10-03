#!/usr/bin/env python3
"""Reproduce the bounded input-value NONMATCH and every archived source control."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import sys
import tempfile

ROOT=Path(__file__).resolve().parents[4]
WORK=Path(__file__).resolve().parent
sys.path.insert(0,str(ROOT))
from tools.cloud import score
NAME='func_80021150'


def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()


def run():
    score.ASM_DIR=ROOT/'asm/us/boot_tail'
    want=score.targets()[NAME]
    if len(want)!=182:raise ValueError('native extent drift')
    results=[]
    sources=[(WORK/'func_80021150_NONMATCH.c','complete-nonmatch')]
    sources += [(p,'source-control') for p in sorted((WORK/'variants').glob('*.c'))]
    for source,purpose in sources:
        for level in [2,1]:
            flags='-g0 -O%d -mips2 -G 0 -non_shared'%level
            with tempfile.TemporaryDirectory(prefix='c13-input-') as t:
                obj=Path(t)/'function.o'
                score.compile_single(source,flags,obj)
                result=score.compare(obj,NAME,show=0)
                if score.symbols(obj)!={NAME:0}:raise ValueError('unexpected function boundary')
                if result.accepted():raise ValueError('Unexpected new match; independently review before claiming')
                results.append({'name':NAME,'purpose':purpose,'source_path':str(source.relative_to(ROOT)),
                    'source_sha256':digest(source),'flags':flags,'effective_flags':flags+' -Wab,-r4300_mul',
                    'native_bytes':728,'object_text_bytes':len(score.text_words(obj))*4,
                    'differing_words':result.differing,'total_words':result.total,'extra_words':result.extra_words,
                    'unresolved':result.unresolved,'unverified':result.unverified,'errors':result.errors,
                    'strict_match':result.accepted()})
    best=results[0]
    if (best['differing_words'],best['extra_words'])!=(24,0):raise ValueError('frozen residual drift')
    if best['unresolved'] or best['unverified'] or best['errors']:raise ValueError('best source has relocation uncertainty')
    paths=['asm/us/boot_tail/SHA256SUMS','asm/us/boot_tail/boot_tail_8000f3a4.s',
           'asm/us/boot_tail/extents.json','asm/us/boot_tail/symbols.json',
           'specs/015-boot-tail-runtime/inventory.json','tools/cloud/score.py']
    return {'schema_version':1,'result':'COMPLETE-NONMATCH','source_base':'301d9e7552ad4fd7f54a38796db84671e1000d35',
            'target_body_sha256':hashlib.sha256(struct.pack('>182I',*want)).hexdigest(),
            'source_hashes':{p:digest(ROOT/p) for p in paths},
            'compiler_files_sha256':{p.name:digest(p) for p in sorted(score.IDO.iterdir()) if p.is_file()},
            'results':results}


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--check',action='store_true');args=parser.parse_args()
    text=json.dumps(run(),indent=2)+'\n'
    if args.check:
        if text!=(WORK/'scores.json').read_text():raise SystemExit('input-value receipt drift')
        print('Input-value: complete NONMATCH frozen at 24/182, zero excess; all controls reproduced')
    else:print(text,end='')
