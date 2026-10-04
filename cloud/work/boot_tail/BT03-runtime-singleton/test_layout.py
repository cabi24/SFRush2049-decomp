#!/usr/bin/env python3
"""Prove actual O32 field offsets with the pinned IDO compiler."""
import hashlib,json,sys,tempfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[3];sys.path.insert(0,str(ROOT))
from tools.cloud import score
CHECKS=[('sizeof(void*)',4),('sizeof(SequenceEvent)',12),('sizeof(Track)',16),('sizeof(Playback)',40),('sizeof(SequenceHeader)',24),('sizeof(Context)',0xFC8)]
FIELDS={'SequenceEvent':{'time':0,'program':4,'controller':5,'code':8,'value':10},'SequenceHeader':{'patterns':4,'channels':8,'loop_time':20},'Pattern':{'optional04':4,'optional08':8,'data':12},'Track':{'start':0,'cursor':4,'fraction':8,'time':12},'Playback':{'counter00':0,'counter04':4,'counter08':8,'cursor':12,'optional10':16,'optional14':20,'value18':24,'value1A':26,'counter1C':28,'counter20':32,'channel':36,'second':37,'first':38,'index':39},'Context':{'sequence':0x10C,'step_fraction':0x118,'step_time':0x11C,'lookahead':0x120,'tracks':0x128,'playback':0x568,'stop_loop':0xFC5,'loop_count':0xFC6}}
for typ,fields in FIELDS.items():
    CHECKS.extend(('(unsigned int)&((%s*)0)->%s'%(typ,field),value) for field,value in fields.items())
def run():
    source=P/'nonmatch/func_80018634.c'
    with tempfile.TemporaryDirectory(prefix='runtime-layout-') as tmp:
        path=Path(tmp)/'layout.c';path.write_text(source.read_text()+'\n'+'\n'.join('typedef char layout_%d[(%s == %d) ? 1 : -1];'%(i,e,v) for i,(e,v) in enumerate(CHECKS))+'\n')
        score.compile_single(path,score.DEFAULT_FLAGS,path.with_suffix('.o'))
    return {'result':'PASS','source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'checks':[{'expression':e,'value':v} for e,v in CHECKS]}
if __name__=='__main__':print(json.dumps(run(),indent=2))
