import sys,re,json,subprocess
from pathlib import Path
sys.path.insert(0,'tools/cloud');import score
p=Path.home()/'agents/A/scratch/codex_A9'; pins=set(re.findall(r'lib_[a-f0-9]+(?=\.o)',(p/'rom/opt_overrides.mk').read_text()));out=[]
for src in sorted((p/'rom').glob('*.c')):
 text=src.read_text()
 if 'PROMOTED' not in text:continue
 flags=['-g0','-G','0','-mips2','-O1' if src.stem in pins else '-O2','-non_shared','-Wab,-r4300_mul','-Xcpluscomm','-D_LANGUAGE_C'];row={'tu':src.name,'optimization':flags[4]};objs=[]
 for variant in ['baseline_headers','headers']:
  h=p/variant;obj=p/'proof'/(variant+'_'+src.stem+'.o');q=subprocess.run([score.ido('cc'),'-c',*flags,'-I'+str(h),'-I'+str(h/'include'),'-I'+str(h/'include/PR'),str(src),'-o',str(obj)],capture_output=True,text=True)
  if q.returncode:row[variant+'_error']=q.stderr;break
  objs.append(obj)
 if len(objs)==2:
  a,b=map(score.text_words,objs);row.update(text_words=len(a),raw_word_diff=sum(x!=y for x,y in zip(a,b))+abs(len(a)-len(b)))
 out.append(row)
 if 'raw_word_diff' not in row or row['raw_word_diff']:print(row,flush=True)
(p/'header_regression.json').write_text(json.dumps(out,indent=2)+'\n');print('Compiled promoted TUs',len(out),'raw-equal',sum(x.get('raw_word_diff')==0 for x in out),'failed',sum('raw_word_diff' not in x for x in out))
