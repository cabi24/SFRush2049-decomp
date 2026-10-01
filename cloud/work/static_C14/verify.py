"""Independent standalone acceptance check for the one frozen ready C14 body."""
import argparse, hashlib, json, os, shlex, subprocess, sys
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('--repo',type=Path,required=True);p.add_argument('--toolkit',type=Path,required=True);p.add_argument('--targets',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
os.environ['CONVEYOR_TOOLKIT']=str(a.toolkit);sys.path[:0]=[str(a.repo/'tools/conveyor/jobs'),str(a.repo/'tools/cloud')]
import scoring
import score
src=Path(__file__).parent/'dll_reschedule.c';record=next(r for r in json.loads((src.parent/'final_verification.json').read_text()) if r['function']=='dll_reschedule');target=a.targets/'dll_reschedule.C14.target.o'
for path,key in [(src,'source_sha256'),(target,'target_sha256')]:
 if hashlib.sha256(path.read_bytes()).hexdigest()!=record[key]:raise SystemExit('hash mismatch: '+str(path))
a.output.mkdir(parents=True,exist_ok=True);obj=a.output/'dll_reschedule.o';subprocess.run([str(a.toolkit/'ido/cc'),'-c']+shlex.split(record['flags'])+[str(src),'-o',str(obj)],check=True)
x=score.text_words(target);y=score.text_words(obj);s=scoring.score(target,obj,stack_differences=True);raw=sum(i!=j for i,j in zip(x,y))+abs(len(x)-len(y));result={'function':'dll_reschedule','flags':record['flags'],'strict_score':s,'raw_word_diff':raw,'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest()};print(json.dumps(result));(a.output/'verification.json').write_text(json.dumps(result,indent=2)+'\n');raise SystemExit(0 if s==raw==0 else 1)
