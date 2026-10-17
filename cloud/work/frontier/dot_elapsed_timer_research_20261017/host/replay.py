from pathlib import Path
import tempfile,subprocess,json,hashlib
D=Path(__file__).resolve().parent;L=D.parent
prior=(D/'prior.c').read_text();src='#define OWNED_ELAPSED_TABLE 1\n'+(L/'group/candidate.c').read_text().split('/* Existing locked text callback')[0]
variants={'prior':prior,'current':src,'wrong_animid_mask':src.replace('win = blt->animID;','win = blt->animID & 15;'),'wrong_restore':src.replace('object_byte9_set(fixedWidth);','object_byte9_set(0);'),'wrong_depth':src.replace('blt->state = 100;','blt->state = 99;'),'wrong_alpha':src.replace('blt->alpha = D_80116187;','blt->alpha = 128;')}
h=(D/'harness.c').read_text();outs={}
with tempfile.TemporaryDirectory(prefix='elapsed-host-') as temp:
 for name,text in variants.items():
  p=Path(temp)/name;p.with_suffix('.c').write_text(text+'\n'+h)
  r=subprocess.run(['gcc','-std=c99','-O2',str(p.with_suffix('.c')),'-o',str(p)],capture_output=True,text=True)
  if r.returncode:raise RuntimeError(name+': '+r.stderr)
  outs[name]=subprocess.check_output([str(p)])
if outs['prior']!=outs['current']:raise RuntimeError('Original win/Hidden recovery changed callback behavior')
neg={}
for name in ('wrong_animid_mask','wrong_restore','wrong_depth','wrong_alpha'):
 count=sum(a!=b for a,b in zip(outs['current'].splitlines(),outs[name].splitlines()))
 if not count:raise RuntimeError('negative not detected '+name)
 neg[name]={'differing_scenarios':count,'total_scenarios':8192}
r={'scope':'Host callback trace only, authenticated native position initializer, not N64 ABI/image/matching proof','scenarios':8192,'prior_candidate_equal':True,'output_sha256':hashlib.sha256(outs['current']).hexdigest(),'negatives':neg}
(D/'results.json').write_text(json.dumps(r,indent=2)+'\n');print(json.dumps(r,indent=2))
