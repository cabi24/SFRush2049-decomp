#!/usr/bin/env python3
import argparse,hashlib,json,os,sys,tempfile
from dataclasses import asdict
from pathlib import Path
ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,required=True);ap.add_argument('--toolkit',type=Path,required=True);ap.add_argument('--target-dir',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);a=ap.parse_args();packet=Path(__file__).resolve().parent
for name,sha in json.loads((packet/'source_hashes.json').read_text()).items():assert hashlib.sha256((packet/name).read_bytes()).hexdigest()==sha,name
os.environ['CONVEYOR_TOOLKIT']=str(a.toolkit);os.environ['IDO_DIR']=str(a.toolkit/'ido');sys.path[:0]=[str(a.repo/'tools/conveyor/jobs'),str(a.repo/'tools/cloud')]
import scoring
import score as retail
results=[]
with tempfile.TemporaryDirectory(prefix='C24-motion-') as scratch:
 obj=Path(scratch)/'group.o';spec=retail.compile_group(packet,obj)
 for target in json.loads((packet/'targets.json').read_text()):
  name=target['target_id'];to=a.target_dir/(name+'.target.o');assert hashlib.sha256(to.read_bytes()).hexdigest()==target['target_o_sha']
  scorer=scoring.Scorer(target_o=str(to),stack_differences=True,algorithm='difflib',debug_mode=False,ign_branch_targets=True,objdump_command=scoring.objdump_command()+' --disassemble='+name)
  row={'function':name,'strict_score':scorer.score(str(obj))[0],'linked':asdict(retail.compare(obj,name,show=0))};results.append(row)
 result={'source_hashes':json.loads((packet/'source_hashes.json').read_text()),'flags':spec['flags'],'keep':spec['keep'],'claims':spec['claims'],'results':results};a.output.write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
 assert all(r['strict_score']==0 and r['linked']['differing']==0 and not any(r['linked'][k] for k in ['unresolved','unverified','errors','extra_words']) for r in results)
