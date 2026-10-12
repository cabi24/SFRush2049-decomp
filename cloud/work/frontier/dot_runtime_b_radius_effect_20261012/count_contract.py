#!/usr/bin/env python3
"""Current accepted shared-count view plus an independent native read witness.

This establishes reuse of an accepted project contract, not original spelling,
an asynchronous/device writer, or a general inference of volatile from code.
"""
import hashlib
import json
from pathlib import Path
import re
import struct

COUNT=0x801543CA
PRODUCTION=('func_800EC914','func_80108DA8','func_80108F40','func_80109A60','world_physics_tick')


def require(value,message):
    if not value:
        raise ValueError(message)


def sha(data):
    return hashlib.sha256(data).hexdigest()


def packed(words):
    return struct.pack('>%dI'%len(words),*words)


def evidence(root,score):
    old=score.ASM_DIR
    try:
        identities={}
        for population in ('blob','ovl_b'):
            score.ASM_DIR=root/'asm/us'/population
            identities[population]=score.image_symbols()['D_801543CA']
        require(identities=={'blob':COUNT,'ovl_b':COUNT},'shared symbol identity')
        native_b=score.targets()['func_8038D798']
        for high,low,load,base,result in ((4,0x48,0x4c,14,15),(0x268,0x26c,0x270,11,12)):
            require(native_b[high//4]==(15<<26|base<<16|0x8015),'B count upper address')
            require(native_b[low//4]==(9<<26|base<<21|base<<16|0x43ca),'B count lower address')
            require(native_b[load//4]==(33<<26|base<<21|result<<16),'B signed halfword observation')
        score.ASM_DIR=root/'asm/us/blob'
        locks=json.loads((root/'blob_matched.lock.json').read_text())
        accepted={}
        for name in PRODUCTION:
            row=locks[name];path=root/row['source'];source=path.read_bytes();text=source.decode()
            require(row['verified']=='image_gate' and sha(source)==row['source_sha256'],'accepted source identity: '+name)
            require('extern volatile s16 D_801543CA;' in text and re.search(r'typedef (?:signed )?short s16;',text),'accepted signed/qualified type: '+name)
            accepted[name]={'path':row['source'],'source_sha256':sha(source),'flagset':row['flagset'],
                            'target_sha256':sha(packed(score.targets()[name]))}
        name='func_800EB90C';base=score.image_symbols()[name];words=score.targets()[name]
        def word(pc):
            require(base<=pc<base+len(words)*4 and (pc-base)%4==0,'witness PC range')
            return words[(pc-base)//4]
        require(word(0x800EB9A4)==(15<<26|4<<16|0x8015),'witness upper address')
        require(word(0x800EB9AC)==(9<<26|4<<21|4<<16|0x43ca),'witness lower address')
        require(word(0x800EB9B8)==word(0x800EB9FC)==(33<<26|4<<21|12<<16),'witness count loads')
        edge=word(0x800EB9F8)
        require(edge>>26==1 and edge>>21&31==24 and edge>>16&31==2,'witness BLTZL')
        offset=struct.unpack('>h',struct.pack('>H',edge&65535))[0]
        require(0x800EB9F8+4+4*offset==0x800EB9BC,'continuing backedge')
        exit_branch=word(0x800EB9EC)
        require(exit_branch>>26==21 and exit_branch>>21&31==15 and exit_branch>>16&31==0 and word(0x800EB9F0)>>26==40,'opposite exit/annulled store')
        path=list(range(0x800EB9BC,0x800EB9F0,4))+[0x800EB9F4,0x800EB9F8,0x800EB9FC]
        allowed={0,1,5,9,15,21,32,33}
        for pc in path:
            instruction=word(pc);op=instruction>>26
            require(op in allowed,'unknown witness operation')
            if op==0:
                fn=instruction&63
                require(fn in (0,16,18,25,33,37,42),'unknown witness SPECIAL')
                require(fn==25 or instruction>>11&31!=4,'count address clobber')
            if op in (9,15,32,33):
                require(instruction>>16&31!=4,'count address clobber')
        return {'shared_address':hex(COUNT),'signed_width':16,'accepted_declaration':'extern volatile s16 D_801543CA;',
                'accepted_sources':accepted,'image_b_load_offsets':[0x4c,0x270],
                'independent_witness':{'function':name,'body_sha256':sha(packed(words)),'first_read':hex(0x800EB9B8),
                    'continuing_read':hex(0x800EB9FC),'backedge':hex(0x800EB9F8),'destination':hex(0x800EB9BC),
                    'continuing_path_has_calls_or_stores':False,'opposite_exit_store':hex(0x800EB9F0)},
                'qualification_basis':'Reuse of current accepted shared-global view, independently corroborated by native repeated observation.',
                'limit':'Original declaration spelling and asynchronous/device writer remain unestablished.'}
    finally:
        score.ASM_DIR=old
