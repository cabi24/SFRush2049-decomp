#!/usr/bin/env python3
"""Fresh private builder compile and full-word static original-address replay.
No protected target metadata, words, layout, locks, source TUs or masks changed.
"""
import argparse,hashlib,json,sys,tempfile,subprocess
from pathlib import Path
ap=argparse.ArgumentParser();ap.add_argument('--repo',type=Path,required=True);ap.add_argument('--target-dir',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);ap.add_argument('--builder',default='Rocky');a=ap.parse_args()
sys.path[:0]=[str(a.repo),str(a.repo/'tools/cloud'),str(a.repo/'tools/conveyor/jobs')]
from tools.conveyor.pipeline.diagnose import compile_batch
import scoring,score
p=Path(__file__).resolve().parent;metadata=json.loads((p/'verification.json').read_text());out=[];jobs=[]
for n,m in metadata['functions'].items():
 src=p/(n+'.c');assert hashlib.sha256(src.read_bytes()).hexdigest()==m['source_sha256'];assert src.read_text().splitlines()[0]=='/* flags: '+m['flags']+' */';t=a.target_dir/(n+'.target.o');assert hashlib.sha256(t.read_bytes()).hexdigest()==m['target_sha256'];jobs.append({'function':n,'source':src.read_text(),'flags':m['flags']})
with tempfile.TemporaryDirectory(prefix='C49-verify-') as temp:
 work=Path(temp);objects,failures=compile_batch(jobs,work,builder=a.builder);assert not failures,failures
 for n,m in metadata['functions'].items():
  ld=work/(n+'.ld');ld.write_text('SECTIONS { .text 0x%08x : { *(.text) *(.text.*) } /DISCARD/ : { *(.reginfo) *(.MIPS.abiflags) } } '%m['address']+' '.join(k+' = '+v+';' for k,v in metadata['bindings'].items()))
  linked=work/(n+'.linked.o');cp=subprocess.run(['mips-linux-gnu-ld','-T',str(ld),str(objects[n]),'-o',str(linked)],capture_output=True,text=True);assert cp.returncode==0,cp.stderr
  target=a.target_dir/(n+'.target.o');anchored=work/(n+'.target.vma.o');cp=subprocess.run(['mips-linux-gnu-objcopy','--change-section-vma','.text=0x%08x'%m['address'],str(target),str(anchored)],capture_output=True,text=True);assert cp.returncode==0,cp.stderr
  wanted=score.text_words(target);got=score.text_words(linked);assert wanted==score.text_words(anchored),'VMA anchoring changed original words';assert len(wanted)*4==m['size'];r={'function':n,'flags':m['flags'],'source_sha256':m['source_sha256'],'target_sha256':m['target_sha256'],'raw_word_count':len(wanted),'full_unmasked_word_differences':sum(x!=y for x,y in zip(wanted,got))+abs(len(wanted)-len(got)),'private_vma_only_strict_score':scoring.score(anchored,linked,stack_differences=True),'original_target_text_unchanged':True};out.append(r);assert r['full_unmasked_word_differences']==0 and r['private_vma_only_strict_score']==0,r
 a.output.write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out))
