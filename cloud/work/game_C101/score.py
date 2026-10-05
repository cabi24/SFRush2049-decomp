import os,sys,json,hashlib
from dataclasses import asdict
from pathlib import Path
p=Path.home()/'agents/C/scratch/game-C101';wt=Path.home()/'agents/C/wt'
os.environ['CONVEYOR_TOOLKIT']=str(Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5')
sys.path[:0]=[str(wt/'tools/conveyor/jobs')]
import scoring
source_name=sys.argv[1] if len(sys.argv)>1 else 'group.c'
object_name=sys.argv[2] if len(sys.argv)>2 else 'candidate.o'
output_name=sys.argv[3] if len(sys.argv)>3 else 'score.json'
r=scoring.score(p/'target.o',p/object_name,stack_differences=True)
print(r)
sys.path[:0]=[str(wt/'tools/cloud')]
import score
linked=asdict(score.compare(p/object_name,'camera_target_track',show=0))
print(linked)
(p/output_name).write_text(json.dumps({'source_sha256':hashlib.sha256((p/source_name).read_bytes()).hexdigest(),'object_sha256':hashlib.sha256((p/object_name).read_bytes()).hexdigest(),'strict':r,'linked':linked},indent=2)+'\n')
