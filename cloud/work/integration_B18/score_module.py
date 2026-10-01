from pathlib import Path
import os,sys,json,hashlib
base=Path(__file__).resolve().parent;tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5';repo=Path.home()/'agents/B/wt';os.environ['CONVEYOR_TOOLKIT']=str(tk);sys.path[:0]=[str(repo/'tools/conveyor/jobs'),str(repo/'tools/cloud')]
import scoring,score
module=base/'lib_7630.o';words=score.text_words(module);syms=score.symbols(module);rows=[]
for fn,count in [('osSetEventMesg',96),('vi_manager_main',100)]:
 target=base/(fn+'.target.o');start=syms[fn];part=words[start//4:start//4+count];want=score.text_words(target)
 # Exact canonical scorer settings, restricted to same function in both objects.
 scorer=scoring.Scorer(target_o=str(target),stack_differences=True,algorithm='difflib',debug_mode=False,ign_branch_targets=True,objdump_command=scoring.objdump_command()+' --disassemble='+fn)
 value,_=scorer.score(str(module));row=dict(function=fn,strict_score=value,raw_word_diff=sum(a!=b for a,b in zip(part,want))+abs(len(part)-len(want)),object_start=start,words=count,target_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),module_sha256=hashlib.sha256(module.read_bytes()).hexdigest(),flags='-g0 -O2 -mips2 -G 0 -non_shared',protocol='canonical scorer weights/settings, function-scoped objdump; raw original object word slice, not linked or relocated',source_sha256=hashlib.sha256((base/'lib_7630.c').read_bytes()).hexdigest());rows.append(row)
 print(json.dumps(row))
(base/'module_strict.json').write_text(json.dumps(rows,indent=2))
