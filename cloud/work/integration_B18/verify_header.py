from pathlib import Path
import json,subprocess,os,sys,hashlib,shutil,re
base=Path(__file__).resolve().parent;tk=Path.home()/'rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5';repo=Path.home()/'agents/B/wt';os.environ['CONVEYOR_TOOLKIT']=str(tk);sys.path[:0]=[str(repo/'tools/conveyor/jobs'),str(repo/'tools/cloud')]
import score
flags=['-g0','-O2','-mips2','-G','0','-non_shared'];hashof=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
for v in ('before','after'):
 ctx=base/v;shutil.copytree(base/'snapshot',ctx,dirs_exist_ok=True)
 if v=='after':shutil.copy(base/'os_thread_proposed.h',ctx/'include/PR/os_thread.h')
results=[]
def compile(src,obj,ctx):
 p=subprocess.run([str(tk/'ido/cc'),'-c',*flags,'-I'+str(ctx/'include'),'-I'+str(ctx/'include/PR'),'-I'+str(ctx/'src/rom'),'-D_LANGUAGE_C',str(src),'-o',str(obj)],capture_output=True,text=True,timeout=120);return dict(exit_code=p.returncode,stderr=p.stderr,source_sha256=hashof(src))
for file in sorted((base/'snapshot/src/rom').glob('*.c')):
 if 'PROMOTED' not in file.read_text():continue
 row={'file':file.name,'flags':' '.join(flags)};objs=[]
 for v in ('before','after'):
  ctx=base/v;src=ctx/'src/rom'/file.name;src.write_text(re.sub(r'^#pragma GLOBAL_ASM.*\n','',src.read_text(),flags=re.M));obj=base/(file.stem+'_'+v+'.o');row[v]=compile(src,obj,ctx);objs.append(obj)
 if all(row[v]['exit_code']==0 for v in ('before','after')):
  b,a=map(score.text_words,objs);row['raw_text_diff']=sum(x!=y for x,y in zip(b,a))+abs(len(b)-len(a))
 results.append(row)
r=compile(base/'lib_7630.c',base/'lib_7630.o',base/'after');print('module',r);print('TUs',len(results),'failed',[(r['file'],r['before']['stderr'],r['after']['stderr']) for r in results if r['before']['exit_code'] or r['after']['exit_code']],'changed',[(r['file'],r.get('raw_text_diff')) for r in results if r.get('raw_text_diff')]);(base/'proof.json').write_text(json.dumps(dict(header_audit=results,vi_module=r),indent=2))
