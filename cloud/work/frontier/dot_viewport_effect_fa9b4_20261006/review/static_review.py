#!/usr/bin/env python3
"""No-compile alignment/target-baseline check, metadata only."""
import dataclasses,hashlib,json
import review as r

def stack_alignment(words):
 frame=[];fp=[]
 for i,w in enumerate(words):
  op=w>>26;rs=w>>21&31;rt=w>>16&31;imm=r.v.signed(w&65535,16)
  if op==9 and rs==29 and rt==29:
   assert imm%8==0,('unaligned stack adjustment',i*4,imm)
   frame.append({'offset':i*4,'adjustment':imm})
  if op in (53,61):
   assert rs==29 and imm%8==0,('unaligned doubleword stack access',i*4,rs,imm)
   fp.append({'offset':i*4,'stack_offset':imm,'store':op==61})
 assert sum(x['adjustment'] for x in frame)==0
 return {'stack_adjustments':frame,'doubleword_fp_accesses':fp}
result={'native_alignment':stack_alignment(r.words),'existing_object_alignment':stack_alignment(r.compiled)}
for original in (r.words,r.compiled):
 mutant=original[:]
 i=next(i for i,w in enumerate(mutant) if w>>26==61)
 mutant[i]=(mutant[i]&0xffff0000)|((mutant[i]+4)&65535)
 try:stack_alignment(mutant)
 except AssertionError as exc:assert 'unaligned doubleword' in str(exc)
 else:raise AssertionError('unaligned doubleword accepted')
result['negative_controls']=['Misaligned doubleword store rejected in native words','Misaligned doubleword store rejected in existing-object words']
result['comparison']=dataclasses.asdict(r.score.compare(r.obj,r.v.NAME,show=0))
result['source_sha256']=r.FROZEN;result['object_sha256']=hashlib.sha256(r.obj.read_bytes()).hexdigest();result['target_compilations']=0
(r.OUTPUT/'static-review.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
