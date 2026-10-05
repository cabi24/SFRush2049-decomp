"""Fixed source controls for fused versus donor-ordered vector operations."""
import dataclasses,json
from pathlib import Path
from tools.cloud import score

def split_body(body,selected):
 for j,(a,factor) in enumerate([('res','.1f * D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]'),('pos2','D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]'),('res','D_8012E6E8[pl] / D_801108C8[D_8012E6C8[pl]]')]):
  if j not in selected:continue
  before='\n'.join('        delta[pl].'+c+' = ('+a+'['+str(i)+'] - D_8012E690[pl].'+c+') * ('+factor+');' for i,c in enumerate('xyz'))
  after='\n'.join('        delta[pl].'+c+' = '+a+'['+str(i)+'] - D_8012E690[pl].'+c+';' for i,c in enumerate('xyz'))+'\n'+'\n'.join('        delta[pl].'+c+' = delta[pl].'+c+' * ('+factor+');' for c in 'xyz')
  assert body.count(before)==1;body=body.replace(before,after)
 return body

def verify(directory,packet):
 baseline=(packet.GROUP/'group.c').read_text();a,b=packet.body_bounds(baseline);baseline=baseline[a:b]
 candidate=(packet.HERE/'candidate.c').read_text();candidate=candidate[candidate.index('void '+packet.FN):].strip()
 assert candidate==split_body(baseline,{0,1,2}).strip(),'candidate changed beyond the three donor-operation phases'
 result={}
 for name,selected in [('fused_baseline',set()),('first_split',{0}),('second_split',{1}),('third_split',{2}),('last_two_split',{1,2}),('all_split',{0,1,2})]:
  group=packet.make_group(directory/name,split_body(baseline,selected));obj=directory/(name+'.o');score.compile_group(group,obj)
  r={}
  for fn in [packet.FN]+packet.CONTEXT:
   c=dataclasses.asdict(score.compare(obj,fn,show=0));c['elf_function_bytes']=packet.symbol(obj,fn)['size'];r[fn]=c
   if fn in packet.CONTEXT:assert score.compare(obj,fn,show=0).accepted()
  assert r[packet.FN]['differing']==(0 if name=='all_split' else 112)
  result[name]=r
 return result
