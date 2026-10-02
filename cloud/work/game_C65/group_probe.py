import json,sys,hashlib
from pathlib import Path
from dataclasses import asdict
sys.path.insert(0,str(Path.home()/'agents/C/wt/tools/cloud'))
import score
p=Path.home()/'agents/C/scratch/game-C65';out=[]
for v in ['real_group']:
 g=p/v;o=p/(v+'.o');spec=score.compile_group(g,o)
 out.append({'variant':v,'flags':spec['flags'],'source_sha256':hashlib.sha256((g/'group.c').read_bytes()).hexdigest(),'linked':{n:asdict(score.compare(o,n,show=0)) for n in spec['members']+spec['context']}})
(p/'group_results.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out))
