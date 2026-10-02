from pathlib import Path
import os,sys,json,subprocess,shlex,hashlib
p=Path(__file__).resolve().parent
wt=Path.home()/'agents/C/wt'
tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5'
os.environ['CONVEYOR_TOOLKIT']=str(tk);os.environ['LD_LIBRARY_PATH']=str(tk/'lib')
sys.path[:0]=[str(wt/'tools/conveyor/jobs'),str(wt/'tools/cloud')]
import scoring,score
rows=[]
def run(cmd,cwd):
 proc=subprocess.run(cmd,cwd=cwd,capture_output=True,text=True)
 if proc.returncode:raise RuntimeError(Path(cmd[0]).name+': '+proc.stderr[-2000:])
def verify(obj,kind,commands):
 words=score.text_words(obj);symbols=score.symbols(obj)
 for item in json.loads((p/'targets.json').read_text()):
  fn=item['function'];count=item['words'];target=p/(fn+'.target.o')
  if fn not in symbols:
   row=dict(variant=kind,function=fn,missing=True)
  else:
   start=symbols[fn];following=[v for v in symbols.values() if v>start];end=min(following) if following else len(words)*4
   actual=words[start//4:end//4];expected=score.text_words(target)
   scorer=scoring.Scorer(target_o=str(target),stack_differences=True,algorithm='difflib',debug_mode=False,ign_branch_targets=True,objdump_command=scoring.objdump_command()+' --disassemble='+fn)
   value,_=scorer.score(str(obj))
   row=dict(variant=kind,function=fn,strict_score=value,raw_word_diff=sum(a!=b for a,b in zip(actual,expected))+abs(len(actual)-len(expected)),candidate_words=len(actual),target_words=len(expected),source_sha256=hashlib.sha256((p/'module.c').read_bytes()).hexdigest(),object_sha256=hashlib.sha256(obj.read_bytes()).hexdigest(),commands=commands)
  print(json.dumps(row),flush=True);rows.append(row)
for opt in ['O2','O3']:
 flags=['-g0','-'+opt,'-mips2','-G','0','-non_shared','-Xcpluscomm','-I'+str(p/'include'),'-I'+str(p)]
 obj=p/(opt+'.ordinary.o');command=[str(tk/'ido/cc'),*flags,'-c',str(p/'module.c'),'-o',str(obj)];run(command,p);verify(obj,opt+'_ordinary',[command])
 for keephelper in [True,False]:
  work=p/(opt+('_keep_helper' if keephelper else '_private_helper'));work.mkdir(exist_ok=True)
  (work/'module.c').write_text((p/'module.c').read_text());(work/'keep.txt').write_text('lzss_decode\ninflate_flush_window\nhuft_alloc\n'+('inflate_io_wait\n' if keephelper else ''))
  common=['-mips2','-EB','-g0','-'+opt];ido=lambda n:str(tk/'ido'/n)
  commands=[[ido('cc'),'-j',*flags,'module.c'],[ido('uld'),'-L/usr/lib/mips2/nonshared','-_SYSTYPE_SVR4','-mips2','-non_shared','-g0','-no_AutoGnum','-kp','keep.txt','module.u','-ko','linked'],[ido('usplit'),'-mips2','-o','split','-t','st','linked'],[ido('umerge'),'-Olimit','5000',*common,'split','-o','merged','-t','st'],[ido('uopt'),'-G','0','-Olimit','5000',*common,'merged','opt','-t','st','optlog'],[ido('ugen'),'-G','0',*common,'opt','-o','gen','-t','st','-temp','ugtmp'],[ido('as1'),'-elf','-G','0','-p0',*common,'-Olimit','5000','gen','-o',str(work/'module.o'),'-t','st']]
  try:
   for command in commands:run(command,work)
   verify(work/'module.o',work.name,commands)
  except Exception as e:
   row=dict(variant=work.name,error=str(e));print(json.dumps(row),flush=True);rows.append(row)
(p/'results.json').write_text(json.dumps(rows,indent=2)+'\n')
