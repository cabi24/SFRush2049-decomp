import json,hashlib,subprocess,os,sys,re,shutil
from pathlib import Path
base=Path(__file__).resolve().parent;tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5';repo=Path.home()/'agents/B/wt'
os.environ['CONVEYOR_TOOLKIT']=str(tk);sys.path[:0]=[str(repo/'tools/conveyor/jobs'),str(repo/'tools/cloud')]
import scoring,score
flags=['-g0','-O2','-mips2','-G','0','-non_shared'];hashof=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for variant in ('before','after'):
 ctx=base/variant;shutil.copytree(base/'snapshot',ctx,dirs_exist_ok=True)
 if variant=='after':
  p=ctx/'include/PR/os_pfs.h';s=p.read_text();s,n=re.subn(r's32 osPfsReadWriteFile\(OSPfs \*pfs, s32 fileNo, u8 flag, s32 offset,\s*s32 size, u8 \*data\);','s32 osPfsReadWriteFile(OSPfs *,u16,u32,u8 *,u8 *,int,s32 *);',s);assert n==1;p.write_text(s)
  p=ctx/'src/rom/rom_tu.h';p.write_text(p.read_text()+'\n'+(ctx/'minimal.h').read_text())
 # Body sources use shared minimal context via rom_tu.h only.
 for p in (ctx/'candidates').glob('*.c'):p.write_text(p.read_text().replace('#include "context.h"\n',''))
results=[]
def compile_source(ctx,src,obj):
 p=subprocess.run([str(tk/'ido/cc'),'-c',*flags,'-I'+str(ctx/'include'),'-I'+str(ctx/'include/PR'),'-I'+str(ctx/'src/rom'),'-D_LANGUAGE_C',str(src),'-o',str(obj)],capture_output=True,text=True,timeout=120)
 return dict(exit_code=p.returncode,stderr=p.stderr,source_sha256=hashof(src))
for src in sorted((base/'after/candidates').glob('*.c')):
 fn=src.stem
 if fn=='context':continue
 r=dict(function=fn,flags=' '.join(flags));ctx=base/'after';obj=base/(fn+'_after.o');r.update(compile_source(ctx,src,obj));target=ctx/'targets'/(fn+'.target.o')
 if not r['exit_code']:
  t,c=score.text_words(target),score.text_words(obj);r.update(strict_score=scoring.score(target,obj,stack_differences=True),raw_word_diff=sum(x!=y for x,y in zip(t,c))+abs(len(t)-len(c)),target_sha256=hashof(target),target_words=len(t),candidate_words=len(c))
 if fn not in ('osPfsReadWriteFile','osPfsGetFileSize','osPfsChecker','__osRepairId','osPiSetDeviceTiming'):
  before=base/'before';bs=before/'candidates'/src.name;bo=base/(fn+'_before.o');br=compile_source(before,bs,bo)
  if not br['exit_code']:
   bw,aw=score.text_words(bo),score.text_words(obj);br['before_after_raw_word_diff']=sum(x!=y for x,y in zip(bw,aw))+abs(len(bw)-len(aw));br['strict_score']=scoring.score(target,bo,stack_differences=True)
  r['before']=br
 results.append(r);print(json.dumps(r),flush=True)
# Every current ROM TU with promoted C: compile untouched bodies before/after, no assembly pragmas.
tr=[]
for file in sorted((base/'snapshot/src/rom').glob('*.c')):
 if 'PROMOTED' not in file.read_text():continue
 row={'file':file.name,'flags':' '.join(flags)};objects=[]
 for variant in ('before','after'):
  ctx=base/variant;src=ctx/'src/rom'/file.name;src.write_text(re.sub(r'^#pragma GLOBAL_ASM.*\n','',src.read_text(),flags=re.M));obj=base/(file.stem+'_'+variant+'.o');r=compile_source(ctx,src,obj);row[variant]=r;objects.append(obj)
 if all(row[v]['exit_code']==0 for v in ('before','after')):
  b,a=map(score.text_words,objects);row['before_after_raw_word_diff']=sum(x!=y for x,y in zip(b,a))+abs(len(b)-len(a))
 tr.append(row)
(base/'proof.json').write_text(json.dumps({'body_results':results,'whole_tu_before_after':tr},indent=2)+'\n');print('whole TUs',len(tr),'failed',sum(any(r[v]['exit_code'] for v in ('before','after')) for r in tr),'changed',sum(r.get('before_after_raw_word_diff',0)!=0 for r in tr),flush=True)
