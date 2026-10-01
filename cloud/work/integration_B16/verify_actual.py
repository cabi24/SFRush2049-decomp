exec((__import__('pathlib').Path(__file__).resolve().parent/'verify.py').read_text().split("for variant in ('before','after'):")[0])
ctx=base/'actual';rows=[]
for src in sorted((ctx/'candidates').glob('*.c')):
 fn=src.stem;obj=base/(fn+'_actual.o');target=ctx/'targets'/(fn+'.target.o');p=subprocess.run([str(tk/'ido/cc'),'-c',*flags,'-I'+str(ctx/'include'),'-I'+str(ctx/'include/PR'),'-I'+str(ctx/'src/rom'),'-D_LANGUAGE_C',str(src),'-o',str(obj)],capture_output=True,text=True,timeout=120);r=dict(function=fn,flags=' '.join(flags),exit_code=p.returncode,stderr=p.stderr,source_sha256=hashof(src),target_sha256=hashof(target))
 if not p.returncode:
  t,c=score.text_words(target),score.text_words(obj);r.update(strict_score=scoring.score(target,obj,stack_differences=True),raw_word_diff=sum(x!=y for x,y in zip(t,c))+abs(len(t)-len(c)))
 rows.append(r)
tr=[]
for f in sorted((ctx/'src/rom').glob('*.c')):
 if 'PROMOTED' not in f.read_text():continue
 f.write_text(re.sub(r'^#pragma GLOBAL_ASM.*\n','',f.read_text(),flags=re.M));obj=base/(f.stem+'_actual.o');p=subprocess.run([str(tk/'ido/cc'),'-c',*flags,'-I'+str(ctx/'include'),'-I'+str(ctx/'include/PR'),'-I'+str(ctx/'src/rom'),'-D_LANGUAGE_C',str(f),'-o',str(obj)],capture_output=True,text=True,timeout=120);r=dict(file=f.name,exit_code=p.returncode,stderr=p.stderr,source_sha256=hashof(f))
 if not p.returncode:
  b,a=map(score.text_words,[base/(f.stem+'_before.o'),obj]);r['before_actual_raw_word_diff']=sum(x!=y for x,y in zip(b,a))+abs(len(b)-len(a))
 tr.append(r)
(base/'actual_proof.json').write_text(json.dumps(dict(body_results=rows,whole_tu_before_actual=tr),indent=2)+'\n');print('body failures',[(r['function'],r.get('strict_score'),r.get('raw_word_diff'),r['stderr']) for r in rows if r['exit_code'] or r.get('strict_score') or r.get('raw_word_diff')]);print('TU failures',[(r['file'],r.get('before_actual_raw_word_diff'),r['stderr']) for r in tr if r['exit_code'] or r.get('before_actual_raw_word_diff')]);print('count',len(rows),len(tr))
