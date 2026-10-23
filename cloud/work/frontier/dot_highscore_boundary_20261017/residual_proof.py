"""Classify every partial root/Ghost word; no register/opcode blanket masking."""
import sys
from pathlib import Path
R=next(p for p in Path(__file__).resolve().parents if (p/'tools/cloud/score.py').is_file());sys.path.insert(0,str(R));from tools.cloud import score
from window_proof import check,require

def proof(path,name):
 targets=score.targets();want=targets[name];words=score.text_words(path);fns=score.symbols(path);start=fns[name];end=min((v for v in fns.values() if v>start),default=len(words)*4)
 got,masks,unresolved,unverified,errors=score.relocate(path,words,start,end,score.image_symbols());got=got[start//4:end//4]
 while len(got)>len(want) and got[-1]==0:got.pop()
 require(len(got)==len(want),'Function extent changed: '+name);require(not unresolved,'Unresolved external: '+name)
 if name=='highscore_animText':
  delta=24;sp_sites=[0,0x3c,0x16c,0x1bc,0x1d8,0x1dc,0x1e4,0x1e8,0x1f0,0x210,0x218,0x21c,0x220,0x2b4,0x2c4,0x6e8];swap=[0x5e4,0x5e8];own=[0x58,0x60,0x3cc,0x3d8]
 else:
  require(name=='DrawGhost','Unknown residual target');delta=8;sp_sites=[0,0x1e0,0x2cc,0x530,0x5c4,0x614,0x7fc,0x82c,0x89c,0x958];swap=[];own=[0x1ec,0x1f0,0x2e4,0x2e8,0x380,0x384,0x548,0x54c,0x640,0x648,0x6c4,0x6cc]
 require(set(masks)=={start+x for x in own},'Own-section reference inventory changed: '+name)
 for i,(native,actual) in enumerate(zip(want,got)):
  offset=i*4
  if offset in own:
   require(native&0xffff0000==actual&0xffff0000,'Owned reference instruction changed: '+name)
  elif offset in sp_sites:
   require((native>>21)&31==29 and native&0xffff0000==actual&0xffff0000,'Stack operation changed: '+name)
   change=delta if offset==0 else -delta
   if name=='DrawGhost' and offset not in [0,0x958]:change=-4
   require((native+change)&0xffff==actual&0xffff,'Stack residual changed: '+name)
  elif offset in swap:
   require(actual==want[swap[1-swap.index(offset)]//4],'Constant-load order changed: '+name)
  else:require(native==actual,'Unclassified instruction difference: '+name+' '+hex(offset))
 windows=check(path,name)
 result=score.compare(path,name,show=0)
 require(not result.accepted() and bool(result.errors) and len(result.unverified)==len(own),'Strict layout refusal was lost: '+name)
 return {'words':len(want),'strict_differing':result.differing,'frame_gap_bytes':delta,'stack_difference_sites':[hex(x) for x in sp_sites],'constant_load_swap_sites':[hex(x) for x in swap],'unverified_sites':[hex(x) for x in own],'owned_window_content':windows,'whole_section_layout_accepted':False}

if __name__=='__main__':
 import json,contextlib,io
 with contextlib.redirect_stdout(io.StringIO()):result={n:proof(Path(sys.argv[1]),n) for n in ['highscore_animText','DrawGhost']}
 print(json.dumps(result,indent=2,sort_keys=True))
