"""Prepare finite lane manifests; never generate or adapt frozen proposals."""
import json
from pathlib import Path
import sys
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT))
from tools.cloud import hypothesis_batch as batch
HERE=Path(__file__).parent

def prepare(lane):
 root=HERE/lane
 plan=json.loads((HERE/'plan-template.json').read_text())
 predictions=json.loads((root/'predictions.json').read_text())
 for p in predictions.values():
  if 'semantic_scope' in p:
   p['semantic_justification'] += ' Semantic scope: '+str(p.pop('semantic_scope'))
 assert len(predictions)=={'low':25,'medium':20,'high':15,'xhigh':10}[lane]
 plan['predictions']=predictions
 plan['limits']['variants']=len(predictions)
 plan['editable_helpers']=['set_volume','set_pan','set_depth','set_rate']
 plan['hypothesis']='Source-structure-informed '+lane+' effort: real setter diamonds, lifetime/control flow and path-local recycling; authentic helper context unresolved.'
 (root/'context.json').write_bytes((HERE/'baseline-context.json').read_bytes())
 experiment={'schema':'decomp-workbench-experiment-v2','family':'camera-source-structure-'+lane,'baseline':'baseline.c','parameters':{'variant':list(predictions)},'candidates':[{'source':p['source'],'parameters':{'variant':k}} for k,p in predictions.items()]}
 batch.write_json(root/'experiment.json',experiment)
 batch.write_json(root/'batch.json',plan)
 batch.validate(root/'batch.json')
 batch.write_json(root/'runner-validation.json',{'status':'passed','candidate_count':len(predictions),'context_scope':'Only camera_transform and four explicitly named static setter bodies editable; all signatures, declarations, other helpers frozen.'})
 return plan
if __name__=='__main__':
 for lane in sys.argv[1:]: prepare(lane)
