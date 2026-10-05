#!/usr/bin/env python3
"""Metadata-only native contract / complete-object audit. No target bytes emitted."""
import argparse
from contextlib import redirect_stdout
from dataclasses import asdict
import hashlib
import io
import json
import re
from pathlib import Path
import struct
import sys
import tempfile

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(ROOT / 'tools/cloud'))
import score

NAMES = ('func_800D348C', 'stunt_combo_display', 'camera_follow_path', 'func_8008B2E4')

def packed(words):
    return struct.pack('>%dI' % len(words), *words)

def digest(data):
    return hashlib.sha256(data).hexdigest()

def calls(words, address):
    return [(address+i*4, 0x80000000|((w&0x3ffffff)<<2))
            for i,w in enumerate(words) if w >> 26 == 3]

def frame(words):
    for w in words[:40]:
        if w >> 16 == 0x27bd and w & 0x8000:
            return 65536-(w&0xffff)
    return 0

def native():
    targets = score.targets()
    addresses = score.image_symbols()
    by = {v:k for k,v in addresses.items() if k in targets}
    out = {}
    for n in NAMES:
        words=targets[n];addr=addresses[n]
        incoming=[]
        for caller,body in targets.items():
            for site,dest in calls(body,addresses[caller]):
                if dest==addr:incoming.append({'caller':caller,'site':hex(site)})
        out[n]={'start':hex(addr),'end_exclusive':hex(addr+4*len(words)),
                'bytes':4*len(words),'sha256':digest(packed(words)),
                'frame_bytes':frame(words),'direct_callers':incoming,
                'calls':[{'site':hex(site),'destination':hex(dest),
                          'name':by.get(dest)} for site,dest in calls(words,addr)]}
    return out

def safe_metadata(value):
    """Do not preserve literal retail bytes from own-data failure messages."""
    if isinstance(value,str):
        return re.sub(r"retail [0-9a-f]+, got [0-9a-f]+", "retail and candidate literal bytes differ", value)
    if isinstance(value,list):return [safe_metadata(v) for v in value]
    if isinstance(value,dict):return {k:safe_metadata(v) for k,v in value.items()}
    return value

def compiled(obj,n):
    data,secs=score._elf(obj)
    syms=[s for i,sec in enumerate(secs) if sec['type']==2
          for s in score._symbol_table(data,secs,i)
          if s['type']==2 and s['section']==score._text_index(secs)]
    sym=next(s for s in syms if s['name']==n)
    words=score.text_words(obj);start=sym['value'];end=start+sym['size']
    next_start=min([s['value'] for s in syms if s['value']>start]+[len(words)*4])
    assert end<=next_start, 'ELF function overlaps another function'
    body=words[start//4:end//4]
    padding=words[end//4:next_start//4]
    resolved,masks,unresolved,unverified,errors=score.relocate(
        obj,words,start,end,score.image_symbols())
    with redirect_stdout(io.StringIO()):comparison=score.compare(obj,n,show=0)
    want=score.targets()[n]
    return {'elf_start':start,'elf_end_exclusive':end,'elf_bytes':sym['size'],
            'frame_bytes':frame(body),'zero_alignment_bytes':len(padding)*4 if not any(padding) else None,
            'nonzero_alignment_words':sum(w!=0 for w in padding),
            'native_bytes':len(want)*4,'full_body_nonzero_excess_words':sum(w!=0 for w in body[len(want):]),
            'full_body_resolved_sha256':digest(packed(resolved[start//4:end//4])),
            'full_body_relocations':{'unresolved':unresolved,'unverified':unverified,'errors':errors},
            'comparison':safe_metadata(asdict(comparison)),'own_data_notes':safe_metadata(list(comparison.notes)),
            'strict_match':comparison.accepted() and len(body)==len(want),
            'summary':safe_metadata(comparison.summary())}

def audit(replay=False):
    receipt={'schema':1,'base':'f88dc3cb','classification':'NONMATCH / incomplete IPA closure',
             'claims':[],'native':native(),
             'source_hashes':{p:digest((HERE/p).read_bytes()) for p in ('group.c','group.json','helper_seed.c')}}
    if replay:
        with tempfile.TemporaryDirectory(prefix='camera-contract-audit-') as d:
            path=Path(d);obj=path/'group.o';spec=score.compile_group(HERE,obj)
            receipt['natural_group']={n:compiled(obj,n) for n in NAMES}
            original=(HERE/'group.c').read_text();a=original.index('void func_800D348C(CameraCar *');b=original.index('void camera_follow_path(s32 pathMode')
            (path/'group.c').write_text(original[:a]+(HERE/'helper_seed.c').read_text()+original[b:])
            (path/'group.json').write_text(json.dumps(spec));score.compile_group(path,obj)
            receipt['diagnostic_seed_group']={n:compiled(obj,n) for n in NAMES}
    return receipt

if __name__=='__main__':
    p=argparse.ArgumentParser();p.add_argument('--replay',action='store_true');a=p.parse_args()
    print(json.dumps(audit(a.replay),indent=2,sort_keys=True))
