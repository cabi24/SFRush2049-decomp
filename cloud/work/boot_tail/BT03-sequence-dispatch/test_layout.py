#!/usr/bin/env python3
"""Prove actual O32 type extents and every reconstructed field with pinned IDO."""
import hashlib,json,sys,tempfile
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parents[3];sys.path.insert(0,str(ROOT))
from tools.cloud import score
CHECKS=[('sizeof(void*)',4),('sizeof(SequenceRequest)',40),('sizeof(SequenceOptions)',32),('sizeof(SequenceContext)',4088)]
FIELDS={'SequenceRequest':{'identifier':0,'duration':4,'nextIdentifier':8,'nextDuration':12,'stream':16,'group':20,'program':22,'channel':24,'first':28,'second':32,'value':36,'flags':38},'SequenceOptions':{'flags':0,'first':4,'second':8,'value':12,'duration':14,'channel':16,'mapCount':18,'map':20,'channelCount':24,'channels':28},'SequenceContext':{'request':0xFC8,'pendingResult':0xFF0,'pending':0xFF4}}
for typ,fields in FIELDS.items():CHECKS.extend(('(unsigned int)&((%s*)0)->%s'%(typ,field),value) for field,value in fields.items())
def run():
    source=P/'nonmatch/func_80019490.c'
    with tempfile.TemporaryDirectory(prefix='sequence-layout-') as tmp:
        path=Path(tmp)/'layout.c';path.write_text(source.read_text()+'\n'+'\n'.join('typedef char layout_%d[(%s == %d) ? 1 : -1];'%(i,e,v) for i,(e,v) in enumerate(CHECKS))+'\n')
        score.compile_single(path,score.DEFAULT_FLAGS,path.with_suffix('.o'))
    return {'result':'PASS','source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'checks':[{'expression':e,'value':v} for e,v in CHECKS]}
if __name__=='__main__':print(json.dumps(run(),indent=2))
