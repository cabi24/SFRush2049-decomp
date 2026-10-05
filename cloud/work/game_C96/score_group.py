import os,sys,json,hashlib,subprocess
from dataclasses import asdict
from pathlib import Path
source_name=sys.argv[1] if len(sys.argv)>1 else 'group.c'
output_name=sys.argv[2] if len(sys.argv)>2 else 'group_scores.json'
p=Path.home()/'agents/C/scratch/game-C96';wt=Path.home()/'agents/C/wt'
sys.path[:0]=[str(wt/'tools/cloud')]
import score
names=['func_800A1C6C','track_data_decompress','func_800A1DD4','func_800A1BB4','track_collision_setup','MaxPathZeroControls','slot_state_lookup']
results=[]
for n in names:
 try:r={'function':n,**asdict(score.compare(p/'group.o',n,show=0))}
 except Exception as e:r={'function':n,'refusal':str(e)}
 print(json.dumps(r));results.append(r)
(p/output_name).write_text(json.dumps({'object_sha256':hashlib.sha256((p/'group.o').read_bytes()).hexdigest(),'source_sha256':hashlib.sha256((p/source_name).read_bytes()).hexdigest(),'results':results},indent=2)+'\n')
