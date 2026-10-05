"""Unchanged canonical scorer, scoped only to a real named function."""
from pathlib import Path
import os,sys,json,hashlib
base=Path(__file__).resolve().parent
tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5'
repo=Path.home()/'agents/C/wt';os.environ['CONVEYOR_TOOLKIT']=str(tk)
os.environ['LD_LIBRARY_PATH']=str(tk/'lib')
sys.path[:0]=[str(repo/'tools/conveyor/jobs'),str(repo/'tools/cloud')]
import scoring,score
module=base/'full_module.o';words=score.text_words(module);syms=score.symbols(module);rows=[]
for item in json.loads((base/'linked_targets.json').read_text()):
 fn=item['function'];count=item['words'];target=base/(fn+'.target.o');start=syms[fn]
 part=words[start//4:start//4+count];want=score.text_words(target)[:count]
 scorer=scoring.Scorer(target_o=str(target),stack_differences=True,algorithm='difflib',debug_mode=False,ign_branch_targets=True,objdump_command=scoring.objdump_command()+' --disassemble='+fn)
 value,_=scorer.score(str(module))
 row=dict(function=fn,strict_score=value,raw_word_diff=sum(a!=b for a,b in zip(part,want))+abs(len(part)-len(want)),object_start=start,words=count,target_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),module_sha256=hashlib.sha256(module.read_bytes()).hexdigest(),flags='-g0 -O1 -mips2 -G 0 -non_shared -Xcpluscomm',protocol='unchanged canonical scorer settings, function-scoped objdump; raw unlinked object slices, no masks or linked substitutions',source_sha256=hashlib.sha256((base/'full_module.c').read_bytes()).hexdigest())
 rows.append(row);print(json.dumps(row))
(base/'strict_verification.json').write_text(json.dumps(rows,indent=2)+'\n')
