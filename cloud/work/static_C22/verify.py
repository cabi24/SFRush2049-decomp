"""Independent serial strict/raw replay; no DB or shared-tree writes."""
import argparse,hashlib,json,os,shlex,subprocess,sys,tempfile
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('--repo',type=Path,required=True);p.add_argument('--toolkit',type=Path,required=True);p.add_argument('--target-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--context-dir',type=Path);a=p.parse_args();a.repo=a.repo.resolve();a.toolkit=a.toolkit.resolve();a.target_dir=a.target_dir.resolve();a.context_dir=(a.context_dir or a.repo).resolve()
base=Path(__file__).resolve().parent;os.environ['CONVEYOR_TOOLKIT']=str(a.toolkit);os.environ['LD_LIBRARY_PATH']=str(a.toolkit/'lib')
sys.path[:0]=[str(a.repo/'tools/conveyor/jobs'),str(a.repo/'tools/cloud')]
import scoring,score
FLAGS='-g0 -O2 -mips2 -G 0 -non_shared -Xcpluscomm -Wab,-r4300_mul'
rows=[]
with tempfile.TemporaryDirectory(prefix='static-C22-proof-') as scratch:
 work=Path(scratch)
 for name in ['sinf','cosf','full_module']:
  src=base/(name+'.c');obj=work/(name+'.o');command=[str(a.toolkit/'ido/cc'),'-c',*shlex.split(FLAGS),'-I'+str(a.context_dir/'include'),'-I'+str(a.context_dir/'src/rom'),str(src),'-o',str(obj)]
  proc=subprocess.run(command,capture_output=True,text=True)
  if proc.returncode:raise SystemExit(proc.stderr)
  words=score.text_words(obj);symbols=score.symbols(obj)
  for fn in (['sinf','cosf'] if name=='full_module' else [name]):
   target=a.target_dir/(fn+'.target.o');expected=score.text_words(target);start=symbols[fn];actual=words[start//4:start//4+len(expected)]
   scorer=scoring.Scorer(target_o=str(target),stack_differences=True,algorithm='difflib',debug_mode=False,ign_branch_targets=True,objdump_command=scoring.objdump_command()+' --disassemble='+fn)
   strict,_=scorer.score(str(obj));raw=sum(x!=y for x,y in zip(actual,expected))+abs(len(actual)-len(expected))
   row={'source':name+'.c','function':fn,'flags':FLAGS,'strict_score':strict,'raw_word_diff':raw,'compared_words_including_alignment':len(expected),'source_sha256':hashlib.sha256(src.read_bytes()).hexdigest(),'object_sha256':hashlib.sha256(obj.read_bytes()).hexdigest(),'target_sha256':hashlib.sha256(target.read_bytes()).hexdigest(),'command':command}
   rows.append(row);print(json.dumps(row),flush=True)
   if strict or raw:raise SystemExit('nonzero proof')
a.output.write_text(json.dumps(rows,indent=2)+'\n')
